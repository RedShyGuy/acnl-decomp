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
    DECOMP_ALWAYS_INLINE void DestroyInline()
    {
        _C_destroy(m_pBegin, m_pEnd);
        nn::nex::MemoryManager::Free(m_pBegin);
    }
    // the elements of the range are destroyed (out of line in some instances: they are specialized)
    void _C_destroy(T* pFirst, T* pLast)
    {
        for (T* p = pFirst; p != pLast; p++) {
            p->~T();
        }
    }

    // (out of line in the original, one instance per type; not decompiled yet; inline in a few
    // instances: they specialize it with the body of AssignInline)
    DECOMP_NOINLINE vector& operator=(const vector& other) { return AssignInline(other); }
    // (RW: with a shorter or equal length the end of the capacity becomes the new end)
    DECOMP_ALWAYS_INLINE vector& AssignInline(const vector& other)
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
    // (out of line per instance) the size becomes num, new values are value
    DECOMP_NOINLINE void resize(u32 num, T value);
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
    // (the body of assign: all values are erased, then the range is inserted)
    DECOMP_ALWAYS_INLINE void AssignRangeInline(const T* pFirst, const T* pLast)
    {
        clear();
        InsertRangeInline(m_pBegin, pFirst, pLast);
    }
    // (the body of the RW _C_insert_aux2) the values of the range at pos
    template <typename It>
    DECOMP_ALWAYS_INLINE void InsertRangeInline(T* pos, It first, It last)
    {
        if (first == last) {
            return;
        }
        u32 num = last - first;
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
            for (It p = first; p != last; p++, pDst++) {
                ::new (static_cast<void*>(pDst)) T(*p);
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
            return;
        }
        T* pOldEnd = m_pEnd;
        m_pEnd = pOldEnd + num;
        if (pOldEnd > pos + num) {
            // the last num values to the new end, the others behind them, then the range
            T* pDst = pOldEnd;
            for (T* p = pOldEnd - num; p != pOldEnd; p++, pDst++) {
                ::new (static_cast<void*>(pDst)) T(*p);
            }
            pDst = pOldEnd;
            for (T* p = pOldEnd - num; p != pos;) {
                *--pDst = *--p;
            }
            pDst = pos;
            for (It p = first; p != last; p++, pDst++) {
                *pDst = *p;
            }
        } else {
            It mid = first + (pOldEnd - pos);
            T* pDst = pOldEnd;
            for (It p = mid; p != last; p++, pDst++) {
                ::new (static_cast<void*>(pDst)) T(*p);
            }
            pDst = pos + num;
            for (T* p = pos; p != pOldEnd; p++, pDst++) {
                ::new (static_cast<void*>(pDst)) T(*p);
            }
            pDst = pos;
            for (It p = first; p != mid; p++, pDst++) {
                *pDst = *p;
            }
        }
    }
    template <typename It>
    void assign(It first, It last);
    // (out of line per instance) the values of the range at pPos
    template <typename It>
    DECOMP_NOINLINE void insert(T* pPos, It first, It last);
    template <typename It>
    DECOMP_NOINLINE void _C_insert_aux2(T* pPos, It first, It last);

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
    // (error 9 = out of range, with the index and the size)
    T& at(u32 index) const
    {
        if (index >= size()) {
            StlThrow(9, "", "", index, size());
        }
        return m_pBegin[index];
    }
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

    // (inline) one value at pos; the position of the new value
    T* insert(T* pos, const T& value)
    {
        u32 offset = pos - m_pBegin;
        if (m_pEnd != m_pCapacityEnd && pos == m_pEnd) {
            ::new (static_cast<void*>(m_pEnd)) T(value);
            m_pEnd++;
        } else {
            _C_insert_aux(pos, value);
        }
        return m_pBegin + offset;
    }
    // (inline) the value at pos is removed; the position after it
    T* erase(T* pos)
    {
        if (pos + 1 != m_pEnd) {
            for (T* p = pos + 1; p != m_pEnd; p++) {
                p[-1] = *p;
            }
        }
        m_pEnd--;
        m_pEnd->~T();
        return pos;
    }

    // one value at pos (out of line in some instances: they specialize it with the body of
    // InsertAuxInline)
    void _C_insert_aux(T* pos, const T& value) { InsertAuxInline(pos, value); }
    DECOMP_ALWAYS_INLINE void InsertAuxInline(T* pos, const T& value)
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

template <typename T, typename Alloc>
void vector<T, Alloc>::resize(u32 num, T value)
{
    ResizeInline(num, value);
}

template <typename T, typename Alloc>
template <typename It>
void vector<T, Alloc>::insert(T* pPos, It first, It last)
{
    InsertRangeInline(pPos, first, last);
}

