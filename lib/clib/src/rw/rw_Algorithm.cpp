// std::sort of the C++ library (Rogue Wave): introsort down to ranges of 16 elements (the caller finishes with an
// insertion sort), heap sort when the depth runs out. The instance for int (ARM code in the original) is used by pia
// and imgdb. Names after the reference symbols and the C++ library; the helpers are inlined in the original.

#include "decomp.h"
#include "types.h"

namespace __rw {
template <typename T>
struct __rw_lt
{
    bool operator()(const T& a, const T& b) const { return a < b; }
};
} // namespace __rw

namespace std {
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

template <typename Iterator, typename Distance, typename Compare>
DECOMP_ALWAYS_INLINE void __partial_sort(Iterator first, Iterator middle, Iterator last, Distance* pDistance,
                                         Compare compare)
{
    __make_heap(first, middle, pDistance, compare);
    for (Iterator i = middle; i < last; ++i) {
        if (compare(*i, *first)) {
            int value = *i;
            *i = *first;
            __adjust_heap(first, Distance(0), Distance(middle - first), value, compare);
        }
    }
    // (sort_heap)
    for (; middle - first > 1; --middle) {
        int value = middle[-1];
        middle[-1] = *first;
        __adjust_heap(first, Distance(0), Distance(middle - 1 - first), value, compare);
    }
}

const int __rw_threshold = 16;

template <typename Iterator, typename Distance, typename Compare>
void __introsort_loop(Iterator first, Iterator last, Distance maxDepth, Compare compare)
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

// 0x00837BA4 | fefates:bytes [tier B]
template void __introsort_loop<int*, int, __rw::__rw_lt<int> >(int* first, int* last, int maxDepth,
                                                              __rw::__rw_lt<int> compare);
} // namespace std
