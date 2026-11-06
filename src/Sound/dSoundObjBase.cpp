#include "Sound/dSoundObjBase.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "stage/stage_Api.h"
#include <cmath>

// (in the original near, far and the offset are static and the initializer builds the two VEC2 members: they have a
// constructor; GCC evaluates the constexpr constructor and puts the whole table into .data)
// 0x0094E0E8
SoundObjBase::TypeParam SoundObjBase::s_TypeParams[TYPE_NUM] = {
    {50.0f, 250.0f, nn::math::VEC2(0.0f, 0.0f), nn::math::VEC2(180.0f, 180.0f), 20.0f},
    {50.0f, 250.0f, nn::math::VEC2(0.0f, 0.0f), nn::math::VEC2(180.0f, 180.0f), 20.0f},
    {50.0f, 250.0f, nn::math::VEC2(0.0f, 0.0f), nn::math::VEC2(180.0f, 180.0f), 20.0f},
    {40.0f, 330.0f, nn::math::VEC2(20.0f, 20.0f), nn::math::VEC2(230.0f, 150.0f), 50.0f},
    {50.0f, 400.0f, nn::math::VEC2(0.0f, 0.0f), nn::math::VEC2(180.0f, 180.0f), 20.0f},
    {100.0f, 500.0f, nn::math::VEC2(0.0f, 0.0f), nn::math::VEC2(180.0f, 250.0f), 20.0f},
    {50.0f, 300.0f, nn::math::VEC2(0.0f, 0.0f), nn::math::VEC2(180.0f, 180.0f), 20.0f},
    {50.0f, 350.0f, nn::math::VEC2(0.0f, 0.0f), nn::math::VEC2(180.0f, 180.0f), 50.0f},
    {50.0f, 300.0f, nn::math::VEC2(0.0f, 0.0f), nn::math::VEC2(180.0f, 180.0f), 20.0f},
    {50.0f, 450.0f, nn::math::VEC2(20.0f, 20.0f), nn::math::VEC2(230.0f, 150.0f), 50.0f},
    {60.0f, 500.0f, nn::math::VEC2(20.0f, 20.0f), nn::math::VEC2(230.0f, 150.0f), 50.0f},
    {50.0f, 500.0f, nn::math::VEC2(0.0f, 0.0f), nn::math::VEC2(180.0f, 180.0f), 20.0f},
    {50.0f, 1000.0f, nn::math::VEC2(0.0f, 0.0f), nn::math::VEC2(500.0f, 500.0f), 20.0f},
    {50.0f, 1000.0f, nn::math::VEC2(0.0f, 0.0f), nn::math::VEC2(500.0f, 500.0f), 50.0f},
};

// the parameters read for a sound without parameters
// 0x0098501C
SoundParam SoundHandleSetBase::s_SoundParam;

