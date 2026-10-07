#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/transport/transport_StationLocation.h"

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport21StationConnectionInfoE @ 0x008D0248
// vtable 0x00901F5C (vptr 0x00901F64), offset_to_top 0, 9 entries
//
// The locations of both ends of a connection to a station: the public one and the one in the
// local network (the order is a guess from the use in the connection jobs). Besides the
// current format there is an older, packed one (slots 0x14 to 0x1C). The member names and the
// names of the slots 0x14 to 0x1C and of SetStationConnectionInfo are ours.
class StationConnectionInfo : public ::nn::pia::common::RootObject
{
public:
    // the size of the older format: both internet addresses (6 bytes each), both extension ids
    // and 16 bytes per location
    static const u32 LEGACY_SERIALIZED_SIZE = 48;

    StationConnectionInfo(); // 0x0045B7C4 | fefates:bytes [tier B]
    StationConnectionInfo(const nn::pia::transport::StationConnectionInfo& rhs); // 0x0045B780 | fefates:bytes [tier B]
    virtual ~StationConnectionInfo(); // 0x0045B810 slot 0x00 | fefates:callgraph
    // 0x0045B7E8 slot 0x04 (deleting dtor)
    virtual u32 GetSerializedSize() const; // 0x00736228 slot 0x08 | fefates:bytes
    virtual nn::Result Serialize(unsigned char* pBuffer, unsigned int* pSize, unsigned int bufferSize) const; // 0x00736428 slot 0x0C | fefates:bytes
    virtual nn::Result Deserialize(const unsigned char* pBuffer); // 0x0045B518 slot 0x10 | fefates:bytes
    // the buffer size the older format asks for (more than it writes)
    virtual u32 GetLegacySerializedSize() const; // 0x0073641C slot 0x14 (name is ours)
    virtual nn::Result SerializeLegacy(unsigned char* pBuffer, unsigned int* pSize, unsigned int bufferSize) const; // 0x0073624C slot 0x18 (name is ours)
    virtual nn::Result DeserializeLegacy(const unsigned char* pBuffer); // 0x0045B590 slot 0x1C (name is ours)
    virtual void Trace(u64 flag) const; // 0x00736424 slot 0x20

    void SetStationConnectionInfo(const nn::pia::transport::StationConnectionInfo& rhs); // 0x00451234 (name is ours)
    StationConnectionInfo& operator=(const nn::pia::transport::StationConnectionInfo& rhs); // 0x0045B834 | fefates:bytes [tier B]
    bool operator==(const nn::pia::transport::StationConnectionInfo& rhs) const; // 0x00736508 | fefates:bytes [tier B]

    StationLocation m_PublicLocation;  // 0x04
    StationLocation m_PrivateLocation; // 0x2C
};
ASSERT_SIZE(StationConnectionInfo, 0x54);
} // namespace transport
} // namespace pia
} // namespace nn
