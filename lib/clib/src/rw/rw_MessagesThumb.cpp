// std::messages<char> of the C++ library (Thumb code in the original) with the catalogs of the C library below it
// (catopen/catgets/catclose; in this program catopen always fails) and the error messages from the catalog
// "rwstderr". See rw/rw_Locale.h.

#include "decomp.h"
#include "rw/rw_Exception.h"
#include "rw/rw_Locale.h"
#include "clib/clib_Thumb.h"

extern "C" {
char* strrchr(const char* string, int c);
int sscanf(const char* pString, const char* pFormat, ...);
}

void* operator new(size_t size);
void operator delete(void* p);

inline void* operator new(size_t, void* p)
{
    return p;
}

namespace __rw {
// an open catalog file of the C library (name is ours; the implementation of the program has none)
struct CatalogFile
{
};

// 0x0011C222 (name is ours)
template <>
DECOMP_NOINLINE DECOMP_NOIPA SimpleVector<CatalogFile*>::~SimpleVector()
{
    delete[] m_pData;
}

// the open catalogs of the C library
struct CatalogFileList : public SimpleVector<CatalogFile*>
{
    explicit CatalogFileList(u32 count)
    {
        m_pData = new CatalogFile*[count];
        m_Size = count;
        for (u32 i = count; i != 0; i--) {
            m_pData[i - 1] = NULL;
        }
    }
};

// (the static initializer of this list also registers its destructor)
// 0x0011C160 (static initializer)
// 0x00982FF8 (name is ours)
CatalogFileList s_CatalogFiles(2);

// the index of the catalog, the count if it is not open
// 0x002FF7A8 (name is ours)
DECOMP_NOINLINE DECOMP_NOIPA u32 FindCatalog(int catalog)
{
    CatalogFile** pFiles = s_CatalogFiles.m_pData;
    u32 count = s_CatalogFiles.m_Size;
    u32 i = 0;
    while (i < count && pFiles[i] != NULL && pFiles[i] != reinterpret_cast<CatalogFile*>(catalog)) {
        i++;
    }
    if (pFiles[i] == NULL) {
        return count;
    }
    return i;
}
} // namespace __rw

extern "C" {

// (the opening itself is removed: the new catalog is freed again)
// 0x001063D8 (name is ours, after POSIX)
DECOMP_NOINLINE DECOMP_NOIPA int catopen(const char* pName, int flag)
{
    void* pFile = operator new(sizeof(__rw::CatalogFile));
    operator delete(pFile);
    return -1;
}

// (the lookup is removed: always the default)
// 0x001063B8 (name is ours, after POSIX)
DECOMP_NOINLINE DECOMP_NOIPA char* catgets(int catalog, int set, int id, const char* pDefault)
{
    __rw::FindCatalog(catalog);
    return const_cast<char*>(pDefault);
}

// 0x0010693C (name is ours, after POSIX)
DECOMP_NOINLINE int catclose(int catalog)
{
    u32 i = __rw::FindCatalog(catalog);
    if (i >= __rw::s_CatalogFiles.m_Size) {
        return -1;
    }
    operator delete(__rw::s_CatalogFiles.m_pData[i]);
    __rw::CatalogFile** pFiles = __rw::s_CatalogFiles.m_pData;
    for (i++; pFiles[i] != NULL && i < __rw::s_CatalogFiles.m_Size; i++) {
        pFiles[i - 1] = pFiles[i];
    }
    if (i < __rw::s_CatalogFiles.m_Size) {
        pFiles[i] = NULL;
    }
    return 0;
}

} // extern "C"

// ---- the catalogs of messages

// 0x0011BE08 (name is ours)
DECOMP_NOINLINE __rw::CatalogEntry::CatalogEntry() : m_Catalog(-1), m_Locale(std::locale::classic())
{
}

// 0x0011BE22 (name is ours)
__rw::CatalogEntry::~CatalogEntry()
{
    // (only the members are destroyed)
}

