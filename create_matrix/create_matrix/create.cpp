#include <iostream>
#include <fstream>
#include <cstdlib>  
#include <ctime>   

void generateMatrixToFile(const std::string& filename, int size) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error!" << std::endl;
        return;
    }

    outFile << size << std::endl;

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {

            int randomValue = (std::rand() % 199) - 99;  
            outFile << randomValue << " ";
        }
        outFile << std::endl;
    }

    outFile.close();

    std::cout << "" << filename << std::endl;
}

int main() {

    std::srand(std::time(0));

    generateMatrixToFile("../../matrix/matrix10_1.txt", 10);
    generateMatrixToFile("../../matrix/matrix10_2.txt", 10);
    generateMatrixToFile("../../matrix/matrix50_1.txt", 50);
    generateMatrixToFile("../../matrix/matrix50_2.txt", 50);
    generateMatrixToFile("../../matrix/matrix100_1.txt", 100);
    generateMatrixToFile("../../matrix/matrix100_2.txt", 100);
    generateMatrixToFile("../../matrix/matrix250_1.txt", 250);
    generateMatrixToFile("../../matrix/matrix250_2.txt", 250);
    generateMatrixToFile("../../matrix/matrix500_1.txt", 500);
    generateMatrixToFile("../../matrix/matrix500_2.txt", 500);
    generateMatrixToFile("../../matrix/matrix750_1.txt", 750);
    generateMatrixToFile("../../matrix/matrix750_2.txt", 750);
    generateMatrixToFile("../../matrix/matrix1000_1.txt", 1000);
    generateMatrixToFile("../../matrix/matrix1000_2.txt", 1000);
    generateMatrixToFile("../../matrix/matrix1250_1.txt", 1250);
    generateMatrixToFile("../../matrix/matrix1250_2.txt", 1250);
    generateMatrixToFile("../../matrix/matrix1500_1.txt", 1500);
    generateMatrixToFile("../../matrix/matrix1500_2.txt", 1500);
    generateMatrixToFile("../../matrix/matrix1750_1.txt", 1750);
    generateMatrixToFile("../../matrix/matrix1750_2.txt", 1750);
    generateMatrixToFile("../../matrix/matrix2000_1.txt", 2000);
    generateMatrixToFile("../../matrix/matrix2000_2.txt", 2000);

    return 0;
}