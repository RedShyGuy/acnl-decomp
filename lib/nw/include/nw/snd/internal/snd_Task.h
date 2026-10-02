#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
// RTTI N2nw3snd8internal4TaskE @ 0x008D0A54
// vtable 0x009032F8 (vptr 0x00903300), offset_to_top 0, 3 entries
class Task
{
public:
    Task(); // ctor candidate(s) 0x004C9B60 (unverified)
    virtual void vf_0x00(); // 0x004C9BBC slot 0x00 | fefates:callgraph
    virtual void vf_0x04(); // 0x004C9BA8 slot 0x04 | virtual slot, introduced by nw::snd::internal::Task
    virtual void Execute(); // 0x0011C12F slot 0x08 | slot vf_0x00 of ChangeRentalBase
};
} // namespace internal
} // namespace snd
} // namespace nw