namespace {
// the slopes of the curves (names are ours)
// 0x00985030
f32 s_VolumeSlopes[SoundObjBase::TYPE_NUM] = {
    1.0f / (SoundObjBase::s_TypeParams[0].nearDistance - SoundObjBase::s_TypeParams[0].farDistance),
    1.0f / (SoundObjBase::s_TypeParams[1].nearDistance - SoundObjBase::s_TypeParams[1].farDistance),
    1.0f / (SoundObjBase::s_TypeParams[2].nearDistance - SoundObjBase::s_TypeParams[2].farDistance),
    1.0f / (SoundObjBase::s_TypeParams[3].nearDistance - SoundObjBase::s_TypeParams[3].farDistance),
    1.0f / (SoundObjBase::s_TypeParams[4].nearDistance - SoundObjBase::s_TypeParams[4].farDistance),
    1.0f / (SoundObjBase::s_TypeParams[5].nearDistance - SoundObjBase::s_TypeParams[5].farDistance),
    1.0f / (SoundObjBase::s_TypeParams[6].nearDistance - SoundObjBase::s_TypeParams[6].farDistance),
    1.0f / (SoundObjBase::s_TypeParams[7].nearDistance - SoundObjBase::s_TypeParams[7].farDistance),
    1.0f / (SoundObjBase::s_TypeParams[8].nearDistance - SoundObjBase::s_TypeParams[8].farDistance),
    1.0f / (SoundObjBase::s_TypeParams[9].nearDistance - SoundObjBase::s_TypeParams[9].farDistance),
    1.0f / (SoundObjBase::s_TypeParams[10].nearDistance - SoundObjBase::s_TypeParams[10].farDistance),
    1.0f / (SoundObjBase::s_TypeParams[11].nearDistance - SoundObjBase::s_TypeParams[11].farDistance),
    1.0f / (SoundObjBase::s_TypeParams[12].nearDistance - SoundObjBase::s_TypeParams[12].farDistance),
    1.0f / (SoundObjBase::s_TypeParams[13].nearDistance - SoundObjBase::s_TypeParams[13].farDistance),
};
// 0x00985068
f32 s_PanSlopes[SoundObjBase::TYPE_NUM] = {
    1.0f / (SoundObjBase::s_TypeParams[0].outer.x - SoundObjBase::s_TypeParams[0].inner.x),
    1.0f / (SoundObjBase::s_TypeParams[1].outer.x - SoundObjBase::s_TypeParams[1].inner.x),
    1.0f / (SoundObjBase::s_TypeParams[2].outer.x - SoundObjBase::s_TypeParams[2].inner.x),
    1.0f / (SoundObjBase::s_TypeParams[3].outer.x - SoundObjBase::s_TypeParams[3].inner.x),
    1.0f / (SoundObjBase::s_TypeParams[4].outer.x - SoundObjBase::s_TypeParams[4].inner.x),
    1.0f / (SoundObjBase::s_TypeParams[5].outer.x - SoundObjBase::s_TypeParams[5].inner.x),
    1.0f / (SoundObjBase::s_TypeParams[6].outer.x - SoundObjBase::s_TypeParams[6].inner.x),
    1.0f / (SoundObjBase::s_TypeParams[7].outer.x - SoundObjBase::s_TypeParams[7].inner.x),
    1.0f / (SoundObjBase::s_TypeParams[8].outer.x - SoundObjBase::s_TypeParams[8].inner.x),
    1.0f / (SoundObjBase::s_TypeParams[9].outer.x - SoundObjBase::s_TypeParams[9].inner.x),
    1.0f / (SoundObjBase::s_TypeParams[10].outer.x - SoundObjBase::s_TypeParams[10].inner.x),
    1.0f / (SoundObjBase::s_TypeParams[11].outer.x - SoundObjBase::s_TypeParams[11].inner.x),
    1.0f / (SoundObjBase::s_TypeParams[12].outer.x - SoundObjBase::s_TypeParams[12].inner.x),
    1.0f / (SoundObjBase::s_TypeParams[13].outer.x - SoundObjBase::s_TypeParams[13].inner.x),
};
// 0x009850A0
f32 s_SurroundSlopes[SoundObjBase::TYPE_NUM] = {
    1.0f / (SoundObjBase::s_TypeParams[0].inner.y - SoundObjBase::s_TypeParams[0].outer.y),
    1.0f / (SoundObjBase::s_TypeParams[1].inner.y - SoundObjBase::s_TypeParams[1].outer.y),
    1.0f / (SoundObjBase::s_TypeParams[2].inner.y - SoundObjBase::s_TypeParams[2].outer.y),
    1.0f / (SoundObjBase::s_TypeParams[3].inner.y - SoundObjBase::s_TypeParams[3].outer.y),
    1.0f / (SoundObjBase::s_TypeParams[4].inner.y - SoundObjBase::s_TypeParams[4].outer.y),
    1.0f / (SoundObjBase::s_TypeParams[5].inner.y - SoundObjBase::s_TypeParams[5].outer.y),
    1.0f / (SoundObjBase::s_TypeParams[6].inner.y - SoundObjBase::s_TypeParams[6].outer.y),
    1.0f / (SoundObjBase::s_TypeParams[7].inner.y - SoundObjBase::s_TypeParams[7].outer.y),
    1.0f / (SoundObjBase::s_TypeParams[8].inner.y - SoundObjBase::s_TypeParams[8].outer.y),
    1.0f / (SoundObjBase::s_TypeParams[9].inner.y - SoundObjBase::s_TypeParams[9].outer.y),
    1.0f / (SoundObjBase::s_TypeParams[10].inner.y - SoundObjBase::s_TypeParams[10].outer.y),
    1.0f / (SoundObjBase::s_TypeParams[11].inner.y - SoundObjBase::s_TypeParams[11].outer.y),
    1.0f / (SoundObjBase::s_TypeParams[12].inner.y - SoundObjBase::s_TypeParams[12].outer.y),
    1.0f / (SoundObjBase::s_TypeParams[13].inner.y - SoundObjBase::s_TypeParams[13].outer.y),
};
// 0x009850D8
f32 s_SurroundBackOffsets[SoundObjBase::TYPE_NUM] = {
    2.0f * SoundObjBase::s_TypeParams[0].inner.y - SoundObjBase::s_TypeParams[0].outer.y,
    2.0f * SoundObjBase::s_TypeParams[1].inner.y - SoundObjBase::s_TypeParams[1].outer.y,
    2.0f * SoundObjBase::s_TypeParams[2].inner.y - SoundObjBase::s_TypeParams[2].outer.y,
    2.0f * SoundObjBase::s_TypeParams[3].inner.y - SoundObjBase::s_TypeParams[3].outer.y,
    2.0f * SoundObjBase::s_TypeParams[4].inner.y - SoundObjBase::s_TypeParams[4].outer.y,
    2.0f * SoundObjBase::s_TypeParams[5].inner.y - SoundObjBase::s_TypeParams[5].outer.y,
    2.0f * SoundObjBase::s_TypeParams[6].inner.y - SoundObjBase::s_TypeParams[6].outer.y,
    2.0f * SoundObjBase::s_TypeParams[7].inner.y - SoundObjBase::s_TypeParams[7].outer.y,
    2.0f * SoundObjBase::s_TypeParams[8].inner.y - SoundObjBase::s_TypeParams[8].outer.y,
    2.0f * SoundObjBase::s_TypeParams[9].inner.y - SoundObjBase::s_TypeParams[9].outer.y,
    2.0f * SoundObjBase::s_TypeParams[10].inner.y - SoundObjBase::s_TypeParams[10].outer.y,
    2.0f * SoundObjBase::s_TypeParams[11].inner.y - SoundObjBase::s_TypeParams[11].outer.y,
    2.0f * SoundObjBase::s_TypeParams[12].inner.y - SoundObjBase::s_TypeParams[12].outer.y,
    2.0f * SoundObjBase::s_TypeParams[13].inner.y - SoundObjBase::s_TypeParams[13].outer.y,
};
} // namespace
// (builds the slopes, s_SoundParam and in the original the VEC2 members of s_TypeParams)
// 0x00793288 (static initializer)

