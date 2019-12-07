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


int calc_next_pref (std::vector<size_t>& pi, int pref_prev, std::string& pattern, char next_symbol) {
    while (pref_prev > 0 && pattern[pref_prev] != next_symbol) {
        pref_prev = pi[pref_prev - 1];
    }
    if (pattern[pref_prev] == next_symbol) {
        ++pref_prev;
    }

    return pref_prev;
}

// Compute Prefix Function
// O(p) mem
std::vector<size_t> cmp (std::string& pattern) {
    std::vector<size_t> pi(pattern.size(), 0);
    pi[0] = 0;

    for (size_t i = 1; i < pattern.length(); ++i) {
        pi[i] = calc_next_pref(pi, pi[i - 1], pattern, pattern[i]);
    }

    return pi;
}

// Knuth - Morris - Pratt algorithm
void kmp_matcher (std::string& pattern) {
    std::vector<size_t> pi = cmp(pattern);
    size_t tmp_pi = 0;
    std::istream_iterator<char> read_it (std::cin);
    std::istream_iterator<char> eos;

    for (size_t iter = 0; read_it != std::istream_iterator<char>() && read_it != eos; ++read_it, ++iter) {
        tmp_pi = calc_next_pref(pi, tmp_pi, pattern, *read_it);
        if (tmp_pi == pattern.size()) {
            std::cout << iter + 1 - pattern.size() << " ";
            tmp_pi = pi[tmp_pi - 1];
        }
    }
}

int main() {
    std::string p;
    std::cin >> p;

    kmp_matcher(p);

    return 0;
}