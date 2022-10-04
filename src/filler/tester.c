//
// Created by artem on 22.10.2020.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define HI 100
#define LO 0
#define DATA_FILE "../../data/data.txt"
#define PATT_FILE "../../data/patt.txt"


int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("missing args\nUSAGE: %s [num of vectors] [size of vector]\n", argv[0]);
		exit(1);
	}
	
    srand(getpid());

	//vector setup
    int all = atoi(argv[1]);
    int vec_size = atoi(argv[2]);
    double *arr = (double *) calloc(vec_size, sizeof(double));
	//vector setup

    FILE *data, *mypattern;

	//writing patt
    if ((mypattern = fopen(PATT_FILE, "wb")) == NULL)
{
		printf("error");
		exit(1);
}

	fwrite(&vec_size, sizeof(int), 1, mypattern);

	for (int i = 0; i < vec_size; i++)
        arr[i] = LO + (double) (rand()) / ((double) (RAND_MAX / (HI - LO)));
    fwrite(arr, sizeof(double), vec_size, mypattern);

	fclose(mypattern);
	//writing patt


	//writing data
    if ((data = fopen(DATA_FILE, "wb")) == NULL)
		exit(1);

	//meta
    fwrite(&all, sizeof(int), 1, data);
    fwrite(&vec_size, sizeof(int), 1, data);
	//meta


	//vectors
    for (int j = 0; j < all; j++)
    {
        for (int i = 0; i < vec_size; i++)
            arr[i] = LO + (double) (rand()) / ((double) (RAND_MAX / (HI - LO)));
        fwrite(arr, sizeof(double), vec_size, data);
    }
	//vectors
	//writing data


    free(arr);
    fclose(data);

    return 0;
}
