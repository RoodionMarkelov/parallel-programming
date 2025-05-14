#include <iostream>
#include <fstream>
#include <string>
#include <cuda_runtime.h>
#include <vector>
#include <chrono>

void readMatrixFromFile(const std::string& filename, int**& matrix, int& size) {
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Error reading file: " << filename << std::endl;
        exit(1);
    }

    inFile >> size;
    matrix = new int* [size];
    for (int i = 0; i < size; ++i) {
        matrix[i] = new int[size];
        for (int j = 0; j < size; ++j)
            inFile >> matrix[i][j];
    }
    inFile.close();
}

void writeMatrixToFile(const std::string& filename, int** matrix, int size) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error writing to file: " << filename << std::endl;
        exit(1);
    }

    outFile << size << std::endl;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j)
            outFile << matrix[i][j] << " ";
        outFile << std::endl;
    }
    outFile.close();
}

__global__ void matrixMulKernel(const int* A, const int* B, int* C, int N) {
    const int TILE_SIZE = 16;

    __shared__ int tileA[TILE_SIZE][TILE_SIZE];
    __shared__ int tileB[TILE_SIZE][TILE_SIZE];

    int row = blockIdx.y * TILE_SIZE + threadIdx.y;
    int col = blockIdx.x * TILE_SIZE + threadIdx.x;

    int temp = 0;
    for (int t = 0; t < (N + TILE_SIZE - 1) / TILE_SIZE; ++t) {
        if (row < N && t * TILE_SIZE + threadIdx.x < N)
            tileA[threadIdx.y][threadIdx.x] = A[row * N + t * TILE_SIZE + threadIdx.x];
        else
            tileA[threadIdx.y][threadIdx.x] = 0;

        if (col < N && t * TILE_SIZE + threadIdx.y < N)
            tileB[threadIdx.y][threadIdx.x] = B[(t * TILE_SIZE + threadIdx.y) * N + col];
        else
            tileB[threadIdx.y][threadIdx.x] = 0;

        __syncthreads();

        for (int k = 0; k < TILE_SIZE; ++k)
            temp += tileA[threadIdx.y][k] * tileB[k][threadIdx.x];

        __syncthreads();
    }

    if (row < N && col < N)
        C[row * N + col] = temp;
}

void mul_matrix_cuda(const std::string& pathA, const std::string& pathB, const std::string& pathC) {
    int size;
    int** matA = nullptr;
    int** matB = nullptr;

    readMatrixFromFile(pathA, matA, size);
    readMatrixFromFile(pathB, matB, size);

    int N = size;
    size_t bytes = N * N * sizeof(int);

    int* h_A = new int[N * N];
    int* h_B = new int[N * N];
    int* h_C = new int[N * N];

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) {
            h_A[i * N + j] = matA[i][j];
            h_B[i * N + j] = matB[i][j];
        }

    int* d_A, * d_B, * d_C;
    cudaMalloc(&d_A, bytes);
    cudaMalloc(&d_B, bytes);
    cudaMalloc(&d_C, bytes);

    cudaMemcpy(d_A, h_A, bytes, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, bytes, cudaMemcpyHostToDevice);

    dim3 threads(16, 16);
    dim3 blocks((N + 15) / 16, (N + 15) / 16);

    auto start = std::chrono::high_resolution_clock::now();
    matrixMulKernel <<<blocks, threads >>> (d_A, d_B, d_C, N);
    cudaDeviceSynchronize();
    auto end = std::chrono::high_resolution_clock::now();

    long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "CUDA time for " << N << "x" << N << ": " << ms << " ms" << std::endl;

    cudaMemcpy(h_C, d_C, bytes, cudaMemcpyDeviceToHost);

    int** result = new int* [N];
    for (int i = 0; i < N; ++i) {
        result[i] = new int[N];
        for (int j = 0; j < N; ++j)
            result[i][j] = h_C[i * N + j];
    }

    writeMatrixToFile(pathC, result, N);

    for (int i = 0; i < N; ++i) {
        delete[] matA[i];
        delete[] matB[i];
        delete[] result[i];
    }
    delete[] matA;
    delete[] matB;
    delete[] result;
    delete[] h_A;
    delete[] h_B;
    delete[] h_C;
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    std::cout << "Completed " << N << "x" << N << "\n" << std::endl;
}

int main() {
    const std::vector<std::string> sizes = { "10", "50", "100", "250", "500",
                                             "750", "1000", "1250", "1500", "1750", "2000" };
    const int NUM_RUNS = 5;

    for (const auto& size : sizes) {
        std::string path1 = "../matrix/matrix" + size + "_1.txt";
        std::string path2 = "../matrix/matrix" + size + "_2.txt";
        std::string result_path = "../matrix/matrix" + size + "_result_CUDA.txt";

        long long total_time = 0;

        for (int i = 0; i < NUM_RUNS; ++i) {
            auto start = std::chrono::high_resolution_clock::now();

            mul_matrix_cuda(path1, path2, result_path);

            auto end = std::chrono::high_resolution_clock::now();
            long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            total_time += ms;
        }
    }

    return 0;
}
