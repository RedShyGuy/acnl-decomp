#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
class BasicSound;
} // namespace internal

// The handle of the game to a sound (the class and function names are from the reference symbols, the
// member is ours).
// RTTI N2nw3snd11SoundHandleE @ 0x008D08D8
class SoundHandle
{
public:
    // (inline)
    SoundHandle() : m_pSound(nullptr) {}

    void DetachSound(); // 0x00137B30 | nintendogs:bytes [tier A]
    void detail_AttachSound(internal::BasicSound* pSound); // 0x004BF660 | nintendogs:bytes [tier A]
    void detail_AttachSoundAsTempHandle(internal::BasicSound* pSound); // 0x004BF68C | nintendogs:bytes [tier A]

    internal::BasicSound* m_pSound; // 0x0 (name is ours)
};
} // namespace snd
} // namespace nw
