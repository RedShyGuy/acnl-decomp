#include "Sound/dSoundObjSymbolTree.h"
#include "Sound/dSoundFxMgr.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "Sound/sound_Extern.h"
#include "stage/stage_Api.h"

namespace {
const s32 TYPE_SYMBOL_TREE = 8;
const f32 NEAR_DISTANCE = 100.0f;
const u32 NEAR_FRAMES = 150;
const f32 FX_VOLUME = 0.05f;

inline u32 GetSoundId(s8 sound)
{
    return sound >= 0 ? sound + SE_ENV_SYMBOL_TREE_1 : SE_SYS_SILENT;
}
} // namespace

// the number of levels of each size
// 0x0083ED28
const u8 SoundObjSymbolTree::s_LevelNums[SIZE_NUM] = {1, 1, 1, 4, 5, 5, 6, 7};
// 0x0083ED30
const s8 SoundObjSymbolTree::s_Sounds[SIZE_NUM][LEVEL_NUM] = {
    {-1, -1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1, -1}, {2, 2, 3, -1, -1, -1, -1},
    {1, 2, 2, 3, -1, -1, -1},     {1, 1, 2, 3, -1, -1, -1},     {0, 1, 1, 2, 3, -1, -1},     {0, 0, 1, 1, 2, 3, -1},
};

// 0x002E9610
SoundObjSymbolTree::SoundObjSymbolTree() : m_IsNear(false), m_NearFrames(0), m_Sound(-1), m_Size(0)
{
}

// 0x002E9694
// 0x002E9640 (deleting dtor)
SoundObjSymbolTree::~SoundObjSymbolTree()
{
    // (only the base is destroyed)
}

// 0x002E93C8
void SoundObjSymbolTree::Start(const nn::math::VEC3* world, const nn::math::VEC3* relative)
{
    m_Type = TYPE_SYMBOL_TREE;
    SoundObjHold::Start(world, relative, GetSoundId(m_Sound));
}

// 0x002E93F0
void SoundObjSymbolTree::SetPosition(const nn::math::VEC3* world, const nn::math::VEC3* relative)
{
    if (m_SoundId != SE_SYS_SILENT) {
        SoundObjHold::SetPosition(world, relative);
    } else {
        SoundObjBase::SetPosition(world, relative);
        SoundObjBase::Calc();
    }
    if (stage::GetCurrent() == 0 && s_Sounds[m_Size][0] >= 0) {
        f32 distance = m_Distance;
        if (!(0.0f > distance) && distance < NEAR_DISTANCE) {
            if (++m_NearFrames > NEAR_FRAMES) {
                m_IsNear = true;
            }
        } else {
            m_IsNear = false;
            m_NearFrames = 0;
        }
    }
    SoundMgr* mgr = SoundMgr::s_pInstance;
    if (mgr->m_State == 0 || mgr->m_State == 3 || m_SoundId == SE_SYS_SILENT) {
        return;
    }
    // the reverb near the tree
    SoundFxMgr& fx = mgr->m_FxMgr;
    if (fx.m_FxType != SoundFxMgr::FX_DREAM) {
        if (m_DistanceVolume > FX_VOLUME) {
            if (fx.m_State != SoundFxMgr::STATE_ON) {
                fx.Start(SoundFxMgr::FX_4);
            }
        } else if (m_DistanceVolume <= 0.0f && fx.m_FxType == SoundFxMgr::FX_4) {
            fx.FadeOut(SoundFxMgr::FX_4, 0);
        }
    }
    sead::SoundHandle& handle = m_HandleSet.m_Handles[0];
    if (handle.GetId() == m_SoundId && handle.m_pSound != NULL) {
        handle.m_pSound->SetFxSend(nw::snd::AUX_BUS_A, SoundFxMgr::s_SendLevels[SoundFxMgr::FX_DREAM]);
    }
}

// 0x002E9578
void SoundObjSymbolTree::SetSize(u32 size, u32 level)
{
    if (size >= SIZE_NUM || level >= s_LevelNums[size]) {
        return;
    }
    m_Size = size;
    m_Sound = -1;
    if (!Func63061C() && s_Sounds[size][0] >= 0 && stage::GetCurrent() != stage::NAME_VILLAGE_PROLOGUE) {
        m_Sound = s_Sounds[size][level];
    }
    m_SoundId = GetSoundId(m_Sound);
}
