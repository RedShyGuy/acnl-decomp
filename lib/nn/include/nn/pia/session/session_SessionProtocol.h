#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_StationAddress.h"
#include "nn/pia/pia_Types.h"
#include "nn/pia/transport/transport_Protocol.h"
#include "nn/pia/transport/transport_ReliableSlidingWindow.h"

namespace nn {
namespace pia {
namespace common {
class SignatureSetting;
}
namespace session {
// RTTI N2nn3pia7session15SessionProtocolE @ 0x008CFFB4
// vtable 0x00901850 (vptr 0x00901858), offset_to_top 0, 9 entries
//
// The messages of the joint sessions between the stations (Session creates it when the network
// uses the station id table): a ReliableSlidingWindow per other station like
// transport::ReliableProtocol, a buffer for one message and handlers for the message types
// 3..24 that go to JointSessionJob, ConfigParticipationJobBase and StationIdStatusTable. The
// message layouts are from the send and receive functions; all names except the slots are
// ours.
class SessionProtocol : public ::nn::pia::transport::Protocol
{
public:
    static const u32 BUFFER_SIZE = 256;

    // a message that arrived (ParseMessage)
    struct ReceivedMessage
    {
        const u8* m_pData;              // 0x00
        u32 m_Size;                     // 0x04
        StationIndex m_StationIndex;    // 0x08, the sender
        common::StationAddress m_Address; // 0x0C, of the sender
        u32 m_Unknown0x1C;              // 0x1C
        u8 m_Unknown0x20;               // 0x20
    };

    SessionProtocol(); // 0x00436C64
    virtual ~SessionProtocol(); // 0x00436CA4 slot 0x00
    // 0x00436C94 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x007338E4 slot 0x08
    virtual u16 GetProtocolType() const; // 0x007338DC slot 0x0C
    virtual nn::Result Startup(nn::pia::StationIndex localStationIndex); // 0x0043687C slot 0x10
    virtual void Cleanup(); // 0x00436810 slot 0x14
    virtual nn::Result Dispatch(); // 0x004368CC slot 0x18
    virtual nn::Result UpdateProtocolEvent(const nn::pia::transport::ProtocolEvent& event); // 0x004350EC slot 0x1C

    nn::Result Initialize(u32 stationNum); // 0x0043425C
    void Finalize(); // 0x00436BF0

    // the received messages
    void ParseMessage(const ReceivedMessage& message); // 0x0043437C
    void ReceiveSessionInfo(const ReceivedMessage& message); // 0x00434CE0 (type 3)
    void ReceiveStationList8(const ReceivedMessage& message); // 0x00435208 (type 8)
    void ReceiveStationList10(const ReceivedMessage& message); // 0x00435470 (type 10)
    void ReceiveStationList6(const ReceivedMessage& message); // 0x00435654 (type 6)
    void ReceiveMessage14(const ReceivedMessage& message); // 0x004361F8 (type 14)
    void ReceiveMessage19(const ReceivedMessage& message); // 0x00436414 (type 19)
    void ReceiveMessage23(const ReceivedMessage& message); // 0x00436730 (type 23)
    // (ARMCC inlines it only in the station list loops of the receive functions)
    DECOMP_NOINLINE StationId DeserializeStationId(const u8* pData) const; // 0x004351BC

    // the sent messages
    nn::Result SendSessionInfo(u8 phase, u32 value1, u32 value2, u32 value3, const common::SignatureSetting* pSignatureSetting,
                               const StationId* pTargets, u32 targetNum); // 0x004349A0 (type 3)
    DECOMP_NOINLINE nn::Result SendStationList(u8 type, u8 phase, const StationId* pStationIds, u32 stationNum, const StationId* pStationId,
                               const StationId* pTargets, u32 targetNum); // 0x00434E6C
    nn::Result SendStationList8(u8 phase, const StationId* pStationIds, u32 stationNum, const StationId* pStationId, const StationId* pTargets,
                                u32 targetNum); // 0x004351D4
    nn::Result SendStationList10(u8 phase, const StationId* pStationIds, u32 stationNum, const StationId* pStationId, const StationId* pTargets,
                                 u32 targetNum); // 0x0043543C
    nn::Result SendStationList20(u8 phase, const StationId* pStationIds, u32 stationNum, const StationId* pStationId, const StationId* pTargets,
                                 u32 targetNum); // 0x004355EC
    nn::Result SendStationList6(u8 phase, const StationId* pStationIds, u32 stationNum, const StationId* pStationId, const StationId* pTargets,
                                u32 targetNum); // 0x00435620
    // type, phase, value and the local station id (11 bytes) to one station
    nn::Result SendMessage(u8 type, u8 phase, const StationId& stationId, u8 value); // 0x00435904
    nn::Result SendMessage7(u8 phase, const StationId& stationId, u8 value); // 0x00435420
    nn::Result SendMessage9(u8 phase, const StationId& stationId, u8 value); // 0x00435E00
    nn::Result SendMessage15(u32 sessionId, u8 value, const StationId* pTargets, u32 targetNum); // 0x004359DC
    nn::Result SendMessage12(u32 sessionId, u8 value1, const StationId* pTargets, u32 targetNum, u8 value2); // 0x00435BE0
    nn::Result SendMessage22(u8 phase, const StationId& stationId); // 0x00435E1C
    nn::Result SendMessage14(u32 sessionId, u8 value1, u8 value2, const StationId* pTargets, u32 targetNum); // 0x00435EEC
    nn::Result SendMessage4(u8 phase, const StationId& stationId, u32 sessionId, u8 value); // 0x004360FC
    nn::Result SendMessage19(u8 phase, const StationId& stationId, u8 value); // 0x0043633C
    nn::Result SendMessage13(const StationId& stationId, u8 value); // 0x004365A8
    nn::Result SendMessage23(const StationId& stationId, u8 value); // 0x00436668

    // the window of a station (the local station has none; inline)
    transport::ReliableSlidingWindow* GetWindow(StationIndex stationIndex) const
    {
        return &m_pWindows[m_LocalStationIndex <= stationIndex ? stationIndex - 1 : stationIndex];
    }
    // whether the window takes size more bytes: RESULT_NOT_INITIALIZED if it does not run (inline)
    DECOMP_ALWAYS_INLINE static nn::Result CheckWindow(transport::ReliableSlidingWindow* pWindow, u32 size);
    // the message in m_Buffer to the stations of the list except skipStationId (inline); with
    // isMarked the status table notes the stations
    DECOMP_ALWAYS_INLINE nn::Result SendToStations(const StationId* pTargets, u32 targetNum, const StationId& skipStationId, u32 size, bool isMarked);

    StationIndex m_LocalStationIndex;               // 0x14, UNIDENTIFIED before Startup
    u32 m_WindowNum;                                // 0x18, the stations but the local one
    transport::ReliableSlidingWindow* m_pWindows;   // 0x1C
    u32 m_DispatchIndex;                            // 0x20, the window that Dispatch begins with (round robin)
    u8 m_Buffer[BUFFER_SIZE];                       // 0x24, of the message to send or the one received
};
ASSERT_OFFSET(SessionProtocol, m_LocalStationIndex, 0x14);
ASSERT_OFFSET(SessionProtocol, m_Buffer, 0x24);
ASSERT_SIZE(SessionProtocol, 0x124);
} // namespace session
} // namespace pia
} // namespace nn
