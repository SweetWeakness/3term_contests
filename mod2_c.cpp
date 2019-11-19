/*
 * Заданы две строки s, t и целое число k.
 * Рассмотрим множество всех таких непустых строк, которые встречаются как подстроки в s и t одновременно.
 * Найдите k-ую в лексикографическом порядке строку из этого множества.
 * Полезная статья про сравнение указателей: https://stackoverflow.com/questions/9086372/how-to-compare-pointers.
*/
#include <iostream>
#include <vector>


void sufix_array(std::string& s, int alphabet, std::vector<int>& suffs) {
    int n = s.size();
    int cnt_len = (n > alphabet ? n : alphabet);
    std::vector<int> cnt(cnt_len, 0);
    std::vector<int> classes(n, 0);

    for (int i = 0; i < n; ++i) { // карманная сортировка подсчетом
        ++cnt[s[i] - '\0'];
    }
    for (int i = 1; i < alphabet; ++i) // считаем границы
        cnt[i] += cnt[i-1];
    // suffs будет хранить индексы начал отсортированных подстрок текущей длины
    for (int i = 0; i < n; ++i) {
        suffs[--cnt[s[i] - '\0']] = i;
    }

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




    // нулевая итерация завершена
    // сортируем подстроки длиной 2 * cur_len = 2^k
    int cur_len = 1;
    std::vector<int> sorted_by2(n, 0);
    std::vector<int> new_classes(n, 0);
    while (cur_len <= n){
        // сортируем по второй половине подстроки
        for (int i = 0; i < n; ++i){
            sorted_by2[i] = (suffs[i] + n - cur_len) % n;
        }
        // сортируем по первой половине
        // сортировка устойчивая, значит получим целиком отсортированные подстроки
        for (int i = 0; i < cnt_len; ++i){ // обнуляем cnt
            cnt[i] = 0;
        }

        for (int i = 0; i < n; ++i){
            ++cnt[classes[sorted_by2[i]]];
        }

        for (int i = 1; i < cnt_len; ++i) // считаем границы
            cnt[i] += cnt[i-1];


        for (int i = n - 1; i >= 0; --i){
            suffs[--cnt[classes[sorted_by2[i]]]] = sorted_by2[i];
        }

        class_numb = 0;
        for (int i = 1; i < n; ++i){
            int mid1 = (suffs[i] + cur_len) % n;
            int mid2 = (suffs[i - 1] + cur_len) % n;
            if (classes[suffs[i]] != classes[suffs[i - 1]] or classes[mid1] != classes[mid2])
                ++class_numb;
            new_classes[suffs[i]] = class_numb;
        }
        for (int i = 0; i < n; ++i){
            classes[i] = new_classes[i];
        }
        cur_len *= 2;
    }
}

void lcp_array(std::string& s, std::vector<int>& suffs, std::vector<int>& lcp){
    int n = s.length();
    int k = 0;
    std::vector<int> pos(n);
    for(int i = 0; i < n; ++i) pos[suffs[i]] = i; // pos = suffs^-1

    for(int i = 0; i < n; ++i){
        if (k > 0) --k;
        if (pos[i] == n - 1){
            k = 0;
            lcp[n - 1] = -1;
        }else{
            int j = suffs[pos[i] + 1];
            while ((i + k > j + k ? i + k : j + k) < n and s[i + k] == s[j + k])
                ++k;
            lcp[pos[i]] = k;
        }
    }

    for(int i = n - 1; i > 0; --i){
        lcp[i] = lcp[i - 1];
    }
    lcp[0] = -1;
}

bool is_changed(std::vector<int>& p, int i, int pivot){
    return (p[i] < pivot && p[i - 1] > pivot) ||
           (p[i] > pivot && p[i - 1] < pivot);
}


std::string find_k_substring(std::string& s, std::string& l, int64_t k){
    int old_s_size = s.size();
    s = s + "#" + l + "$";

    std::vector<int> p(s.size(), 0);
    std::vector<int> lcp(s.size(), 0);
    sufix_array(s, 256, p);
    lcp_array(s, p, lcp);

    for (int i = 0; i < s.size(); ++i) {
        //std::cout << p[i] << "|" << lcp[i] << " \n";
    }

    int lcp_old = 0;
    std::string ans;

    for (int i = 1; i < s.size(); ++i){
        if (is_changed(p, i, old_s_size)){
            //std::cout << p[i - 1] << " comp with " << p[i] << " " << "(" << lcp[i] << ") " << k << "\n";
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
        if (lcp[i] < lcp_old){
            lcp_old = lcp[i];
        }
    }

    if (k > 0)
        ans = "-1";

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
