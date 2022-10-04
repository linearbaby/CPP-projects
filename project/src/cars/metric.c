#include "car_db_functions.h"


#include <string.h>
#include <math.h>


static double _clean_dispersion(const double disp){
    const double eps = 0.5;
    if (disp < eps)
        return 1;
    else
        return disp;
}


static double _calculate_metric(const car c1, const car c2, const double ozh_disp[][3]){

    double metric = fabs((double)c1.enginev - (double)c2.enginev) / _clean_dispersion(ozh_disp[1][0]) +\
                    fabs((double)c1.speed - (double)c2.speed) / _clean_dispersion(ozh_disp[1][1]) +\
                    fabs((double)c1.fuel_consum - (double)c2.fuel_consum) / _clean_dispersion(ozh_disp[1][2]) +\
                    (double)(strcmp(c1.body_shape, c2.body_shape) != 0 ? 1 : 0);
    return metric;
}

size_t find_nearest(const car_db db, const car c){
    if (db.size == 0)
        return -1; //невозможно выполнить поиск в пустом массиве

    double metric_lowest, metric;
    size_t nearest = 0;
    metric_lowest = _calculate_metric(c, db.car_items[0], db.ozh_disp);
    for (int i = 1; i < db.size; i++){
        metric = _calculate_metric(c, db.car_items[i], db.ozh_disp);
        if (metric < metric_lowest) {
            nearest = i;
            metric_lowest = metric;
        }
    }
    return nearest;
}
