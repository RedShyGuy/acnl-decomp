#pragma once

#include <new>
#include "decomp.h"
#include "nn/nex/nex_MemoryManager.h"

// the error of the RW STL (error 8 = length error, with the function and the sizes; the strings
// are empty in the binary)
extern "C" void StlThrow(int error, ...); // 0x0030757C (name is ours)

namespace nn {
namespace nex {
// The vector of the RW STL with nex::MemAllocator (qVector<T> derives from it): begin, end and
// the end of the capacity. ARMCC inlines its functions; push_back is written like the RW one (it
// grows by 1 + 1/2 + 1/8 but at least by 32 elements). The member names are ours.
template <typename T0>
class qVector
{
public:
    qVector() : m_pBegin(nullptr), m_pEnd(nullptr), m_pCapacityEnd(nullptr) {}
    ~qVector()
    {
        for (T0* p = m_pBegin; p != m_pEnd; p++) {
            p->~T0();
        }
        MemoryManager::Free(m_pBegin);
    }

    // (inline: at least 32 elements of capacity)
    qVector(const qVector& other) : m_pBegin(nullptr), m_pEnd(nullptr), m_pCapacityEnd(nullptr)
    {
        u32 num = other.m_pEnd - other.m_pBegin;
        if (num < 32) {
            num = 32;
        }
        m_pBegin = static_cast<T0*>(MemoryManager::Allocate(num * sizeof(T0)));
        T0* pDst = m_pBegin;
        for (const T0* p = other.m_pBegin; p != other.m_pEnd; p++, pDst++) {
            ::new (static_cast<void*>(pDst)) T0(*p);
        }
        m_pEnd = m_pBegin + (other.m_pEnd - other.m_pBegin);
        m_pCapacityEnd = m_pBegin + num;
    }
    // the values of a range (the RW constructor inserts them at the end)
    template <typename It>
    qVector(It first, It last) : m_pBegin(nullptr), m_pEnd(nullptr), m_pCapacityEnd(nullptr)
    {
        insert(m_pEnd, first, last);
    }
    // (out of line in the original, one instance per type; not decompiled yet)
    qVector& operator=(const qVector& other);
    void assign(const T0* pFirst, const T0* pLast);
    template <typename It>
    void assign(It first, It last);
    template <typename It>
    void insert(T0* pPos, It first, It last);

    // (the RW erase of all values)
    void clear()
    {
        if (m_pBegin != m_pEnd) {
            for (T0* p = m_pBegin; p != m_pEnd; p++) {
                p->~T0();
            }
            m_pEnd -= m_pEnd - m_pBegin;
        }
    }

    T0* begin() const { return m_pBegin; }
    T0* end() const { return m_pEnd; }
    u32 size() const { return m_pEnd - m_pBegin; }
    u32 capacity() const { return m_pCapacityEnd - m_pBegin; }
    T0& operator[](u32 index) const { return m_pBegin[index]; }
    u32 max_size() const { return static_cast<u32>(-1) / sizeof(T0); }

    void reserve(u32 num)
    {
        if (num > max_size()) {
            StlThrow(8, "", "", num, max_size());
        }
        if (capacity() < num) {
            u32 oldNum = size();
            u32 newCapacity = oldNum + (oldNum >> 1) + (oldNum >> 3);
            if (oldNum + 32 > newCapacity) {
                newCapacity = oldNum + 32;
            }
            if (newCapacity > num) {
                num = newCapacity;
            }
            T0* pNew = static_cast<T0*>(MemoryManager::Allocate(num * sizeof(T0)));
            T0* pDst = pNew;
            for (T0* p = m_pBegin; p != m_pEnd; p++, pDst++) {
                ::new (static_cast<void*>(pDst)) T0(*p);
            }
            MemoryManager::Free(m_pBegin);
            m_pEnd = pNew + (m_pEnd - m_pBegin);
            m_pBegin = pNew;
            m_pCapacityEnd = pNew + num;
        }
    }

    void push_back(const T0& value)
    {
        if (m_pEnd != m_pCapacityEnd) {
            ::new (static_cast<void*>(m_pEnd)) T0(value);
            m_pEnd++;
        } else {
            InsertOne(m_pEnd, value);
        }
    }

    // one value at pos (the RW _C_insert_1)
    void InsertOne(T0* pos, const T0& value)
    {
        if (size() < capacity()) {
            ::new (static_cast<void*>(m_pEnd)) T0(m_pEnd[-1]);
            T0* last = m_pEnd - 1;
            m_pEnd++;
            while (last != pos) {
                *last = last[-1];
                last--;
            }
            *pos = value;
            return;
        }
        u32 num = size();
        u32 newCapacity = num + (num >> 1) + (num >> 3);
        if (num + 32 > newCapacity) {
            newCapacity = num + 32;
        }
        T0* pNew = static_cast<T0*>(MemoryManager::Allocate(newCapacity * sizeof(T0)));
        T0* pDst = pNew;
        for (T0* p = m_pBegin; p != pos; p++, pDst++) {
            ::new (static_cast<void*>(pDst)) T0(*p);
        }
        ::new (static_cast<void*>(pDst)) T0(value);
        pDst++;
        for (T0* p = pos; p != m_pEnd; p++, pDst++) {
            ::new (static_cast<void*>(pDst)) T0(*p);
        }
        MemoryManager::Free(m_pBegin);
        m_pBegin = pNew;
        m_pEnd = pNew + num + 1;
        m_pCapacityEnd = pNew + newCapacity;
    }

    T0* m_pBegin;       // 0x0
    T0* m_pEnd;         // 0x4
    T0* m_pCapacityEnd; // 0x8
};
} // namespace nex
} // namespace nn
