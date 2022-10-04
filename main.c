#include <stdio.h>
#include "src/vector/vector.h"
#include <stdlib.h>

//#ifdef PARALLEL		//почему то не потребовались...
#include "libs/parallel_lib/Parallel_search.h"
//#endif

//#ifdef LINEAR
#include "libs/linear_lib/Linear_search.h"
//#endif

#define DATA_FILE "data/data.txt"
#define PATT_FILE "data/patt.txt"


int main() {
    vector *finder, *patt;


    //GETTING PATTERN
    FILE *fpatt;
    if ((fpatt = fopen(PATT_FILE, "rb")) == NULL)
        exit(1);
    patt = LoadPattern(fpatt);
    fclose(fpatt);

    printf("HERE'S PATTERN:\n");
    PrintVec(patt);
    printf("\n\n\n");
    //GETTING PATTERN

    finder = Find_vector(patt, DATA_FILE);

    //PRINT ANSWER
    printf("FINDED THIS:\n");
    PrintVec(finder);
    printf("\ndifference : %lf\n", vec_cmp(patt, finder));
    //PRINT ANSWER

    FreeVec(finder);
    FreeVec(patt);

    return 0;
}
