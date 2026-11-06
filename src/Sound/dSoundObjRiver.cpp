#include "Sound/dSoundObjRiver.h"
#include "Sound/dSoundMgr.h"
#include "Sound/dSoundRiverMgr.h"
#include <cmath>

namespace {
// farther away the object does not count
const f32 MAX_DISTANCE = 300.0f;
} // namespace

// 0x00247190
SoundObjRiver::SoundObjRiver()
    : m_Index(0xFFFFFFFF), m_IsInRange(false), m_IsFrozen(false), m_FrozenPos(2000.0f, 2000.0f, 2000.0f)
{
}

// 0x00247224
// 0x002471D0 (deleting dtor)
SoundObjRiver::~SoundObjRiver()
{
    SoundObjRiver::Stop();
}

// 0x00246EF0
void SoundObjRiver::Start(const nn::math::VEC3* world, const nn::math::VEC3* relative)
{
    if (m_IsActive || SoundRiverMgr::s_pInstance == NULL) {
        return;
    }
    m_Index = SoundRiverMgr::s_pInstance->Register(this);
    m_IsInRange = false;
    m_Type = 7;
    m_IsIgnoreMaster = 1;
    m_IsUseHeight = 0;
    m_IsFaded = 0;
    SoundObjBase::Start(world, relative);
}

// 0x00246F68
bool SoundObjRiver::UpdateChanged()
{
    Calc();
    return true;
}

// 0x00246F80
void SoundObjRiver::SetPosition(const nn::math::VEC3* world, const nn::math::VEC3* relative)
{
    if (!m_IsActive) {
        return;
    }
    m_IsInRange = false;
    if (fabsf(relative->x) > MAX_DISTANCE ||
        fabsf(relative->y + s_TypeParams[m_Type].positionOffsetZ) > MAX_DISTANCE) {
        return;
    }
    m_IsInRange = true;

    SoundMgr* mgr = SoundMgr::s_pInstance;
    u8 state = mgr->m_FadeState;
    if (state == 0) {
        SoundObjBase::SetPosition(world, relative);
        if (m_IsFrozen) {
            m_IsFrozen = false;
        }
    } else if (state == 1 || state == 2) {
        if (m_IsFrozen) {
            SoundObjBase::SetPosition(world, &m_FrozenPos);
        } else {
            m_FrozenPos = *relative;
            m_IsFrozen = true;
            SoundObjBase::SetPosition(world, relative);
        }
    } else if (state == 3) {
        s32 half = mgr->m_FadeFrames / 2;
        if (mgr->m_FadeTimer < half) {
            nn::math::VEC3 position;
            position.x = SoundMgr::Lerp(half, mgr->m_FadeFrames, 0, m_FrozenPos.x, relative->x);
            position.y = SoundMgr::Lerp(half, SoundMgr::s_pInstance->m_FadeFrames, 0, m_FrozenPos.y, relative->y);
            position.z = SoundMgr::Lerp(half, SoundMgr::s_pInstance->m_FadeFrames, 0, m_FrozenPos.z, relative->z);
            SoundObjBase::SetPosition(world, &position);
        } else {
            SoundObjBase::SetPosition(world, &m_FrozenPos);
        }
    }
    UpdateChanged();
}

// 0x0024714C
void SoundObjRiver::Stop()
{
    if (!m_IsActive) {
        return;
    }
    if (m_Index != 0xFFFFFFFF && SoundRiverMgr::s_pInstance != NULL) {
        SoundRiverMgr::s_pInstance->Unregister(m_Index);
    }
    SoundObjBase::Stop();
}
