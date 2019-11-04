/*
 3_2. Сортировка почти упорядоченной последовательности.
 Дана последовательность целых чисел a1...an и натуральное число k, такое что для любых i, j: если j >= i + k, то a[i] <= a[j]. Требуется отсортировать последовательность. Последовательность может быть очень длинной. Время работы O(n * log(k)). Память O(k). Использовать слияние.
 */

#include <iostream>
#include <vector>

void sort_part(std::vector<int>& array, std::vector<int>& buff, size_t lo, size_t size) {
    size_t mid = std::min(lo + size, array.size());
    size_t hi = std::min(mid + size, array.size());
    size_t t = 0;
    size_t i = lo;
    size_t j = mid;
    while (i < mid & j < hi) {
        if (array[i] <= array[j]) {
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

void merge_sort(std::vector<int>& array, size_t k) {
    if (k <= 1) {
        return;
    }
    
    size_t size;
    for (size = 1; size < 2 * k; size *= 2) {
        std::vector<int> target(2 * size);
        
        for (size_t lo = 0; lo < array.size(); lo += 2 * size) {
            sort_part(array, target, lo, size);
        }
    }
    
    size /= 2;
    std::vector<int> target(2 * size);
    for (size_t lo = size; lo < array.size(); lo += 2 * size) {
        sort_part(array, target, lo, size);
    }
    
}

int main(int argc, const char * argv[]) {
    size_t n, k;
    std::cin >> n;
    std::cin >> k;
    std::vector<int> a(n);
    for (size_t i = 0; i < n; i++) {
        std::cin >> a[i];
    }

    merge_sort(a, k);
    for (int i : a) {
        std::cout << i << ' ';
    }
    
    return 0;
}
