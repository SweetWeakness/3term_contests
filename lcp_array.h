#ifndef INC_3TERM_CONTESTS_LCP_ARRAY_H
#define INC_3TERM_CONTESTS_LCP_ARRAY_H


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

    return lcp;
}


#endif //INC_3TERM_CONTESTS_LCP_ARRAY_H
