#include "Player/dPlayerModel.h"

#include <cstddef>
#include "Bank/bank_Extern.h"
#include "Bank/dBankTable.h"
#include "Bs/dBsPlayerMgr.h"
#include "Bs/dBsSvMgr.h"
#include "Human/dHumanAnimName.h"
#include "Human/dHumanFaceAnimControl.h"
#include "Human/human_Extern.h"
#include "Item/item_Api.h"
#include "Other/dToolBank.h"
#include "Player/dPlayerAcceBank.h"
#include "Player/dPlayerBottomsBank.h"
#include "Player/dPlayerCapBank.h"
#include "Player/dPlayerFaceBank.h"
#include "Player/dPlayerHeadBank.h"
#include "Player/dPlayerLegsBank.h"
#include "Player/dPlayerShoesBank.h"
#include "Player/dPlayerTopsBank.h"
#include "Player/player_Extern.h"
#include "Sound/dSoundMgr.h"
#include "Sv/dSvFgName.h"
#include "Sv/dSvPersonalId.h"
#include "g3d/dSkeletalModel.h"
#include "nn/math/math_ARMv6.h"
#include "nw/gfx/gfx_CalculatedTransform.h"
#include "nw/gfx/gfx_SkeletalModel.h"
#include "nw/gfx/gfx_Skeleton.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"
#include "nw/gfx/res/gfx_ResModel.h"
#include "netgame/dGameFunctions.h"
#include "sead/seadMathCalcCommon.h"
#include "script/dScriptExtern.h"
#include "ssys/ma/ma_Math.h"
#include "stage/stage_Api.h"

extern const nn::math::MTX34 MTX34_IDENTITY;

// The exchanges of the cap, the head and the accessory are copies the original calls (only ExchangeParts uses them).
template <>
s32 ExchangeBank<32191u, BankVramConfig<2500u, 3u, 12400u, 3u, false>, PlayerCapBankKeyword, true>::Exchange(
    const PlayerCapBankKeyword& keyword, bool isApply);
template <>
s32 ExchangeBank<26275u, BankVramConfig<1800u, 4u, 6600u, 4u, false>, PlayerHeadBankKeyword, false>::Exchange(
    const PlayerHeadBankKeyword& keyword, bool isApply);
template <>
s32 ExchangeBank<14156u, BankVramConfig<300u, 2u, 2000u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>,
                 true>::Exchange(const ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>& keyword, bool isApply);

namespace {
// the results of the exchanges (ExchangeBank::ExchangeResult)
const s32 EXCHANGE_LOADING = 0;
const s32 EXCHANGE_SAME = 1;
const s32 EXCHANGE_SHOWN = 2;
const s32 EXCHANGE_LOADED = 3;

// the player of an appearance (SvPlayerUnk0::playerAppearance)
inline const SvPlayerUnk0* GetPlayerOfAppearance(const SvPlayerAppearance* appearance)
{
    return reinterpret_cast<const SvPlayerUnk0*>(reinterpret_cast<uptr>(appearance) -
                                                 offsetof(SvPlayerUnk0, playerAppearance));
}

// a frames-th of the way from the brightness of the bank to target (target with 0 frames)
inline f32 GetFadeBrightness(ExchangeModelBankBase* bank, f32 target, s32 frames, f32 frameNum)
{
    if (frames == 0) {
        return target;
    }
    const f32 current = Func3287B0(bank, 0);
    return (target - current) / frameNum + current;
}
} // namespace

// 0x00954F9C
f32 PlayerModel::s_ToolScaleMin = 0.01f;

// 0x007CA258
template <>
s32 ExchangeBank<32191u, BankVramConfig<2500u, 3u, 12400u, 3u, false>, PlayerCapBankKeyword, true>::Exchange(
    const PlayerCapBankKeyword& keyword, bool isApply)
{
    return ExchangeImpl(keyword, isApply);
}

// 0x007C9CDC
template <>
s32 ExchangeBank<26275u, BankVramConfig<1800u, 4u, 6600u, 4u, false>, PlayerHeadBankKeyword, false>::Exchange(
    const PlayerHeadBankKeyword& keyword, bool isApply)
{
    return ExchangeImpl(keyword, isApply);
}

// 0x007C8A2C
template <>
s32 ExchangeBank<14156u, BankVramConfig<300u, 2u, 2000u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>,
                 true>::Exchange(const ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>& keyword, bool isApply)
{
    return ExchangeImpl(keyword, isApply);
}

inline Bank<11059u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >* PlayerModel::GetBodyBank()
{
    return BsPlayerMgr::GetBankTables()->m_Body.GetBank(&m_BodyHandle);
}

inline PlayerFaceBank* PlayerModel::GetFaceBank(BankHandle* handle)
{
    return BsPlayerMgr::GetBankTables()->m_Face.GetBank(handle);
}

inline PlayerHeadBank* PlayerModel::GetHeadBank()
{
    return BsPlayerMgr::GetBankTables()->m_Head.GetBank(&m_HeadHandle);
}

inline PlayerTopsBank* PlayerModel::GetTopsBank()
{
    return BsPlayerMgr::GetBankTables()->m_Tops.GetBank(&m_TopsHandle);
}

inline PlayerBottomsBank* PlayerModel::GetBottomsBank()
{
    return BsPlayerMgr::GetBankTables()->m_Bottoms.GetBank(&m_BottomsHandle);
}

inline PlayerLegsBank* PlayerModel::GetLegsBank()
{
    return BsPlayerMgr::GetBankTables()->m_Legs.GetBank(&m_LegsHandle);
}

