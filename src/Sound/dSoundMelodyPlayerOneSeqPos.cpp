#include "Sound/dSoundMelodyPlayerOneSeqPos.h"
#include "Sound/dSoundObjBase.h"
#include "nw/snd/internal/snd_BasicSound.h"

// 0x00342BD8
SoundMelodyPlayerOneSeqPos::SoundMelodyPlayerOneSeqPos()
    : m_pObj(NULL), m_ObjVolume(0.0f), m_ObjPan(0.0f), m_ObjSurroundPan(0.0f)
{
}

// 0x00342C40
// 0x00342C18 (deleting dtor)
SoundMelodyPlayerOneSeqPos::~SoundMelodyPlayerOneSeqPos()
{
    // empty (in the original too)
}

// 0x00343424
void SoundMelodyPlayerOneSeqPos::Update()
{
    if (m_Handle.m_pSound != NULL) {
        if (m_pObj != NULL) {
            m_ObjVolume = m_pObj->m_Volume;
            m_ObjPan = m_pObj->m_Pan;
            m_ObjSurroundPan = m_pObj->m_SurroundPan;
        }
        m_Handle.m_pSound->SetVolume(m_ObjVolume, 0);
        if (m_Handle.m_pSound != NULL) {
            m_Handle.m_pSound->SetPan(m_ObjPan);
        }
        if (m_Handle.m_pSound != NULL) {
            m_Handle.m_pSound->SetSurroundPan(m_ObjSurroundPan);
        }
    }
    SoundMelodyPlayerOneSeqBase::Update();
}

// 0x00342B68
void SoundMelodyPlayerOneSeqPos::StartAtObj(u32 soundId, const u8* notes)
{
    Start(soundId, notes);
    if (m_pObj != NULL) {
        m_ObjVolume = m_pObj->m_Volume;
        m_ObjPan = m_pObj->m_Pan;
        m_ObjSurroundPan = m_pObj->m_SurroundPan;
    }
    if (m_Handle.m_pSound != NULL) {
        m_Handle.m_pSound->SetVolume(m_ObjVolume, 0);
    }
    m_Handle.SetPan(m_ObjPan);
    m_Handle.SetSurroundPan(m_ObjSurroundPan);
}

