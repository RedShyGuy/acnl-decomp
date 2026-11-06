#include "Sound/dSoundObj.h"
#include "Sound/dSoundMgr.h"
#include "nw/snd/internal/snd_BasicSound.h"
#include "nw/snd/internal/snd_SequenceSound.h"
#include "nw/snd/snd_SequenceSoundHandle.h"

template <auto T0>
SoundObj<T0>::~SoundObj()
{
    SoundObj<T0>::Stop();
}

template <auto T0>
void SoundObj<T0>::Stop()
{
    for (s32 i = 0; i < T0; i++) {
        m_HandleSet.m_Handles[i].DetachSound();
    }
    SoundObjBase::Stop();
}

template <auto T0>
void SoundObj<T0>::SetPosition(const nn::math::VEC3* world, const nn::math::VEC3* relative)
{
    SoundObjBase::SetPosition(world, relative);
    bool isPlaying = false;
    for (s32 i = 0; i < T0; i++) {
        if (m_HandleSet.m_Handles[i].m_pSound != NULL) {
            isPlaying = true;
            break;
        }
    }
    if (isPlaying) {
        UpdateChanged();
        for (s32 i = 0; i < T0; i++) {
            if (m_HandleSet.m_Handles[i].m_pSound != NULL) {
                SetSoundVolume(m_Volume, i);
                SoundHandleSetBase::ApplyParam(&m_HandleSet.m_Handles[i], m_PlayerPriority, &m_HandleSet.m_Params[i],
                                               m_Pan, m_SurroundPan);
            }
        }
    }
    m_Counter++;
}

template <auto T0>
bool SoundObj<T0>::UpdateChanged()
{
    if (m_Counter <= 0) {
        return false;
    }
    SoundObjBase::Calc();
    m_Counter = 0;
    return true;
}

template <auto T0>
void SoundObj<T0>::SetSoundVolume(f32 volume, s32 index)
{
    m_HandleSet.SetVolume(volume, index);
}

template <auto T0>
inline s32 SoundObj<T0>::PrepareStart(u32 soundId, bool isStopEqual, const SoundParam** outParam)
{
    const SoundParam* param = NULL;
    for (s32 i = 0; i < T0; i++) {
        if (m_HandleSet.m_Params[i].soundId == soundId) {
            param = &m_HandleSet.m_Params[i];
            break;
        }
    }
    UpdateChanged();
    if (m_DistanceVolume <= 0.0f) {
        if (param == NULL) {
            SoundMgr::s_pInstance->ReadSoundUserParam(soundId);
            return -1;
        }
        if (param->_D == 0) {
            return -1;
        }
    }
    s32 index = m_HandleSet.FindFreeIndex(soundId);
    if (index == -1) {
        return -1;
    }
    if (param == NULL) {
        SoundMgr::s_pInstance->ReadSoundParam(&SoundHandleSetBase::s_SoundParam, soundId);
        param = &SoundHandleSetBase::s_SoundParam;
    }
    if (!SoundMgr::s_pInstance->CheckPlayerRoom(soundId, param, m_PlayerPriority, isStopEqual, this)) {
        return -1;
    }
    *outParam = param;
    return index;
}

template <auto T0>
inline void SoundObj<T0>::ApplyToHandle(s32 index, const SoundParam* param)
{
    if (m_HandleSet.m_Handles[index].m_pSound == NULL) {
        return;
    }
    if (&m_HandleSet.m_Params[index] != param) {
        m_HandleSet.m_Params[index] = *param;
    }
    if (m_HandleSet.m_Handles[index].m_pSound != NULL) {
        SetSoundVolume(m_Volume, index);
        SoundHandleSetBase::ApplyParam(&m_HandleSet.m_Handles[index], m_PlayerPriority, &m_HandleSet.m_Params[index],
                                       m_Pan, m_SurroundPan);
    }
}

