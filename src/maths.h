#ifndef maths_h
#define maths_h

void matadd(const float* A, const float* B, float* C, int N, int M);
void scalmult(const float* A, const float S, float* B, int N, int M);
void transpose(const float* B, float* B_t, int M, int K);
void matmul_cpu(const float* A, const float* B, float* C, int M, int N, int K);
void matmul_gpu(const float* A, const float* B, float* C, int M, int N, int K);

#endif