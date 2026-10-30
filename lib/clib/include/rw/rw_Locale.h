#pragma once

// The locale of the C++ library ARMCC linked in (Rogue Wave; Thumb code in the original). The class names come from
// the RTTI of the binary, the virtual functions are named after the C++ standard; all members and the other names
// are ours. This header must not be used together with the headers of GCC's C++ library.

#include "types.h"

namespace std {
class locale;
} // namespace std

namespace __rw {
class __rw_synchronized
{
};

// the data every facet has (behind the vptr of locale::facet)
class __rw_facet_base : public __rw_synchronized
{
protected:
    u32 m_Category;     // 0x4 (the bit of the locale category)
    u32 _unknown0x8;
    s32 m_RefCount;     // 0xC
};

template <typename T>
class __rw_ctype_helper;

template <>
class __rw_ctype_helper<char>
{
protected:
    u32 _unknown0x0;
};
} // namespace __rw

namespace std {
class locale
{
public:
    class facet;
};

class locale::facet : public __rw::__rw_facet_base
{
public:
    // the categories (bits of m_Category)
    static const u32 CATEGORY_CTYPE = 0x20;

    facet(size_t refs, u32 category)
    {
        m_Category = category;
        _unknown0x8 = 0;
        m_RefCount = refs;
    }

    virtual ~facet();
    // (called with the locale the facet belongs to; name is ours)
    virtual void Initialize(const locale& loc);
};

struct ctype_base
{
    typedef u16 mask;
    static const mask space = 0x1;
    static const mask print = 0x2;
    static const mask cntrl = 0x4;
    static const mask upper = 0x8;
    static const mask lower = 0x10;
    static const mask alpha = 0x20;
    static const mask digit = 0x40;
    static const mask punct = 0x80;
    static const mask xdigit = 0x100;
};

template <typename T>
class ctype;

template <>
class ctype<char> : public locale::facet, public ctype_base, public __rw::__rw_ctype_helper<char>
{
public:
    ctype(const mask* pTable, bool deleteTable, size_t refs);
    virtual ~ctype();
    virtual void Initialize(const locale& loc);

    bool is(mask m, char c) const;

protected:
    virtual const char* do_toupper(char* pLow, const char* pHigh) const;
    virtual const char* do_tolower(char* pLow, const char* pHigh) const;
    virtual char do_toupper(char c) const;
    virtual char do_tolower(char c) const;
    virtual char do_widen(char c) const;
    virtual const char* do_widen(const char* pLow, const char* pHigh, char* pTo) const;
    virtual char do_narrow(char c, char defaultChar) const;
    virtual const char* do_narrow(const char* pLow, const char* pHigh, char defaultChar, char* pTo) const;

private:
    // the characters with the mask: from the first to the last one
    void FindRange(mask m, u32* pFirst, u32* pLast) const;
    // the case conversions of the characters first..last ("a"-"z" with offset -32 or "A"-"Z" with +32)
    void FillCaseTable(char* pTable, u32 first, u32 last, mask from, mask to, char base, int offset) const;

    const mask* m_pTable;   // 0x14 (the classes of the characters 0-255)
    bool m_DeleteTable;     // 0x18
    u32 m_LowerFirst;       // 0x1C (the range of the lower case characters)
    u32 m_LowerLast;        // 0x20
    u32 m_UpperFirst;       // 0x24
    u32 m_UpperLast;        // 0x28
    char* m_pToUpper;       // 0x2C (indexed by c - m_LowerFirst)
    char* m_pToLower;       // 0x30 (indexed by c - m_UpperFirst)
};
} // namespace std
