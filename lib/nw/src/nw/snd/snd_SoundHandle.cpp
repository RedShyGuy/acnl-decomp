#include "nw/snd/snd_SoundHandle.h"

#include "nw/snd/internal/snd_BasicSound.h"

namespace nw {
namespace snd {
// 0x00137B30 | nintendogs:bytes [tier A]
void nw::snd::SoundHandle::DetachSound()
{
    if (m_pSound == nullptr) {
        return;
    }
    if (m_pSound->m_pGeneralHandle == this) {
        m_pSound->m_pGeneralHandle = nullptr;
    }
    if (m_pSound->m_pTempGeneralHandle == this) {
        m_pSound->m_pTempGeneralHandle = nullptr;
    }
    if (m_pSound != nullptr) {
        m_pSound = nullptr;
    }
}

// 0x004BF660 | nintendogs:bytes [tier A]
void nw::snd::SoundHandle::detail_AttachSound(internal::BasicSound* pSound)
{
    m_pSound = pSound;
    if (pSound->IsAttachedGeneralHandle()) {
        m_pSound->DetachGeneralHandle();
    }
    m_pSound->m_pGeneralHandle = this;
}

// 0x004BF68C | nintendogs:bytes [tier A]
void nw::snd::SoundHandle::detail_AttachSoundAsTempHandle(internal::BasicSound* pSound)
{
    m_pSound = pSound;
    if (pSound->IsAttachedTempGeneralHandle()) {
        m_pSound->DetachTempGeneralHandle();
    }
    m_pSound->m_pTempGeneralHandle = this;
}

} // namespace snd
} // namespace nw
