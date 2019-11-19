/*
 * Дана строка длины n. Найти количество ее различных подстрок. Используйте суффиксный массив.
 * Построение суффиксного массива выполняйте за O(n log n).
 * Вычисление количества различных подстрок выполняйте за O(n).
*/


#include <iostream>
#include <vector>


// сортируем посимвольно строку
std::vector<int> zero_iteration (const std::string &s, const int alphabet)  {
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


    for (int cur_len = 1; cur_len <= n; cur_len *= 2){
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


std::vector<int> make_lcp_array(const std::string& s, const std::vector<int>& suffs){
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

int find_substrings(std::string& s){
    s += "$";
    std::vector<int> p;
    std::vector<int> lcp;

    p = make_suff_array(s, 256);
    lcp = make_lcp_array(s, p);

    int ans = 0;
    for (int i = 0; i < s.size(); ++i) {
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
