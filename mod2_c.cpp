/*
 * Заданы две строки s, t и целое число k.
 * Рассмотрим множество всех таких непустых строк, которые встречаются как подстроки в s и t одновременно.
 * Найдите k-ую в лексикографическом порядке строку из этого множества.
 * Полезная статья про сравнение указателей: https://stackoverflow.com/questions/9086372/how-to-compare-pointers.
*/


#include <iostream>
#include <vector>
#include <suffix_array.h>
#include <lcp_array.h>


constexpr size_t ALPHABET = 256;


bool is_changed (const std::vector<size_t>& p, const size_t i, const size_t pivot) {
    return (p[i] < pivot && p[i - 1] > pivot) ||
           (p[i] > pivot && p[i - 1] < pivot);
}


std::string find_k_substring(std::string& s, const std::string& l, int64_t k){
    const size_t old_s_size = s.size();
    s = s + "#" + l + "$";

    std::vector<size_t > p;
    std::vector<int> lcp;
    p = make_suff_array(s);
    lcp = make_lcp_array(s, p);

    size_t lcp_old = 0;
    std::string ans;

    for (size_t i = 1; i < s.size(); ++i){
        if (is_changed(p, i, old_s_size)) {
            if (lcp_old + k > lcp[i]) {
                if (lcp[i] >= lcp_old) {
                    k -= lcp[i] - lcp_old;
                }
                lcp_old = lcp[i];

            }else{
                for (size_t j = p[i]; j < lcp_old + k + p[i]; ++j) {
                    std::cout << s[j];
                }

                k = 0;
                break;
            }
        }
        if (lcp[i] < lcp_old) {
            lcp_old = lcp[i];
        }
    }

    if (k > 0) {
        ans = "-1";
    }

    return ans;
}

int main() {
    std::string s, l;
    int64_t k;
    std::cin >> s;
    std::cin >> l;
    std::cin >> k;

    std::cout << find_k_substring(s, l, k);

    return 0;
}
