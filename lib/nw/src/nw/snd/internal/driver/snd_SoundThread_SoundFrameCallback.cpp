#include "nw/snd/internal/driver/snd_SoundThread_SoundFrameCallback.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// 0x004CBA14 slot 0x0C (name is ours)
void nw::snd::internal::driver::SoundThread::SoundFrameCallback::OnEndSoundFrame()
{
    // empty (in the original too)
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
