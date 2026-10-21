#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_qVector.h"

#include <cstring>

namespace nn {
namespace nex {
// A ring buffer of a fixed capacity. The name is from the reference symbols; the member names
// and the names of the inline functions are ours.
template <typename T>
class qReservedQueue : public RootObject
{
public:
    // (inline)
    explicit qReservedQueue(u32 capacity) : m_Buffer(capacity), m_WriteIndex(0), m_ReadIndex(0), m_Capacity(capacity), m_Count(0) {}

    // (out of line per type) copies the values behind the last ones (the caller checks the room)
    void push(const T* pValues, unsigned size);
    // (inline) moves the first values out
    void PopInline(T* pValues, u32 size)
    {
        u32 rest = m_Capacity - m_ReadIndex;
        if (rest > size) {
            memcpy(pValues, m_Buffer.m_pBegin + m_ReadIndex, size * sizeof(T));
        } else {
            memcpy(pValues, m_Buffer.m_pBegin + m_ReadIndex, rest * sizeof(T));
            if (size - rest != 0) {
                memcpy(pValues + rest, m_Buffer.m_pBegin, (size - rest) * sizeof(T));
            }
        }
        m_ReadIndex = (m_ReadIndex + size) % m_Capacity;
        m_Count -= size;
    }
    // (inline) push inline
    void PushInline(const T* pValues, u32 size)
    {
        u32 rest = m_Capacity - m_WriteIndex;
        if (rest > size) {
            memcpy(m_Buffer.m_pBegin + m_WriteIndex, pValues, size * sizeof(T));
            m_WriteIndex += size;
            m_Count += size;
        } else {
            memcpy(m_Buffer.m_pBegin + m_WriteIndex, pValues, rest * sizeof(T));
            m_Count += rest;
            m_WriteIndex = (m_WriteIndex + rest) % m_Capacity;
            u32 left = size - rest;
            if (left != 0) {
                memcpy(m_Buffer.m_pBegin, pValues + rest, left * sizeof(T));
                m_WriteIndex = left;
                m_Count += left;
            }
        }
    }
    // (inline) one value behind the last one (the caller checks the room)
    void PushBack(const T& value)
    {
        m_Buffer.m_pBegin[m_WriteIndex] = value;
        m_WriteIndex = (m_WriteIndex + 1) % m_Capacity;
        m_Count++;
    }
    // (inline) the first value, which is removed (the caller checks the count)
    T PopFront()
    {
        T value = m_Buffer.m_pBegin[m_ReadIndex];
        m_ReadIndex = (m_ReadIndex + 1) % m_Capacity;
        m_Count--;
        return value;
    }
    // (inline)
    void Clear()
    {
        m_Count = 0;
        m_WriteIndex = 0;
        m_ReadIndex = 0;
    }
    u32 GetCount() const { return m_Count; }
    u32 GetFreeSize() const { return m_Capacity - m_Count; }

    u32 m_Unknown0x00;  // 0x00 (not set by the constructor)
    qVector<T> m_Buffer; // 0x04
    u32 m_WriteIndex;   // 0x10
    u32 m_ReadIndex;    // 0x14
    u32 m_Count;        // 0x18
    u32 m_Capacity;     // 0x1C
};
} // namespace nex
} // namespace nn
