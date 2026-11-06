#include "Sound/dSoundObjWithMelody.h"
#include "stage/stage_Api.h"

namespace {
const s32 TYPE_INDOOR = 10;
} // namespace

// 0x002E987C
SoundObjWithMelody::SoundObjWithMelody()
{
    // (only the members are constructed)
}

// 0x002E98F4
// 0x002E98E0 (deleting dtor)
SoundObjWithMelody::~SoundObjWithMelody()
{
    SoundObjWithMelody::Stop();
}

// 0x002E96E4
void SoundObjWithMelody::Start(const nn::math::VEC3* world, const nn::math::VEC3* relative)
{
    if (m_IsActive) {
        return;
    }
    if (stage::IsKind(stage::KIND_INDOOR, stage::GetCurrent())) {
        m_Type = TYPE_INDOOR;
    }
    SoundObjBase::Start(world, relative);
    m_Melody.m_pObj = this;
    m_DoorChime.m_pObj = this;
}

// 0x002E9740
void SoundObjWithMelody::PlayDoorChime(u32 soundId)
{
    PlayDoorChime(soundId, NULL);
}

// 0x002E9748
void SoundObjWithMelody::PlayDoorChime(u32 soundId, const u8* notes)
{
    if (m_IsActive) {
        m_DoorChime.StartAtObj(soundId, notes);
    }
}

// 0x002E9760
void SoundObjWithMelody::SetPosition(const nn::math::VEC3* world, const nn::math::VEC3* relative)
{
    if (!m_IsActive) {
        return;
    }
    if (m_Melody.m_State != 0 || m_DoorChime.m_State != 0) {
        UpdateChanged();
    }
    SoundObj<1>::SetPosition(world, relative);
    m_Melody.Calc();
    m_DoorChime.Update();
}

// 0x002E9834
void SoundObjWithMelody::Stop()
{
    if (!m_IsActive) {
        return;
    }
    m_Melody.m_pObj = NULL;
    m_Melody.Finalize();
    m_DoorChime.m_pObj = NULL;
    m_DoorChime.StopDoorChime();
    SoundObj<1>::Stop();
}
