#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local11LocalFacadeE @ 0x008CFABC
// vtable 0x0090084C (vptr 0x00900854), offset_to_top 0, 5 entries
class LocalFacade : public ::nn::pia::common::RootObject
{
public:
    LocalFacade(); // ctor candidate(s) 0x00414678 (unverified)
    virtual void Startup(); // 0x004147F8 slot 0x00 | fefates:bytes-fuzzy
    virtual void Cleanup(); // 0x004147C8 slot 0x04 | fefates:bytes
    virtual void vf_0x08(); // 0x0041493C slot 0x08 | virtual slot, introduced by nn::pia::local::LocalFacade
    virtual ~LocalFacade(); // 0x00414938 slot 0x0C | slot vf_0x0C of nn::pia::local::LocalFacade
    virtual void vf_0x10(); // 0x0072FBAC slot 0x10 | virtual slot, introduced by nn::pia::local::LocalFacade
    void CreateInstance(); // 0x00414678 | fefates:bytes [tier B]
    void DestroyInstance(); // 0x00414710 | fefates:bytes [tier B]
    void LocalFacadeUpdateEventCallback(nn::pia::local::LocalUpdateEvent, unsigned char, void*); // 0x00414740 | fefates:bytes [tier B]
    void GetHostStationConnectionInfo(nn::pia::transport::StationConnectionInfo*) const; // 0x0072FAD4 | fefates:bytes [tier B]
};
} // namespace local
} // namespace pia
} // namespace nn
