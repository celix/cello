#ifndef SRC_COMMON_STRING_UTILITY_H
#define SRC_COMMON_STRING_UTILITY_H

#include <string>
#include <vector>
#include <assert.h>

using std::string;
using std::vector;

class StringUtility {
public:
    static void Split(const string& str, char ch, vector<string>* result) {
        for(size_t i = 0, k = 0; i <= str.length(); ++i) {
            if(str[i] == ch || (str.length() == i && k < i)) {
                result->push_back(str.substr(k, i-k));
                k = i + 1;
            }
        }
    }

    static char ** CreateArgArray(const vector<string>& source) {
        char ** dest = new char*[source.size() + 1];
        memset(dest, 0, (source.size() + 1)*sizeof(char**));
        assert(dest);
        for (size_t i = 0; i < source.size(); ++i)
            dest[i] = strnewcpy(source[i]);
        return dest;
    }

    /// @brief: string copy with new memory space 
    static char* strnewcpy(const string& src) {
        char* dest = new char[src.size() + 1];
        memset(dest, 0, src.size() + 1);
        strncpy(dest, src.c_str(), src.size() + 1);
        return dest;
    }

    /// @brief: delete char* array
    static void DestoryArgArray(char ** source) {
        for (int i = 0; source[i]; ++i)
            delete [] source[i];
        delete [] source;
    }

    static string Trim(const string& source) {
        if (source.empty())
            return source;
        int len = source.length();
        int beg, end;
        for (beg = 0; beg < len; ++beg)
            if (!isspace((unsigned char)source[beg]))
                break;
        for (end = len - 1; end > 0; --end)
            if (!isspace((unsigned char)source[end]))
                break;
        if (beg > end)
            return "";
        return source.substr(beg, end - beg + 1);
    }
};

#endif
