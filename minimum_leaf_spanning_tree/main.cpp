#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>

int cost = 0;
std::vector<std::pair<int, int>> v;
std::vector<std::vector<std::pair<int, int>>> sectors;
std::vector<std::pair<int, std::pair<int, int>>> ribs;

int dist(std::pair<int, int> &v1, std::pair<int, int> &v2) {
    return abs(v1.first - v2.first) + abs(v1.second - v2.second);
}

void addRib(int &ind_v1, int &ind_v2) {
    ribs.emplace_back();
    ribs.back().second.first = ind_v1;
    ribs.back().second.second = ind_v2;
    ribs.back().first = dist(v[ind_v1], v[ind_v2]);
    cost += ribs.back().first;
}

int moveLastVFromSector(int initIndV, int indSector) {
    int result = sectors[indSector].back().second;
    addRib(initIndV, sectors[indSector].back().second);
    sectors[indSector].pop_back();
    return result;
}

void f(int initIndV, int indBeginSector, int indEndSector) {
    if (indBeginSector == indEndSector) {
        int predInd = initIndV;
        while (!sectors[indBeginSector].empty()) {
            predInd = moveLastVFromSector(predInd, indBeginSector);
        }
        return;
    }
    int indSectorMin = -1;
    for (int i = indBeginSector; i <= indEndSector; i++) {
        if (indSectorMin != -1) {
            if (sectors[i].empty())
                continue;
            if (dist(v[initIndV], v[sectors[i].back().second])
                < dist(v[initIndV], v[sectors[indSectorMin].back().second]))
                indSectorMin = i;

        } else {
            if (!sectors[i].empty())
                indSectorMin = i;
        }
    }

    if (indSectorMin == -1)
        return;
    initIndV = moveLastVFromSector(initIndV, indSectorMin);

    f(initIndV, indBeginSector, (indBeginSector + indEndSector) / 2);
    f(initIndV, (indBeginSector + indEndSector) / 2 + 1, indEndSector);
}

void fMain(int indVCenter, int l, double phiSector) {
    cost = 0;
    ribs.clear();
    for (auto &s: sectors)
        s.clear();

    for (int j = 0; j < v.size(); j++) {
        if (j == indVCenter)
            continue;
        double phiV;
        int x = v[j].first - v[indVCenter].first, y = v[j].second - v[indVCenter].second;
        if (x == 0) {
            if (y > 0)
                phiV = M_PI / 2;
            else
                phiV = 3 * M_PI / 2;
        } else if (x < 0) {
            phiV = std::atan(double(y) / double(x)) + M_PI;
        } else {
            phiV = std::atan(double(y) / double(x));
        }
        if (phiV < 0) {
            phiV += M_PI * 2;
        }
        auto indSector = static_cast<int>(phiV / phiSector);
        sectors[indSector].push_back(std::pair<int, int>(dist(v[j], v[indVCenter]), j));
    }

    for (auto &sector: sectors) {
        std::sort(sector.rbegin(), sector.rend());
    }

    int stepTree = l / 3;
    for (int j = 0; j < l; j += stepTree) {
        f(indVCenter, j, j + stepTree - 1);
    }
}


int main() {
    std::fstream in;
    in.open("../input/Taxicab_64.txt");
    int n;

    in >> n;
    v.emplace_back();
    in >> v.back().first >> v.back().second;
    for (int i = 1; i < n; i++) {
        v.emplace_back();
        in >> v.back().first >> v.back().second;
    }
    in.close();

    int lMax = n / 16, l = 3;
    while (l < lMax) {
        l <<= 1;
    }
    l >>= 1;
    sectors.resize(static_cast<unsigned long>(l));

    double phiSector = 2 * M_PI / l;

    fMain(0, l, phiSector);
    int costMin = cost, indVMin = 0;

    for (int j = 1; j < v.size(); ++j) {
        fMain(j, l, phiSector);
        if (cost < costMin) {
            costMin = cost;
            indVMin = j;
        }
    }

    fMain(indVMin, l, phiSector);

    std::vector<int> powV(v.size(), 0);
    for (auto &i: ribs) {
        powV[i.second.first] += 1;
        powV[i.second.second] += 1;
    }
    int count_leaf = 0;
    for (auto &i: powV) {
        if (i == 1)
            count_leaf += 1;
    }


    std::fstream result("../output/Kulikova_64_1.txt");
    result << "c Вес дерева = " << cost << ", число листьев = " << count_leaf << ',' << std::endl;
    result << "p edge " << n << ' ' << ribs.size() << std::endl;
    for (auto &rib: ribs) {
        result << "e " << rib.second.first + 1 << ' ' << rib.second.second + 1 << std::endl;
    }
    std::cout << "stop" << std::endl;

    return 0;
}
