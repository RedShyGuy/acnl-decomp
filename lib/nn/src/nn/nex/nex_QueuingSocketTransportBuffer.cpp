#include "nn/nex/nex_QueuingSocketTransportBuffer.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_Condition.h"

namespace nn {
namespace nex {
// (inline in SocketTransport)
nn::nex::QueuingSocketTransportBuffer::QueuingSocketTransportBuffer(u32 context, nn::nex::RootTransport* pTransport, u32 maxPacketSize,
                                                                    nn::nex::TransportBuffer* pTransportBuffer, u32 bufferCount, u32 receiveCount, u32 sendCount)
    : QueuingSocket(context, pTransport, maxPacketSize, pTransportBuffer), m_ReceiveSet(pTransportBuffer, bufferCount), m_SendSet(pTransportBuffer, bufferCount),
      m_ReceiveCount(receiveCount), m_SendCount(sendCount), m_pCondition(nullptr)
{
    m_ReceiveSet.m_BufferSize = m_MaxPacketSize;
    m_SendSet.m_BufferSize = m_MaxPacketSize;
    m_pReceiveBuffer = new Buffer(m_MaxPacketSize);
}

// 0x003BDDFC slot 0x20
s32 nn::nex::QueuingSocketTransportBuffer::ReceiveDirect(nn::nex::Buffer* pBuffer, nn::nex::InetAddress* pAddress)
{
    m_ReceiveSet.m_Lock.Enter();
    TransportBufferSet::Entry* pEntry = m_ReceiveSet.PopUsed();
    m_ReceiveSet.m_Lock.Leave();
    if (pEntry == nullptr || pEntry->m_pBuffer == nullptr) {
        return RESULT_ERROR;
    }
    *pBuffer = *pEntry->m_pBuffer;
    pAddress->m_SockAddr = pEntry->m_Address.m_SockAddr;
    ScopedCS lock(m_ReceiveSet.m_Lock);
    m_ReceiveSet.Release(pEntry);
    return RESULT_SUCCESS;
}

// 0x003BDF00 slot 0x1C
s32 nn::nex::QueuingSocketTransportBuffer::SendDirect(nn::nex::Buffer* pBuffer, nn::nex::InetAddress* pAddress)
{
    if (pBuffer->m_ContentSize == 0) {
        return RESULT_ERROR;
    }
    ScopedCS lock(m_SendSet.m_Lock);
    TransportBufferSet::Entry* pEntry = m_SendSet.Allocate();
    if (pEntry == nullptr || pEntry->m_pBuffer == nullptr) {
        if (m_pCondition != nullptr) {
            m_pCondition->Signal();
        }
        return RESULT_ERROR;
    }
    *pEntry->m_pBuffer = *pBuffer;
    pEntry->m_Address.m_SockAddr = pAddress->m_SockAddr;
    if (m_pCondition != nullptr) {
        m_pCondition->Signal();
    }
    return RESULT_SUCCESS;
}

// 0x003BDFDC (name is ours)
bool nn::nex::QueuingSocketTransportBuffer::SendBuffers()
{
    for (u32 i = 0; i < m_SendCount; i++) {
        if (m_SendSet.m_HasData != true || !m_IsAvailable) {
            return false;
        }
        m_SendSet.m_Lock.Enter();
        TransportBufferSet::Entry* pEntry = m_SendSet.PopUsed();
        m_SendSet.m_Lock.Leave();
        if (pEntry == nullptr || pEntry->m_pBuffer == nullptr) {
            return false;
        }
        s32 result = SendInline(pEntry->m_pBuffer, &pEntry->m_Address);
        m_SendSet.m_Lock.Enter();
        m_SendSet.Release(pEntry);
        m_SendSet.m_Lock.Leave();
        if (result != RESULT_SUCCESS) {
            return false;
        }
    }
    return true;
}

// 0x003BE178 (name is ours)
bool nn::nex::QueuingSocketTransportBuffer::ReceiveBuffers(bool isNonBlocking)
{
    for (u32 i = 0; i < m_ReceiveCount; i++) {
        if (m_ReceiveSet.m_IsFull || !m_IsAvailable) {
            return false;
        }
        InetAddress address;
        m_pReceiveBuffer->m_ContentSize = 0;
        m_pReceiveBuffer->m_HeaderSize = 0;
        Buffer* pBuffer = m_pReceiveBuffer;
        u32 receivedSize = 0;
        if (RecvFrom(pBuffer->m_pData + pBuffer->m_HeaderSize, pBuffer->m_Size - pBuffer->m_HeaderSize, &receivedSize, &address,
                     isNonBlocking ? SocketDriver::SOCKET_FLAG_DONT_WAIT : SocketDriver::SOCKET_FLAG_NONE) != RESULT_SUCCESS) {
            return false;
        }
        pBuffer->m_ContentSize = receivedSize;
        Buffer* pReceived = m_pReceiveBuffer;
        ScopedCS lock(m_ReceiveSet.m_Lock);
        TransportBufferSet::Entry* pEntry = m_ReceiveSet.Allocate();
        if (pEntry != nullptr && pEntry->m_pBuffer != nullptr) {
            *pEntry->m_pBuffer = *pReceived;
            pEntry->m_Address.m_SockAddr = address.m_SockAddr;
        }
    }
    return true;
}

// 0x003BE368 slot 0x00
// 0x003BE2E8 (deleting dtor)
nn::nex::QueuingSocketTransportBuffer::~QueuingSocketTransportBuffer()
{
    m_ReceiveSet.ReleaseAll(&m_ReceiveSet.m_UsedEntries);
    m_SendSet.ReleaseAll(&m_SendSet.m_UsedEntries);
    if (m_pReceiveBuffer != nullptr) {
        delete m_pReceiveBuffer;
    }
}

// 0x0072DE44 slot 0x18
bool nn::nex::QueuingSocketTransportBuffer::CanReceive()
{
    if (m_InputEmulator.m_pDevice->m_IsEnabled) {
        return true;
    }
    return m_ReceiveSet.m_HasData;
}

} // namespace nex
} // namespace nn
