#include <iostream>
#include <vector>


void prefix_function (std::string& s, std::vector<int>& pi) {
    int n = s.length();

    for (int i=1; i<n; ++i) {
        int j = pi[i-1];

        while (j > 0 && s[i] != s[j])
            j = pi[j-1];

        if (s[i] == s[j])
            ++j;

        pi[i] = j;
    }
}

int main() {
    std::string s, pattern;
    std::cin >> pattern;
    std::cin >> s;
    s = pattern + '#' + s;
    int n = s.length();

    std::vector<int> pi (n, 0);
    prefix_function(s, pi);

    for (int i=2*pattern.length(); i<n; ++i) {
        if (pi[i] == pattern.length()){
            std::cout << i - 2*pattern.length() << " ";
        }
    }

    /*
    std::cout << "\n";
    for (int i=0; i<n; ++i) {
        std::cout << s[i] << " ";
    }

    std::cout << "\n";
    for (int i=0; i<n; ++i) {
        std::cout << pi[i] << " ";
    }
     */
    return 0;
}
