#pragma once

// The algorithms of the C++ library ARMCC linked in (Rogue Wave) that the program uses: min/max/swap, for_each,
// copy_backward and sort (introsort down to ranges of 16 elements, then insertion sort). Names after the C++ standard
// and the reference symbols; std::__introsort_loop<int*> is out of line in the original (rw_Algorithm.cpp), the rest is
// inlined.

#include "decomp.h"
#include "rw/rw_Utility.h"

namespace __rw {
template <typename T>
struct __rw_lt
{
    bool operator()(const T& a, const T& b) const { return a < b; }
};
} // namespace __rw

namespace std {
template <typename T>
inline const T& min(const T& a, const T& b)
{
    return b < a ? b : a;
}

template <typename T>
inline const T& max(const T& a, const T& b)
{
    return a < b ? b : a;
}

template <typename Iterator, typename Function>
inline Function for_each(Iterator first, Iterator last, Function function)
{
    for (; first != last; ++first) {
        function(*first);
    }
    return function;
}

template <typename Iterator, typename T>
inline void fill(Iterator first, Iterator last, const T& value)
{
    for (; first != last; ++first) {
        *first = value;
    }
}

template <typename Iterator, typename T>
inline Iterator find(Iterator first, Iterator last, const T& value)
{
    while (first != last && !(*first == value)) {
        ++first;
    }
    return first;
}

template <typename Iterator1, typename Iterator2>
inline Iterator2 copy(Iterator1 first, Iterator1 last, Iterator2 result)
{
    for (; first != last; ++result, ++first) {
        *result = *first;
    }
    return result;
}

template <typename Iterator1, typename Iterator2, typename T>
inline Iterator2 remove_copy(Iterator1 first, Iterator1 last, Iterator2 result, const T& value)
{
    for (; first != last; ++first) {
        if (!(*first == value)) {
            *result = *first;
            ++result;
        }
    }
    return result;
}

// (the elements equal to value are left out; the new end)
template <typename Iterator, typename T>
inline Iterator remove(Iterator first, Iterator last, const T& value)
{
    first = find(first, last, value);
    Iterator next = first;
    return first == last ? first : remove_copy(++next, last, first, value);
}

template <typename Iterator1, typename Iterator2>
inline Iterator2 copy_backward(Iterator1 first, Iterator1 last, Iterator2 result)
{
    while (first != last) {
        *--result = *--last;
    }
    return result;
}

template <typename T, typename Compare>
DECOMP_ALWAYS_INLINE const T& __median(const T& a, const T& b, const T& c, Compare compare)
{
    if (compare(a, b)) {
        if (compare(b, c)) {
            return b;
        }
        return compare(a, c) ? c : a;
    }
    if (compare(a, c)) {
        return a;
    }
    return compare(b, c) ? c : b;
}

template <typename Iterator, typename T, typename Compare>
DECOMP_ALWAYS_INLINE Iterator __unguarded_partition(Iterator first, Iterator last, T pivot, Compare compare)
{
    for (;;) {
        while (compare(*first, pivot)) {
            ++first;
        }
        --last;
        while (compare(pivot, *last)) {
            --last;
        }
        if (!(first < last)) {
            return first;
        }
        T swapped = *first;
        *first = *last;
        *last = swapped;
        ++first;
    }
}

template <typename Iterator, typename Distance, typename T, typename Compare>
DECOMP_ALWAYS_INLINE void __adjust_heap(Iterator first, Distance hole, Distance length, T value, Compare compare)
{
    Distance top = hole;
    Distance child = 2 * hole + 2;
    for (; child < length; child = 2 * child + 2) {
        if (compare(first[child], first[child - 1])) {
            child--;
        }
        first[hole] = first[child];
        hole = child;
    }
    if (child == length) {
        first[hole] = first[child - 1];
        hole = child - 1;
    }
    for (Distance parent = (hole - 1) / 2; hole > top && compare(first[parent], value); parent = (hole - 1) / 2) {
        first[hole] = first[parent];
        hole = parent;
    }
    first[hole] = value;
}

template <typename Iterator, typename Distance, typename Compare>
DECOMP_ALWAYS_INLINE void __make_heap(Iterator first, Iterator last, Distance*, Compare compare)
{
    Distance length = last - first;
    if (length < 2) {
        return;
    }
    for (Distance parent = (length - 2) / 2;; parent--) {
        __adjust_heap(first, parent, length, first[parent], compare);
        if (parent == 0) {
            return;
        }
    }
}

template <typename T, typename Distance, typename Compare>
DECOMP_ALWAYS_INLINE void __partial_sort(T* first, T* middle, T* last, Distance* pDistance, Compare compare)
{
    __make_heap(first, middle, pDistance, compare);
    for (T* i = middle; i < last; ++i) {
        if (compare(*i, *first)) {
            T value = *i;
            *i = *first;
            __adjust_heap(first, Distance(0), Distance(middle - first), value, compare);
        }
    }
    // (sort_heap)
    for (; middle - first > 1; --middle) {
        T value = middle[-1];
        middle[-1] = *first;
        __adjust_heap(first, Distance(0), Distance(middle - 1 - first), value, compare);
    }
}

const int __rw_threshold = 16;

template <typename Iterator, typename Distance, typename Compare>
DECOMP_NOIPA void __introsort_loop(Iterator first, Iterator last, Distance maxDepth, Compare compare)
{
    while (last - first > __rw_threshold) {
        if (maxDepth == 0) {
            __partial_sort(first, last, last, static_cast<Distance*>(0), compare);
            return;
        }
        Iterator cut =
            __unguarded_partition(first, last, __median(*first, first[(last - first) / 2], last[-1], compare), compare);
        maxDepth /= 2;
        __introsort_loop(cut, last, maxDepth, compare);
        last = cut;
    }
}

// (the instance for int is out of line in the library: rw_Algorithm.cpp)
extern template void __introsort_loop<int*, int, __rw::__rw_lt<int> >(int* first, int* last, int maxDepth,
                                                                     __rw::__rw_lt<int> compare);

template <typename Iterator, typename T, typename Compare>
DECOMP_ALWAYS_INLINE void __unguarded_linear_insert(Iterator last, T value, Compare compare)
{
    Iterator next = last;
    --next;
    while (compare(value, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = value;
}

template <typename T, typename Compare>
DECOMP_ALWAYS_INLINE void __insertion_sort(T* first, T* last, Compare compare)
{
    if (first == last) {
        return;
    }
    for (T* i = first + 1; i != last; ++i) {
        T value = *i;
        if (compare(value, *first)) {
            copy_backward(first, i, i + 1);
            *first = value;
        } else {
            __unguarded_linear_insert(i, value, compare);
        }
    }
}

template <typename Iterator, typename Compare>
DECOMP_ALWAYS_INLINE void __final_insertion_sort(Iterator first, Iterator last, Compare compare)
{
    if (last - first > __rw_threshold) {
        __insertion_sort(first, first + __rw_threshold, compare);
        for (Iterator i = first + __rw_threshold; i != last; ++i) {
            __unguarded_linear_insert(i, *i, compare);
        }
    } else {
        __insertion_sort(first, last, compare);
    }
}

template <typename Iterator, typename Compare>
inline void sort(Iterator first, Iterator last, Compare compare)
{
    if (!(first == last)) {
        __introsort_loop(first, last, last - first, compare);
        __final_insertion_sort(first, last, compare);
    }
}

template <typename T>
inline void sort(T* first, T* last)
{
    sort(first, last, __rw::__rw_lt<T>());
}
} // namespace std