inline PlayerShoesBank* PlayerModel::GetShoesBank()
{
    return BsPlayerMgr::GetBankTables()->m_Shoes.GetBank(&m_ShoesHandle);
}

inline PlayerAcceBank* PlayerModel::GetAcceBank()
{
    return BsPlayerMgr::GetBankTables()->m_Acce.GetBank(&m_AcceHandle);
}

inline PlayerCapBank* PlayerModel::GetCapBank()
{
    return BsPlayerMgr::GetBankTables()->m_Cap.GetBank(&m_CapHandle);
}

inline ToolBank* PlayerModel::GetToolBank()
{
    return BsPlayerMgr::GetBankTables()->m_Tool.GetBank(&m_ToolHandle);
}

inline HumanResTextureAnimBank* PlayerModel::GetEyeBank(BankHandle* handle)
{
    return BsPlayerMgr::GetBankTables()->m_EyeTextureAnim.GetBank(handle);
}

inline HumanResTextureAnimBank* PlayerModel::GetMouthBank(BankHandle* handle)
{
    return BsPlayerMgr::GetBankTables()->m_MouthTextureAnim.GetBank(handle);
}

// 0x001CE97C
void PlayerModel::Update(bool isEvaluate, bool force)
{
    if (isEvaluate) {
        m_Model.EvaluateAnims();
        GetToolBank()->ApplyBoneRotate();
    }
    GetHeadBank()->EndFaceFallback(force);
    m_RootCtrl.Save(this);
    ApplyBodyPitch();
    m_HeadCtrl.Update();
}

// 0x001CEA8C
void PlayerModel::ChangeScale(f32 target, s32 frames)
{
    if (frames < 0) {
        frames = 0;
    }
    const f32 frameNum = frames;
    f32 scale = target;
    if (frames != 0) {
        scale = (target - m_Scale) / frameNum + m_Scale;
    }
    if (m_Scale != scale) {
        m_Model.SetScale(nn::math::VEC3(scale, scale, scale));
        m_Scale = scale;
    }
}

// 0x001CEB04
bool PlayerModel::PlayAnimWithTool(u16 id, u32 playMode, u8 flags, f32 frame, f32 speed, f32 duration, f32 toolDuration,
                                   f32 toolMorphFrames, u8 morphType)
{
    bool changed;
    bool result = PlayBodyAnim(id, playMode, flags, frame, speed, duration, &changed, morphType);
    SvFgName item(*Func71B394(&m_Outfit));
    if (id == HumanAnimName::SIGNBOARD_WAIT_1 && Func71B394(&m_Outfit)->GetKind() == item::KIND_HAMMER) {
        StopToolAnim(8.0f);
    } else {
        PlayToolAnim(&item, false, false, toolDuration, false, false, false);
    }
    if (changed) {
        ToolBank* tool = GetToolBank();
        const u32 idx = IsArmAnimPlaying();
        result |= tool->PlayToolAnim(m_AnimIds[idx], GetAnimPlayMode(idx), false, GetAnimFrame(idx), GetAnimSpeed(idx),
                                     toolMorphFrames, 2);
    }
    return result;
}

// 0x001CECB4 slot 0x24
void PlayerModel::SetMtx(const nn::math::MTX34* mtx)
{
    if (Func6E5880(m_AnimIds[ANIM_SLOT_BODY])) {
        m_RootCtrl.ApplyWorldCurve(this, mtx);
    }
    m_Model.SetMatrix(*mtx);
    g3d::SkeletalModel* model = GetBottomsBank()->GetCurrentModel();
    if (model != NULL) {
        model->SetMatrix(*mtx);
    }
    model = GetTopsBank()->GetCurrentModel();
    if (model != NULL) {
        model->SetMatrix(*mtx);
    }
    model = GetLegsBank()->GetCurrentModel();
    if (model != NULL) {
        model->SetMatrix(*mtx);
    }
    model = GetShoesBank()->GetCurrentModel();
    if (model != NULL) {
        model->SetMatrix(*mtx);
    }
}

// 0x001CEEF0
void PlayerModel::SetModelColors(const nw::gfx::res::ResModelData* model, s32 hairColor, s32 eyeColor,
                                 const ExchangeBankKeywordUnknown3* skinColor, u32 flags)
{
    ExchangeBankKeywordUnknown3 skin;
    ExchangeBankKeywordUnknown3 hair;
    ExchangeBankKeywordUnknown3 eye;
    skin = *skinColor;
    Func20D3A0(&hair, hairColor);
    Func20D358(&eye, eyeColor);
    if (flags & 1) {
        g3d::SetConstantColor(model, "m", 0, skin.m_0, skin.m_1, skin.m_2, -1);
    }
    if (flags & 2) {
        g3d::SetConstantColor(model, "h", 0, skin.m_0, skin.m_1, skin.m_2, -1);
        g3d::SetConstantColor(model, "h", 1, hair.m_0, hair.m_1, hair.m_2, -1);
    }
    if (flags & 4) {
        g3d::SetConstantColor(model, "e", 0, skin.m_0, skin.m_1, skin.m_2, -1);
        g3d::SetConstantColor(model, "e", 1, eye.m_0, eye.m_1, eye.m_2, -1);
    }
    if (flags & 8) {
        g3d::SetConstantColor(model, "cloth", 0, skin.m_0, skin.m_1, skin.m_2, -1);
    }
    if (flags & 16) {
        g3d::SetConstantColor(model, "legs", 0, skin.m_0, skin.m_1, skin.m_2, -1);
    }
}

