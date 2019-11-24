/*
 * Найдите все вхождения шаблона в строку. Длина шаблона – p, длина строки – n. Время O(n + p), доп. память – O(p).
p <= 30000, n <= 300000.
Использовать один из методов:
- С помощью префикс-функции;
- С помощью z-функции.
 */

#include <iostream>
#include <vector>
#include <iterator>


// Compute Prefix Function
// O(p) mem
std::vector<size_t> CMP (std::string& pattern) {
    std::vector<size_t> pi(pattern.size(), 0);
    pi[0] = 0;
    size_t tmp_pi = 0;

    for (size_t i = 1; i < pattern.length(); ++i) {
        while (tmp_pi > 0 && pattern[tmp_pi] != pattern[i]) {
            tmp_pi = pi[tmp_pi - 1];
        }
        if (pattern[tmp_pi] == pattern[i]) {
            ++tmp_pi;
        }
        pi[i] = tmp_pi;
    }

    return pi;
}

// Knuth - Morris - Pratt algorithm
void KMP_Matcher (std::string& pattern) {
    std::vector<size_t> pi = CMP(pattern);
    size_t tmp_pi = 0;
    std::istream_iterator<char> read_it (std::cin);

    for (size_t iter = 0; read_it != std::istream_iterator<char>(); ++read_it, ++iter) {
        while (tmp_pi > 0 && pattern[tmp_pi] != *read_it) {
            tmp_pi = pi[tmp_pi - 1];
        }
        if (pattern[tmp_pi] == *read_it){
            ++tmp_pi;
        }
        if (tmp_pi == pattern.size()) {
            std::cout << iter + 1 - pattern.size() << " ";
            tmp_pi = pi[tmp_pi - 1];
        }
    }
}

int main() {
    std::string p;
    std::cin >> p;

    KMP_Matcher(p);

    return 0;
}
