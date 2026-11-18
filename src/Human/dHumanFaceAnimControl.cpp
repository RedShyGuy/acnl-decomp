#include "Human/dHumanFaceAnimControl.h"
#include "Human/dHumanResTextureAnimBank.h"
#include "Human/human_Extern.h"
#include "g3d/dModel.h"
#include "g3d/dResourceSet.h"
#include "netgame/dGameFunctions.h"

namespace {
// the flag bits of the animation table read here (HumanAnimInfo::flags)
const u32 ANIM_FLAG_HOLD_SHIFT = 4; // a one-time animation of the first slot stays at its end
const u32 ANIM_FLAG_LOOP_SHIFT = 5; // the play mode of playMode 2
// the play mode that takes the loop flag of the animation
const s32 PLAY_MODE_FROM_ANIM = 2;
// the material objects of the slots in the model
const s32 OBJECT_A = 0;
const s32 OBJECT_B = 1;
// the restart timer of the default animation: 20 + random(60) frames
const u32 TIMER_RANDOM = 60;
const s32 TIMER_MIN = 20;

// (inline) the flag of id at shift
inline u32 GetAnimFlag(u32 id, u32 shift)
{
    return (Func6E6268(id)->flags >> shift) & 1;
}

// (inline) the restart timer
inline s32 GetRestartTime()
{
    return netgame::Func2FF63C(TIMER_RANDOM) + TIMER_MIN;
}
} // namespace

// 0x0031DDE0 (name is ours)
void HumanFaceAnimControl::PlayAnimA(u16 id, s32 playMode, bool useSpeed, f32 frame, f32 speed)
{
    if (id >= HumanAnimName::INVALID) {
        return;
    }
    if (playMode == PLAY_MODE_FROM_ANIM) {
        playMode = GetAnimFlag(id, ANIM_FLAG_LOOP_SHIFT);
    }
    if (!useSpeed) {
        speed = speed >= 0.0f ? 1.0f : -1.0f;
    }
    if (PlayAnim(&m_SlotA, Func6E5778, Func6E57C0, frame, speed, OBJECT_A, id, playMode,
                 PLAY_FLAG_DEFAULT_IF_MISSING)) {
        m_SlotA.m_IsHold = GetAnimFlag(id, ANIM_FLAG_HOLD_SHIFT);
    }
}

// 0x0031DEE4 (name is ours)
void HumanFaceAnimControl::EndFallbackB(bool evaluate, bool force)
{
    if (m_pModel == NULL) {
        return;
    }
    if (evaluate) {
        if (!force && !m_IsFallbackB) {
            return;
        }
        m_pModel->EvaluateAnims();
    }
    if (m_IsFallbackB) {
        StopAnim(&m_SlotB, OBJECT_B);
        m_IsFallbackB = false;
    }
}

// 0x0031DF3C (name is ours)
void HumanFaceAnimControl::PlayAnimB(u16 id, s32 playMode, bool useSpeed, f32 frame, f32 speed)
{
    if (id >= HumanAnimName::INVALID) {
        return;
    }
    if (playMode == PLAY_MODE_FROM_ANIM) {
        playMode = GetAnimFlag(id, ANIM_FLAG_LOOP_SHIFT);
    }
    if (!useSpeed) {
        speed = speed >= 0.0f ? 1.0f : -1.0f;
    }
    const bool hadAnim = m_SlotB.m_AnimId != HumanAnimName::INVALID;
    if (PlayAnim(&m_SlotB, Func6E5864, Func6E58AC, frame, speed, OBJECT_B, id, playMode, 0)) {
        return;
    }
    if (!hadAnim || m_IsFallbackB) {
        return;
    }
    const u16 fallback = Func770970(&g_HumanAnimTable, m_DefaultAnimIdB)->anim2.ptr() != NULL
                             ? m_DefaultAnimIdB
                             : static_cast<u16>(HumanAnimName::ANIM_FA);
    PlayAnim(&m_SlotB, Func6E5864, Func6E58AC, 0.0f, 0.0f, OBJECT_B, fallback, 1, 0);
    m_IsFallbackB = true;
}

