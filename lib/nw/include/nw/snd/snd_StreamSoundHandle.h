#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
class StreamSoundHandle
{
public:
    StreamSoundHandle(); // TODO: default ctor added so derived stubs compile - may not exist
    void DetachSound(); // 0x004C08BC | fefates:bytes [tier B]
    StreamSoundHandle(nw::snd::SoundHandle*); // 0x004C08F4 | nintendogs:bytes-fuzzy [tier A]
};
} // namespace snd
} // namespace nw
