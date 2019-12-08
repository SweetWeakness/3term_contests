#ifndef INC_3TERM_CONTESTS_GEOMETRY_H
#define INC_3TERM_CONTESTS_GEOMETRY_H


#include <vector>
#include <iostream>

struct Point {
    double x, y;

    Point();

    Point (double a, double b);

    Point (const Point& p);

    // расстояние между точками
    double getDistance (const Point& p) const;

    Point operator+=(const Point& p);

    Point& operator=(const Point& p);
};


struct ConvexHull {
    std::vector<Point> points;

    ConvexHull ();

    void addPoint (const Point& p);

    void normalize ();
};


Point operator+(const Point& p1, const Point& p2);

Point operator-(const Point& p1, const Point& p2);

double findCos (const Point& c, const Point& b, const Point& a);

bool isSinMoreThanZero (const Point& a, const Point& b, const Point& c);

ConvexHull makeCommonHull(ConvexHull& first_hull, ConvexHull& second_hull);

bool isPointRelatedToHull (ConvexHull& hull, Point& pnt);


#endif //INC_3TERM_CONTESTS_GEOMETRY_H
