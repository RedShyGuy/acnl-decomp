#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
// Instantiations found in the binary:
//   nw::snd::internal::LoaderManager<nw::snd::internal::driver::SequenceSoundLoader>  typeinfo 0x008D09B4  vtable 0x00903108
//   nw::snd::internal::LoaderManager<nw::snd::internal::driver::WaveSoundLoader>  typeinfo 0x008D09A8  vtable 0x009030F0
template <typename T0>
class LoaderManager
{
public:
    // TODO: members unknown
};
} // namespace internal
} // namespace snd
} // namespace nw
