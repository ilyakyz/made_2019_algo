/*
 11_1. Цикл минимальной длины
 Дан невзвешенный неориентированный граф. Найдите цикл минимальной длины.
 Ввод: v: кол-во вершин (макс. 50000), n: кол-во ребер (макс. 200000), n пар реберных вершин.
 Вывод: одно целое число равное длине минимального цикла. Если цикла нет, то вывести -1.
 */

#include <iostream>
#include <vector>
#include <queue>

typedef std::vector<std::vector<size_t>> Graph;

struct NodeTrack {
    size_t node;
    size_t from;
};

size_t cycle_length(size_t start, const Graph& graph) {
    std::queue<NodeTrack> queue;
    queue.push({start, 0});
    
    std::vector<bool> visited(graph.size(), false);
    visited[start] = 0;
    
    std::vector<size_t> distances(graph.size());
    
    while (!queue.empty()) {
        NodeTrack cur_node = queue.front();
        queue.pop();
        
        for (size_t child: graph[cur_node.node]) {
            if (child == cur_node.from)
                continue;
            
            if (visited[child]) {
                return distances[cur_node.node] + distances[child] + 1;
            }
            
            queue.push({child, cur_node.node});
            visited[child] = true;
            distances[child] = distances[cur_node.node] + 1;
        }
        
    }
    return -1;
}

size_t min_cycle(const Graph& graph) {
    size_t min_cycle = -1;
    for (size_t i = 0; i < graph.size(); ++i) {
        min_cycle = std::min(cycle_length(i, graph), min_cycle);
    }
    return min_cycle;
}

int main(int argc, const char * argv[]) {
    size_t v, n;
    std::cin >> n;
    std::cin >> v;
    
    Graph graph(n);
    
    for (size_t i = 0; i < v; ++i) {
        size_t first, second;
        std::cin >> first >> second;
        graph[first].push_back(second);
        graph[second].push_back(first);
    }
    
    std::cout << int(min_cycle(graph));
    return 0;
}
