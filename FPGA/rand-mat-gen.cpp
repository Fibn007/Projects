/*
**	Radnom matrix generator. It takes the numbers of rows N and columns M,
** and outputs a NxM matrix where the number of cell is in the range [0, 255]
*/

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 3) {
        cout << "Correct format: cmd rows columns" << endl;
        return 1;
    }

    unsigned rows = atoi(argv[1]);
    unsigned cols = atoi(argv[2]);
    
    /* initialize random seed: */
    srand (time(NULL));
    
    for(unsigned i = 0; i < rows; i++) {
        for(unsigned j = 0; j < cols; j++) {
            int N = 1, M = 256;
            
            unsigned r = M + rand() / (RAND_MAX / (N - M + 1) + 1);
            printf("%4d ", r);
        }
        printf("\n") ;
    }
    
    return 0;
}