// 0x001CF134
bool PlayerModel::Load(const SvPlayer* player, bool isToolShown, bool isFaceB, bool isBothFaces, u32 playerNo)
{
    return LoadAppearance(&player->playerAppearance, ~player->playerInfo.playerData.gender & 1, isToolShown, isFaceB,
                          isBothFaces, playerNo);
}

// 0x001CF17C
bool PlayerModel::LoadAppearance(const SvPlayerAppearance* appearance, bool isMale, bool isToolShown, bool isFaceB,
                                 bool isBothFaces, u32 playerNo)
{
    if (m_Flags & FLAG_LOADED) {
        return true;
    }
    if (!(m_Flags & FLAG_SETUP)) {
        m_Flags |= FLAG_SETUP;
        Setup(appearance, isMale, isFaceB, isBothFaces, playerNo);
        SetToolScale(isToolShown ? 1.0f : 0.0f);
    }
    bool isReady = true;
    PlayerFaceBank* face = GetFaceBank(&m_FaceHandleA);
    if (face != NULL) {
        isReady = face->Load(appearance, isMale, false);
    }
    face = GetFaceBank(&m_FaceHandleB);
    if (face != NULL) {
        isReady &= face->Load(appearance, isMale, true);
    }
    if (!isReady || !ExchangeParts() || (m_Flags & FLAG_LOADED)) {
        return false;
    }
    m_Flags |= FLAG_LOADED;
    if (isMale) {
        m_Flags |= FLAG_IS_MALE;
    } else {
        m_Flags &= ~FLAG_IS_MALE;
    }
    vf_0x10();
    return true;
}

// 0x001CF3C0
void PlayerModel::SetAnimSpeed(u32 idx, f32 speed)
{
    HumanModel::SetAnimSpeed(idx, speed);
    if (idx == ANIM_SLOT_BODY) {
        ToolBank* tool = GetToolBank();
        if (tool->HasKindFlag()) {
            tool->SetAnimSpeed(speed);
        }
    }
}

// 0x001CF470
bool Func1CF470(void* owner, u16 id, s32 playMode, f32 frame, f32 speed, f32 morphFrames)
{
    return reinterpret_cast<PlayerModel*>(static_cast<u8*>(owner) + 0x1B4)
        ->PlayToolBankAnim(id, playMode, frame, speed, morphFrames);
}

// 0x001CF478
bool PlayerModel::PlayToolBankAnim(u16 id, s32 playMode, f32 frame, f32 speed, f32 morphFrames)
{
    return GetToolBank()->PlayAnim(id, playMode, false, frame, speed, morphFrames);
}

// 0x001CF518
bool PlayerModel::Setup(const SvPlayerAppearance* appearance, bool isMale, bool isFaceB, bool isBothFaces, u32 playerNo)
{
    BsPlayerMgr::BankTables* tables = BsPlayerMgr::GetBankTables();
    Func583828(&GetPlayerOfAppearance(appearance)->playerMii);
    m_pAppearance = appearance;
    m_Tan = Func713798(appearance);
    m_Outfit = appearance->playerOutfit;
    m_HeadKeyword.Set(Func713768(appearance), Func713788(appearance), Func713778(appearance), *Func71B33C(&m_Outfit),
                      isFaceB);
    tables->m_Body.Alloc(&m_BodyHandle);
    tables->m_EyeTextureAnim.Alloc(&m_EyeHandleA);
    tables->m_EyeTextureAnim.Alloc(&m_EyeHandleB);
    tables->m_MouthTextureAnim.Alloc(&m_MouthHandleA);
    tables->m_MouthTextureAnim.Alloc(&m_MouthHandleB);
    if (isBothFaces) {
        tables->m_Face.Alloc(&m_FaceHandleB);
        tables->m_Face.Alloc(&m_FaceHandleA);
    } else if (isFaceB) {
        tables->m_Face.Alloc(&m_FaceHandleB);
    } else {
        tables->m_Face.Alloc(&m_FaceHandleA);
    }
    tables->m_Head.Alloc(&m_HeadHandle);
    tables->m_Bottoms.Alloc(&m_BottomsHandle);
    tables->m_Tops.Alloc(&m_TopsHandle);
    tables->m_Legs.Alloc(&m_LegsHandle);
    tables->m_Shoes.Alloc(&m_ShoesHandle);
    tables->m_Acce.Alloc(&m_AcceHandle);
    tables->m_Cap.Alloc(&m_CapHandle);
    tables->m_Tool.Alloc(&m_ToolHandle);
    Build();
    GetHeadBank()->SetBanks(GetFaceBank(&m_FaceHandleA), GetFaceBank(&m_FaceHandleB), appearance,
                            GetEyeBank(&m_EyeHandleA), GetEyeBank(&m_EyeHandleB), GetMouthBank(&m_MouthHandleA),
                            GetMouthBank(&m_MouthHandleB));
    GetTopsBank()->m_pAppearance = appearance;
    GetBottomsBank()->m_IsMale = isMale;
    GetLegsBank()->m_pAppearance = appearance;
    GetCapBank()->m_pAppearance = appearance;
    GetToolBank()->SetPlayerNo(playerNo);
    return true;
}

// 0x001CFBCC
void PlayerModel::SetAnimFrame(u32 idx, f32 frame)
{
    HumanModel::SetAnimFrame(idx, frame);
    if (idx == ANIM_SLOT_BODY && frame >= 0.0f) {
        ToolBank* tool = GetToolBank();
        if (tool->HasKindFlag()) {
            tool->SetAnimFrame(frame);
        }
    }
}

