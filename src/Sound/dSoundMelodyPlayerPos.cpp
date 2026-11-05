#include "Sound/dSoundMelodyPlayerPos.h"
#include "Sound/dSoundObjBase.h"
#include "nw/snd/internal/snd_BasicSound.h"

// 0x00322270
SoundMelodyPlayerPos::SoundMelodyPlayerPos() : m_pObj(NULL), m_ObjVolume(0.0f), m_ObjPan(0.0f), m_ObjSurroundPan(0.0f)
{
}

// 0x0013E56C (falls into ~SoundMelodyPlayerBase)
// 0x003222B0 (deleting dtor)
SoundMelodyPlayerPos::~SoundMelodyPlayerPos()
{
    // empty (in the original too)
}

inline void SoundMelodyPlayerPos::ApplyObj()
{
    if (m_pObj != NULL) {
        m_ObjVolume = m_pObj->m_Volume;
        m_ObjPan = m_pObj->m_Pan;
        m_ObjSurroundPan = m_pObj->m_SurroundPan;
    }
}

// 0x003221F8
void SoundMelodyPlayerPos::PlayNote(f32 frames)
{
    SoundMelodyPlayerBase::PlayNote(frames);
    ApplyObj();
    if (m_Handle.m_pSound != NULL) {
        m_Handle.m_pSound->SetVolume(m_ObjVolume * m_Volume, 0);
    }
    if (m_Handle.m_pSound != NULL) {
        m_Handle.m_pSound->SetPan(m_ObjPan);
    }
    if (m_Handle.m_pSound != NULL) {
        m_Handle.m_pSound->SetSurroundPan(m_ObjSurroundPan);
    }
}

// 0x0032C434
void SoundMelodyPlayerPos::Calc()
{
    if (m_Handle.m_pSound != NULL) {
        ApplyObj();
        m_Handle.m_pSound->SetVolume(m_ObjVolume * m_Volume, 0);
        if (m_Handle.m_pSound != NULL) {
            m_Handle.m_pSound->SetPan(m_ObjPan);
        }
        if (m_Handle.m_pSound != NULL) {
            m_Handle.m_pSound->SetSurroundPan(m_ObjSurroundPan);
        }
    }
    SoundMelodyPlayerBase::Calc();
}
