/*
 Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией. Хранимые строки непустые и состоят из строчных латинских букв. Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера. Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4. Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества и проверки принадлежности данной строки множеству. 1_1. Для разрешения коллизий используйте квадратичное пробирование. i-ая проба g(k, i)=g(k, i-1) + i (mod m). m - степень двойки. 1_2. Для разрешения коллизий используйте двойное хеширование.
 */

#include <iostream>
#include <string>
#include <vector>
#include <utility>

size_t calc_hash(const std::string& str, uint32_t a, size_t m) {
    size_t hash = 0;
    for (char c: str) {
        hash = (hash * a + c) % m;
    }
    return hash;
}

size_t calc_odd_hash(const std::string& str, uint32_t a, size_t m) {
    return (calc_hash(str, a, m) * 2 + 1) % m;
}

const std::string DELETED = "-";

class HashSet {
public:
    HashSet(size_t size=8): data(size), added(0) {
    };
    
    bool find(const std::string& str) const {
        size_t pos = 0;
        return find(str, pos);
    }
    
    bool add(const std::string& str) {
        size_t pos = 0;
        
        if (find(str, pos)) {
            return false;
        }
        
        data[pos] = str;
        ++added;
        
        if (4 * added >= 3 * data.size()) {
            reallocate();
        }
        
        return true;
    }
    
    bool remove(const std::string& str) {
        size_t pos = 0;
        
        if (!find(str, pos)) {
            return false;
        }
        
        data[pos] = DELETED;
        return true;
    }
    
private:
    bool find(const std::string& str, size_t& pos) const {
        size_t hash1 = calc_hash(str, a1, data.size());
        pos = hash1;
        
        size_t hash2 = 0;
        if (!data[pos].empty()) {
            hash2 = calc_odd_hash(str, a2, data.size());
        }
        
        while (!data[pos].empty()) {
            if (data[pos] == str) {
                return true;
            }
            pos = (pos + hash2) % data.size();
        }
        return false;
    }
    
    
    void reallocate() {
        HashSet tmp(2*data.size());
        for (const std::string& str: data) {
            if (!str.empty() && str != DELETED) {
                tmp.add(str);
            }
        }
        std::swap(*this, tmp);
    }
    
    const static uint32_t a1 = 3;
    const static uint32_t a2 = 5;
    
    std::vector<std::string> data;
    size_t added;
};


int main(int argc, const char * argv[]) {
    std::vector<std::pair<char, std::string>> requests;
    
    HashSet hash_set;
    
    while (!std::cin.eof()) {
        std::pair<char, std::string> request;
        std::cin >> request.first;
        std::cin >> request.second;
        requests.push_back(request);
    }
    
    for (auto& request: requests) {
        switch (request.first) {
            case '+':
                std::cout << (hash_set.add(request.second) ? "OK" : "FAIL") << std::endl;
                break;
            case '-':
                std::cout << (hash_set.remove(request.second) ? "OK" : "FAIL") << std::endl;
                break;
            case '?':
                std::cout << (hash_set.find(request.second) ? "OK" : "FAIL") << std::endl;
            default:
                break;
        }
    }

    return 0;
}