// 0x001CFC90
bool PlayerModel::PlayBodyAnim(u16 id, u32 playMode, u8 flags, f32 frame, f32 speed, f32 duration, bool* changed,
                               u8 morphType)
{
    const u32 isUpperBody = id < HumanAnimName::COUNT ? Func6E6268(id)->flags & 1 : 0;
    const u32 allowsUpperBody =
        m_AnimIds[ANIM_SLOT_BODY] < HumanAnimName::COUNT ? Func6E6268(m_AnimIds[ANIM_SLOT_BODY])->flags2 >> 7 : 0;
    if (allowsUpperBody & isUpperBody) {
        m_Flags |= FLAG_ANIM_UPPER_BODY;
        if (!PlayAnimImpl(id, ANIM_SLOT_ARMS, playMode, frame, speed, duration, flags, changed, morphType)) {
            return false;
        }
        m_Anim.AssignBones(ANIM_SLOT_ARMS, "chest");
        const f32 animFrame = GetAnimFrame(ANIM_SLOT_ARMS);
        const u8 animPlayMode = GetAnimPlayMode(ANIM_SLOT_ARMS);
        PlayerHeadBank* head = GetHeadBank();
        if (head != NULL) {
            head->StartMiiExpressionAnim(id, animPlayMode, animFrame);
        }
    } else {
        if (m_Flags & FLAG_ANIM_UPPER_BODY) {
            StopAnim(ANIM_SLOT_ARMS, 16.0f);
            m_Flags &= ~FLAG_ANIM_UPPER_BODY;
        }
        if (!HumanModel::PlayAnim(id, playMode, flags, frame, speed, duration, changed, morphType)) {
            return false;
        }
        const f32 animFrame = GetAnimFrame(ANIM_SLOT_BODY);
        const u8 animPlayMode = GetAnimPlayMode(ANIM_SLOT_BODY);
        PlayerHeadBank* head = GetHeadBank();
        if (head != NULL) {
            head->StartMiiExpressionAnim(id, animPlayMode, animFrame);
        }
    }
    UpdateRhythmHead(id);
    return true;
}

// 0x001CFEF4
void PlayerModel::SetToolScale(f32 scale)
{
    ToolBank* tool = GetToolBank();
    m_ToolScale = scale;
    static_cast<ExchangeModelBankBase&>(*tool).unknown = m_ToolScale * m_ToolScaleFactor;
}

// 0x001CFF80
bool PlayerModel::PlayToolAnim(const SvFgName* item, bool armsOnly, bool force, f32 duration, bool restart,
                               bool clearRightArm, bool stopArmAnim)
{
    if (stopArmAnim) {
        StopArmAnim(duration);
    }
    if (clearRightArm) {
        m_ToolArmFlags &= ~ARM_FLAG_RIGHT;
    }
    u16 id = Func6E61CC(item);
    if ((!force && std::fabs(m_ToolScale) < s_ToolScaleMin) || id == 0) {
        if (!(Func6E6268(m_AnimIds[IsArmAnimPlaying()])->flags & 0x40)) {
            StopToolAnim(duration);
            return false;
        }
        id = HumanAnimName::TRANS_F_HAND_1;
    } else if (!force && std::fabs(m_ToolScale) < s_ToolScaleMin) {
        StopToolAnim(duration);
        return false;
    }
    ToolBank* tool = GetToolBank();
    if (tool->IsBalloon() && (tool->IsBalloonReleased() || tool->IsBalloonGone())) {
        StopToolAnim(duration);
        return false;
    }
    const u16 current =
        m_AnimIds[IsArmAnimPlaying()] < HumanAnimName::COUNT ? m_AnimIds[IsArmAnimPlaying()] : m_AnimIds[ANIM_SLOT_BODY];
    const u8 freeArms = Func6E6268(current)->freeArms;
    u8 arms = Func6E6268(id)->armFlags & 15;
    if (!armsOnly) {
        arms &= freeArms;
    }
    if (!(m_Flags & FLAG_ANIM_UPPER_BODY) && !restart && m_AnimIds[ANIM_SLOT_TOOL] == id && m_ToolArmFlags == arms) {
        return true;
    }
    if (m_ToolArmFlags != 0) {
        StopAnim(ANIM_SLOT_TOOL, duration);
    }
    m_ToolArmFlags = 0;
    if (arms == 0) {
        return true;
    }
    m_PrevAnimIds[ANIM_SLOT_TOOL] = m_AnimIds[ANIM_SLOT_TOOL];
    m_AnimIds[ANIM_SLOT_TOOL] = id;
    m_Anim.ChangeAnim(ANIM_SLOT_TOOL, Func770970(&g_HumanAnimTable, id)->anim0, 0, duration);
    if (arms & ARM_FLAG_RIGHT) {
        m_Anim.AssignBones(ANIM_SLOT_TOOL, s_BoneNames[BONE_NAME_R_ARM_1]);
    }
    if (arms & ARM_FLAG_LEFT) {
        m_Anim.AssignBones(ANIM_SLOT_TOOL, s_BoneNames[BONE_NAME_L_ARM_1]);
    }
    m_ToolArmFlags = arms;
    return true;
}

