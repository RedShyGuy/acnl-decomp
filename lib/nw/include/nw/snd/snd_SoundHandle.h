#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_BasicSound.h"

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
    ~SoundHandle() { DetachSound(); }
    // (name is ours)
    bool IsAttached() const { return m_pSound != nullptr; }
    // (inline; the calls on the attached sound, names after BasicSound)
    u32 GetId() const { return m_pSound != nullptr ? m_pSound->m_Id : 0xFFFFFFFF; }
    void Stop(int fadeFrames)
    {
        if (m_pSound != nullptr) {
            m_pSound->Stop(fadeFrames);
        }
    }
    void Pause(bool isPause, int fadeFrames)
    {
        if (m_pSound != nullptr) {
            m_pSound->Pause(isPause, fadeFrames);
        }
    }
    void FadeIn(int frames)
    {
        if (m_pSound != nullptr) {
            m_pSound->FadeIn(frames);
        }
    }
    void SetVolume(f32 volume, int frames = 0)
    {
        if (m_pSound != nullptr) {
            m_pSound->SetVolume(volume, frames);
        }
    }
    void SetPitch(f32 pitch)
    {
        if (m_pSound != nullptr) {
            m_pSound->m_Pitch = pitch;
        }
    }
    void SetPan(f32 pan)
    {
        if (m_pSound != nullptr) {
            m_pSound->SetPan(pan);
        }
    }
    void SetSurroundPan(f32 surroundPan)
    {
        if (m_pSound != nullptr) {
            m_pSound->m_SurroundPan = surroundPan;
        }
    }
    void SetBiquadFilter(int type, f32 value)
    {
        if (m_pSound != nullptr) {
            m_pSound->SetBiquadFilter(type, value);
        }
    }

    void DetachSound(); // 0x00137B30 | nintendogs:bytes [tier A]
    void detail_AttachSound(internal::BasicSound* pSound); // 0x004BF660 | nintendogs:bytes [tier A]
    void detail_AttachSoundAsTempHandle(internal::BasicSound* pSound); // 0x004BF68C | nintendogs:bytes [tier A]

    internal::BasicSound* m_pSound; // 0x0 (name is ours)
};
} // namespace snd
} // namespace nw
