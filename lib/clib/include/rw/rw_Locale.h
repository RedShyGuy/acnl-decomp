#pragma once

// The locale of the C++ library ARMCC linked in (Rogue Wave; Thumb code in the original). The class names come from
// the RTTI of the binary, the virtual functions are named after the C++ standard; all members and the other names
// are ours. This header must not be used together with the headers of GCC's C++ library.

#include "types.h"
#include "rw/rw_String.h"

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
public:
    static const u32 FACET_INITIALIZED = 0x1;

    u32 m_Category;     // 0x4 (the bit of the locale category)
    u32 m_Flags;        // 0x8 (FACET_INITIALIZED)
    s32 m_RefCount;     // 0xC (the locales that hold it)
};

template <typename T>
class __rw_ctype_helper;

template <>
class __rw_ctype_helper<char>
{
protected:
    u32 _unknown0x0;
};

// the punctuation of numbers
template <typename T>
class __rw_punct_data;

template <>
class __rw_punct_data<char>
{
public:
    ~__rw_punct_data();

protected:
    char m_DecimalPoint;    // 0x0
    char m_ThousandsSep;    // 0x1
    std::string m_Grouping; // 0x4
};

class __rw_numpunct_impl_base
{
};

template <typename T>
class __rw_numpunct_impl_data;

template <>
class __rw_numpunct_impl_data<char> : public __rw_numpunct_impl_base, public __rw_punct_data<char>
{
public:
    ~__rw_numpunct_impl_data();

protected:
    std::string m_TrueName;     // 0x8
    std::string m_FalseName;    // 0xC
};

template <typename T>
class __rw_numpunct_impl;

template <>
class __rw_numpunct_impl<char> : public __rw_numpunct_impl_data<char>
{
protected:
    // the names of the bool values for the input of numbers
    struct BoolName
    {
        const char* m_pName;
        int m_Value;
    };

    BoolName m_BoolNames[2];    // 0x10
    int m_BoolNameCount;        // 0x20
    const void* m_pPunct;       // 0x24 (the data above)
};

// the categories of the C library (LC_*) as the bits of the facets
u32 CategoryMask(int category);

// a pointer and a count (the vectors of the library; names are ours)
template <typename T>
struct SimpleVector
{
    T* m_pData;
    u32 m_Size;

    ~SimpleVector();
    T* At(u32 index);
    // to count elements, the new ones are fill; the new data
    T* Grow(u32 count, const T& fill);
    // the same with default elements (for objects)
    T* Resize(u32 count);
};

class LocaleBody;
} // namespace __rw

namespace std {
class locale
{
public:
    class facet;
    class id
    {
    public:
        u32 m_Index;    // (0 until the first facet of the kind is installed)
    };

    locale();
    locale(const locale& other);
    explicit locale(__rw::LocaleBody* pBody);
    ~locale();
    locale& operator=(const locale& other);

    string name() const;
    static const locale& classic();

    // puts the facet with the id into this locale (initialized on its first use)
    void InstallFacet(facet* pFacet, id* pId);
    // removes the facets of the categories (C categories below 0x10)
    void RemoveFacets(u32 categories);

    __rw::LocaleBody* m_pBody;
};
} // namespace std

namespace __rw {
// the data of a locale, shared by its copies (name is ours)
class LocaleBody
{
public:
    LocaleBody(const char* pName, u32 facetCount, s32 refCount);
    LocaleBody(const LocaleBody& other, const char* pName, s32 refCount);
    ~LocaleBody();

    SimpleVector<std::string> m_Names;              // 0x0 (the names of the six categories)
    SimpleVector<std::locale::facet*> m_Facets;     // 0x8 (indexed by the facet id)
    u32 m_Categories;                               // 0x10 (the categories that are not "C")
    u32 m_NamedCategories;                          // 0x14 (the categories with a name of their own)
    char* m_pName;                                  // 0x18
    s32 m_RefCount;                                 // 0x1C
};

// the locales of the library
extern LocaleBody* s_pClassicBody;  // "C"
extern LocaleBody* s_pNoneBody;     // (empty name, all categories)
extern LocaleBody* s_pGlobalBody;   // (NULL until the first use)
void InitializeLocales();
void FinalizeLocales();
} // namespace __rw

