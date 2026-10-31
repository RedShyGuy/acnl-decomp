// std::locale of the C++ library (Thumb code in the original): a locale holds a reference counted body with the
// names of the categories and the facets, indexed by the ids of their kinds. The "C" locale is created on the first
// use. See rw/rw_Locale.h.

#include "decomp.h"
#include "rw/rw_Locale.h"
#include "clib/clib_Thumb.h"

extern "C" {
size_t strlen(const char* string);
void* memcpyWithResult(void* destination, const void* source, size_t size);
}

namespace __rw {
// 0x00982FE0 (name is ours)
LocaleBody* s_pClassicBody;
// 0x00982FE4 (name is ours)
LocaleBody* s_pNoneBody;
// 0x00982FE8 (name is ours)
LocaleBody* s_pGlobalBody;
// the last id given to a kind of facet
// 0x00983620 (name is ours)
u32 s_FacetIdCount;

// 0x0010C132 (name is ours)
u32 CategoryMask(int category)
{
    switch (category) {
    case 1:
        return 0x10;
    case 2:
        return 0x20;
    case 4:
        return 0x40;
    case 8:
        return 0x80;
    case 16:
        return 0x100;
    case 31:
        return 0x3F0;
    default:
        return 0;
    }
}

// 0x0031B8E4 (name is ours)
template <>
DECOMP_NOINLINE std::locale::facet** SimpleVector<std::locale::facet*>::At(u32 index)
{
    return &m_pData[index];
}

// 0x0031B7C2 (name is ours)
template <>
DECOMP_NOINLINE std::locale::facet** SimpleVector<std::locale::facet*>::Grow(u32 count, std::locale::facet* const& fill)
{
    std::locale::facet** pData = new std::locale::facet*[count];
    u32 kept = m_Size < count ? m_Size : count;
    std::locale::facet** pTo = pData;
    for (std::locale::facet** pFrom = m_pData; pFrom != m_pData + kept;) {
        *pTo++ = *pFrom++;
    }
    for (; kept < count; kept++) {
        pData[kept] = fill;
    }
    delete[] m_pData;
    m_pData = pData;
    m_Size = count;
    return pData;
}

// 0x0011C214 (name is ours)
template <>
DECOMP_NOINLINE DECOMP_NOIPA SimpleVector<std::locale::facet*>::~SimpleVector()
{
    delete[] m_pData;
}

// 0x003076EC (name is ours)
template <>
DECOMP_NOINLINE DECOMP_NOIPA SimpleVector<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >::~SimpleVector()
{
    delete[] m_pData;
}

// 0x0011C230 (name is ours)
template <>
DECOMP_NOINLINE std::basic_string<char, std::char_traits<char>, std::allocator<char> >* SimpleVector<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >::At(u32 index)
{
    return &m_pData[index];
}

// the names of the categories start empty (six of them)
// 0x0031BE34 (name is ours)
LocaleBody::LocaleBody(const char* pName, u32 facetCount, s32 refCount)
{
    m_Names.m_pData = NULL;
    m_Names.m_Size = 0;
    m_Facets.m_pData = facetCount != 0 ? new std::locale::facet*[facetCount] : NULL;
    m_Facets.m_Size = facetCount;
    for (u32 i = facetCount; i != 0; i--) {
        m_Facets.m_pData[i - 1] = NULL;
    }
    m_Categories = 0;
    m_NamedCategories = 0;
    m_pName = NULL;
    m_RefCount = refCount;
    if (pName != NULL) {
        size_t size = strlen(pName) + 1;
        m_pName = static_cast<char*>(memcpyWithResult(new char[size], pName, size));
    }
    const u32 NAME_COUNT = 6;
    std::string empty;
    std::string* pNames = new std::string[NAME_COUNT];
    u32 kept = m_Names.m_Size < NAME_COUNT ? m_Names.m_Size : NAME_COUNT;
    std::string* pTo = pNames;
    for (std::string* pFrom = m_Names.m_pData; pFrom != m_Names.m_pData + kept; pFrom++, pTo++) {
        *pTo = *pFrom;
    }
    for (; kept < NAME_COUNT; kept++) {
        pNames[kept] = empty;
    }
    delete[] m_Names.m_pData;
    m_Names.m_pData = pNames;
    m_Names.m_Size = NAME_COUNT;
}

// a copy of other with its own name; the facets get one more reference
// 0x0010C170 (name is ours)
LocaleBody::LocaleBody(const LocaleBody& other, const char* pName, s32 refCount)
{
    m_Names.m_pData = other.m_Names.m_Size != 0 ? new std::string[other.m_Names.m_Size] : NULL;
    m_Names.m_Size = other.m_Names.m_Size;
    for (u32 i = m_Names.m_Size; i-- != 0;) {
        m_Names.m_pData[i] = other.m_Names.m_pData[i];
    }
    m_Facets.m_pData = other.m_Facets.m_Size != 0 ? new std::locale::facet*[other.m_Facets.m_Size] : NULL;
    m_Facets.m_Size = other.m_Facets.m_Size;
    for (u32 i = m_Facets.m_Size; i-- != 0;) {
        m_Facets.m_pData[i] = other.m_Facets.m_pData[i];
    }
    m_Categories = other.m_Categories;
    m_NamedCategories = other.m_NamedCategories;
    m_pName = NULL;
    m_RefCount = refCount;
    if (pName != NULL) {
        size_t size = strlen(pName) + 1;
        m_pName = static_cast<char*>(memcpyWithResult(new char[size], pName, size));
    }
    for (u32 i = m_Facets.m_Size; i-- != 0;) {
        std::locale::facet* pFacet = m_Facets.m_pData[i];
        if (pFacet != NULL) {
            pFacet->m_RefCount++;
        }
    }
}

// 0x0031BF1C (name is ours)
DECOMP_NOINLINE LocaleBody::~LocaleBody()
{
    delete[] m_pName;
}

// 0x0031BF38 (name is ours)
DECOMP_NOIPA void InitializeLocales()
{
    if (s_pGlobalBody != NULL) {
        return;
    }
    s_pClassicBody = new LocaleBody("C", 10, 2);
    u32 i = 0;
    for (u32 bit = 0x10; ((bit >> 4) & 0x3F) != 0; bit <<= 1) {
        *s_pClassicBody->m_Names.At(i) = s_pClassicBody->m_pName;
        i++;
    }
    s_pNoneBody = new LocaleBody("", 10, 1);
    s_pNoneBody->m_Categories = 0x3F0;
    s_pGlobalBody = s_pClassicBody;
}

// (the "C" body; the others stay)
// 0x0031BF96 (name is ours)
void FinalizeLocales()
{
    delete s_pClassicBody;
    s_pGlobalBody = NULL;
    s_pClassicBody = NULL;
}
} // namespace __rw

