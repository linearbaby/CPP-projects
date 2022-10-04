#include "FileMapper.h"
#include "MyMsg.h"
#include "Errors.h"
#include "ProcessFunctions.h"
#include "CodingFeatures.h"


#include <sys/sysinfo.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>


#define MIN(a, b) ((a) < (b)) ? (a) : (b)


int allfree(size_t num_allocated, struct process_map *pm){
    if (pm == NULL)
        return 1;
    for (size_t i = 0; i < num_allocated; i++){
        if (pm->map_size[i] != 0) {
            munmap(pm->map[i], pm->map_size[i]);
        }
    }
    free(pm->map_size);
    free(pm->map);
    free(pm->pid);
    pm->map = NULL;
    pm->pid = NULL;
    pm->map_size = NULL;
    return 0;
}


int clear_all_processes(){
    kill(0, SIGKILL);
    int st;
    while (waitpid(0, &st, WNOHANG) > 0)
        ;
    return 0;
}


int termination(size_t num_allocated, struct process_map *pm, int msgqid) {
    clear_all_processes();
    allfree(num_allocated, pm);
    msgctl(msgqid, IPC_RMID, NULL);
    return 0;
}



int FindNumSymbols(size_t *num_of_symbols, const char file_path[], const char symbols[], size_t coding, size_t memory_available){
    struct sysinfo si;
    sysinfo(&si);
    size_t page = getpagesize();
    size_t procs = get_nprocs();


    //обработка входных параметров, подстройка под параметры системы.
    if (coding == 0){
        coding = 1; //кодировка по умолчанию (например ASCII)
    }
    if (memory_available == 0)
        memory_available = si.freeram / 2;

    if ((memory_available < coding) || (memory_available < page))
        return ERROR_RAM; // слишком мало памяти для загрузки]

    //TO DO возможное вырванивание по кодировке.

    //открытие файла
    int fd;
    if ((fd = open(file_path, O_RDONLY)) == -1)
        return ERROR_OPEN_FILE; //не удалось открыть файл.
    struct stat st;
    stat(file_path, &st);
    size_t file_len = st.st_size;

    size_t map_one_proc = MIN((memory_available / procs / page) * page, (file_len / procs / page + 1) * page);

    if (map_one_proc == 0){
        map_one_proc = page;
        procs = memory_available / page;
    }

    //убераем повторяющиеся символы из строки шаблона
    size_t symbols_len = strlen(symbols);
    if ((symbols_len % coding != 0) || (page % coding != 0))
        return BAD_CODING;
    char *symbols_m = malloc(sizeof(char) * (symbols_len + 1));
    strcpy(symbols_m, symbols);
    size_t j = 1;
    for (size_t i = 1; i * coding < symbols_len; i++){
        size_t z = 0;
        while ((z < i) && (!CompareWithCoding(symbols + coding * z, symbols + coding * i, coding)))
            z++;
        if (z == i){
            CopyWithCoding(symbols_m + coding * j, symbols + coding * i, coding);
            j++;
        }
    }
    symbols_m[j * coding] = '\0';

    //по полученным данным выполняем поиск.
    *num_of_symbols = 0;
    int result = MapAndSearch(num_of_symbols, fd, symbols_m, file_len, coding, procs, map_one_proc);
    free(symbols_m);
    close(fd);
    return result;
}


size_t DoMmap(size_t *fd_length, size_t map_one_proc, int fd, size_t file_offset, struct process_map *pm, size_t elem){
    if (*fd_length > map_one_proc){
        pm->map[elem] = mmap(NULL, map_one_proc, PROT_READ, MAP_PRIVATE, fd, file_offset * map_one_proc);
        pm->map_size[elem] = map_one_proc;
        *fd_length -= map_one_proc;
    }
    else{
        pm->map[elem] = mmap(NULL, *fd_length, PROT_READ, MAP_PRIVATE, fd, file_offset * map_one_proc);
        pm->map_size[elem] = *fd_length;
        if (pm->map[elem] == MAP_FAILED) {
            return ERROR_MAP;
        }
        return 1;
    }
    if (pm->map[elem] == MAP_FAILED) {
        return ERROR_MAP;
    }
    return 0;
}


int MapAndSearch(size_t *num_of_symbols, const int fd, const char symbols[], size_t fd_length, size_t coding, size_t procs, size_t map_one_proc){
    struct process_map pm;
    pm.pid = (pid_t *) malloc(sizeof(pid_t) * procs);
    pm.map = (char **) malloc(sizeof(char *) * procs);
    pm.map_size = (size_t *) malloc(sizeof(size_t) * procs);
    memset(pm.map_size, 0, sizeof(size_t) * procs);
    memset(pm.pid, 0, sizeof(pid_t) * procs);


    //загрузим возможную часть файла в память
    size_t i = 0;
    int e_o_f = 0;
    while(i < procs && !e_o_f){
        e_o_f = DoMmap(&fd_length, map_one_proc, fd, i, &pm, i);
        if (e_o_f == ERROR_MAP){
            allfree(i, &pm);
            return ERROR_MAP;
        }
        i++;
    }
    size_t file_offset = i;

    //создаем очередь сообщений
    int msgqid;
    if ((msgqid = msgget(IPC_PRIVATE, IPC_CREAT|0660)) == -1){
        allfree(procs, &pm);
        return ERROR_MESSAGE_Q;
    }

    //раздаем начальные работы
    i = 0;
    pid_t pid = 1;
    while ((i < procs) && (pm.map_size[i] != 0) && (pid > 0)) {
        if ((pid = fork()) < 0) {
            termination(procs, &pm, msgqid);
            return ERROR_FORK;
        }
        else if (pid == 0)
            FindSymbolInMap(pm.map[i], pm.map_size[i], symbols, coding, msgqid);
        else{
            pm.pid[i] = pid;
            i++;
        }
    }
    size_t num_processes = i;

    //цикл раздачи новых работ, пока не будет обработан весь файл.
    int status;
    char *end = NULL;
    struct my_msgbuf buf;
    // file_offset ранее полученная переменная, для итерации по файлу, отображающая его смещение.
    while (num_processes != 0){
        pid = wait(&status);
        msgrcv(msgqid, (struct msgbuf *) &buf, MAX_SEND_SIZE, MSG_TYPE, IPC_NOWAIT);
        *num_of_symbols += strtol(buf.mtext, &end, 10);

        i = find_pid(pid, pm.pid, procs);
        pm.pid[i] = 0;
        munmap(pm.map[i], pm.map_size[i]);
        pm.map[i] = NULL;
        pm.map_size[i] = 0;
        if (!e_o_f){
            e_o_f = DoMmap(&fd_length, map_one_proc, fd, file_offset, &pm, i);
            if (e_o_f == ERROR_MAP) {
                termination(procs, &pm, msgqid);
                return ERROR_MAP;
            }
            file_offset++;
            if ((pid = fork()) < 0) {
                termination(procs, &pm, msgqid);
                return ERROR_FORK;
            }
            else if (pid > 0)
                pm.pid[i] = pid;
            else if (pid == 0)
                FindSymbolInMap(pm.map[i], pm.map_size[i], symbols, coding, msgqid);
        }
        else
            num_processes--;
    }

    allfree(0, &pm);
    msgctl(msgqid, IPC_RMID, NULL);
    return 0;
}



size_t find_pid(pid_t pid, const pid_t pid_array[], size_t processes){
    for (size_t i = 0; i < processes; i++){
        if (pid == pid_array[i])
            return i;
    }
    return processes;
}
