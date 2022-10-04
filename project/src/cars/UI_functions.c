#include "car_db_functions.h"
#include "models_backs.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


/*
в данном файле представлены функции, реализующие пользовательский интерфейс для работы с
данной библиотекой. Они будут использоваться для валидации правильности введенных данных.
 */


int _to_lower_string(char *str){
    for (size_t i = 0; i < strlen(str) + 1; i++)
        str[i] = tolower(str[i]);
    return 0;
}


int init(car_db *db) {
    return _init_db(db);
}


int print_car(const car c) {
    printf("----%s---\n", c.model);
    printf("Мощность двигателя : %zu \n", c.enginev);
    printf("Скорость           : %zu \n", c.speed);
    printf("Потребление топлива: %zu \n", c.fuel_consum);
    printf("Форма кузова       : %s  \n", c.body_shape);
    return 0;
}


int print_all(const car_db bd) {
    for (int i = 0; i < bd.size; i++)
        print_car(bd.car_items[i]);
    return 0;
}


int validate_car(car c) {
    size_t i = 0;
    _to_lower_string(c.body_shape);
    _to_lower_string(c.model);

    while ((i < body_number) && (strcmp(c.body_shape, body_shapes[i]) != 0))
        i++;
    if (i == body_number)
        return 0;

    i = 0;
    while ((i < models_number) && (strcmp(c.model, available_models[i]) != 0))
        i++;
    if (i == models_number)
        return 0;

    return 1;
}


int add(struct car_db *db, const car c, const size_t pos) {
    if (!validate_car(c))
        return 2;   //структура имеет неверный формат
    if (pos > db->size)
        return -1;  //неправильный параметр позиции
    _add(db, c, pos);
    _calclute_normalization(db);
    return 0;
}


static int _prompt_string(char **str, size_t type, FILE *fp) {
    char str_t[11];
    size_t size;
    char **check;
    if (type == MODEL_PROMPT) {
        strcpy(str_t, "model");
        check = available_models;
        size = models_number;
    } else if (type == BACK_PROMPT) {
        strcpy(str_t, "body shape");
        check = body_shapes;
        size = body_number;
    }


    size_t scanned = 0;
    char buffer[200];
    while (scanned != 1) {
        if (fp == stdin)
            printf("type in %s: ", str_t);
        if (fscanf(fp, "%s", buffer) != 1)
            printf("INPUT ERROR, TRY AGAIN\n");
        else {
            _to_lower_string(buffer);
            int i = 0;
            while ((i < size) && (strcmp(buffer, check[i]) != 0))
                i++;
            if (i == size)
                printf("wrong %s, message admin if your lovely %s is not in our pool!\n"
                       "for now insert valid %s.\n", str_t, str_t, str_t);
            else {
                scanned = 1;
                *str = (char *) calloc(strlen(buffer) + 1, sizeof(char));
                strcpy(*str, buffer);
            }
        }
    }
    return scanned;
}


static int _prompt_int(size_t *i, FILE *fp, const char prompt[]) {
    if (fp == stdin)
        printf("%s", prompt);
    while (fscanf(fp, "%zu", i) != 1) {
        printf("ERROR, try again\n");
        while (fgetc(fp) != '\n');
        printf("%s", prompt);
    }

    if (fp == stdin)
        while (fgetc(fp) != '\n');

    return 0;
}


int add_prompt(car_db *db, FILE *fp) {
    car c;
    _prompt_string(&(c.model), MODEL_PROMPT, fp);
    _prompt_string(&(c.body_shape), BACK_PROMPT, fp);

    _prompt_int(&(c.enginev), fp, "enter engine power: ");

    _prompt_int(&(c.speed), fp, "enter car speed: ");

    _prompt_int(&(c.fuel_consum), fp, "enter fuel consumption: ");

    _append(db, c);
    _calclute_normalization(db);
    free(c.body_shape);
    free(c.model);
    return 0;
}

int append(car_db *db, const car c) {
    if (!validate_car(c))
        return 2;   //структура имеет неверный формат
    _append(db, c);
    _calclute_normalization(db);
    return 0;
}


int delete_car_from_db(car_db *db, const size_t pos) {
    if (pos >= db->size)
        return -1;  //неправильный параметр позиции
    _delete(db, pos);
    _calclute_normalization(db);
    return 0;
}


int clear(car_db *db) {
    char c;
    printf("proceed clearing? Y/N : ");
    while ((fscanf(stdin, "%c", &c) != 1) && ((c != 'Y') || (c != 'N') || (c != 'y') || (c != 'n'))) {
        printf("WRONG ANSWER\n");
        while (fgetc(stdin) != '\n');
        printf("proceed clearing? Y/N : ");
    }
    if ((c == 'N') || (c == 'n')) {
        printf("No clearing done.\n");
        return 0;
    } else {
        printf("Making clear.\n");
        _clear(db);
        return 0;
    }
}
