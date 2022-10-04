#pragma once

#include "car_db_functions.h"


#include <stdio.h>


int init(car_db *);
int _to_lower_string(char *);
int print_car(const car );
int print_all(const car_db );
int validate_car(car );
int add(struct car_db *, const car , const size_t );
int add_prompt(car_db *, FILE *);
int append(car_db *, const car );
int delete_car_from_db(car_db *, const size_t );
int clear(car_db *);

