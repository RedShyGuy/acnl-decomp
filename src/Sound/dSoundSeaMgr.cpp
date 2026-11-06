#include "Sound/dSoundSeaMgr.h"
#include "Sound/dSoundSeaMgr_SingletonDisposer_.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "nw/snd/internal/snd_BasicSound.h"
#include "sead/seadNew.h"
#include "stage/stage_Api.h"
#include "time/time_Api.h"
#include <cmath>

// (the VEC2/VEC3 tables are built by the static initializer in the original: they have a constructor; GCC evaluates the
// constexpr constructor and puts them into .data)
// 0x0079178C (static initializer)

namespace {
// the village: where the sound starts and how far it is to full volume; [0..1] the sea, [2..3] the wind on the cliff,
// each per ocean side
// 0x00985110
const nn::math::VEC2 s_VillageStart[4] = {
    nn::math::VEC2(1300.0f, 2000.0f),
    nn::math::VEC2(2284.0f, 2000.0f),
    nn::math::VEC2(1300.0f, 2000.0f),
    nn::math::VEC2(2284.0f, 2000.0f),
};
// 0x00985130
const nn::math::VEC2 s_VillageFade[4] = {
    nn::math::VEC2(550.0f, 400.0f),
    nn::math::VEC2(-550.0f, 400.0f),
    nn::math::VEC2(550.0f, 300.0f),
    nn::math::VEC2(-550.0f, 300.0f),
};
// the positions of full and of no panning, per ocean side
// 0x00985150
const nn::math::VEC2 s_VillagePanFull[2] = {
    nn::math::VEC2(900.0f, 2100.0f),
    nn::math::VEC2(2684.0f, 2100.0f),
};
// 0x00985160
const nn::math::VEC2 s_VillagePanNone[2] = {
    nn::math::VEC2(550.0f, 2500.0f),
    nn::math::VEC2(3034.0f, 2500.0f),
};
// the same for TYPE_TOUR_2, per side of the center
// 0x00985170
const nn::math::VEC2 s_Tour2Start[2] = {
    nn::math::VEC2(1200.0f, 1200.0f),
    nn::math::VEC2(2384.0f, 1872.0f),
};
// 0x00985180
const nn::math::VEC2 s_Tour2Fade[2] = {
    nn::math::VEC2(550.0f, 550.0f),
    nn::math::VEC2(-550.0f, -550.0f),
};
// 0x00985190
const nn::math::VEC2 s_Tour2PanFull[2] = {
    nn::math::VEC2(800.0f, 900.0f),
    nn::math::VEC2(2784.0f, 2172.0f),
};
// 0x009851A0
const nn::math::VEC2 s_Tour2PanNone[2] = {
    nn::math::VEC2(500.0f, 500.0f),
    nn::math::VEC2(3084.0f, 2572.0f),
};
// 0x009851B0
const nn::math::VEC3 s_Tour2Center(1792.0f, 32.0f, 1536.0f);
// 0x009851BC
const nn::math::VEC3 s_IslandCenter(1024.0f, 32.0f, 1024.0f);

// TYPE_DOWNTOWN: [ocean side] the sea, [2 + ocean side] the wind on the cliff
// 0x0083ED68
const f32 s_DownTownPanFull[2] = {400.0f, 1648.0f};
// 0x0083ED70
const f32 s_DownTownPanNone[2] = {-100.0f, 2148.0f};
// 0x0083EDA0
const f32 s_DownTownSilent[4] = {700.0f, 1348.0f, 700.0f, 1348.0f};
// 0x0083EDB0
const f32 s_DownTownFull[4] = {0.0f, 2048.0f, 0.0f, 2048.0f};

const f32 ISLAND_MIN_VOLUME = 0.4f;
const f32 TOUR_1_START = 1900.0f;
const f32 TOUR_1_END = 2900.0f;
const f32 TOUR_1_FADE = 500.0f;
// above this height the time chime does not sound distant
const f32 HEIGHT_OF_CLIFF = 85.0f;

inline f32 Clamp(f32 value, f32 min, f32 max)
{
    if (value < min) {
        return min;
    }
    if (value > max) {
        return max;
    }
    return value;
}

inline void SetHandleVolume(nw::snd::SoundHandle& handle, f32 volume)
{
    if (handle.m_pSound != NULL) {
        handle.m_pSound->SetVolume(volume, 0);
    }
}

// no wind at these hours
inline bool IsWindHour()
{
    s8 hour = time::GetCurrentDate()->hour;
    if (hour == 7 || hour == 8) {
        return false;
    }
    if (hour == 17 || hour == 18) {
        return false;
    }
    return true;
}
} // namespace

