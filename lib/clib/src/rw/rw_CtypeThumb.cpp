// std::ctype<char> of the C++ library (Thumb code in the original): the character classes from a mask table (the
// "C" table by default) and case tables for the range of lower and upper case characters. See rw/rw_Locale.h.

#include "decomp.h"
#include "rw/rw_Locale.h"
#include "clib/clib_Locale.h"
#include "clib/clib_Thumb.h"

extern "C" {
void* memcpy(void* destination, const void* source, size_t size);
int strcmp(const char* a, const char* b);
size_t strlen(const char* string);
char* strcpy(char* destination, const char* source);
char* setlocale(int categories, const char* pName);
}

namespace {
// the categories of the C library (setlocale)
const int LC_CTYPE = 2;
} // namespace

namespace __rw {
// 0x0031B59E (name is ours)
DECOMP_NOINLINE LocaleGuard::LocaleGuard(const char* pName, int category)
{
    m_Category = category;
    m_pSaved = setlocale(category, NULL);
    if (m_pSaved != NULL) {
        char* pCopy = new char[strlen(m_pSaved) + 1];
        strcpy(pCopy, m_pSaved);
        m_pSaved = pCopy;
    }
    setlocale(category, pName);
}

// 0x0031B5D6 (name is ours)
DECOMP_NOINLINE LocaleGuard::~LocaleGuard()
{
    if (m_pSaved != NULL) {
        setlocale(m_Category, m_pSaved);
        delete[] m_pSaved;
    }
}

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

// 0x00982FD0 (name after the C++ standard)
std::locale::id std::ctype<char>::id;

// 0x0031B7B4 (name after the C++ standard)
DECOMP_NOINLINE bool std::ctype<char>::is(mask m, char c) const
{
    return (m_pTable[static_cast<u8>(c)] & m) != 0;
}

DECOMP_ALWAYS_INLINE void std::ctype<char>::FindRange(mask m, u32* pFirst, u32* pLast) const
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

DECOMP_ALWAYS_INLINE void std::ctype<char>::FillCaseTable(char* pTable, u32 first, u32 last, mask from, mask to, char base,
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

DECOMP_ALWAYS_INLINE void std::ctype<char>::MakeCaseTables()
{
    FindRange(lower, &m_LowerFirst, &m_LowerLast);
    FillCaseTable(m_pToUpper, m_LowerFirst, m_LowerLast, lower, upper, 'a', -32);
    FindRange(upper, &m_UpperFirst, &m_UpperLast);
    FillCaseTable(m_pToLower, m_UpperFirst, m_UpperLast, upper, lower, 'A', 32);
}

// the case tables again for the character classes of the locale (the tables keep their size)
// 0x0011BE2E (name is ours)
void std::ctype<char>::Initialize(const locale& loc)
{
    __rw::LocaleGuard guard(loc.name().c_str(), LC_CTYPE);
    MakeCaseTables();
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

// ---- ctype_byname<char>

// 0x0011BAF4 | libgarden (name after the reference symbols; symbols.json had it at 0x00158E20)
DECOMP_NOINLINE const std::ctype_base::mask* std::ctype_byname<char>::_C_get_mask_table(const char* pName)
{
    if (strcmp(pName, "C") == 0 || strcmp(pName, "POSIX") == 0) {
        return __rw::s_ClassicCtypeTable;
    }
    mask* pTable = new mask[256];
    __rw::LocaleGuard guard(pName, LC_CTYPE);
    for (int c = 0; c != 256; c++) {
        // (the class macros of the C library)
        u8 classes = (*__rt_ctype_table())[c];
        mask m = (classes & clib::CTYPE_SPACE) != 0 ? space : 0;
        if ((classes & (clib::CTYPE_PUNCT | clib::CTYPE_BLANK | clib::CTYPE_LOWER | clib::CTYPE_UPPER |
                        clib::CTYPE_DIGIT)) != 0) {
            m |= print;
        }
        if ((classes & clib::CTYPE_CNTRL) != 0) {
            m |= cntrl;
        }
        if ((classes & clib::CTYPE_UPPER) != 0) {
            m |= upper;
        }
        if ((classes & clib::CTYPE_LOWER) != 0) {
            m |= lower;
        }
        if ((classes & (clib::CTYPE_LOWER | clib::CTYPE_UPPER | clib::CTYPE_XDIGIT)) != 0) {
            m |= alpha;
        }
        if (classes == clib::CTYPE_DIGIT) {
            m |= digit;
        }
        if ((classes & clib::CTYPE_PUNCT) != 0) {
            m |= punct;
        }
        if ((((classes << 2) ^ classes) & 0x80) != 0) {
            m |= xdigit;
        }
        pTable[c] = m;
    }
    return pTable;
}

// 0x0011BBC0 (name after the C++ standard)
std::ctype_byname<char>::ctype_byname(const char* pName, size_t refs) : ctype<char>(_C_get_mask_table(pName), false, refs)
{
    if (m_pTable != __rw::s_ClassicCtypeTable) {
        m_DeleteTable = true;
        __rw::LocaleGuard guard(pName, LC_CTYPE);
        MakeCaseTables();
    }
}

// 0x0011BD0C (name after the C++ standard)
// 0x0011BD00 (deleting dtor)
std::ctype_byname<char>::~ctype_byname()
{
    // (only the base is destroyed)
}

// ---- getting facets

// 0x0011C1CC (name is ours)
std::locale::facet* __rw::CreateCtypeFacet(int kind, const char* pName, size_t refs)
{
    switch (kind) {
    case 0:
        return new std::ctype<char>(NULL, false, 0);
    case 1:
        return new std::ctype<char>(NULL, false, 1);
    default:
        return new std::ctype_byname<char>(pName, refs);
    }
}

namespace {
// the facet of the id in the body (the facets grow to the id first)
inline std::locale::facet* FindFacet(__rw::LocaleBody* pBody, std::locale::id* pId)
{
    u32 index = pId->m_Index;
    if (pBody->m_Facets.m_Size <= index) {
        std::locale::facet* none = NULL;
        pBody->m_Facets.Grow(index + 1, none);
    }
    return pBody->m_Facets.m_pData[index];
}
} // namespace

// 0x0011B9A4 (name is ours)
DECOMP_NOINLINE std::locale::facet* __rw::GetFacet(std::locale& loc, std::locale::id* pId, bool create, u32 category,
                                   FacetFactory factory)
{
    // the body that holds the shared facets of the category: none for a named one
    LocaleBody* pShared;
    if ((loc.m_pBody->m_NamedCategories & category) != 0) {
        pShared = NULL;
    } else if ((loc.m_pBody->m_Categories & category) != 0) {
        pShared = s_pNoneBody;
    } else {
        pShared = s_pClassicBody;
    }
    std::locale::facet* pFacet;
    if (pShared != NULL && (pFacet = FindFacet(pShared, pId)) != NULL) {
        loc.InstallFacet(pFacet, pId);
        return pFacet;
    }
    if (!create) {
        ThrowError(ERROR_BAD_CAST);
    }
    if (pShared != NULL && (pFacet = FindFacet(pShared, pId)) != NULL) {
        loc.InstallFacet(pFacet, pId);
        return pFacet;
    }
    int kind = 0;
    const char* pName = "";
    if ((loc.m_pBody->m_NamedCategories & category) != 0) {
        kind = 2;
        u32 bit = 0x10;
        for (u32 i = 0; i < 6; i++, bit <<= 1) {
            if ((category & bit) != 0) {
                pName = loc.m_pBody->m_Names.m_pData[i].c_str();
                break;
            }
        }
    } else if ((loc.m_pBody->m_Categories & category) != 0) {
        kind = 1;
    }
    pFacet = factory(kind, pName, 0);
    if (pShared != NULL && loc.m_pBody != pShared) {
        // the shared body keeps the new facet, too (without keeping a reference of the locale)
        std::locale shared(pShared);
        shared.InstallFacet(pFacet, pId);
        shared.m_pBody = NULL;
        pShared->m_RefCount--;
    }
    loc.InstallFacet(pFacet, pId);
    return pFacet;
}
