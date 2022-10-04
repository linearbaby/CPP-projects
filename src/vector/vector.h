//
// Created by artem on 21.10.2020.
//

#ifndef IZ2_VECTOR_H
#define IZ2_VECTOR_H

#include <stdio.h>
typedef struct vector{
    unsigned max;
    unsigned now;
    double *values;
} vector;

vector *LoadVector(unsigned, FILE *);
vector *InitVector();
vector *CopyVec(vector *s);
int PushBack(vector *, double);
int FreeVec(vector *);
double GetValue(vector *, unsigned);
double vec_cmp(vector *, vector *);
int PrintVec(vector *);

#endif //IZ2_VECTOR_H
