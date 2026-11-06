#include "Sound/dSoundHandleSet.h"
#include "Sound/dSoundMgr.h"
#include "nw/snd/internal/snd_BasicSound.h"

// 0x009838BC
nw::snd::SoundArchive::SoundInfo SoundHandleSetBase::s_SoundInfo;
// 0x0083B920
const f32 SoundHandleSetBase::s_VolumeCurveOffsets[3] = {0.0f, 0.33f, 0.66f};
// 0x009838D4
f32 SoundHandleSetBase::s_VolumeCurveScales[3] = {1.0f, 1.0f / 0.67f, 1.0f / 0.34f};
// the initializer of s_VolumeCurveScales
// 0x0079A610 (static initializer)

// 0x003220D4
u32 SoundHandleSetBase::ReadUserParam(u32 soundId)
{
    return SoundMgr::s_pInstance->ReadSoundUserParam(soundId);
}

// 0x003220E4
bool SoundHandleSetBase::ReadSoundInfo(nw::snd::SoundArchive::SoundInfo* info, u32 soundId)
{
    return SoundMgr::s_pInstance->ReadSoundInfo(info, soundId);
}

// 0x003220F4
void SoundHandleSetBase::ApplyParam(nw::snd::SoundHandle* handle, s32 priorityOffset, const SoundParam* param, f32 pan,
                                    f32 surroundPan)
{
    if (handle == NULL || handle->m_pSound == NULL) {
        return;
    }
    if (param->isPan == true) {
        if (pan < -1.0f) {
            pan = -1.0f;
        } else if (pan > 1.0f) {
            pan = 1.0f;
        }
        handle->m_pSound->SetPan(pan);
    }
    if (param->isSurroundPan == true) {
        if (surroundPan < 0.0f) {
            surroundPan = 0.0f;
        } else if (surroundPan > 2.0f) {
            surroundPan = 2.0f;
        }
        if (handle->m_pSound != NULL) {
            handle->m_pSound->SetSurroundPan(surroundPan);
        }
    }
    if (param->isPriority == true) {
        s32 priority = param->priority - priorityOffset;
        if (priority < 0) {
            priority = 0;
        } else if (priority > 127) {
            priority = 127;
        }
        if (handle->m_pSound != NULL) {
            handle->m_pSound->SetPlayerPriority(priority);
        }
    }
}

// 0x007D0BB8
template s32 SoundHandleSet<1>::FindFreeIndex(u32 soundId);
// 0x007D0C30
template s32 SoundHandleSet<2>::FindFreeIndex(u32 soundId);
// 0x007D0E98
template s32 SoundHandleSet<3>::FindFreeIndex(u32 soundId);
// 0x007D0F94
template s32 SoundHandleSet<4>::FindFreeIndex(u32 soundId);
// 0x007D1090
template sead::SoundHandle* SoundHandleSet<8>::FindFreeHandle(u32 soundId);
// 0x00826D64
template bool SoundHandleSet<8>::IsPlaying(u32 soundId);
// 0x007D0D2C
template void SoundHandleSet<2>::SetVolume(f32 volume, s32 index);
