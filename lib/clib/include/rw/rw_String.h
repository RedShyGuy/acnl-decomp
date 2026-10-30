#pragma once

// std::string of the C++ library ARMCC linked in (Rogue Wave; Thumb code in the original): a pointer to the characters,
// in front of them a reference counted header (StringRep). Strings share a header until one of them changes; a
// header with -1 references belongs to one string only. The empty string uses a static header. Class names after
// the C++ standard, everything else is ours. Not for use together with GCC's C++ headers.

#include "types.h"

namespace __rw {
// the errors of ThrowError (the values the library passes; names are ours)
const int ERROR_BAD_ALLOC = 3;
const int ERROR_LENGTH = 8;
const int ERROR_OUT_OF_RANGE = 9;

// reports an error of the library to the handler the program set (without one nothing happens)
void ThrowError(int id, ...);

// raw memory of the containers
void* RawAllocate(size_t size, const void* pHint);
void RawFree(void* p, size_t size, const void* pHint);

struct StringRep
{
    s32 m_RefCount;     // 0x0 (the other strings that share it; -1: not shareable)
    u32 m_Capacity;     // 0x4
    u32 m_Length;       // 0x8

    char* Data() { return reinterpret_cast<char*>(this + 1); }
    void AddRef();
};

// the header of the empty string (and its terminator)
struct EmptyString
{
    StringRep m_Rep;
    char m_Data[4];
};
extern EmptyString s_EmptyString;
} // namespace __rw

namespace std {
template <typename T>
struct char_traits;

template <>
struct char_traits<char>
{
    static char* copy(char* pDestination, const char* pSource, size_t size);
};

template <typename T>
class allocator
{
};

template <typename T>
const T& max(const T& a, const T& b);

template <typename C, typename T = char_traits<C>, typename A = allocator<C> >
class basic_string;

template <>
class basic_string<char, char_traits<char>, allocator<char> >
{
public:
    // the largest capacity (the header and the terminator must fit into size_t)
    static const size_t MAX_SIZE = ~static_cast<size_t>(14);

    basic_string(const char* pString, const allocator<char>& alloc = allocator<char>());
    basic_string(const basic_string& other);
    ~basic_string();

    basic_string& operator=(const basic_string& other);
    basic_string& operator=(const char* pString);
    basic_string& replace(size_t position, size_t count, const char* pString, size_t length);

    const char* c_str() const { return m_pData; }
    size_t size() const { return Rep()->m_Length; }

private:
    __rw::StringRep* Rep() const { return reinterpret_cast<__rw::StringRep*>(m_pData) - 1; }

    // a header for capacity characters holding length of them
    __rw::StringRep* GetRep(size_t capacity, size_t length);
    // the characters of a new header: at least capacity, grown from oldCapacity
    char* AllocateData(size_t oldCapacity, size_t capacity, size_t length);
    // drops the reference to the header; the last one frees it
    void Unlink();
    // replaces count characters at position by length characters of pString from sourcePosition on (of
    // sourceSize); the characters at position
    char* Replace(size_t position, size_t count, const char* pString, size_t sourceSize, size_t sourcePosition,
                  size_t length);

    char* m_pData;
};

typedef basic_string<char> string;
} // namespace std
