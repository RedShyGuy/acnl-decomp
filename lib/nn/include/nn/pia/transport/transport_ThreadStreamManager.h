#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport19ThreadStreamManagerE @ 0x008D0200
// vtable 0x00901EB8 (vptr 0x00901EC0), offset_to_top 0, 1 entries
class ThreadStreamManager : public ::nn::pia::common::RootObject
{
public:
    ThreadStreamManager(); // ctor candidate(s) 0x00457F58, 0x004580A4 (unverified)
    virtual void vf_0x00(); // 0x007360A4 slot 0x00 | virtual slot, introduced by nn::pia::transport::ThreadStreamManager
    void CreateInstance(nn::pia::transport::NetworkFactory*, unsigned int, unsigned int, unsigned int, unsigned int, bool); // 0x00457F58 | fefates:bytes [tier B]
    void DestroyInstance(); // 0x004580A4 | fefates:bytes [tier B]
    void SetMonitoringData(); // 0x00458164 | fefates:bytes [tier B]
    void Cleanup(); // 0x004581E0 | fefates:bytes [tier B]
    void Startup(); // 0x0045821C | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
