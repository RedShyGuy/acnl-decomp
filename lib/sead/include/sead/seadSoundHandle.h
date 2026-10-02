#pragma once

#include "decomp.h"
#include "nw/snd/snd_SoundHandle.h"

namespace sead {
// RTTI N4sead11SoundHandleE @ 0x008D1430
class SoundHandle : public ::nw::snd::SoundHandle
{
public:
    SoundHandle(); // ctor address unknown
    void pause(int); // 0x00540BA8 | mk7dlp:bytes [tier B]
    void unpause(int); // 0x00540BD8 | mk7dlp:bytes [tier B]
};
} // namespace sead
