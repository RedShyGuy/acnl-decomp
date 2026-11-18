#include "Player/dPlayerHeadBank.h"
#include <cstddef>
#include "Bank/bank_Extern.h"
#include "Human/dHumanResTextureAnimBank.h"
#include "Human/human_Extern.h"
#include "Player/dPlayerFaceBank.h"
#include "Sv/dSvPlayer.h"
#include "g3d/dSkeletalModel.h"
#include "g3d/g3d_System.h"
#include "nw/anim/res/anim_ResAnimCurve.h"
#include "nw/anim/res/anim_ResMemberAnim.h"
#include "nw/gfx/res/gfx_ResTexture.h"
#include "nw/ut/ut_ResDicPatricia.h"
#include "sead/seadFixedSafeString.h"
#include "sead/seadStringFormatter.h"
#include "ssys/ma/ma_Math.h"

// (in dPlayerHeadBankTable.cpp, otherwise GCC folds them)
extern const f32 s_MiiScale;
extern const s32 s_MiiExpressions[8];

namespace {
// the alignment of the head model file
const u32 FILE_ALIGNMENT = 128;
// the hair styles of each sex (the second half are the styles of the girls)
const u32 HAIR_STYLE_NUM = 17;
// the hair style with the file name "<sex>_msd"
const u32 HAIR_STYLE_MSD = 16;
// Func311840: the kinds of headwear that change the head model
const s32 HEADWEAR_MODEL_B = 41;
const s32 HEADWEAR_NO_HEAD = 43;
const s32 HEADWEAR_BALD_EAR_0 = 44;
const s32 HEADWEAR_BALD_EAR_1 = 45;
const s32 HEADWEAR_BALD_0 = 46;
const s32 HEADWEAR_BALD_1 = 47;
// the CFL source of a Mii given by its data and the description of the Mii model
const s32 MII_SOURCE_DATA = 5;
const s32 MII_RESOLUTION = 128;
const u32 MII_EXPRESSION_MASK = 0x3F;
// the offset of the Mii in the player data after the checksum (SvPlayer::playerMii)
const u32 MII_DATA_OFFSET = offsetof(SvPlayerUnk0, playerMii) - offsetof(SvPlayerUnk0, playerAppearance);
// the turn of the Mii at a bone (-90 degrees)
const s16 MII_BONE_ROTATE = -0x4000;
// the flag of the animation table: a one-time animation (the expression stops at the end frame)
const u8 ANIM_FLAG_LOOP = 1 << 5;
// the model lists of the object 0x00951378
const u32 MODEL_LIST_OFFSET = 0x12BE0;
// the light set count of SkeletalModel::Create of the head
const s32 HEAD_MODEL_CREATE_FLAGS = 2;
// the colors of the face (Func1CEEF0)
const s32 FACE_COLOR_KIND = 7;
} // namespace

// 0x00271584 (name is ours)
void PlayerHeadBank::RequestDraw()
{
    HeadMii& mii = m_Miis[GetCurrentSlot()];
    if (mii.m_IsUsed && &mii.m_Model != NULL) {
        mii.m_Model.RequestDraw();
        return;
    }
    g3d::SkeletalModel* model = GetCurrentModel();
    if (model == NULL || g_pUnknown951378 == NULL) {
        return;
    }
    BankModelList* list = reinterpret_cast<BankModelList*>(g_pUnknown951378 + MODEL_LIST_OFFSET);
    if (list->m_Count < 7) {
        list->m_pModels[list->m_Count] = model;
        list->m_Count++;
    }
}

// 0x00271608 (name is ours)
void PlayerHeadBank::StartMiiExpressionAnim(u16 id, f32 frame)
{
    const HumanAnimInfo* info = Func6E6268(id);
    MiiExpressionAnim& anim = m_MiiExpressionAnim;
    anim.m_AnimId = id;
    anim.m_Frame = frame;
    anim.m_Expression = 0;
    anim.m_EndFrame = 0.0f;
    if ((info->flags & ANIM_FLAG_LOOP) != 0) {
        f32 endFrame = 0.0f;
        const nw::anim::res::ResAnimData* data = Func770970(&g_HumanAnimTable, id)->anim0.ptr();
        if (data != NULL) {
            endFrame = data->frameSize;
        }
        anim.m_EndFrame = endFrame;
    }
    anim.Update();
}

