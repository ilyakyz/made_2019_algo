/*
Ребро неориентированного графа называется мостом, если удаление этого ребра из графа увеличивает число компонент связности.
Дан неориентированный граф, требуется найти в нем все мосты.
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <stack>

struct OutEdge {
    size_t to;
    size_t edge_number;
};

struct DFSPosition {
    size_t vertex;
    size_t from;
    size_t idx;
};

typedef std::vector<std::vector<OutEdge>> Graph;

class BridgesFinder {
public:
    BridgesFinder(const Graph& graph):
        graph(graph),
        used(graph.size()),
        entry(graph.size()),
        lowest(graph.size())
    {
    }
    
    const std::vector<size_t>& find_bridges() {
        for (size_t vertex = 0; vertex < graph.size(); ++vertex) {
            if (!used[vertex]) {
                dfs(vertex);
            }
        }
        return bridges;
    }
    
private:
    void dfs(size_t vertex) {
        size_t cur_time = 0;
        std::stack<DFSPosition> back_stack;
        size_t idx = -1;
        size_t from = -1;
        while (true) {
            if (idx == -1) {
                used[vertex] = true;
                entry[vertex] = lowest[vertex] = cur_time++;
                idx = 0;
            }
            
            const std::vector<OutEdge>& edges = graph[vertex];
            for (; idx < edges.size(); ++idx) {
                const OutEdge& out_edge = edges[idx];
                size_t next = out_edge.to;
                if (out_edge.edge_number == from)
                    continue;
                if (used[next]) {
                    lowest[vertex] = std::min(lowest[vertex], entry[next]);
                } else {
                    back_stack.push({vertex, from, idx});
                    from = out_edge.edge_number;
                    vertex = next;
                    idx = -1;
                    break;
                }
            }
            if (idx == -1) {
                continue;
            }
                
            
            if (back_stack.empty())
                break;
                
            DFSPosition ret_position = back_stack.top();
            back_stack.pop();
            
            size_t prev_vertex = vertex;
            vertex = ret_position.vertex;
            from = ret_position.from;
            idx = ret_position.idx;
            
            lowest[vertex] = std::min(lowest[vertex], lowest[prev_vertex]);
            if (lowest[prev_vertex] > entry[vertex]) {
                bridges.push_back(graph[vertex][idx].edge_number);
            }
            ++idx;
            
        }
    }

    Graph graph;
    std::vector<size_t> bridges;
    std::vector<size_t> used;
    std::vector<size_t> entry;
    std::vector<size_t> lowest;
};

int main(int argc, const char * argv[]) {
    std::ifstream in("bridges.in");
    std::ofstream out("bridges.out");
    
    size_t n, v;
    in >> n;
    in >> v;
    
    Graph graph(n);
    
    for (size_t i = 0; i < v; ++i) {
        size_t first, second;
        in >> first >> second;
        --first;
        --second;
        graph[first].push_back({second, i+1});
        graph[second].push_back({first, i+1});
    }
    
    std::vector<size_t> bridges = BridgesFinder(graph).find_bridges();
    std::sort(bridges.begin(), bridges.end());
    out << bridges.size() << '\n';
    for (size_t bridge: bridges) {
        out << bridge << ' ';
    }
    
    return 0;
}
