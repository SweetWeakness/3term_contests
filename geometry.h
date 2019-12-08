#ifndef INC_3TERM_CONTESTS_GEOMETRY_H
#define INC_3TERM_CONTESTS_GEOMETRY_H


struct Point {
    double x, y, z;

    Point (double a, double b, double c);

    Point ();
};

struct Segment {
    Point one, two;

    Segment (const Point& a, const Point& b);
};


// расстояние между точками
const double getDistance(const Point& one, const Point& two);

Point operator+(const Point& one, const Point& two);

Point operator*(const Point& p, double mult);

Point operator/(const Point& p, double mult);

double getDistancePointToSegment(const Point& p, const Segment& seg, double eps = 0.0000001);

double getDistanceSegmentToSegment(const Segment& seg1, const Segment& seg2, double eps = 0.0000001);


#endif //INC_3TERM_CONTESTS_GEOMETRY_H
