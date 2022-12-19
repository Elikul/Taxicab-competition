import numpy as np


def read_from_file(filename):
    file = open(filename, "r")
    try:
        count_v = int(file.readline().replace("n = ", ""))
        vertexes = []
        for i in range(1, count_v + 1):
            vertexes.append(tuple(int(i.strip()) for i in file.readline().split('\t')))
        return count_v, vertexes
    finally:
        file.close()


def get_manhattan_distance(p, q):
    distance = 0
    for p_i, q_i in zip(p, q):
        distance += abs(p_i - q_i)

    return distance


def count_weights(count_v, vertexes):
    weights = np.zeros(shape=(count_v, count_v))
    for i in range(count_v):
        for j in range(count_v):
            weights[i, j] = get_manhattan_distance(vertexes[i], vertexes[j])
    return weights


def main():
    n, V = read_from_file("../input/Taxicab_64.txt")
    matrix_weight = count_weights(n, V)
    new_matrix = np.zeros((n, n))
    for i in range(n):
        x = 0
        y = 0
        k = (new_matrix != 0).sum(1)[i]
        if k == 0:
            k = 3
        elif k == 1:
            k = 2
        elif k == 2:
            k = 1
        else:
            k = 0
        if k != 0:
            for _ in range(k):
                minimum = matrix_weight[i][0]
                for j in range(n):
                    if i != j and new_matrix[i][j] == 0 and new_matrix[j][i] == 0 and (new_matrix != 0).sum(1)[
                        i] < 3 and (new_matrix != 0).sum(0)[j] < 3 and matrix_weight[i][j] <= minimum + 54:
                        minimum = matrix_weight[i][j]
                        x = i
                        y = j
                new_matrix[x][y] = minimum
                new_matrix[y][x] = minimum
                matrix_weight[x][y] = 1000000000000000
                matrix_weight[y][x] = 1000000000000000
        if i % 100 == 0:
            print("STEP:", i)

    for i in range(n):
        for j in range(n):
            if new_matrix[i][j] != 0 and new_matrix[i][j] == new_matrix[j][i]:
                new_matrix[j][i] = 0
    suma = int(np.sum(new_matrix))
    dlin_r = int(np.max(new_matrix))

    with open("../output/Kulikova_64_1.txt", "w", encoding="utf-8") as f:
        f.write(f"c Вес кубического подграфа = {suma}, самое длинное ребро = {dlin_r},\n")
        f.write(f"p edge {n} {int((new_matrix != 0).sum())}\n")
        for i in range(n):
            for j in range(n):
                if new_matrix[i][j] != 0:
                    f.write(f"e {i + 1} {j + 1}\n")
    print(np.sum(new_matrix))
    print(np.max(new_matrix))
    print((new_matrix != 0).sum())


main()