// 0x0031E078 (name is ours)
void HumanFaceAnimControl::Update(bool updateFrames)
{
    if (m_pModel == NULL) {
        return;
    }
    if (m_SlotA.m_AnimId != HumanAnimName::INVALID) {
        if (m_SlotA.m_Timer != 0) {
            m_SlotA.m_Timer--;
        }
        if (m_SlotA.m_Anim.m_PlayMode == g3d::FrameController::PLAY_MODE_ONETIME && !m_SlotA.m_IsHold &&
            m_SlotA.m_Anim.IsFinished()) {
            const u16 defaultId = Func770970(&g_HumanAnimTable, m_DefaultAnimIdA)->anim1.ptr() != NULL
                                      ? m_DefaultAnimIdA
                                      : static_cast<u16>(HumanAnimName::ANIM_F7);
            if (m_SlotA.m_AnimId != defaultId && Func770970(&g_HumanAnimTable, defaultId)->anim1.ptr() != NULL) {
                PlayAnim(&m_SlotA, Func6E5778, Func6E57C0, 0.0f, 1.0f, OBJECT_A, defaultId, 0, PLAY_FLAG_RESTART);
            } else if (m_SlotA.m_Timer == 0) {
                m_SlotA.m_Timer = GetRestartTime();
                m_SlotA.m_Anim.SetFrame(0.0f);
            }
        }
    }
    FaceCtrl::Update();
    if (updateFrames) {
        m_pModel->UpdateAnimFrames();
    }
}

// 0x0031E1C0 (name is ours)
void HumanFaceAnimControl::PlayAnimB2(u16 id, s32 playMode, bool useSpeed, f32 frame, f32 speed)
{
    if (id >= HumanAnimName::INVALID) {
        return;
    }
    if (playMode == PLAY_MODE_FROM_ANIM) {
        playMode = GetAnimFlag(id, ANIM_FLAG_LOOP_SHIFT);
    }
    if (!useSpeed) {
        speed = speed >= 0.0f ? 1.0f : -1.0f;
    }
    if (PlayAnim(&m_SlotB, Func6E5864, Func6E58AC, frame, speed, OBJECT_B, id, playMode, 0) || m_IsFallbackB) {
        return;
    }
    const u16 fallback = Func770970(&g_HumanAnimTable, m_DefaultAnimIdB)->anim2.ptr() != NULL
                             ? m_DefaultAnimIdB
                             : static_cast<u16>(HumanAnimName::ANIM_FA);
    PlayAnim(&m_SlotB, Func6E5864, Func6E58AC, 0.0f, 0.0f, OBJECT_B, fallback, 1, 0);
    m_IsFallbackB = true;
}

// 0x0031E2E0 (name is ours)
bool HumanFaceAnimControl::PlayAnim(AnimSlot* slot, GetAnimFunc getAnim, CloneAnimFunc cloneAnim, f32 frame, f32 speed,
                                    s32 objectIdx, u16 id, s32 playMode, u32 flags)
{
    if (m_pModel == NULL || slot->m_pBank == NULL) {
        return false;
    }
    bool isRestart = false;
    const bool isRestartFlag = (flags & PLAY_FLAG_RESTART) != 0;
    if (isRestartFlag) {
        isRestart = true;
    } else if ((flags & PLAY_FLAG_DEFAULT_IF_MISSING) != 0 && getAnim(id)->ptr() == NULL) {
        const u16 defaultId = Func770970(&g_HumanAnimTable, m_DefaultAnimIdA)->anim1.ptr() != NULL
                                  ? m_DefaultAnimIdA
                                  : static_cast<u16>(HumanAnimName::ANIM_F7);
        if (getAnim(defaultId)->ptr() != NULL) {
            id = defaultId;
            isRestart = true;
        }
    }
    const u16 current = slot->m_AnimId;
    if (current == id) {
        if (current == HumanAnimName::INVALID) {
            return false;
        }
        if (isRestartFlag) {
            slot->m_Anim.SetFrame(frame);
            slot->m_Anim.SetSpeed(speed);
            slot->m_Anim.SetPlayMode(0);
            slot->m_Timer = GetRestartTime();
        } else {
            slot->m_Anim.SetSpeed(speed);
            if (isRestart) {
                slot->m_Anim.SetPlayMode(0);
            } else {
                if (slot->m_Anim.m_PlayMode == g3d::FrameController::PLAY_MODE_ONETIME) {
                    slot->m_Anim.SetFrame(frame);
                }
                slot->m_Anim.SetPlayMode(playMode);
            }
        }
        return true;
    }
    nw::os::IAllocator* allocator = &slot->m_pBank->m_Allocators[0];
    if (current != HumanAnimName::INVALID) {
        if (!(m_IsFallbackB && objectIdx == OBJECT_B && getAnim(id)->ptr() == NULL)) {
            StopAnim(slot, objectIdx);
        }
    }
    if (getAnim(id)->ptr() == NULL) {
        return false;
    }
    if (isRestart) {
        playMode = 0;
        frame = 0.0f;
        speed = 1.0f;
        slot->m_Timer = GetRestartTime();
    }
    slot->m_ResAnim = cloneAnim(id, allocator);
    slot->m_ResAnim.Setup(allocator, m_pResourceSet->m_GraphicsFile);
    slot->m_AnimId = id;
    slot->m_Anim.ChangeAnim(slot->m_ResAnim);
    if (!m_Unknown24) {
        slot->m_Anim.UnbindMemberAnim(1);
    }
    slot->m_Anim.SetPlayMode(playMode);
    slot->m_Anim.SetSpeed(speed);
    slot->m_Anim.SetFrame(frame);
    m_pModel->SetMaterialAnim(&slot->m_Anim, objectIdx);
    m_Unknown16 = false;
    return true;
}

