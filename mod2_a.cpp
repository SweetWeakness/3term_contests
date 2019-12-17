/*
 * Дана строка длины n. Найти количество ее различных подстрок. Используйте суффиксный массив.
 * Построение суффиксного массива выполняйте за O(n log n).
 * Вычисление количества различных подстрок выполняйте за O(n).
*/


#include <iostream>
#include <vector>
#include <suffix_array.h>
#include <lcp_array.h>


constexpr size_t ALPHABET = 256;


size_t find_substrings(std::string& s){
    s += "$";
    std::vector<size_t> p;
    std::vector<int> lcp;

    p = make_suff_array(s);
    lcp = make_lcp_array(s, p);

    size_t ans = 0;
    for (size_t i = 0; i < s.size(); ++i) {
        ans += s.size() - 1 - p[i] - lcp[i];
    }

    return --ans;
}

int main() {
    std::string s;
    std::cin >> s;

    std::cout << find_substrings(s);

    return 0;
}
