/*
 * Даны два отрезка в пространстве (x1, y1, z1) - (x2, y2, z2) и (x3, y3, z3) - (x4, y4, z4).
 * Найдите расстояние между отрезками.
 *
 */


#include <iostream>
#include <math.h>
#include <vector>


static const int int_max = 1001;


struct Point {
    double x, y;

    Point (double a, double b) : x (a), y (b) {}

    Point (const Point& p) : x (p.x), y (p.y) {}

    Point () : x (0), y (0) {}

    void update (double a, double b) {
        x = a;
        y = b;
    }

    // расстояние между точками
    double getDistance (const Point& p) const {
        return sqrt(pow((p.x - this->x), 2) + pow((p.y - this->y), 2));
    }

    Point operator+=(const Point& p) {
        this->x += p.x;
        this->y += p.y;
        return *this;
    }

    Point& operator=(const Point& p) {
        this->x = p.x;
        this->y = p.y;
        return *this;
    }

    void pr(){
        std::cout << x << " "<<y<<"\n";
    }
};


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

template <typename T>
void shiftLeft (std::vector<T>& arr, int k) {
    k %= arr.size();
    std::vector<T> buffer (arr.begin(), arr.begin() + k);
    for (int i = k; i < arr.size(); ++i) {
        arr[i - k] = arr[i];
    }
    for (int i = arr.size() - k; i < arr.size(); ++i) {
        arr[i] = buffer[i - arr.size() + k];
    }
}

struct ConvexHull {
    std::vector<Point> points;

    ConvexHull () : points (0) {}

    void addPoint (const Point& p) {
        points.push_back(p);
    }

    void normalize () {
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

    ConvexHull makeCommonHull(ConvexHull& hull) {
        int this_ptr = this->points.size() - 1;
        int hull_ptr = hull.points.size() - 1;

        ConvexHull cmn_hull;

        Point c_k (points[this_ptr]);
        c_k += hull.points[hull_ptr]; // c_0 = a_0 + b_0
        cmn_hull.addPoint(Point(c_k));

        Point c_k_prev (c_k);
        c_k_prev.y += 1;

        // начинаем заворачивать подарок на основе двух готовых других
        while (hull_ptr != 0 && this_ptr != 0) {
            Point b_j = hull.points[hull_ptr]; // b_j
            Point a_i = this->points[this_ptr]; // a_i
            Point b_j_next = hull.points[hull_ptr - 1]; // b_{j+1}
            Point a_i_next = this->points[this_ptr - 1]; // a_{i+1}
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
            Point last_p = hull.points[0];
            while (this_ptr != 0) {
                cmn_hull.addPoint(last_p + this->points[this_ptr - 1]);
                --this_ptr;
            }
        } else {
            Point last_p = this->points[0];
            while (hull_ptr != 0) {
                cmn_hull.addPoint(last_p + hull.points[hull_ptr - 1]);
                --hull_ptr;
            }
        }

        return cmn_hull;
    }

    bool isPointRelated (Point& pnt) {
        for (int i = 0; i < points.size() - 1; ++i) {
            if (!isSinMoreThanZero(points[i + 1], pnt, points[i])) {
                return false;
            }
        }
        return isSinMoreThanZero(points[0], pnt, points[points.size() - 1]);
    }

    void pr(){
        for(int i=0;i<points.size();++i){
            std::cout <<points[i].x << " " << points[i].y <<"\n";
        }
    }
};


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
    //set1.pr();
    //std::cout << "\n";
    //set2.pr();
    //std::cout << "\n";


    ConvexHull set = set1.makeCommonHull(set2);
    //set.pr();
    Point zero_p (0, 0);
    std::cout << (set.isPointRelated(zero_p) ? "YES" : "NO");

    return 0;
}
