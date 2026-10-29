#pragma once

#include "decomp.h"
#include "nw/snd/snd_SoundHandle.h"
#include "nw/snd/internal/snd_BasicSound.h"

namespace sead {
// RTTI N4sead11SoundHandleE @ 0x008D1430
//
// The sound handle of nw with the calls that test for an attached sound (pause/unpause are the names of
// the reference symbols, setPan/setVolume are ours).
class SoundHandle : public ::nw::snd::SoundHandle
{
public:
    SoundHandle() : nw::snd::SoundHandle() {}
    void pause(int fadeFrames); // 0x00540BA8 | mk7dlp:bytes [tier B]
    void setPan(f32 pan); // 0x00540BC4 (name is ours)
    void unpause(int fadeFrames); // 0x00540BD8 | mk7dlp:bytes [tier B]
    void setVolume(f32 volume, int frames); // 0x00540BF4 (name is ours)
};
} // namespace sead
