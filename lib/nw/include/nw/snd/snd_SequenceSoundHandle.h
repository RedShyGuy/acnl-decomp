#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
class SoundHandle;

namespace internal {
class SequenceSound;
} // namespace internal

// A handle of the game to a sequence sound (the class name is from the reference symbols, the member is
// ours; only what SequenceSound uses).
class SequenceSoundHandle
{
public:
    // the sequence sound of the handle (the handle becomes its temporary special handle)
    explicit SequenceSoundHandle(SoundHandle* pHandle); // 0x004C39A4 | nintendogs:bytes-fuzzy (fefates: WaveSoundHandle; the code casts to SequenceSound)
    // (armlink folded the code with WaveSoundHandle::DetachSound 0x00132530)
    void DetachSound();
    // (the game calls it)
    void detail_AttachSoundAsTempHandle(internal::SequenceSound* pSound); // 0x004C3964 (name after SoundHandle)

    internal::SequenceSound* m_pSound; // 0x0
};
} // namespace snd
} // namespace nw
