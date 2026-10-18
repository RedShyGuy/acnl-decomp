#include "nn/nex/nex_PacketBufferManager.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_MemoryManager.h"

namespace nn {
namespace nex {
// 0x00ADE8A8 (name is ours)
CriticalSection PacketBufferManager::s_Lock(0x20000000);

// 0x003940C4 (name is ours)
bool nn::nex::PacketBufferManager::IsInstanceInitialized()
{
    ScopedCS lock(s_Lock);
    PacketBufferManager* pManager = GetInstance();
    return pManager != nullptr && pManager->m_IsInitialized;
}

// (inline in the constructor of Network)
nn::nex::PacketBufferManager::PacketBufferManager()
    : PseudoSingleton(KIND), m_pAllocateFunction(nullptr), m_pAllocateObjectFunction(nullptr), m_Unknown0x1C(nullptr), m_Unknown0x20(0),
      m_Unknown0x24(0), m_Unknown0x28(8), m_Unknown0x2C(4), m_Unknown0x30(0.0f), m_IsInitialized(false), m_pPool(nullptr), m_pPool2(nullptr)
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x00394188
// 0x00394140 (deleting dtor)
nn::nex::PacketBufferManager::~PacketBufferManager()
{
    m_Unknown0x24 = 0;
    Finalize();
}

// 0x003551B8 (name is ours)
void* nn::nex::PacketBufferManager::AllocateObject(u32 size, bool isDisabled)
{
    if (isDisabled) {
        return nullptr;
    }
    void* (*pFunction)(u32) = GetInstance()->m_pAllocateObjectFunction;
    if (pFunction != nullptr) {
        return pFunction(size);
    }
    return MemoryManager::Allocate(size);
}

// 0x00393B7C (name is ours)
bool nn::nex::PacketBufferManager::HasFreeBuffers(u32 size, u32 count)
{
    Pool* pPool = m_pPool;
    if (pPool == nullptr || size > pPool->m_MaxUnitSize) {
        return true;
    }
    bool hasFree = false;
    if (pPool->m_pMutex != nullptr && !CriticalSection::s_IsLockDisabled) {
        pPool->m_pMutex->Enter();
    }
    u32 freeCount = 0;
    for (u32 i = 0; i < pPool->m_UnitCount; i++) {
        Unit* pUnit = &pPool->m_pUnits[i];
        if (pUnit->m_UnitSize < size) {
            continue;
        }
        // the units of the heap (with their headers) less the used ones
        freeCount += pUnit->m_HeapSize / ((pUnit->m_UnitSize + 11) & ~3u) - pUnit->m_UsedCount;
        if (count <= freeCount) {
            hasFree = true;
            break;
        }
    }
    if (!hasFree) {
        pPool->m_pUnits[pPool->m_UnitCount - 1].m_FailureCount++;
    }
    pPool->m_IsShort = !hasFree;
    if (pPool->m_pMutex != nullptr && !CriticalSection::s_IsLockDisabled) {
        pPool->m_pMutex->Leave();
    }
    return hasFree;
}

} // namespace nex
} // namespace nn

namespace std {
// 0x0083144C (name is ours)
template vector<nn::nex::PacketBufferManager::UnitHeapParam, nn::nex::MemAllocator<nn::nex::PacketBufferManager::UnitHeapParam> >::~vector();
} // namespace std
