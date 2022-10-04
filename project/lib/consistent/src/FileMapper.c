#include "FileMapper.h"
#include "Search.h"
#include "Errors.h"
#include "CodingFeatures.h"


#include <sys/sysinfo.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>


int FindNumSymbols(size_t *num_of_symbols, const char *file_path, const char symbols[], size_t coding, size_t memory_available){
    struct sysinfo si;
    if (sysinfo(&si) != 0)
        return ERROR_SYS_CALL;
    size_t page = getpagesize();

    //обработка входных параметров, подстройка под параметры системы.
    if (coding == 0){
        coding = 1; //кодировка по умолчанию (например ASCII)
    }
    if (memory_available == 0)
        memory_available = si.freeram / 2;

    if ((memory_available < coding) || (memory_available < page))
        return ERROR_RAM; // слишком мало памяти для загрузки]

    //выравнивание для правильной работы mmap
    memory_available -= memory_available % page;


    //TO DO возможное вырванивание по кодировке.

    //открытие файла
    int fd;
    if ((fd = open(file_path, O_RDONLY)) == -1)
        return ERROR_OPEN_FILE; //не удалось открыть файл.
    struct stat st;
    if (stat(file_path, &st) != 0)
        return ERROR_SYS_CALL;
    size_t file_len = st.st_size;


    //убераем повторяющиеся символы из строки шаблона
    size_t symbols_len = strlen(symbols);
    if ((symbols_len % coding != 0) || (page % coding != 0))
        return BAD_CODING;
    char *symbols_m = malloc(sizeof(char) * (symbols_len + 1));
    strcpy(symbols_m, symbols);
    size_t j = 1;
    for (size_t i = 1; i * coding < symbols_len; ++i){
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
    int result = MmapAndSearch(num_of_symbols, fd, file_len, symbols_m, coding, memory_available);
    close(fd);
    free(symbols_m);
    return result;
}


int MmapAndSearch(size_t *num_of_symbols, int fd, size_t file_len, const char symbols[], size_t coding, size_t memory_available) {
    int e_o_f = 0;
    char *map;
    size_t size_map;
    size_t i = 0;
    while (!e_o_f){
        if (file_len > memory_available){
            map = mmap(NULL, memory_available, PROT_READ, MAP_PRIVATE, fd, i * memory_available);
            size_map = memory_available;
            file_len -= memory_available;
        }
        else{
            map = mmap(NULL, file_len, PROT_READ, MAP_PRIVATE, fd, i * memory_available);
            size_map = file_len;
            e_o_f = 1;
        }
        if (map == MAP_FAILED) {
            return ERROR_MAP;
        }
        ++i;
        FindSymbolInMap(num_of_symbols , map, size_map, symbols, coding);
        munmap(map, size_map);
    }
    return 0;
}
