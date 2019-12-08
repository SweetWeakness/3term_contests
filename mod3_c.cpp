/*
 * Даны два выпуклых многоугольника на плоскости. В первом n точек, во втором m.
 * Определите, пересекаются ли они за O(n + m). Указание. Используйте сумму Минковского.
 *
 */


#include <iostream>
#include <math.h>
#include <vector>
#include "geometry.h"


int main() {
    double x, y;
    int n;
    ConvexHull set1, set2;

    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::cin >> x >> y;
        Point read_p (x, y);
        set1.addPoint(read_p);
    }
    set1.normalize();

    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::cin >> x >> y;
        Point read_p (-x, -y);
        set2.addPoint(read_p);
    }
    set2.normalize();



    ConvexHull set = makeCommonHull(set1, set2);
    Point zero_p (0, 0);
    std::cout << (isPointRelatedToHull(set, zero_p) ? "YES" : "NO");

    return 0;
}
