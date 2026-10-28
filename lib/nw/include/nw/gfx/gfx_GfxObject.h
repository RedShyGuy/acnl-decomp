#pragma once

#include "decomp.h"
#include "nw/os/os_IAllocator.h"

namespace nw {
namespace gfx {
// The results of the gfx setup and binding functions: bit 31 marks a failure (names are ours).
const u32 RESULT_SUCCESS = 0;
const u32 RESULT_FAILURE_MASK = 0x80000000;

// The runtime type of a gfx object: GetRuntimeTypeInfo returns the address of a static object per
// class, which points to the one of the base class (the root: NULL). Each file sets its objects
// in its static initializer. Struct and member names are ours.
struct TypeInfo
{
    explicit TypeInfo(const TypeInfo* parent) : pParent(parent) {}

    const TypeInfo* pParent; // 0x0
};

// RTTI N2nw3gfx9GfxObjectE @ 0x008D082C
// The base of the gfx objects: they remember the allocator of their memory. The deleting
// destructors free nothing (the memory goes back to the allocator elsewhere). Member names are ours.
class GfxObject
{
public:
    // (inline)
    explicit GfxObject(os::IAllocator* pAllocator) : m_pAllocator(pAllocator) {}
    virtual ~GfxObject() {}
    static void operator delete(void* /*p*/) {}

    os::IAllocator* GetAllocator() const { return m_pAllocator; }

    os::IAllocator* m_pAllocator; // 0x4
};
ASSERT_SIZE(GfxObject, 0x8);

// (inline) the object destroyed and given back to its allocator, the pointer set to NULL (name is
// ours)
template <typename T>
inline void SafeDestroy(T*& pObject)
{
    if (pObject != nullptr) {
        pObject->~T();
        pObject->GetAllocator()->Free(pObject);
        pObject = nullptr;
    }
}

// A pointer to a gfx object that destroys the object when it owns it (the class name is from the
// reference symbols, the rest is ours). Copying moves the object; assigning swaps.
template <typename T>
class GfxPtr
{
public:
    // (inline)
    GfxPtr() : m_pObject(nullptr), m_IsOwner(true) {}
    explicit GfxPtr(T* pObject, bool isOwner = true) : m_pObject(pObject), m_IsOwner(isOwner) {}
    GfxPtr(GfxPtr& other) : m_pObject(other.m_pObject), m_IsOwner(other.m_IsOwner) { other.m_pObject = nullptr; }
    ~GfxPtr()
    {
        if (m_pObject != nullptr && m_IsOwner) {
            m_pObject->~T();
            m_pObject->GetAllocator()->Free(m_pObject);
        }
    }
    GfxPtr& operator=(GfxPtr other)
    {
        Swap(other);
        return *this;
    }
    void Swap(GfxPtr& other)
    {
        T* pObject = m_pObject;
        bool isOwner = m_IsOwner;
        m_pObject = other.m_pObject;
        m_IsOwner = other.m_IsOwner;
        other.m_pObject = pObject;
        other.m_IsOwner = isOwner;
    }

    T* Get() const { return m_pObject; }
    T* operator->() const { return m_pObject; }

    T* m_pObject;   // 0x0
    bool m_IsOwner; // 0x4
};
} // namespace gfx
} // namespace nw
