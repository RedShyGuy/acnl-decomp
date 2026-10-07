#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CriticalSection.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/transport/transport_Station.h"
#include <string.h>

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport23IdentificationInfoTableE @ 0x008D0260
// vtable 0x00901F98 (vptr 0x00901FA0), offset_to_top 0, 3 entries
//
// The identification info of each station (and the local one), locked by a critical section.
// Layout from CreateInstance and Initialize; the member names are ours.
class IdentificationInfoTable : public ::nn::pia::common::RootObject
{
public:
    static const u32 STATION_NUM_MAX = 12;

    // (inline in CreateInstance)
    IdentificationInfoTable() : m_pInfos(nullptr), m_StationNum(0), m_CriticalSection(-1)
    {
        memset(&m_LocalInfo, 0, sizeof(m_LocalInfo));
    }
    virtual ~IdentificationInfoTable(); // 0x0045C670 slot 0x00
    // 0x0045C62C slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x007366BC slot 0x08

    static nn::Result CreateInstance(); // 0x0045C304 | fefates:bytes [tier B]
    static void DestroyInstance(); // 0x0045C410 | fefates:callgraph [tier C]
    nn::Result Initialize(unsigned int stationNum); // 0x0045C240 | fefates:bytes [tier B]

    nn::Result AddToTable(const nn::pia::transport::Station* pStation, const nn::pia::transport::Station::IdentificationInfo* pInfo); // 0x0045C0D0 | fefates:bytes [tier B]
    void EraseFromTable(const nn::pia::transport::Station* pStation); // 0x0045C3A0 | fefates:bytes [tier B]
    void ClearTable(); // 0x0045C1EC | fefates:bytes-fuzzy [tier B]

    nn::Result GetIdentificationInfo(const nn::pia::transport::Station* pStation, nn::pia::transport::Station::IdentificationInfo* pInfo); // 0x0045C440 | fefates:bytes [tier B]
    nn::Result GetPlayerName(const nn::pia::transport::Station* pStation, nn::pia::transport::Station::PlayerName* pName); // 0x0045C504 | fefates:bytes [tier B]
    nn::Result GetLocalIdentificationInfo(nn::pia::transport::Station::IdentificationInfo* pInfo); // 0x0045C5CC | fefates:bytes [tier B]
    void SetLocalIdentificationInfo(const nn::pia::transport::Station::IdentificationInfo* pInfo); // 0x0045C608 | fefates:bytes [tier B]

    static IdentificationInfoTable* s_pInstance;

    Station::IdentificationInfo m_LocalInfo;           // 0x04
    Station::IdentificationInfo* m_pInfos;             // 0x50
    const Station* m_pStations[STATION_NUM_MAX];       // 0x54
    u32 m_StationNum;                                  // 0x84
    common::CriticalSection m_CriticalSection;         // 0x88
};
ASSERT_OFFSET(IdentificationInfoTable, m_pInfos, 0x50);
ASSERT_OFFSET(IdentificationInfoTable, m_StationNum, 0x84);
ASSERT_SIZE(IdentificationInfoTable, 0x94);
} // namespace transport
} // namespace pia
} // namespace nn
