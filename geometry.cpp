#include "geometry.h"
#include <math.h>
#include "ternary_search.h"

Point::Point(double a, double b, double c) : x (a), y (b), z (c) {}

Point::Point() : x (0), y (0), z (0) {}

Segment::Segment (const Point& a, const Point& b) {
    one = a;
    two = b;
}


// расстояние между точками
const double getDistance(const Point& one, const Point& two) {
    return sqrt(pow((one.x - two.x), 2) + pow((one.y - two.y), 2) + pow((one.z - two.z), 2));
}

Point operator+(const Point& one, const Point& two) {
    return Point(one.x + two.x, one.y + two.y, one.z + two.z);
}

Point operator*(const Point& p, double mult) {
    return Point(p.x * mult, p.y * mult, p.z * mult);
}

Point operator/(const Point& p, double mult) {
    return Point(p.x / mult, p.y / mult, p.z / mult);
}

double getDistancePointToSegment(const Point& p, const Segment& seg, double eps) {
    auto func = [&p](const Point& p_to) {
        return getDistance(p_to, p);
    };
    // 1ый аргумент - точка, до которой кратчайшее растояние от p, 2ой - p
    Point least_p_in_seg = ternarySearchMin(seg.one, seg.two, func, getDistance, eps);
    return getDistance(least_p_in_seg, p);
}


double getDistanceSegmentToSegment(const Segment& seg1, const Segment& seg2, double eps) {
    auto func = [&seg2](const Point& p_to) {
        return getDistancePointToSegment(p_to, seg2);
    };
    // 1ый аргумент - точка, до которой кратчайшее растояние от p, 2ой - p
    Point x = ternarySearchMin(seg1.one, seg1.two, func, getDistance, eps);
    return getDistancePointToSegment(x, seg2);
}
