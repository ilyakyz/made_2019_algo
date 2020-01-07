#ifndef Point_hpp
#define Point_hpp
#include <vector>

struct Point {
    float x;
    float y;
};

std::vector<Point> gen_points(size_t count);

float distance(const Point& p1, const Point& p2);

#endif /* Point_hpp */
