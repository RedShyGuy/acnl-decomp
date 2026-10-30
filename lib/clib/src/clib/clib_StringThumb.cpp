// The string functions of the ARM C library that are compiled C (Thumb code in the original); wchar_t is 16 bits.

#include "decomp.h"
#include "clib/clib_Locale.h"
#include "clib/clib_Thumb.h"

extern "C" {

void* memcpyWithResult(void* destination, const void* source, size_t size);
void* memmoveWithResult(void* destination, const void* source, size_t size);
void _memset_w(void* destination, size_t size, int value);
wchar_t towlower(wchar_t c);
int strcmp(const char* a, const char* b);

// 0x001007DC | manual:ours [tier A]
void* bsearch(const void* key, const void* base, size_t count, size_t size, int (*compare)(const void*, const void*))
{
    const u8* low = static_cast<const u8*>(base);
    for (;;) {
        if (count == 0) {
            return NULL;
        }
        if (count == 1) {
            if (compare(key, low) == 0) {
                return const_cast<u8*>(low);
            }
            return NULL;
        }
        size_t half = count >> 1;
        const u8* middle = low + size * half;
        int order = compare(key, middle);
        if (order == 0) {
            return const_cast<u8*>(middle);
        }
        if (order < 0) {
            count = half;
        } else {
            low = middle + size;
            count = count - half - 1;
        }
    }
}

// 0x00100834 | manual:ours [tier A]
size_t wcslen(const wchar_t* string)
{
    size_t length = 0;
    while (*string != 0) {
        length++;
        string++;
    }
    return length;
}

// 0x00100D28 | manual:ours [tier A]
int memcmp(const void* first, const void* second, size_t size)
{
    const u8* a = static_cast<const u8*>(first);
    const u8* b = static_cast<const u8*>(second);
    if (((reinterpret_cast<uptr>(a) | reinterpret_cast<uptr>(b)) & 3) == 0) {
        // aligned: by words; the first different byte (little endian) decides
        while (size >= 4) {
            u32 x = *reinterpret_cast<const u32*>(a);
            u32 y = *reinterpret_cast<const u32*>(b);
            a += 4;
            b += 4;
            size -= 4;
            if (x != y) {
                int difference = static_cast<int>(x << 24 >> 24) - static_cast<int>(y << 24 >> 24);
                if (difference != 0) {
                    return difference;
                }
                difference = static_cast<int>(x << 16 >> 16) - static_cast<int>(y << 16 >> 16);
                if (difference != 0) {
                    return difference;
                }
                difference = static_cast<int>(x << 8 >> 8) - static_cast<int>(y << 8 >> 8);
                if (difference != 0) {
                    return difference;
                }
                return static_cast<int>(x >> 24) - static_cast<int>(y >> 24);
            }
        }
    }
    for (; size != 0; size--) {
        int difference = *a++ - *b++;
        if (difference != 0) {
            return difference;
        }
    }
    return 0;
}

// 0x00100D9C | manual:ours [tier A]
char* strncat(char* destination, const char* source, size_t size)
{
    char* end = destination - 1;
    while (*++end != 0) {
    }
    while (size-- != 0) {
        char c = *source++;
        *end++ = c;
        if (c == 0) {
            return destination;
        }
    }
    *end = 0;
    return destination;
}

// 0x001017E0 | manual:abi [tier B]
wchar_t* wcscpy(wchar_t* destination, const wchar_t* source)
{
    wchar_t* start = destination;
    wchar_t c;
    do {
        c = *source++;
        *destination++ = c;
    } while (c != 0);
    return start;
}

// copies at most size characters; after the end of source the rest is filled with 0
// 0x001017FC | manual:abi [tier B]
wchar_t* wcsncpy(wchar_t* destination, const wchar_t* source, size_t size)
{
    wchar_t* start = destination;
    while (size-- != 0) {
        *destination = *source;
        destination++;
        if (*source != 0) {
            source++;
        }
    }
    return start;
}

// 0x00101818 | manual:abi [tier B]
wchar_t* wmemcpy(wchar_t* destination, const wchar_t* source, size_t size)
{
    return static_cast<wchar_t*>(memcpyWithResult(destination, source, size * 2));
}

// 0x00101822 | manual:abi [tier B]
wchar_t* wmemmove(wchar_t* destination, const wchar_t* source, size_t size)
{
    return static_cast<wchar_t*>(memmoveWithResult(destination, source, size * 2));
}

// 0x0010182C | manual:abi [tier B]
wchar_t* wcscat(wchar_t* destination, const wchar_t* source)
{
    wchar_t* start = destination;
    while (*destination != 0) {
        destination++;
    }
    wchar_t c;
    do {
        c = *source++;
        *destination++ = c;
    } while (c != 0);
    return start;
}

// 0x00101848 | manual:abi [tier B]
int wcscmp(const wchar_t* a, const wchar_t* b)
{
    wchar_t c;
    while ((c = *a) != 0 && c == *b) {
        a++;
        b++;
    }
    return c - *b;
}

// 0x00101864 | manual:abi [tier B]
int wcsncmp(const wchar_t* a, const wchar_t* b, size_t size)
{
    for (; size != 0; size--) {
        wchar_t c = *a;
        if (c == 0 || c != *b) {
            return c - *b;
        }
        a++;
        b++;
    }
    return 0;
}

// 0x0010188C | manual:abi [tier B]
wchar_t* wcsstr(const wchar_t* string, const wchar_t* part)
{
    for (;; string++) {
        size_t i;
        for (i = 0; part[i] != 0; i++) {
            wchar_t c = string[i];
            if (c == 0) {
                return NULL;
            }
            if (c != part[i]) {
                break;
            }
        }
        if (part[i] == 0) {
            return const_cast<wchar_t*>(string);
        }
    }
}

// splits string at the characters of delimiters; *next keeps the position for the next call (string NULL)
// 0x001018B8 (name is ours, after the C standard)
wchar_t* wcstok(wchar_t* string, const wchar_t* delimiters, wchar_t** next)
{
    if (string == NULL) {
        string = *next;
        if (string == NULL) {
            return NULL;
        }
    }
    // skip the delimiters at the start
    wchar_t c;
    for (; (c = *string) != 0; string++) {
        size_t i;
        for (i = 0; delimiters[i] != 0; i++) {
            if (delimiters[i] == c) {
                break;
            }
        }
        if (delimiters[i] == 0) {
            break;
        }
    }
    if (c == 0) {
        *next = NULL;
        return NULL;
    }
    // the token up to the next delimiter
    wchar_t* token = string;
    for (; *string != 0; string++) {
        size_t i;
        for (i = 0; delimiters[i] != 0; i++) {
            if (delimiters[i] == *string) {
                break;
            }
        }
        if (delimiters[i] != 0) {
            if (*string != 0) {
                *string = 0;
                *next = string + 1;
                return token;
            }
            break;
        }
    }
    *next = NULL;
    return token;
}

// 0x00101936 (name is ours, after the C standard)
wchar_t* wmemset(wchar_t* destination, wchar_t c, size_t size)
{
    wchar_t* start = destination;
    if ((reinterpret_cast<uptr>(destination) & 3) != 0) {
        if (size == 0) {
            goto last;
        }
        size--;
        *destination++ = c;
    }
    if (size >= 2) {
        // pairs of characters as words
        size_t pairs = (size >> 1) << 1;
        size_t bytes = pairs * 2;
        _memset_w(destination, bytes, c << 16 | c);
        size -= pairs;
        destination = reinterpret_cast<wchar_t*>(reinterpret_cast<u8*>(destination) + bytes);
    }
last:
    if (size != 0) {
        *destination = c;
    }
    return start;
}

// compares case-insensitively (ASCII)
// 0x00101974 | manual:ours [tier A]
int wcsicmp(const wchar_t* a, const wchar_t* b)
{
    for (size_t i = 0;; i++) {
        int difference = towlower(a[i]) - towlower(b[i]);
        if (difference != 0 || a[i] == 0) {
            return difference;
        }
    }
}

// 0x0010199E (name is ours, after the C standard)
void* memchr(const void* memory, int c, size_t size)
{
    const u8* p = static_cast<const u8*>(memory);
    u8 value = c;
    for (; size != 0; size--, p++) {
        if (*p == value) {
            return const_cast<u8*>(p);
        }
    }
    return NULL;
}

// 0x001019C0 | manual:ours [tier A]
char* strcat(char* destination, const char* source)
{
    char* end = destination - 1;
    while (*++end != 0) {
    }
    char c;
    do {
        c = *source++;
        *end++ = c;
    } while (c != 0);
    return destination;
}

// ---- qsort: quicksort down to partitions of at most 10 elements, then one insertion sort over everything

namespace {
// (the alignment of the first element and the size decide between words and bytes)
inline void SwapElements(u8* a, u8* b, int size)
{
    if (((size | reinterpret_cast<uptr>(a)) & 3) == 0) {
        do {
            u32 x = *reinterpret_cast<u32*>(b);
            u32 y = *reinterpret_cast<u32*>(a);
            *reinterpret_cast<u32*>(a) = x;
            a += 4;
            *reinterpret_cast<u32*>(b) = y;
            b += 4;
        } while ((size -= 4) != 0);
    } else {
        do {
            u8 x = *b;
            u8 y = *a;
            *a = x;
            *b = y;
            a++;
            b++;
        } while (--size != 0);
    }
}

const int QSORT_SMALL = 10;    // partitions of this size are left to the insertion sort
} // namespace

// the partitioning of qsort with a stack of its own (median of three; the larger part is pushed)
// 0x001014FA (name is ours)
void __qsort_partitions(u8* base, int count, int size, int (*compare)(const void*, const void*))
{
    u8* stackBase[32];
    int stackCount[32];
    int depth = 1;
    stackBase[0] = base;
    stackCount[0] = count;
    do {
        depth--;
        u8* low = stackBase[depth];
        count = stackCount[depth];
        for (;;) {
            int half = (count >> 1) * size;
            u8* middle = low + half;
            u8* last = middle + ((count & 1) != 0 ? half : half - size);
            if (compare(low, middle) > 0) {
                SwapElements(low, middle, size);
            }
            if (compare(middle, last) > 0) {
                if (compare(low, last) > 0) {
                    SwapElements(low, last, size);
                }
                SwapElements(middle, last, size);
            }
            u8* pivot = last - size;
            SwapElements(middle, pivot, size);
            u8* left = low;
            u8* right = pivot;
            for (;;) {
                do {
                    left += size;
                } while (compare(left, pivot) < 0);
                do {
                    right -= size;
                } while (compare(right, pivot) > 0);
                if (right <= left) {
                    break;
                }
                SwapElements(left, right, size);
            }
            SwapElements(left, pivot, size);
            int leftCount = (left - low) / size - 1;
            int rightCount = count - leftCount - 1;
            if (leftCount > rightCount) {
                if (rightCount > QSORT_SMALL) {
                    stackBase[depth] = low;
                    stackCount[depth] = leftCount;
                    depth++;
                    low = left;
                    count = rightCount;
                } else if (leftCount > QSORT_SMALL) {
                    count = leftCount;
                } else {
                    break;
                }
            } else {
                if (leftCount > QSORT_SMALL) {
                    stackBase[depth] = left;
                    stackCount[depth] = rightCount;
                    depth++;
                    count = leftCount;
                } else if (rightCount > QSORT_SMALL) {
                    low = left;
                    count = rightCount;
                } else {
                    break;
                }
            }
        }
    } while (depth != 0);
}

// 0x001016F8 (name is ours, after the C standard)
void qsort(void* base, size_t count, size_t size, int (*compare)(const void*, const void*))
{
    u8* first = static_cast<u8*>(base);
    if (first == NULL || count == 0) {
        return;
    }
    if (count > QSORT_SMALL) {
        __qsort_partitions(first, count, size, compare);
    }
    u8* end = first + (count - 1) * size;
    for (u8* element = first; element < end;) {
        u8* before = element;
        element += size;
        while (before >= first && compare(before, element) > 0) {
            before -= size;
        }
        // move the element behind "before", column by column
        u8* insert = before + size;
        if (((size | reinterpret_cast<uptr>(element)) & 3) == 0) {
            for (size_t i = 0; i < size; i += 4) {
                u8* p = element + i;
                u32 x = *reinterpret_cast<u32*>(p);
                for (p -= size; p >= insert; p -= size) {
                    *reinterpret_cast<u32*>(p + size) = *reinterpret_cast<u32*>(p);
                }
                *reinterpret_cast<u32*>(p + size) = x;
            }
        } else {
            for (size_t i = 0; i < size; i++) {
                u8* p = element + i;
                u8 x = *p;
                for (p -= size; p >= insert; p -= size) {
                    p[size] = *p;
                }
                p[size] = x;
            }
        }
    }
}

// 0x00101780 (name is ours, after the C standard)
char* strchr(const char* string, int c)
{
    char wanted = c;
    for (string--;;) {
        char x = *++string;
        if (x == wanted) {
            return const_cast<char*>(string);
        }
        if (x == '\0') {
            return NULL;
        }
    }
}

// 0x00101796 (name is ours, after the C standard)
char* strstr(const char* string, const char* part)
{
    for (;; string++) {
        const u8* a = reinterpret_cast<const u8*>(string);
        const u8* b = reinterpret_cast<const u8*>(part);
        u8 x;
        u8 y;
        do {
            x = *a++;
            y = *b++;
        } while (x != 0 && x == y);
        if (y == 0) {
            return const_cast<char*>(string);
        }
        if (x == 0) {
            return NULL;
        }
    }
}

// 0x001017BE (name is ours, after the C standard)
char* strrchr(const char* string, int c)
{
    u8 wanted = c;
    const char* found = NULL;
    u8 x;
    do {
        x = *string++;
        if (x == wanted) {
            found = string - 1;
        }
    } while (x != 0);
    return const_cast<char*>(found);
}

// strcmp through the collation table of the locale (none in the "C" locale)
// 0x00103330 (name is ours, after the C standard)
int strcoll(const char* a, const char* b)
{
    const s32* pCollate = GetCLibraryLocale()->m_pCollate;
    if (*pCollate == 0) {
        return strcmp(a, b);
    }
    const u8* pTable = reinterpret_cast<const u8*>(pCollate) + *pCollate;
    const u8* x = reinterpret_cast<const u8*>(a);
    const u8* y = reinterpret_cast<const u8*>(b);
    u8 c;
    int weightA;
    int weightB;
    do {
        c = *x++;
        weightA = pTable[c];
        weightB = pTable[*y++];
    } while (c != 0 && weightA == weightB);
    return weightA - weightB;
}

} // extern "C"

