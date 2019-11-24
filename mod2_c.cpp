/*
 * Заданы две строки s, t и целое число k.
 * Рассмотрим множество всех таких непустых строк, которые встречаются как подстроки в s и t одновременно.
 * Найдите k-ую в лексикографическом порядке строку из этого множества.
 * Полезная статья про сравнение указателей: https://stackoverflow.com/questions/9086372/how-to-compare-pointers.
*/


#include <iostream>
#include <vector>


constexpr size_t ALPHABET = 256;


// сортируем посимвольно строку
std::vector<size_t> zero_iteration (const std::string &s, const size_t alphabet = ALPHABET) {
    std::vector<size_t> suffs (s.length(), 0); // for return
    std::vector<size_t> cnt (alphabet, 0);

    // карманная сортировка подсчетом
    for (size_t i = 0; i < s.length(); ++i) {
        ++cnt[s[i] - '\0'];
    }
    // считаем границы
    for (size_t i = 1; i < alphabet; ++i) {
        cnt[i] += cnt[i - 1];
    }
    // suffs будет хранить индексы начал отсортированных подстрок текущей длины
    for (size_t i = 0; i < s.length(); ++i) {
        suffs[--cnt[s[i] - '\0']] = i;
    }

    return suffs;
}


// создаем классы для последующих итераций
std::vector<size_t> make_classes (const std::string& s, const std::vector<size_t>& suffs) {
    std::vector<size_t> classes (s.length(), 0); // for return
    size_t class_numb = 0;
    char last_char = '$';

    // каждому суффиксу длины 1 сопоставляем класс
    for (size_t i = 0; i < s.length(); ++i) {
        if (s[suffs[i]] != last_char){
            last_char = s[suffs[i]];
            ++class_numb;
        }
        classes[suffs[i]] = class_numb;
    }

    return classes;
}


// сортируем циклические сдвиги строки длины степеней двойки (2, 4, 8, ... n)
void main_iterations (std::vector<size_t>& suffs, std::vector<size_t>& classes) {
    const size_t s_length = suffs.size();
    std::vector<size_t> cnt (s_length, 0);
    std::vector<size_t> sorted_by2 (s_length, 0);
    std::vector<size_t> new_classes (s_length, 0);


    for (size_t cur_len = 1; cur_len <= s_length; cur_len *= 2) {
        // сортируем по второй половине подстроки
        for (size_t i = 0; i < s_length; ++i){
            sorted_by2[i] = (suffs[i] + s_length - cur_len) % s_length;
        }
        // сортируем по первой половине
        // сортировка устойчивая, значит получим целиком отсортированные подстроки
        // обнуляем cnt
        for (size_t i = 0; i < s_length; ++i){
            cnt[i] = 0;
        }

        for (size_t i = 0; i < s_length; ++i){
            ++cnt[classes[sorted_by2[i]]];
        }

        for (size_t i = 1; i < s_length; ++i) {// считаем границы
            cnt[i] += cnt[i - 1];
        }

        for (int i = s_length - 1; i >= 0; --i){
            suffs[--cnt[classes[sorted_by2[i]]]] = sorted_by2[i];
        }

        //подсчтываем классы заново
        size_t class_numb = 0;
        for (size_t i = 1; i < s_length; ++i){
            size_t mid1 = (suffs[i] + cur_len) % s_length;
            size_t mid2 = (suffs[i - 1] + cur_len) % s_length;
            if (classes[suffs[i]] != classes[suffs[i - 1]] || classes[mid1] != classes[mid2]) {
                ++class_numb;
            }
            new_classes[suffs[i]] = class_numb;
        }

        classes = new_classes;
    }
}


std::vector<size_t> make_suff_array (const std::string& s, const size_t alphabet = ALPHABET) {
    std::vector<size_t> suffs = zero_iteration(s, alphabet);
    std::vector<size_t> classes = make_classes(s, suffs);
    main_iterations(suffs, classes);
    return suffs;
}


std::vector<int> make_lcp_array(const std::string& s, const std::vector<size_t>& suffs){
    std::vector<int> lcp (s.length(), 0);
    size_t cur_lcp = 0;
    std::vector<size_t> pos(s.length());
    for(size_t i = 0; i < s.length(); ++i) {
        pos[suffs[i]] = i; // pos = suffs^-1
    }

    for (size_t i = 0; i < s.length(); ++i) {
        if (cur_lcp > 0) {
            --cur_lcp;
        }
        if (pos[i] == s.length() - 1) {
            cur_lcp = 0;
            lcp[s.length() - 1] = -1;
        } else {
            size_t j = suffs[pos[i] + 1];
            while ((i + cur_lcp > j + cur_lcp ? i + cur_lcp : j + cur_lcp) < s.length()
                   && s[i + cur_lcp] == s[j + cur_lcp]) {
                ++cur_lcp;
            }
            lcp[pos[i]] = cur_lcp;
        }
    }

    for (int i = lcp.size() - 1; i >= 0; --i) {
        lcp[i] = lcp[i - 1];
    }
    lcp[0] = -1;

    return lcp;
}


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
