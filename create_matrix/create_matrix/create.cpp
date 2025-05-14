#include <iostream>
#include <fstream>
#include <cstdlib>  
#include <ctime>   
#include <vector>
#include <string>

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

	std::vector<int> sizes = {
			10, 50, 100, 200, 250, 300, 400, 500,
			750, 1000, 1250, 1500, 1750, 2000
	};

	for (int size : sizes) {
		std::string path1 = "../../matrix/matrix" + std::to_string(size) + "_1.txt";
		std::string path2 = "../../matrix/matrix" + std::to_string(size) + "_2.txt";
		generateMatrixToFile(path1, size);
		generateMatrixToFile(path2, size);
	}

	return 0;
}