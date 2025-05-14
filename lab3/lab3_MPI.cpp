#include <iostream>
#include <fstream>
#include <mpi.h>
#include <vector>
#include <chrono>
#include <numeric>

using namespace std;
using namespace std::chrono;

void readMatrixFromFile(const std::string& filename, int*& matrix, int& size) {
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Error opening file: " << filename << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
        return;
    }

    inFile >> size;
    matrix = new int[size * size];

    for (int i = 0; i < size * size; ++i) {
        inFile >> matrix[i];
    }

    inFile.close();
}

void writeMatrixToFile(const std::string& filename, int* matrix, int size) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error creating file: " << filename << std::endl;
        return;
    }

    outFile << size << "\n";
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            outFile << matrix[i * size + j] << " ";
        }
        outFile << "\n";
    }
    outFile.close();
}

void Flip(int* B, int dim) {
    for (int i = 0; i < dim; ++i) {
        for (int j = i + 1; j < dim; ++j) {
            std::swap(B[i * dim + j], B[j * dim + i]);
        }
    }
}

long long MatrixMultiplicationMPI(int* A, int* B, int* C, int Size, int ProcNum, int ProcRank) {
    int dim = Size;
    int ProcPartSize = dim / ProcNum;
    int ProcPartElem = ProcPartSize * dim;

    int* bufA = new int[ProcPartElem];
    int* bufC = new int[ProcPartElem];

    MPI_Scatter(A, ProcPartElem, MPI_INT, bufA, ProcPartElem, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    auto start = high_resolution_clock::now();

    for (int i = 0; i < ProcPartSize; ++i) {
        for (int j = 0; j < dim; ++j) {
            int temp = 0;
            for (int k = 0; k < dim; ++k) {
                temp += bufA[i * dim + k] * B[j * dim + k];  // B уже транспонирована
            }
            bufC[i * dim + j] = temp;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    auto end = high_resolution_clock::now();
    long long duration_ms = duration_cast<milliseconds>(end - start).count();

    MPI_Gather(bufC, ProcPartElem, MPI_INT, C, ProcPartElem, MPI_INT, 0, MPI_COMM_WORLD);

    delete[] bufA;
    delete[] bufC;

    return duration_ms;
}

long long mul_matrix(const std::string& path1, const std::string& path2, const std::string& result_path) {
    int ProcNum, ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    int size = 0;
    int* A = nullptr, * B = nullptr, * C = nullptr;

    if (ProcRank == 0) {
        int size1, size2;
        readMatrixFromFile(path1, A, size1);
        readMatrixFromFile(path2, B, size2);

        if (size1 != size2) {
            std::cerr << "Matrix sizes don't match!" << std::endl;
            delete[] A;
            delete[] B;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        size = size1;

        if (size % ProcNum != 0) {
            std::cerr << "Matrix size must be divisible by number of processes!" << std::endl;
            delete[] A;
            delete[] B;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        C = new int[size * size];
        Flip(B, size);  // Транспонируем B один раз
    }

    MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (ProcRank != 0) {
        A = new int[size * size];
        B = new int[size * size];
        C = new int[size * size];  // Только чтобы передать указатель (не используется)
    }

    // Рассылаем A и B
    MPI_Bcast(A, size * size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(B, size * size, MPI_INT, 0, MPI_COMM_WORLD);

    long long duration = MatrixMultiplicationMPI(A, B, C, size, ProcNum, ProcRank);

    if (ProcRank == 0) {
        writeMatrixToFile(result_path, C, size);
    }

    delete[] A;
    delete[] B;
    if (ProcRank == 0) delete[] C;

    return duration;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    const std::vector<std::string> sizes = { "100", "200", "300", "400", "500", "1000", "1500", "2000" };
    const int NUM_RUNS = 10;

    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    for (const auto& size : sizes) {
        std::string path1 = "../matrix/matrix" + size + "_1.txt";
        std::string path2 = "../matrix/matrix" + size + "_2.txt";
        std::string result_path = "../matrix/matrix" + size + "_result_MPI.txt";

        vector<long long> durations;
        durations.reserve(NUM_RUNS);

        for (int run = 0; run < NUM_RUNS; ++run) {
            long long duration = mul_matrix(path1, path2, result_path);

            if (ProcRank == 0) {
                durations.push_back(duration);
                cout << "Size " << size << ", run " << run + 1 << ": " << duration << " ms" << endl;
            }
        }

        if (ProcRank == 0 && !durations.empty()) {
            long long sum = accumulate(durations.begin(), durations.end(), 0LL);
            double average = static_cast<double>(sum) / NUM_RUNS;

            cout << "----------------------------------------" << endl;
            cout << "Matrix size " << size << " average multiplication time: "
                << average << " ms (over " << NUM_RUNS << " runs)" << endl;
            cout << "----------------------------------------" << endl;

            ofstream time_log("average_times.txt", ios::app);
            if (time_log) {
                time_log << size << " " << average << endl;
            }
        }
    }

    MPI_Finalize();
    return 0;
}
