/*
 * Найдите все вхождения шаблона в строку. Длина шаблона – p, длина строки – n. Время O(n + p), доп. память – O(p).
p <= 30000, n <= 300000.
Использовать один из методов:
- С помощью префикс-функции;
- С помощью z-функции.
 */

#include <iostream>
#include <vector>


// Compute Prefix Function
// O(p) mem
std::vector<int> CMP (std::string& p) {
    int m = p.length();
    std::vector<int> pi(m, 0);
    pi[0] = 0;
    int k = 0;

    for (int q = 1; q < m; ++q) {
        while (k > 0 && pi[k] != pi[q]) {
            k = pi[k - 1];
        }
        if (p[k] == p[q]) {
            ++k;
        }
        pi[q] = k;
    }

    return pi;
}

// Knuth - Morris - Pratt algorithm
void KMP_Matcher (std::string &t, std::string& p) {
    int n = t.length();
    int m = p.length();
    std::vector<int> pi = CMP(p);

    int q = 0;

    for (int i = 0; i < n; ++i) {
        while (q > 0 && p[q] != t[i]) {
            q = pi[q - 1];
        }
        if (p[q] == t[i]){
            ++q;
        }
        if (q == m) {
            std:: cout << i - m + 1 <<" ";
            q = pi[q - 1];
        }
    }
}

int main() {
    std::string s, p;
    std::cin >> p;
    std::cin >> s;

    KMP_Matcher(s, p);

    return 0;
}
