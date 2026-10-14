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
    // (out of line in some instances: they are specialized with the body of DestroyInline)
    ~vector() { DestroyInline(); }
    void DestroyInline()
    {
        for (T* p = m_pBegin; p != m_pEnd; p++) {
            p->~T();
        }
        nn::nex::MemoryManager::Free(m_pBegin);
    }

    // (out of line in the original, one instance per type; not decompiled yet; inline in a few
    // instances: they specialize it with the body of AssignInline)
    DECOMP_NOINLINE vector& operator=(const vector& other) { return AssignInline(other); }
    // (RW: with a shorter or equal length the end of the capacity becomes the new end)
    vector& AssignInline(const vector& other)
    {
        if (&other != this) {
            u32 num = other.m_pEnd - other.m_pBegin;
            if (num > capacity()) {
                u32 oldNum = size();
                u32 grown = oldNum + (oldNum >> 1) + (oldNum >> 3);
                u32 newCapacity = oldNum + 32 > grown ? oldNum + 32 : grown;
                if (num > newCapacity) {
                    newCapacity = num;
                }
                T* pNew = static_cast<T*>(nn::nex::MemoryManager::Allocate(newCapacity * sizeof(T)));
                T* pDst = pNew;
                for (const T* p = other.m_pBegin; p != other.m_pEnd; p++, pDst++) {
                    ::new (static_cast<void*>(pDst)) T(*p);
                }
                for (T* p = m_pBegin; p != m_pEnd; p++) {
                    p->~T();
                }
                nn::nex::MemoryManager::FreeInline(m_pBegin);
                m_pBegin = pNew;
                m_pEnd = pNew + (other.m_pEnd - other.m_pBegin);
                m_pCapacityEnd = pNew + newCapacity;
            } else if (size() >= num) {
                T* pDst = m_pBegin;
                for (const T* p = other.m_pBegin; p != other.m_pEnd; p++, pDst++) {
                    *pDst = *p;
                }
                for (T* p = pDst; p != m_pEnd; p++) {
                    p->~T();
                }
                m_pEnd = m_pBegin + (other.m_pEnd - other.m_pBegin);
                m_pCapacityEnd = m_pEnd;
            } else {
                u32 oldNum = size();
                T* pDst = m_pBegin;
                for (const T* p = other.m_pBegin; p != other.m_pBegin + oldNum; p++, pDst++) {
                    *pDst = *p;
                }
                m_pEnd = m_pBegin + (other.m_pEnd - other.m_pBegin);
                pDst = m_pBegin + oldNum;
                for (const T* p = other.m_pBegin + oldNum; p != other.m_pEnd; p++, pDst++) {
                    ::new (static_cast<void*>(pDst)) T(*p);
                }
            }
        }
        return *this;
    }
    // (out of line per instance; not decompiled yet) the size becomes num, new values are value
    void resize(u32 num, T value);
    // (resize where ARMCC inlined it; the values behind num are destroyed as by the RW erase)
    DECOMP_ALWAYS_INLINE void ResizeInline(u32 num, T value)
    {
        if (num > size()) {
            _C_insert_aux(m_pEnd, num - size(), value);
        } else if (num < size()) {
            for (T* p = m_pBegin + num; p != m_pEnd; p++) {
                p->~T();
            }
            m_pEnd = m_pBegin + num;
        }
    }
    // inserts num values at pos (out of line in some instances: they are specialized with the
    // body of InsertFillInline)
    DECOMP_ALWAYS_INLINE void _C_insert_aux(T* pos, u32 num, const T& value) { InsertFillInline(pos, num, value); }
    DECOMP_ALWAYS_INLINE void InsertFillInline(T* pos, u32 num, const T& value)
    {
        if (num == 0) {
            return;
        }
        if (size() + num > capacity()) {
            u32 oldNum = size();
            u32 newCapacity = oldNum + (oldNum >> 1) + (oldNum >> 3);
            if (oldNum + 32 > newCapacity) {
                newCapacity = oldNum + 32;
            }
            if (size() + num >= newCapacity) {
                newCapacity = size() + num;
            }
            T* pNew = static_cast<T*>(nn::nex::MemoryManager::Allocate(newCapacity * sizeof(T)));
            T* pDst = pNew;
            for (T* p = m_pBegin; p != pos; p++, pDst++) {
                ::new (static_cast<void*>(pDst)) T(*p);
            }
            pDst = pNew + (pos - m_pBegin);
            for (u32 i = num; i != 0; i--, pDst++) {
                ::new (static_cast<void*>(pDst)) T(value);
            }
            pDst = pNew + (pos - m_pBegin) + num;
            for (T* p = pos; p != m_pEnd; p++, pDst++) {
                ::new (static_cast<void*>(pDst)) T(*p);
            }
            u32 oldSize = m_pEnd - m_pBegin;
            for (T* p = m_pBegin; p != m_pEnd; p++) {
                p->~T();
            }
            nn::nex::MemoryManager::Free(m_pBegin);
            m_pCapacityEnd = pNew + newCapacity;
            m_pEnd = pNew + oldSize + num;
            m_pBegin = pNew;
        } else {
            T* pOldEnd = m_pEnd;
            T* pPosEnd = pos + num;
            m_pEnd = pOldEnd + num;
            if (pOldEnd > pPosEnd) {
                // the last num values to the new end, the others behind them, then the new values
                T* pDst = pOldEnd;
                for (T* p = pOldEnd - num; p != pOldEnd; p++, pDst++) {
                    ::new (static_cast<void*>(pDst)) T(*p);
                }
                pDst = pOldEnd;
                for (T* p = pOldEnd - num; p != pos;) {
                    *--pDst = *--p;
                }
                for (T* p = pos; p != pPosEnd; p++) {
                    *p = value;
                }
            } else {
                T* pDst = pOldEnd;
                for (u32 i = num - (pOldEnd - pos); i != 0; i--, pDst++) {
                    ::new (static_cast<void*>(pDst)) T(value);
                }
                pDst = pPosEnd;
                for (T* p = pos; p != pOldEnd; p++, pDst++) {
                    ::new (static_cast<void*>(pDst)) T(*p);
                }
                for (T* p = pos; p != pOldEnd; p++) {
                    *p = value;
                }
            }
        }
    }
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

    // one value at pos (out of line in some instances: they specialize it with the body of
    // InsertAuxInline)
    void _C_insert_aux(T* pos, const T& value) { InsertAuxInline(pos, value); }
    void InsertAuxInline(T* pos, const T& value)
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

    // num values T() (at least 32 elements of capacity; out of line in some instances, they are
    // specialized)
    explicit qVector(int num) { InitializeInline(num); }
    void InitializeInline(int num)
    {
        u32 capacity = static_cast<u32>(num) >= 32 ? num : 32;
        this->m_pBegin = static_cast<T0*>(MemoryManager::Allocate(capacity * sizeof(T0)));
        T0* p = this->m_pBegin;
        for (u32 i = num; i != 0; i--, p++) {
            ::new (static_cast<void*>(p)) T0();
        }
        this->m_pCapacityEnd = this->m_pBegin + capacity;
        this->m_pEnd = this->m_pBegin + num;
    }

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

