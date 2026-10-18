#include "nn/nex/nex_TransportBufferSet.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_TransportBuffer.h"

namespace nn {
namespace nex {
// 0x003907DC (name is ours)
void nn::nex::TransportBufferSet::ReleaseAll(qReservedQueue<Entry*>* pEntries)
{
    while (pEntries->m_Count != 0) {
        Entry* pEntry = pEntries->m_Buffer.m_pBegin[pEntries->m_ReadIndex];
        if (pEntry != nullptr) {
            Release(pEntry);
        }
        pEntries->m_ReadIndex = (pEntries->m_ReadIndex + 1) % pEntries->m_Capacity;
        pEntries->m_Count--;
    }
}

// 0x003908D4 (name is ours)
nn::nex::TransportBufferSet::Entry* nn::nex::TransportBufferSet::Allocate()
{
    if (m_FreeEntries.m_Count == 0) {
        m_FailureCount++;
        return nullptr;
    }
    Buffer* pBuffer;
    if (m_pTransportBuffer != nullptr) {
        TransportBuffer* pTransportBuffer = m_pTransportBuffer;
        pBuffer = nullptr;
        ScopedCS lock(pTransportBuffer->m_Lock);
        qReservedQueue<Buffer*>* pBuffers = pTransportBuffer->m_pBuffers;
        if (pBuffers->m_Count != 0) {
            pBuffer = pBuffers->PopFront();
            pBuffer->m_ContentSize = 0;
            pBuffer->m_HeaderSize = 0;
        } else {
            pTransportBuffer->m_FailureCount++;
        }
    } else {
        pBuffer = new Buffer(m_BufferSize);
    }
    if (pBuffer == nullptr) {
        return nullptr;
    }
    m_HasData = true;
    Entry* pEntry = m_FreeEntries.PopFront();
    pEntry->m_pBuffer = pBuffer;
    m_UsedEntries.PushBack(pEntry);
    return pEntry;
}

// 0x00390A24 (name is ours)
void nn::nex::TransportBufferSet::Release(Entry* pEntry)
{
    if (pEntry == nullptr) {
        return;
    }
    Buffer* pBuffer = pEntry->m_pBuffer;
    if (pBuffer != nullptr) {
        if (m_pTransportBuffer == nullptr) {
            delete pBuffer;
        } else {
            TransportBuffer* pTransportBuffer = m_pTransportBuffer;
            ScopedCS lock(pTransportBuffer->m_Lock);
            pTransportBuffer->m_pBuffers->PushBack(pBuffer);
        }
    }
    m_FreeEntries.PushBack(pEntry);
}

// 0x00390AE0 | fefates:callgraph [tier C]
nn::nex::TransportBufferSet::TransportBufferSet(nn::nex::TransportBuffer* pTransportBuffer, int count)
    : m_HasData(false), m_IsFull(false), m_UsedEntries(count), m_FreeEntries(count), m_BufferSize(DEFAULT_BUFFER_SIZE), m_Lock(LOCK_LEVEL), m_FailureCount(0),
      m_pTransportBuffer(pTransportBuffer)
{
    for (int i = 0; i < count; i++) {
        m_FreeEntries.PushBack(new Entry());
    }
}

// 0x00390CBC | fefates:bytes [tier B]
nn::nex::TransportBufferSet::~TransportBufferSet()
{
    while (m_FreeEntries.m_Count != 0) {
        Entry* pEntry = m_FreeEntries.m_Buffer.m_pBegin[m_FreeEntries.m_ReadIndex];
        delete pEntry;
        m_FreeEntries.m_ReadIndex = (m_FreeEntries.m_ReadIndex + 1) % m_FreeEntries.m_Capacity;
        m_FreeEntries.m_Count--;
    }
}

} // namespace nex
} // namespace nn
