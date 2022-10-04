//
// Created by artem on 21.10.2020.
//

#include "sys_info.h"
#include <stdio.h>
#include <stdlib.h>

#define CMD_N_CPU "nproc"
#define CMD_FREE_MEMORY "vmstat -s | grep \"free memory\""

unsigned Get_RAM()
{
    unsigned free_ram;
    FILE *pipe_in;

    if ((pipe_in = popen(CMD_FREE_MEMORY, "r")) == NULL) {
        exit(1);
    }
    if (fscanf(pipe_in, "%d", &free_ram) != 1)
        exit(1);
    pclose(pipe_in);

    return free_ram * 1000; // так как результат free в килобайтах
}

unsigned Get_Processor_Cores()
{
    unsigned n_processors;
    FILE *pipe_in;

    if ((pipe_in = popen(CMD_N_CPU, "r")) == NULL) {
        exit(1);
    }
    if (fscanf(pipe_in, "%d", &n_processors) != 1)
        exit(1);
    pclose(pipe_in);

    return n_processors;
}
