/*
 Шаблон поиска задан строкой длины m, в которой кроме обычных символов могут встречаться символы “?”. Найти позиции всех вхождений шаблона в тексте длины n. Каждое вхождение шаблона предполагает, что все обычные символы совпадают с соответствующими из текста, а вместо символа “?” в тексте встречается произвольный символ.
 */

#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>

struct Node {
    int from;
    char symbol;
    std::vector<int> childs;
    std::vector<int> go;
    int back_link;
    std::vector<int> chars_till_end;
    
    Node(int from, char symbol): from(from), symbol(symbol), childs(std::vector<int>(26, -1)),
        go(std::vector<int>(26, -1)), back_link(-1) {
    }
};

struct PatternPart {
    std::string str;
    int chars_till_end;
};

class Matcher {
public:
    void build(const std::string& pattern) {
        std::vector<PatternPart> parts;
        std::stringstream stream(pattern);
        std::string part;
        int chars_till_end = int(pattern.size());
        while (std::getline(stream, part, '?')) {
            chars_till_end -= part.size();
            if (part.size()) {
                parts.push_back({part, chars_till_end});
            }
            chars_till_end -= 1;
        }
        
        parts_count = parts.size();
        trie.push_back({-1, 0});
        for (PatternPart& part: parts) {
            add_part(part);
        }
        pattern_size = pattern.size();
    }
    
    std::vector<int> math(const std::string& str) {
        int current = 0;
        parts_mathed.resize(str.size(), 0);
        for (size_t pos = 0; pos < str.size(); ++pos) {
            char c = str[pos] - 'a';
            current = go(current, c);
            int link = current;
            while (link != 0) {
                for (int c_till_end: trie[link].chars_till_end) {
                    if (pos + c_till_end < parts_mathed.size())
                        ++parts_mathed[pos + c_till_end];
                }
                link = get_link(link);
            }
        }
        std::vector<int> result;
        for (int i = int(pattern_size)-1; i < parts_mathed.size(); ++i) {
            if (parts_mathed[i] == parts_count)
                result.push_back(i + 1 - int(pattern_size));
        }
        return result;
    };
    
private:
    void add_part(const PatternPart& part) {
        int current = 0;
        for (char cur_char: part.str) {
            char c = cur_char - 'a';
            if (trie[current].childs[c] == -1) {
                trie[current].childs[c] = int(trie.size());
                trie.push_back({current, c});
            }
            current = trie[current].childs[c];
        }
        trie[current].chars_till_end.push_back(part.chars_till_end);
    }

    int go(int current, char c) {
        if (trie[current].go[c] == -1) {
            if (trie[current].childs[c] != -1)
                trie[current].go[c] = trie[current].childs[c];
            else
                trie[current].go[c] = current==0 ? 0 : go(get_link(current), c);
        }
        return trie[current].go[c];
    }
    
    int get_link(int current) {
        if (trie[current].back_link == -1)
            if (current == 0 || trie[current].from == 0)
                trie[current].back_link = 0;
            else
                trie[current].back_link = go(get_link(trie[current].from), trie[current].symbol);
        return trie[current].back_link;
    }
    
    size_t parts_count;
    std::vector<int> parts_mathed;
    std::vector<Node> trie;
    size_t pattern_size;
};

int main(int argc, const char * argv[]) {

    Matcher m;
    std::string pattern, str;
    std::cin >> pattern >> str;
    if (std::find_if(pattern.begin(), pattern.end(), [](char c) {return c != '?';}) == pattern.end()) {
        for (int i = 0; i + pattern.size() <= str.size(); ++i) {
            std::cout << i << ' ';
        }
    } else {
        m.build(pattern);
        
        std::vector<int> result = m.math(str);
        for (int pos : result) {
            std::cout << pos << ' ';
        }
    }
    
    return 0;
}
