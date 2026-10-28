#include "nw/snd/snd_SequenceSoundHandle.h"

#include "nw/snd/internal/snd_SequenceSound.h"
#include "nw/snd/snd_SoundHandle.h"

namespace nw {
namespace snd {
// 0x004C3964 (name after SoundHandle)
void nw::snd::SequenceSoundHandle::detail_AttachSoundAsTempHandle(internal::SequenceSound* pSound)
{
    m_pSound = pSound;
    if (pSound->IsAttachedTempSpecialHandle()) {
        m_pSound->DetachTempSpecialHandle();
    }
    m_pSound->m_pTempSpecialHandle = this;
}

// 0x004C39A4 | nintendogs:bytes-fuzzy (fefates: WaveSoundHandle; the code casts to SequenceSound)
nw::snd::SequenceSoundHandle::SequenceSoundHandle(SoundHandle* pHandle) : m_pSound(nullptr)
{
    if (pHandle == nullptr || pHandle->m_pSound == nullptr) {
        return;
    }
    internal::SequenceSound* pSound = internal::DynamicCast<internal::SequenceSound>(pHandle->m_pSound);
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
