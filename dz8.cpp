/*
 Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией. Хранимые строки непустые и состоят из строчных латинских букв. Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера. Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4. Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества и проверки принадлежности данной строки множеству. 1_1. Для разрешения коллизий используйте квадратичное пробирование. i-ая проба g(k, i)=g(k, i-1) + i (mod m). m - степень двойки. 1_2. Для разрешения коллизий используйте двойное хеширование.
 */

#include <iostream>
#include <string>
#include <vector>
#include <utility>

size_t calc_hash(const std::string& str, size_t m, uint32_t a) {
    size_t hash = 0;
    for (char c: str) {
        hash = (hash * a + c) % m;
    }
    return hash;
}

size_t calc_hash1(const std::string& str, size_t m) {
    return calc_hash(str, m, 3);
}

size_t calc_odd_hash(const std::string& str, size_t m) {
    return (calc_hash(str, m, 5) * 2 + 1) % m;
}

template<typename T, size_t(*hash1_func)(const T&, size_t), size_t(*hash2_func)(const T&, size_t)>
class HashSet {
public:
    HashSet(T empty, T deleted, size_t size=8): empty(empty), deleted(deleted), data(size, empty), added(0) {
    };
    
    bool find(const T& elem) const {
        size_t pos = 0;
        return find(elem, pos);
    }
    
    bool add(const T& elem) {
        size_t pos = 0;
        
        if (find(elem, pos)) {
            return false;
        }
        
        data[pos] = elem;
        ++added;
        
        if (4 * added >= 3 * data.size()) {
            reallocate();
        }
        
        return true;
    }
    
    bool remove(const T& elem) {
        size_t pos = 0;
        
        if (!find(elem, pos)) {
            return false;
        }
        
        data[pos] = deleted;
        return true;
    }
    
private:
    bool find(const T& elem, size_t& pos) const {
        pos = hash1_func(elem, data.size());
        
        size_t hash2 = 0;
        if (data[pos] != empty) {
            hash2 = hash2_func(elem, data.size());
        }
        
        while (data[pos] != empty) {
            if (data[pos] == elem) {
                return true;
            }
            pos = (pos + hash2) % data.size();
        }
        return false;
    }
    
    void reallocate() {
        HashSet tmp(empty, deleted, 2*data.size());
        for (const T& elem: data) {
            if (elem != empty && elem != deleted) {
                tmp.add(elem);
            }
        }
        std::swap(*this, tmp);
    }
    
    T empty;
    T deleted;
    std::vector<T> data;
    size_t added;
};


int main(int argc, const char * argv[]) {
    std::vector<std::pair<char, std::string>> requests;
    
    HashSet<std::string, calc_hash1, calc_odd_hash> hash_set("", "-");
    
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
