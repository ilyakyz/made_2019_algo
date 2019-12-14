/*
 Дан неориентированный связный граф. Требуется найти вес минимального остовного дерева в этом графе.
 */

#include <iostream>
#include <vector>
#include <algorithm>

class DSU {
 public:
  explicit DSU(size_t size);

  int find(int element);
  void merge(int left, int right);
  void print() const;

 private:
  std::vector<int> parent;
  std::vector<int> rank;
};

DSU::DSU(size_t size) :
    parent(size, -1),
    rank(size, 1) {}

int DSU::find(int element) {
  if (parent[element] == -1) {
    return element;
  }
  return parent[element] = find(parent[element]);
}

void DSU::merge(int left, int right) {
  const int deputat_left = find(left);
  const int deputat_right = find(right);
  if (rank[deputat_left] == rank[deputat_right]) {
    parent[deputat_right] = deputat_left;
    ++rank[deputat_left];
  } else if(rank[deputat_left] > rank[deputat_right]) {
    parent[deputat_right] = deputat_left;
  } else {
    parent[deputat_left] = deputat_right;
  }
}

void DSU::print() const {
  for (const auto parent_element : parent) {
    std::cout << parent_element << " ";
  }
  std::cout << std::endl;
}

struct Edge {
    int begin;
    int end;
    int weight;
};

int min_tree_weight(size_t n, std::vector<Edge>& edges) {
    std::sort(edges.begin(), edges.end(),
              [](const Edge& first, const Edge& second) {return first.weight < second.weight;});
    
    int weight = 0;
 
    DSU dsu(n);
    
    for (const Edge& e : edges) {
        if (dsu.find(e.begin) != dsu.find(e.end)) {
            weight += e.weight;
            dsu.merge(e.begin, e.end);
        }
    }
    return weight;
}

int main(int argc, const char * argv[]) {
    size_t n, m;
    std::cin >> n >> m;
    
    std::vector<Edge> edges(m);
    for (size_t i = 0; i < m; ++i) {
        int vertex;
        std::cin >> vertex;
        edges[i].begin = vertex - 1;
        std::cin >> vertex;
        edges[i].end = vertex - 1;
        std::cin >> edges[i].weight;
    }

    std::cout << min_tree_weight(n, edges);
    return 0;
}
