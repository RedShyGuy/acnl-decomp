#include "Sound/dSoundObjFieldObj.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "nw/snd/snd_SequenceSoundHandle.h"
#include "nw/snd/internal/snd_SequenceSound.h"

// 0x002BD5A4
SoundObjFieldObj::SoundObjFieldObj() : m_IsFrozen(false), m_FrozenPos(2000.0f, 2000.0f, 2000.0f)
{
    m_Priority = 7;
}

// 0x002BD670
// 0x002BD618 (deleting dtor)
SoundObjFieldObj::~SoundObjFieldObj()
{
    // (only the base is destroyed)
}

// 0x002BD3AC
void SoundObjFieldObj::Start(const nn::math::VEC3* world, const nn::math::VEC3* relative)
{
    if (!m_IsActive) {
        m_IsFaded = 0;
        SoundObjBase::Start(world, relative);
    }
}

// 0x002BD3C8
void SoundObjFieldObj::SetPosition(const nn::math::VEC3* world, const nn::math::VEC3* relative)
{
    if (!m_IsActive) {
        return;
    }
    SoundMgr* mgr = SoundMgr::s_pInstance;
    u8 state = mgr->m_FadeState;
    if (state == 0) {
        SoundObj<1>::SetPosition(world, relative);
        if (m_IsFrozen) {
            m_IsFrozen = false;
        }
    } else if (state == 1 || state == 2) {
        if (m_IsFrozen) {
            SoundObj<1>::SetPosition(world, &m_FrozenPos);
        } else {
            m_FrozenPos = *relative;
            m_IsFrozen = true;
            SoundObj<1>::SetPosition(world, relative);
        }
    } else if (state == 3) {
        s32 half = mgr->m_FadeFrames / 2;
        if (mgr->m_FadeTimer < half) {
            nn::math::VEC3 position;
            position.x = SoundMgr::Lerp(half, mgr->m_FadeFrames, 0, m_FrozenPos.x, relative->x);
            position.y = SoundMgr::Lerp(half, SoundMgr::s_pInstance->m_FadeFrames, 0, m_FrozenPos.y, relative->y);
            position.z = SoundMgr::Lerp(half, SoundMgr::s_pInstance->m_FadeFrames, 0, m_FrozenPos.z, relative->z);
            SoundObj<1>::SetPosition(world, &position);
        } else {
            SoundObj<1>::SetPosition(world, &m_FrozenPos);
        }
    }
}

// 0x002BD520
sead::SoundHandle* SoundObjFieldObj::StartSound(u32 soundId)
{
    if (soundId == SE_OBJ_BELL_JPN) {
        SetType(12);
    }
    return SoundObj<1>::StartSound(soundId);
}

// 0x002BD558
sead::SoundHandle* SoundObjFieldObj::StartSound(u32 soundId, s16 value)
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
