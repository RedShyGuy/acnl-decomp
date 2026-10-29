#include "sead/seadNew.h"
#include "sead/seadHeap.h"
#include "sead/seadHeapMgr.h"

#include <cstdlib>

namespace sead {
// 0x009762E0
bool s_IsDeleteFromCurrentHeapFirst = false;

// 0x001328EC (name is ours)
void* allocFromHeap_(Heap* heap, size_t size, s32 alignment, bool)
{
    if (HeapMgr::s_pInstance == nullptr) {
        return std::malloc(size);
    }
    if (heap == nullptr) {
        heap = HeapMgr::s_pInstance->getCurrentHeap();
        if (heap == nullptr) {
            return nullptr;
        }
    }
    return heap->tryAlloc(size, alignment);
}

namespace {
// the heap that holds p
Heap* findFreeHeap(void* p)
{
    if (s_IsDeleteFromCurrentHeapFirst) {
        Heap* current = HeapMgr::s_pInstance->getCurrentHeap();
        if (current != nullptr) {
            Heap* heap = current->findContainHeap_(p);
            if (heap != nullptr) {
                return heap;
            }
        }
    }
    return HeapMgr::s_pInstance->findContainHeap(p);
}
} // namespace
} // namespace sead

// 0x002FD0BC (name after symbols_us)
void* operator new(size_t size, sead::Heap* heap, int alignment)
{
    return sead::allocFromHeap_(heap, size, alignment, true);
}

// 0x0012E3C0 (name after symbols_us)
void* operator new[](size_t size, sead::Heap* heap, int alignment)
{
    return sead::allocFromHeap_(heap, size, alignment, true);
}

// 0x001328D8
void* operator new(size_t size, sead::Heap* heap, int alignment, const std::nothrow_t&)
{
    return sead::allocFromHeap_(heap, size, alignment, false);
}

// 0x002FF7DC (name after the C++ operator)
void* operator new[](size_t size)
{
    return sead::allocFromHeap_(nullptr, size, 4, true);
}

// 0x0013A008 (name after the C++ operator)
void* operator new(size_t size, const std::nothrow_t&) noexcept
{
    return sead::allocFromHeap_(nullptr, size, 4, false);
}

// 0x0077A51C (name after the C++ operator)
void* operator new[](size_t size, const std::nothrow_t&) noexcept
{
    return sead::allocFromHeap_(nullptr, size, 4, false);
}

// 0x002FF7F8
void operator delete(void* p)
{
    if (sead::HeapMgr::s_pInstance == nullptr) {
        std::free(p);
        return;
    }
    if (p == nullptr) {
        return;
    }
    sead::Heap* heap = sead::findFreeHeap(p);
    if (heap != nullptr) {
        heap->free(p);
    }
}

// 0x002F8958
void operator delete[](void* p)
{
    if (sead::HeapMgr::s_pInstance == nullptr) {
        std::free(p);
        return;
    }
    if (p == nullptr) {
        return;
    }
    sead::Heap* heap = sead::findFreeHeap(p);
    if (heap != nullptr) {
        heap->free(p);
    }
}
