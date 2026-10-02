#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport9TransportE @ 0x008D02CC
// vtable 0x00902094 (vptr 0x0090209C), offset_to_top 0, 1 entries
class Transport : public ::nn::pia::common::RootObject
{
public:
    class DispatchJob;
    struct Setting { u32 _unknown; }; // TODO: real type unknown (placeholder)
    virtual void vf_0x00(); // 0x007370E8 slot 0x00 | virtual slot, introduced by nn::pia::transport::Transport
    void initialize(const nn::pia::transport::Transport::Setting&); // 0x0045FA5C | fefates:bytes-fuzzy [tier B]
    void GetInputStream(); // 0x0045FEE0 | fefates:bytes [tier B]
    void DestroyInstance(); // 0x0045FF08 | fefates:bytes [tier B]
    void GetOutputStream(); // 0x00460000 | fefates:bytes [tier B]
    void OutputStreamUpdateEvent(); // 0x00460060 | fefates:bytes [tier B]
    void Cleanup(); // 0x00460254 | fefates:bytes [tier B]
    void Startup(const nn::pia::common::StationAddress*, const nn::pia::common::CryptoSetting*); // 0x00460330 | fefates:bytes [tier B]
    void finalize(); // 0x004605AC | fefates:bytes [tier B]
    Transport(); // 0x004606B0 | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
