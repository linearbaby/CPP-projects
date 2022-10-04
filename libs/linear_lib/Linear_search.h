//
// Created by artem on 21.10.2020.
//

#ifndef IZ2_LINEAR_SEARCH_H
#define IZ2_LINEAR_SEARCH_H

#include "../../src/system/sys_info.h"
#include "../../src/vector/vector.h"

double vec_cmp_fast(vector *, vector *, unsigned);
vector *LoadPattern(FILE *);
unsigned Load_vectors(vector **, unsigned, FILE *, unsigned, unsigned);
void free_arr_vec(vector **, unsigned);
vector * Find_nearest_local(vector **, vector *, unsigned, unsigned);
vector * Find_vector(vector *, char *);

#endif //IZ2_LINEAR_SEARCH_H
