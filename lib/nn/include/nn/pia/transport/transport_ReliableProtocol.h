#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/pia_Types.h"
#include "nn/pia/transport/transport_Protocol.h"

namespace nn {
namespace pia {
namespace transport {
class ProtocolEvent;
class ReliableSlidingWindow;

// RTTI N2nn3pia9transport16ReliableProtocolE @ 0x008D01C4
// vtable 0x00901E38 (vptr 0x00901E40), offset_to_top 0, 9 entries
//
// Reliable messages to the other stations of the session: one ReliableSlidingWindow per station
// (without the local one: the windows of the indices above it are one lower). Layout from the
// constructor and Initialize; the member names and the names marked so are ours.
class ReliableProtocol : public ::nn::pia::transport::Protocol
{
public:
    ReliableProtocol(); // 0x00452FC4 | fefates:bytes [tier B]
    virtual ~ReliableProtocol(); // 0x0045311C slot 0x00
    // 0x00453084 slot 0x04 | fefates:bytes (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x007356D0 slot 0x08
    virtual u16 GetProtocolType() const; // 0x00735668 slot 0x0C
    virtual nn::Result Startup(nn::pia::StationIndex localStationIndex); // 0x00452CEC slot 0x10 | fefates:bytes-fuzzy
    virtual void Cleanup(); // 0x00452C18 slot 0x14 | fefates:bytes
    virtual nn::Result Dispatch(); // 0x00452D4C slot 0x18 | fefates:callseq
    virtual nn::Result UpdateProtocolEvent(const nn::pia::transport::ProtocolEvent& event); // 0x00452924 slot 0x1C | fefates:bytes

    // sendNum, receiveNum: the window sizes
    nn::Result Initialize(unsigned int sendNum, unsigned int receiveNum); // 0x00452664 | fefates:bytes [tier B]
    void Finalize(); // 0x00452F48 | fefates:bytes [tier B]

    // stationIndex 255: to all stations (only if all of them have room)
    nn::Result Send(nn::pia::StationIndex stationIndex, const void* pData, unsigned int size); // 0x004529C0 | fefates:callseq [tier C]
    nn::Result Send(nn::pia::StationId stationId, const void* pData, unsigned int size); // 0x00452BCC | fefates:bytes [tier B]
    nn::Result Receive(nn::pia::StationId* pStationId, void* pBuffer, unsigned int* pSize, unsigned int bufferSize); // 0x00452C84 | fefates:bytes [tier B]
    // the next message of any station; withStationId: only from the stations that have an id
    nn::Result ReceiveImpl(nn::pia::StationIndex* pStationIndex, void* pBuffer, unsigned int* pSize, unsigned int bufferSize, bool withStationId); // 0x004527C0 | fefates:bytes [tier B]
    // (name is ours)
    bool IsInCommunication(nn::pia::StationId stationId) const; // 0x00735670

    // the window of the station (inline everywhere; name is ours)
    ReliableSlidingWindow* GetWindow(StationIndex stationIndex) const;

    StationIndex m_LocalStationIndex;     // 0x14, UNIDENTIFIED before Startup
    u32 m_StationNum;                     // 0x18, Transport::m_StationNum (the windows are one less)
    ReliableSlidingWindow* m_pWindows;    // 0x1C
    u32 m_DispatchIndex;                  // 0x20, the window that Dispatch begins with (round robin)
};
ASSERT_SIZE(ReliableProtocol, 0x24);
} // namespace transport
} // namespace pia
} // namespace nn
