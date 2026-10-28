#include "nw/snd/snd_WaveSoundHandle.h"

#include "nw/snd/internal/snd_WaveSound.h"

namespace nw {
namespace snd {
// 0x00132530 | fefates:bytes [tier B]
void nw::snd::WaveSoundHandle::DetachSound()
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

} // namespace snd
} // namespace nw