// 0x001D0298
bool PlayerModel::Build()
{
    Bank<11059u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >* body = GetBodyBank();
    if (body == NULL) {
        return false;
    }
    ssys::ma::HeapAllocator* allocator = &body->m_Allocators[0];
    void* res = Func5C2938()->m_GraphicsFile.GetModels(0);
    if (!m_Model.Create(nw::gfx::res::ResSceneObject(res), allocator, allocator, 0, true, 1)) {
        return false;
    }
    InitializeBones();
    if (!BuildAnim(HumanAnimName::NML_WAIT_1, allocator, Func6E6258())) {
        return false;
    }
    PlayAnim(HumanAnimName::NML_WAIT_1, 1, 15, 0.0f, 1.0f, 0.0f, NULL, 0);
    sead::Matrix34CalcCtr<f32>::copy(m_BaseMtx, MTX34_IDENTITY);
    nw::gfx::SkeletalModel* model = m_Model.GetSkeletalModel();
    if (model != NULL) {
        m_pBaseTransform = &model->GetSkeleton()->GetLocalTransforms()->begin()[m_BoneIndices[BONE_BASE]];
        if (m_pBaseTransform != NULL) {
            m_RootCtrl.Bind(this);
        }
    }
    return true;
}

// 0x001D0438
bool PlayerModel::GetToolEffectPosition(nn::math::VEC3* out)
{
    ToolBank* tool = GetToolBank();
    return tool->GetBonePosition(out, tool->m_EffectBoneIndex, true);
}

// 0x001D04B8
bool PlayerModel::TransformByTool(nn::math::VEC3* out, const nn::math::VEC3* position)
{
    return GetToolBank()->TransformByModel(out, position, true);
}

// 0x001D053C
void PlayerModel::UpdateRhythmHead(u16 id)
{
    if (stage::GetCurrent() != stage::NAME_CLUB444_DJKK) {
        return;
    }
    // (the animations that leave the head free: bit 2 of freeArms)
    if (Func6E6268(id)->freeArms & 4) {
        if (m_IsRhythmHeadPlaying) {
            return;
        }
        m_RhythmHeadIndex = netgame::Func2FF63C(s_RhythmHeadAnimNum);
        const u16 anim = Func6E58D0(m_RhythmHeadIndex);
        m_PrevAnimIds[ANIM_SLOT_HEAD] = m_AnimIds[ANIM_SLOT_HEAD];
        m_AnimIds[ANIM_SLOT_HEAD] = anim;
        m_Anim.ChangeAnim(ANIM_SLOT_HEAD, Func770970(&g_HumanAnimTable, anim)->anim0, 0, 8.0f);
        m_Anim.AssignBones(ANIM_SLOT_HEAD, s_BoneNames[BONE_NAME_HEAD]);
        HumanModel::SetAnimPlayMode(0, ANIM_SLOT_HEAD);
        SetAnimFrame(ANIM_SLOT_HEAD, 0.0f);
        HumanModel::SetAnimSpeed(ANIM_SLOT_HEAD, 0.0f);
        m_IsRhythmHeadPlaying = true;
    } else if (m_IsRhythmHeadPlaying) {
        StopAnim(ANIM_SLOT_HEAD, 8.0f);
        m_IsRhythmHeadPlaying = false;
    }
}

// 0x001D0654
void PlayerModel::SetSkinColors()
{
    ExchangeBankKeywordUnknown3 skinColor;
    Func20D3E8(&skinColor, m_Tan);
    PlayerCapBank* cap = GetCapBank();
    if (IsMiiMask(&cap->m_Keywords[cap->GetCurrentSlot()].m_Name) && m_pAppearance != NULL) {
        u8 color[4] = {255, 0, 0, 0};
        if (Func753C10(&GetPlayerOfAppearance(m_pAppearance)->playerMii, color)) {
            skinColor.m_0 = color[3];
            skinColor.m_1 = color[2];
            skinColor.m_2 = color[1];
        }
    }
    GetCapBank()->SetColors(&skinColor);
    GetHeadBank()->SetFaceColors(&skinColor);
    GetTopsBank()->SetColors(&skinColor);
    GetLegsBank()->SetColors(&skinColor);
}

// 0x001D090C
void PlayerModel::SetAnimPlayMode(s32 playMode, u32 idx)
{
    HumanModel::SetAnimPlayMode(playMode, idx);
    if (idx == ANIM_SLOT_BODY) {
        ToolBank* tool = GetToolBank();
        if (tool->HasKindFlag()) {
            tool->SetAnimPlayMode(playMode);
        }
    }
}

// 0x001D09B0
void PlayerModel::UpdateRhythmHeadFrame()
{
    if (!m_IsRhythmHeadPlaying) {
        return;
    }
    const f32 frame = GetAnimFrame(ANIM_SLOT_HEAD);
    f32 endFrame = 0.0f;
    const nw::anim::res::ResAnimData* data = Func770970(&g_HumanAnimTable, HumanAnimName::RHYTHM_HEAD_1)->anim0.ptr();
    if (data != NULL) {
        endFrame = data->frameSize;
    }
    const f32 target = (endFrame - 1.0f) * SoundMgr::SGetDJBeatPhase();
    if (frame < target) {
        SetAnimFrame(ANIM_SLOT_HEAD, target);
        return;
    }
    m_RhythmHeadIndex = (m_RhythmHeadIndex + 1) % s_RhythmHeadAnimNum;
    const u16 anim = Func6E58D0(m_RhythmHeadIndex);
    m_PrevAnimIds[ANIM_SLOT_HEAD] = m_AnimIds[ANIM_SLOT_HEAD];
    m_AnimIds[ANIM_SLOT_HEAD] = anim;
    m_Anim.ChangeAnim(ANIM_SLOT_HEAD, Func770970(&g_HumanAnimTable, anim)->anim0, 0, 0.0f);
    HumanModel::SetAnimPlayMode(0, ANIM_SLOT_HEAD);
    SetAnimFrame(ANIM_SLOT_HEAD, target);
    HumanModel::SetAnimSpeed(ANIM_SLOT_HEAD, 0.0f);
}

