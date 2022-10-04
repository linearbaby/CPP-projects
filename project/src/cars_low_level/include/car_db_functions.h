#pragma once


#include <glob.h>


typedef struct car{
    size_t enginev;
    size_t speed;
    size_t fuel_consum;
    char * body_shape;
    char * model;
} car;


typedef struct car_db{
    car * car_items;
    double ozh_disp[2][3];   //нужно для правильного нахождения ближайшей машины.
    size_t size;
} car_db;


int _calclute_normalization(car_db *db);
int _copy_car(car *, car);
int _add(car_db *, car, size_t);
int _append(car_db *, car);
int _delete(car_db *, size_t);
int _clear(car_db *);
int _init_db(car_db *);


#define MODEL_PROMPT 1
#define BACK_PROMPT 0

#define DELIM " "

#define SIZE_NORMALIZATION 3