// 0x00271680 (name is ours)
void PlayerHeadBank::Update()
{
    m_FaceAnimControls[GetCurrentSlot()].Update(true);
    HeadMii& mii = m_Miis[GetCurrentSlot()];
    if (mii.m_IsUsed && &mii.m_Model != NULL) {
        m_MiiExpressionAnim.Update();
        const s32 expression = m_MiiExpressionAnim.m_Expression;
        // (a virtual call in the original)
        static_cast<cfl::MiiModelBase&>(mii.m_Model).SetExpression(expression);
        return;
    }
    m_MiiExpressionAnim.Update();
}

// 0x00271708 (name is ours)
void PlayerHeadBank::MiiExpressionAnim::Update()
{
    if (m_AnimId < HumanAnimName::INVALID) {
        const f32 frame = m_Frame;
        const nw::anim::res::ResAnimData* anim = Func770970(&g_HumanAnimTable, m_AnimId)->anim1.ptr();
        const nw::anim::res::ResTextureAnimData* member = NULL;
        if (anim != NULL) {
            const nw::ut::ResDicPatricia dictionary = {
                static_cast<const nw::ut::ResDicPatricia::Data*>(anim->memberAnimDic.ToPtr())};
            member = static_cast<const nw::anim::res::ResTextureAnimData*>(dictionary.GetDataByIndex(0));
        }
        if (member != NULL) {
            f32 value;
            if ((member->flags & 1) != 0) {
                value = member->index.constantValue;
            } else {
                value = nw::anim::res::CalcFloatCurve(
                    static_cast<const nw::anim::res::ResFloatCurveData*>(member->index.curve.ToPtr()), frame);
            }
            const nw::ut::Offset* textures = static_cast<const nw::ut::Offset*>(member->textures.ToPtr());
            const nw::gfx::res::ResReferenceTextureData* texture =
                static_cast<const nw::gfx::res::ResReferenceTextureData*>(
                    textures[static_cast<u32>(value)].ToPtr());
            const char* name = static_cast<const char*>(texture->path.ToPtr());
            const u32 digit = name[2] - '0';
            if (digit < 8) {
                m_Expression = s_MiiExpressions[digit];
            }
        } else {
            m_Expression = 0;
            m_AnimId = HumanAnimName::INVALID;
        }
    } else {
        m_Expression = 0;
        m_AnimId = HumanAnimName::INVALID;
    }
    m_Frame += 1.0f;
    if (m_EndFrame != 0.0f && m_EndFrame <= m_Frame) {
        m_Frame = m_EndFrame;
    }
}

// 0x0027184C (name is ours)
bool PlayerHeadBank::SetFaceColors(const ExchangeBankKeywordUnknown3* colors)
{
    if (m_pPlayer == NULL) {
        return false;
    }
    const u32 slot = GetCurrentSlot();
    g3d::ResourceLoader* loader = GetLoader(slot, 0);
    if (loader->m_SetupResult == g3d::ResourceSet::SETUP_RESULT_NONE || !loader->m_GraphicsFile.IsValid() ||
        loader->m_GraphicsFile.ptr()->models.count == 0) {
        return false;
    }
    const Keyword& keyword = m_Keywords[slot];
    Func1CEEF0(static_cast<const nw::gfx::res::ResModelData*>(loader->GetGraphicsFile().GetModels(0)),
               keyword.hairColor, keyword.eyeColor, colors, FACE_COLOR_KIND);
    return true;
}

// 0x002718F8 slot 0x10 (name is ours)
bool PlayerHeadBank::LoadResource(const Keyword& keyword, u32 slot)
{
    const u8 hairStyle = keyword.hairStyle;
    SvFgName item(keyword.item);
    if (IsMiiMask(&item)) {
        return true;
    }
    sead::FixedSafeString<64> path;
    if (!GetHeadModelPath(&path, hairStyle, item)) {
        return false;
    }
    sead::Heap* heap = GetAllocator(slot)->m_pHeap;
    if (!GetLoader(slot, 0)->Load(path, heap, FILE_ALIGNMENT)) {
        return false;
    }
    return true;
}

