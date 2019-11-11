/*
 3_2. Сортировка почти упорядоченной последовательности.
 Дана последовательность целых чисел a1...an и натуральное число k, такое что для любых i, j: если j >= i + k, то a[i] <= a[j]. Требуется отсортировать последовательность. Последовательность может быть очень длинной. Время работы O(n * log(k)). Память O(k). Использовать слияние.
 */

#include <iostream>
#include <vector>

template<typename IsLess>
void sort_part(std::vector<int>& array, std::vector<int>& buff, size_t lo, size_t size, IsLess is_less) {
    size_t mid = std::min(lo + size, array.size());
    size_t hi = std::min(mid + size, array.size());
    size_t t = 0;
    size_t i = lo;
    size_t j = mid;
    while (i < mid & j < hi) {
        if (is_less(array[i], array[j])) {
            buff[t++] = array[i++];
        } else {
            buff[t++] = array[j++];
        }
    }
    while (i < mid) {
        buff[t++] = array[i++];
    }
    while (j < hi) {
        buff[t++] = array[j++];
    }
    std::copy(buff.begin(), buff.begin() + (hi - lo), array.begin() + lo);
}

template<typename IsLess>
void merge_sort(std::vector<int>& array, IsLess is_less) {
    size_t size;
    for (size = 1; size < array.size(); size *= 2) {
        std::vector<int> buff(2 * size);
        
        for (size_t lo = 0; lo < array.size(); lo += 2 * size) {
            sort_part(array, buff, lo, size, is_less);
        }
    }
}

int main(int argc, const char * argv[]) {
    size_t n, k;
    std::cin >> n;
    std::cin >> k;
    
    size_t i = 0;
    std::vector<int> a;
    a.reserve(2 * k);
    for (; i < n && i < k; ++i) {
        int num;
        std::cin >> num;
        a.push_back(num);
    }
    
    while (true) {
        for (size_t j = 0; j < k && i < n; ++j, ++i) {
            int num;
            std::cin >> num;
            a.push_back(num);
        }
        merge_sort(a, std::less<int>());
        for (size_t j = 0; j < k && j < a.size(); ++j) {
            std::cout << a[j] << ' ';
        }
        if (i == n) {
            for (size_t j = k; j < a.size(); ++j) {
                std::cout << a[j] << ' ';
            }
            break;
        }
        std::copy(a.begin() + k, a.end(), a.begin());
        a.resize(k);
    };
    
    return 0;
}
