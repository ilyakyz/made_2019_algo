/*
 Найдите все вхождения шаблона в строку. Длина шаблона – p, длина строки – n. Время O(n + p), доп. память – O(p).
 */

#include <iostream>
#include <vector>
#include <string>

std::vector<int> z_function(const std::string& str) {
    int l = 0;
    int r = 0;
    std::vector<int> z(str.length());
    for (int i = 1; i < str.length(); ++i) {
        if (i <= r)
            z[i] = std::min(r-i+1, z[i-l]);
        while (i + z[i] < str.length() && str[z[i]] == str[i+z[i]])
            ++z[i];
        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
    return z;
}

std::vector<int> find(const std::string& pattern, const std::string& str) {
    std::vector<int> pattern_z = z_function(pattern);
    
    std::vector<int> result;
    int l = 0;
    int r = 0;
    for (int i = 0; i < str.length(); ++i) {
        int z = 0;
        if (i <= r)
            z = std::min(r-i+1, pattern_z[i-l]);
        while (i + z < str.length() && pattern[z] == str[i+z]) {
            ++z;
            if (z == pattern.size()) {
                result.push_back(i);
                break;
            }
        }
        if (i + z - 1 > r) {
            l = i;
            r = i + z - 1;
        }
    }
    return result;
}

int main(int argc, const char * argv[]) {
    std::string pattern, str;
    std::cin >> pattern >> str;
    
    for (int i : find(pattern, str)) {
        std::cout << i << ' ';
    }

    return 0;
}
