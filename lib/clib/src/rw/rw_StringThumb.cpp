// std::string of the C++ library (Thumb code in the original). See rw/rw_String.h.

#include "decomp.h"
#include "rw/rw_String.h"
#include "clib/clib_Thumb.h"

extern "C" {
size_t strlen(const char* string);
void* memcpy(void* destination, const void* source, size_t size);
void* memmove(void* destination, const void* source, size_t size);
}

void* operator new(size_t size);
void operator delete(void* p);

namespace __rw {
// 0x00AF9130 (name is ours)
EmptyString s_EmptyString;

// (the original catches nothing: the nops of a removed handler)
// 0x0010BF94 (name is ours)
DECOMP_NOINLINE void* RawAllocate(size_t size, const void* pHint)
{
    void* p = operator new(size);
    if (p == NULL) {
        ThrowError(ERROR_BAD_ALLOC);
    }
    return p;
}

// 0x0010C128 (name is ours)
DECOMP_NOINLINE void RawFree(void* p, size_t size, const void* pHint)
{
    operator delete(p);
}

// 0x0031C6E8 (name is ours)
DECOMP_NOINLINE void StringRep::AddRef()
{
    if (this != &s_EmptyString.m_Rep) {
        m_RefCount++;
    }
}
} // namespace __rw

// 0x0031C3C8 (name after the C++ standard)
DECOMP_NOINLINE char* std::char_traits<char>::copy(char* pDestination, const char* pSource, size_t size)
{
    memcpy(pDestination, pSource, size);
    return pDestination;
}

// 0x0031C33A (name after the C++ standard)
template <>
DECOMP_NOINLINE const unsigned int& std::max<unsigned int>(const unsigned int& a, const unsigned int& b)
{
    return a < b ? b : a;
}

namespace {
inline char* MoveCharacters(char* pDestination, const char* pSource, size_t size)
{
    memmove(pDestination, pSource, size);
    return pDestination;
}
} // namespace

// 0x0031C348 (name is ours)
DECOMP_NOINLINE __rw::StringRep* std::basic_string<char, std::char_traits<char>, std::allocator<char> >::GetRep(size_t capacity, size_t length)
{
    if (capacity > MAX_SIZE) {
        __rw::ThrowError(__rw::ERROR_LENGTH, "", "", capacity, MAX_SIZE);
    }
    if (length > capacity) {
        __rw::ThrowError(__rw::ERROR_LENGTH, "", "", length, capacity);
    }
    if (capacity == 0) {
        return &__rw::s_EmptyString.m_Rep;
    }
    __rw::StringRep* pRep = static_cast<__rw::StringRep*>(__rw::RawAllocate(capacity + sizeof(__rw::StringRep) + 2, NULL));
    pRep->m_Length = length;
    pRep->m_RefCount = 0;
    pRep->m_Capacity = capacity;
    pRep->Data()[length] = '\0';
    return pRep;
}

// 0x0011C47C (name is ours)
DECOMP_NOINLINE char* std::basic_string<char, std::char_traits<char>, std::allocator<char> >::AllocateData(size_t oldCapacity, size_t capacity, size_t length)
{
    size_t grown = oldCapacity + oldCapacity / 2 + oldCapacity / 8;
    if (oldCapacity + 32 > grown) {
        grown = oldCapacity + 32;
    }
    return GetRep(max(grown, capacity), length)->Data();
}

// 0x0031C3D4 (name is ours)
DECOMP_NOINLINE void std::basic_string<char, std::char_traits<char>, std::allocator<char> >::Unlink()
{
    if (m_pData == __rw::s_EmptyString.m_Data) {
        return;
    }
    __rw::StringRep* pRep = Rep();
    if (pRep->m_RefCount != -1) {
        if (pRep == &__rw::s_EmptyString.m_Rep) {
            return;
        }
        if (--pRep->m_RefCount != -1) {
            return;
        }
    }
    __rw::RawFree(Rep(), Rep()->m_Capacity + sizeof(__rw::StringRep) + 2, NULL);
}

// 0x002FAE9C (name after the C++ standard)
DECOMP_NOINLINE std::basic_string<char, std::char_traits<char>, std::allocator<char> >::basic_string(const allocator<char>& alloc)
{
    m_pData = __rw::s_EmptyString.m_Data;
}

// 0x00103A02 (name after the C++ standard)
std::basic_string<char, std::char_traits<char>, std::allocator<char> >::basic_string() : basic_string(allocator<char>())
{
}

// 0x0031B644 (name after the C++ standard)
std::basic_string<char, std::char_traits<char>, std::allocator<char> >::basic_string(const char* pString, size_t length, const allocator<char>& alloc)
{
    if (length > MAX_SIZE) {
        __rw::ThrowError(__rw::ERROR_LENGTH, "", "", length, MAX_SIZE);
    }
    const size_t MIN_CAPACITY = 32;
    m_pData = (length != 0 ? GetRep(max(MIN_CAPACITY, length), length) : &__rw::s_EmptyString.m_Rep)->Data();
    if (pString != NULL) {
        char_traits<char>::copy(m_pData, pString, length);
    }
}

// a shared header is copied first; afterwards the header is not shareable
// 0x0031B6A4 (name after the C++ standard)
char* std::basic_string<char, std::char_traits<char>, std::allocator<char> >::begin()
{
    __rw::StringRep* pRep = Rep();
    if (static_cast<u32>(pRep->m_RefCount + 1) > 1) {
        size_t length = pRep->m_Length;
        char* pData = GetRep(length, length < ~static_cast<size_t>(0) ? length : ~static_cast<size_t>(0))->Data();
        char_traits<char>::copy(pData, m_pData, Rep()->m_Length);
        Unlink();
        m_pData = pData;
    }
    if (Rep() != &__rw::s_EmptyString.m_Rep) {
        Rep()->m_RefCount = -1;
    }
    return m_pData;
}