// 0x0011BFC0 (name after the C++ standard)
std::locale::locale()
{
    if (__rw::s_pGlobalBody == NULL) {
        __rw::InitializeLocales();
    }
    m_pBody = __rw::s_pGlobalBody;
    m_pBody->m_RefCount++;
}

// 0x0011BFB2 (name after the C++ standard)
std::locale::locale(const locale& other)
{
    m_pBody = other.m_pBody;
    m_pBody->m_RefCount++;
}

// 0x0031B80C (name after the C++ standard)
std::locale::locale(__rw::LocaleBody* pBody)
{
    m_pBody = pBody;
    if (__rw::s_pGlobalBody == NULL) {
        __rw::InitializeLocales();
    }
    m_pBody->m_RefCount++;
}

// 0x0031B8B0 (name after the C++ standard)
std::locale::~locale()
{
    if (m_pBody != NULL && --m_pBody->m_RefCount == 0) {
        delete m_pBody;
    }
}

// 0x0031B738 (name after the C++ standard)
std::locale& std::locale::operator=(const locale& other)
{
    if (m_pBody != other.m_pBody) {
        other.m_pBody->m_RefCount++;
        if (--m_pBody->m_RefCount == 0) {
            delete m_pBody;
        }
        m_pBody = other.m_pBody;
    }
    return *this;
}

// 0x0011BAAC (name after the C++ standard)
std::string std::locale::name() const
{
    if (m_pBody->m_pName != NULL) {
        return string(m_pBody->m_pName);
    }
    return string("*");
}

// 0x0031B76C (name after the C++ standard)
const std::locale& std::locale::classic()
{
    if (__rw::s_pGlobalBody == NULL) {
        __rw::InitializeLocales();
    }
    // 0x00982FDC (guard 0x00982FD8)
    static locale s_Classic(__rw::s_pClassicBody);
    return s_Classic;
}

// 0x0031B830 (name is ours)
void std::locale::InstallFacet(facet* pFacet, id* pId)
{
    if ((pFacet->m_Flags & facet::FACET_INITIALIZED) == 0) {
        if (pId->m_Index == 0) {
            pId->m_Index = ++__rw::s_FacetIdCount;
        }
        pFacet->Initialize(*this);
        pFacet->m_Flags |= facet::FACET_INITIALIZED;
    }
    u32 index = pId->m_Index;
    if (m_pBody->m_Facets.m_Size <= index) {
        facet* none = NULL;
        m_pBody->m_Facets.Grow(index + 1, none);
    }
    facet* pOld = *m_pBody->m_Facets.At(index);
    if (pOld != NULL && --pOld->m_RefCount == 0) {
        delete pOld;
    }
    pFacet->m_RefCount++;
    *m_pBody->m_Facets.At(index) = pFacet;
}

// 0x0011BF54 (name is ours)
void std::locale::RemoveFacets(u32 categories)
{
    if ((categories & 0xF) != 0) {
        categories = __rw::CategoryMask(categories);
    }
    u32 count = m_pBody->m_Facets.m_Size;
    for (u32 i = 0; i != count; i++) {
        facet* pFacet = *m_pBody->m_Facets.At(i);
        if (pFacet != NULL && (pFacet->m_Category & categories) != 0) {
            if (--pFacet->m_RefCount == 0) {
                delete pFacet;
            }
            *m_pBody->m_Facets.At(i) = NULL;
        }
    }
}