// 0x0094E560
SoundSeaMgr* SoundSeaMgr::s_pInstance = NULL;
// 0x0094E564
SoundSeaMgr::SingletonDisposer_* SoundSeaMgr::s_pSingletonDisposer = NULL;
// 0x0094E568
nn::math::VEC2 SoundSeaMgr::s_Unused(3584.0f, 3072.0f);

SoundSeaMgr::SoundSeaMgr()
    : m_Type(TYPE_VILLAGE), m_IsActive(false), m_SoundId(0xFFFFFFFF), m_pListenerPos(NULL), m_OceanSide(0)
{
}

// 0x001DA7C4
SoundSeaMgr* SoundSeaMgr::createInstance(sead::Heap* heap)
{
    if (s_pInstance == NULL) {
        u8* buffer = new (heap, 4) u8[sizeof(SoundSeaMgr)];
        SoundSeaMgr* instance = NULL;
        if (buffer == NULL) {
            s_pSingletonDisposer = NULL;
        } else {
            s_pSingletonDisposer = new (buffer) SingletonDisposer_(heap);
            instance = new (buffer) SoundSeaMgr();
        }
        s_pInstance = instance;
    }
    return s_pInstance;
}

// 0x00123500
void SoundSeaMgr::destroyInstance()
{
    if (s_pSingletonDisposer != NULL) {
        SingletonDisposer_* disposer = s_pSingletonDisposer;
        s_pSingletonDisposer = NULL;
        disposer->~SingletonDisposer_();
        if (s_pInstance != NULL) {
            s_pInstance->~SoundSeaMgr();
            ::operator delete(s_pInstance);
        }
        s_pInstance = NULL;
    }
}

// 0x001DA7BC
void SoundSeaMgr::SetType(u8 type)
{
    m_Type = type;
}

// 0x001DA858
void SoundSeaMgr::SetOceanSide(bool side)
{
    m_OceanSide = side;
    SoundMgr::s_pInstance->SetFlagCC5(side);
}

// 0x001DA92C
void SoundSeaMgr::CalcTour2()
{
    if (m_pListenerPos == NULL) {
        return;
    }
    f32 x = m_pListenerPos->x;
    f32 z = m_pListenerPos->z;
    int sideX = x > s_Tour2Center.x ? 1 : 0;
    int sideZ = z > s_Tour2Center.z ? 1 : 0;

    f32 volumeX = Clamp((s_Tour2Start[sideX].x - x) / s_Tour2Fade[sideX].x, 0.0f, 1.0f);
    f32 volumeZ = Clamp((s_Tour2Start[sideZ].y - z) / s_Tour2Fade[sideZ].y, 0.0f, 1.0f);
    f32 sum = volumeX + volumeZ;
    if (sum == 0.0f) {
        return;
    }

    f32 panZ;
    if (sideZ != 0) {
        panZ = Clamp((z - s_Tour2PanNone[sideZ].y) / (s_Tour2PanFull[sideZ].y - s_Tour2PanNone[sideZ].y), 0.0f, 1.0f);
    } else {
        panZ = Clamp((s_Tour2PanNone[0].y - z) / (s_Tour2PanFull[0].y - s_Tour2PanNone[0].y), -1.0f, 0.0f);
    }
    f32 panX;
    if (sideX != 0) {
        panX = Clamp((x - s_Tour2PanNone[sideX].x) / (s_Tour2PanFull[sideX].x - s_Tour2PanNone[sideX].x), 0.0f, 1.0f);
    } else {
        panX = Clamp((x - s_Tour2PanNone[0].x) / (s_Tour2PanNone[0].x - s_Tour2PanFull[0].x), -1.0f, 0.0f);
    }

    f32 volume = volumeX > volumeZ ? volumeX : volumeZ;
    f32 surroundPan = sideZ != 0 ? Clamp(panZ * volumeZ / sum, 0.0f, 1.0f) : Clamp(panZ * volumeZ / sum, -1.0f, 0.0f);
    f32 pan = sideX != 0 ? Clamp(panX * volumeX / sum, 0.0f, 1.0f) : Clamp(panX * volumeX / sum, -1.0f, 0.0f);

    SoundMgr::s_pInstance->HoldSound(&m_Handles[0], m_SoundId);
    SetHandleVolume(m_Handles[0], volume);
    m_Handles[0].SetPan(pan);
    m_Handles[0].SetSurroundPan(surroundPan + 1.0f);
}

