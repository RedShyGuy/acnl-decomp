#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_ObjList.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/common/common_SimpleContainer.h"
#include "nn/pia/pia_Types.h"
#include "nn/pia/transport/transport_Station.h"

namespace nn {
namespace pia {
namespace common {
class StationAddress;
}
namespace transport {
class NetworkFactory;

// RTTI N2nn3pia9transport14StationManagerE @ 0x008D0188
// vtable 0x00901DC8 (vptr 0x00901DD0), offset_to_top 0, 1 entries
//
// The stations of the session: all of them in one list (on the transport heap), the ones in use
// and the free ones as pointers, and the local one. Layout from the constructor and Initialize;
// the member names are ours.
class StationManager : public ::nn::pia::common::RootObject
{
public:
    static const u32 STATION_NUM_MAX = 12;
    typedef common::SimpleContainer<Station*, STATION_NUM_MAX> StationContainer;

    StationManager(); // 0x0044FDAC | fefates:bytes [tier B]
    // (inline: only Finalize)
    ~StationManager()
    {
        if (common::IsValidPointer(m_pBuffer)) {
            Finalize();
        }
    }
    virtual void Trace(u64 flag) const; // 0x007352E4 slot 0x00

    static nn::Result CreateInstance(); // 0x0044FA24 | fefates:callgraph [tier C]
    static void DestroyInstance(); // 0x0044FBD4 | fefates:bytes [tier B]

    // stationNum stations (at most 12) with the jobs of the factory
    nn::Result Initialize(nn::pia::transport::NetworkFactory* pFactory, unsigned int stationNum); // 0x0044F7BC | fefates:bytes [tier B]
    void Finalize(); // 0x0044FCE4 | fefates:bytes [tier B]

    // a free station in use now (null if there is none)
    Station* CreateStation(); // 0x0044F980 | fefates:bytes [tier B]
    // the local station, only once
    Station* CreateLocalStation(); // 0x0044FC28 | fefates:bytes [tier B]
    void DestroyStation(nn::pia::transport::Station* pStation); // 0x0044FA88 | fefates:bytes [tier B]

    // the stations in use; the index must be one of a station or 254
    Station* GetStation(nn::pia::StationIndex stationIndex); // 0x0044F690 | fefates:callgraph [tier C]
    Station* GetStation(nn::pia::StationIndex stationIndex) const; // 0x00735140 | fefates:callgraph [tier C]
    Station* GetStation(nn::pia::StationId stationId); // 0x0044F6E4 | fefates:bytes [tier B]
    Station* GetStation(const nn::pia::common::StationAddress& address); // 0x0044F754 | fefates:bytes [tier B]
    nn::Result GetStationAddress(nn::pia::common::StationAddress* pAddress, nn::pia::StationId stationId) const; // 0x00735194 | libgarden [tier A]
    nn::Result GetStationAddress(nn::pia::common::StationAddress* pAddress, nn::pia::StationIndex stationIndex) const; // 0x007351C0 | fefates:bytes [tier B]
    // the connected stations; without the local one unless includeLocal
    u32 GetParticipatingStationBitmap(bool includeLocal) const; // 0x00735270 | fefates:bytes [tier B]

    static StationManager* s_pInstance;

    common::ObjList<Station> m_StationList;   // 0x04
    void* m_pBuffer;                           // 0x30
    StationContainer m_ActiveStations;         // 0x34
    StationContainer m_FreeStations;           // 0x6C
    Station* m_pLocalStation;                  // 0xA4
    StationIndex m_Unknown0xA8;                // 0xA8
};
ASSERT_OFFSET(StationManager, m_pBuffer, 0x30);
ASSERT_OFFSET(StationManager, m_ActiveStations, 0x34);
ASSERT_OFFSET(StationManager, m_pLocalStation, 0xA4);
ASSERT_SIZE(StationManager, 0xAC);
} // namespace transport
} // namespace pia
} // namespace nn