namespace std {

class locale::facet : public __rw::__rw_facet_base
{
public:
    // the categories (bits of m_Category)
    static const u32 CATEGORY_CTYPE = 0x20;

    facet(size_t refs, u32 category)
    {
        m_Category = category;
        m_Flags = 0;
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

    static locale::id id;

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

protected:
    // the case tables again for the classes of the current table (they keep their size)
    void MakeCaseTables();


    const mask* m_pTable;   // 0x14 (the classes of the characters 0-255)
    bool m_DeleteTable;     // 0x18
    u32 m_LowerFirst;       // 0x1C (the range of the lower case characters)
    u32 m_LowerLast;        // 0x20
    u32 m_UpperFirst;       // 0x24
    u32 m_UpperLast;        // 0x28
    char* m_pToUpper;       // 0x2C (indexed by c - m_LowerFirst)
    char* m_pToLower;       // 0x30 (indexed by c - m_UpperFirst)
};

template <typename T>
class numpunct;

template <>
class numpunct<char> : public locale::facet, public __rw::__rw_numpunct_impl<char>
{
public:
    virtual ~numpunct();
    virtual void Initialize(const locale& loc);

protected:
    virtual char do_decimal_point() const;
    virtual char do_thousands_sep() const;
    virtual string do_grouping() const;
    virtual string do_truename() const;
    virtual string do_falsename() const;
};

template <typename T>
class ctype_byname;

template <>
class ctype_byname<char> : public ctype<char>
{
public:
    ctype_byname(const char* pName, size_t refs);
    virtual ~ctype_byname();

    // the classes of the characters of the locale (new[]; the "C" table for "C" and "POSIX")
    static const mask* _C_get_mask_table(const char* pName);
};

struct messages_base
{
    typedef int catalog;
};

class __rw_messages_base : public messages_base
{
public:
    explicit __rw_messages_base(const string& localeName) : m_LocaleName(localeName) {}

    // a catalog of the C library for the locale; its index in the list of open catalogs, or -1
    int Open(const string& name, const locale& loc) const;
    void Close(int catalog) const;
    // the locale of the catalog ("C" if there is none)
    const locale& GetLocale(int catalog) const;
    // the message, or NULL
    const char* Get(int catalog, int set, int id) const;

protected:
    string m_LocaleName;    // 0x0 (the locale the catalogs are opened with)
};

template <typename T>
class messages;

template <>
class messages<char> : public locale::facet, public __rw_messages_base
{
public:
    static const u32 CATEGORY_MESSAGES = 0x200;

    messages(size_t refs = 0) : facet(refs, CATEGORY_MESSAGES), __rw_messages_base(string("C")) {}
    virtual ~messages();

    catalog open(const string& name, const locale& loc) const { return do_open(name, loc); }
    string get(catalog cat, int set, int id, const string& dflt) const { return do_get(cat, set, id, dflt); }
    void close(catalog cat) const { do_close(cat); }

protected:
    virtual catalog do_open(const string& name, const locale& loc) const;
    virtual string do_get(catalog cat, int set, int id, const string& dflt) const;
    virtual void do_close(catalog cat) const;
};
// ---- collate
} // namespace std

namespace __rw {
// the collation weights: ranges of characters with a table of weights or an offset
struct CollateRange
{
    CollateRange* m_pLower;     // 0x0 (the ranges below)
    CollateRange* m_pHigher;    // 0x4 (the ranges above)
    const u8* m_pWeights;       // 0x8 (indexed by c - m_First; NULL: c + m_Offset)
    u8 m_First;                 // 0xC
    u8 m_Last;                  // 0xD
    u8 m_Offset;                // 0xE
};

template <typename T>
class __rw_collate_data;

template <>
class __rw_collate_data<char>
{
};

template <typename T>
class __rw_collate_impl;

template <>
class __rw_collate_impl<char> : public __rw_collate_data<char>
{
public:
    // the weight of the character
    u8 Weight(u8 c) const;

protected:
    const u32* m_pTable;    // 0x0 (a count, then the root range)
};
} // namespace __rw

namespace std {
template <typename T>
class collate;

template <>
class collate<char> : public locale::facet, public __rw::__rw_collate_impl<char>
{
public:
    virtual ~collate();

protected:
    virtual int do_compare(const char* pLow1, const char* pHigh1, const char* pLow2, const char* pHigh2) const;
    virtual string do_transform(const char* pLow, const char* pHigh) const;
    virtual long do_hash(const char* pLow, const char* pHigh) const;
};

template <typename T>
class collate_byname;

template <>
class collate_byname<char> : public collate<char>
{
public:
    virtual ~collate_byname();

protected:
    virtual int do_compare(const char* pLow1, const char* pHigh1, const char* pLow2, const char* pHigh2) const;
    virtual string do_transform(const char* pLow, const char* pHigh) const;

