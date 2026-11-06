#pragma once

#include "decomp.h"

// A ring of N elements in the object (the class is ours; the same form as SoundSongQueue).
template <typename T, s32 N>
class SoundRing
{
public:
    SoundRing() : m_pData(NULL), m_Capacity(0), m_Head(0), m_Count(0)
    {
        T* buffer = m_Buffer;
        if (buffer != NULL) {
            m_pData = buffer;
            m_Capacity = N;
        }
    }

    void Clear()
    {
        m_Count = 0;
        m_Head = 0;
    }
    // element 0 for an index out of range
    T* At(u32 i)
    {
        if (static_cast<u32>(m_Count) <= i) {
            return m_pData;
        }
        s32 pos = i + m_Head;
        if (m_Capacity <= pos) {
            pos -= m_Capacity;
        }
        return &m_pData[pos];
    }
    // NULL for an index out of range
    T* Get(s32 i)
    {
        if (i < 0 || static_cast<u32>(m_Count) <= static_cast<u32>(i)) {
            return NULL;
        }
        s32 pos = i + m_Head;
        if (m_Capacity <= pos) {
            pos -= m_Capacity;
        }
        return &m_pData[pos];
    }
    void PushBack(const T& value)
    {
        if (m_Count < m_Capacity) {
            s32 pos = m_Count++ + m_Head;
            if (pos >= m_Capacity) {
                pos -= m_Capacity;
            }
            m_pData[pos] = value;
        }
    }

    T* m_pData;     // 0x00
    s32 m_Capacity; // 0x04
    s32 m_Head;     // 0x08
    s32 m_Count;    // 0x0C
    T m_Buffer[N];  // 0x10
};
