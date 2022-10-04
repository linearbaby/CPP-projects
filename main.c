#include <stdio.h>
#include "mylist.h"
#include "ship_header.h"

#define ship_stat "ship_data2.txt"


int main() {
    FILE *fp = fopen(ship_stat, "r");
    char usage;

    node *list;
    list = load(fp);
    do {
        printf("F - найти в базе корабли соответствующие требованиям\n");
        printf("A - добавить в базу новые составляющие\n");
        printf("D - удалить из базе корабли соответствующие требованиям\n");
        printf("E - закончить работу\n");
        printf("что вы хотите сделать с базой данных? ");
        usage = getchar();
        printf("\n\n\n");

        switch(usage){
            case('F'):{
                find_ships(list);
                break;
            }
            case('D'):{
                list = search_and_delete(list);
                break;
            }
            case('A'):{
                list = add(list);
            }
            case('E'): break;
            default: perror("exit WRONG_ARGS"); exit(5);
        }
    }while (usage != 'E');

    fclose(fp);
    fp = fopen(ship_stat, "w");
    write_list(fp, list);
    fclose(fp);
    return 0;
}
