/*
 Дано множество точек на плоскости (x, y). Постройте выпуклую оболочку этого множества и вычислите ее периметр.
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

struct Point {
    double x;
    double y;
};

double cross_product(const Point& start, const Point& p1, const Point& p2) {
   double y1 = p1.y - start.y;
   double y2 = p2.y - start.y;
   double x1 = p1.x - start.x;
   double x2 = p2.x - start.x;
   return y2 * x1 - y1 * x2;
}

double distance(const Point& p1, const Point& p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return sqrt(dx * dx + dy * dy);
}

std::vector<Point> find_hull(const std::vector<Point>& points) {
    Point start = points.front();
    for (const Point& point: points) {
        if (point.y < start.y) {
            start = point;
        } else if (point.y == start.y && point.x < start.x) {
            start = point;
        }
    }
    
    std::vector<Point> result;
    Point current = start;
    result.push_back(current);
    
    while (true) {
        const Point* next = nullptr;
        for (const Point& point: points) {
            if (point.x == current.x && point.y == current.y)
                continue;
            if (next == nullptr) {
                next = &point;
                continue;
            }
            double prod = cross_product(current, *next, point);
            if (prod < -1e-15) {
                next = &point;
            } else if (prod < 1e-15) {
                if (distance(current, point) > distance(current, *next)) {
                    next = &point;
                }
            }
        }
        current = *next;
        if (current.x == start.x && current.y == start.y)
            break;
        result.push_back(current);
    }
    
    return result;
}

double calc_perimeter(const std::vector<Point>& hull) {
    double perimeter = 0;
    const Point* prev = &hull.back();
    for (const Point& point: hull) {
        perimeter += distance(point, *prev);
        prev = &point;
    }
    return perimeter;
}

int main(int argc, const char * argv[]) {
    size_t n;
    std::cin >> n;
    
    std::vector<Point> points;
    points.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        double x, y;
        std::cin >> x >> y;
        points.push_back({x, y});
    }
    
    double perimeter = calc_perimeter(find_hull(points));

    std::cout << std::setprecision(9) << perimeter << std::endl;
    
    return 0;
}
