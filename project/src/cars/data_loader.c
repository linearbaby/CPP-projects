#include "car_db_functions.h"
#include "UI_functions.h"


#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int read_data(car_db *db, const char *file_name) {
    FILE *fp;
    if ((fp = fopen(file_name, "r")) == NULL) {
        printf("Cannot open file.\n");
        return -1;
    }

    _clear(db);
    car c;
    char *token;
    char *buf = NULL;
    size_t n = 0;
    while (getline(&buf, &n, fp) != -1) {
        size_t flag = 0;
        token = strtok(buf, DELIM);

        c.model = token;
        _to_lower_string(c.model);
        token = strtok(NULL, DELIM);
        c.body_shape = token;
        _to_lower_string(c.model);

        token = strtok(NULL, DELIM);
        flag += (sscanf(token, "%zu", &(c.enginev)) != 1) ? 1 : 0;
        token = strtok(NULL, DELIM);
        flag += (sscanf(token, "%zu", &(c.speed)) != 1) ? 1 : 0;
        token = strtok(NULL, DELIM);
        flag += (sscanf(token, "%zu", &(c.fuel_consum)) != 1) ? 1 : 0;
        if (!validate_car(c) || flag != 0){
            printf("ERROR WHILE READING DATA FILE");
            _clear(db);
            fclose(fp);
            if (buf != NULL)
                free(buf);
            return -1;
        }
        else{
            _append(db, c);
        }
    }

    _calclute_normalization(db);

    fseek(fp, 0, SEEK_SET);
    fclose(fp);
    if (buf != NULL)
        free(buf);
    return 0;
}
