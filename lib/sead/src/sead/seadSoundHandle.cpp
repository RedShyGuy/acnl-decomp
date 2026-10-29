#include "sead/seadSoundHandle.h"

namespace sead {
// 0x00540BA8 | mk7dlp:bytes [tier B]
void sead::SoundHandle::pause(int fadeFrames)
{
    if (m_pSound != nullptr) {
        m_pSound->Pause(true, fadeFrames);
    }
}

// 0x00540BC4 (name is ours)
void sead::SoundHandle::setPan(f32 pan)
{
    if (m_pSound != nullptr) {
        m_pSound->SetPan(pan);
    }
}

// 0x00540BD8 | mk7dlp:bytes [tier B]
void sead::SoundHandle::unpause(int fadeFrames)
{
    if (m_pSound != nullptr) {
        m_pSound->Pause(false, fadeFrames);
    }
}

// 0x00540BF4 (name is ours)
void sead::SoundHandle::setVolume(f32 volume, int frames)
{
    if (m_pSound != nullptr) {
        m_pSound->SetVolume(volume, frames);
    }
}
} // namespace sead
