//
// Created by artem on 15.10.2020.
//


#include "ship_header.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define THISYEAR 2020


info *init_info() {
    info *init_ptr;
    init_ptr = (info *) malloc(sizeof(info));

    if (init_ptr == NULL)
        exit - 1;

    return init_ptr;
}

info *Uread_info() {
    info *info_read_ptr = init_info();

    printf("введите название корабля: ");
    info_read_ptr->ship_name = read_str(stdin);

    printf("введите название верфи: ");
    info_read_ptr->shipyard_name = read_str(stdin);

    printf("введите год спуска на воду: ");
    info_read_ptr->first_sailing_year = read_int(stdin);
    if ((info_read_ptr->first_sailing_year < 0) || (info_read_ptr->first_sailing_year > THISYEAR))
        exit(3);

    printf("введите историю участия в военных компаниях: ");
    info_read_ptr->battle_history = read_str(stdin);

    printf("введите численность экипажа: ");
    info_read_ptr->crew = read_int(stdin);
    if (info_read_ptr->crew <= 0)
        exit(3);

    printf("введите состояние корабля: ");
    info_read_ptr->status = read_str(stdin);

    fgetc(stdin); //игнорирование \n в конце ввода

    return info_read_ptr;
}

info *Fread_info(FILE *fptr) {
    info *ptr = init_info();

    ptr->ship_name = read_str(fptr);

    ptr->shipyard_name = read_str(fptr);

    ptr->first_sailing_year = read_int(fptr);
    if ((ptr->first_sailing_year < 0) || (ptr->first_sailing_year > THISYEAR))
        exit(3);

    ptr->battle_history = read_str(fptr);

    ptr->crew = read_int(fptr);
    if (ptr->first_sailing_year <= 0)
        exit(3);

    ptr->status = read_str(fptr);

    fgetc(fptr); //игнорирование \n в конце ввода
    return ptr;
}

int Uwrite_info(info *ship) {
    printf("название корабля: %s\n", ship->ship_name);
    printf("название верфи: %s\n", ship->shipyard_name);
    printf("год спуска на воду: %d\n", ship->first_sailing_year);
    printf("история участия в военных компаниях: %s\n", ship->battle_history);
    printf("численность экипажа: %d\n", ship->crew);
    printf("состояние корабля: %s\n", ship->status);
    putchar('\n');
    return 0;
}

int Fwrite_info(info *ship, FILE *fp) {
    if (fprintf(fp, "%s\n", ship->ship_name) < 0)
        exit(2);

    if (fprintf(fp, "%s\n", ship->shipyard_name) < 0)
        exit(2);

    if (fprintf(fp, "%d\n", ship->first_sailing_year) < 0)
        exit(2);

    if (fprintf(fp, "%s\n", ship->battle_history) < 0)
        exit(2);

    if (fprintf(fp, "%d\n", ship->crew) < 0)
        exit(2);

    if (fprintf(fp, "%s\n", ship->status) < 0)
        exit(2);
    return 0;
}

int find(int id, char *patt, info *ship) {
    switch (id) {
        case (1):
            if (!strcmp(ship->ship_name, patt)) return 1;
            break;
        case (2):
            if (!strcmp(ship->shipyard_name, patt)) return 1;
            break;
        case (3): {
            int temp;
            if (sscanf(patt, "%d", &temp) != 1) exit(1);
            if (temp == ship->first_sailing_year) return 1;
            break;
        }
        case (4):
            if (!strcmp(ship->battle_history, patt)) return 1;
            break;
        case (5): {
            int temp;
            if (sscanf(patt, "%d", &temp) != 1) exit(1);
            if (temp == ship->crew) return 1;
            break;
        }
        case (6):
            if (!strcmp(ship->status, patt)) return 1;
            break;
    }
    return 0;
}

void delete_info(info *ship)
{
    free(ship->ship_name);
    free(ship->shipyard_name);
    free(ship->battle_history);
    free(ship->status);
    free(ship);
}


/*helper functions*/
char *read_str(FILE *fptr) {
    char buf[4000], *str;

    while (fscanf(fptr, "%s", buf) != 1) {
        printf("ошибка, попробуйте снова");
        while (getchar() != '\n');
    }

    str = (char *) malloc((strlen(buf) + 1) * sizeof(char));
    strcpy(str, buf);
    return str;
}

int endin(FILE *fptr) {
    char c;
    if ((c = fgetc(fptr)) == EOF)
        return 1;
    ungetc(c, fptr);
    return 0;
}

int read_int(FILE *fptr) {
    int i_temp;
    while (fscanf(fptr, "%d", &i_temp) != 1) {
        printf("ошибка, попробуйте снова\n");
        while (getchar() != '\n');
    }
    return i_temp;
}
/*end of helper functios*/