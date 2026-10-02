#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport14StationManagerE @ 0x008D0188
// vtable 0x00901DC8 (vptr 0x00901DD0), offset_to_top 0, 1 entries
class StationManager : public ::nn::pia::common::RootObject
{
public:
    virtual void vf_0x00(); // 0x007352E4 slot 0x00 | virtual slot, introduced by nn::pia::transport::StationManager
    void GetStation(nn::pia::StationId); // 0x0044F6E4 | fefates:bytes [tier B]
    void GetStation(const nn::pia::common::StationAddress&); // 0x0044F754 | fefates:bytes [tier B]
    void Initialize(nn::pia::transport::NetworkFactory*, unsigned int); // 0x0044F7BC | fefates:bytes [tier B]
    void CreateStation(); // 0x0044F980 | fefates:bytes [tier B]
    void DestroyStation(nn::pia::transport::Station*); // 0x0044FA88 | fefates:bytes [tier B]
    void DestroyInstance(); // 0x0044FBD4 | fefates:bytes [tier B]
    void CreateLocalStation(); // 0x0044FC28 | fefates:bytes [tier B]
    void Finalize(); // 0x0044FCE4 | fefates:bytes [tier B]
    StationManager(); // 0x0044FDAC | fefates:bytes [tier B]
    void GetStationAddress(nn::pia::common::StationAddress*, nn::pia::StationId) const; // 0x00735194 | libgarden [tier A]
    void GetStationAddress(nn::pia::common::StationAddress*, nn::pia::StationIndex) const; // 0x007351C0 | fefates:bytes [tier B]
    void GetParticipatingStationBitmap(bool) const; // 0x00735270 | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
