#include "maths.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <arm_neon.h> // neon intrinsics
#include <omp.h> // openmp

#include <stdio.h>

#define BLOCK_SIZE 64 // must be multiple of 4

void matadd(const float* A, const float* B, float* C, int N, int M) {
    #pragma omp parallel for
    for (int i = 0; i < M * N; i += 4) {
        float32x4_t a = vld1q_f32(&A[i]);
        float32x4_t b = vld1q_f32(&B[i]);
        float32x4_t c = vaddq_f32(a, b);
        vst1q_f32(&C[i], c);
    }
}

void scalmult(const float* A, const float S, float* B, int N, int M) {
    #pragma omp parallel for
    for (int i = 0; i < M * N; i += 4) {
        float32x4_t s = vdupq_n_f32(S);
        float32x4_t a = vld1q_f32(&A[i]);
        float32x4_t b = vmulq_f32(a, s);
        vst1q_f32(&B[i], b);
    }
}



void transpose(const float* B, float* B_t, int M, int K) {
    for (int m = 0; m < M; ++m) {
        for (int k = 0; k < K; ++k) {
            B_t[k * M + m] = B[m * K + k];
        }
    }
}

// A = N*M
// B = M*K
// C = N * K

// Assume perfect sized matrices for blocksize
void matmul_cpu(const float* A, const float* B, float* C, int N, int M, int K) {
    memset(C, 0, N * K * sizeof(float));
    float* B_t = (float*) malloc(K * M * sizeof(float));

    if (B_t == NULL) {
        printf("B_t malloc failed \n");
        return;
    }

    transpose(B, B_t, M, K);

    # pragma omp parallel for collapse(2) schedule(static)
    for (int i = 0; i < N; i += BLOCK_SIZE) {
        for (int j = 0; j < K; j += BLOCK_SIZE) {

            for (int ii = i; ii < i + BLOCK_SIZE; ++ii) {
                for (int jj = j; jj < j + BLOCK_SIZE; ++jj) {

                    float total_sum = 0.0f;

                    for (int k = 0; k < M; k += BLOCK_SIZE) {
                        float32x4_t accum_vec = vdupq_n_f32(0.0f);

                        int kk;
                        for (kk = k; kk < k + BLOCK_SIZE; kk+=4) {
                            float32x4_t a = vld1q_f32(&A[ii * M + kk]);
                            float32x4_t b = vld1q_f32(&B_t[jj * M + kk]);
                            accum_vec = vmlaq_f32(accum_vec, a, b);
                        }

                        total_sum += vaddvq_f32(accum_vec);
                    }

                    C[ii*K + jj] = total_sum;
                }
            }
        }
    }

    free(B_t);
}