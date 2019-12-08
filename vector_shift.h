#ifndef INC_3TERM_CONTESTS_VECTOR_SHIFT_H
#define INC_3TERM_CONTESTS_VECTOR_SHIFT_H


#include <vector>

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


#endif //INC_3TERM_CONTESTS_VECTOR_SHIFT_H
