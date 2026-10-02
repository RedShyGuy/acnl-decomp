#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
class StreamSoundFile
{
public:
    class FileHeader;
    class ChannelInfo;
    class InfoBlockBody;
    struct RegionInfo { u32 _unknown; }; // TODO: real type unknown (placeholder)
    struct StreamSoundInfo { u32 _unknown; }; // TODO: real type unknown (placeholder)
};
} // namespace internal
} // namespace snd
} // namespace nw
