#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace common {
// RTTI N2nn3pia6common3JobE @ 0x008CFEC4
// vtable 0x009015F0 (vptr 0x009015F8), offset_to_top 0, 4 entries
class Job : public ::nn::pia::common::RootObject
{
public:
    virtual ~Job(); // 0x00428BA8 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x00428BA0 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Reset(bool); // 0x00428934 slot 0x08 | fefates:bytes-fuzzy
    virtual void ExecuteCore(); // 0x0011C12F slot 0x0C | slot vf_0x00 of ChangeRentalBase
    void Ready(bool); // 0x004288C4 | fefates:bytes [tier B]
    void Resume(bool); // 0x004289A4 | fefates:bytes [tier B]
    void Execute(bool); // 0x00428A14 | fefates:bytes-fuzzy [tier B]
    Job(); // 0x00428B74 | fefates:bytes [tier B]
    void IsForeground() const; // 0x007331F0 | fefates:bytes-fuzzy [tier B]
};
} // namespace common
} // namespace pia
} // namespace nn
