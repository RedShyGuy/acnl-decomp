#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex16TransportAdapterE @ 0x008CE62C
// vtable 0x008FD28C (vptr 0x008FD294), offset_to_top 0, 13 entries
class TransportAdapter : public ::nn::nex::RootObject
{
public:
    TransportAdapter(); // ctor address unknown
    virtual ~TransportAdapter(); // 0x00383F78 slot 0x00 | slot vf_0x00 of nn::nex::TransportAdapter
    virtual void vf_0x04(); // 0x00383F54 slot 0x04 | virtual slot, introduced by nn::nex::TransportAdapter
    virtual void GetLocalURLs(nn::nex::qList<nn::nex::StationURL>*); // 0x00383A4C slot 0x08 | fefates:bytes
    virtual void vf_0x0C(); // 0x00383D90 slot 0x0C | fefates:callseq
    virtual void ReleaseAddress(const nn::nex::StationURL&); // 0x00383D8C slot 0x10 | slot vf_0x10 of nn::nex::TransportAdapter
    virtual void vf_0x14(); // 0x00383E40 slot 0x14 | fefates:callseq
    virtual void vf_0x18(); // 0x00383DD8 slot 0x18 | virtual slot, introduced by nn::nex::TransportAdapter
    virtual void vf_0x1C(); // 0x0072B778 slot 0x1C | virtual slot, introduced by nn::nex::TransportAdapter
    virtual void Load(); // 0x00383F4C slot 0x20 | slot vf_0x20 of nn::nex::TransportAdapter
    virtual void Unload(); // 0x00383F50 slot 0x24 | slot vf_0x24 of nn::nex::TransportAdapter
    virtual void SetNetworkInterfaceInfo(nn::nex::InterfaceInfo*); // 0x00383DF8 slot 0x28 | fefates:bytes
    virtual void SetLocalAddress(const nn::nex::String&); // 0x00383DE0 slot 0x2C | mk7dlp:bytes
    virtual void SetPortNumber(unsigned short); // 0x00383D7C slot 0x30 | slot vf_0x30 of nn::nex::TransportAdapter
};
} // namespace nex
} // namespace nn
