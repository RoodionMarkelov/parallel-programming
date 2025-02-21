import numpy as np

def read_matrix_from_file(filename):
    """Чтение матрицы из файла."""
    with open(filename, 'r') as file:
        size = int(file.readline())
        matrix = []
        for _ in range(size):
            row = list(map(int, file.readline().split()))
            matrix.append(row)
        return np.array(matrix)

def check_matrix_multiplication(matrix1_file, matrix2_file, result_file):
    """Проверка перемножения матриц."""
    matrix1 = read_matrix_from_file(matrix1_file)
    matrix2 = read_matrix_from_file(matrix2_file)
    result_cpp = read_matrix_from_file(result_file)

    result_np = np.dot(matrix1, matrix2)

    if np.array_equal(result_cpp, result_np):
        print(f"Результаты для {matrix1.shape} совпадают!")
    else:
        print(f"Результаты для {matrix1.shape} НЕ совпадают!")
        print("Результат, посчитанный на C++:")
        print(result_cpp)
        print("Результат, посчитанный numpy:")
        print(result_np)

if __name__ == "__main__":
    check_matrix_multiplication(
        "../../matrix/matrix10_1.txt",
        "../../matrix/matrix10_2.txt",
        "../../matrix/matrix10_result.txt"
    )
    check_matrix_multiplication(
        "../../matrix/matrix50_1.txt",
        "../../matrix/matrix50_2.txt",
        "../../matrix/matrix50_result.txt"
    )
    check_matrix_multiplication(
        "../../matrix/matrix100_1.txt",
        "../../matrix/matrix100_2.txt",
        "../../matrix/matrix100_result.txt"
    )
    check_matrix_multiplication(
        "../../matrix/matrix250_1.txt",
        "../../matrix/matrix250_2.txt",
        "../../matrix/matrix250_result.txt"
    )
    check_matrix_multiplication(
        "../../matrix/matrix500_1.txt",
        "../../matrix/matrix500_2.txt",
        "../../matrix/matrix500_result.txt"
    )
    check_matrix_multiplication(
        "../../matrix/matrix750_1.txt",
        "../../matrix/matrix750_2.txt",
        "../../matrix/matrix750_result.txt"
    )
    check_matrix_multiplication(
        "../../matrix/matrix1000_1.txt",
        "../../matrix/matrix1000_2.txt",
        "../../matrix/matrix1000_result.txt"
    )
    check_matrix_multiplication(
        "../../matrix/matrix1250_1.txt",
        "../../matrix/matrix1250_2.txt",
        "../../matrix/matrix1250_result.txt"
    )
    check_matrix_multiplication(
        "../../matrix/matrix1500_1.txt",
        "../../matrix/matrix1500_2.txt",
        "../../matrix/matrix1500_result.txt"
    )
    check_matrix_multiplication(
        "../../matrix/matrix1750_1.txt",
        "../../matrix/matrix1750_2.txt",
        "../../matrix/matrix1750_result.txt"
    )
    check_matrix_multiplication(
        "../../matrix/matrix2000_1.txt",
        "../../matrix/matrix2000_2.txt",
        "../../matrix/matrix2000_result.txt"
    )
