#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_ExternalSoundPlayer.h"

namespace nw {
namespace snd {
// A sound source in the game; the game has no code of it, only BasicSound reads its parameters (the class
// name is from the reference symbols, the layout is only what BasicSound and SoundArchivePlayer read, names
// are ours).
class SoundActor
{
public:
    // the parameters the actor adds to its sounds
    struct ActorParam
    {
        // (inline)
        ActorParam() : volume(1.0f), pitch(1.0f), pan(0.0f), lpfFreq(0.0f) {}

        f32 volume;  // 0x0
        f32 pitch;   // 0x4
        f32 pan;     // 0x8
        f32 lpfFreq; // 0xC
    };

    static const int ACTOR_PLAYER_COUNT = 4;

    u8 m_Unknown0x00[0x8];                                           // 0x00
    internal::ExternalSoundPlayer m_ActorPlayers[ACTOR_PLAYER_COUNT]; // 0x08 (SoundInfo::actorPlayerId)
    ActorParam m_ActorParam;                                         // 0x48
};
} // namespace snd
} // namespace nw
