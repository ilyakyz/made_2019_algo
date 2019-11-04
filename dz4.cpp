/*
Даны неотрицательные целые числа n, k и массив целых чисел из диапазона [0..109] размера n.
Требуется найти k-ю порядковую статистику. т.е. напечатать число, которое бы стояло на позиции с индексом k ∈[0..n-1] в отсортированном массиве.
Напишите нерекурсивный алгоритм.
Требования к дополнительной памяти: O(n).
Требуемое среднее время работы: O(n).
Функцию Partition следует реализовывать методом прохода двумя итераторами в одном направлении.
Описание для случая прохода от начала массива к концу:
Выбирается опорный элемент.
Опорный элемент меняется с последним элементом массива.
Во время работы Partition в начале массива содержатся элементы, не бОльшие опорного. Затем располагаются элементы, строго бОльшие опорного. В конце массива лежат нерассмотренные элементы. Последним элементом лежит опорный.
Итератор (индекс) i указывает на начало группы элементов, строго бОльших опорного.
Итератор j больше i, итератор j указывает на первый нерассмотренный элемент.
Шаг алгоритма. Рассматривается элемент, на который указывает j. Если он больше опорного, то сдвигаем j. Если он не больше опорного, то меняем a[i] и a[j] местами, сдвигаем i и сдвигаем j.
В конце работы алгоритма меняем опорный и элемент, на который указывает итератор i.
*/

#include <iostream>
#include <vector>

typedef std::vector<int>::iterator v_it;

v_it median(v_it first, v_it second, v_it third) {
    if (*first < *second) {
        if (*second < *third)
            return second;
        if (*third < *first)
            return first;
        return third;
    }
    if (*second > *third)
        return second;
    if (*third > *first)
        return first;
    return third;
}

v_it partition(v_it first, v_it last) {
    v_it mid = first + (last - first) / 2;
    v_it med_it = median(first, mid, last);
    std::swap(*med_it, *last);
    int pivot = *last;
    while (first < last && *first <= pivot) {
        ++first;
    }
    v_it second = first;
    while (second < last) {
        if (*second > pivot) {
            ++second;
        } else {
            std::swap(*first, *second);
            ++first;
            ++second;
        }
    }
    std::swap(*first, *last);
    
    return first;
}

int k_statistics(std::vector<int>& array, size_t k) {
    v_it first = array.begin();
    v_it last = array.end() - 1;
    while (true) {
        v_it mid = partition(first, last);
       // std::cout << first - array.begin() << ' ' << last - array.begin() << ' ' << mid - array.begin() << std::endl;
        if (mid == array.begin() + k) {
            return *mid;
        }
        if (mid > array.begin() + k) {
            last = mid - 1;
        } else {
            first = mid + 1;
        }
    }
    return 0;
}
    

int main(int argc, const char * argv[]) {
    size_t n, k;
    std::cin >> n;
    std::cin >> k;
    std::vector<int> sequence(n);
    for (size_t i = 0; i < n; ++i) {
        std::cin >> sequence[i];
    }
    std::cout << k_statistics(sequence, k);
    return 0;
}
