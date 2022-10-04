//
// Created by artem on 21.10.2020.
//

#include "vector.h"
#include <stdlib.h>
#include <math.h>

vector *InitVector()
{
    vector *vec;
    if ((vec = (vector*) malloc(sizeof(vector))) == NULL)
        exit(1); //memory lost

    if ((vec->values = (double *)calloc(100, sizeof(double))) == NULL)
        exit(1); // memory lost

    vec->max = 100;
    vec->now = 0;

    return vec;
}

vector *LoadVector(unsigned size_vec, FILE *data)
{
    vector *vec;
    if ((vec = (vector*) malloc(sizeof(vector))) == NULL)
        exit(1); //memory lost

    if ((vec->values = (double *)calloc(size_vec, sizeof(double))) == NULL)
        exit(1); // memory lost

    vec->max = size_vec;
    vec->now = size_vec;

    if (fread(vec->values, sizeof(double), size_vec, data) != size_vec)
        exit(4); // READ ERRORS

    return vec;
}

int PushBack(vector *vec, double value)
{
    if (vec->now == vec->max)
        {
        vec->max += 50;
        if ((vec->values = (double *) realloc(vec->values, (vec->max) * sizeof(double))) == NULL)
            exit(1); //memory lost
        }
    vec->values[vec->now] = value;
    vec->now++;

    return 0;
}

vector *CopyVec(vector *src)
{
    vector *dest;
    dest = (vector *)malloc(sizeof(vector));
    dest->max = src->max;
    dest->now = src->now;
    dest->values = (double *) calloc(src->max, sizeof(double));
    for (int i = 0; i < src->max; i++)
        dest->values[i] = src->values[i];

    return dest;
}

int FreeVec(vector *vec)
{
    free(vec->values);
    free(vec);
    return 0;
}

double GetValue(vector *vec, unsigned number)
{
    if (number > vec->now)
        exit(2); // array faults
    return vec->values[number];
}

double vec_cmp(vector *vec1, vector *vec2)
{
    double diff = 0;

    if (vec1->now != vec2->now)
        exit(3); // VECTOR ERRORS

    unsigned size_vec = vec1->now;
    do {
        size_vec--;
        diff += fabs(GetValue(vec1, size_vec) - GetValue(vec2, size_vec));
    }while (size_vec != 0);

    return diff;
}

int PrintVec(vector *vec_for_print)
{
    for (int i = 0; i < vec_for_print->now; i++)
        printf("%lf  ", GetValue(vec_for_print, i));

    return 0;
}
