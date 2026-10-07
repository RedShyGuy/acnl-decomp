#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/local/local_LocalNetworkTypes.h"
#include "nn/pia/session/session_ISessionInfo.h"

namespace nn {
namespace pia {
namespace local {
class LocalNetworkDescription;

// the stations of a found session into the buffer of the caller (UdsSessionInfo::GetStationInfos;
// the name and the member names are ours)
struct LocalStationInfoBuffer
{
    u32 m_Unknown0x0;                          // 0x0
    nn::pia::local::LocalStationInfo* m_pInfos; // 0x4
    u8 m_InfoNum;                              // 0x8
};

// RTTI N2nn3pia5local16LocalSessionInfoE @ 0x008CFB30
// vtable 0x0090095C (vptr 0x00900964), offset_to_top 0, 18 entries
//
// A session of the local network that the matchmaking found (UdsSessionInfo implements it with the
// description of the uds network). The slot names and the member names are ours.
class LocalSessionInfo : public ::nn::pia::session::ISessionInfo
{
public:
    LocalSessionInfo(); // 0x00416BC0
    // (the destructor of UdsSessionInfo is a nop that falls into it)
    virtual ~LocalSessionInfo(); // 0x00416BE0 slot 0x00
    // 0x00416BD8 slot 0x04 (deleting dtor)
    virtual u8 GetSubId() const = 0; // slot 0x08
    virtual u32 GetSessionId() const = 0; // slot 0x0C
    virtual u8 GetCurrentParticipants() const = 0; // slot 0x10
    virtual bool IsValid() const; // 0x00730288 slot 0x14
    virtual u8 GetMaxParticipants() const = 0; // slot 0x18
    virtual bool IsOpened() const = 0; // slot 0x1C
    virtual void Clear(); // 0x00416BB0 slot 0x20
    virtual void Trace(u64 flag) const = 0; // slot 0x24
    virtual nn::Result GetApplicationData(void* pBuffer, u32 bufferSize) const = 0; // slot 0x28
    virtual nn::Result GetApplicationDataSize(u32* pSize) const = 0; // slot 0x2C
    virtual nn::Result GetLinkLevel(u8* pLevel) const; // 0x0073024C slot 0x30 (name is ours)
    virtual nn::Result GetStationInfos(nn::pia::local::LocalStationInfoBuffer* pBuffer) const = 0; // slot 0x34
    virtual nn::Result GetBssid(u8* pBssid) const = 0; // slot 0x38
    virtual const nn::pia::local::LocalNetworkDescription* GetNetworkDescription() const = 0; // slot 0x3C
    virtual void SetNetworkDescription(const nn::pia::local::LocalNetworkDescription* pDescription) = 0; // slot 0x40
    // the stations of the found network of the index (LocalNetwork::GetStationInfoList)
    virtual nn::Result UpdateStationInfos(u32 index) = 0; // slot 0x44

    // the link level of the found network of the index (name is ours)
    nn::Result UpdateLinkLevel(u32 index); // 0x00416B98

    bool m_IsValid;    // 0x4
    u8 m_LinkLevel;    // 0x5, of the network (LocalNetwork::GetLinkLevel)
};
ASSERT_SIZE(LocalSessionInfo, 0x8);
} // namespace local
} // namespace pia
} // namespace nn
