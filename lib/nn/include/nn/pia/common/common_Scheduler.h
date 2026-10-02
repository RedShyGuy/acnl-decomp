#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace common {
// RTTI N2nn3pia6common9SchedulerE @ 0x008CFF24
// vtable 0x00901624 (vptr 0x0090162C), offset_to_top 0, 1 entries
class Scheduler : public ::nn::pia::common::RootObject
{
public:
    Scheduler(); // ctor candidate(s) 0x00429B2C, 0x00429C44 (unverified)
    virtual void vf_0x00(); // 0x0073357C slot 0x00 | virtual slot, introduced by nn::pia::common::Scheduler
    void EntryJobNext(nn::pia::common::Job*); // 0x004295DC | fefates:bytes [tier B]
    void CreateInstance(int); // 0x00429B2C | fefates:bytes [tier B]
    void DestroyInstance(); // 0x00429C44 | fefates:bytes [tier B]
    void Dispatch(unsigned int); // 0x00429CB0 | fefates:bytes [tier B]
    void EntryJob(nn::pia::common::Job*, bool); // 0x00429F04 | fefates:bytes-fuzzy [tier B]
    void ResetJob(nn::pia::common::Job*); // 0x00429FA8 | fefates:bytes [tier B]
};
} // namespace common
} // namespace pia
} // namespace nn
