#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalSessionInfo.h"
#include "nn/pia/local/local_UdsNetworkDescription.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local14UdsSessionInfoE @ 0x008CFAEC
// vtable 0x009008B4 (vptr 0x009008BC), offset_to_top 0, 18 entries
//
// A found session of the uds network: its description and its stations. The member names are
// ours.
class UdsSessionInfo : public ::nn::pia::local::LocalSessionInfo
{
public:
    static const u32 STATION_INFO_NUM = 12;

    UdsSessionInfo(); // 0x004168B4
    // (a nop that falls into the destructor of LocalSessionInfo)
    virtual ~UdsSessionInfo(); // 0x00416BDC slot 0x00
    // 0x00416938 slot 0x04 (deleting dtor)
    virtual u8 GetSubId() const; // 0x00730084 slot 0x08
    virtual u32 GetSessionId() const; // 0x007300A0 slot 0x0C
    virtual u8 GetCurrentParticipants() const; // 0x007301A0 slot 0x10
    virtual u8 GetMaxParticipants() const; // 0x007300E0 slot 0x18
    virtual bool IsOpened() const; // 0x00730214 slot 0x1C
    virtual void Clear(); // 0x00416868 slot 0x20
    virtual void Trace(u64 flag) const; // 0x004168B0 slot 0x24
    virtual nn::Result GetApplicationData(void* pBuffer, u32 bufferSize) const; // 0x0041672C slot 0x28
    virtual nn::Result GetApplicationDataSize(u32* pSize) const; // 0x004167C4 slot 0x2C
    virtual nn::Result GetStationInfos(nn::pia::local::LocalStationInfoBuffer* pBuffer) const; // 0x007300FC slot 0x34
    virtual nn::Result GetBssid(u8* pBssid) const; // 0x007301C4 slot 0x38
    virtual const nn::pia::local::LocalNetworkDescription* GetNetworkDescription() const; // 0x007301BC slot 0x3C
    virtual void SetNetworkDescription(const nn::pia::local::LocalNetworkDescription* pDescription); // 0x00416844 slot 0x40
    virtual nn::Result UpdateStationInfos(u32 index); // 0x004167A8 slot 0x44

    nn::pia::local::UdsNetworkDescription m_Description;               // 0x008
    nn::pia::local::LocalStationInfo m_StationInfos[STATION_INFO_NUM]; // 0x114
};
ASSERT_SIZE(UdsSessionInfo, 0x2F4);
} // namespace local
} // namespace pia
} // namespace nn