// 0x0020B4D4
SoundObjBase::SoundObjBase()
    : m_Priority(8), m_PosX(2000.0f), m_PosZ(2000.0f), m_WorldHeight(100.0f), m_VolumeScale(1.0f),
      m_DistanceVolume(0.0f), m_Volume(0.0f), m_Pan(0.0f), m_SurroundPan(1.0f), m_PlayerPriority(0), m_Distance(-1.0f),
      m_BackScale(1.0f), m_IsActive(false), m_IsIgnoreMaster(0), m_IsNoDistance(0), m_IsUseHeight(1),
      m_IsAlwaysInRange(false), m_IsFaded(1), m_Type(3), m_Counter(0)
{
}

// 0x0020B570
// 0x0020B56C (deleting dtor)
SoundObjBase::~SoundObjBase()
{
    // empty (in the original too)
}

// 0x0020AA88
void SoundObjBase::Start(const nn::math::VEC3*, const nn::math::VEC3* relative)
{
    if (m_IsActive) {
        return;
    }
    if (stage::IsKind(stage::KIND_WORLD_CURVE, stage::GetCurrent())) {
        if (m_Type == 3) {
            if (stage::GetCurrent() == stage::NAME_DOWNTOWN) {
                m_Type = 1;
            } else if (stage::IsKind(stage::KIND_EXHIBITION, stage::GetCurrent()) &&
                       stage::GetCurrent() != stage::NAME_EXHIBITION_ENTRANCE) {
                m_Type = 2;
            } else {
                m_Type = 0;
            }
        }
        m_BackScale = 1.7f;
    } else {
        if (stage::GetCurrent() == stage::NAME_MINI_GAME_0) {
            m_Type = 13;
        }
        m_BackScale = 1.0f;
    }
    m_PosX = relative->x;
    f32 z = relative->y + s_TypeParams[m_Type].positionOffsetZ;
    if (z >= 0.0f) {
        m_PosZ = z;
    } else {
        m_PosZ = m_BackScale * z;
    }
    if (m_IsUseHeight == 1) {
        OnEnterStage();
    }
    m_IsActive = true;
    Calc();
}

