#pragma once

// How the boss objects read and write their properties: at most the size of the field is copied,
// a shorter value is filled up with zeros. Inline in the original (the names are ours).

#include <string.h>
#include "decomp.h"

namespace nn {
namespace boss {
namespace detail {
template <typename T>
inline void GetPropertyValue(void* pValue, const T& field, size_t size)
{
    memcpy(pValue, &field, size > sizeof(T) ? sizeof(T) : size);
}

template <typename T>
inline void SetPropertyValue(T& field, const void* pValue, size_t size)
{
    memset(reinterpret_cast<u8*>(&field) + size, 0, size < sizeof(T) ? sizeof(T) - size : 0);
    memcpy(&field, pValue, size < sizeof(T) ? size : sizeof(T));
}
} // namespace detail
} // namespace boss
} // namespace nn