// 0x001D0AD8
void PlayerModel::StopToolAnim(f32 duration)
{
    if (m_ToolArmFlags != 0) {
        StopAnim(ANIM_SLOT_TOOL, duration);
    }
    m_ToolArmFlags = 0;
}

// 0x001D0B04
void PlayerModel::SetToolScaleFactor(f32 factor)
{
    ToolBank* tool = GetToolBank();
    m_ToolScaleFactor = factor;
    static_cast<ExchangeModelBankBase&>(*tool).unknown = m_ToolScale * m_ToolScaleFactor;
}

// 0x001D0B90
bool PlayerModel::ExchangeParts()
{
    const s32 headBone = m_BoneIndices[BONE_HEAD];
    const s32 handBone = m_BoneIndices[BONE_R_HAND];
    ExchangeBankKeywordUnknown3 skinColor;
    Func20D3E8(&skinColor, m_Tan);
    const void* designs = GetPlayerOfAppearance(m_pAppearance)->patterns;

    // the cap, the head and the accessory: all loaded first, then one shown per frame
    PlayerCapBank* cap = GetCapBank();
    const PlayerCapBankKeyword capKeyword(*Func71B33C(&m_Outfit), designs, skinColor, m_HeadKeyword.hairColor);
    IsMiiMask(&capKeyword.m_Name); // (the result is not used)
    s32 result = cap->Exchange(capKeyword, false);
    bool isReady = result == EXCHANGE_SAME || result == EXCHANGE_LOADED;
    PlayerHeadBank* head = GetHeadBank();
    {
        const PlayerHeadBankKeyword headKeyword(m_HeadKeyword, *Func71B33C(&m_Outfit), skinColor);
        result = head->Exchange(headKeyword, false);
        isReady &= result == EXCHANGE_SAME || result == EXCHANGE_LOADED;
    }
    PlayerAcceBank* acce = GetAcceBank();
    {
        const PlayerAcceBank::Keyword acceKeyword(*Func71B370(&m_Outfit));
        result = acce->Exchange(acceKeyword, false);
        isReady &= result == EXCHANGE_SAME || result == EXCHANGE_LOADED;
    }
    if (isReady) {
        bool isChanged = false;
        if (cap->Exchange(capKeyword, true) == EXCHANGE_SHOWN) {
            Func3286CC(cap, &m_Model, headBone);
            isChanged = true;
        }
        {
            const PlayerHeadBankKeyword headKeyword(m_HeadKeyword, *Func71B33C(&m_Outfit), skinColor);
            if (head->Exchange(headKeyword, true) == EXCHANGE_SHOWN) {
                head->SetBoneMatrix(&m_Model, headBone);
                isChanged = true;
            }
        }
        if (isChanged) {
            isReady = false;
        } else {
            const PlayerAcceBank::Keyword acceKeyword(*Func71B370(&m_Outfit));
            if (acce->Exchange(acceKeyword, true) == EXCHANGE_SHOWN) {
                Func3286CC(acce, &m_Model, headBone);
            }
        }
    }

    // the tool and the clothes
    ToolBank* tool = GetToolBank();
    {
        const ToolBank::Keyword toolKeyword(*Func71B394(&m_Outfit), designs);
        result = tool->Exchange(toolKeyword);
        if (result == EXCHANGE_SHOWN) {
            tool->SetBoneMatrix(&m_Model, handBone);
        }
        isReady &= result != EXCHANGE_LOADING;
    }
    PlayerBottomsBank* bottoms = GetBottomsBank();
    {
        const PlayerBottomsBank::Keyword bottomsKeyword(*Func71AEF8(&m_Outfit));
        result = bottoms->Exchange(bottomsKeyword);
        if (result == EXCHANGE_SHOWN) {
            Func328778(bottoms, &m_Model);
        }
        isReady &= result != EXCHANGE_LOADING;
    }
    PlayerTopsBank* tops = GetTopsBank();
    {
        const PlayerTopsBank::Keyword topsKeyword(*Func71B3B8(&m_Outfit), designs, skinColor);
        result = tops->Exchange(topsKeyword);
        if (result == EXCHANGE_SHOWN) {
            Func328778(tops, &m_Model);
        }
        isReady &= result != EXCHANGE_LOADING;
    }
    PlayerLegsBank* legs = GetLegsBank();
    {
        const PlayerLegsBank::Keyword legsKeyword(*Func71B400(&m_Outfit), designs, skinColor);
        result = legs->Exchange(legsKeyword);
        if (result == EXCHANGE_SHOWN) {
            Func328778(legs, &m_Model);
        }
        isReady &= result != EXCHANGE_LOADING;
    }
    PlayerShoesBank* shoes = GetShoesBank();
    {
        const PlayerShoesBank::Keyword shoesKeyword(*Func71B3DC(&m_Outfit));
        result = shoes->Exchange(shoesKeyword);
        if (result == EXCHANGE_SHOWN) {
            Func328778(shoes, &m_Model);
        }
        isReady &= result != EXCHANGE_LOADING;
    }
    SetSkinColors();
    return isReady;
}

// 0x001D2270
void Func1D2270(void* owner)
{
    u8* object = static_cast<u8*>(owner);
    reinterpret_cast<PlayerModel*>(object + 0x1B4)
        ->SetBrightness(Func1AC174(reinterpret_cast<const nn::math::VEC3*>(object + 0x14)));
}

