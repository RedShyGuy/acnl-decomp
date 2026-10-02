#include "nw/snd/internal/driver/snd_Bank.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// 0x004D3C58 | fefates:bytes [tier B]
void nw::snd::internal::driver::Bank::CalcChannelVelocityVolume(unsigned char)
{
}

// 0x0074168C | nintendogs:callseq [tier A]
void nw::snd::internal::driver::Bank::NoteOn(const void*, const nw::snd::internal::driver::NoteOnInfo&, const nw::snd::SoundArchive&, const nw::snd::SoundArchivePlayer&, const nw::snd::internal::PlayerHeapDataManager*) const
{
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
