// src/test_main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "maths.h"

// Helper function to print a matrix
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

int main() {
    // Define matrix dimensions:
    // A(N, K) @ B(K, M) -> C(N, M)
    // All dimensions are even, as requested.
    int N = 64; // A rows, C rows
    int K = 64; // A cols, B rows
    int M = 64; // B cols, C cols

    // Allocate memory on the heap
    float* A = (float*)malloc(N * K * sizeof(float));
    float* B = (float*)malloc(K * M * sizeof(float));
    float* C = (float*)malloc(N * M * sizeof(float));

    // Check if malloc failed
    if (A == NULL || B == NULL || C == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

        // Fill A with row = i+1 (so row 0 has 1s, row 1 has 2s, ...)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < K; j++) {
            A[i*K + j] = (float)(i + 1);
        }
    }

    // Fill B with col = j+1 (so column 0 has 1s, column 1 has 2s, ...)
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < M; j++) {
            B[i*M + j] = (float)(j + 1);
        }
    }

    // --- Run the function to test ---
    matmul_cpu(A, B, C, N, M, K);
    // --------------------------------

    // Print the inputs and the final result
    print_matrix(A, N, K, "Matrix A");
    print_matrix(B, K, M, "Matrix B");
    print_matrix(C, N, M, "Result C (A @ B)");

    // Clean up memory
    free(A);
    free(B);
    free(C);

    return 0;
}