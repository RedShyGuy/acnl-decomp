#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/pia_Types.h"
#include "nn/pia/transport/transport_Protocol.h"

namespace nn {
namespace pia {
namespace common {
class StationAddress;
} // namespace common
namespace transport {
class ProtocolEvent;
struct ReceivedMessageAccessor;
class ReliableSlidingWindow;
class Station;
} // namespace transport
namespace session {
class JoinMeshJob;
class LeaveMeshJob;
class ProcessJoinRequestJob;

// RTTI N2nn3pia7session12MeshProtocolE @ 0x008CFF54
// vtable 0x00901680 (vptr 0x00901688), offset_to_top 0, 9 entries
//
// The protocol of the mesh: join, leave and destroy, the station data list of the host, the kickout,
// the host migration and the relay routes. Some messages go unreliably (resent by
// ResendingMessageManager or twice), the others through one ReliableSlidingWindow per other station
// (protocol port 1). The message types are from the code; their names, the layout names and the
// names marked so are ours.
class MeshProtocol : public ::nn::pia::transport::Protocol
{
public:
    // byte 0 of the messages
    enum MessageType : u8
    {
        MESSAGE_TYPE_JOIN_REQUEST = 1,
        MESSAGE_TYPE_JOIN_RESPONSE = 2,
        MESSAGE_TYPE_LEAVE_REQUEST = 4,
        MESSAGE_TYPE_LEAVE_RESPONSE = 8,
        MESSAGE_TYPE_DESTROY_MESH = 16,
        MESSAGE_TYPE_DESTROY_RESPONSE = 17,
        MESSAGE_TYPE_STATION_DATA_LIST = 32,
        MESSAGE_TYPE_KICKOUT_NOTICE = 33,
        MESSAGE_TYPE_CONNECTION_CHECK = 34,
        MESSAGE_TYPE_CONNECTION_CHECK_RESPONSE = 35,
        MESSAGE_TYPE_CONNECTION_FAILURE_NOTICE = 36,
        MESSAGE_TYPE_GREETING = 64,
        MESSAGE_TYPE_MIGRATION_FINISH = 65,
        MESSAGE_TYPE_GREETING_RESPONSE = 66,
        MESSAGE_TYPE_MIGRATION_REQUEST = 68,
        MESSAGE_TYPE_MIGRATION_RESPONSE = 72,
        MESSAGE_TYPE_MULTI_MIGRATION_RANKING = 73,
        MESSAGE_TYPE_MULTI_MIGRATION_RANK_DECISION = 74,
        MESSAGE_TYPE_CONNECTION_REPORT = 128,
        MESSAGE_TYPE_RELAY_ROUTE_DIRECTIONS = 129,
    };

    // the watermarks of the reliable windows (the names are from the binary)
    static const int WATERMARK_SEND_BUFFER = 7;
    static const int WATERMARK_RECEIVE_BUFFER = 8;

    MeshProtocol(); // 0x00430918
    virtual ~MeshProtocol(); // 0x0045FA54 slot 0x00
    // 0x00430A24 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x00733868 slot 0x08
    virtual u16 GetProtocolType() const; // 0x0073383C slot 0x0C
    virtual nn::Result Startup(nn::pia::StationIndex localStationIndex); // 0x0045FA14 slot 0x10
    virtual nn::Result Dispatch(); // 0x0042FB30 slot 0x18
    virtual nn::Result UpdateProtocolEvent(const nn::pia::transport::ProtocolEvent& event); // 0x0042E8F8 slot 0x1C | fefates:callseq

    // the reliable windows and the buffers (names are ours)
    nn::Result Initialize(); // 0x0042CB00
    void Finalize(); // 0x00430830
    // the jobs are forgotten
    nn::Result Startup(); // 0x0042FB10 | fefates:callgraph [tier C]

    void ParseHelper(const nn::pia::transport::ReceivedMessageAccessor& accessor); // 0x0042CE28 | fefates:bytes-fuzzy [tier B]
    void ParseJoinRequest(const nn::pia::transport::ReceivedMessageAccessor& accessor); // 0x0042DEFC | fefates:bytes [tier B]
    // (names are ours)
    void ParseLeaveRequest(const nn::pia::transport::ReceivedMessageAccessor& accessor); // 0x0042E214
    void ParseStationDataList(const nn::pia::transport::ReceivedMessageAccessor& accessor); // 0x0042DC40
    void ParseConnectionFailureNotice(const nn::pia::transport::ReceivedMessageAccessor& accessor); // 0x0042F750
    void ParseMultiMigrationRankDecision(const nn::pia::transport::ReceivedMessageAccessor& accessor); // 0x0042F9D4