template <typename T, typename Alloc>
template <typename It>
void vector<T, Alloc>::_C_insert_aux2(T* pPos, It first, It last)
{
    InsertRangeInline(pPos, first, last);
}
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
    DECOMP_ALWAYS_INLINE void InitializeInline(int num)
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

    // (inline: at least 32 elements of capacity; out of line in some instances, they are specialized)
    qVector(const qVector& other) { InitializeCopyInline(other); }
    DECOMP_ALWAYS_INLINE void InitializeCopyInline(const qVector& other)
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
class qResult;
class DataStoreMetaInfo;
class DataStoreRatingInfo;
class DataStoreReqGetInfo;
class DataStoreNotification;
class DataStorePasswordInfo;
class DataStoreNotificationV1;
class DataStorePersistenceInfo;
class DataStoreSpecificMetaInfo;
class DataStoreSpecificMetaInfoV1;
// (out of line)
template <>
DECOMP_NOINLINE qVector<u8>::qVector(int num);
template <>
DECOMP_NOINLINE qVector<String>::qVector(int num);
template <>
DECOMP_NOINLINE qVector<DataStoreKeyValue>::qVector(int num);
template <>
DECOMP_NOINLINE qVector<u8>::qVector(const qVector<u8>& other);
template <>
DECOMP_NOINLINE qVector<String>::qVector(const qVector<String>& other);
template <>
DECOMP_NOINLINE qVector<DataStoreKeyValue>::~qVector();
template <>
DECOMP_NOINLINE qVector<DataStoreMetaInfo>::~qVector();
template <>
DECOMP_NOINLINE qVector<DataStoreRatingInfo>::~qVector();
template <>
DECOMP_NOINLINE qVector<DataStoreReqGetInfo>::~qVector();
template <>
DECOMP_NOINLINE qVector<DataStoreNotification>::~qVector();
template <>
DECOMP_NOINLINE qVector<DataStorePasswordInfo>::~qVector();
template <>
DECOMP_NOINLINE qVector<DataStoreNotificationV1>::~qVector();
template <>
DECOMP_NOINLINE qVector<DataStorePersistenceInfo>::~qVector();
template <>
DECOMP_NOINLINE qVector<DataStoreSpecificMetaInfo>::~qVector();
template <>
DECOMP_NOINLINE qVector<DataStoreSpecificMetaInfoV1>::~qVector();
} // namespace nex
} // namespace nn

namespace std {
// (out of line)
template <>
DECOMP_NOINLINE vector<u64, nn::nex::MemAllocator<u64> >::~vector();
template <>
DECOMP_NOINLINE vector<nn::nex::qResult, nn::nex::MemAllocator<nn::nex::qResult> >::~vector();
template <>
DECOMP_NOINLINE void vector<nn::nex::qResult, nn::nex::MemAllocator<nn::nex::qResult> >::_C_destroy(nn::nex::qResult* pFirst, nn::nex::qResult* pLast);
template <>
DECOMP_NOINLINE void vector<nn::nex::qResult, nn::nex::MemAllocator<nn::nex::qResult> >::_C_insert_aux(nn::nex::qResult* pos, const nn::nex::qResult& value);
template <>
DECOMP_NOINLINE void vector<u64, nn::nex::MemAllocator<u64> >::_C_insert_aux(u64* pos, const u64& value);
template <>
DECOMP_NOINLINE void vector<u8, nn::nex::MemAllocator<u8> >::_C_insert_aux(u8* pos, const u8& value);
template <>
DECOMP_NOINLINE void vector<u8, nn::nex::MemAllocator<u8> >::_C_insert_aux(u8* pos, u32 num, const u8& value);
template <>
DECOMP_NOINLINE void vector<u32, nn::nex::MemAllocator<u32> >::_C_insert_aux(u32* pos, u32 num, const u32& value);
template <>
DECOMP_NOINLINE void vector<u32, nn::nex::MemAllocator<u32> >::_C_insert_aux(u32* pos, const u32& value);
template <>
DECOMP_NOINLINE void vector<u32, nn::nex::MemAllocator<u32> >::assign(const u32* pFirst, const u32* pLast);

// (inline in Key::operator=; no out-of-line instance)
template <>
inline vector<u8, nn::nex::MemAllocator<u8> >& vector<u8, nn::nex::MemAllocator<u8> >::operator=(const vector& other)
{
    return AssignInline(other);
}
// (inline in DataStoreSearchParam::SetDataTypes; no out-of-line instance)
template <>
inline vector<u16, nn::nex::MemAllocator<u16> >& vector<u16, nn::nex::MemAllocator<u16> >::operator=(const vector& other)
{
    return AssignInline(other);
}

// std::vector<bool> of the RW STL: the bits in words, the begin and the end as (word, bit). The
// member names are ours.
template <>
class vector<bool, nn::nex::MemAllocator<bool> >
{
public:
    static const int WORD_BIT = 32;

    // a bit position (inline)
    struct iterator
    {
        iterator() {}
        iterator(u32* pWord, u32 offset) : m_pWord(pWord), m_Offset(offset) {}

