#pragma once

#include "decomp.h"
#include "nn/nex/nex_InstanceTable.h"
#include "nn/nex/nex_MutexPrimitive.h"
#include "nn/nex/nex_PseudoSingleton.h"

namespace nn {
namespace nex {
// The pools of the packet buffers (unit heaps for buffers up to a size). The application can
// replace the allocation of the buffers. The names of the members and of the functions other than
// Finalize are ours.
// RTTI N2nn3nex19PacketBufferManagerE @ 0x008CE7E4
// vtable 0x008FD6C8 (vptr 0x008FD6D0), offset_to_top 0, 2 entries
class PacketBufferManager : public ::nn::nex::PseudoSingleton
{
public:
    static const u32 KIND = 14;
    // the size of the largest buffers of the pools (the name is ours)
    static const u32 MAX_POOL_SIZE = 0x554;

    // the size and number of the units of a unit heap of the pools
    struct UnitHeapParam
    {
        u32 m_UnitSize; // 0x0
        u32 m_Count;    // 0x4
    };

    // a unit heap of the pool (0x34 bytes; the layout is not known yet)
    struct Unit
    {
        u32 m_Unknown0x00;
        u32 m_HeapSize;  // 0x04
        u32 m_UnitSize;  // 0x08
        u32 m_UsedCount; // 0x0C
        void* m_pHeap;   // 0x10
        u8 m_Unknown0x14[0x30 - 0x14];
        u32 m_FailureCount; // 0x30
    };
    // the unit heaps by unit size
    struct Pool
    {
        Unit* m_pUnits;            // 0x00 (new[])
        u32 m_UnitCount;           // 0x04
        u32 m_MaxUnitSize;         // 0x08
        bool m_IsInitialized;      // 0x0C
        bool m_IsShort;            // 0x0D (the last HasFreeBuffers failed)
        MutexPrimitive* m_pMutex;  // 0x10
    };

    // (inline in the constructor of Network)
    PacketBufferManager();
    virtual ~PacketBufferManager(); // 0x00394188 slot 0x00
    // 0x00394140 slot 0x04 (deleting dtor)

    // (inline)
    static PacketBufferManager* GetInstance()
    {
        return static_cast<PacketBufferManager*>(InstanceTable::s_Instance.GetInstance(KIND, GetCurrentContext()));
    }
    // (inline) the largest buffer of the pools
    u32 GetMaxUnitSize() const
    {
        if (m_pPool == nullptr) {
            return 0;
        }
        return m_pPool->m_MaxUnitSize;
    }

    // the memory of a packet object (with the allocation function of the application, else from
    // the MemoryManager); none if isDisabled
    static void* AllocateObject(u32 size, bool isDisabled); // 0x003551B8 (name is ours)
    // the pools have count free buffers for size bytes (always if the pools do not hold the size)
    bool HasFreeBuffers(u32 size, u32 count); // 0x00393B7C (name is ours)
    // (not decompiled yet)
    void Finalize(); // 0x00393DE8 | fefates:callgraph [tier C]
    // true if the manager of the context is initialized
    static bool IsInstanceInitialized(); // 0x003940C4 (name is ours)

    // the lock of the users of the managers (name is ours)
    static CriticalSection s_Lock;

    void* (*m_pAllocateFunction)(u32 size);        // 0x14, the buffers of the pools
    void* (*m_pAllocateObjectFunction)(u32 size);  // 0x18
    void* m_Unknown0x1C;                           // 0x1C
    u32 m_Unknown0x20;                             // 0x20
    u32 m_Unknown0x24;                             // 0x24
    u32 m_Unknown0x28;                             // 0x28
    u32 m_Unknown0x2C;                             // 0x2C
    f32 m_Unknown0x30;                             // 0x30
    bool m_IsInitialized;                          // 0x34
    Pool* m_pPool;                                 // 0x38
    Pool* m_pPool2;                                // 0x3C
};
ASSERT_SIZE(PacketBufferManager, 0x40);
} // namespace nex
} // namespace nn
