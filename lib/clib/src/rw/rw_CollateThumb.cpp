// std::collate<char> and std::codecvt_byname<wchar_t, char, __mbstate_t> of the C++ library (Thumb code in the
// original): collate compares through a table of weights (collate_byname through strcoll/strxfrm of the C library),
// codecvt_byname converts through wcrtomb/mbrtowc. See rw/rw_Locale.h.

#include "decomp.h"
#include "rw/rw_Locale.h"
#include "clib/clib_Thumb.h"

extern "C" {
void* memcpy(void* destination, const void* source, size_t size);
int strcoll(const char* a, const char* b);
size_t strxfrm(char* pDestination, const char* pSource, size_t size);
size_t wcrtomb(char* pDestination, wchar_t c, mbstate_t* pState);
size_t mbrtowc(wchar_t* pDestination, const char* pSource, size_t size, mbstate_t* pState);
int __mb_cur_max();
}

namespace {
const int LC_COLLATE = 1;
} // namespace

// 0x0031C300 (name is ours)
u8 __rw::__rw_collate_impl<char>::Weight(u8 c) const
{
    for (const CollateRange* pRange = reinterpret_cast<const CollateRange*>(m_pTable + 1); pRange != NULL;) {
        if (c < pRange->m_First) {
            pRange = pRange->m_pLower;
        } else if (c > pRange->m_Last) {
            pRange = pRange->m_pHigher;
        } else if (pRange->m_pWeights != NULL) {
            return pRange->m_pWeights[c - pRange->m_First];
        } else {
            return pRange->m_Offset + c;
        }
    }
    return c;
}

// ---- collate<char>

// 0x0011BFEC (name after the C++ standard)
// 0x0011BFE4 (deleting dtor)
std::collate<char>::~collate()
{
    // empty (in the original too)
}

// 0x0011C2CC (name after the C++ standard)
int std::collate<char>::do_compare(const char* pLow1, const char* pHigh1, const char* pLow2, const char* pHigh2) const
{
    size_t length1 = pHigh1 - pLow1;
    size_t length2 = pHigh2 - pLow2;
    size_t count = length1 < length2 ? length1 : length2;
    while (count-- != 0) {
        int difference = Weight(*pLow1) - Weight(*pLow2);
        if (difference != 0) {
            return difference < 0 ? -1 : 1;
        }
        pLow1++;
        pLow2++;
    }
    if (length1 < length2) {
        return -1;
    }
    return length2 < length1 ? 1 : 0;
}

// 0x0011C32A (name after the C++ standard)
std::string std::collate<char>::do_transform(const char* pLow, const char* pHigh) const
{
    string result(NULL, pHigh - pLow);
    char* p = result.begin();
    for (; pLow != pHigh; pLow++) {
        *p++ = Weight(*pLow);
    }
    return result;
}

// 0x0011C36A (name after the C++ standard)
long std::collate<char>::do_hash(const char* pLow, const char* pHigh) const
{
    string weights = do_transform(pLow, pHigh);
    u32 hash = 0;
    for (const char* p = weights.c_str(); p != weights.c_str() + weights.size(); p++) {
        hash = (hash << 4) + static_cast<u8>(*p);
        s32 high = hash & 0xF0000000;
        if (high != 0) {
            hash = ((high >> 24) ^ hash) & 0x0FFFFFFF;
        }
    }
    return hash;
}

// ---- collate_byname<char>

// 0x0011BDF0 (name after the C++ standard)
// 0x0011BDE2 (deleting dtor)
std::collate_byname<char>::~collate_byname()
{
    // (only the members are destroyed)
}

// the two ranges as C strings in one buffer (on the stack if they fit)
// 0x0011B6E0 (name after the C++ standard)
int std::collate_byname<char>::do_compare(const char* pLow1, const char* pHigh1, const char* pLow2,
                                          const char* pHigh2) const
{
    __rw::LocaleGuard guard(m_Name.c_str(), LC_COLLATE);
    size_t length1 = pHigh1 - pLow1;
    size_t length2 = pHigh2 - pLow2;
    size_t total = length1 + length2;
    char buffer[512];
    char* pBuffer = total + 2 >= sizeof(buffer) ? new char[total + 2] : buffer;
    memcpy(pBuffer, pLow1, length1);
    pBuffer[length1] = '\0';
    char* pSecond = pBuffer + length1 + 1;
    memcpy(pSecond, pLow2, length2);
    pBuffer[total + 1] = '\0';
    int order = strcoll(pBuffer, pSecond);
    if (pBuffer != buffer) {
        delete[] pBuffer;
    }
    if (order == 0) {
        return 0;
    }
    return order > 0 ? 1 : -1;
}