// 0x001DAC54
void SoundSeaMgr::CalcIsland()
{
    if (m_pListenerPos == NULL) {
        return;
    }
    f32 x = m_pListenerPos->x - s_IslandCenter.x;
    f32 y = m_pListenerPos->y - s_IslandCenter.y;
    f32 z = m_pListenerPos->z - s_IslandCenter.z;
    f32 distance = sqrtf(x * x + y * y + z * z);
    f32 volume = Clamp(ISLAND_MIN_VOLUME + distance * 0.6f * (1.0f / 300.0f), ISLAND_MIN_VOLUME, 1.0f);

    SoundMgr::s_pInstance->HoldSound(&m_Handles[0], m_SoundId);
    SetHandleVolume(m_Handles[0], volume);
    m_Handles[0].SetSurroundPan(1.0f);
}

// 0x001DAD38
void SoundSeaMgr::CalcVillage()
{
    if (m_pListenerPos == NULL) {
        return;
    }
    SoundMgr* mgr = SoundMgr::s_pInstance;
    int side = m_OceanSide;
    f32 x = m_pListenerPos->x;
    f32 z = m_pListenerPos->z;
    f32 volumeX = Clamp((s_VillageStart[side].x - x) / s_VillageFade[side].x, 0.0f, 1.0f);
    f32 volumeZ = Clamp((s_VillageStart[side].y - z) / -s_VillageFade[side].y, 0.0f, 1.0f);
    f32 sum = volumeX + volumeZ;
    if (sum != 0.0f) {
        f32 panZ = Clamp((z - s_VillagePanNone[side].y) / (s_VillagePanFull[side].y - s_VillagePanNone[side].y), 0.0f, 1.0f);
        f32 panX;
        if (side != 0) {
            panX = Clamp((x - s_VillagePanNone[side].x) / (s_VillagePanFull[side].x - s_VillagePanNone[side].x), 0.0f, 1.0f);
        } else {
            panX = Clamp((x - s_VillagePanNone[0].x) / (s_VillagePanNone[0].x - s_VillagePanFull[0].x), -1.0f, 0.0f);
        }
        f32 volume = volumeX > volumeZ ? volumeX : volumeZ;
        f32 surroundPan = Clamp(panZ * volumeZ / sum, 0.0f, 1.0f);
        f32 pan = side != 0 ? Clamp(panX * volumeX / sum, 0.0f, 1.0f) : Clamp(panX * volumeX / sum, -1.0f, 0.0f);

        mgr->HoldSound(&m_Handles[0], m_SoundId);
        SetHandleVolume(m_Handles[0], volume);
        m_Handles[0].SetPan(pan);
        m_Handles[0].SetSurroundPan(surroundPan + 1.0f);
    }

    SoundMelodyPlayerOneSeqTimeTone& chime = mgr->m_MelodyMgr.m_TimeChime;
    if (m_pListenerPos->y > HEIGHT_OF_CLIFF) {
        if (IsWindHour()) {
            int wind = m_OceanSide + 2;
            f32 windX = Clamp((s_VillageStart[wind].x - m_pListenerPos->x) / s_VillageFade[wind].x, 0.0f, 1.0f);
            f32 windZ = Clamp((s_VillageStart[wind].y - m_pListenerPos->z) / -s_VillageFade[wind].y, 0.0f, 1.0f);
            f32 volume = windX > windZ ? windX : windZ;
            if (volume > 0.0f) {
                mgr->HoldSound(&m_Handles[1], SE_ENV_CLIFF_WIND);
                SetHandleVolume(m_Handles[1], volume);
            }
        }
        if (chime.m_IsDistant == 1) {
            chime.SetDistant(false);
        }
    } else if (chime.m_IsDistant == 0) {
        chime.SetDistant(true);
    }
}

