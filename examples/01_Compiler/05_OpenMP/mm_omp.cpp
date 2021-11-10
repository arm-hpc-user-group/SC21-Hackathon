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

#if defined(USE_ARMPL)

#include <armpl.h>
#define LIBRARY_NAME "ArmPL"

#elif defined(USE_SSL2)

#include <cssl.h>
#define LIBRARY_NAME "SSL2"

#elif defined(USE_CBLAS)

#include <cblas.h>
#define LIBRARY_NAME "CBLAS"

#else
#error Unsupported library
#endif


using timer_clock= std::chrono::high_resolution_clock;


inline unsigned int min(unsigned int a, unsigned int b)
{
    return (a < b) ? a : b;
}


void openmp_multiply(double **matA, double **matB, double **matC, unsigned int n,
        unsigned int m, unsigned int l, unsigned int blockSize)
{
    timer_clock::time_point t1= timer_clock::now();
    unsigned int i, j, jj, k, kk;
    // Perform the matrix-matrix multiplication with a bit of blocking and
    // loop unrolling. We cheat with the loop unrolling, and just expect the
    // dimensions that are passed in to be a multiple of two.
    #pragma omp parallel for collapse(2) private(i, j, jj, k, kk)
    for (kk= 0; kk < m; kk+= blockSize){
        for(jj= 0; jj < l; jj+= blockSize){
            for(i= 0; i < n; i+= 2){
                for(j= jj; j < min(l, jj + blockSize); j+= 2){
                    for(k =kk; k < min(m, kk + blockSize); ++k){
                        matC[i][j] += matA[i][k] * matB[k][j];
                        matC[i][j+1] += matA[i][k] * matB[k][j+1];
                        matC[i+1][j] += matA[i+1][k] * matB[k][j];
                        matC[i+1][j+1] += matA[i+1][k] * matB[k][j+1];
                    }
                }
            }
        }
    }
    timer_clock::time_point t2= timer_clock::now();
    std::chrono::duration<double> time_span= std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    printf("OpenMP multiply took: %.3lf seconds\n", time_span.count());
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
    unsigned int blockSize;

    unsigned int i, j, k;

    if (argc < 5){
        printf("Usage: %s n m l blockSize\n", argv[0]);
        return 1;
    }

    n= (unsigned int)atoi(argv[1]);
    m= (unsigned int)atoi(argv[2]);
    l= (unsigned int)atoi(argv[3]);
    blockSize= (unsigned int)atoi(argv[4]);

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

    // Perform the matrix-matrix multiplication with a bit of blocking and
    // loop unrolling
    //printf("Performing multiply\n");
    //openmp_multiply(matA, matB, matC, n, m, l, blockSize);

    // Perform the matrix-matrix multiplication with the library
    t1= timer_clock::now();
#if defined(USE_ARMPL) || defined(USE_CBLAS)
    printf("Using DGEMM routine from %s library\n", LIBRARY_NAME);
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, m, l, 1, dataA,
            m, dataB, l, 1, dataC, m);
#elif defined(USE_SSL2)
    printf("Using DVMGGM routine from %s library\n", LIBRARY_NAME);
    int icon, ierr;
    ierr = c_dvmggm(dataA, m, dataB, l, dataC, l, n, m, l, &icon);
#endif
    t2= timer_clock::now();
    time_span= std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    printf("%s library took: %.3lf seconds\n", LIBRARY_NAME, time_span.count());

    // Free memory
    free(matA);
    free(dataA);
    free(matB);
    free(dataB);
    free(matC);
    free(dataC);
}