// 0x0020AC08
void SoundObjBase::SetType(u32 type)
{
    if (type < TYPE_NUM && m_Type != static_cast<s32>(type)) {
        m_Type = type;
        m_Counter++;
    }
}

// 0x0020AC30
void SoundObjBase::Calc()
{
    const TypeParam& param = s_TypeParams[m_Type];
    if (m_IsNoDistance == 0 && !m_IsAlwaysInRange) {
        if (fabsf(m_PosX) > param.farDistance || fabsf(m_PosZ) > param.farDistance) {
            m_DistanceVolume = 0.0f;
            m_Volume = 0.0f;
            m_Pan = 0.0f;
            m_SurroundPan = 0.0f;
            m_Distance = -1.0f;
            m_PlayerPriority = 32;
            return;
        }
    }
    SoundMgr* mgr = SoundMgr::s_pInstance;
    f32 heightVolume = 1.0f;
    if (m_IsUseHeight != 0 && m_IsNoDistance == 0) {
        heightVolume = mgr->GetHeightVolume(m_WorldHeight);
    }
    f32 x = m_PosX;
    f32 z = m_PosZ;
    f32 distance = sqrtf(x * x + z * z);
    f32 pan;
    f32 surround;
    f32 surroundLow;
    f32 surroundHigh;
    if (x >= 0.0f) {
        pan = s_PanSlopes[m_Type] * (x - param.inner.x);
    } else {
        pan = s_PanSlopes[m_Type] * (x + param.inner.x);
    }
    if (z >= 0.0f) {
        surround = s_SurroundSlopes[m_Type] * (z - param.outer.y);
        surroundLow = 0.0f;
        surroundHigh = 1.0f;
    } else {
        surround = s_SurroundSlopes[m_Type] * (z + s_SurroundBackOffsets[m_Type]);
        surroundLow = 1.0f;
        surroundHigh = 2.0f;
    }
    if (m_IsNoDistance != 0) {
        m_DistanceVolume = 1.0f;
    } else {
        f32 over = distance - param.farDistance;
        if (z >= 0.0f && mgr->m_FadeState != 0 && m_IsFaded != 0 && z > 0.0f) {
            f32 fade = mgr->m_Fader.GetValue();
            f32 scale = ((1.0f - fade) * z + fade * 10.0f - 50.0f) * -0.025f;
            if (scale < fade) {
                scale = fade;
            } else if (scale > 1.0f) {
                scale = 1.0f;
            }
            over *= scale;
        }
        f32 volume = s_VolumeSlopes[m_Type] * over;
        if (volume > 1.0f) {
            volume = 1.0f;
        }
        m_DistanceVolume = volume;
    }
    if (x >= 0.0f) {
        if (pan < 0.0f) {
            pan = 0.0f;
        } else if (pan > 1.0f) {
            pan = 1.0f;
        }
    } else {
        if (pan < -1.0f) {
            pan = -1.0f;
        } else if (pan > 0.0f) {
            pan = 0.0f;
        }
    }
    m_Pan = pan;
    if (surround < surroundLow) {
        surround = surroundLow;
    } else if (surround > surroundHigh) {
        surround = surroundHigh;
    }
    m_SurroundPan = surround;
    f32 volume = m_DistanceVolume * m_VolumeScale;
    if (m_IsIgnoreMaster == 0) {
        volume *= mgr->m_VolumeMgr.m_MasterFader.GetValue() * heightVolume;
    } else {
        volume *= heightVolume;
    }
    if (volume < 0.0f) {
        volume = 0.0f;
    }
    m_Volume = volume;
    m_Distance = distance;
    m_PlayerPriority = SoundMgr::LerpInt(32, 0, m_DistanceVolume, 0.0f, 1.0f);
}

