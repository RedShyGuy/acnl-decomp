#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_HeapManager.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/pia_Types.h"
#include "nn/pia/transport/transport_ProtocolId.h"
#include "pead/peadHeapMgr.h"
#include "pead/peadTickTime.h"
#include <new>

namespace nn {
namespace pia {
namespace transport {
class PacketHandler;
class ProtocolMessageReader;

// RTTI N2nn3pia9transport21ReliableSlidingWindowE @ 0x008D023C
// vtable 0x00901F48 (vptr 0x00901F50), offset_to_top 0, 3 entries
//
// Reliable data to one station (Station has one): the data is cut into numbered messages that
// are sent again until the station acknowledges them; the received ones are put back together
// in order. The layout is from the constructor and Initialize; the member names, the nested types
// and the names of their members are ours.
class ReliableSlidingWindow : public ::nn::pia::common::RootObject
{
public:
    // the header in front of the data of a message (24 bytes, big endian)
    struct MessageHeader
    {
        u16 m_Flags;          // 0x00, FLAG_*
        u16 m_DataSize;       // 0x02
        u32 m_Reserved;       // 0x04
        u32 m_SequenceId;     // 0x08
        u32 m_AckSequenceId;  // 0x0C, the next sequence id the sender waits for
        u32 m_AckBitmap[2];   // 0x10, the sequence ids after it that it has (64 bits)
    };

    static const u16 FLAG_DATA = 1 << 0; // the message has data
    static const u16 FLAG_LAST = 1 << 1; // the last message of the data
    static const u32 DATA_SIZE_MAX = 1400;

    // a message in the send buffer
    struct SendData
    {
        u64 m_ResendTime;        // 0x000, the time it is sent (again); all bits set: acknowledged
        u32 m_Size;              // 0x008, the message size (header and data)
        u16 m_ResendCount;       // 0x00C
        MessageHeader m_Header;  // 0x010
        u8 m_Data[DATA_SIZE_MAX]; // 0x028
    };

    // a message in the receive buffer
    struct ReceiveData
    {
        bool m_IsReceived;        // 0x000
        bool m_IsLast;            // 0x001
        u16 m_Size;               // 0x002
        u8 m_Data[DATA_SIZE_MAX]; // 0x004
    };

    // a buffer of num value initialized objects on the pia heap (inline everywhere)
    template <typename T>
    class Buffer
    {
    public:
        Buffer() : m_pBuffer(nullptr), m_Num(0) {}
        ~Buffer() { Finalize(); }

        nn::Result Initialize(u32 num)
        {
            if (m_pBuffer != nullptr) {
                return common::RESULT_ALREADY_INITIALIZED;
            }
            if (num == 0) {
                return common::RESULT_INVALID_ARGUMENT;
            }
            T* pBuffer = static_cast<T*>(pead::AllocMemory(num * sizeof(T), common::HeapManager::GetHeap()));
            if (pBuffer != nullptr) {
                for (u32 i = 0; i < num; i++) {
                    ::new (&pBuffer[i]) T();
                }
            }
            m_pBuffer = pBuffer;
            m_Num = num;
            return nn::Result();
        }
        void Finalize()
        {
            if (m_pBuffer != nullptr) {
                // the number of objects from the size of the memory block (their destructors are
                // trivial)
                pead::GetMemoryBlockInfo(m_pBuffer);
                pead::FreeMemory(m_pBuffer);
                m_pBuffer = nullptr;
            }
            m_Num = 0;
        }

        T* m_pBuffer; // 0x0
        u32 m_Num;    // 0x4
    };

    // the timing of the resending (name is ours); the callback, if there is one, gives the
    // interval in ms from the round trip time and the number of resends
    struct Setting
    {
        f32 m_RttFactor;   // 0x0, the interval is the round trip time times this
        u32 m_RttSampleNum; // 0x4, the round trip time is the median of that many
        s32 (*m_pResendIntervalCallback)(s32 rtt, u16 resendCount); // 0x8
    };
    static Setting s_Setting;

    ReliableSlidingWindow(); // 0x0045B3E0 | fefates:bytes [tier B]
    virtual ~ReliableSlidingWindow(); // 0x0045B45C slot 0x00 | fefates:bytes
    // 0x0045B44C slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x00736224 slot 0x08

