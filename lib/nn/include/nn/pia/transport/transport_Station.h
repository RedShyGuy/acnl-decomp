#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/common/common_StationAddress.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/pia_Types.h"
#include "nn/pia/transport/transport_SequenceIdController.h"

namespace nn {
namespace pia {
namespace transport {
class ConnectStationJob;
class DisconnectStationJob;
class NetworkFactory;
class ProcessConnectionRequestJob;
class ReliableSlidingWindow;
class StationProtocol;

// RTTI N2nn3pia9transport7StationE @ 0x008D02A8
// vtable 0x00902044 (vptr 0x0090204C), offset_to_top 0, 1 entries
//
// A station of the session (the local one too): its address, index and id, its state, the
// reliable data to it and the jobs that connect and disconnect it. StationManager holds them.
// Layout from the constructor and helperStartupCleanup; the member names, the enumerators and
// SetStationId are ours. Its destructor is not virtual (the vtable only has Trace).
class Station : public ::nn::pia::common::RootObject
{
public:
    // the name of the player (copied with memcpy, so not word aligned; the layout is not known)
    struct PlayerName
    {
        u8 m_Data[0x20]; // 0x00
    };
    // the identification of a station (IdentificationInfoTable); only the name is known
    struct IdentificationInfo
    {
        PlayerName m_PlayerName; // 0x00
        u16 m_Unknown0x20[16];   // 0x20
        u16 m_Unknown0x40;       // 0x40
        u8 m_Unknown0x42;        // 0x42
        u8 m_Unknown0x43;        // 0x43
        u8 m_Unknown0x44;        // 0x44
        u32 m_Unknown0x48;       // 0x48
    };

    // the type name is from the signature of helperStartupCleanup
    enum StationState : u8
    {
        STATION_STATE_NONE = 0,      // not started (Cleanup)
        STATION_STATE_STARTED = 1,   // started (Startup)
        STATION_STATE_2 = 2,          // (StationProtocol checks its ConnectStationJob then)
        STATION_STATE_REQUESTED = 3,  // it asked for the connection (StationProtocol; name is ours)
        STATION_STATE_CONNECTING = 4, // the jobs connect it (name is ours)
        STATION_STATE_CONNECTED = 5, // the state of the stations in the participating bitmap
        STATION_STATE_DISCONNECTED = 6, // it left, was replaced or could not connect (name is ours)
    };

    // the route of the connection (0x52; name is ours)
    enum ConnectionRoute : u8
    {
        CONNECTION_ROUTE_RELAY = 0,
        CONNECTION_ROUTE_DIRECT = 1,
    };

    Station(); // 0x0045F7E4 | fefates:callgraph [tier C]
    ~Station(); // 0x0045F894 | fefates:bytes [tier B]
    virtual void Trace(u64 flag) const; // 0x00736DC4 slot 0x00

    // the jobs from the factory and the buffers of the reliable data
    nn::Result Initialize(nn::pia::transport::NetworkFactory* pFactory); // 0x0045F338 | fefates:bytes [tier B]
    void Finalize(); // 0x0045F784 | fefates:bytes [tier B]
    bool Startup(nn::pia::transport::StationProtocol* pProtocol); // 0x0045F658 | fefates:bytes [tier B]
    bool Startup(nn::pia::transport::StationProtocol* pProtocol, nn::pia::StationIndex stationIndex, const nn::pia::common::StationAddress& address); // 0x0045F6D4 | fefates:bytes [tier B]
    bool Startup(nn::pia::transport::StationProtocol* pProtocol, const nn::pia::common::StationAddress& address); // 0x0045F740 | fefates:bytes [tier B]
    void Cleanup(); // 0x0045F5FC | fefates:bytes [tier B]
    void CleanupJobs(); // 0x0045F3A0 | fefates:bytes [tier B]
    DECOMP_NOINLINE void helperStartupCleanup(bool isStartup, nn::pia::transport::StationProtocol* pProtocol, nn::pia::StationIndex stationIndex, const nn::pia::common::StationAddress& address, nn::pia::transport::Station::StationState state); // 0x0045F510 | fefates:bytes [tier B]

    void SetStationId(StationId stationId); // 0x0045F430 (name is ours)
    nn::Result GetPrincipalId(unsigned int* pPrincipalId); // 0x0045F43C | fefates:bytes [tier B]
    nn::Result GetPlayerName(nn::pia::transport::Station::PlayerName* pName); // 0x0045F5C0 | fefates:bytes [tier B]
    bool IsConnectionRouteRelay() const; // 0x00736D90 | fefates:bytes [tier B]
    bool IsConnectionRouteDirect() const; // 0x00736DAC | fefates:bytes [tier B]
    // the round trip time (the median of the latest num), -1 without one
    s32 GetRtt(unsigned int num) const; // 0x00736DC8 | fefates:bytes [tier B]
    s32 GetRtt() const; // 0x00736E24 | fefates:bytes [tier B]
    const SequenceIdController* GetSequenceIdController() const { return &m_SequenceIdController; }

    common::StationAddress m_StationAddress;                      // 0x04
    StationIndex m_StationIndex;                                  // 0x14
    StationId m_StationId;                                        // 0x18
    StationState m_State;                                         // 0x20
    ReliableSlidingWindow* m_pReliableSlidingWindow;              // 0x24
    StationProtocol* m_pStationProtocol;                          // 0x28
    ConnectStationJob* m_pConnectStationJob;                      // 0x2C
    DisconnectStationJob* m_pDisconnectStationJob;                // 0x30
    ProcessConnectionRequestJob* m_pProcessConnectionRequestJob;  // 0x34
    SequenceIdController m_SequenceIdController;                  // 0x38
    u8 m_LocalConnectionId;  // 0x50, sent in byte 5 of the packets to it (random 2..255, StationProtocol)
    u8 m_RemoteConnectionId; // 0x51, expected in byte 5 of its packets (from its connection request)
    ConnectionRoute m_ConnectionRoute;                            // 0x52
    common::Time m_LastSendTime;                                  // 0x58 (KeepAliveSender)
    common::Time m_LastReceiveTime;                               // 0x60 (KeepAliveReceiver)
    bool m_Unknown0x68;                                           // 0x68
    bool m_Unknown0x69;                                           // 0x69
};
ASSERT_OFFSET(Station, m_StationIndex, 0x14);
ASSERT_OFFSET(Station, m_State, 0x20);
ASSERT_OFFSET(Station, m_SequenceIdController, 0x38);
ASSERT_OFFSET(Station, m_LastSendTime, 0x58);
ASSERT_SIZE(Station, 0x70);
} // namespace transport
} // namespace pia
} // namespace nn