// 0x0020B264
bool SoundObjBase::UpdateChanged()
{
    return false;
}

// 0x0020B270
void SoundObjBase::SetTypeEntry11()
{
    m_Type = 11;
}

// 0x0020B280
void SoundObjBase::SetIsUseHeight(bool isUseHeight)
{
    if (m_IsUseHeight != isUseHeight) {
        m_IsUseHeight = isUseHeight;
        Calc();
    }
}

// 0x0020B2A4
void SoundObjBase::SetIsNoDistance(bool isNoDistance)
{
    if (m_IsNoDistance != isNoDistance) {
        m_IsNoDistance = isNoDistance;
        Calc();
    }
}

// 0x0020B2C8
void SoundObjBase::SetIsIgnoreMaster(bool isIgnoreMaster)
{
    if (stage::GetCurrent() == stage::NAME_TITLE) {
        isIgnoreMaster = false;
    }
    if (m_IsIgnoreMaster != isIgnoreMaster) {
        m_IsIgnoreMaster = isIgnoreMaster;
        Calc();
    }
}

// 0x0020B308
void SoundObjBase::OnEnterStage()
{
    if (stage::IsKind(stage::KIND_VILLAGE, stage::GetCurrent()) && stage::GetCurrent() != stage::NAME_TITLE) {
        m_IsUseHeight = 1;
    } else {
        m_IsUseHeight = 0;
    }
    if (stage::GetCurrent() == stage::NAME_MINI_GAME_0 && m_Type == 3) {
        m_Type = 13;
    } else {
        m_Type = 3;
    }
}

// 0x0020B364
void SoundObjBase::UpdateTrainFade(u32 soundId)
{
    switch (soundId) {
    case SE_OBJ_TRAIN_WAIT:
    case SE_OBJ_TRAIN_RUN:
    case SE_OBJ_TRAIN_RUN_TUNNEL:
    case SE_OBJ_TRAIN_RUN_BRIDGE: {
        const nn::math::VEC3* listener = SoundMgr::s_pInstance->m_pListenerPos;
        if (listener == NULL || listener->y > 40.0f) {
            m_IsFaded = 1;
        } else {
            m_IsFaded = 0;
        }
        break;
    }
    }
}

// 0x0020B3C8
void SoundObjBase::SetIsFaded(bool isFaded)
{
    if (m_IsFaded != isFaded) {
        m_IsFaded = isFaded;
        Calc();
    }
}

// 0x0020B3E8
void SoundObjBase::SetPosition(const nn::math::VEC3* world, const nn::math::VEC3* relative)
{
    m_PosX = relative->x;
    f32 z = relative->y + s_TypeParams[m_Type].positionOffsetZ;
    if (z >= 0.0f) {
        m_PosZ = z;
    } else {
        m_PosZ = m_BackScale * z;
    }
    m_WorldHeight = world->y;
}

// 0x0020B444
void SoundObjBase::BindBgm()
{
    SoundMgr::s_pInstance->m_BgmMgr.m_pObj = this;
    m_IsAlwaysInRange = true;
    m_IsFaded = 0;
}

// 0x0020B498
void SoundObjBase::Stop()
{
    if (m_IsActive) {
        SoundMgr::s_pInstance->m_BgmMgr.ClearObj(this);
        m_IsActive = false;
    }
}
