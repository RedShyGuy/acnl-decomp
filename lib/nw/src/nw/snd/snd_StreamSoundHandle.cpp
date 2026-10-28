#include "nw/snd/snd_StreamSoundHandle.h"

#include "nw/snd/internal/snd_StreamSound.h"
#include "nw/snd/snd_SoundHandle.h"

namespace nw {
namespace snd {
// 0x004C08BC | fefates:bytes [tier B]
void nw::snd::StreamSoundHandle::DetachSound()
{
    if (m_pSound == nullptr) {
        return;
    }
    if (m_pSound->m_pTempSpecialHandle == this) {
        m_pSound->m_pTempSpecialHandle = nullptr;
    }
    if (m_pSound != nullptr) {
        m_pSound = nullptr;
    }
}

// 0x004C08F4 | nintendogs:bytes-fuzzy [tier A]
nw::snd::StreamSoundHandle::StreamSoundHandle(SoundHandle* pHandle) : m_pSound(nullptr)
{
    if (pHandle == nullptr || pHandle->m_pSound == nullptr) {
        return;
    }
    internal::StreamSound* pSound = internal::DynamicCast<internal::StreamSound>(pHandle->m_pSound);
    if (pSound == nullptr) {
        return;
    }
    m_pSound = pSound;
    if (pSound->IsAttachedTempSpecialHandle()) {
        pSound->DetachTempSpecialHandle();
    }
    m_pSound->m_pTempSpecialHandle = this;
}

} // namespace snd
} // namespace nw