// 0x001D228C
void PlayerModel::SetBrightness(f32 brightness)
{
    GetHeadBank()->SetBrightness(0, brightness);
    Func328808(GetBottomsBank(), 0, brightness);
    Func328808(GetTopsBank(), 0, brightness);
    Func328808(GetLegsBank(), 0, brightness);
    Func328808(GetShoesBank(), 0, brightness);
    Func328808(GetAcceBank(), 0, brightness);
    Func328808(GetCapBank(), 0, brightness);
    Func328808(GetToolBank(), 0, brightness);
}

// 0x001D264C
f32 PlayerModel::FadeBrightness(f32 target, s32 frames)
{
    if (frames < 0) {
        frames = 0;
    }
    const f32 frameNum = frames;
    PlayerHeadBank* head = GetHeadBank();
    head->SetBrightness(0, GetFadeBrightness(head, target, frames, frameNum));
    ExchangeModelBankBase* bank = GetBottomsBank();
    Func328808(bank, 0, GetFadeBrightness(bank, target, frames, frameNum));
    bank = GetTopsBank();
    Func328808(bank, 0, GetFadeBrightness(bank, target, frames, frameNum));
    bank = GetLegsBank();
    Func328808(bank, 0, GetFadeBrightness(bank, target, frames, frameNum));
    bank = GetShoesBank();
    Func328808(bank, 0, GetFadeBrightness(bank, target, frames, frameNum));
    bank = GetAcceBank();
    Func328808(bank, 0, GetFadeBrightness(bank, target, frames, frameNum));
    bank = GetCapBank();
    Func328808(bank, 0, GetFadeBrightness(bank, target, frames, frameNum));
    bank = GetToolBank();
    const f32 brightness = GetFadeBrightness(bank, target, frames, frameNum);
    Func328808(bank, 0, brightness);
    return brightness;
}

// 0x001D2B74
void PlayerModel::ApplyBodyPitch()
{
    nw::gfx::CalculatedTransform* transform = m_pBaseTransform;
    if (transform == NULL) {
        return;
    }
    const f32 x = transform->m_TransformMatrix.m[0][3];
    const f32 y = transform->m_TransformMatrix.m[1][3];
    const f32 z = transform->m_TransformMatrix.m[2][3];
    m_BaseMtx = transform->m_TransformMatrix;
    if (m_Rotation.x.value == 0) {
        return;
    }
    nn::math::MTX34 mtx;
    mtx = transform->m_TransformMatrix;
    nn::math::MTX34 rotate;
    sead::Matrix34CalcCtr<f32>::makeIdentity(rotate);
    ssys::ma::RotateX(&rotate, m_Rotation.x.value);
    sead::Matrix34CalcCtr<f32>::multiply(mtx, rotate, mtx);
    nn::math::ARMv6::MTX34CopyAsm(&transform->m_TransformMatrix, &mtx);
    transform->m_TransformMatrix.m[0][3] = x;
    transform->m_TransformMatrix.m[1][3] = y;
    transform->m_TransformMatrix.m[2][3] = z;
    transform->DisableFlags(nw::gfx::CalculatedTransform::FLAG_ROTATE_UNSET |
                            nw::gfx::CalculatedTransform::FLAG_IS_ROTATE_ZERO);
    transform->EnableFlags(nw::gfx::CalculatedTransform::FLAG_IS_DIRTY);
}

// 0x001D2C50
bool PlayerModel::PlayChairSitAnim(f32 duration, f32 toolDuration, f32 toolMorphFrames)
{
    return PlayAnimWithTool(HumanAnimName::CHAIR_SITDOWN_WAIT_1, 1, 1, 0.0f, 1.0f, duration, toolDuration,
                            toolMorphFrames, 0);
}

// 0x001D2C8C slot 0x08
void PlayerModel::vf_0x08()
{
    m_Model.UpdateAnimFrames();
    GetHeadBank()->Update();
    GetToolBank()->UpdateFrames();
}

// 0x001D2D70 slot 0x14
void PlayerModel::vf_0x14()
{
    Draw(true, false);
}

// 0x001D2D7C
bool PlayerModel::Draw(bool isDrawTool, bool isSkipExchange)
{
    bool isReady;
    if (isSkipExchange) {
        isReady = false;
    } else {
        isReady = ExchangeParts();
    }
    PlayerCapBank* cap = GetCapBank();
    const bool isMiiMask = IsMiiMask(&cap->m_Keywords[cap->GetCurrentSlot()].m_Name);
    if (!isMiiMask) {
        cap->RequestDraw(0);
    }
    GetHeadBank()->RequestDraw(0);
    GetLegsBank()->RequestDraw(0);
    GetShoesBank()->RequestDraw(0);
    PlayerTopsBank* tops = GetTopsBank();
    tops->RequestDraw(0);
    const SvFgName* topsItem = &tops->m_Keywords[tops->GetCurrentSlot()].m_Name;
    if (Func76A68C(topsItem) || topsItem->IsEmpty()) {
        GetBottomsBank()->RequestDraw(0);
    }
    PlayerAcceBank* acce = GetAcceBank();
    if (!isMiiMask) {
        acce->RequestDraw(0);
    }
    if (isDrawTool) {
        GetToolBank()->AddToScene(0);
    }
    return isReady;
}