// 0x0031C1C8 (name is ours)
DECOMP_NOINLINE __rw::CatalogEntry& __rw::CatalogEntry::operator=(const CatalogEntry& other)
{
    m_Catalog = other.m_Catalog;
    m_Locale = other.m_Locale;
    return *this;
}

namespace __rw {
// 0x0011C2B8 (name is ours)
template <>
DECOMP_NOINLINE SimpleVector<CatalogEntry>::~SimpleVector()
{
    delete[] m_pData;
}

// 0x0011C238 (name is ours)
template <>
DECOMP_NOINLINE CatalogEntry* SimpleVector<CatalogEntry>::Resize(u32 count)
{
    CatalogEntry fill;
    CatalogEntry* pData = new CatalogEntry[count];
    u32 kept = m_Size < count ? m_Size : count;
    CatalogEntry* pTo = pData;
    for (CatalogEntry* pFrom = m_pData; pFrom != m_pData + kept; pFrom++, pTo++) {
        *pTo = *pFrom;
    }
    for (; kept < count; kept++) {
        pData[kept] = fill;
    }
    delete[] m_pData;
    m_Size = count;
    m_pData = pData;
    return pData;
}

// 0x0031B6F8 (name is ours)
DECOMP_NOINLINE SimpleVector<CatalogEntry>& GetCatalogs()
{
    // 0x00983628 (guard 0x00983624)
    static SimpleVector<CatalogEntry> s_Catalogs = {NULL, 0};
    return s_Catalogs;
}
} // namespace __rw

// 0x0011B7E0 (name is ours)
DECOMP_NOINLINE int std::__rw_messages_base::Open(const string& name, const locale& loc) const
{
    const int LC_MESSAGES = 6;
    int index = -1;
    __rw::LocaleGuard guard(m_LocaleName.c_str(), LC_MESSAGES);
    int catalog = catopen(name.c_str(), -1);
    if (catalog != -1) {
        __rw::SimpleVector<__rw::CatalogEntry>& catalogs = __rw::GetCatalogs();
        __rw::CatalogEntry* pEntry = catalogs.m_pData;
        for (;; pEntry++) {
            if (pEntry == catalogs.m_pData + catalogs.m_Size) {
                catalogs.Resize(catalogs.m_Size + 1);
                pEntry = catalogs.m_pData + catalogs.m_Size - 1;
                break;
            }
            if (pEntry->m_Catalog == -1) {
                break;
            }
        }
        pEntry->m_Catalog = catalog;
        pEntry->m_Locale = loc;
        index = pEntry - catalogs.m_pData;
    }
    return index;
}

// 0x0011B84E (name is ours)
DECOMP_NOINLINE void std::__rw_messages_base::Close(int catalog) const
{
    __rw::SimpleVector<__rw::CatalogEntry>& catalogs = __rw::GetCatalogs();
    if (catalog < static_cast<int>(catalogs.m_Size) && catalogs.m_pData[catalog].m_Catalog != -1) {
        catclose(catalogs.m_pData[catalog].m_Catalog);
        catalogs.m_pData[catalog].m_Catalog = -1;
        catalogs.m_pData[catalog].m_Locale = locale::classic();
    }
}

// 0x0011B88A (name is ours)
DECOMP_NOINLINE const std::locale& std::__rw_messages_base::GetLocale(int catalog) const
{
    __rw::SimpleVector<__rw::CatalogEntry>& catalogs = __rw::GetCatalogs();
    if (catalog < static_cast<int>(catalogs.m_Size)) {
        return catalogs.m_pData[catalog].m_Locale;
    }
    return locale::classic();
}

