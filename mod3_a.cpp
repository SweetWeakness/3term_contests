/*
 * Даны два отрезка в пространстве (x1, y1, z1) - (x2, y2, z2) и (x3, y3, z3) - (x4, y4, z4).
 * Найдите расстояние между отрезками.
 *
 * */


#include <iostream>
#include <math.h>
#include <stdio.h>
#include "geometry.h"


int main() {
    int x, y, z;

    std::cin >> x >> y >> z;
    Point a (x, y, z);
    std::cin >> x >> y >> z;
    Point b (x, y, z);
    std::cin >> x >> y >> z;
    Point c (x, y, z);
    std::cin >> x >> y >> z;
    Point d (x, y, z);

    Segment seg1 (c, d);
    Segment seg2 (a, b);

    printf("%.9lf", getDistanceSegmentToSegment(seg1, seg2));
    return 0;
}
