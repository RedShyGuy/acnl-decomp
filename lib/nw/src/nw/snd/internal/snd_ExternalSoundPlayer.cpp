#include "nw/snd/internal/snd_ExternalSoundPlayer.h"

#include "nw/snd/internal/snd_BasicSound.h"

namespace nw {
namespace snd {
namespace internal {
// 0x004C4F64 | fefates:bytes [tier B]
void nw::snd::internal::ExternalSoundPlayer::RemoveSound(BasicSound* pSound)
{
    m_SoundList.Erase(pSound);
    pSound->DetachExternalSoundPlayer(this);
}

// 0x004C8D18 | fefates:bytes [tier B]
bool nw::snd::internal::ExternalSoundPlayer::AppendSound(BasicSound* pSound)
{
    const int priority = pSound->CalcCurrentPlayerPriority();
    if (m_PlayableCount == 0) {
        return false;
    }
    // the sounds with a lower priority make room
    while (m_SoundList.size() >= m_PlayableCount) {
        BasicSound* pLowest = GetLowestPrioritySound();
        if (pLowest == nullptr) {
            return false;
        }
        if (priority < pLowest->CalcCurrentPlayerPriority()) {
            return false;
        }
        pLowest->Finalize();
    }
    m_SoundList.PushBack(pSound);
    pSound->m_pExtSoundPlayer = this;
    return true;
}

// 0x004C8DE8 | nintendogs:bytes-fuzzy [tier A]
bool nw::snd::internal::ExternalSoundPlayer::detail_CanPlaySound(int priority)
{
    if (m_PlayableCount == 0) {
        return false;
    }
    if (m_PlayableCount > m_SoundList.size()) {
        return true;
    }
    BasicSound* pLowest = GetLowestPrioritySound();
    if (pLowest == nullptr) {
        return false;
    }
    return pLowest->CalcCurrentPlayerPriority() <= priority;
}

// 0x004C8E3C | nintendogs:bytes-fuzzy [tier A]
BasicSound* nw::snd::internal::ExternalSoundPlayer::GetLowestPrioritySound()
{
    int lowest = 128;
    BasicSound* pLowest = nullptr;
    for (SoundList::iterator it = m_SoundList.begin(); it != m_SoundList.end(); ++it) {
        BasicSound* pSound = SoundList::GetPointerFromNode(it.GetNode());
        const int priority = pSound->CalcCurrentPlayerPriority();
        if (lowest > priority) {
            pLowest = pSound;
            lowest = priority;
        }
    }
    return pLowest;
}

} // namespace internal
} // namespace snd
} // namespace nw
