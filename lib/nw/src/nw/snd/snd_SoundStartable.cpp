#include "nw/snd/snd_SoundStartable.h"

#include "nw/snd/internal/snd_BasicSound.h"
#include "nw/snd/snd_SoundHandle.h"

namespace nw {
namespace snd {
// 0x004C0384 | fefates:bytes [tier B]
nw::snd::SoundStartable::StartResult nw::snd::SoundStartable::StartSound(SoundHandle* pHandle, const char* pLabel,
                                                                         const StartInfo* pStartInfo)
{
    const u32 soundId = detail_GetItemId(pLabel);
    if (soundId == 0xFFFFFFFF) {
        return START_ERR_INVALID_LABEL;
    }
    return StartSound(pHandle, soundId, pStartInfo);
}

// 0x004C03F8 | nintendogs:bytes [tier A]
nw::snd::SoundStartable::StartResult nw::snd::SoundStartable::StartSound(SoundHandle* pHandle, u32 soundId,
                                                                         const StartInfo* pStartInfo)
{
    const StartResult result = detail_SetupSound(pHandle, soundId, false, pStartInfo);
    if (result != START_SUCCESS) {
        return result;
    }
    if (pHandle->m_pSound != nullptr) {
        pHandle->m_pSound->StartPrepared();
    }
    return START_SUCCESS;
}

// 0x004C0438 | nintendogs:bytes [tier A]
nw::snd::SoundStartable::StartResult nw::snd::SoundStartable::PrepareSound(SoundHandle* pHandle, u32 soundId,
                                                                           const StartInfo* pStartInfo)
{
    return detail_SetupSound(pHandle, soundId, false, pStartInfo);
}

// 0x004C0458 (name is ours)
nw::snd::SoundStartable::StartResult nw::snd::SoundStartable::HoldSound(SoundHandle* pHandle, const char* pLabel,
                                                                        const StartInfo* pStartInfo)
{
    const u32 soundId = detail_GetItemId(pLabel);
    if (soundId == 0xFFFFFFFF) {
        return START_ERR_INVALID_LABEL;
    }
    return HoldSound(pHandle, soundId, pStartInfo);
}

// 0x004C04A4 (name is ours)
nw::snd::SoundStartable::StartResult nw::snd::SoundStartable::HoldSound(SoundHandle* pHandle, u32 soundId,
                                                                        const StartInfo* pStartInfo)
{
    if (pHandle->m_pSound != nullptr && pHandle->m_pSound->m_Id == soundId) {
        pHandle->m_pSound->SetAutoStopCounter(1);
    } else {
        const StartResult result = detail_SetupSound(pHandle, soundId, true, pStartInfo);
        if (result != START_SUCCESS) {
            return result;
        }
        if (pHandle->m_pSound != nullptr) {
            pHandle->m_pSound->StartPrepared();
        }
        pHandle->m_pSound->SetAutoStopCounter(1);
    }
    if (pStartInfo != nullptr && (pStartInfo->enableFlag & StartInfo::ENABLE_HOLD_FADE_FRAMES) && pHandle->m_pSound != nullptr) {
        pHandle->m_pSound->Stop(pStartInfo->holdFadeFrames);
    }
    return START_SUCCESS;
}

} // namespace snd
} // namespace nw
