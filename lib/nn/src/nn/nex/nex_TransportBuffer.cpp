#include "nn/nex/nex_TransportBuffer.h"
#include "nn/nex/nex_Buffer.h"

namespace nn {
namespace nex {
// 0x0037E580 | fefates:callgraph [tier C]
nn::nex::TransportBuffer::TransportBuffer(unsigned int count, unsigned int bufferSize) : m_Lock(LOCK_LEVEL), m_FailureCount(0)
{
    m_pBuffers = new qReservedQueue<Buffer*>(count);
    for (u32 i = 0; i < count; i++) {
        m_pBuffers->PushBack(new Buffer(bufferSize));
    }
}

// 0x0037E6BC
nn::nex::TransportBuffer::~TransportBuffer()
{
    ScopedCS lock(m_Lock);
    while (m_pBuffers->m_Count != 0) {
        Buffer* pBuffer = m_pBuffers->m_Buffer.m_pBegin[m_pBuffers->m_ReadIndex];
        if (pBuffer != nullptr) {
            delete pBuffer;
        }
        m_pBuffers->m_ReadIndex = (m_pBuffers->m_ReadIndex + 1) % m_pBuffers->m_Capacity;
        m_pBuffers->m_Count--;
    }
    delete m_pBuffers;
    m_pBuffers = nullptr;
}

} // namespace nex
} // namespace nn
