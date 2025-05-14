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
    sizes = [10, 50, 100, 250, 500, 750, 1000, 1250, 1500, 1750, 2000]
    for size in sizes:
        check_matrix_multiplication(
            f"../../matrix/matrix{size}_1.txt",
            f"../../matrix/matrix{size}_2.txt",
            f"../../matrix/matrix{size}_result_CUDA.txt"
        )
