#pragma once

#include "types.h"

// The "C" locale of the C library of the program (the ARM C library ARMCC linked in). nninitLocale
// stores the character classes and the numeric formats in the locale state; the character functions
// of the C library read the classes through __rt_ctype_table. Names are ours except __rt_ctype_table.

namespace clib {
// the flags of the character classes
const u8 CTYPE_SPACE = 0x01;    // white space
const u8 CTYPE_PUNCT = 0x02;    // punctuation
const u8 CTYPE_BLANK = 0x04;    // the space character
const u8 CTYPE_LOWER = 0x08;
const u8 CTYPE_UPPER = 0x10;
const u8 CTYPE_DIGIT = 0x20;
const u8 CTYPE_CNTRL = 0x40;
const u8 CTYPE_XDIGIT = 0x80;   // the letters of hexadecimal digits

// a block of a locale category: its size, its name and the data
template <typename T>
struct LocaleBlock
{
    u32 m_Size;            // 0x0 (of the whole block)
    char m_Name[4];        // 0x4
    s32 m_NameOffset;      // 0x8 (from the data to the name: -8)
    T m_Data;              // 0xC
};

// the character classes: EOF, then the characters 0-255
struct CtypeData
{
    u8 m_Classes[1 + 256];
    u8 _padding[3];
};

// the numeric formats: the strings at offsets from the data
struct NumericData
{
    s32 m_DecimalPointOffset;       // 0x0
    s32 m_ThousandsSeparatorOffset; // 0x4
    s32 m_GroupingOffset;           // 0x8
    char m_Strings[4];              // 0xC (".", then the empty separator and grouping)
};

// the locale state of the C library (only the categories known)
struct CLibraryLocale
{
    u32 _unknown0x0;
    const u8* m_pCtypeClasses;      // 0x4 (from the class of EOF on: indexed by the character)
    u32 _unknown0x8;
    const NumericData* m_pNumeric;  // 0xC
};

extern const LocaleBlock<CtypeData> s_CtypeLocale;
extern const LocaleBlock<NumericData> s_NumericLocale;
extern CLibraryLocale s_CLibraryLocale;
} // namespace clib

extern "C" {
clib::CLibraryLocale* GetCLibraryLocale(); // 0x002F922C (name is ours)
const u8* GetCtypeLocale(int category, const char* pName); // 0x00142770 (name is ours)
const clib::NumericData* GetNumericLocale(int category, const char* pName); // 0x0011DC08 (name is ours)
const u8** __rt_ctype_table(); // 0x002F7A54 | manual:abi [tier B]
}
