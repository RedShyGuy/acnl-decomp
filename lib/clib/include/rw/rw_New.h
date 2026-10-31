#pragma once

// The new and delete operators of the C++ library: the global ones are those of sead (sead/seadNew.h), the placement
// forms are inline. nothrow_t after the C++ standard.

#include <stddef.h>

namespace std {
struct nothrow_t
{
};
extern const nothrow_t nothrow;
} // namespace std

void* operator new(size_t size);
void* operator new[](size_t size);
void* operator new(size_t size, const std::nothrow_t&) noexcept;
void* operator new[](size_t size, const std::nothrow_t&) noexcept;
void operator delete(void* p) noexcept;
void operator delete[](void* p) noexcept;

inline void* operator new(size_t, void* p) noexcept
{
    return p;
}

inline void* operator new[](size_t, void* p) noexcept
{
    return p;
}

inline void operator delete(void*, void*) noexcept {}
inline void operator delete[](void*, void*) noexcept {}
