#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// RTTI N2nw3snd8internal6driver15DisposeCallbackE @ 0x008D0A7C
// vtable 0x00903320 (vptr 0x00903328), offset_to_top 0, 3 entries
class DisposeCallback
{
public:
    DisposeCallback(); // ctor address unknown
    virtual ~DisposeCallback(); // 0x004CCD88 slot 0x00 | slot vf_0x00 of nw::snd::internal::driver::DisposeCallback
    virtual void vf_0x04(); // 0x004CCD84 slot 0x04 | virtual slot, introduced by nw::snd::internal::driver::DisposeCallback
    virtual void InvalidateData(const void*, const void*); // 0x0011C12F slot 0x08 | slot vf_0x00 of ChangeRentalBase
};
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
