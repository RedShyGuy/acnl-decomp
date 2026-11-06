#include "Sound/dSoundObjFurnitureInst.h"

// 0x0032CD78
SoundObjFurnitureInst::SoundObjFurnitureInst()
{
    // (only the members are constructed)
}

// 0x0032CDF0
// 0x0032CD98 (deleting dtor)
SoundObjFurnitureInst::~SoundObjFurnitureInst()
{
    SoundObjFurnitureInst::Stop();
}

// 0x0032CCC0
void SoundObjFurnitureInst::InitializeInstrument()
{
    if (m_Instrument.m_pTracker == NULL) {
        m_Instrument.Initialize(!((m_pInfo->flags >> 1) & 1), this);
    }
}

// 0x0032CCF4
void SoundObjFurnitureInst::PlayNoteAt(s16 note)
{
    if (m_pInfo != NULL && m_pInfo->soundOn != 0) {
        m_Instrument.PlayNoteAt(m_pInfo->soundOn, note);
    }
}

// 0x0032CD18
void SoundObjFurnitureInst::Stop()
{
    if (!m_IsActive) {
        return;
    }
    if (m_Instrument.m_pTracker != NULL) {
        m_Instrument.Unregister();
    }
    SoundObjFurniture::Stop();
}

// 0x0032CD54
u8 SoundObjFurnitureInst::PlayNote()
{
    if (m_pInfo != NULL && m_pInfo->soundOn != 0) {
        return m_Instrument.Play(m_pInfo->soundOn);
    }
    return 0;
}

// 0x002E9DAC
void SoundObjFurnitureInst::Update(const nn::math::VEC3* world, const nn::math::VEC3* relative, f32 frame)
{
    SoundObjFurniture::Update(world, relative, frame);
    m_Instrument.Update();
}
