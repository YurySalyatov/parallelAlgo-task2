#include <tuple>

std::tuple<int, int, int> getXYZ(const int i, const int size) {
    int x = i % size;
    int y = i / size % size;
    int z = i / (size * size);
    return {x, y, z};
}

int getI(const int x, const int y, const int z, const int size) {
    return x + size * y + size * size * z;
}


std::vector<std::vector<int> > generate_graph(const int size) {
    std::vector<std::vector<int> > res(size * size * size);
    for (int i = 0; i < size * size * size; i++) {
        auto [x, y, z] = getXYZ(i, size);
        if (x > 0) {
            res[i].push_back(getI(x - 1, y, z, size));
        }
        if (x < size - 1) {
            res[i].push_back(getI(x + 1, y, z, size));
        }
        if (y > 0) {
            res[i].push_back(getI(x, y - 1, z, size));
        }
        if (y < size - 1) {
            res[i].push_back(getI(x, y + 1, z, size));
        }
        if (z > 0) {
            res[i].push_back(getI(x, y, z - 1, size));
        }
        if (z < size - 1) {
            res[i].push_back(getI(x, y, z + 1, size));
        }
    }
    return res;
}