// 0x001DB0F8
void SoundSeaMgr::CalcDownTown()
{
    if (m_pListenerPos == NULL) {
        return;
    }
    int side = m_OceanSide;
    f32 x = m_pListenerPos->x;
    f32 volume = Clamp((x - s_DownTownSilent[side]) / (s_DownTownFull[side] - s_DownTownSilent[side]), 0.0f, 1.0f);
    if (volume > 0.0f) {
        f32 pan;
        if (side != 0) {
            pan = Clamp((x - s_DownTownPanNone[side]) / (s_DownTownPanFull[side] - s_DownTownPanNone[side]), 0.0f, 1.0f);
        } else {
            pan = Clamp((x - s_DownTownPanNone[0]) / (s_DownTownPanNone[0] - s_DownTownPanFull[0]), -1.0f, 0.0f);
        }
        SoundMgr::s_pInstance->HoldSound(&m_Handles[0], m_SoundId);
        SetHandleVolume(m_Handles[0], volume);
        m_Handles[0].SetPan(pan);
        m_Handles[0].SetSurroundPan(1.0f);
    }

    if (IsWindHour()) {
        int wind = m_OceanSide + 2;
        f32 start = s_DownTownSilent[wind];
        f32 windVolume = Clamp((m_pListenerPos->x - start) / (s_DownTownFull[wind] - start), 0.0f, 1.0f);
        if (windVolume > 0.0f) {
            SoundMgr::s_pInstance->HoldSound(&m_Handles[1], SE_ENV_CLIFF_WIND);
            SetHandleVolume(m_Handles[1], windVolume);
        }
    }
}

// 0x001DB314
void SoundSeaMgr::CalcTour1()
{
    if (m_pListenerPos == NULL) {
        return;
    }
    f32 z = m_pListenerPos->z;
    if (z > TOUR_1_START) {
        f32 volume = Clamp((z - TOUR_1_START) / TOUR_1_FADE, 0.0f, 1.0f);
        f32 surroundPan = Clamp((TOUR_1_END - z) / TOUR_1_FADE, 1.0f, 2.0f);
        SoundMgr::s_pInstance->HoldSound(&m_Handles[0], m_SoundId);
        SetHandleVolume(m_Handles[0], volume);
        m_Handles[0].SetSurroundPan(surroundPan);
    }
}

// 0x001DB418
void SoundSeaMgr::Update()
{
    if (!SoundMgr::s_pInstance->IsPaused() && m_IsActive) {
        (this->*s_CalcFuncs[m_Type])();
    }
}

// 0x001DB480
void SoundSeaMgr::OnEnterStage()
{
    SoundMgr* mgr = SoundMgr::s_pInstance;
    u8 type = m_Type;
    u32 soundId = SE_ENV_SEA;
    bool isNight = mgr->IsNight();
    switch (type) {
    case TYPE_VILLAGE:
        if (stage::IsKind(stage::KIND_TOUR, stage::GetCurrent()) && !mgr->m_EnvMgr.IsRaining() && !isNight) {
            soundId = SE_ENV_SEA_KAMOME;
        }
        break;
    case TYPE_TOUR_1:
    case TYPE_TOUR_2:
        if (!isNight) {
            soundId = SE_ENV_SEA_KAMOME;
        }
        break;
    case TYPE_ISLAND:
        soundId = SE_ENV_SEA_SUR;
        break;
    }
    m_SoundId = soundId;
    m_IsActive = true;
}
