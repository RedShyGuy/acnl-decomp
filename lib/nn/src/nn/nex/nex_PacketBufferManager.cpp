#include "nn/nex/nex_PacketBufferManager.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_GlobalVariables.h"
#include "nn/nex/nex_MemoryManager.h"
#include "nn/nex/nex_Network.h"
#include "nn/nex/nex_RootTransport.h"

namespace std {
// (inline in Pool::GetStatus; no out-of-line instances)
template <>
DECOMP_ALWAYS_INLINE void vector<nn::nex::PacketBufferManager::Status, nn::nex::MemAllocator<nn::nex::PacketBufferManager::Status> >::_C_insert_aux(nn::nex::PacketBufferManager::Status* pos, const nn::nex::PacketBufferManager::Status& value)
{
    InsertAuxInline(pos, value);
}
template <>
DECOMP_ALWAYS_INLINE void vector<nn::nex::PacketBufferManager::Status, nn::nex::MemAllocator<nn::nex::PacketBufferManager::Status> >::push_back(const nn::nex::PacketBufferManager::Status& value)
{
    if (m_pEnd != m_pCapacityEnd) {
        ::new (static_cast<void*>(m_pEnd)) nn::nex::PacketBufferManager::Status(value);
        m_pEnd++;
    } else {
        _C_insert_aux(m_pEnd, value);
    }
}
} // namespace std

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

namespace {
// the flags of the trace of the statuses (name is ours)
const u64 STATUS_TRACE_FLAGS = 0x0000008000801000ull;
} // namespace

// (inline)
bool nn::nex::PacketBufferManager::IsPoolEnabled()
{
    ScopedCS lock(s_Lock);
    return GlobalVariables::s_pInstance->m_Unknown0x18C;
}

// 0x00393C98 (name is ours)
void nn::nex::PacketBufferManager::TraceStatus(u64 traceFlags)
{
    if (m_pPool != nullptr) {
        std::vector<Status, MemAllocator<Status> > statuses;
        m_pPool->GetStatus(&statuses);
        for (Status* pStatus = statuses.begin(); pStatus != statuses.end(); pStatus++) {
            // (the trace of the status is removed)
        }
        for (u32 i = 1; i <= statuses.size(); i++) {
            // (the trace of the totals is removed)
        }
    }
    if (m_pPool2 != nullptr) {
        std::vector<Status, MemAllocator<Status> > statuses;
        m_pPool2->GetStatus(&statuses);
        for (Status* pStatus = statuses.begin(); pStatus != statuses.end(); pStatus++) {
            // (the trace of the status is removed)
        }
        for (u32 i = 1; i <= statuses.size(); i++) {
            // (the trace of the totals is removed)
        }
    }
}

// 0x00393DE8 | fefates:callgraph [tier C]
void nn::nex::PacketBufferManager::Finalize()
{
    ScopedCS lock(s_Lock);
    if (m_Unknown0x24 != 0 || !IsPoolEnabled() || !IsInstanceInitialized()) {
        return;
    }
    // the packets in the pools go first
    Network* pNetwork = Network::GetInstance();
    if (pNetwork != nullptr && pNetwork->m_pRootTransport != nullptr) {
        pNetwork->m_pRootTransport->DropPoolPackets();
    }
    TraceStatus(STATUS_TRACE_FLAGS);
    if (m_pPool2 != nullptr) {
        m_pPool2->Finalize();
        delete m_pPool2;
        m_pPool2 = nullptr;
    }
    if (m_pPool != nullptr) {
        m_pPool->Finalize();
        delete m_pPool;
        m_pPool = nullptr;
    }
    MemoryManager::FreeInline(m_Unknown0x1C);
    m_Unknown0x1C = nullptr;
    m_pAllocateFunction = nullptr;
    m_pAllocateObjectFunction = nullptr;
    m_IsInitialized = false;
}

// 0x0072D6B0 (name is ours)
void nn::nex::PacketBufferManager::Pool::GetStatus(std::vector<Status, MemAllocator<Status> >* pStatuses)
{
    if (m_pMutex != nullptr && !CriticalSection::s_IsLockDisabled) {
        m_pMutex->Enter();
    }
    for (u32 i = 0; i < m_UnitCount; i++) {
        pStatuses->push_back(m_pUnits[i].m_Status);
    }
    if (m_pMutex != nullptr && !CriticalSection::s_IsLockDisabled) {
        m_pMutex->Leave();
    }
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
        pPool->m_pUnits[pPool->m_UnitCount - 1].m_Status.m_FailureCount++;
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
// 0x0083144C | fefates:bytes [tier B]
template vector<nn::nex::PacketBufferManager::UnitHeapParam, nn::nex::MemAllocator<nn::nex::PacketBufferManager::UnitHeapParam> >::~vector();
} // namespace std
