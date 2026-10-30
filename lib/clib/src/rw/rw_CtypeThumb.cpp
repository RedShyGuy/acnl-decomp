// std::ctype<char> of the C++ library (Thumb code in the original): the character classes from a mask table (the
// "C" table by default) and case tables for the range of lower and upper case characters. See rw/rw_Locale.h.

#include "decomp.h"
#include "rw/rw_Locale.h"
#include "clib/clib_Thumb.h"

extern "C" void* memcpy(void* destination, const void* source, size_t size);

namespace __rw {
// the classes of the characters of the "C" locale (ASCII; nothing above 0x7F)
// 0x008C9670 (name is ours)
extern const std::ctype_base::mask s_ClassicCtypeTable[256] = {
    0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x005, 0x005, 0x005, 0x005, 0x005, 0x004, 0x004,
    0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004,
    0x003, 0x082, 0x082, 0x082, 0x082, 0x082, 0x082, 0x082, 0x082, 0x082, 0x082, 0x082, 0x082, 0x082, 0x082, 0x082,
    0x142, 0x142, 0x142, 0x142, 0x142, 0x142, 0x142, 0x142, 0x142, 0x142, 0x082, 0x082, 0x082, 0x082, 0x082, 0x082,
    0x082, 0x12A, 0x12A, 0x12A, 0x12A, 0x12A, 0x12A, 0x02A, 0x02A, 0x02A, 0x02A, 0x02A, 0x02A, 0x02A, 0x02A, 0x02A,
    0x02A, 0x02A, 0x02A, 0x02A, 0x02A, 0x02A, 0x02A, 0x02A, 0x02A, 0x02A, 0x02A, 0x082, 0x082, 0x082, 0x082, 0x082,
    0x082, 0x132, 0x132, 0x132, 0x132, 0x132, 0x132, 0x032, 0x032, 0x032, 0x032, 0x032, 0x032, 0x032, 0x032, 0x032,
    0x032, 0x032, 0x032, 0x032, 0x032, 0x032, 0x032, 0x032, 0x032, 0x032, 0x032, 0x082, 0x082, 0x082, 0x082, 0x004,
};
} // namespace __rw

// ---- locale::facet

// 0x0011BFB0 (name after the C++ standard)
// 0x0011BFA8 (deleting dtor)
std::locale::facet::~facet()
{
    // empty (in the original too)
}

// 0x0010C130 (name is ours)
void std::locale::facet::Initialize(const locale& loc)
{
    // empty (in the original too)
}

// ---- ctype<char>

// 0x0031B7B4 (name after the C++ standard)
DECOMP_NOINLINE bool std::ctype<char>::is(mask m, char c) const
{
    return (m_pTable[static_cast<u8>(c)] & m) != 0;
}

inline void std::ctype<char>::FindRange(mask m, u32* pFirst, u32* pLast) const
{
    u32 first = 0;
    u32 last = 255;
    for (; first < last; first++) {
        if (is(m, first)) {
            for (; last > first; last--) {
                if (is(m, last)) {
                    break;
                }
            }
            break;
        }
    }
    *pLast = last;
    *pFirst = first;
}

inline void std::ctype<char>::FillCaseTable(char* pTable, u32 first, u32 last, mask from, mask to, char base,
                                            int offset) const
{
    for (u32 c = first; c <= last; c++) {
        char converted = c;
        if (c - base <= 25 && is(from, c)) {
            char other = c + offset;
            if (is(to, other)) {
                converted = other;
            }
        }
        *pTable++ = converted;
    }
}

// pTable: the classes of the characters 0-255 (NULL: the "C" table)
// 0x0031BAE4 (name after the C++ standard)
std::ctype<char>::ctype(const mask* pTable, bool deleteTable, size_t refs) : facet(refs, CATEGORY_CTYPE)
{
    m_pTable = pTable;
    m_DeleteTable = deleteTable;
    if (pTable == NULL) {
        m_pTable = __rw::s_ClassicCtypeTable;
        m_DeleteTable = false;
    }
    FindRange(lower, &m_LowerFirst, &m_LowerLast);
    m_pToUpper = new char[m_LowerLast - m_LowerFirst + 1];
    FillCaseTable(m_pToUpper, m_LowerFirst, m_LowerLast, lower, upper, 'a', -32);
    FindRange(upper, &m_UpperFirst, &m_UpperLast);
    m_pToLower = new char[m_UpperLast - m_UpperFirst + 1];
    FillCaseTable(m_pToLower, m_UpperFirst, m_UpperLast, upper, lower, 'A', 32);
}

// 0x0031BC18 (name after the C++ standard)
// 0x0011BF48 (deleting dtor)
std::ctype<char>::~ctype()
{
    if (m_DeleteTable) {
        delete[] m_pTable;
    }
    delete[] m_pToUpper;
    delete[] m_pToLower;
}

// the case tables again for the character classes of the locale (the tables keep their size)
// 0x0011BE2E (name is ours)
void std::ctype<char>::Initialize(const locale& loc)
{
    // (the original sets the C library to the locale's name for the time of this: needs std::string and
    // setlocale, not decompiled yet)
}

// 0x0011B92C (name after the C++ standard)
const char* std::ctype<char>::do_toupper(char* pLow, const char* pHigh) const
{
    for (; pLow < pHigh; pLow++) {
        if (is(lower, *pLow)) {
            *pLow = m_pToUpper[static_cast<u8>(*pLow) - m_LowerFirst];
        }
    }
    return pLow;
}

// 0x0011B8DE (name after the C++ standard)
const char* std::ctype<char>::do_tolower(char* pLow, const char* pHigh) const
{
    for (; pLow < pHigh; pLow++) {
        if (is(upper, *pLow)) {
            *pLow = m_pToLower[static_cast<u8>(*pLow) - m_UpperFirst];
        }
    }
    return pLow;
}

// 0x0011B95A (name after the C++ standard)
char std::ctype<char>::do_toupper(char c) const
{
    if (is(lower, c)) {
        return m_pToUpper[static_cast<u8>(c) - m_LowerFirst];
    }
    return c;
}

// 0x0011B90C (name after the C++ standard)
char std::ctype<char>::do_tolower(char c) const
{
    if (is(upper, c)) {
        return m_pToLower[static_cast<u8>(c) - m_UpperFirst];
    }
    return c;
}

// 0x0011B98A (name after the C++ standard)
char std::ctype<char>::do_widen(char c) const
{
    return c;
}

// 0x0011B97A (name after the C++ standard)
const char* std::ctype<char>::do_widen(const char* pLow, const char* pHigh, char* pTo) const
{
    memcpy(pTo, pLow, pHigh - pLow);
    return pHigh;
}

// 0x0011B99E (name after the C++ standard)
char std::ctype<char>::do_narrow(char c, char defaultChar) const
{
    return c;
}

// 0x0011B98E (name after the C++ standard)
const char* std::ctype<char>::do_narrow(const char* pLow, const char* pHigh, char defaultChar, char* pTo) const
{
    memcpy(pTo, pLow, pHigh - pLow);
    return pHigh;
}
