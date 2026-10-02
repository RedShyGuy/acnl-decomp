#include "nw/snd/internal/driver/snd_DisposeCallback.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// ctor address unknown
nw::snd::internal::driver::DisposeCallback::DisposeCallback()
{
}

// 0x004CCD88 slot 0x00 | slot vf_0x00 of nw::snd::internal::driver::DisposeCallback
nw::snd::internal::driver::DisposeCallback::~DisposeCallback()
{
}

// 0x004CCD84 slot 0x04 | virtual slot, introduced by nw::snd::internal::driver::DisposeCallback
void nw::snd::internal::driver::DisposeCallback::vf_0x04()
{
}

// 0x0011C12F slot 0x08 | slot vf_0x00 of ChangeRentalBase
void nw::snd::internal::driver::DisposeCallback::InvalidateData(const void*, const void*)
{
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
