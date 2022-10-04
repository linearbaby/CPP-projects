#include "car_db_functions.h"


#include <stdio.h>


int write_data(car_db *db, const char *file_name){
    FILE *fp;

    if((fp=fopen(file_name, "r +")) == NULL) {
        printf("Cannot open file.\n");
        return -1;
    }
    for (int i = 0; i < db->size; i++) {
        fprintf(fp, "%s ", db->car_items[i].model);
        fprintf(fp, "%s ", db->car_items[i].body_shape);
        fprintf(fp, "%zu ", db->car_items[i].enginev);
        fprintf(fp, "%zu ", db->car_items[i].speed);
        fprintf(fp, "%zu \n", db->car_items[i].fuel_consum);
    }

    fclose(fp);
    return 0;
}
