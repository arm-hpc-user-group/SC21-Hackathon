//------------------------------------------------------------------------------------
// Copyright (C) Arm Limited, 2019-2020 All rights reserved.
//
// The example code is provided to you as an aid to learning when working
// with Arm-based technology, including but not limited to programming tutorials.
//
// Arm hereby grants to you, subject to the terms and conditions of this Licence,
// a non-exclusive, non-transferable, non-sub-licensable, free-of-charge copyright
// licence, to use, copy and modify the Software solely for the purpose of internal
// demonstration and evaluation.
//
// You accept that the Software has not been tested by Arm therefore the Software
// is provided "as is", without warranty of any kind, express or implied. In no
// event shall the authors or copyright holders be liable for any claim, damages
// or other liability, whether in action or contract, tort or otherwise, arising
// from, out of or in connection with the Software or the use of Software.
//------------------------------------------------------------------------------------

#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <chrono>


using timer_clock= std::chrono::high_resolution_clock;


inline unsigned int min(unsigned int a, unsigned int b)
{
    return (a < b) ? a : b;
}


void naive_multiply(double **matA, double **matB, double **matC, unsigned int n,
        unsigned int m, unsigned int l)
{
    timer_clock::time_point t1= timer_clock::now();
    unsigned int i, j, k;
    // Perform the matrix-matrix multiplication naively
    for (i= 0; i < n; ++i){
        for (j= 0; j < l; ++j){
            for (k= 0; k < m; ++k){
                matC[i][j] += matA[i][k] * matB[k][j];
            }
        }
    }
    timer_clock::time_point t2= timer_clock::now();
    std::chrono::duration<double> time_span= std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    printf("Naive multiply took: %.3lf seconds\n", time_span.count());
}


int main(int argc, char** argv)
{
    double *dataA= NULL;
    double **matA= NULL;
    double *dataB= NULL;
    double **matB= NULL;
    double *dataC= NULL;
    double **matC= NULL;

    // Assume that the matrix dimensions have been passed in as n, m and l, where
    // A = n x m matrix
    // B = m x l matrix
    // C = n x l matrix
    
    unsigned int n, m, l;

    unsigned int i, j, k;

    if (argc < 4){
        printf("Usage: %s n m l\n", argv[0]);
        return 1;
    }

    n= (unsigned int)atoi(argv[1]);
    m= (unsigned int)atoi(argv[2]);
    l= (unsigned int)atoi(argv[3]);

    // Assign memory
    dataA= (double*)malloc(n*m*sizeof(double));
    matA= (double**)malloc(n*sizeof(double*));
    dataB= (double*)malloc(m*l*sizeof(double));
    matB= (double**)malloc(m*sizeof(double*));
    dataC= (double*)malloc(n*l*sizeof(double));
    matC= (double**)malloc(n*sizeof(double*));

    timer_clock::time_point t1= timer_clock::now();
    // Set up the matrices in row major format
    for(i= 0; i < n; ++i){
        matA[i]= dataA + i*m;
        matC[i]= dataC + i*l;
    }

    for(i= 0; i < m; ++i){
        matB[i]= dataB + i*l;
    }

    // Initialise matrices. Matrices A and B get random data. Results matrix C
    // is initialised to zero
    srand(time(NULL));
    for (i= 0; i < n; ++i){
        for (j= 0; j < m; ++j){
            matA[i][j]= ((double)rand()) / RAND_MAX;
        }

        for (j= 0; j < l; ++j){
            matC[i][j]= 0.0;
        }
    }

    for (i= 0; i < m; ++i){
        for (j= 0; j < l; ++j){
            matB[i][j]= ((double)rand()) / RAND_MAX;
        }
    }

    timer_clock::time_point t2= timer_clock::now();
    std::chrono::duration<double> time_span= std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    printf("Set up of matrices took: %.3lf seconds\n", time_span.count());

    // Perform the matrix-matrix multiplication naively
    printf("Performing multiply\n");
    naive_multiply(matA, matB, matC, n, m, l);

    // Free memory
    free(matA);
    free(dataA);
    free(matB);
    free(dataB);
    free(matC);
    free(dataC);
}
