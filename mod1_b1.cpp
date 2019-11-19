/*
 * Найти лексикографически-минимальную строку, построенную по префикс-функции, в алфавите a-z.
 */


#include <iostream>
#include <vector>


const int alphabet = 26;
const char zero_chr = 'a';


char new_character(std::string& s, const std::vector<int>& pi, int index) {
    std::vector<bool> available_symbols(alphabet, true);

    while (index > 0) {
        index = pi[index - 1];
        available_symbols[s[index] - zero_chr] = false;
    }

    for (int i = 0; i < alphabet; ++i) {
        if (available_symbols[i]) {
            return zero_chr + i;
        }
    }

}

std::string buildFromPrefix(const std::vector<int>& pi) {
    std::string s;

    for(int i = 0; i<pi.size(); i++){
        if (pi[i] == 0){
            s += new_character(s, pi, i);
        }else{
            s += s[pi[i] - 1];
        }
    }

    return s;
}

int main() {
    std::vector<int> pi;
    int tmp;

    while(std::cin >> tmp){
        pi.push_back(tmp);
    }

    std::cout << buildFromPrefix(pi);

    return 0;
}