        bool operator==(const iterator& other) const { return m_pWord == other.m_pWord && m_Offset == other.m_Offset; }
        bool operator!=(const iterator& other) const { return !(*this == other); }
        // the bits between two positions
        int operator-(const iterator& other) const
        {
            return (m_pWord - other.m_pWord) * WORD_BIT + m_Offset - other.m_Offset;
        }
        bool Get() const { return (*m_pWord & (1u << m_Offset)) != 0; }
        void Set(bool value)
        {
            if (value) {
                *m_pWord |= 1u << m_Offset;
            } else {
                *m_pWord &= ~(1u << m_Offset);
            }
        }
        iterator& operator++()
        {
            if (++m_Offset == WORD_BIT) {
                m_Offset = 0;
                ++m_pWord;
            }
            return *this;
        }
        iterator& operator--()
        {
            if (m_Offset-- == 0) {
                m_Offset = WORD_BIT - 1;
                --m_pWord;
            }
            return *this;
        }
        iterator& operator+=(int num)
        {
            int position = num + static_cast<int>(m_Offset);
            m_pWord += position / WORD_BIT;
            position %= WORD_BIT;
            if (position < 0) {
                m_Offset = position + WORD_BIT;
                --m_pWord;
            } else {
                m_Offset = position;
            }
            return *this;
        }
        iterator operator+(int num) const
        {
            iterator it = *this;
            return it += num;
        }

        u32* m_pWord;  // 0x0
        u32 m_Offset;  // 0x4
    };

    // (inline) count bits of the value
    explicit vector(u32 count, bool value = false)
    {
        u32 words = (count + 31) / 32;
        u32* pWords = static_cast<u32*>(nn::nex::MemoryManager::Allocate(words * sizeof(u32)));
        m_Begin.m_pWord = pWords;
        m_Begin.m_Offset = 0;
        m_End.m_pWord = pWords + count / 32;
        m_End.m_Offset = count % 32;
        m_pEndOfStorage = pWords + words;
        for (u32 i = 0; i < words; i++) {
            pWords[i] = value ? ~0u : 0;
        }
    }
    // (inline)
    ~vector() { nn::nex::MemoryManager::FreeInline(m_Begin.m_pWord); }

    // (inline) the word and the mask of a bit
    u32* GetWord(int index, u32* pMask) const
    {
        int position = m_Begin.m_Offset + index;
        u32* pWord = m_Begin.m_pWord + position / 32;
        int bit = position % 32;
        if (bit < 0) {
            bit += 32;
            pWord--;
        }
        *pMask = 1u << bit;
        return pWord;
    }
    bool Get(int index) const
    {
        u32 mask;
        return (*GetWord(index, &mask) & mask) != 0;
    }
    void Set(int index)
    {
        u32 mask;
        u32* pWord = GetWord(index, &mask);
        *pWord |= mask;
    }
    void Clear(int index)
    {
        u32 mask;
        u32* pWord = GetWord(index, &mask);
        *pWord &= ~mask;
    }
    iterator begin() const { return m_Begin; }
    iterator end() const { return m_End; }
    u32 size() const { return end() - begin(); }
    u32 capacity() const { return iterator(m_pEndOfStorage, 0) - begin(); }

    // (out of line) bits are appended with the value or erased at the end
    void resize(u32 count, bool value); // 0x00834FBC
    // (inline) num bits of the value in front of pos
    DECOMP_ALWAYS_INLINE void insert(iterator pos, u32 num, const bool& value)
    {
        if (num == 0) {
            return;
        }
        if (capacity() - size() >= num) {
            CopyBackward(pos, end(), end() + num);
            Fill(pos, pos + num, value);
            m_End += num;
        } else {
            u32 oldSize = size();
            u32 length = oldSize + (oldSize < num ? num : oldSize);
            u32 words = (length + WORD_BIT - 1) / WORD_BIT;
            u32* pWords = static_cast<u32*>(nn::nex::MemoryManager::Allocate(words * sizeof(u32)));
            iterator it = _C_copy(begin(), pos, iterator(pWords, 0));
            FillN(it, num, value);
            m_End = _C_copy(pos, end(), it + num);
            nn::nex::MemoryManager::FreeInline(m_Begin.m_pWord);
            m_pEndOfStorage = pWords + words;
            m_Begin = iterator(pWords, 0);
        }
    }
    // (inline) the bits from first to last are erased
    iterator erase(iterator first, iterator last)
    {
        m_End = _C_copy(last, end(), first);
        return first;
    }
    // copies the bits from first to last to result; the end of the copy (out of line)
    template <typename It>
    DECOMP_NOINLINE iterator _C_copy(It first, It last, iterator result)
    {
        while (first != last) {
            result.Set(first.Get());
            ++result;
            ++first;
        }
        return result;
    }
    // (inline, the std algorithms on the bits)
    static void CopyBackward(iterator first, iterator last, iterator result)
    {
        while (first != last) {
            --result;
            --last;
            result.Set(last.Get());
        }
    }
    static void Fill(iterator first, iterator last, bool value)
    {
        for (; first != last; ++first) {
            first.Set(value);
        }
    }
    static void FillN(iterator first, u32 num, bool value)
    {
        for (; num > 0; num--) {
            first.Set(value);
            ++first;
        }
    }

    iterator m_Begin;         // 0x00
    iterator m_End;           // 0x08
    u32* m_pEndOfStorage;     // 0x10
};
} // namespace std
