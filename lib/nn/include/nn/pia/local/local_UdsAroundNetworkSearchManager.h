#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalAroundNetworkSearchManager.h"
#include "nn/pia/local/local_LocalNetworkTypes.h"
#include "nn/pia/local/local_UdsNetworkDescription.h"

namespace nn {
namespace pia {
namespace local {
// a network around with uds: its description and its stations (the name is ours, after
// LocalAroundNetworkInfo)
struct UdsAroundNetworkInfo : public ::nn::pia::local::LocalAroundNetworkInfo
{
    static const u32 STATION_INFO_NUM = 12;

    UdsAroundNetworkInfo() {}

    nn::pia::local::UdsNetworkDescription m_Description;                  // 0x004
    nn::pia::local::LocalStationInfo m_StationInfos[STATION_INFO_NUM];    // 0x110
};
ASSERT_SIZE(UdsAroundNetworkInfo, 0x2F0);

// RTTI N2nn3pia5local29UdsAroundNetworkSearchManagerE @ 0x008CFD48
// vtable 0x009012F0 (vptr 0x009012F8), offset_to_top 0, 10 entries
//
// The search of the networks around with uds: a status message carries the key of the network and
// its UdsAroundNetworkInfo. The member name is ours.
class UdsAroundNetworkSearchManager : public ::nn::pia::local::LocalAroundNetworkSearchManager
{
public:
    UdsAroundNetworkSearchManager(); // 0x004235B0
    // (a nop that falls into the destructor of LocalAroundNetworkSearchManager)
    virtual ~UdsAroundNetworkSearchManager(); // 0x004249BC slot 0x00
    // 0x00423604 slot 0x04 (deleting dtor)
    virtual nn::pia::local::LocalAroundNetworkInfo* CreateLocalAroundNetworkInfo(); // 0x0042343C slot 0x10
    virtual nn::pia::local::LocalAroundNetworkSearchBackgroundJob* CreateLocalAroundNetworkSearchBackgroundJob(); // 0x00423588 slot 0x14
    virtual nn::Result GetAroundNetworkInfoList(void* pBuffer, u32* pNum, u32 bufferNum); // 0x007316E4 slot 0x18
    // (the linker put it in front of LocalMessage::SetData(const void*, u16), into which it falls)
    virtual void SerializeAroundNetworkStatus(nn::pia::local::LocalAroundNetworkSearchManager::LocalAroundNetworkStatusMessage* pMessage, const nn::pia::local::LocalAroundNetworkSearchManager::AroundNetworkStatus* pStatus) const; // 0x004149FC slot 0x1C
    virtual void DeserializeAroundNetworkStatus(const nn::pia::local::LocalAroundNetworkSearchManager::LocalAroundNetworkStatusMessage* pMessage); // 0x00423484 slot 0x20
    virtual void vf_0x24(); // 0x00731768 slot 0x24

    nn::pia::local::UdsAroundNetworkInfo m_ReceivedInfo; // 0x7338, of the last status message
};
ASSERT_SIZE(UdsAroundNetworkSearchManager, 0x7628);
} // namespace local
} // namespace pia
} // namespace nn
