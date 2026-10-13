#pragma once

#include <new>
#include "decomp.h"
#include "nn/nex/nex_MemAllocator.h"
#include "nn/nex/nex_MemoryManager.h"

// the error of the RW STL (error 8 = length error, with the function and the sizes; the strings
// are empty in the binary)
extern "C" void StlThrow(int error, ...); // 0x0030757C (name is ours)

namespace std {
// The vector of the RW STL as nex instantiates it (with nex::MemAllocator; nex::qVector derives
// from it): begin, end and the end of the capacity. ARMCC inlines the small functions; the others
// (operator=, insert, _C_insert_aux2) are out of line per instance and only declared here.
// _C_insert_aux is out of line in some instances and inline in others. It grows by 1 + 1/2 + 1/8
// but at least by 32 elements. The class and function names are from the binary, the member names
// are ours.
template <typename T, typename Alloc = nn::nex::MemAllocator<T> >
class vector
{
public:
    vector() : m_pBegin(nullptr), m_pEnd(nullptr), m_pCapacityEnd(nullptr) {}
    ~vector()
    {
        for (T* p = m_pBegin; p != m_pEnd; p++) {
            p->~T();
        }
        nn::nex::MemoryManager::Free(m_pBegin);
    }

    // (out of line in the original, one instance per type; not decompiled yet)
    vector& operator=(const vector& other);
    void assign(const T* pFirst, const T* pLast);
    template <typename It>
    void assign(It first, It last);
    template <typename It>
    void insert(T* pPos, It first, It last);
    template <typename It>
    void _C_insert_aux2(T* pPos, It first, It last);

    // (the RW erase of all values)
    void clear()
    {
        if (m_pBegin != m_pEnd) {
            for (T* p = m_pBegin; p != m_pEnd; p++) {
                p->~T();
            }
            m_pEnd -= m_pEnd - m_pBegin;
        }
    }

    T* begin() const { return m_pBegin; }
    T* end() const { return m_pEnd; }
    u32 size() const { return m_pEnd - m_pBegin; }
    u32 capacity() const { return m_pCapacityEnd - m_pBegin; }
    T& operator[](u32 index) const { return m_pBegin[index]; }
    u32 max_size() const { return static_cast<u32>(-1) / sizeof(T); }

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
            T* pNew = static_cast<T*>(nn::nex::MemoryManager::Allocate(num * sizeof(T)));
            T* pDst = pNew;
            for (T* p = m_pBegin; p != m_pEnd; p++, pDst++) {
                ::new (static_cast<void*>(pDst)) T(*p);
            }
            nn::nex::MemoryManager::Free(m_pBegin);
            m_pEnd = pNew + (m_pEnd - m_pBegin);
            m_pBegin = pNew;
            m_pCapacityEnd = pNew + num;
        }
    }

    void push_back(const T& value)
    {
        if (m_pEnd != m_pCapacityEnd) {
            ::new (static_cast<void*>(m_pEnd)) T(value);
            m_pEnd++;
        } else {
            _C_insert_aux(m_pEnd, value);
        }
    }

    // one value at pos
    void _C_insert_aux(T* pos, const T& value)
    {
        if (size() < capacity()) {
            ::new (static_cast<void*>(m_pEnd)) T(m_pEnd[-1]);
            T* last = m_pEnd - 1;
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
        T* pNew = static_cast<T*>(nn::nex::MemoryManager::Allocate(newCapacity * sizeof(T)));
        T* pDst = pNew;
        for (T* p = m_pBegin; p != pos; p++, pDst++) {
            ::new (static_cast<void*>(pDst)) T(*p);
        }
        ::new (static_cast<void*>(pDst)) T(value);
        pDst++;
        for (T* p = pos; p != m_pEnd; p++, pDst++) {
            ::new (static_cast<void*>(pDst)) T(*p);
        }
        nn::nex::MemoryManager::Free(m_pBegin);
        m_pBegin = pNew;
        m_pEnd = pNew + num + 1;
        m_pCapacityEnd = pNew + newCapacity;
    }

    T* m_pBegin;       // 0x0
    T* m_pEnd;         // 0x4
    T* m_pCapacityEnd; // 0x8
};
} // namespace std

namespace nn {
namespace nex {
// std::vector with MemAllocator. Its constructors and destructor are its own (inline, with copies
// out of line per type).
template <typename T0>
class qVector : public std::vector<T0, MemAllocator<T0> >
{
public:
    qVector() {}
    ~qVector() {}

    // (inline: at least 32 elements of capacity)
    qVector(const qVector& other)
    {
        u32 num = other.m_pEnd - other.m_pBegin;
        if (num < 32) {
            num = 32;
        }
        this->m_pBegin = static_cast<T0*>(MemoryManager::Allocate(num * sizeof(T0)));
        T0* pDst = this->m_pBegin;
        for (const T0* p = other.m_pBegin; p != other.m_pEnd; p++, pDst++) {
            ::new (static_cast<void*>(pDst)) T0(*p);
        }
        this->m_pEnd = this->m_pBegin + (other.m_pEnd - other.m_pBegin);
        this->m_pCapacityEnd = this->m_pBegin + num;
    }
    // the values of a range (the RW constructor inserts them at the end)
    template <typename It>
    qVector(It first, It last)
    {
        this->insert(this->m_pEnd, first, last);
    }
    qVector& operator=(const qVector& other)
    {
        std::vector<T0, MemAllocator<T0> >::operator=(other);
        return *this;
    }
};
} // namespace nex
} // namespace nn
