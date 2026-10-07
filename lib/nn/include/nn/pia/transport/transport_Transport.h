#pragma once

#include "decomp.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_Job.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/transport/transport_ProtocolManager.h"
#include "nn/pia/transport/transport_StationIdTable.h"
#include "nn/pia/pia_Types.h"
#include "nn/Result.h"

namespace nn {
namespace pia {
namespace common {
class CryptoSetting;
class IPacketInput;
class IPacketOutput;
class StationAddress;
} // namespace common
namespace transport {
class NetworkFactory;
class RelayRouteManager;
class StationPacketHandler;
// RTTI N2nn3pia9transport9TransportE @ 0x008D02CC
// vtable 0x00902094 (vptr 0x0090209C), offset_to_top 0, 1 entries
class Transport : public ::nn::pia::common::RootObject
{
public:
    // RTTI N2nn3pia9transport9Transport11DispatchJobE @ 0x008D02C0
    // vtable 0x0090207C (vptr 0x00902084), offset_to_top 0, 4 entries
    //
    // The job that dispatches the transport in every Scheduler::Dispatch.
    class DispatchJob : public ::nn::pia::common::Job
    {
    public:
        // (inline in the constructor of Transport)
        DispatchJob() {}
        virtual ~DispatchJob(); // 0x00428BA4 slot 0x00
        // 0x0045FD94 slot 0x04 (deleting dtor)
        virtual nn::pia::common::ExecuteResult ExecuteCore(); // 0x0045FD6C slot 0x0C | fefates:bytes
    };

    // the argument of CreateInstance (the member names and the constructor name are ours)
    struct Setting
    {
        Setting(); // 0x00460314

        NetworkFactory* m_pNetworkFactory; // 0x00
        u32 m_StationNumMax;               // 0x04, 1..12
        u32 m_SendPacketNum;               // 0x08 (ThreadStreamManager)
        u32 m_ReceivePacketNum;            // 0x0C
        s32 m_AnalysisIntervalSec;         // 0x10, the analysis is printed this often (0: never)
    };

    // (inline in CreateInstance)
    Transport(); // 0x004606B0 | fefates:bytes [tier B]
    // (inline in CreateInstance and DestroyInstance)
    ~Transport();
    virtual void Trace(u64 flag) const; // 0x007370E8 slot 0x00

    // (name is ours)
    static nn::Result CreateInstance(const Setting& setting); // 0x0045FDA4
    static void DestroyInstance(); // 0x0045FF08 | fefates:bytes [tier B]
    nn::Result initialize(const nn::pia::transport::Transport::Setting& setting); // 0x0045FA5C | fefates:bytes-fuzzy [tier B]
    void finalize(); // 0x004605AC | fefates:bytes [tier B]
    nn::Result Startup(const nn::pia::common::StationAddress* pRelayNodeAddress, const nn::pia::common::CryptoSetting* pCryptoSetting); // 0x00460330 | fefates:bytes [tier B]
    void Cleanup(); // 0x00460254 | fefates:bytes [tier B]
    // the work of every dispatch (DispatchJob)
    nn::Result dispatch(); // 0x00460408 | fefates:callseq [tier C]

    common::IPacketInput* GetInputStream(); // 0x0045FEE0 | fefates:bytes [tier B]
    common::IPacketOutput* GetOutputStream(); // 0x00460000 | fefates:bytes [tier B]
    void OutputStreamUpdateEvent(); // 0x00460060 | fefates:bytes [tier B]
    // the least and the most round trip time to the stations into the monitoring data (of the
    // session begin or the session state)
    void SetMonitoringNetworkRtt(bool isSessionBegin); // 0x0046009C | fefates:callseq [tier C]
    // (names are ours)
    void EnableKeepAlive(); // 0x0045FFEC
    // the interval of the keep alive packets (name is ours)
    nn::Result SetKeepAliveInterval(int intervalMSec); // 0x00450048
    s32 GetKeepAliveIntervalMSec() const; // 0x00736FAC

    nn::Result ConvertToStationId(nn::pia::StationId* pStationId, nn::pia::StationIndex stationIndex) const; // 0x00736E84 | fefates:callseq [tier C]
    nn::Result ConvertToStationIndex(nn::pia::StationIndex* pStationIndex, nn::pia::StationId stationId) const; // 0x00736FB8 | fefates:callseq [tier C]

    // only a success, INVALID_STATE or INVALID_STATE_TEMPORARY
    nn::Result SetState(nn::Result state); // 0x00460028 (name is ours)

    // the time of the current dispatch (name is ours)
    const common::Time& GetDispatchTime() const { return m_DispatchTime; }

    // the time of the current dispatch, or the system time without an instance (inline
    // everywhere; name is ours)
    static common::Time GetCurrentTime()
    {
        if (common::IsValidPointer(s_pInstance)) {
            return s_pInstance->m_DispatchTime;
        }
        common::Time time;
        time.SetNow();
        return time;
    }

    // the only instance (name is ours)
    static Transport* s_pInstance;

    u32 m_Unknown0x4;                        // 0x04 (not set here)
    common::Time m_DispatchTime;             // 0x08
    ProtocolManager m_ProtocolManager;       // 0x10
    StationPacketHandler* m_pPacketHandler;  // 0x2C
    u32 m_StationNum;                        // 0x30
    bool m_IsStarted;                        // 0x34
    nn::Result m_StreamResult;               // 0x38, the error of a stream thread
    DispatchJob m_DispatchJob;               // 0x40
    u32 m_Unknown0x60;                       // 0x60
    RelayRouteManager* m_pRelayRouteManager; // 0x64
    s32 m_AnalysisIntervalSec;               // 0x68
    common::Time m_AnalysisTime;             // 0x70, of the last printed analysis
    // asked by inet::NexDisconnectStationJob for the station id of a station that left; 1: the
    // station is not gone for good (type and name are ours)
    s32 (*m_pStationIdCallback)(const StationIdTable::Entry* pEntry); // 0x78
    StationIdTable* m_pStationIdTable;       // 0x7C
    bool m_IsUsingStationIdTable;            // 0x80
    // INVALID_STATE before Startup (name is ours)
    nn::Result m_State;                      // 0x84
};
ASSERT_OFFSET(Transport, m_DispatchJob, 0x40);
ASSERT_OFFSET(Transport, m_pRelayRouteManager, 0x64);
ASSERT_SIZE(Transport, 0x88);
} // namespace transport
} // namespace pia
} // namespace nn
