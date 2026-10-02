#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport15AttendanceTableE @ 0x008D0194
// vtable 0x00901DD4 (vptr 0x00901DDC), offset_to_top 0, 1 entries
class AttendanceTable : public ::nn::pia::common::RootObject
{
public:
    AttendanceTable(); // ctor candidate(s) 0x0044FEF0 (unverified)
    virtual void vf_0x00(); // 0x007352E8 slot 0x00 | virtual slot, introduced by nn::pia::transport::AttendanceTable
    void Initialize(); // 0x0044FEC0 | fefates:bytes [tier B]
    void CreateInstance(); // 0x0044FEF0 | fefates:bytes [tier B]
    void Update(bool, nn::pia::StationIndex); // 0x0044FF94 | fefates:bytes [tier B]
    void Startup(); // 0x0044FFFC | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
