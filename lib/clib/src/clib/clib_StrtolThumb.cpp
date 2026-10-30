// The conversion of strings to integers of the ARM C library (Thumb code in the original): a core per width reads
// the digits (base 0: decimal, octal with 0, hexadecimal with 0x) and keeps the value in 16 bit parts to see an
// overflow; the C functions skip the white space and the sign around it.

#include "decomp.h"
#include "clib/clib_Locale.h"
#include "clib/clib_Thumb.h"

namespace {
// the values of errno (names are ours)
const int ERROR_RANGE = 2;

inline bool IsSpace(int c)
{
    return ((*__rt_ctype_table())[c] & clib::CTYPE_SPACE) != 0;
}
} // namespace

extern "C" {

int* __aeabi_errno_addr();

// the value of the digit c in base, or -1
// 0x002F98F8 (name is ours)
DECOMP_NOINLINE int __digit_value(u32 c, u32 base)
{
    if (c < '9' + 1) {
        c -= '0';
    }
    u32 upper = c & ~0x20;
    if (upper >= 'A') {
        c = upper - ('A' - 10);
    }
    if (c >= base) {
        return -1;
    }
    return c;
}

// 0x002F80DC (name is ours)
DECOMP_NOINLINE u32 __strtoul_core(const char* pString, char** ppEnd, int base)
{
    bool any = false;
    const char* p = pString;
    int c = static_cast<u8>(*p++);
    bool overflow = false;
    if (c == '0') {
        c = static_cast<u8>(*p++);
        any = true;
        if (c == 'x' || c == 'X') {
            if (base == 0 || base == 16) {
                c = static_cast<u8>(*p++);
                any = false;
                base = 16;
            }
        } else if (base == 0) {
            base = 8;
        }
    } else if (base == 0) {
        base = 10;
    }
    // the value in two parts: the low 16 bits and the rest
    u32 low = 0;
    u32 high = 0;
    for (int digit; (digit = __digit_value(c, base)) >= 0;) {
        any = true;
        u32 value = low * base + digit;
        low = value & 0xFFFF;
        high = high * base + (value >> 16);
        if (high >= 0x10000) {
            overflow = true;
        }
        c = static_cast<u8>(*p++);
    }
    if (ppEnd != NULL) {
        *ppEnd = const_cast<char*>(any ? p - 1 : pString);
    }
    if (overflow) {
        *__aeabi_errno_addr() = ERROR_RANGE;
        return 0xFFFFFFFF;
    }
    return high << 16 | low;
}

// 0x00102204 (name is ours)
DECOMP_NOINLINE u32 __wcstoul_core(const wchar_t* pString, wchar_t** ppEnd, int base)
{
    bool any = false;
    const wchar_t* p = pString;
    int c = *p++;
    bool overflow = false;
    if (c == '0') {
        c = *p++;
        any = true;
        if (c == 'x' || c == 'X') {
            if (base == 0 || base == 16) {
                c = *p++;
                any = false;
                base = 16;
            }
        } else if (base == 0) {
            base = 8;
        }
    } else if (base == 0) {
        base = 10;
    }
    u32 low = 0;
    u32 high = 0;
    for (int digit; (digit = __digit_value(c, base)) >= 0;) {
        any = true;
        u32 value = low * base + digit;
        low = value & 0xFFFF;
        high = high * base + (value >> 16);
        if (high >= 0x10000) {
            overflow = true;
        }
        c = *p++;
    }
    if (ppEnd != NULL) {
        *ppEnd = const_cast<wchar_t*>(any ? p - 1 : pString);
    }
    if (overflow) {
        *__aeabi_errno_addr() = ERROR_RANGE;
        return 0xFFFFFFFF;
    }
    return high << 16 | low;
}

// 0x00102F4C (name is ours)
DECOMP_NOINLINE u64 __strtoull_core(const char* pString, char** ppEnd, int base)
{
    bool any = false;
    bool overflow = false;
    const char* p = pString;
    int c = static_cast<u8>(*p++);
    if (c == '0') {
        any = true;
        c = static_cast<u8>(*p++);
        if (c == 'x' || c == 'X') {
            if (base == 0 || base == 16) {
                any = false;
                c = static_cast<u8>(*p++);
                base = 16;
            }
        } else if (base == 0) {
            base = 8;
        }
    } else if (base == 0) {
        base = 10;
    }
    // the value in two parts: the low 16 bits and the rest
    u64 high = 0;
    u32 low = 0;
    for (int digit; (digit = __digit_value(c, base)) >= 0;) {
        any = true;
        u32 value = low * base + digit;
        high = high * static_cast<s64>(base) + (value >> 16);
        low = value & 0xFFFF;
        if (static_cast<u32>(high >> 32) >= 0x10000) {
            overflow = true;
        }
        c = static_cast<u8>(*p++);
    }
    if (ppEnd != NULL) {
        *ppEnd = const_cast<char*>(any ? p - 1 : pString);
    }
    if (overflow) {
        *__aeabi_errno_addr() = ERROR_RANGE;
        return 0xFFFFFFFFFFFFFFFF;
    }
    return high << 16 | low;
}

// 0x00102118 (name is ours, after the C standard)
DECOMP_NOINLINE long strtol(const char* pString, char** ppEnd, int base)
{
    bool isNegative = false;
    const char* p = pString;
    int c;
    do {
        c = static_cast<u8>(*p++);
    } while (c != 0 && IsSpace(c));
    if (c != '+') {
        if (c == '-') {
            isNegative = true;
        } else {
            p--;
        }
    }
    long value = __strtoul_core(p, ppEnd, base);
    if (ppEnd != NULL && *ppEnd == p) {
        *ppEnd = const_cast<char*>(pString);
    }
    if (isNegative) {
        value = -value;
        if (value > 0) {
            *__aeabi_errno_addr() = ERROR_RANGE;
            return static_cast<long>(0x80000000);
        }
    } else if (value < 0) {
        *__aeabi_errno_addr() = ERROR_RANGE;
        return 0x7FFFFFFF;
    }
    return value;
}

// 0x00101330 (name is ours, after the C standard)
int atoi(const char* pString)
{
    int* pErrno = __aeabi_errno_addr();
    int saved = *pErrno;
    int value = strtol(pString, NULL, 10);
    *pErrno = saved;
    return value;
}

// 0x0010134A (name is ours, after the C standard)
unsigned long strtoul(const char* pString, char** ppEnd, int base)
{
    bool isNegative = false;
    int* pErrno = __aeabi_errno_addr();
    int saved = *pErrno;
    const char* p = pString;
    int c;
    do {
        c = static_cast<u8>(*p++);
    } while (c != 0 && IsSpace(c));
    if (c != '+') {
        if (c == '-') {
            isNegative = true;
        } else {
            p--;
        }
    }
    *pErrno = 0;
    u32 value = __strtoul_core(p, ppEnd, base);
    if (ppEnd != NULL && *ppEnd == p) {
        *ppEnd = const_cast<char*>(pString);
    }
    if (*pErrno == ERROR_RANGE) {
        return value;
    }
    *pErrno = saved;
    if (isNegative) {
        return -value;
    }
    return value;
}

// 0x00102188 (name is ours)
DECOMP_NOINLINE u64 __strtoull(const char* pString, char** ppEnd, int base)
{
    bool isNegative = false;
    int* pErrno = __aeabi_errno_addr();
    int saved = *pErrno;
    const char* p = pString;
    int c;
    do {
        c = static_cast<u8>(*p++);
    } while (c != 0 && IsSpace(c));
    if (c != '+') {
        if (c == '-') {
            isNegative = true;
        } else {
            p--;
        }
    }
    *pErrno = 0;
    u64 value = __strtoull_core(p, ppEnd, base);
    if (ppEnd != NULL && *ppEnd == p) {
        *ppEnd = const_cast<char*>(pString);
    }
    if (*pErrno == ERROR_RANGE) {
        return value;
    }
    *pErrno = saved;
    if (isNegative) {
        return -value;
    }
    return value;
}

// 0x001013C0 | manual:abi [tier B]
u64 strtoull(const char* pString, char** ppEnd, int base)
{
    return __strtoull(pString, ppEnd, base);
}

// the classes of a wide character: the ctype classes and derived ones (0x400 alpha, 0x800 alnum, 0x100 graph,
// 0x200 print, 0x1000 blank)
// 0x001024FA (name is ours)
DECOMP_NOINLINE u32 __wctype_classes(u32 c)
{
    if (c > 0xFF) {
        return 0;
    }
    u32 classes = (*__rt_ctype_table())[c];
    if ((classes & (clib::CTYPE_LOWER | clib::CTYPE_UPPER | clib::CTYPE_XDIGIT)) != 0 &&
        (classes & clib::CTYPE_SPACE) == 0) {
        classes |= 0x400;
    }
    if ((classes & (0x400 | clib::CTYPE_DIGIT)) != 0) {
        classes |= 0x800;
    }
    if ((classes & 0xBA) != 0) {
        classes |= 0x100;
    }
    if ((classes & 0xBE) != 0) {
        classes |= 0x200;
    }
    if ((classes & clib::CTYPE_DIGIT) != 0 && (classes & clib::CTYPE_XDIGIT) == 0) {
        classes |= clib::CTYPE_XDIGIT;
    } else {
        classes &= ~clib::CTYPE_DIGIT;
    }
    if (((classes & clib::CTYPE_SPACE) != 0 && (classes & clib::CTYPE_CNTRL) == 0) || c == '\t') {
        classes |= 0x1000;
    }
    return classes;
}

// 0x00101A1A (name is ours, after the C standard)
DECOMP_NOINLINE int iswspace(u32 c)
{
    return __wctype_classes(c) & 1;
}

// 0x001013C8 | manual:abi [tier B]
unsigned long wcstoul(const wchar_t* pString, wchar_t** ppEnd, int base)
{
    bool isNegative = false;
    int* pErrno = __aeabi_errno_addr();
    int saved = *pErrno;
    const wchar_t* p = pString;
    int c;
    do {
        c = *p++;
    } while (c != 0 && iswspace(c));
    if (c != '+') {
        if (c == '-') {
            isNegative = true;
        } else {
            p--;
        }
    }
    *pErrno = 0;
    u32 value = __wcstoul_core(p, ppEnd, base);
    if (ppEnd != NULL && *ppEnd == p) {
        *ppEnd = const_cast<wchar_t*>(pString);
    }
    if (*pErrno == ERROR_RANGE) {
        return value;
    }
    *pErrno = saved;
    if (isNegative) {
        return -value;
    }
    return value;
}

} // extern "C"
