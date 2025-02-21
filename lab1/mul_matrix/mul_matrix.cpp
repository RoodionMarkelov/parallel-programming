#include <iostream>
#include <fstream>

void readMatrixFromFile(const std::string& filename, int**& matrix, int& size) {
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Error!" << std::endl;
        return;
    }

    inFile >> size;  

    matrix = new int* [size];
    for (int i = 0; i < size; ++i) {
        matrix[i] = new int[size];
    }

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            inFile >> matrix[i][j];
        }
    }

    inFile.close();
}

void writeMatrixToFile(const std::string& filename, int** matrix, int size) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error creating file: " << filename << std::endl;
        return;
    }

    outFile << size << std::endl;

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            outFile << matrix[i][j] << " ";
        }
        outFile << std::endl;
    }

    outFile.close();
}

void mul_matrix(const std::string& path_to_matrix1, const std::string& path_to_matrix2, const std::string& path_to_result_matrix) {
    int size1, size2;
    int** matrix1 = nullptr;
    readMatrixFromFile(path_to_matrix1, matrix1, size1);

    int** matrix2 = nullptr;
    readMatrixFromFile(path_to_matrix2, matrix2, size2);

    if (size1 != size2) {
        std::cerr << "Sizes do not match!" << std::endl;
        for (int i = 0; i < size1; ++i) delete[] matrix1[i];
        for (int i = 0; i < size2; ++i) delete[] matrix2[i];
        delete[] matrix1;
        delete[] matrix2;
        return;
    }

    int size = size1;

    /* std::cout << "Matrix from file: " << filename << ":\n"; // вывод матрицы 1 в консоль.
   for (int i = 0; i < size; ++i) {
       for (int j = 0; j < size; ++j) {
           std::cout << matrix1[i][j] << " ";
       }
       std::cout << "\n";
   }*/

   /* std::cout << "Matrix from file: " << filename << ":\n"; // вывод матрицы 2 в консоль.
  for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size; ++j) {
          std::cout << matrix2[i][j] << " ";
      }
      std::cout << "\n";
  }*/

    int** resultMatrix = new int* [size];
    for (int i = 0; i < size; ++i) {
        resultMatrix[i] = new int[size];
    }

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            resultMatrix[i][j] = 0; 
            for (int k = 0; k < size; ++k) {
                resultMatrix[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    writeMatrixToFile(path_to_result_matrix, resultMatrix, size);

    for (int i = 0; i < size; ++i) {
        delete[] matrix1[i];
        delete[] matrix2[i];
        delete[] resultMatrix[i];
    }
    delete[] matrix1;
    delete[] matrix2;
    delete[] resultMatrix;

    std::cout << "Complete " << size << "x" << size << "!" << std::endl;
}

int main() {
    mul_matrix("../../matrix/matrix10_1.txt", "../../matrix/matrix10_2.txt", "../../matrix/matrix10_result.txt");
    mul_matrix("../../matrix/matrix50_1.txt", "../../matrix/matrix50_2.txt", "../../matrix/matrix50_result.txt");
    mul_matrix("../../matrix/matrix100_1.txt", "../../matrix/matrix100_2.txt", "../../matrix/matrix100_result.txt");
    mul_matrix("../../matrix/matrix250_1.txt", "../../matrix/matrix250_2.txt", "../../matrix/matrix250_result.txt");
    mul_matrix("../../matrix/matrix500_1.txt", "../../matrix/matrix500_2.txt", "../../matrix/matrix500_result.txt");
    mul_matrix("../../matrix/matrix750_1.txt", "../../matrix/matrix750_2.txt", "../../matrix/matrix750_result.txt");
    mul_matrix("../../matrix/matrix1000_1.txt", "../../matrix/matrix1000_2.txt", "../../matrix/matrix1000_result.txt");
    mul_matrix("../../matrix/matrix1250_1.txt", "../../matrix/matrix1250_2.txt", "../../matrix/matrix1250_result.txt");
    mul_matrix("../../matrix/matrix1500_1.txt", "../../matrix/matrix1500_2.txt", "../../matrix/matrix1500_result.txt");
    mul_matrix("../../matrix/matrix1750_1.txt", "../../matrix/matrix1750_2.txt", "../../matrix/matrix1750_result.txt");
    mul_matrix("../../matrix/matrix2000_1.txt", "../../matrix/matrix2000_2.txt", "../../matrix/matrix2000_result.txt");
    return 0;
}