    void MakeJoinRequestData(unsigned char* pData); // 0x0042E648 | fefates:bytes [tier B]
    u32 GetJoinRequestDataSize() const; // 0x00733844 | fefates:bytes [tier B]
    // the join response in up to three parts; the number of parts (0 if it does not fit)
    u32 MakeJoinResponseData(nn::pia::StationIndex stationIndex, unsigned char** ppData, unsigned int* pSizes); // 0x0042E9C8 | fefates:callgraph

    bool SendGreeting(nn::pia::StationIndex stationIndex); // 0x0042D7C4 | fefates:bytes-fuzzy [tier B]
    bool SendDestroyMesh(nn::pia::StationIndex stationIndex); // 0x0042DDB8 | fefates:bytes-fuzzy [tier B]
    void SendLeaveRequest(); // 0x0042E0E8
    void SendLeaveResponse(nn::pia::transport::Station* pStation); // 0x0042E3EC | fefates:bytes-fuzzy [tier B]
    bool SendKickoutNotice(nn::pia::StationIndex stationIndex, unsigned char reason); // 0x0042E330
    bool SendMigrationFinish(bool value); // 0x0042E7FC | fefates:bytes-fuzzy [tier B]
    bool SendConnectionReport(nn::pia::StationIndex stationIndex, unsigned int value1, unsigned int value2, unsigned int* pNoRouteBitmap); // 0x0042ED90 | fefates:bytes-fuzzy [tier B]
    bool SendRelayRouteDirections(); // 0x0042F518 | fefates:bytes-fuzzy [tier B]
    void SendConnectionFailureNotice(nn::pia::StationIndex destination, nn::pia::StationIndex stationIndex1, nn::pia::StationIndex stationIndex2, unsigned char reason, unsigned int version); // 0x0042F68C | fefates:bytes-fuzzy [tier B]
    bool SendMultiMigrationRankDecision(nn::pia::StationIndex stationIndex, long long timeout, unsigned int* pAckId); // 0x0042F89C | fefates:bytes-fuzzy [tier B]
    // (names are ours)
    void SendStationDataList(bool isNew); // 0x0042D898
    bool SendConnectionCheck(nn::pia::StationIndex stationIndex); // 0x0042E050
    bool SendMigrationRequest(nn::pia::StationIndex stationIndex, nn::pia::StationIndex newHostStationIndex); // 0x0042E544
    bool SendDestroyResponse(nn::pia::StationIndex stationIndex); // 0x0042E6AC
    bool SendGreetingResponse(nn::pia::StationIndex stationIndex); // 0x0042F004
    bool SendMigrationResponse(nn::pia::StationIndex stationIndex); // 0x0042F104
    void SendJoinRejection(const nn::pia::common::StationAddress& address, unsigned char reason); // 0x0042F254
    bool SendMultiMigrationRanking(bool* pIsSent); // 0x0042F384

    // (inline; names are ours)
    // the window of another station (the local one has none)
    transport::ReliableSlidingWindow* GetReliableSlidingWindow(StationIndex stationIndex, StationIndex localStationIndex) const;
    nn::Result PushData(transport::ReliableSlidingWindow* pWindow, const void* pData, u32 size);

    ProcessJoinRequestJob* m_pProcessJoinRequestJob; // 0x14, waits for join requests (ParseJoinRequest)
    JoinMeshJob* m_pJoinMeshJob;               // 0x18, waits for the join response
    u32 m_Unknown0x1C;                         // 0x1C
    LeaveMeshJob* m_pLeaveMeshJob;             // 0x20, waits for the leave response
    s32 m_KeepAliveTimeoutMSec;                // 0x24 (10000), also spreads the sends of the list
    s32 m_StationDataListIntervalMSec;         // 0x28 (10000), the list is resent this often
    common::Time m_Unknown0x30;                // 0x30
    bool m_IsStationDataListPending;           // 0x38, a new list after the host migration
    u32 m_ReliableSlidingWindowNum;            // 0x3C, one less than the stations
    transport::ReliableSlidingWindow* m_pReliableSlidingWindows; // 0x40
    u32 m_Unknown0x44;                         // 0x44
    u32 m_BufferSize;                          // 0x48
    u8* m_pBuffer;                             // 0x4C, of the messages
    u32 m_StationDataListSize;                 // 0x50
    u8* m_pStationDataList;                    // 0x54
    bool m_IsStationDataListValid;             // 0x58
    u32 m_NextSendTimeNum;                     // 0x5C
    common::Time* m_pNextSendTimes;            // 0x60, of the station data list, per window
};
ASSERT_OFFSET(MeshProtocol, m_pProcessJoinRequestJob, 0x14);
ASSERT_OFFSET(MeshProtocol, m_Unknown0x30, 0x30);
ASSERT_OFFSET(MeshProtocol, m_ReliableSlidingWindowNum, 0x3C);
ASSERT_OFFSET(MeshProtocol, m_pNextSendTimes, 0x60);
ASSERT_SIZE(MeshProtocol, 0x68);
} // namespace session
} // namespace pia
} // namespace nn
