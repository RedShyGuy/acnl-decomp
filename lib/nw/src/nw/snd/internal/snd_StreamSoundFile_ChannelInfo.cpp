#include "nw/snd/internal/snd_StreamSoundFile_ChannelInfo.h"

namespace nw {
namespace snd {
namespace internal {
// 0x0073FBA0 | nintendogs:bytes [tier A]
const nw::snd::internal::StreamSoundFile::DspAdpcmChannelInfo* nw::snd::internal::StreamSoundFile::ChannelInfo::GetDspAdpcmChannelInfo() const
{
    if (adpcmInfo.typeId != TYPE_DSP_ADPCM_INFO) {
        return nullptr;
    }
    return static_cast<const DspAdpcmChannelInfo*>(GetReferedPointer(this, adpcmInfo));
}

} // namespace internal
} // namespace snd
} // namespace nw
