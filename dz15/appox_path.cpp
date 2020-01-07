#include "appox_path.hpp"
#include "DSU.hpp"
#include <stack>

std::vector<Edge> min_tree(size_t n, std::vector<Edge>& edges) {
    std::sort(edges.begin(), edges.end(),
              [](const Edge& first, const Edge& second) {return first.weight < second.weight;});
 
    DSU dsu(n);
    
    std::vector<Edge> result;
    for (const Edge& e : edges) {
        if (dsu.find(e.begin) != dsu.find(e.end)) {
            result.push_back(e);
            dsu.merge(e.begin, e.end);
        }
    }
    return result;
}

struct DFSState {
    int vertex;
    int idx;
    int from;
};

std::vector<int> traversal_path(size_t n, std::vector<Edge>& edges) {
    std::vector<Edge> tree = min_tree(n, edges);
    
    std::vector<std::vector<int>> adjacency(n);
    for (const Edge& edge: tree) {
        adjacency[edge.begin].push_back(edge.end);
        adjacency[edge.end].push_back(edge.begin);
    }
    std::vector<bool> used(n);
    
    std::vector<int> result;
    std::stack<DFSState> stack;
    int vertex = 0;
    int idx = -1;
    int from = -1;
    
    while (true) {
        if (!used[vertex]) {
            result.push_back(vertex);
            used[vertex] = true;
        }
        std::vector<int>& cur_adjacency = adjacency[vertex];
        ++idx;
        
        if (idx < cur_adjacency.size() && cur_adjacency[idx] == from)
            ++idx;
        
        if (idx < cur_adjacency.size()) {
            stack.push({vertex, idx, from});
            from = vertex;
            vertex = cur_adjacency[idx];
            idx = -1;
            
            continue;
        }
        if (stack.empty()) {
            break;
        }
        DFSState state = stack.top();
        vertex = state.vertex;
        idx = state.idx;
        from = state.from;
        stack.pop();
    }

    return result;
}
