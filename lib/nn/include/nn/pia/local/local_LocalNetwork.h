#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local12LocalNetworkE @ 0x008CFAD4
// vtable 0x00900880 (vptr 0x00900888), offset_to_top 0, 3 entries
class LocalNetwork : public ::nn::pia::common::RootObject
{
public:
    LocalNetwork(); // ctor candidate(s) 0x004164D0 (unverified)
    virtual void vf_0x00(); // 0x00416600 slot 0x00 | virtual slot, introduced by nn::pia::local::LocalNetwork
    virtual void vf_0x04(); // 0x004165D8 slot 0x04 | virtual slot, introduced by nn::pia::local::LocalNetwork
    virtual void vf_0x08(); // 0x00730034 slot 0x08 | virtual slot, introduced by nn::pia::local::LocalNetwork
    void CreateJobs(); // 0x00414AB4 | fefates:bytes-fuzzy [tier B]
    void CleanupJobs(); // 0x00414C50 | fefates:bytes-fuzzy [tier B]
    void DestroyJobs(); // 0x00414D70 | fefates:bytes [tier B]
    void ScanNetwork(nn::pia::common::CallContext*, unsigned int, unsigned char); // 0x00414EF0 | fefates:bytes-fuzzy [tier B]
    void DestroyNetwork(nn::pia::common::CallContext*); // 0x00415350 | fefates:bytes-fuzzy [tier B]
    void DestroyInstance(); // 0x004157F8 | fefates:bytes [tier B]
    void AllowParticipating(); // 0x00415BF4 | fefates:bytes-fuzzy [tier B]
    void DisallowParticipating(bool); // 0x00415D80 | fefates:bytes-fuzzy [tier B]
    void GetNetworkDescription(unsigned int); // 0x00415F20 | fefates:bytes [tier B]
    void Cleanup(); // 0x00416210 | fefates:bytes-fuzzy [tier B]
    void GetApplicationData(void*, unsigned int*, unsigned int, const nn::pia::local::LocalNetworkDescription*) const; // 0x0072FD00 | fefates:bytes-fuzzy [tier B]
    void IsDuringHostMigration() const; // 0x0072FE50 | fefates:bytes [tier B]
    void IsEnableHostMigration() const; // 0x0072FE90 | fefates:bytes [tier B]
    void IsEnableAroundNetworkSearch() const; // 0x0072FF98 | fefates:bytes [tier B]
};
} // namespace local
} // namespace pia
} // namespace nn