// 0x0031E588 (name is ours)
void HumanFaceAnimControl::StopAnim(AnimSlot* slot, s32 objectIdx)
{
    if (m_pModel != NULL) {
        m_pModel->ClearMaterialAnim(objectIdx);
    }
    slot->m_AnimId = HumanAnimName::INVALID;
    slot->m_Timer = 0;
    slot->m_Unknown31 = false;
    if (slot->m_ResAnim.ptr() != NULL) {
        nw::anim::res::ResAnim::DestroyClone(&slot->m_pBank->m_Allocators[0], slot->m_ResAnim);
        slot->m_ResAnim = nw::anim::res::ResAnim();
    }
    slot->m_IsHold = false;
}

// 0x0031E5E4 (name is ours)
void HumanFaceAnimControl::Initialize(g3d::Model* model, HumanResTextureAnimBank* bankA, HumanResTextureAnimBank* bankB,
                                      void* /*unused*/, g3d::ResourceSet* resourceSet, bool unknown24)
{
    m_SlotB.m_pBank = bankB;
    m_pModel = model;
    m_Unknown24 = unknown24;
    m_pResourceSet20 = resourceSet;
    m_SlotA.m_pBank = bankA;
    m_pResourceSet = resourceSet;
    m_Unknown16 = false;
    m_SlotA.m_Anim.Build(model, Func770970(&g_HumanAnimTable, HumanAnimName::ANIM_F7)->anim1, &bankA->m_Allocators[0],
                         false);
    m_SlotB.m_Anim.Build(model, Func770970(&g_HumanAnimTable, HumanAnimName::ANIM_FA)->anim2, &bankB->m_Allocators[0],
                         false);
    PlayAnimA(HumanAnimName::NML_WAIT_1, PLAY_MODE_FROM_ANIM, false, 0.0f, 1.0f);
    PlayAnimB(HumanAnimName::NML_WAIT_1, PLAY_MODE_FROM_ANIM, false, 0.0f, 1.0f);
}

// 0x0031E6C4 (name is ours)
void HumanFaceAnimControl::Finalize()
{
    StopAnim(&m_SlotA, OBJECT_A);
    m_SlotA.m_Anim.Finalize();
    StopAnim(&m_SlotB, OBJECT_B);
    m_SlotB.m_Anim.Finalize();
    m_pModel = NULL;
    m_pResourceSet20 = NULL;
    m_SlotA.m_pBank = NULL;
    m_SlotB.m_pBank = NULL;
}

// 0x0031E710
HumanFaceAnimControl::HumanFaceAnimControl()
    : m_pResourceSet20(NULL), m_Unknown24(true), m_DefaultAnimIdA(HumanAnimName::ANIM_F7),
      m_DefaultAnimIdB(HumanAnimName::ANIM_FA), m_IsFallbackB(false)
{
}

// 0x0031E7F0 slot 0x00
// 0x0031E7C8 (deleting dtor)
HumanFaceAnimControl::~HumanFaceAnimControl()
{
    // (only the members are destroyed)
}
