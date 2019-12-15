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


using std::vector;
using std::string;
using std::istream_iterator;
using std::ostream_iterator;


int calc_next_pref (vector<size_t>& pi, int pref_prev, string& pattern, char next_symbol) {
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
vector<size_t> cmp (string& pattern) {
    vector<size_t> pi(pattern.size(), 0);
    pi[0] = 0;

    for (size_t i = 1; i < pattern.length(); ++i) {
        pi[i] = calc_next_pref(pi, pi[i - 1], pattern, pattern[i]);
    }

    return pi;
}

// Knuth - Morris - Pratt algorithm
void kmp_matcher (string& pattern, istream_iterator<char>& read_it, ostream_iterator<int>& out_it) {
    vector<size_t> pi = cmp(pattern);
    size_t tmp_pi = 0;


    for (size_t iter = 0; read_it != istream_iterator<char>(); ++read_it, ++iter, ++out_it) {
        tmp_pi = calc_next_pref(pi, tmp_pi, pattern, *read_it);
        if (tmp_pi == pattern.size()) {
            *out_it = iter + 1 - pattern.size();
            std::cin.tie(nullptr);
            tmp_pi = pi[tmp_pi - 1];
        }
    }
}

int main() {
    string p;
    std::cin >> p;

    istream_iterator<char> read_it (std::cin);
    ostream_iterator<int> out_it (std::cout, " ");

    kmp_matcher(p, read_it, out_it);

    return 0;
}
