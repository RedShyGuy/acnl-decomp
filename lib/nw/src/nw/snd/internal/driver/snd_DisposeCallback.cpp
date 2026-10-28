#include "nw/snd/internal/driver/snd_DisposeCallback.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// 0x004CCD88 slot 0x00
// 0x004CCD84 (deleting dtor)
nw::snd::internal::driver::DisposeCallback::~DisposeCallback()
{
    // empty (in the original too)
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
