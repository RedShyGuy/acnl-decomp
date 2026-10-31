// setlocale of the ARM C library (Thumb code in the original): the state holds one word per category; a single
// category is named after its data block, several as "*" and the five words in hexadecimal, which setlocale also
// takes back. Only the "C" locale exists. Names except setlocale are ours.

#include "decomp.h"
#include "clib/clib_Locale.h"
#include "clib/clib_Thumb.h"

using namespace clib;

namespace {
const int CATEGORY_COUNT = 5;

// the loaders of the categories without data: armlink replaced their calls (empty functions) by nops
inline const void* GetMonetaryLocale(const char* pDefault, const char* pName)
{
    return pDefault;
}

inline const void* GetTimeLocale(const char* pDefault, const char* pName)
{
    return pDefault;
}

inline u32* StateWords()
{
    return reinterpret_cast<u32*>(GetCLibraryLocale());
}
} // namespace

extern "C" {

// the name setlocale returns
// 0x00AF87EC (name is ours)
char s_LocaleName[44];

// 0x002FAC38 (name is ours)
const char* __locale_name(char* pBuffer, int categories)
{
    if ((categories & ~(categories & -categories)) != 0) {
        char* p = pBuffer;
        *p++ = '*';
        const u32* pWords = StateWords();
        for (int i = 0; i < CATEGORY_COUNT; i++) {
            u32 word = pWords[i];
            for (int j = 0; j < 8; j++) {
                u32 digit = word >> 28;
                word <<= 4;
                *p++ = digit <= 9 ? digit + '0' : digit + 'a' - 10;
            }
        }
        *p = '\0';
        return pBuffer;
    }
    const void* pData;
    switch (categories) {
    case 1:
        pData = GetCLibraryLocale()->m_pCollate;
        break;
    case 2:
        pData = GetCLibraryLocale()->m_pCtypeClasses;
        break;
    case 4:
        pData = GetCLibraryLocale()->m_pMonetary;
        break;
    case 8:
        pData = GetCLibraryLocale()->m_pNumeric;
        break;
    case 16:
        pData = GetCLibraryLocale()->m_pTime;
        break;
    default:
        return NULL;
    }
    if (pData == NULL) {
        return "C";
    }
    // the name of the block of the data (the character classes are one byte into theirs)
    const s32* pBlockData = reinterpret_cast<const s32*>(reinterpret_cast<uptr>(pData) & ~3);
    return reinterpret_cast<const char*>(pBlockData) + pBlockData[-1];
}

// 0x002FAD3A (name is ours)
const char* __set_locale_by_name(char* pBuffer, int categories, const char* pName)
{
    if ((categories & 2) != 0) {
        const u8* pClasses = GetCtypeLocale("", pName);
        if (pClasses == NULL) {
            return NULL;
        }
        GetCLibraryLocale()->m_pCtypeClasses = pClasses + 1;
    }
    if ((categories & 1) != 0) {
        const CollateData* pCollate = GetCollateLocale("", pName);
        if (pCollate == NULL) {
            return NULL;
        }
        GetCLibraryLocale()->m_pCollate = reinterpret_cast<const s32*>(pCollate);
    }
    if ((categories & 4) != 0) {
        const void* pMonetary = GetMonetaryLocale("C", pName);
        if (pMonetary == NULL) {
            return NULL;
        }
        GetCLibraryLocale()->m_pMonetary = pMonetary;
    }
    if ((categories & 8) != 0) {
        const NumericData* pNumeric = GetNumericLocale("", pName);
        if (pNumeric == NULL) {
            return NULL;
        }
        GetCLibraryLocale()->m_pNumeric = pNumeric;
    }
    if ((categories & 16) != 0) {
        const void* pTime = GetTimeLocale("", pName);
        if (pTime == NULL) {
            return NULL;
        }
        GetCLibraryLocale()->m_pTime = pTime;
    }
    return __locale_name(pBuffer, categories);
}

// pName: NULL asks for the name, "*..." sets the state words of the categories
// 0x002FACD6 (name is ours)
const char* __set_locale(char* pBuffer, int categories, const char* pName)
{
    if (categories == 0 || (categories >> 5) != 0) {
        return NULL;
    }
    if (pName == NULL) {
        return __locale_name(pBuffer, categories);
    }
    if (pName[0] != '*') {
        return __set_locale_by_name(pBuffer, categories, pName);
    }
    const char* p = pName + 1;
    for (int i = 0; i < CATEGORY_COUNT; i++) {
        u32 word = 0;
        for (int j = 0; j < 8; j++) {
            u32 c = static_cast<u8>(*p++);
            word = (word << 4) | (c <= '9' ? c - '0' : c - 'a' + 10);
        }
        if ((categories & (1 << i)) != 0) {
            StateWords()[i] = word;
        }
    }
    return __locale_name(pBuffer, categories);
}

// categories: bits 1 LC_COLLATE, 2 LC_CTYPE, 4 LC_MONETARY, 8 LC_NUMERIC, 16 LC_TIME
// 0x00307988 (name is ours, after the C standard)
char* setlocale(int categories, const char* pName)
{
    return const_cast<char*>(__set_locale(s_LocaleName, categories, pName));
}

} // extern "C"
