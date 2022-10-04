//
// Created by artem on 21.10.2020.
//

#include "Parallel_search.h"
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <wait.h>

#define PARALLEL

#define RAM_SAVER_MULTIPLIER 2

FILE * data_file_setter(const char *data_path, int id_proc, unsigned vecs_read_by_process, unsigned vec_size)
{
    FILE *data;
    if ((data = fopen(data_path, "r")) == NULL)
        exit(1);

    fseek(data, 2*sizeof(unsigned) + id_proc * vecs_read_by_process * vec_size * sizeof(double), SEEK_SET);
    return data;
}

double vec_cmp_fast(vector * vec1, vector *vec2, unsigned size_vec) // РАСШИРЕНИЕ ФУНКЦИИ ИЗ vector.h ДЛЯ БЫСТРОГО СРАВНЕНИЯ БЕЗ ИСПОЛЬЗОВАНИЯ if
{
    double diff = 0;

    do {
        size_vec--;

        // оказалось дольше чем мат функция фабса
        //diff += (GetValue(vec1, size_vec) - GetValue(vec2, size_vec)) * (GetValue(vec1, size_vec) - GetValue(vec2, size_vec));
        // оказалось дольше чем мат функция фабса

        diff += fabs(GetValue(vec1, size_vec) - GetValue(vec2, size_vec));
    }while (size_vec != 0);

    return diff;
}

vector *LoadPattern(FILE *fpatt)
{
	int patt_size;
    fread(&patt_size, sizeof(int), 1, fpatt);
	vector *patt = LoadVector(patt_size, fpatt);
	return patt;
}

unsigned Load_vectors(vector **buf_vec, const unsigned max_bufVEC_elements, FILE *data, const unsigned vec_size, unsigned left_in_file)
{
    unsigned for_read = (left_in_file > max_bufVEC_elements) ? max_bufVEC_elements : left_in_file;


    for (int i = 0; i < for_read; i++){
        buf_vec[i] = LoadVector(vec_size, data);

    }

    return for_read;
}

void free_arr_vec(vector **arr_vec, unsigned size_arr)
{
    for (int i = 0; i < size_arr; i++)
        FreeVec(arr_vec[i]);
}

vector *Find_nearest_local(vector **arr_vec, vector *patt, const unsigned vec_size, const unsigned arr_size)
{
    vector *nearest;
    double min_diff, diff;

    min_diff = vec_cmp_fast(arr_vec[0], patt, vec_size);
    nearest = arr_vec[0];

    for (int i = 1; i < arr_size; i++)
    {
        diff = vec_cmp_fast(arr_vec[i], patt, vec_size);
        if (diff < min_diff)
        {
            min_diff = diff;
            nearest = arr_vec[i];
        }
    }

    return nearest;
}

vector * Find_vector_PROCESS(int id_proc, vector *patt, const char *data_path, unsigned vecs_read_by_process, unsigned vec_size, unsigned max_bufVEC_elements)
{
    FILE *data = data_file_setter(data_path, id_proc, vecs_read_by_process, vec_size);

    unsigned readed_vecs;
    vector *find_for_return = NULL, *find_vec;

    vector **buf_vec;
    if ((buf_vec = (vector **) calloc((max_bufVEC_elements > vecs_read_by_process) ? vecs_read_by_process : max_bufVEC_elements, sizeof(vector *))) == NULL)
        exit(1); // memory lost


    do{
        readed_vecs = Load_vectors(buf_vec, max_bufVEC_elements, data, vec_size, vecs_read_by_process);
        vecs_read_by_process -= readed_vecs;
        find_vec = CopyVec(Find_nearest_local(buf_vec, patt, vec_size, readed_vecs));
        //PrintVec(find_vec);
        //printf("\n%lf\n", vec_cmp_fast(patt, find_vec, vec_size));

        free_arr_vec(buf_vec, readed_vecs);

        find_for_return = (find_for_return == NULL) ? find_vec : find_for_return;
        if (vec_cmp_fast(patt, find_vec, vec_size) < vec_cmp_fast(patt, find_for_return, vec_size))
        {
            FreeVec(find_for_return);
            find_for_return = find_vec;
        }
    } while (vecs_read_by_process > 0);

    free(buf_vec);
    fclose(data);

    return find_for_return;
}

