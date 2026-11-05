#include "Sound/dSoundMgr.h"
#include "sead/seadAudioPlayerCtr.h"
#include "time/time_Api.h"

// 0x0094E080
SoundMgr* SoundMgr::s_pInstance = NULL;

namespace {
// 0x0094E090
const sead::SafeStringBase<char> s_RomSoundRoot("rom:/Sound/");
// 0x0094E098
const sead::SafeStringBase<char> s_PatchSoundRoot("patch://Sound/");
} // namespace
// (builds s_RomSoundRoot and s_PatchSoundRoot)
// 0x007859B4 (static initializer)

// 0x0058DE9C | libgarden [tier A]
void SoundMgr::PlaySound(SeID, float)
{
}

// 0x006B2884
nw::snd::SoundStartable::StartResult SoundMgr::StartSound(nw::snd::SoundHandle* handle, u32 soundId)
{
    if (m_pPlayer == NULL) {
        return static_cast<nw::snd::SoundStartable::StartResult>(0);
    }
    return static_cast<nw::snd::SoundArchivePlayer*>(m_pPlayer)->StartSound(handle, soundId, NULL);
}

// 0x00767EF0
bool SoundMgr::IsNight()
{
    if (time::GetCurrentDate()->hour < 7) {
        return true;
    }
    return time::GetCurrentDate()->hour >= 19;
}

// 0x006B55F8
f32 SoundMgr::Lerp(s32 x, s32 x0, s32 x1, f32 y0, f32 y1)
{
    if (x0 == x1) {
        return y0;
    }
    return static_cast<f32>(x - x0) * (y1 - y0) / static_cast<f32>(x1 - x0) + y0;
}

// 0x006B28A0
nw::snd::SoundStartable::StartResult SoundMgr::StartSound(nw::snd::SoundHandle* handle, u32 soundId,
                                                          const nw::snd::SoundStartable::StartInfo* info)
{
    if (m_pPlayer == NULL) {
        return static_cast<nw::snd::SoundStartable::StartResult>(0);
    }
    return static_cast<nw::snd::SoundArchivePlayer*>(m_pPlayer)->StartSound(handle, soundId, info);
}

// 0x006B2BCC
nw::snd::SoundStartable::StartResult SoundMgr::PrepareSound(nw::snd::SoundHandle* handle, u32 soundId)
{
    if (m_pPlayer == NULL) {
        return static_cast<nw::snd::SoundStartable::StartResult>(0);
    }
    return static_cast<nw::snd::SoundArchivePlayer*>(m_pPlayer)->PrepareSound(handle, soundId, NULL);
}

// 0x006B2BE8
nw::snd::SoundStartable::StartResult SoundMgr::PrepareSound(nw::snd::SoundHandle* handle, u32 soundId,
                                                            const nw::snd::SoundStartable::StartInfo* info)
{
    if (m_pPlayer == NULL) {
        return static_cast<nw::snd::SoundStartable::StartResult>(0);
    }
    return static_cast<nw::snd::SoundArchivePlayer*>(m_pPlayer)->PrepareSound(handle, soundId, info);
}

// 0x006B4CC0
bool SoundMgr::StartSoundChecked(nw::snd::SoundHandle* handle, u32 soundId)
{
    if (m_pPlayer == NULL) {
        return false;
    }
    m_LastStartResult = static_cast<nw::snd::SoundArchivePlayer*>(m_pPlayer)->StartSound(handle, soundId, NULL);
    return m_LastStartResult == 0;
}

// 0x0013CFDC
bool SoundMgr::ReadSoundInfo(nw::snd::SoundArchive::SoundInfo* info, u32 soundId)
{
    return m_pArchive->ReadSoundInfo(soundId, info);
}

// 0x00767E80
u32 SoundMgr::ReadSoundUserParam(u32 soundId)
{
    u32 value = 0;
    if (m_pArchive->ReadSoundUserParam(soundId, 0, value)) {
        return value;
    }
    return 0;
}

// 0x006B31B4
void SoundMgr::SetVolume(u32 volume, f32 value, s32 frames)
{
    if (m_IsInitialized && volume < 5) {
        m_VolumeMgr.SetVolume(volume, value, frames);
    }
}

// 0x006B315C
void SoundMgr::SetExternalFileRoot(bool isRom)
{
    const char* root;
    if (isRom) {
        root = s_RomSoundRoot.getCString();
    } else {
        root = s_PatchSoundRoot.getCString();
    }
    m_pArchive->SetExternalFileRoot(root);
}
