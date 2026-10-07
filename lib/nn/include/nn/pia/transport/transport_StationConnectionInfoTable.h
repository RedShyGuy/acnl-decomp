#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CriticalSection.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/common/common_StationAddress.h"
#include "nn/pia/pia_Types.h"
#include "nn/pia/transport/transport_StationConnectionInfo.h"

namespace nn {
namespace pia {
namespace transport {
class Station;
class StationLocation;

// RTTI N2nn3pia9transport26StationConnectionInfoTableE @ 0x008D0290
// vtable 0x00902010 (vptr 0x00902018), offset_to_top 0, 3 entries
//
// The connection info of each station (and the one of the local station), locked by a critical
// section. Layout from CreateInstance and Initialize; the member names and the enumerator names
// are ours.
class StationConnectionInfoTable : public ::nn::pia::common::RootObject
{
public:
    static const u32 STATION_NUM_MAX = 12;

    // the type name is from the signature of GetStationPartialMatch
    enum PartialMatchMode
    {
        PARTIAL_MATCH_MODE_STATION_KEY = 0, // the station key and the extension id
        PARTIAL_MATCH_MODE_ADDRESS = 1,     // the internet address (without the port)
    };

    // (inline in CreateInstance)
    StationConnectionInfoTable() : m_pInfos(nullptr), m_StationNum(0), m_CriticalSection(-1) {}
    virtual ~StationConnectionInfoTable(); // 0x0045E848 slot 0x00
    // 0x0045E788 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x00736D08 slot 0x08

    static nn::Result CreateInstance(); // 0x0045E644 | fefates:bytes [tier B]
    static void DestroyInstance(); // 0x0045E758 | fefates:callgraph [tier C]
    nn::Result Initialize(unsigned int stationNum); // 0x0045E598 | fefates:bytes [tier B]

    nn::Result AddToTable(const nn::pia::transport::Station* pStation, const nn::pia::transport::StationConnectionInfo& info); // 0x0045E444 | fefates:bytes [tier B]
    void EraseFromTable(const nn::pia::transport::Station* pStation); // 0x0045E6E8 | fefates:bytes [tier B]
    void ClearTable(); // 0x0045E544 | fefates:bytes [tier B]

    // (the callers change the station: not const)
    Station* GetStation(const nn::pia::transport::StationConnectionInfo& info) const; // 0x007367EC | fefates:bytes [tier B]
    Station* GetStationPartialMatch(const nn::pia::transport::StationLocation& location, nn::pia::transport::StationConnectionInfoTable::PartialMatchMode mode) const; // 0x00736A4C | fefates:bytes [tier B]
    nn::Result GetStationKey(nn::pia::StationIndex stationIndex, unsigned int* pKey) const; // 0x007368B4 | fefates:bytes [tier B]
    nn::Result GetStationKey(nn::pia::transport::Station* pStation, unsigned int* pKey) const; // 0x00736974 | fefates:bytes [tier B]
    nn::Result GetLocalStationKey(unsigned int* pKey) const; // 0x00736A24 | fefates:bytes [tier B]
    u32 GetPrincipalIdByStation(const nn::pia::transport::Station* pStation) const; // 0x00736B34 | fefates:bytes [tier B]
    nn::Result GetStationConnectionInfo(const nn::pia::transport::Station* pStation, nn::pia::transport::StationConnectionInfo* pInfo) const; // 0x00736BC4 | fefates:bytes [tier B]
    // the index of the station with the principal id, STATION_INDEX_UNIDENTIFIED if there is none
    StationIndex GetStationIndexByPrincipalID(unsigned int principalId) const; // 0x00736C80 | fefates:bytes [tier B]

    static StationConnectionInfoTable* s_pInstance;

    StationConnectionInfo m_LocalInfo;                   // 0x004
    StationConnectionInfo m_HostInfo;                    // 0x058, the local one on the host (session::CreateMeshJob)
    common::StationAddress m_HostAddress;                // 0x0AC
    StationConnectionInfo* m_pInfos;                     // 0x0BC
    const Station* m_pStations[STATION_NUM_MAX];         // 0x0C0
    u32 m_StationNum;                                    // 0x0F0
    mutable common::CriticalSection m_CriticalSection;   // 0x0F4
};
ASSERT_OFFSET(StationConnectionInfoTable, m_pInfos, 0xBC);
ASSERT_OFFSET(StationConnectionInfoTable, m_StationNum, 0xF0);
ASSERT_SIZE(StationConnectionInfoTable, 0x100);
} // namespace transport
} // namespace pia
} // namespace nn
