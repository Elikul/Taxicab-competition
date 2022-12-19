import numpy as np
import networkx as nx
import itertools


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


def findsubsets(S, m):
    return set(itertools.combinations(S, m))


def main():
    n, V = read_from_file("./input/Taxicab_64.txt")
    matrix_weight = count_weights(n, V)

    G = nx.MultiGraph(incoming_graph_data=matrix_weight)
    new_G = nx.maximum_spanning_tree(G)
    suma, ed = 0, 0
    for i, j, w in new_G.edges(data=True):
        suma += w['weight']
        ed += 1
    with open("./output/Kulikova_4096_1.txt", "w", encoding="utf-8") as f:
        f.write(f"c Вес подграфа = {int(suma)}\n")
        f.write(f"p edge {n} {ed}\n")
        for i, j in new_G.edges():
            f.write(f"e {i + 1} {j + 1}\n")


main()
