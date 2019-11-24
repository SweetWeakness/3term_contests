/*
 * Найти лексикографически-минимальную строку, построенную по префикс-функции, в алфавите a-z.
 */


#include <iostream>
#include <vector>


constexpr size_t ALPHABET = 26;
constexpr char ZERO_CHR = 'a';


char newCharacter(std::string& s, const std::vector<size_t >& pi, size_t index) {
    std::vector<bool> available_symbols(ALPHABET, true);

    while (index > 0) {
        index = pi[index - 1];
        available_symbols[s[index] - ZERO_CHR] = false;
    }

    for (size_t i = 0; i < ALPHABET; ++i) {
        if (available_symbols[i]) {
            return ZERO_CHR + i;
        }
    }
}

std::string buildFromPrefix(const std::vector<size_t>& pi) {
    std::string s;

    for (size_t i = 0; i < pi.size(); i++) {
        if (pi[i] == 0) {
            s += newCharacter(s, pi, i);
        } else {
            s += s[pi[i] - 1];
        }
    }

    return s;
}

int main() {
    std::vector<size_t> pi;
    size_t tmp;

    while(std::cin >> tmp){
        pi.push_back(tmp);
    }

    std::cout << buildFromPrefix(pi);

    return 0;
}
