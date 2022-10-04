//
// Created by artem on 15.10.2020.
//

#ifndef IZ1_RELEASE_SHIP_HEADER_H
#define IZ1_RELEASE_SHIP_HEADER_H


#include <stdio.h>
#include <stdlib.h>

typedef struct ship_info{
    char *ship_name;
    char *shipyard_name;
    int first_sailing_year;
    char *battle_history;
    int crew;
    char *status;
} info;


info *init_info();
info* Uread_info();
info *Fread_info(FILE *);
int Uwrite_info(info *);
int Fwrite_info(info *, FILE *);
int find(int, char*, info *);
void delete_info(info *);


//helper functions
char *read_str(FILE*);
int read_int(FILE*);
int endin(FILE *);
//end of helper functions


#endif //IZ1_RELEASE_SHIP_HEADER_H
