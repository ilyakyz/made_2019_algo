#include "Point.hpp"
#include <cmath>

std::vector<Point> gen_points(size_t count) {
    std::vector<Point> result;
    result.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        float s = 0;
        float x = 0;
        float y = 0;
        while (s == 0 or s > 1) {
            x = -1. + static_cast<float>(rand()) / static_cast<float>(RAND_MAX/2);
            y = -1. + static_cast<float>(rand()) / static_cast<float>(RAND_MAX/2);
            s = x * x + y * y;
        }
        float z0 = x * sqrt(-2 * log(s) / s);
        float z1 = y * sqrt(-2 * log(s) / s);
        result.push_back({z0, z1});
    }
    return result;
}

float distance(const Point& p1, const Point& p2) {
    float dx = p1.x  - p2.x;
    float dy = p1.y  - p2.y;
    return sqrt(dx * dx + dy * dy);
}
