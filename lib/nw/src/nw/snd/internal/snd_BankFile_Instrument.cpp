#include "nw/snd/internal/snd_BankFile_Instrument.h"

namespace nw {
namespace snd {
namespace internal {
// 0x00742C84 | nintendogs:bytes [tier A]
const nw::snd::internal::BankFile::KeyRegion* nw::snd::internal::BankFile::Instrument::GetKeyRegion(u32 key) const
{
    return static_cast<const KeyRegion*>(GetRegion(keyRegionTable, this, key));
}

} // namespace internal
} // namespace snd
} // namespace nw