namespace nn {
namespace nex {
class DataStoreKeyValue;
class String;
// (out of line)
template <>
DECOMP_NOINLINE qVector<u8>::qVector(int num);
template <>
DECOMP_NOINLINE qVector<String>::qVector(int num);
template <>
DECOMP_NOINLINE qVector<DataStoreKeyValue>::qVector(int num);
} // namespace nex
} // namespace nn

namespace std {
// (out of line)
template <>
DECOMP_NOINLINE vector<u64, nn::nex::MemAllocator<u64> >::~vector();
template <>
DECOMP_NOINLINE void vector<u8, nn::nex::MemAllocator<u8> >::_C_insert_aux(u8* pos, const u8& value);
template <>
DECOMP_NOINLINE void vector<u8, nn::nex::MemAllocator<u8> >::_C_insert_aux(u8* pos, u32 num, const u8& value);
template <>
DECOMP_NOINLINE void vector<u32, nn::nex::MemAllocator<u32> >::_C_insert_aux(u32* pos, u32 num, const u32& value);
template <>
DECOMP_NOINLINE void vector<u32, nn::nex::MemAllocator<u32> >::_C_insert_aux(u32* pos, const u32& value);

// (inline in Key::operator=; no out-of-line instance)
template <>
inline vector<u8, nn::nex::MemAllocator<u8> >& vector<u8, nn::nex::MemAllocator<u8> >::operator=(const vector& other)
{
    return AssignInline(other);
}
} // namespace std
