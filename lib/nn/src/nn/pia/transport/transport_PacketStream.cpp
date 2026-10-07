#include "nn/pia/transport/transport_PacketStream.h"
#include "nn/pia/common/common_HeapManager.h"
#include "nn/pia/common/common_Packet.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_WatermarkManager.h"
#include "pead/peadHeapMgr.h"
#include <new>

namespace nn {
namespace pia {
namespace transport {
namespace {
// the alignment of the packets
const int PACKET_ALIGNMENT = 4;
} // namespace

// 0x0044DAAC | fefates:bytes [tier B]
nn::Result nn::pia::transport::PacketStream::Initialize(unsigned int packetNum, unsigned int watermarkIndex)
{
    if (packetNum < 2) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_pPackets != nullptr) {
        return common::RESULT_ALREADY_INITIALIZED;
    }
    pead::Heap* pHeap = common::HeapManager::GetHeap();
    if (static_cast<int>(packetNum) > 0) {
        common::Packet* pPackets = static_cast<common::Packet*>(
            operator new(packetNum * sizeof(common::Packet), pHeap, PACKET_ALIGNMENT));
        if (pPackets != nullptr) {
            for (u32 i = 0; i < packetNum; i++) {
                new (&pPackets[i]) common::Packet();
            }
        }
        if (static_cast<int>(packetNum) > 0 && pPackets != nullptr) {
            m_pPackets = pPackets;
            m_PacketNum = packetNum;
        }
    }
    m_Writer.m_Head = -1;
    m_Writer.m_Position = -1;
    m_Reader.m_Head = -1;
    m_Writer.m_Count = 0;
    m_Reader.m_Position = -1;
    m_Reader.m_Count = 0;
    m_WatermarkIndex = watermarkIndex;
    return nn::Result();
}

// 0x0044DB78 | fefates:bytes [tier B]
void nn::pia::transport::PacketStream::Reader::PullAll()
{
    m_Position = m_pStream->m_Writer.m_Head;
    m_Count = m_Position - m_Head;
    if (m_Count < 0) {
        m_Count += m_pStream->m_PacketNum;
    }
}

// 0x0044DBA8 | fefates:bytes [tier B]
common::Packet* nn::pia::transport::PacketStream::Reader::PullOne()
{
    PacketStream* pStream = m_pStream;
    if (m_Position == pStream->m_Writer.m_Head) {
        return nullptr;
    }
    s32 index = m_Position;
    m_Position++;
    if (static_cast<s32>(pStream->m_PacketNum) <= m_Position) {
        m_Position = 0;
    }
    m_Count++;
    return pStream->GetPacket(index);
}

// 0x0044DC18 | fefates:bytes [tier B]
void nn::pia::transport::PacketStream::Reader::Release()
{
    m_Head = m_Position;
    m_Count = 0;
}

// 0x0044DC2C | fefates:bytes [tier B]
void nn::pia::transport::PacketStream::Writer::Push()
{
    m_Head = m_Position;
    m_Count = 0;
}

// 0x0044DC40 | fefates:bytes [tier B]
common::Packet* nn::pia::transport::PacketStream::Writer::Assign()
{
    PacketStream* pStream = m_pStream;
    s32 free = pStream->m_Reader.m_Head - m_Position;
    if (pStream->m_Reader.m_Head <= m_Position) {
        free += pStream->m_PacketNum;
    }
    if (free - 1 <= 0) {
        return nullptr;
    }
    s32 index = m_Position;
    m_Position = static_cast<s32>(pStream->m_PacketNum) <= m_Position + 1 ? 0 : m_Position + 1;
    m_Count++;
    free = pStream->m_Reader.m_Head - m_Position;
    if (pStream->m_Reader.m_Head <= m_Position) {
        free += pStream->m_PacketNum;
    }
    s32 used = pStream->m_PacketNum - (free - 1);
    if (common::WatermarkManager::s_pInstance != nullptr) {
        common::WatermarkManager::s_pInstance->GetWatermark(pStream->m_WatermarkIndex)->Update(used);
    }
    return pStream->GetPacket(index);
}

// 0x0044DD14 | fefates:bytes [tier B]
void nn::pia::transport::PacketStream::Cleanup()
{
    m_Writer.m_Head = -1;
    m_Writer.m_Position = -1;
    m_Writer.m_Count = 0;
    m_Reader.m_Head = -1;
    m_Reader.m_Count = 0;
    m_Reader.m_Position = -1;
}

// 0x0044DD38 | fefates:bytes [tier B]
nn::Result nn::pia::transport::PacketStream::Startup()
{
    if (m_pPackets == nullptr) {
        return common::RESULT_NOT_INITIALIZED;
    }
    m_Writer.m_Head = 0;
    m_Writer.m_Position = 0;
    m_Writer.m_Count = 0;
    m_Reader.m_Head = 0;
    m_Reader.m_Position = 0;
    m_Reader.m_Count = 0;
    return nn::Result();
}

// 0x0044DD70 | fefates:bytes [tier B]
common::Packet* nn::pia::transport::PacketStream::Accessor::Get(int index)
{
    u32 position = index + m_Head;
    PacketStream* pStream = m_pStream;
    if (pStream->m_PacketNum <= position) {
        position -= pStream->m_PacketNum;
    }
    return pStream->GetPacket(position);
}

// 0x0044DDAC | fefates:bytes [tier B]
void nn::pia::transport::PacketStream::Finalize()
{
    common::Packet* pPackets = m_pPackets;
    if (pPackets != nullptr) {
        // the number of packets from the size of the memory block
        u32 packetNum = pead::GetMemoryBlockInfo(pPackets) / sizeof(common::Packet);
        for (u32 i = 0; i < packetNum; i++) {
            pPackets[i].~Packet();
        }
        pead::FreeMemory(pPackets);
        m_pPackets = nullptr;
        m_PacketNum = 0;
    }
    m_Writer.m_Head = -1;
    m_Writer.m_Position = -1;
    m_Writer.m_Count = 0;
    m_Reader.m_Head = -1;
    m_Reader.m_Count = 0;
    m_Reader.m_Position = -1;
}

// 0x0044DE40 | fefates:bytes [tier B]
nn::pia::transport::PacketStream::PacketStream()
    : m_PacketNum(0), m_pPackets(nullptr), m_Writer(this), m_Reader(this)
{
}

} // namespace transport
} // namespace pia
} // namespace nn
