#ifndef SRC_COMMON_STRING_UTILITY_H
#define SRC_COMMON_STRING_UTILITY_H

#include <string>
#include <vector>

using std::string;
using std::vector;

class StringUtility {
public:
    static void Split(const string& str, char ch, vector<string>* result) {
        for(int i = 0, k = 0; i <= str.length(); ++i) {
            if(str[i] == ch || str.length() == i) {
                result->push_back(str.substr(k, i-k));
                k = i + 1;
            }
        }
    }
};

#endif