template <auto T0>
sead::SoundHandle* SoundObj<T0>::StartSound(u32 soundId)
{
    if (!m_IsActive || SoundMgr::s_pInstance->m_State == 3) {
        return NULL;
    }
    const SoundParam* param;
    s32 index = PrepareStart(soundId, true, &param);
    if (index == -1) {
        return NULL;
    }
    sead::SoundHandle* handle = &m_HandleSet.m_Handles[index];
    handle->Stop(0);
    SoundMgr::s_pInstance->StartSound(handle, soundId);
    ApplyToHandle(index, param);
    return handle;
}

template <auto T0>
sead::SoundHandle* SoundObj<T0>::StartSound(u32 soundId, s16 value)
{
    sead::SoundHandle* handle = StartSound(soundId);
    if (handle != NULL) {
        nw::snd::SequenceSoundHandle sequence(handle);
        if (sequence.m_pSound != NULL) {
            sequence.m_pSound->WriteVariable(15, value);
        }
    }
    return handle;
}

template <auto T0>
sead::SoundHandle* SoundObj<T0>::HoldSound(u32 soundId)
{
    if (!m_IsActive) {
        return NULL;
    }
    for (s32 i = 0; i < T0; i++) {
        if (m_HandleSet.m_Handles[i].GetId() == soundId) {
            if (m_DistanceVolume <= 0.0f && m_HandleSet.m_Params[i]._D == 0) {
                return NULL;
            }
            SoundMgr::s_pInstance->HoldSound(&m_HandleSet.m_Handles[i], soundId);
            return &m_HandleSet.m_Handles[i];
        }
    }
    const SoundParam* param;
    s32 index = PrepareStart(soundId, false, &param);
    if (index == -1) {
        return NULL;
    }
    sead::SoundHandle* handle = &m_HandleSet.m_Handles[index];
    handle->Stop(0);
    SoundMgr::s_pInstance->HoldSound(handle, soundId);
    ApplyToHandle(index, param);
    UpdateTrainFade(soundId);
    return handle;
}

template <auto T0>
sead::SoundHandle* SoundObj<T0>::HoldSound(u32 soundId, s16 value)
{
    sead::SoundHandle* handle = HoldSound(soundId);
    if (handle != NULL) {
        nw::snd::SequenceSoundHandle sequence(handle);
        if (sequence.m_pSound != NULL) {
            sequence.m_pSound->WriteVariable(15, value);
        }
    }
    return handle;
}

template <auto T0>
sead::SoundHandle* SoundObj<T0>::PrepareSound(u32 soundId)
{
    if (!m_IsActive) {
        return NULL;
    }
    const SoundParam* param;
    s32 index = PrepareStart(soundId, true, &param);
    if (index == -1) {
        return NULL;
    }
    sead::SoundHandle* handle = &m_HandleSet.m_Handles[index];
    handle->Stop(0);
    SoundMgr::s_pInstance->PrepareSound(handle, soundId);
    ApplyToHandle(index, param);
    return handle;
}

template <auto T0>
void SoundObj<T0>::StopSounds(s32 fadeFrames)
{
    if (!m_IsActive) {
        return;
    }
    for (s32 i = 0; i < T0; i++) {
        if (m_HandleSet.m_Handles[i].m_pSound != NULL) {
            m_HandleSet.m_Handles[i].Stop(fadeFrames);
        }
    }
}

// 0x00820E1C
template bool SoundObj<1>::UpdateChanged();
// 0x00820E4C
template void SoundObj<1>::SetSoundVolume(f32 volume, s32 index);
// 0x00820F54
template sead::SoundHandle* SoundObj<1>::HoldSound(u32 soundId);
// 0x0082114C
template sead::SoundHandle* SoundObj<1>::HoldSound(u32 soundId, s16 value);
// 0x00821198
template void SoundObj<1>::SetPosition(const nn::math::VEC3* world, const nn::math::VEC3* relative);
// 0x00821210
template sead::SoundHandle* SoundObj<1>::StartSound(u32 soundId);
// 0x008213A4
template sead::SoundHandle* SoundObj<1>::StartSound(u32 soundId, s16 value);
// 0x0020B47C
template void SoundObj<1>::Stop();
// 0x008213F0
template sead::SoundHandle* SoundObj<1>::PrepareSound(u32 soundId);
// 0x00821574
template void SoundObj<1>::StopSounds(s32 fadeFrames);
// 0x008215E8
// 0x002D5FB4 (copy)
// 0x002D60BC (copy)
// 0x00250B1C (copy)
// 0x00821594 (deleting dtor)
template SoundObj<1>::~SoundObj();
// 0x00250ACC
template SoundObj<1>::SoundObj();

