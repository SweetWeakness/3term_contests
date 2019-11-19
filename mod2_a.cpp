/*
 * Дана строка длины n. Найти количество ее различных подстрок. Используйте суффиксный массив.
 * Построение суффиксного массива выполняйте за O(n log n).
 * Вычисление количества различных подстрок выполняйте за O(n).
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
}

int find_substrings(std::string& s){
    s += "$";

    std::vector<int> p(s.size(), 0);
    std::vector<int> lcp(s.size(), 0);
    sufix_array(s, 256, p);
    lcp_array(s, p, lcp);

    int ans = 0;
    for (int i = 0; i < s.size(); ++i) {
        //std::cout << p[i] << "|" << lcp[i] << " \n";
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
