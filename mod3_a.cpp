/*
 * Даны два отрезка в пространстве (x1, y1, z1) - (x2, y2, z2) и (x3, y3, z3) - (x4, y4, z4).
 * Найдите расстояние между отрезками.
 *
 * */
#include <iostream>
#include <math.h>
#include <stdio.h>



struct Point {
    double x, y, z;

    Point (double a, double b, double c) : x (a), y (b), z (c) {}

    Point () : x (0), y (0), z (0) {}

    // расстояние между точками
    double getDistance(const Point& p) {
        return sqrt(pow((p.x - this->x), 2) + pow((p.y - this->y), 2) + pow((p.z - this->z), 2));
    }

    Point operator+=(const Point& p) {
        this->x += p.x;
        this->y += p.y;
        this->z += p.z;
        return *this;
    }

    Point operator/=(const double mult) {
        this->x /= mult;
        this->y /= mult;
        this->z /= mult;
        return *this;
    }

    double getDistanceToSegment(Point l, Point r, double eps = 0.0000001) {
        Point a, b;

        while (l.getDistance(r) > eps) {
            a = Point (l.x * 2 + r.x, l.y * 2 + r.y, l.z * 2 + r.z) /= 3;
            b = Point (l.x + r.x * 2, l.y + r.y * 2, l.z + r.z * 2) /= 3;

            if (this->getDistance(a) < this->getDistance(b)) {
                r = b;
            } else {
                l = a;
            }
        }

        return this->getDistance((l += r) /= 2);
    }
};


struct Segment {
    Point one, two;

    Segment (const Point& a, const Point& b) {
        one = a;
        two = b;
    }

    double getDistanceToSegment(const Segment& seg, const double eps = 0.0000001) {
        Point a, b;
        Point l = this->one;
        Point r = this->two;

        while (l.getDistance(r) > eps) {
            a = Point (l.x * 2 + r.x, l.y * 2 + r.y, l.z * 2 + r.z) /= 3;
            b = Point (l.x + r.x * 2, l.y + r.y * 2, l.z + r.z * 2) /= 3;

            if (a.getDistanceToSegment(seg.one, seg.two) < b.getDistanceToSegment(seg.one, seg.two)) {
                r = b;
            } else {
                l = a;
            }
        }

        return ((l += r) /= 2).getDistanceToSegment(seg.one, seg.two);
    }
};


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

    Segment seg (c, d);

    printf("%.9lf", Segment(a, b).getDistanceToSegment(seg));
    return 0;
}
