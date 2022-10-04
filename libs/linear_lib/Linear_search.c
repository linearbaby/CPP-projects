//
// Created by artem on 21.10.2020.
//

#include "Linear_search.h"
#include <stdlib.h>
#include <math.h>

#define LINEAR

#define RAM_SAVER_MULTIPLIER 2

double vec_cmp_fast(vector * vec1, vector *vec2, unsigned size_vec) // РАСШИРЕНИЕ ФУНКЦИИ ИЗ vector.h ДЛЯ БЫСТРОГО СРАВНЕНИЯ БЕЗ ИСПОЛЬЗОВАНИЯ if
{
    double diff = 0;

    do {
        size_vec--;

        // оказалось дольше чем мат функция фабса
        //diff += (GetValue(vec1, size_vec) - GetValue(vec2, size_vec)) * (GetValue(vec1, size_vec) - GetValue(vec2, size_vec));
        // оказалось дольше чем мат функция фабса

        diff += fabs(GetValue(vec1, size_vec) - GetValue(vec2, size_vec));
    }while (size_vec != 0);

    return diff;
}

vector *LoadPattern(FILE *fpatt)
{
	int patt_size;
    fread(&patt_size, sizeof(int), 1, fpatt);
	vector *patt = LoadVector(patt_size, fpatt);
	return patt;
}

unsigned Load_vectors(vector **buf_vec, const unsigned max_bufVEC_elements, FILE *data, const unsigned vec_size, unsigned left_in_file)
{
    unsigned for_read = (left_in_file > max_bufVEC_elements) ? max_bufVEC_elements : left_in_file;

    for (int i = 0; i < for_read; i++)
        buf_vec[i] = LoadVector(vec_size, data);

    return for_read;
}

void free_arr_vec(vector **arr_vec, unsigned size_arr)
{
    for (int i = 0; i < size_arr; i++)
        FreeVec(arr_vec[i]);
}

vector *Find_nearest_local(vector **arr_vec, vector *patt, const unsigned vec_size, const unsigned arr_size)
{
    vector *nearest;
    double min_diff, diff;

    min_diff = vec_cmp_fast(arr_vec[0], patt, vec_size);
    nearest = arr_vec[0];

    for (int i = 1; i < arr_size; i++)
    {
        diff = vec_cmp_fast(arr_vec[i], patt, vec_size);
        if (diff < min_diff)
        {
            min_diff = diff;
            nearest = arr_vec[i];
        }
    }

    return nearest;
}

vector * Find_vector(vector *patt, char *data_file)
{
    unsigned vec_size, max_bufVEC_elements, vecs_left_in_file;

	FILE *data;
	if ((data = fopen(data_file, "rb")) == NULL)
		exit(1);

    //GETTING META FROM DATAFILE
    fread(&vecs_left_in_file, sizeof(unsigned), 1, data);
    fread(&vec_size, sizeof(unsigned), 1, data);
    //GETTING META FROM DATAFILE

    max_bufVEC_elements = Get_RAM() / (RAM_SAVER_MULTIPLIER * (sizeof(vector) + vec_size * sizeof(double)));

    unsigned readed_vecs;
    vector *find_for_return = NULL, *find_vec;

    vector **buf_vec;
    if ((buf_vec = (vector **) calloc((max_bufVEC_elements > vecs_left_in_file) ? vecs_left_in_file : max_bufVEC_elements, sizeof(vector *))) == NULL)
        exit(1); // memory lost

    do{
        readed_vecs = Load_vectors(buf_vec, max_bufVEC_elements, data, vec_size, vecs_left_in_file);
        vecs_left_in_file -= readed_vecs;
        find_vec = CopyVec(Find_nearest_local(buf_vec, patt, vec_size, readed_vecs));

        free_arr_vec(buf_vec, readed_vecs);

        find_for_return = (find_for_return == NULL) ? find_vec : find_for_return;
        if (vec_cmp_fast(patt, find_vec, vec_size) < vec_cmp_fast(patt, find_for_return, vec_size))
        {
            FreeVec(find_for_return);
            find_for_return = find_vec;
        }
    } while (vecs_left_in_file > 0);

    free(buf_vec);

    return find_for_return;
}
