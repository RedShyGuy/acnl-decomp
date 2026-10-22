#pragma once

#include <new>

#include "decomp.h"
#include "nw/os/os_IAllocator.h"

namespace nw {
namespace ut {
// An array in a buffer from an allocator (the class name and push_back/Resize are from the reference
// symbols; everything else is ours). Assigning one moves the buffer (the old one is freed with the
// temporary). The top two bits of the capacity word say whether the array may grow.
template <typename T>
class MoveArray
{
public:
    enum Kind
    {
        KIND_FIXED = 0,    // the buffer as given
        KIND_VARIABLE = 1, // a new buffer from the allocator when full
        KIND_SHIFT = 30,
    };
    static const u32 KIND_MASK = 0xC0000000;

    // (inline)
    MoveArray() : m_pAllocator(nullptr), m_pBegin(nullptr), m_pEnd(nullptr), m_Capacity(0 & ~KIND_MASK) {}
    // an empty array that takes its buffers from the allocator
    explicit MoveArray(os::IAllocator* pAllocator)
        : m_pAllocator(pAllocator), m_pBegin(nullptr), m_pEnd(nullptr), m_Capacity(0 | (static_cast<u32>(KIND_VARIABLE) << KIND_SHIFT))
    {
    }
    MoveArray(void* pBuffer, s32 capacity, os::IAllocator* pAllocator = nullptr, Kind kind = KIND_FIXED)
        : m_pAllocator(pAllocator), m_pBegin(static_cast<T*>(pBuffer)), m_pEnd(static_cast<T*>(pBuffer)),
          m_Capacity((capacity & ~KIND_MASK) | (static_cast<u32>(kind) << KIND_SHIFT))
    {
    }
    ~MoveArray()
    {
        clear();
        if (m_pAllocator != nullptr && m_pBegin != nullptr) {
            m_pAllocator->Free(m_pBegin);
        }
    }
    // takes the buffer of the other array (which frees the old one when it is destroyed)
    MoveArray& operator=(MoveArray& other)
    {
        if (this != &other) {
            Swap(other);
        }
        return *this;
    }

    void Swap(MoveArray& other)
    {
        os::IAllocator* pAllocator = m_pAllocator;
        T* pBegin = m_pBegin;
        T* pEnd = m_pEnd;
        u32 capacity = m_Capacity;
        m_pAllocator = other.m_pAllocator;
        m_pBegin = other.m_pBegin;
        m_pEnd = other.m_pEnd;
        m_Capacity = other.m_Capacity;
        other.m_pAllocator = pAllocator;
        other.m_pBegin = pBegin;
        other.m_pEnd = pEnd;
        other.m_Capacity = capacity;
    }

    s32 size() const { return m_pEnd - m_pBegin; }
    s32 capacity() const { return m_Capacity & ~KIND_MASK; }
    Kind GetKind() const { return static_cast<Kind>(m_Capacity >> KIND_SHIFT); }
    bool empty() const { return m_pBegin == m_pEnd; }
    T* begin() { return m_pBegin; }
    const T* begin() const { return m_pBegin; }
    T* end() { return m_pEnd; }
    const T* end() const { return m_pEnd; }
    T& operator[](s32 index) { return m_pBegin[index]; }
    const T& operator[](s32 index) const { return m_pBegin[index]; }

    void clear()
    {
        for (T* p = m_pBegin; p != m_pEnd; ++p) {
            p->~T();
        }
        m_pEnd = m_pBegin;
    }

    // room for capacity elements; false if the array cannot grow
    bool reserve(s32 capacity)
    {
        if (this->capacity() >= capacity) {
            return true;
        }
        if (m_pAllocator == nullptr || (m_Capacity & KIND_MASK) != (KIND_VARIABLE << KIND_SHIFT)) {
            return false;
        }
        T* pBuffer = static_cast<T*>(m_pAllocator->Alloc(capacity * sizeof(T), 32));
        if (pBuffer == nullptr) {
            return false;
        }
        s32 count = 0;
        if (m_pBegin != m_pEnd) {
            T* pDst = pBuffer;
            for (T* pSrc = m_pBegin; pSrc != m_pEnd; ++pSrc, ++pDst) {
                new (pDst) T(*pSrc);
            }
            count = m_pEnd - m_pBegin;
        }
        if (m_pBegin != nullptr) {
            clear();
            m_pAllocator->Free(m_pBegin);
        }
        m_pBegin = pBuffer;
        m_pEnd = pBuffer + count;
        m_Capacity = (m_Capacity & KIND_MASK) | capacity;
        return true;
    }

    // the element added at the end (the capacity doubled when full); false if there is no room
    bool push_back(const T& value)
    {
        bool isSuccess = true;
        if (capacity() <= size()) {
            isSuccess = reserve(capacity() == 0 ? 1 : capacity() * 2);
            if (!isSuccess) {
                return false;
            }
        }
        new (m_pEnd) T(value);
        ++m_pEnd;
        return isSuccess;
    }

    // (name is ours) the element added at the end without a check of the capacity
    void PushBackFast(const T& value)
    {
        new (m_pEnd++) T(value);
    }

    // count elements: the ones beyond destroyed, new ones default-initialized (as many as fit when
    // the array cannot grow); false if not all of them fit or count is negative
    bool Resize(s32 count)
    {
        if (count < size()) {
            bool isSuccess = true;
            if (count < 0) {
                count = 0;
                isSuccess = false;
            }
            for (T* p = m_pBegin + count; p != m_pEnd; ++p) {
                p->~T();
            }
            m_pEnd = m_pBegin + count;
            return isSuccess;
        }
        const bool isSuccess = reserve(count);
        if (!isSuccess) {
            count = capacity();
        }
        for (s32 i = size(); i < count; ++i) {
            new (&m_pBegin[i]) T;
        }
        m_pEnd = m_pBegin + count;
        return isSuccess;
    }

    os::IAllocator* m_pAllocator; // 0x0
    T* m_pBegin;                  // 0x4
    T* m_pEnd;                    // 0x8
    u32 m_Capacity;               // 0xC the capacity and the Kind in the top two bits
};
} // namespace ut
} // namespace nw