// 0x0011B8A8 (name is ours)
DECOMP_NOINLINE const char* std::__rw_messages_base::Get(int catalog, int set, int id) const
{
    __rw::SimpleVector<__rw::CatalogEntry>& catalogs = __rw::GetCatalogs();
    if (catalog < static_cast<int>(catalogs.m_Size) && catalogs.m_pData[catalog].m_Catalog != -1) {
        char none = '\0';
        const char* pMessage = catgets(catalogs.m_pData[catalog].m_Catalog, set, id, &none);
        if (pMessage != &none) {
            return pMessage;
        }
    }
    return NULL;
}

// ---- messages<char>

// 0x0011BFFA (name after the C++ standard)
// 0x0011BFEE (deleting dtor)
DECOMP_NOINLINE DECOMP_NOIPA std::messages<char>::~messages()
{
    // (only the bases are destroyed)
}

// 0x0011C43C (name after the C++ standard)
std::messages<char>::catalog std::messages<char>::do_open(const string& name, const locale& loc) const
{
    return Open(name, loc);
}

// (the ctype facet of the catalog's locale is fetched for the conversion of the characters, which is none for char)
// 0x0011C3B0 (name after the C++ standard)
std::string std::messages<char>::do_get(catalog cat, int set, int id, const string& dflt) const
{
    const char* pMessage = Get(cat, set, id);
    if (pMessage != NULL) {
        locale& loc = const_cast<locale&>(GetLocale(cat));
        u32 index = ctype<char>::id.m_Index;
        facet* pFacet = index < loc.m_pBody->m_Facets.m_Size ? loc.m_pBody->m_Facets.m_pData[index] : NULL;
        if (pFacet == NULL) {
            __rw::GetFacet(loc, &ctype<char>::id, true, locale::facet::CATEGORY_CTYPE, __rw::CreateCtypeFacet);
        }
        return string(pMessage);
    }
    return dflt;
}

// 0x0011C446 (name after the C++ standard)
void std::messages<char>::do_close(catalog cat) const
{
    Close(cat);
}

// ---- the messages of the library's errors

namespace {
struct ErrorCatalog
{
    const char* m_pName;    // 0x0 (NULL until the first use)
    int m_Set;              // 0x4
    int m_Catalog;          // 0x8
    char m_DefaultName[11]; // 0xC
    u8 _unknown0x17;
};

// the environment of the program: armlink removed the call of getenv (it has none)
inline const char* GetEnvironment(const char* pName)
{
    return NULL;
}
} // namespace

// 0x00982FB8 (name is ours)
ErrorCatalog s_ErrorCatalog = {NULL, 1, -1, "rwstderr:1", 0xFF};
// the messages facet of the error catalog (constructed in place on the first use)
// 0x00AF87D0 (name is ours)
u32 s_ErrorMessagesStorage[5];

// 0x0010BFB8 (name is ours)
char* __rw::GetErrorMessage(char* pBuffer, size_t size, int id, va_list arguments)
{
    std::messages<char>* pMessages = reinterpret_cast<std::messages<char>*>(s_ErrorMessagesStorage);
    if (s_ErrorCatalog.m_pName == NULL) {
        const char* pName = GetEnvironment("RWSTDERR");
        if (pName == NULL) {
            pName = s_ErrorCatalog.m_DefaultName;
        }
        char* pColon = strrchr(pName, ':');
        if (pColon != NULL) {
            *pColon = '\0';
            if (sscanf(pColon + 1, "%d", &s_ErrorCatalog.m_Set) != 1 || s_ErrorCatalog.m_Set <= 0) {
                s_ErrorCatalog.m_Set = 1;
            }
        }
        new (s_ErrorMessagesStorage) std::messages<char>();
        s_ErrorCatalog.m_Catalog = pMessages->open(std::string(pName), std::locale());
        s_ErrorCatalog.m_pName = pName;
    }
    int catalog = s_ErrorCatalog.m_Catalog;
    if (catalog == -1) {
        return NULL;
    }
    std::string message = pMessages->get(catalog, s_ErrorCatalog.m_Set, id, std::string(""));
    if (message.size() == 0) {
        return NULL;
    }
    return FormatString(pBuffer, size, message.c_str(), arguments);
}
