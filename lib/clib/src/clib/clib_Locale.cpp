#include "clib/clib_Locale.h"
#include "decomp.h"
#include <cstring>

// The locale state of the C library and the lookup of the "C" locale categories.

namespace clib {
// (name is ours)
// 0x00AF86C8
CLibraryLocale s_CLibraryLocale;
} // namespace clib

using namespace clib;

extern "C" {
// 0x002F922C (name is ours)
DECOMP_NOINLINE CLibraryLocale* GetCLibraryLocale()
{
    return &s_CLibraryLocale;
}

// 0x002F7A54 | manual:abi [tier B]
const u8** __rt_ctype_table()
{
    return &GetCLibraryLocale()->m_pCtypeClasses;
}

// the character classes of a locale (only "C"; an empty name or none is "C")
// 0x00142770 (name is ours)
const u8* GetCtypeLocale(int category, const char* pName)
{
    if (pName != nullptr && pName[0] != '\0' && strcmp(s_CtypeLocale.m_Name, pName) != 0) {
        return nullptr;
    }
    return s_CtypeLocale.m_Data.m_Classes;
}

// the numeric formats of a locale (only "C")
// 0x0011DC08 (name is ours)
const NumericData* GetNumericLocale(int category, const char* pName)
{
    if (pName != nullptr && pName[0] != '\0' && strcmp(s_NumericLocale.m_Name, pName) != 0) {
        return nullptr;
    }
    return &s_NumericLocale.m_Data;
}
} // extern "C"
