#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include "DSU.hpp"
#include "Edge.hpp"
#include "appox_path.hpp"
#include "Point.hpp"


float path_len(const std::vector<int>& path, const std::vector<std::vector<float>>& distances) {
    float cur_len = distances[path.front()][path.back()];
    for (size_t i = 0; i+1 < path.size(); ++i) {
        cur_len += distances[path[i]][path[i+1]];
    }
    return cur_len;
}

float calc_optimal_len(const std::vector<std::vector<float>>& distances) {
    float result = std::numeric_limits<float>::max();
    std::vector<int> path(distances.size());
    for (int i = 0; i < distances.size(); ++i)
        path[i] = i;
    do {
        float cur_len = path_len(path, distances);
        result = std::min(result, cur_len);
    } while (std::next_permutation(path.begin(), path.end()));
    return result;
}

float approx_error(size_t N) {
    std::vector<Point> points = gen_points(N);
    
    std::vector<Edge> edges;
    std::vector<std::vector<float>> distances(N, std::vector<float>(N, 0));
    for (int i = 0; i < N; ++i) {
        for (int j = i+1; j < N; ++j) {
            float dist = distance(points[i], points[j]);
            distances[i][j] = dist;
            distances[j][i] = dist;
            edges.push_back({i, j, dist});
        }
    }
    
    std::vector<int> approx_path = traversal_path(N, edges);
    float approx_len = path_len(approx_path, distances);
    float optimal_len = calc_optimal_len(distances);
    return (approx_len - optimal_len) / optimal_len;
}

void experiment(size_t N) {
    size_t exp_count = 10;
    
    std::vector<float> errors;
    for (size_t i = 0; i < exp_count; ++i) {
        errors.push_back(approx_error(N));
    }
    float sum = 0;
    float sum_sq = 0;
    for (float e: errors) {
        sum += e;
        sum_sq += e * e;
    }
    
    float mean = sum / exp_count;
    float std = sqrt(sum_sq / exp_count - mean * mean);
    std::cout << std::setw(3) << N << ' ' << std::setprecision(3) << std::setw(8) << mean << ' '
        << std::setw(8) << std << '\n';
}


int main(int argc, const char * argv[]) {
    for (size_t i = 2; i <= 10; ++i) {
        experiment(i);
    }

    return 0;
}
