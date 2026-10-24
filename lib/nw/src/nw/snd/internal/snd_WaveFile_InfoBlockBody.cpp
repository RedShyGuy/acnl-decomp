#include "nw/snd/internal/snd_WaveFile_InfoBlockBody.h"

namespace nw {
namespace snd {
namespace internal {
// 0x007431B8 | nintendogs:bytes [tier A]
const nw::snd::internal::WaveFile::ChannelInfo* nw::snd::internal::WaveFile::InfoBlockBody::GetChannelInfo(int channelIndex) const
{
    return static_cast<const ChannelInfo*>(channelInfoTable.GetReferedItem(channelIndex));
}

} // namespace internal
} // namespace snd
} // namespace nw