// 0x001D3174
bool PlayerModel::Unload()
{
    BsPlayerMgr::BankTables* tables = BsPlayerMgr::GetBankTables();
    bool isFreed = tables->m_Tool.Free(&m_ToolHandle);
    isFreed &= tables->m_Cap.Free(&m_CapHandle);
    isFreed &= tables->m_Acce.Free(&m_AcceHandle);
    isFreed &= tables->m_Shoes.Free(&m_ShoesHandle);
    isFreed &= tables->m_Legs.Free(&m_LegsHandle);
    isFreed &= tables->m_Tops.Free(&m_TopsHandle);
    isFreed &= tables->m_Bottoms.Free(&m_BottomsHandle);
    isFreed &= tables->m_Head.Free(&m_HeadHandle);
    PlayerFaceBank* face = GetFaceBank(&m_FaceHandleA);
    if (face != NULL) {
        face->Unload();
    }
    face = GetFaceBank(&m_FaceHandleB);
    if (face != NULL) {
        face->Unload();
    }
    if (GetFaceBank(&m_FaceHandleA) != NULL) {
        isFreed &= tables->m_Face.Free(&m_FaceHandleA);
    }
    if (GetFaceBank(&m_FaceHandleB) != NULL) {
        isFreed &= tables->m_Face.Free(&m_FaceHandleB);
    }
    isFreed &= tables->m_MouthTextureAnim.Free(&m_MouthHandleB);
    isFreed &= tables->m_MouthTextureAnim.Free(&m_MouthHandleA);
    isFreed &= tables->m_EyeTextureAnim.Free(&m_EyeHandleB);
    isFreed &= tables->m_EyeTextureAnim.Free(&m_EyeHandleA);
    FinalizeModel();
    const bool result = tables->m_Body.Free(&m_BodyHandle) & isFreed;
    if (result) {
        m_Flags &= ~FLAG_SETUP;
        m_Flags &= ~FLAG_LOADED;
    }
    return result;
}

// 0x001D3428 slot 0x00
bool PlayerModel::PlayAnim(u16 id, u32 playMode, u32 flags, f32 frame, f32 speed, f32 duration, bool* /* changed */,
                           u8 morphType)
{
    return PlayAnimWithTool(id, playMode, flags, frame, speed, duration, duration, duration, morphType);
}

// 0x001D3448
void PlayerModel::SetHair(u8 style, s32 color)
{
    m_HeadKeyword.hairStyle = style;
    m_HeadKeyword.hairColor = color;
}

// 0x001D3454
void PlayerModel::SetScale(f32 scale)
{
    if (m_Scale != scale) {
        m_Model.SetScale(nn::math::VEC3(scale, scale, scale));
        m_Scale = scale;
    }
}

// 0x001D34A0 slot 0x10
void PlayerModel::vf_0x10()
{
    const s32 handBone = m_BoneIndices[BONE_R_HAND];
    const s32 headBone = m_BoneIndices[BONE_HEAD];
    m_Model.Update();
    nw::gfx::CalculatedTransform* transform = m_pBaseTransform;
    if (transform != NULL) {
        nn::math::ARMv6::MTX34CopyAsm(&transform->m_TransformMatrix, &m_BaseMtx);
        transform->EnableFlags(nw::gfx::CalculatedTransform::FLAG_IS_DIRTY);
    }
    m_HeadCtrl.Restore();
    GetHeadBank()->SetBoneMatrix(&m_Model, headBone);
    Func3286CC(GetAcceBank(), &m_Model, headBone);
    Func3286CC(GetCapBank(), &m_Model, headBone);
    GetToolBank()->SetBoneMatrix(&m_Model, handBone);
    Func328778(GetBottomsBank(), &m_Model);
    Func328778(GetTopsBank(), &m_Model);
    Func328778(GetLegsBank(), &m_Model);
    Func328778(GetShoesBank(), &m_Model);
}

// 0x001D3884
void PlayerModel::SetOutfit(const SvPlayerOutfit* outfit)
{
    m_Outfit = *outfit;
}

// 0x001D38F8
PlayerModel::PlayerModel()
    : HumanModel(false), m_ToolArmFlags(0), m_IsRhythmHeadPlaying(false), m_RhythmHeadIndex(0), m_Tan(0),
      m_pAppearance(NULL), m_Flags(0)
{
    m_Scale = 1.0f;
    m_ToolScale = 1.0f;
    m_ToolScaleFactor = 1.0f;
    m_RootCtrl.m_RootTranslation = nn::math::VEC3(0.0f, 0.0f, 0.0f);
    m_RootCtrl.m_BaseTranslation = nn::math::VEC3(0.0f, 0.0f, 0.0f);
    m_RootCtrl.m_pPosition = NULL;
    m_RootCtrl.m_pRootTransform = NULL;
    m_RootCtrl.m_pBaseTransform = NULL;
    m_RootCtrl.m_pRootBone = NULL;
    m_RootCtrl.m_pBaseBone = NULL;
    // (cleared a second time)
    m_Flags = 0;
    m_pBaseTransform = NULL;
}

// 0x001D3A24 slot 0x1C
// 0x001D3A14 (deleting dtor)
DECOMP_NOINLINE DECOMP_NOIPA PlayerModel::~PlayerModel()
{
    // (only the members are destroyed)
}

// 0x001AC87C slot 0x0C
void PlayerModel::vf_0x0C()
{
    Update(true, true);
}

// 0x0027231C slot 0x18
HumanFaceAnimControl* PlayerModel::GetFaceCtrl()
{
    return GetHeadBank()->GetFaceAnimControl();
}

// 0x0070F178 slot 0x28
bool PlayerModel::vf_0x28()
{
    return true;
}
