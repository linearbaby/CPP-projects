#include "car_db_functions.h"


#include <stdlib.h>
#include <string.h>
#include <math.h>


#define SQR(X) ((X) * (X))


int _copy_car(car * array_car, const car c){
    array_car->fuel_consum = c.fuel_consum;
    array_car->speed = c.speed;
    array_car->enginev = c.enginev;
    array_car->body_shape = malloc(strlen(c.body_shape) + 1);
    strcpy(array_car->body_shape, c.body_shape);
    array_car->model = malloc(strlen(c.model) + 1);
    strcpy(array_car->model, c.model);
    return 0;
}


int _calclute_normalization(car_db *db){
    //начальное обнуление
    memset(db->ozh_disp, 0, sizeof(size_t) * 6);

    //высчитываем математическое ожидание
    for (size_t i = 0; i < db->size; i++){
        db->ozh_disp[0][0] += db->car_items[i].enginev;
        db->ozh_disp[0][1] += db->car_items[i].speed;
        db->ozh_disp[0][2] += db->car_items[i].fuel_consum;
    }
    for (size_t i = 0; i < SIZE_NORMALIZATION; i++)
        db->ozh_disp[0][i] /= (double) (db->size);

    //Высчитываем дисперсию

    for (size_t i = 0; i < db->size; i++) {
        db->ozh_disp[1][0] += SQR((double)db->car_items[i].enginev - (double)db->ozh_disp[0][0]);
        db->ozh_disp[1][1] += SQR((double)db->car_items[i].speed - (double)db->ozh_disp[0][1]);
        db->ozh_disp[1][2] += SQR((double)db->car_items[i].fuel_consum - (double)db->ozh_disp[0][2]);
    }

    for (size_t i = 0; i < SIZE_NORMALIZATION; i++){
        db->ozh_disp[1][i] = db->ozh_disp[1][i] / (double) (db->size);
        db->ozh_disp[1][i] = sqrt(db->ozh_disp[1][i]);
    }

    return 0;
}


int _add(car_db *db, const car c, const size_t pos){
    if (db->size == 0)
        db->car_items = (car *)malloc(sizeof(car));
    else
        db->car_items = (car *)realloc(db->car_items, (db->size + 1) * sizeof(car));
    db->size++;

    if (pos == db->size - 1){
        _copy_car(&(db->car_items[pos]), c);
        return 0;
    }

    memmove(&(db->car_items[pos + 1]), &(db->car_items[pos]), sizeof(car) * (db->size - 1 - pos));
    _copy_car(&(db->car_items[pos]), c);
    return 0;
}


int _init_db(car_db *db) {
    db->size = 0;
    return 0;
}


int _append(car_db *bd, const car c){
    _add(bd, c, bd->size);
    return 0;
}


int _delete(car_db *db, const size_t pos){
    free(db->car_items[pos].body_shape);
    free(db->car_items[pos].model);
    db->car_items[pos].body_shape = NULL;
    db->car_items[pos].model = NULL;
    if (pos != db->size - 1){
        memmove(&db[pos], &db[pos + 1], sizeof(car) * (db->size - 1 - pos));
    }
    db->car_items = realloc(db->car_items, (db->size - 1) * sizeof(car));
    db->size--;
    if (db->size == 0)
        db->car_items = NULL;
    return 0;
}


int _clear(car_db *db){
    if (db->size > 0) {
        for (int i = 0; i < db->size; i++) {
            free(db->car_items[i].body_shape);
            db->car_items[i].body_shape = NULL;
            free(db->car_items[i].model);
            db->car_items[i].model = NULL;
        }
        free(db->car_items);
        db->car_items = NULL;
        db->size = 0;
    }

    return 0;
}
