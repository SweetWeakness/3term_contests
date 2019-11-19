/*
 * Заданы две строки s, t и целое число k.
 * Рассмотрим множество всех таких непустых строк, которые встречаются как подстроки в s и t одновременно.
 * Найдите k-ую в лексикографическом порядке строку из этого множества.
 * Полезная статья про сравнение указателей: https://stackoverflow.com/questions/9086372/how-to-compare-pointers.
*/


#include <iostream>
#include <vector>


// сортируем посимвольно строку
std::vector<int> zero_iteration (const std::string &s, const int alphabet) {
    const int n = s.length();
    std::vector<int> suffs (n, 0); // for return
    std::vector<int> cnt (alphabet, 0);

    // карманная сортировка подсчетом
    for (int i = 0; i < n; ++i) {
        ++cnt[s[i] - '\0'];
    }
    // считаем границы
    for (int i = 1; i < alphabet; ++i) {
        cnt[i] += cnt[i - 1];
    }
    // suffs будет хранить индексы начал отсортированных подстрок текущей длины
    for (int i = 0; i < n; ++i) {
        suffs[--cnt[s[i] - '\0']] = i;
    }

    return suffs;
}


// создаем классы для последующих итераций
std::vector<int> make_classes (const std::string& s, const std::vector<int>& suffs) {
    const int n = s.length();
    std::vector<int> classes (n, 0); // for return
    int class_numb = 0;
    char last_char = '$';

    // каждому суффиксу длины 1 сопоставляем класс
    for (int i = 0; i < n; ++i) {
        if (s[suffs[i]] != last_char){
            last_char = s[suffs[i]];
            ++class_numb;
        }
        classes[suffs[i]] = class_numb;
    }

    return classes;
}


// сортируем циклические сдвиги строки длины степеней двойки (2, 4, 8, ... n)
void main_iterations (std::vector<int>& suffs, std::vector<int>& classes) {
    const int n = suffs.size();
    std::vector<int> cnt (n, 0);
    std::vector<int> sorted_by2 (n, 0);
    std::vector<int> new_classes (n, 0);


    for (int cur_len = 1; cur_len <= n; cur_len *= 2) {
        // сортируем по второй половине подстроки
        for (int i = 0; i < n; ++i){
            sorted_by2[i] = (suffs[i] + n - cur_len) % n;
        }
        // сортируем по первой половине
        // сортировка устойчивая, значит получим целиком отсортированные подстроки
        // обнуляем cnt
        for (int i = 0; i < n; ++i){
            cnt[i] = 0;
        }

        for (int i = 0; i < n; ++i){
            ++cnt[classes[sorted_by2[i]]];
        }

        for (int i = 1; i < n; ++i) {// считаем границы
            cnt[i] += cnt[i - 1];
        }

        for (int i = n - 1; i >= 0; --i){
            suffs[--cnt[classes[sorted_by2[i]]]] = sorted_by2[i];
        }

        //подсчтываем классы заново
        int class_numb = 0;
        for (int i = 1; i < n; ++i){
            int mid1 = (suffs[i] + cur_len) % n;
            int mid2 = (suffs[i - 1] + cur_len) % n;
            if (classes[suffs[i]] != classes[suffs[i - 1]] || classes[mid1] != classes[mid2]) {
                ++class_numb;
            }
            new_classes[suffs[i]] = class_numb;
        }

        classes = new_classes;
    }
}


std::vector<int> make_suff_array (const std::string& s, const int alphabet) {
    std::vector<int> suffs = zero_iteration(s, alphabet);
    std::vector<int> classes = make_classes(s, suffs);
    main_iterations(suffs, classes);
    return suffs;
}


std::vector<int> make_lcp_array(const std::string& s, const std::vector<int>& suffs) {
    const int n = s.length();
    std::vector<int> lcp (n, 0);
    int k = 0;
    std::vector<int> pos(n);
    for(int i = 0; i < n; ++i) {
        pos[suffs[i]] = i; // pos = suffs^-1
    }

    for(int i = 0; i < n; ++i){
        if (k > 0) {
            --k;
        }
        if (pos[i] == n - 1) {
            k = 0;
            lcp[n - 1] = -1;
        } else {
            int j = suffs[pos[i] + 1];
            while ((i + k > j + k ? i + k : j + k) < n and s[i + k] == s[j + k]) {
                ++k;
            }
            lcp[pos[i]] = k;
        }
    }

    return lcp;
}


bool is_changed (const std::vector<int>& p, const int i, const int pivot) {
    return (p[i] < pivot && p[i - 1] > pivot) ||
           (p[i] > pivot && p[i - 1] < pivot);
}


std::string find_k_substring(std::string& s, const std::string& l, int64_t k){
    const int old_s_size = s.size();
    s = s + "#" + l + "$";

    std::vector<int> p;
    std::vector<int> lcp;
    p = make_suff_array(s, 256);
    lcp = make_lcp_array(s, p);

    int lcp_old = 0;
    std::string ans;

    for (int i = 1; i < s.size(); ++i){
        if (is_changed(p, i, old_s_size)) {
            if (lcp_old + k > lcp[i]) {
                if (lcp[i] >= lcp_old) {
                    k -= lcp[i] - lcp_old;
                }
                lcp_old = lcp[i];

            }else{
                for (int j = p[i]; j < lcp_old + k + p[i]; ++j) {
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
