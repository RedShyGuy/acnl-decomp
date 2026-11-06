#include "Audio/dAudioObjFurnitureInst.h"
#include "Sound/dSoundMgr.h"
#include "Sound/sound_Extern.h"
#include "sead/seadNew.h"

// 0x00325160
AudioObjFurnitureBase* AudioObjFurnitureInst::Create(const SoundCreateArg* arg)
{
    return new (arg->heap, 4) AudioObjFurnitureInst;
}

// 0x0032517C
AudioObjFurnitureInst::AudioObjFurnitureInst() : AudioObjFurnitureBase(2)
{
}

// 0x003251CC
// 0x003251A8 (deleting dtor)
AudioObjFurnitureInst::~AudioObjFurnitureInst()
{
    // (only the members are destroyed)
}

// 0x00325000
void AudioObjFurnitureInst::Start(const nn::math::VEC3* world, u32 kind, bool isOn)
{
    nn::math::VEC3 relative;
    Func1E5E50(&relative, world);
    m_Obj.Start(world, &relative, kind, isOn);
    m_IsStarted = true;
}

// 0x003250E8
void AudioObjFurnitureInst::Update(const nn::math::VEC3* world, f32 frame)
{
    nn::math::VEC3 relative;
    Func1E5E50(&relative, world);
    m_Obj.Update(world, &relative, frame);
}

// 0x00325130
void AudioObjFurnitureInst::Stop()
{
    if (m_IsStarted) {
        m_Obj.Stop();
        m_IsStarted = false;
    }
}

// 0x003250D0
void AudioObjFurnitureInst::SetIsIgnoreMaster(bool isIgnoreMaster)
{
    m_Obj.SetIsIgnoreMaster(isIgnoreMaster);
}

// 0x003250D8
s8 AudioObjFurnitureInst::IsIgnoreMaster()
{
    return m_Obj.m_IsIgnoreMaster;
}

// 0x0032508C
void AudioObjFurnitureInst::SetVolumeScale(f32 scale)
{
    m_Obj.m_VolumeScale = scale;
}

// 0x00325094
bool AudioObjFurnitureInst::IsPlaying()
{
    for (s32 i = 0; i < 2; i++) {
        if (m_Obj.m_HandleSet.m_Handles[i].m_pSound != NULL) {
            return true;
        }
    }
    return false;
}

// 0x00325050
bool AudioObjFurnitureInst::IsPlaying(u32 soundId)
{
    return m_Obj.m_HandleSet.m_Handles[0].GetId() == soundId || m_Obj.m_HandleSet.m_Handles[1].GetId() == soundId;
}

// 0x00325128
void AudioObjFurnitureInst::StartSound(u32 soundId)
{
    m_Obj.StartSound(soundId);
}

// 0x00325158
void AudioObjFurnitureInst::TurnOn()
{
    m_Obj.TurnOn();
}

// 0x00325048
void AudioObjFurnitureInst::TurnOff()
{
    m_Obj.TurnOff();
}

// 0x003250E0
void AudioObjFurnitureInst::HoldSound(u32 soundId)
{
    m_Obj.HoldSound(soundId);
}

// 0x0032CCB8
void AudioObjFurnitureInst::Prepare()
{
    m_Obj.InitializeInstrument();
}

// 0x0032CD4C
u8 AudioObjFurnitureInst::PlayNote()
{
    return m_Obj.PlayNote();
}

// 0x0032CCEC
void AudioObjFurnitureInst::PlayNoteAt(s16 note)
{
    m_Obj.PlayNoteAt(note);
}
