#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport21StationConnectionInfoE @ 0x008D0248
// vtable 0x00901F5C (vptr 0x00901F64), offset_to_top 0, 9 entries
class StationConnectionInfo : public ::nn::pia::common::RootObject
{
public:
    virtual void vf_0x00(); // 0x0045B810 slot 0x00 | fefates:callgraph
    virtual void vf_0x04(); // 0x0045B7E8 slot 0x04 | virtual slot, introduced by nn::pia::transport::StationConnectionInfo
    virtual void GetSerializedSize() const; // 0x00736228 slot 0x08 | fefates:bytes
    virtual void Serialize(unsigned char*, unsigned int*, unsigned int) const; // 0x00736428 slot 0x0C | fefates:bytes
    virtual void Deserialize(const unsigned char*); // 0x0045B518 slot 0x10 | fefates:bytes
    virtual void vf_0x14(); // 0x0073641C slot 0x14 | virtual slot, introduced by nn::pia::transport::StationConnectionInfo
    virtual void vf_0x18(); // 0x0073624C slot 0x18 | virtual slot, introduced by nn::pia::transport::StationConnectionInfo
    virtual void vf_0x1C(); // 0x0045B590 slot 0x1C | virtual slot, introduced by nn::pia::transport::StationConnectionInfo
    virtual void vf_0x20(); // 0x00736424 slot 0x20 | virtual slot, introduced by nn::pia::transport::StationConnectionInfo
    StationConnectionInfo(const nn::pia::transport::StationConnectionInfo&); // 0x0045B780 | fefates:bytes [tier B]
    StationConnectionInfo(); // 0x0045B7C4 | fefates:bytes [tier B]
    void operator=(const nn::pia::transport::StationConnectionInfo&); // 0x0045B834 | fefates:bytes [tier B]
    void operator==(const nn::pia::transport::StationConnectionInfo&) const; // 0x00736508 | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
