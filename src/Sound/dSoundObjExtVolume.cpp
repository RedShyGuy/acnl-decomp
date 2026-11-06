#include "Sound/dSoundObjExtVolume.h"

// 0x002D237C
SoundObjExtVolume::SoundObjExtVolume()
{
    // (only the members are constructed)
}

// 0x002D247C
// 0x002D23E8 (deleting dtor)
SoundObjExtVolume::~SoundObjExtVolume()
{
    // (only the members are destroyed)
}

// 0x002D2198
void SoundObjExtVolume::Start(const nn::math::VEC3* world, const nn::math::VEC3* relative)
{
    SoundObjBase::Start(world, relative);
    for (s32 i = 0; i < 2; i++) {
        m_Faders[i].Reset(1.0f);
    }
}

// 0x002D21E8
void SoundObjExtVolume::SetVolume(s32 index, f32 volume, s32 frames)
{
    m_Faders[index].SetTarget(volume, frames);
    if (frames == 0) {
        SetSoundVolume(m_Volume, index);
    }
}

// 0x002D2238
void SoundObjExtVolume::SetSoundVolume(f32 volume, s32 index)
{
    m_HandleSet.SetVolume(volume * m_Faders[index].GetValue(), index);
}

// 0x002D2260
void SoundObjExtVolume::SetPosition(const nn::math::VEC3* world, const nn::math::VEC3* relative)
{
    for (s32 i = 0; i < 2; i++) {
        m_Faders[i].Update();
    }
    SoundObj<2>::SetPosition(world, relative);
}