    // the buffers for sendNum and receiveNum messages
    nn::Result Initialize(unsigned int sendNum, unsigned int receiveNum); // 0x0045A540 | fefates:bytes [tier B]
    void Finalize(); // 0x0045B14C | fefates:bytes [tier B]
    nn::Result Startup(nn::pia::transport::PacketHandler* pPacketHandler, unsigned int protocolId, nn::pia::StationIndex localStationIndex, nn::pia::StationIndex peerStationIndex); // 0x0045ACF4 | fefates:bytes [tier B]
    void Cleanup(); // 0x0045AB34 | fefates:bytes [tier B]

    bool CanPushData(unsigned int size); // 0x0045A6D8 | fefates:bytes [tier B]
    nn::Result PushData(const void* pData, unsigned int size); // 0x0045B1A8 | fefates:bytes [tier B]
    nn::Result PopData(void* pBuffer, unsigned int* pSize, unsigned int bufferSize); // 0x0045AB68 | fefates:callseq [tier C]
    // the acknowledgement and the data of a received message
    nn::Result AnalyzeProtocolMessage(const nn::pia::transport::ProtocolMessageReader& reader); // 0x0045A738 | fefates:callseq [tier C]
    // sends the messages that are due and the acknowledgement
    nn::Result Dispatch(nn::pia::transport::PacketHandler* pPacketHandler); // 0x0045ADE8 | fefates:bytes-fuzzy [tier B]
    bool IsInCommunication() const; // 0x00736204 | fefates:bytes [tier B]

    // the number of messages for size bytes of data
    u32 GetMessageNum(u32 size) const { return size != 0 ? (size - 1) / m_DataSizeMax + 1 : 1; }
    SendData& GetSendData(u32 index) const
    {
        u32 i = m_SendHead + index;
        if (m_SendBuffer.m_Num <= i) {
            i -= m_SendBuffer.m_Num;
        }
        return m_SendBuffer.m_pBuffer[i];
    }
    ReceiveData* GetReceiveData(u32 index) const
    {
        u32 i = m_ReceiveHead + index;
        if (m_ReceiveBuffer.m_Num <= i) {
            i -= m_ReceiveBuffer.m_Num;
        }
        return &m_ReceiveBuffer.m_pBuffer[i];
    }

    ProtocolId m_ProtocolId;              // 0x04
    StationIndex m_LocalStationIndex;     // 0x08
    StationIndex m_PeerStationIndex;      // 0x09
    Buffer<SendData> m_SendBuffer;        // 0x0C
    u32 m_SendHead;                       // 0x14
    u32 m_SendSequenceId;                 // 0x18, the sequence id of the message at the head
    u32 m_SendCount;                      // 0x1C
    Buffer<ReceiveData> m_ReceiveBuffer;  // 0x20
    u32 m_ReceiveHead;                    // 0x28
    u32 m_ReceiveSequenceId;              // 0x2C, the sequence id of the message at the head
    u32 m_ReceiveCount;                   // 0x30
    u32 m_DataSizeMax;                    // 0x34
    u64 m_Now;                            // 0x38, the ticks of the current dispatch
    u64 m_ResendInterval;                 // 0x40, ticks (500 ms) without a round trip time
    u32 m_AckSequenceId;                  // 0x48, the next sequence id this side waits for
    u64 m_AckBitmap;                      // 0x50
    bool m_IsAckRequired;                 // 0x58
    // the monitoring of a large transfer (100 KiB and more): its size and its start
    u32 m_MonitoredSize;                  // 0x5C
    common::Time m_MonitoringStartTime;   // 0x60
};
ASSERT_OFFSET(ReliableSlidingWindow, m_SendBuffer, 0x0C);
ASSERT_OFFSET(ReliableSlidingWindow, m_ReceiveBuffer, 0x20);
ASSERT_OFFSET(ReliableSlidingWindow, m_Now, 0x38);
ASSERT_OFFSET(ReliableSlidingWindow, m_AckBitmap, 0x50);
ASSERT_SIZE(ReliableSlidingWindow, 0x68);
} // namespace transport
} // namespace pia
} // namespace nn