// 0x0011B76C (name after the C++ standard)
std::string std::collate_byname<char>::do_transform(const char* pLow, const char* pHigh) const
{
    __rw::LocaleGuard guard(m_Name.c_str(), LC_COLLATE);
    size_t length = pHigh - pLow;
    char buffer[256];
    char* pBuffer = length > 255 ? new char[length + 1] : buffer;
    memcpy(pBuffer, pLow, length);
    pBuffer[length] = '\0';
    size_t size = strxfrm(NULL, pBuffer, 0) + 1;
    string result(NULL, size);
    strxfrm(result.begin(), pBuffer, size);
    if (pBuffer != buffer) {
        delete[] pBuffer;
    }
    return result;
}

// ---- codecvt_byname<wchar_t, char, __mbstate_t>

namespace {
inline mbstate_t* CState(std::__mbstate_t& state)
{
    return reinterpret_cast<mbstate_t*>(&state);
}
} // namespace

// 0x0011BDE0 (name after the C++ standard)
// 0x0011BDD8 (deleting dtor)
std::codecvt_byname<wchar_t, char, std::__mbstate_t>::~codecvt_byname()
{
    // empty (in the original too)
}

// 0x0011B682 (name after the C++ standard)
std::codecvt_base::result std::codecvt_byname<wchar_t, char, std::__mbstate_t>::do_out(
    state_type& state, const wchar_t* pFrom, const wchar_t* pFromEnd, const wchar_t*& pFromNext, char* pTo,
    char* pToEnd, char*& pToNext) const
{
    pFromNext = pFrom;
    pToNext = pTo;
    if (__mb_cur_max() > pToEnd - pTo) {
        return partial;
    }
    // (room for the longest character)
    char* pLimit = pToEnd - __mb_cur_max() + 1;
    while (pFromNext < pFromEnd && pToNext < pLimit) {
        int length = wcrtomb(pToNext, *pFromNext, CState(state));
        if (length == -1) {
            return error;
        }
        pToNext += length;
        pFromNext++;
    }
    return pFromNext == pFromEnd ? ok : partial;
}

// 0x0011B632 (name after the C++ standard)
std::codecvt_base::result std::codecvt_byname<wchar_t, char, std::__mbstate_t>::do_in(
    state_type& state, const char* pFrom, const char* pFromEnd, const char*& pFromNext, wchar_t* pTo,
    wchar_t* pToEnd, wchar_t*& pToNext) const
{
    pFromNext = pFrom;
    for (pToNext = pTo; pFromNext < pFromEnd && pToNext < pToEnd; pToNext++) {
        int length = mbrtowc(pToNext, pFromNext, pFromEnd - pFromNext, CState(state));
        if (length == -2) {
            return partial;
        }
        if (length == -1) {
            return error;
        }
        pFromNext += length == 0 ? 1 : length;
    }
    return ok;
}

// (a null character through do_out)
// 0x0011B5E6 (name after the C++ standard)
std::codecvt_base::result std::codecvt_byname<wchar_t, char, std::__mbstate_t>::do_unshift(
    state_type& state, char* pTo, char* pToEnd, char*& pToNext) const
{
    wchar_t none = L'\0';
    const wchar_t* pFromNext = NULL;
    return do_out(state, &none, &none + 1, pFromNext, pTo, pToEnd, pToNext);
}

// 0x0011B62E (name after the C++ standard)
bool std::codecvt_byname<wchar_t, char, std::__mbstate_t>::do_always_noconv() const
{
    return false;
}

// (-1: the encoding has states)
// 0x0011B60A (name after the C++ standard)
int std::codecvt_byname<wchar_t, char, std::__mbstate_t>::do_encoding() const
{
    if (mbrtowc(NULL, NULL, 0, NULL) != 0) {
        return -1;
    }
    return 1;
}

// 0x0011BAE0 (name after the C++ standard)
int std::codecvt_byname<wchar_t, char, std::__mbstate_t>::do_length(state_type& state, const char* pFrom,
                                                                    const char* pFromEnd, size_t max) const
{
    size_t length = pFromEnd - pFrom;
    return length < max ? length : max;
}

// 0x0011B626 (name after the C++ standard)
int std::codecvt_byname<wchar_t, char, std::__mbstate_t>::do_max_length() const
{
    return __mb_cur_max();
}