// 0x002719F4 slot 0x18 (name is ours)
bool PlayerHeadBank::Initialize(const Keyword& keyword, u32 slot, bool isReady)
{
    ssys::ma::HeapAllocator* allocator = GetAllocator(slot);
    HeadMii& mii = m_Miis[slot];
    mii.m_IsUsed = false;
    if (IsMiiMask(&keyword.item)) {
        if (m_pPlayer == NULL) {
            return false;
        }
        mii.m_IsUsed = true;
        cfl::MiiModelBase::Description description;
        description.resolution = MII_RESOLUTION;
        description.expressionMask = MII_EXPRESSION_MASK;
        description.flags = 0;
        description.data = reinterpret_cast<const u8*>(m_pPlayer) + MII_DATA_OFFSET;
        description.expression = 0;
        if (!static_cast<cfl::MiiModelBase&>(mii.m_Model).Initialize(allocator, MII_SOURCE_DATA, 0, description)) {
            return false;
        }
        const nn::math::VEC3 scale(s_MiiScale, s_MiiScale, s_MiiScale);
        mii.m_Model.SetScale(scale);
        return true;
    }
    if (!isReady) {
        return false;
    }
    g3d::ResourceLoader* model = GetLoader(slot, 0);
    PlayerFaceBank* faceBank;
    if (keyword.m_Unknown10) {
        faceBank = m_pFaceBanks[1] != NULL ? m_pFaceBanks[1] : m_pFaceBanks[0];
    } else {
        faceBank = m_pFaceBanks[0] != NULL ? m_pFaceBanks[0] : m_pFaceBanks[1];
    }
    model->Read(0x10000, 0x30000, 0x20000);
    model->SetHeaps(GetVramAHeap(slot), GetVramBHeap(slot), NULL);
    if (m_pPlayer != NULL) {
        Func1CEEF0(static_cast<const nw::gfx::res::ResModelData*>(model->GetGraphicsFile().GetModels(0)),
                   keyword.hairColor, keyword.eyeColor, &keyword.m_Unknown11, FACE_COLOR_KIND);
    }
    if (faceBank != NULL) {
        model->InitializeCgfx(allocator, &faceBank->m_Loader, true);
    }
    model->InitializeCgfx(allocator, Func5C22D8(), true);
    model->InitializeCgfx(allocator, Func536700(), true);
    model->InitializeCgfx(allocator, NULL, true);
    g3d::SetLightSetIndex(static_cast<const nw::gfx::res::ResModelData*>(model->GetGraphicsFile().GetModels(0)),
                          Func1E891C());
    g3d::SkeletalModel& skeletalModel = m_Models[slot];
    if (!skeletalModel.Create(nw::gfx::res::ResSceneObject(model->GetGraphicsFile().GetModels(0)), allocator,
                              allocator, 0, true, HEAD_MODEL_CREATE_FLAGS)) {
        return false;
    }
    HumanResTextureAnimBank* bankA = m_pTextureAnimBanks[0][slot];
    HumanResTextureAnimBank* bankB = m_pTextureAnimBanks[1][slot];
    if (faceBank != NULL && bankA != NULL && bankB != NULL) {
        m_FaceAnimControls[slot].Initialize(&skeletalModel, bankA, bankB, model, &faceBank->m_Loader, true);
    }
    return true;
}

// 0x00271D04 slot 0x1C (name is ours)
bool PlayerHeadBank::Finalize(const Keyword& keyword, u32 slot)
{
    HeadMii& mii = m_Miis[slot];
    if (mii.m_IsUsed && &mii.m_Model != NULL) {
        if (!mii.m_Model.Release()) {
            return false;
        }
        mii.m_IsUsed = false;
        return true;
    }
    m_FaceAnimControls[slot].Finalize();
    m_Models[slot].Finalize();
    GetLoader(slot, 0)->Unload();
    return true;
}

// 0x00271D84 slot 0x14 (name is ours)
s32 PlayerHeadBank::LoadResourceSplit(const Keyword& keyword, u32 slot)
{
    const u8 hairStyle = keyword.hairStyle;
    SvFgName item(keyword.item);
    if (IsMiiMask(&item)) {
        return 2;
    }
    sead::FixedSafeString<64> path;
    if (!GetHeadModelPath(&path, hairStyle, item)) {
        return 2;
    }
    sead::Heap* heap = GetAllocator(slot)->m_pHeap;
    g3d::ResourceLoader* loader = GetLoader(slot, 0);
    if (!loader->LoadAsync(path, heap, FILE_ALIGNMENT)) {
        return 0;
    }
    if (loader->IsLoadFailed()) {
        loader->Unload();
        return 2;
    }
    return 1;
}

