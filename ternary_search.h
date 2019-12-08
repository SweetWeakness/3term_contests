#ifndef INC_3TERM_CONTESTS_TERNARY_SEARCH_H
#define INC_3TERM_CONTESTS_TERNARY_SEARCH_H


template <typename T, class Func>
T ternarySearchMin(T left, T right, Func f, const double (*getDistance)(const T&, const T&), double eps = 0.0000001) {
    while (getDistance(left, right) > eps) {
        T a = (left * 2 + right) / 3;
        T b = (left + right * 2) / 3;
        if (f(a) < f(b)) {
            right = b;
        } else {
            left = a;
        }
    }
    return (left + right) / 2;
}


#endif //INC_3TERM_CONTESTS_TERNARY_SEARCH_H