// 0x00821638
template bool SoundObj<2>::UpdateChanged();
// 0x00821668
template void SoundObj<2>::SetSoundVolume(f32 volume, s32 index);
// 0x00821678
template sead::SoundHandle* SoundObj<2>::HoldSound(u32 soundId);
// 0x0082191C
template sead::SoundHandle* SoundObj<2>::HoldSound(u32 soundId, s16 value);
// 0x00821968
template void SoundObj<2>::SetPosition(const nn::math::VEC3* world, const nn::math::VEC3* relative);
// 0x00821A38
template sead::SoundHandle* SoundObj<2>::StartSound(u32 soundId);
// 0x00821C40
template sead::SoundHandle* SoundObj<2>::StartSound(u32 soundId, s16 value);
// 0x00821C8C
template void SoundObj<2>::Stop();
// 0x00821CC4
template sead::SoundHandle* SoundObj<2>::PrepareSound(u32 soundId);
// 0x00821EC4
template void SoundObj<2>::StopSounds(s32 fadeFrames);
// 0x00821FC0
// 0x00821F2C (deleting dtor)
template SoundObj<2>::~SoundObj();

// 0x00822054
template bool SoundObj<3>::UpdateChanged();
// 0x00822084
template void SoundObj<3>::SetSoundVolume(f32 volume, s32 index);
// 0x008221F0
template sead::SoundHandle* SoundObj<3>::HoldSound(u32 soundId);
// 0x008224A4
template sead::SoundHandle* SoundObj<3>::HoldSound(u32 soundId, s16 value);
// 0x008224F0
template void SoundObj<3>::SetPosition(const nn::math::VEC3* world, const nn::math::VEC3* relative);
// 0x008225C0
template sead::SoundHandle* SoundObj<3>::StartSound(u32 soundId);
// 0x008227D0
template sead::SoundHandle* SoundObj<3>::StartSound(u32 soundId, s16 value);
// 0x0082281C
template void SoundObj<3>::Stop();
// 0x00822854
template sead::SoundHandle* SoundObj<3>::PrepareSound(u32 soundId);
// 0x00822A4C
template void SoundObj<3>::StopSounds(s32 fadeFrames);
// 0x00822B48
// 0x001C0D3C (copy)
// 0x00822AB4 (deleting dtor)
template SoundObj<3>::~SoundObj();

// 0x00822BDC
template bool SoundObj<4>::UpdateChanged();
// 0x00822C0C
template void SoundObj<4>::SetSoundVolume(f32 volume, s32 index);
// 0x00822D78
template sead::SoundHandle* SoundObj<4>::HoldSound(u32 soundId);
// 0x0082302C
template sead::SoundHandle* SoundObj<4>::HoldSound(u32 soundId, s16 value);
// 0x00823078
template void SoundObj<4>::SetPosition(const nn::math::VEC3* world, const nn::math::VEC3* relative);
// 0x00823148
template sead::SoundHandle* SoundObj<4>::StartSound(u32 soundId);
// 0x00823350
template sead::SoundHandle* SoundObj<4>::StartSound(u32 soundId, s16 value);
// 0x0082339C
template void SoundObj<4>::Stop();
// 0x008233D4
template sead::SoundHandle* SoundObj<4>::PrepareSound(u32 soundId);
// 0x008235D4
template void SoundObj<4>::StopSounds(s32 fadeFrames);
// 0x008236D0
// 0x0082363C (deleting dtor)
template SoundObj<4>::~SoundObj();

