//
// Created by artem on 21.10.2020.
//

#ifndef IZ2_LINEAR_SEARCH_H
#define IZ2_LINEAR_SEARCH_H

#include "../../src/system/sys_info.h"
#include <stdio.h>
#include "../../src/vector/vector.h"

double vec_cmp_fast(vector *, vector *, unsigned);
vector *LoadPattern(FILE *);
unsigned Load_vectors(vector **, unsigned, FILE *, unsigned, unsigned);
int send_vector(int, vector*);
vector *recieve_vector(int, unsigned);
void free_arr_vec(vector **, unsigned);
vector * Find_nearest_local(vector **, vector *, unsigned, unsigned);
vector * Find_vector_PROCESS(int, vector *, const char *, unsigned, unsigned, unsigned);
int setting_semaphore();
int write_vector(FILE *, vector *);
vector * Find_vector(vector *, const char *);

#endif //IZ2_LINEAR_SEARCH_H