int setting_semaphore() {
    key_t key;
    int semid;
    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short  *array;
    } arg;

    if ((key = ftok("main.c", 'J')) == -1) {
        perror("ftok");
        exit(1);
    }

    /* create a semaphore set with 1 semaphore: */
    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) == -1) {
        perror("semget");
        exit(1);
    }

    /* initialize semaphore #0 to 1: */
    arg.val = 1;
    if (semctl(semid, 0, SETVAL, arg) == -1) {
        perror("semctl");
        exit(1);
    }

    return semid;
}

int write_vector(FILE *finders_file, vector *finder)
{
    key_t key;
    int semid;
    if ((key = ftok("main.c", 'J')) == -1) {
        perror("ftok");
        exit(1);
    }
    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) == -1) {
        perror("semget");
        exit(1);
    }


    struct sembuf al = {0 , -1, 0} , fr = {0 , 1, 0};
    if (semop(semid, &al, 1) == -1) {
        perror("semop");
        exit(1);
    }
    fseek(finders_file, 0, SEEK_END);
    fwrite(finder->values, sizeof(double), finder->now, finders_file);

    if (semop(semid, &fr, 1) == -1) {
        perror("semop");
        exit(1);
    }

    FreeVec(finder);
    fclose(finders_file);

    return 0;
}

vector * Find_vector(vector *patt, const char *data_path)
{
    const char *finder_file_path = "tmp/finder_file";
    unsigned proc_cores = Get_Processor_Cores();
    pid_t pid[proc_cores];
    unsigned vec_size, max_bufVEC_elements, vecs_left_in_file, vecs_read_by_process;
    FILE *data;

    //GETTING META FROM DATAFILE
    data = fopen(data_path, "r");
    fread(&vecs_left_in_file, sizeof(unsigned), 1, data);
    fread(&vec_size, sizeof(unsigned), 1, data);
    fclose(data);
    //GETTING META FROM DATAFILE

    vecs_read_by_process = vecs_left_in_file / proc_cores + vecs_left_in_file % proc_cores;
    max_bufVEC_elements = Get_RAM() / (proc_cores * RAM_SAVER_MULTIPLIER * (sizeof(vector) + vec_size * sizeof(double)));

    int semid = setting_semaphore();
    FILE *finders_file;

    //CREATE TMP FILE
    finders_file = fopen(finder_file_path, "wb");
    fclose(finders_file);
    //CREATE TMP FILE

    for (int i = 0; i < proc_cores; ++i) {
        pid[i] = fork();
        if (-1 == pid[i]) {
            exit(1);
        }
        else if (0 == pid[i]) {
            int id_proc = i;
            vector *finder;

            finder = Find_vector_PROCESS(id_proc, patt, data_path, vecs_read_by_process, vec_size, max_bufVEC_elements);

            finders_file = fopen(finder_file_path, "r+b");
            write_vector(finders_file, finder);

            exit(0);
        }
    }

    int stat;
    for (int i = 0; i < proc_cores; ++i) {
        waitpid(pid[i], &stat, 0);
        if (WIFEXITED(stat) == 0) {
            printf("CHILD ПАЛ ХРАБРОЙ СМЕРТЬЮ НО НЕ ПО СВОЕЙ ВИНЕ!");
            exit(5);
        }
    }

    //REMOOVING SEMAPHORE
    union semun {
        int val;
        struct semid_ds *buf;
        ushort *array;
    } dummy;
    semctl(semid, 0, IPC_RMID, dummy);
    //SEMAPHORE REMOOVED

    //PROCESSES DID THEIR BEST, TIME TO HANDLE...
    finders_file = fopen(finder_file_path, "rb");
    fseek(finders_file, 0, SEEK_SET);
    //SETED PLACE TO READ FROM FILE

    vector *finder = LoadVector(vec_size, finders_file);
    for (int i = 1; i < proc_cores; ++i) {
        vector * readed_vec = LoadVector(vec_size, finders_file);
        if (vec_cmp_fast(readed_vec, patt, vec_size) < vec_cmp_fast(finder, patt, vec_size))
        {
            FreeVec(finder);
            finder = readed_vec;
        }
    }

    remove(finder_file_path);

    return finder;  //ДОЖДАЛИСЬ!
}