    string m_Name;  // 0x14
};

// ---- codecvt

struct __mbstate_t
{
    u32 m_State;
    u32 _unknown4;
};

struct codecvt_base
{
    enum result
    {
        ok,
        partial,
        error,
        noconv,
    };
};

template <typename I, typename E, typename S>
class codecvt;

template <>
class codecvt<wchar_t, char, __mbstate_t> : public locale::facet, public codecvt_base
{
public:
    typedef __mbstate_t state_type;

protected:
    virtual result do_out(state_type& state, const wchar_t* pFrom, const wchar_t* pFromEnd, const wchar_t*& pFromNext,
                          char* pTo, char* pToEnd, char*& pToNext) const = 0;
    virtual result do_in(state_type& state, const char* pFrom, const char* pFromEnd, const char*& pFromNext,
                         wchar_t* pTo, wchar_t* pToEnd, wchar_t*& pToNext) const = 0;
    virtual result do_unshift(state_type& state, char* pTo, char* pToEnd, char*& pToNext) const = 0;
    virtual bool do_always_noconv() const = 0;
    virtual int do_encoding() const = 0;
    virtual int do_length(state_type& state, const char* pFrom, const char* pFromEnd, size_t max) const = 0;
    virtual int do_max_length() const = 0;
};

template <typename I, typename E, typename S>
class codecvt_byname;

template <>
class codecvt_byname<wchar_t, char, __mbstate_t> : public codecvt<wchar_t, char, __mbstate_t>
{
public:
    virtual ~codecvt_byname();

protected:
    virtual result do_out(state_type& state, const wchar_t* pFrom, const wchar_t* pFromEnd, const wchar_t*& pFromNext,
                          char* pTo, char* pToEnd, char*& pToNext) const;
    virtual result do_in(state_type& state, const char* pFrom, const char* pFromEnd, const char*& pFromNext,
                         wchar_t* pTo, wchar_t* pToEnd, wchar_t*& pToNext) const;
    virtual result do_unshift(state_type& state, char* pTo, char* pToEnd, char*& pToNext) const;
    virtual bool do_always_noconv() const;
    virtual int do_encoding() const;
    virtual int do_length(state_type& state, const char* pFrom, const char* pFromEnd, size_t max) const;
    virtual int do_max_length() const;
};
} // namespace std

namespace __rw {
// sets the C library to a locale for its lifetime (name is ours)
class LocaleGuard
{
public:
    LocaleGuard(const char* pName, int category);
    ~LocaleGuard();

private:
    int m_Category;     // 0x0
    char* m_pSaved;     // 0x4 (the name before, new[])
};

// creates a facet: kind 0 "C", 1 "C" with a reference, 2 by name
typedef std::locale::facet* (*FacetFactory)(int kind, const char* pName, size_t refs);

// the facet with the id in the locale; made with the factory (and also put into the "C" body) if it is missing;
// without create a missing facet is an error
std::locale::facet* GetFacet(std::locale& loc, std::locale::id* pId, bool create, u32 category,
                             FacetFactory factory);
std::locale::facet* CreateCtypeFacet(int kind, const char* pName, size_t refs);

// an open catalog of __rw_messages_base and the locale it was opened with
struct CatalogEntry
{
    int m_Catalog;          // 0x0 (-1: free)
    std::locale m_Locale;   // 0x4

    CatalogEntry();
    ~CatalogEntry();
    CatalogEntry& operator=(const CatalogEntry& other);
};

SimpleVector<CatalogEntry>& GetCatalogs();
} // namespace __rw