// 0x00307668 (name after the C++ standard)
std::basic_string<char, std::char_traits<char>, std::allocator<char> >::basic_string(const char* pString, const allocator<char>& alloc)
{
    size_t length = strlen(pString);
    m_pData = length != 0 ? AllocateData(0, length, length) : __rw::s_EmptyString.m_Data;
    char_traits<char>::copy(m_pData, pString, length);
}

// (a shareable header is shared, else the characters are copied)
// 0x003076A4 (name after the C++ standard)
std::basic_string<char, std::char_traits<char>, std::allocator<char> >::basic_string(const std::basic_string<char, std::char_traits<char>, std::allocator<char> >& other)
{
    __rw::StringRep* pRep = other.Rep();
    if (pRep->m_RefCount != -1) {
        m_pData = other.m_pData;
        pRep->AddRef();
    } else {
        size_t length = pRep->m_Length;
        m_pData = GetRep(length, length)->Data();
        char_traits<char>::copy(m_pData, other.m_pData, length);
    }
}

// 0x003076E0 (name after the C++ standard)
std::basic_string<char, std::char_traits<char>, std::allocator<char> >::~basic_string()
{
    Unlink();
}

// 0x00307898 (name after the C++ standard)
std::string& std::basic_string<char, std::char_traits<char>, std::allocator<char> >::operator=(const std::basic_string<char, std::char_traits<char>, std::allocator<char> >& other)
{
    if (other.Rep()->m_RefCount != -1) {
        other.Rep()->AddRef();
        Unlink();
        m_pData = other.m_pData;
    } else if (this != &other) {
        replace(0, size(), other.m_pData, other.size());
    }
    return *this;
}

// 0x0011C62C (name after the C++ standard)
std::string& std::basic_string<char, std::char_traits<char>, std::allocator<char> >::operator=(const char* pString)
{
    size_t length = strlen(pString);
    if (length != 0) {
        return replace(0, size(), pString, length);
    }
    if (Rep()->m_RefCount == 0) {
        Rep()->m_Length = 0;
        m_pData[0] = '\0';
    } else {
        Unlink();
        m_pData = __rw::s_EmptyString.m_Data;
    }
    return *this;
}

// 0x0031C6FC (name after the C++ standard)
DECOMP_NOINLINE std::string& std::basic_string<char, std::char_traits<char>, std::allocator<char> >::replace(size_t position, size_t count, const char* pString, size_t length)
{
    Replace(position, count, pString, length, 0, length);
    return *this;
}

// in place when the header is not shared, big enough and pString is not inside the string; else into a new header
// 0x0011C4B0 (name is ours)
DECOMP_NOINLINE char* std::basic_string<char, std::char_traits<char>, std::allocator<char> >::Replace(size_t position, size_t count, const char* pString, size_t sourceSize,
                           size_t sourcePosition, size_t length)
{
    size_t size = Rep()->m_Length;
    if (size < position || sourcePosition > sourceSize) {
        __rw::ThrowError(__rw::ERROR_OUT_OF_RANGE, "", "", size < position ? position : sourcePosition,
                         size > sourceSize ? size : sourceSize);
    }
    size = Rep()->m_Length;
    size_t removed = size - position;
    if (count < removed) {
        removed = count;
    }
    size_t inserted = sourceSize - sourcePosition;
    if (length < inserted) {
        inserted = length;
    }
    if (size - removed > MAX_SIZE - inserted) {
        __rw::ThrowError(__rw::ERROR_LENGTH, "", "", size - removed, MAX_SIZE - inserted);
    }
    __rw::StringRep* pRep = Rep();
    size_t kept = pRep->m_Length - removed;
    size_t newLength = kept + inserted;
    if (newLength == 0) {
        Unlink();
        m_pData = __rw::s_EmptyString.m_Data;
        return m_pData + position;
    }
    size_t tail = kept - position;
    size_t tailPosition = position + count;
    const char* pSource = pString + sourcePosition;
    if (static_cast<u32>(pRep->m_RefCount + 1) > 1 || pRep->m_Capacity < newLength ||
        (pString != NULL && m_pData <= pString && pString < m_pData + pRep->m_Length)) {
        size_t oldSize = pRep->m_Length;
        size_t grown = oldSize + oldSize / 2 + oldSize / 8;
        if (oldSize + 128 > grown) {
            grown = oldSize + 128;
        }
        __rw::StringRep* pNew = GetRep(max(grown, newLength), newLength);
        if (position != 0) {
            char_traits<char>::copy(pNew->Data(), m_pData, position);
        }
        if (inserted != 0) {
            char_traits<char>::copy(pNew->Data() + position, pSource, inserted);
        }
        if (tail != 0) {
            char_traits<char>::copy(pNew->Data() + position + inserted, m_pData + tailPosition, tail);
        }
        Unlink();
        m_pData = pNew->Data();
    } else {
        if (tail != 0) {
            MoveCharacters(m_pData + position + inserted, m_pData + tailPosition, tail);
        }
        if (inserted != 0) {
            MoveCharacters(m_pData + position, pSource, inserted);
        }
        Rep()->m_Length = newLength;
        m_pData[newLength] = '\0';
    }
    return m_pData + position;
}
