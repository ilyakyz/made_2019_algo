#ifndef appox_path_hpp
#define appox_path_hpp

#include <stdio.h>
#include <vector>
#include "Edge.hpp"

std::vector<Edge> min_tree(size_t n, std::vector<Edge>& edges);

std::vector<int> traversal_path(size_t n, std::vector<Edge>& edges);

#endif /* appox_path_hpp */