// 0x00271EC8 (name is ours)
bool PlayerHeadBank::GetHeadModelPath(sead::BufferedSafeStringBase<char>* path, u8 hairStyle, SvFgName item)
{
    const bool isBoy = hairStyle < HAIR_STYLE_NUM;
    s32 style = hairStyle % HAIR_STYLE_NUM;
    if (style >= static_cast<s32>(HAIR_STYLE_NUM)) {
        style = HAIR_STYLE_MSD;
    }
    const char* sex = isBoy ? "boy" : "girl";
    const s32 kind = Func311840(&item);
    const bool isMsd = style == static_cast<s32>(HAIR_STYLE_MSD);
    if (kind == HEADWEAR_NO_HEAD) {
        return false;
    }
    switch (kind) {
    case HEADWEAR_BALD_EAR_0:
    case HEADWEAR_BALD_EAR_1: {
        sead::StringFormatter formatter(path);
        formatter.append("Player/head_mdl/bald_ear.bcres").flush();
        break;
    }
    case HEADWEAR_BALD_0:
    case HEADWEAR_BALD_1: {
        sead::StringFormatter formatter(path);
        formatter.append("Player/head_mdl/bald.bcres").flush();
        break;
    }
    case HEADWEAR_MODEL_B:
        if (isMsd) {
            sead::StringFormatter formatter(path);
            formatter.append("Player/head_mdl/%s_msd_b.bcres").append(sex).flush();
        } else {
            sead::StringFormatter formatter(path);
            formatter.append("Player/head_mdl/%s%02db.bcres").append(sex).appendValue(style).flush();
        }
        break;
    default:
        if (isMsd) {
            sead::StringFormatter formatter(path);
            formatter.append("Player/head_mdl/%s_msd_a.bcres").append(sex).flush();
        } else {
            sead::StringFormatter formatter(path);
            formatter.append("Player/head_mdl/%s%02da.bcres").append(sex).appendValue(style).flush();
        }
        break;
    }
    return true;
}

// 0x00272234 (name is ours)
void PlayerHeadBank::SetBoneMatrix(g3d::SkeletalModel* parent, u32 bone)
{
    HeadMii& mii = m_Miis[GetCurrentSlot()];
    if (mii.m_IsUsed && &mii.m_Model != NULL) {
        const nn::math::MTX34* boneMtx = Func7258B0(this, parent, bone, NULL);
        if (boneMtx != NULL) {
            nn::math::MTX34 worldMtx;
            worldMtx = *boneMtx;
            nn::math::MTX34 mtx(worldMtx);
            ssys::ma::RotateZ(&mtx, MII_BONE_ROTATE);
            mii.m_Model.SetMatrix(mtx);
        }
        return;
    }
    Func3286CC(this, parent, bone);
}

// 0x002722C8 (name is ours)
bool PlayerHeadBank::SetBrightness(u32 index, f32 brightness)
{
    HeadMii& mii = m_Miis[GetCurrentSlot()];
    if (mii.m_IsUsed && &mii.m_Model != NULL) {
        mii.m_Model.SetColor(nw::ut::FloatColor(brightness, brightness, brightness, 1.0f));
        return true;
    }
    return Func328808(this, index, brightness);
}

// 0x00272388 (name is ours)
HumanFaceAnimControl* PlayerHeadBank::GetFaceAnimControl()
{
    if (static_cast<const PlayerHeadBank*>(this)->GetCurrentModel() == NULL) {
        return NULL;
    }
    return &m_FaceAnimControls[GetCurrentSlot()];
}

// 0x002723EC (name is ours)
void PlayerHeadBank::SetBanks(PlayerFaceBank* faceBank0, PlayerFaceBank* faceBank1, const SvPlayerAppearance* player,
                              HumanResTextureAnimBank* textureAnimBankA0, HumanResTextureAnimBank* textureAnimBankA1,
                              HumanResTextureAnimBank* textureAnimBankB0, HumanResTextureAnimBank* textureAnimBankB1)
{
    m_pFaceBanks[0] = faceBank0;
    m_pFaceBanks[1] = faceBank1;
    m_pPlayer = player;
    m_pTextureAnimBanks[0][0] = textureAnimBankA0;
    m_pTextureAnimBanks[0][1] = textureAnimBankA1;
    m_pTextureAnimBanks[1][0] = textureAnimBankB0;
    m_pTextureAnimBanks[1][1] = textureAnimBankB1;
}

// 0x00272404
PlayerHeadBank::PlayerHeadBank()
{
    m_MiiExpressionAnim.m_Frame = 0.0f;
    m_MiiExpressionAnim.m_Expression = 0;
    m_MiiExpressionAnim.m_EndFrame = 0.0f;
    m_MiiExpressionAnim.m_AnimId = HumanAnimName::INVALID;
    ClearBanks();
}

// 0x00272798 slot 0x00
// 0x002726C4 (deleting dtor)
PlayerHeadBank::~PlayerHeadBank()
{
    ClearBanks();
}
