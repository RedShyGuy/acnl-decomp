#include "nw/snd/internal/snd_BankFile_KeyRegion.h"

namespace nw {
namespace snd {
namespace internal {
// 0x00742F4C | nintendogs:bytes [tier A]
const nw::snd::internal::BankFile::VelocityRegion* nw::snd::internal::BankFile::KeyRegion::GetVelocityRegion(u32 velocity) const
{
    return static_cast<const VelocityRegion*>(GetRegion(velocityRegionTable, this, velocity));
}

} // namespace internal
} // namespace snd
} // namespace nw
