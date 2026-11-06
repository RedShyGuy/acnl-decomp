#include "Sound/dSoundObjHold.h"
#include "Sound/dSoundMgr.h"
#include <cmath>

// 0x0020B83C
SoundObjHold::SoundObjHold() : m_SoundId(0xFFFFFFFF), m_IsFrozen(false), m_FrozenPos(2000.0f, 2000.0f, 2000.0f)
{
    m_Priority = 5;
}

// 0x0020B918
// 0x0020B8C4 (deleting dtor)
SoundObjHold::~SoundObjHold()
{
    // (only the base is destroyed)
}

// 0x0020B660
void SoundObjHold::Start(const nn::math::VEC3* world, const nn::math::VEC3* relative, u32 soundId)
{
    if (!m_IsActive) {
        m_IsFaded = 0;
        SoundObjBase::Start(world, relative);
        m_SoundId = soundId;
    }
}

// 0x0020B690
void SoundObjHold::SetPosition(const nn::math::VEC3* world, const nn::math::VEC3* relative)
{
    if (!m_IsActive || m_SoundId == 0xFFFFFFFF) {
        return;
    }
    const TypeParam& param = s_TypeParams[m_Type];
    if (fabsf(relative->x) > param.farDistance || fabsf(relative->y + param.positionOffsetZ) > param.farDistance) {
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
    SoundObj<1>::HoldSound(m_SoundId);
}
