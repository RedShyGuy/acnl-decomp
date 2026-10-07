#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/common/common_StationAddress.h"

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport15StationLocationE @ 0x008D01AC
// vtable 0x00901DEC (vptr 0x00901DF4), offset_to_top 0, 6 entries
//
// Where a station is: its address and the values of its nex station URL (inet::NexFacade::
// ConvertNexStationUrlToStationLocation copies them with the getters of nex::StationURL; in the
// old format of StationConnectionInfo they are packed: 0x18 and 0x1C in 22 bits each, 0x20, 0x23
// and 0x24 in 2 bits, 0x22 in 4 bits; 0x26 is only in the new format). Layout from the
// constructor and Serialize; the member names (after the getters) and SetStationAddress are ours.
class StationLocation : public ::nn::pia::common::RootObject
{
public:
    StationLocation(); // 0x00451344 | fefates:bytes [tier B]
    StationLocation(const nn::pia::transport::StationLocation& rhs); // 0x004512C8 | fefates:bytes [tier B]
    virtual ~StationLocation(); // 0x004513B8 slot 0x00 | fefates:bytes
    // 0x0045138C slot 0x04 (deleting dtor)
    virtual u32 GetSerializedSize() const; // 0x007352F0 slot 0x08 | fefates:bytes
    virtual nn::Result Serialize(unsigned char* pBuffer, unsigned int* pSize, unsigned int bufferSize) const; // 0x00735308 slot 0x0C | fefates:bytes
    virtual nn::Result Deserialize(const unsigned char* pBuffer); // 0x00451138 slot 0x10 | fefates:bytes
    virtual void Trace(unsigned long long flag) const; // 0x00735304 slot 0x14

    void SetStationLocation(const nn::pia::transport::StationLocation& rhs); // 0x0045125C | fefates:bytes [tier B]
    void SetStationAddress(const common::StationAddress& address); // 0x0045122C (name is ours)
    StationLocation& operator=(const nn::pia::transport::StationLocation& rhs); // 0x004513E0 | fefates:bytes [tier B]
    bool operator==(const nn::pia::transport::StationLocation& rhs) const; // 0x00735450 | fefates:bytes [tier B]

    common::StationAddress m_StationAddress; // 0x04
    u32 m_PrincipalId;                       // 0x14
    u32 m_ConnectionId;                      // 0x18
    // the RV connection id (StationConnectionInfoTable::GetStationKey)
    u32 m_StationKey;                        // 0x1C
    u8 m_UrlType;                            // 0x20
    u8 m_StreamId;                           // 0x21
    u8 m_StreamType;                         // 0x22
    // the NAT of the station (nex::NATProperties)
    u8 m_NatMapping;                         // 0x23
    u8 m_NatFiltering;                       // 0x24
    // bit 0: behind a NAT, bit 1: public (inet::NexFacade::IsBehindNat, IsPublic)
    u8 m_Type;                               // 0x25
    u8 m_ProbeRequestInitiation;             // 0x26
};
ASSERT_OFFSET(StationLocation, m_PrincipalId, 0x14);
ASSERT_SIZE(StationLocation, 0x28);
} // namespace transport
} // namespace pia
} // namespace nn
