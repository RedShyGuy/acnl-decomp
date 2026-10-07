#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_StationAddress.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/pia_Types.h"
#include "nn/pia/transport/transport_Protocol.h"

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport24BandwidthCheckerProtocolE @ 0x008D0284
// vtable 0x00901FE4 (vptr 0x00901FEC), offset_to_top 0, 9 entries
//
// Measures the bandwidth to a station: the sender asks (REQUEST) until the station accepts
// (ACCEPT), then sends DATA packets at 130 % of the bandwidth for the duration; the receiver
// answers with the received bits per second and packets (RESULT), from which the sender takes the
// packet loss. Afterwards the receiver measures the other direction unless m_IsOneWay. The layout
// is from the constructor and Initialize; the member names and the names marked so are ours.
class BandwidthCheckerProtocol : public ::nn::pia::transport::Protocol
{
public:
    // the first word of the messages (big endian, like the three after it)
    enum MessageType
    {
        MESSAGE_TYPE_DATA = 1,
        MESSAGE_TYPE_RESULT = 2,
        MESSAGE_TYPE_REQUEST = 3,
        MESSAGE_TYPE_ACCEPT = 4,
    };
    // m_SendState
    enum SendState : u8
    {
        SEND_STATE_NONE = 0,
        SEND_STATE_REQUESTING = 1,
        SEND_STATE_SENDING = 2,
        SEND_STATE_WAITING_RESULT = 3,
        SEND_STATE_SUCCEEDED = 4,
        SEND_STATE_FAILED = 5,
    };
    // m_ReceiveState
    enum ReceiveState : u8
    {
        RECEIVE_STATE_NONE = 0,
        RECEIVE_STATE_ACCEPTING = 1,
        RECEIVE_STATE_RECEIVING = 2,
        RECEIVE_STATE_SENDING_RESULT = 3,
    };

    // a message (4 big endian words; name is ours)
    struct Message
    {
        u32 m_Type;
        u32 m_Value0;
        u32 m_Value1;
        u32 m_Value2;
    };

    BandwidthCheckerProtocol(); // 0x0045E360
    virtual ~BandwidthCheckerProtocol(); // 0x0045E418 slot 0x00
    // 0x0045E3E8 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x007367B4 slot 0x08
    virtual u16 GetProtocolType() const; // 0x007367A4 slot 0x0C
    virtual nn::Result Dispatch(); // 0x0045E23C slot 0x18
    virtual bool IsEnableProtocolFiltering() const; // 0x007367AC slot 0x20

    // (names are ours)
    nn::Result Initialize(); // 0x0045D474
    void Finalize(); // 0x0045E284
    // bandwidth: bits per second (1..1000000), packetSize: the size of the DATA packets (a multiple
    // of 4), durationMSec: how long DATA is sent
    nn::Result Setup(int bandwidth, unsigned int packetSize, bool isOneWay, int durationMSec); // 0x0045E178
    // starts a measurement to the station
    nn::Result StartCheck(nn::pia::StationIndex stationIndex); // 0x0045E2C8
    // stops the measurement of the sender (it fails)
    void Cancel(); // 0x0045D458
    // the last check is done (it succeeded or failed; name is ours)
    bool IsCheckDone() const; // 0x0073677C
    // a check can start (name is ours)
    bool IsCheckStartable() const; // 0x007367B8

    // (names are ours)
    void DispatchSend(); // 0x0045D5A0
    void DispatchReceive(); // 0x0045D9F8
    nn::Result SendMessage(nn::pia::StationIndex stationIndex, int type, int value0, int value1, int value2); // 0x0045DCE4
    void UpdateSendState(); // 0x0045DDC8
    void UpdateReceiveState(); // 0x0045DF54

    nn::Result Send(StationIndex stationIndex, const void* pData, u32 size);
    void ResetSend()
    {
        m_SendState = SEND_STATE_NONE;
        m_IsAccepted = false;
        m_Result = -1;
        m_SentNum = 0;
        m_PacketLoss = -1;
        m_IsCancelRequested = false;
        m_SendStartTime = common::Time();
        m_RequestTime = common::Time();
    }
    void ResetReceive()
    {
        m_SenderStationIndex = STATION_INDEX_UNIDENTIFIED;
        m_ReceiveState = RECEIVE_STATE_NONE;
        m_AcceptTime = common::Time();
        m_ReceiveStartTime = common::Time();
        m_ResultTime = common::Time();
        m_ReceivedSize = 0;
        m_SenderAddress.Clear();
        m_ReceivedNum = 0;
        m_IsRequested = false;
        m_FirstSequenceNum = -1;
        m_AcceptMessageTime = common::Time();
        m_LastSequenceNum = -1;
    }

    bool m_IsInitialized;              // 0x14
    bool m_IsSetup;                    // 0x15
    bool m_IsOneWay;                   // 0x16
    u8* m_pBuffer;                     // 0x18, the DATA packet
    s32 m_Bandwidth;                   // 0x1C
    u32 m_PacketSize;                  // 0x20
    s32 m_TimeoutMSec;                 // 0x24
    s32 m_DurationMSec;                // 0x28
    // the sender
    SendState m_SendState;             // 0x2C
    common::Time m_RequestTime;        // 0x30
    bool m_IsAccepted;                 // 0x38
    common::Time m_SendStartTime;      // 0x40
    s32 m_Result;                      // 0x48, bits per second the receiver got (-1: none)
    StationIndex m_TargetStationIndex; // 0x4C
    u32 m_SentNum;                     // 0x50
    s32 m_PacketLoss;                  // 0x54, in 0.01 %
    bool m_IsCancelRequested;          // 0x58
    // the receiver
    ReceiveState m_ReceiveState;       // 0x59
    common::Time m_AcceptTime;         // 0x60
    common::Time m_ReceiveStartTime;   // 0x68
    common::Time m_ResultTime;         // 0x70
    s32 m_ReceivedSize;                // 0x78
    StationIndex m_SenderStationIndex; // 0x7C
    common::StationAddress m_SenderAddress; // 0x80
    u32 m_ReceivedNum;                 // 0x90
    s32 m_FirstSequenceNum;            // 0x94
    s32 m_LastSequenceNum;             // 0x98
    common::Time m_AcceptMessageTime;  // 0xA0
    bool m_IsRequested;                // 0xA8
};
ASSERT_OFFSET(BandwidthCheckerProtocol, m_IsInitialized, 0x14);
ASSERT_OFFSET(BandwidthCheckerProtocol, m_SendState, 0x2C);
ASSERT_OFFSET(BandwidthCheckerProtocol, m_ReceiveState, 0x59);
ASSERT_OFFSET(BandwidthCheckerProtocol, m_SenderAddress, 0x80);
ASSERT_SIZE(BandwidthCheckerProtocol, 0xB0);
} // namespace transport
} // namespace pia
} // namespace nn
