#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
// RTTI N2nw3snd11SoundHandleE @ 0x008D08D8
class SoundHandle
{
public:
    SoundHandle(); // ctor address unknown
    void DetachSound(); // 0x00137B30 | nintendogs:bytes [tier A]
    void detail_AttachSound(nw::snd::internal::BasicSound*); // 0x004BF660 | nintendogs:bytes [tier A]
    void detail_AttachSoundAsTempHandle(nw::snd::internal::BasicSound*); // 0x004BF68C | nintendogs:bytes [tier A]
};
} // namespace snd
} // namespace nw
