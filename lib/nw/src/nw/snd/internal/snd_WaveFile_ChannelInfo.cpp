#include "nw/snd/internal/snd_WaveFile_ChannelInfo.h"

namespace nw {
namespace snd {
namespace internal {
// 0x007431A0 | nintendogs:bytes [tier A]
const void* nw::snd::internal::WaveFile::ChannelInfo::GetDspAdpcmInfo() const
{
    return GetReferedPointer(this, dspAdpcmInfo);
}

// 0x007431AC | nintendogs:bytes [tier A]
const void* nw::snd::internal::WaveFile::ChannelInfo::GetSamplesAddress(const void* pDataBlockBody) const
{
    return GetReferedPointer(pDataBlockBody, samples);
}

} // namespace internal
} // namespace snd
} // namespace nw
