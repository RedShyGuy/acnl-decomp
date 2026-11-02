#include "ssys/ma/dVramExpHeap.h"

namespace ssys {
namespace ma {
// 0x00567314 (name is ours)
void ssys::ma::VramExpHeap::MergeFreeBlock(VramMemBlock* block)
{
    VramMemBlock* prev = static_cast<VramMemBlock*>(block->m_pPrev);
    if (prev != NULL && prev->m_Address + prev->m_Size == block->m_Address) {
        prev->m_Size += block->m_Size;
        m_FreeList.Erase(block);
        block->~VramMemBlock();
        m_pBlockHeap->free(block);
        block = prev;
    }
    VramMemBlock* next = static_cast<VramMemBlock*>(block->m_pNext);
    if (next != NULL && block->m_Address + block->m_Size == next->m_Address) {
        block->m_Size += next->m_Size;
        m_FreeList.Erase(next);
        next->~VramMemBlock();
        m_pBlockHeap->free(next);
    }
}

// 0x005673E8 slot 0x0C
void ssys::ma::VramExpHeap::Free(uptr address)
{
    m_CriticalSection.lock();
    for (VramMemBlock* block = static_cast<VramMemBlock*>(m_UsedList.GetFirst()); block != NULL;
         block = static_cast<VramMemBlock*>(block->m_pNext)) {
        if (block->m_Address + block->m_Offset == address) {
            m_UsedList.Erase(block);
            AddFreeBlock(block);
            m_CriticalSection.unlock();
            return;
        }
    }
    m_CriticalSection.unlock();
}

// 0x00567474 slot 0x08
uptr ssys::ma::VramExpHeap::Alloc(u32 size, s32 alignment)
{
    if (alignment < 4) {
        alignment = 4;
    }
    if (((alignment - 1) & alignment) != 0) {
        alignment = 4;
    }
    size = (size + 3) & ~3;
    m_CriticalSection.lock();
    VramMemBlock* found = NULL;
    u32 rest = m_Size + 1;
    for (VramMemBlock* block = static_cast<VramMemBlock*>(m_FreeList.GetFirst()); block != NULL;
         block = static_cast<VramMemBlock*>(block->m_pNext)) {
        u32 offset = ((block->m_Address + alignment - 1) & ~(alignment - 1)) - block->m_Address;
        u32 free = block->m_Size < offset ? 0 : block->m_Size - offset;
        if (size > free) {
            continue;
        }
        u32 left = free - size;
        if (left == 0 || !m_IsBestFit) {
            rest = left;
            found = block;
            break;
        }
        if (found == NULL || rest > left) {
            rest = left;
            found = block;
        }
    }
    if (found == NULL) {
        m_CriticalSection.unlock();
        return 0;
    }
    VramMemBlock* split = NULL;
    if (rest != 0) {
        void* memory = m_pBlockHeap->tryAlloc(sizeof(VramMemBlock), 4);
        if (memory == NULL) {
            m_CriticalSection.unlock();
            return 0;
        }
        split = new (memory) VramMemBlock();
    }
    m_FreeList.Erase(found);
    found->m_Offset = ((found->m_Address + alignment - 1) & ~(alignment - 1)) - found->m_Address;
    if (split == NULL) {
        m_UsedList.InsertByPriority(found);
    } else {
        found->m_Size -= rest;
        split->m_Address = found->m_Address + found->m_Size;
        split->m_Size = rest;
        split->m_Offset = 0;
        m_UsedList.InsertByPriority(found);
        AddFreeBlock(split);
    }
    uptr address = found->m_Address + found->m_Offset;
    m_CriticalSection.unlock();
    return address;
}

// 0x00567650 (name is ours)
ssys::ma::VramExpHeap* ssys::ma::VramExpHeap::Create(u32 size, const sead::SafeStringBase<char>& name, VramHeap* parent,
                                                     u32 heapSize, sead::Heap* heap)
{
    if (size == 0) {
        size = parent->GetMaxAllocatableSize(4);
    }
    sead::ExpHeap* expHeap = sead::ExpHeap::create(heapSize != 0 ? heapSize : heap->getMaxAllocatableSize(4), name,
                                                   heap, sead::Heap::HEAP_DIRECTION_FORWARD, false);
    if (expHeap == NULL) {
        return NULL;
    }
    expHeap->setAllocMode(sead::ExpHeap::FIND_MODE_FIRST_FIT);
    void* memory = expHeap->tryAlloc(sizeof(VramExpHeap), 4);
    if (memory != NULL) {
        uptr address = parent->Alloc(size, 4);
        if (address != 0) {
            VramExpHeap* vramHeap = new (memory) VramExpHeap(name, expHeap, parent, address, size);
            vramHeap->ResetFreeList();
            return vramHeap;
        }
    }
    expHeap->destroy();
    return NULL;
}

// 0x00567768 slot 0x04
u32 ssys::ma::VramExpHeap::Adjust()
{
    if (m_pParent == NULL) {
        return m_Size;
    }
    m_CriticalSection.lock();
    VramMemBlock* last = static_cast<VramMemBlock*>(m_FreeList.GetLast());
    if (last == NULL) {
        u32 size = m_Size;
        m_CriticalSection.unlock();
        return size;
    }
    u32 newSize = m_Size - last->m_Size;
    if (newSize == 0) {
        m_CriticalSection.unlock();
        return 0;
    }
    m_FreeList.Erase(last);
    last->~VramMemBlock();
    m_pBlockHeap->free(last);
    u32 oldSize = m_Size;
    m_Size = newSize;
    m_pParent->Resize(m_Address, oldSize, newSize);
    m_CriticalSection.unlock();
    return newSize;
}

// 0x0056782C slot 0x20
void ssys::ma::VramExpHeap::Dump()
{
    // empty (in the original too)
}

// (no lock, unlike the other functions)
// 0x00750B08 slot 0x14
u32 ssys::ma::VramExpHeap::GetFreeSize()
{
    m_CriticalSection.lock();
    u32 size = 0;
    for (VramMemBlock* block = static_cast<VramMemBlock*>(m_FreeList.GetFirst()); block != NULL;
         block = static_cast<VramMemBlock*>(block->m_pNext)) {
        size += block->m_Size;
    }
    m_CriticalSection.unlock();
    return size;
}

// 0x00750B4C slot 0x18
u32 ssys::ma::VramExpHeap::GetMaxAllocatableSize(s32 alignment)
{
    m_CriticalSection.lock();
    if (alignment < 4) {
        alignment = 4;
    }
    if (((alignment - 1) & alignment) != 0) {
        alignment = 4;
    }
    u32 maxSize = 0;
    for (VramMemBlock* block = static_cast<VramMemBlock*>(m_FreeList.GetFirst()); block != NULL;
         block = static_cast<VramMemBlock*>(block->m_pNext)) {
        u32 offset = ((block->m_Address + alignment - 1) & ~(alignment - 1)) - block->m_Address;
        u32 size = block->m_Size < offset ? 0 : block->m_Size - offset;
        if (maxSize < size) {
            maxSize = size;
        }
    }
    m_CriticalSection.unlock();
    return maxSize;
}

// 0x00567830 slot 0x1C
bool ssys::ma::VramExpHeap::Resize(uptr address, u32 oldSize, u32 newSize)
{
    if (newSize == 0) {
        return false;
    }
    for (VramMemBlock* block = static_cast<VramMemBlock*>(m_UsedList.GetFirst()); block != NULL;
         block = static_cast<VramMemBlock*>(block->m_pNext)) {
        if (block->m_Address + block->m_Offset != address) {
            continue;
        }
        u32 size = block->m_Size - block->m_Offset;
        if (newSize > size || size != oldSize) {
            return false;
        }
        if (size == newSize) {
            return true;
        }
        void* memory = m_pBlockHeap->tryAlloc(sizeof(VramMemBlock), 4);
        if (memory == NULL) {
            return false;
        }
        VramMemBlock* split = new (memory) VramMemBlock();
        split->m_Address = address + newSize;
        split->m_Size = size - newSize;
        AddFreeBlock(split);
        block->m_Size = block->m_Offset + newSize;
        return true;
    }
    return false;
}

// 0x00567928 slot 0x00
void ssys::ma::VramExpHeap::Destroy()
{
    VramHeap* self = this;
    self->~VramHeap();
    if (m_pHeap != NULL) {
        m_pHeap->destroy();
    }
}

// 0x00567960 slot 0x10
void ssys::ma::VramExpHeap::FreeAll()
{
    m_CriticalSection.lock();
    m_pBlockHeap->freeAll();
    m_UsedList.Clear();
    m_FreeList.Clear();
    ResetFreeList();
    m_CriticalSection.unlock();
}

// 0x00567A24 slot 0x24
// 0x005679A8 (deleting dtor)
ssys::ma::VramExpHeap::~VramExpHeap()
{
    if (m_Address != 0 && m_pParent != NULL) {
        m_pParent->Free(m_Address);
    }
    if (m_pBlockHeap != NULL) {
        m_pBlockHeap->destroy();
        m_pBlockHeap = NULL;
    }
    m_Address = 0;
    m_pParent = NULL;
}

} // namespace ma
} // namespace ssys
