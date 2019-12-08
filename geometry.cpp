#include "geometry.h"
#include <math.h>
#include "vector_shift.h"


static const int int_max = 1001;


Point::Point() : x (0), y (0) {}

Point::Point (double a, double b) : x (a), y (b) {}

Point::Point (const Point& p) : x (p.x), y (p.y) {}

// расстояние между точками
double Point::getDistance (const Point& p) const {
    return sqrt(pow((p.x - this->x), 2) + pow((p.y - this->y), 2));
}

Point Point::operator+=(const Point& p) {
    this->x += p.x;
    this->y += p.y;
    return *this;
}

Point& Point::operator=(const Point& p) {
    this->x = p.x;
    this->y = p.y;
    return *this;
}

Point operator+(const Point& p1, const Point& p2) {
    return Point (p1.x + p2.x, p1.y + p2.y);
}

Point operator-(const Point& p1, const Point& p2) {
    return Point (p1.x - p2.x, p1.y - p2.y);
}

double findCos (const Point& c, const Point& b, const Point& a) {
    double ab = a.getDistance(b);
    double ac = a.getDistance(c);
    double bc = b.getDistance(c);

    return (pow(bc, 2) + pow(ac, 2) - pow(ab, 2)) / (2 * bc * ac);
}

bool isSinMoreThanZero (const Point& a, const Point& b, const Point& c) {
    Point vector1 = c - b;
    Point vector2 = a - b;

    return (vector1.x * vector2.y - vector1.y * vector2.x) >= 0;
}


// Convex methods
ConvexHull::ConvexHull () : points (0) {}

void ConvexHull::addPoint (const Point& p) {
    points.push_back(p);
}

void ConvexHull::normalize () {
    int min_x = int_max;
    int min_y = int_max;
    int iter = 0;

    // ищем самую левую самую нижнюю
    for (int i = 0; i < points.size(); ++i) {
        if (min_x > points[i].x) {
            min_x = points[i].x;
            min_y = points[i].y;
            iter = i;
        }
        if (min_x == points[i].x && min_y > points[i].y) {
            min_y = points[i].y;
            iter = i;
        }
    }

    shiftLeft (points, ++iter);
}

ConvexHull makeCommonHull(ConvexHull& first_hull, ConvexHull& second_hull) {
    int this_ptr = first_hull.points.size() - 1;
    int hull_ptr = second_hull.points.size() - 1;

    ConvexHull cmn_hull;

    Point c_k (first_hull.points[this_ptr]);
    c_k += second_hull.points[hull_ptr]; // c_0 = a_0 + b_0
    cmn_hull.addPoint(Point(c_k));

    Point c_k_prev (c_k);
    c_k_prev.y += 1;

    // начинаем заворачивать подарок на основе двух готовых других
    while (hull_ptr != 0 && this_ptr != 0) {
        Point b_j = second_hull.points[hull_ptr]; // b_j
        Point a_i = first_hull.points[this_ptr]; // a_i
        Point b_j_next = second_hull.points[hull_ptr - 1]; // b_{j+1}
        Point a_i_next = first_hull.points[this_ptr - 1]; // a_{i+1}
        Point c_k_next1 = a_i + b_j_next;
        Point c_k_next2 = b_j + a_i_next;

        double cos1 = findCos (c_k, c_k_prev, c_k_next1);
        double cos2 = findCos (c_k, c_k_prev, c_k_next2);

        if (cos1 < cos2) { // => c_k_next1 подходит
            cmn_hull.addPoint(c_k_next1);
            --hull_ptr;
            c_k_prev = c_k;
            c_k = c_k_next1;
        } else {
            cmn_hull.addPoint(c_k_next2);
            --this_ptr;
            c_k_prev = c_k;
            c_k = c_k_next2;
        }
    }

    // добраем оставшиеся вершины
    if (hull_ptr == 0) {
        Point last_p = second_hull.points[0];
        while (this_ptr != 0) {
            cmn_hull.addPoint(last_p + first_hull.points[this_ptr - 1]);
            --this_ptr;
        }
    } else {
        Point last_p = first_hull.points[0];
        while (hull_ptr != 0) {
            cmn_hull.addPoint(last_p + second_hull.points[hull_ptr - 1]);
            --hull_ptr;
        }
    }

    return cmn_hull;
}

bool isPointRelatedToHull (ConvexHull& hull, Point& pnt) {
    for (int i = 0; i < hull.points.size() - 1; ++i) {
        if (!isSinMoreThanZero(hull.points[i + 1], pnt, hull.points[i])) {
            return false;
        }
    }
    return isSinMoreThanZero(hull.points[0], pnt, hull.points[hull.points.size() - 1]);
}