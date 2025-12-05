// src/test_main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "maths.h"
#include "tokenizer.h"

void print_matrix(const float* M, int rows, int cols, const char* name) {
    printf("%s ( %d x %d ):\n", name, rows, cols);
    for (int i = 0; i < rows; i++) {
        printf("  [");
        for (int j = 0; j < cols; j++) {
            printf("%7.2f", M[i*cols + j]);
            if (j < cols - 1) printf(", ");
        }
        printf(" ]\n");
    }
}

int testmat() {
    // Define matrix dimensions:
    // A(N, M) @ B(M, K) -> C(N, K)
    // All dimensions are even, as requested.
    int N = 1024; // A rows, C rows
    int M = 1024; // A cols, B rows
    int K = 1024; // B cols, C cols

    // Allocate memory on the heap
    float* A = (float*)malloc(N * M * sizeof(float));
    float* B = (float*)malloc(M * K * sizeof(float));
    float* C = (float*)malloc(N * K * sizeof(float));

    // Check if malloc failed
    if (A == NULL || B == NULL || C == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

        // Fill A with row = i+1 (so row 0 has 1s, row 1 has 2s, ...)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            A[i*M + j] = (float)(i + 1);
        }
    }

    // Fill B with col = j+1 (so column 0 has 1s, column 1 has 2s, ...)
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < K; j++) {
            B[i*K + j] = (float)(j + 1);
        }
    }

    double start_time, end_time, elapsed_time;
    start_time = omp_get_wtime();

    matmul_cpu(A, B, C, N, M, K);

    end_time = omp_get_wtime();
    elapsed_time = end_time - start_time;

    printf("\nExecution Time: %f seconds\n", elapsed_time);

    // Clean up memory
    free(A);
    free(B);
    free(C);

    return 0;
}

int main() {
    // testmat();
    tokenize();
}