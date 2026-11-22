#include "Npc/dNpcAccessoryModel.h"

#include "Bs/dBsNpcMgr.h"
#include "Human/dHumanModel.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvGardenPlus.h"
#include "g3d/dModel.h"
#include "g3d/g3d_System.h"
#include "nw/gfx/gfx_Model.h"
#include "nw/gfx/res/gfx_ResModel.h"
#include "nw/gfx/res/gfx_ResSceneObject.h"
#include "sead/seadFormatFixedSafeString.h"
#include "sead/seadMathCalcCommon.h"
#include "script/dScriptExtern.h"
#include "ssys/ma/dAllocator.h"
#include "ssys/ma/ma_Math.h"
#include "stage/stage_Api.h"

namespace {
// the accessory kinds (BsNpcMgr::m_AccessoryKind; names are ours)
const u16 ACCESSORY_NEW_YEARS_HAT = 0;
const u16 ACCESSORY_KIND_1 = 1;

// the degrees of an accessory rotation as an Angle16
// 0x008AF154
extern const f32 s_DegreeToAngle16 = 65536.0f / 360.0f;

// the variants (texture, model) of the accessory of kind 1 by the villager value (the image 0x008821D8)
struct AccessoryVariant
{
    u16 model;
    u16 texture;
};

} // namespace

// 0x001CC0DC
void NpcAccessoryModel::Update(g3d::SkeletalModel* parent)
{
    const nw::gfx::CalculatedTransform* bone = parent->GetBoneWorldTransform(m_Bone);
    if (bone == NULL) {
        return;
    }
    nn::math::MTX34 mtx = bone->m_TransformMatrix;
    const nn::math::VEC3 scale(m_Scale, m_Scale, m_Scale);
    sead::Matrix34CalcCtr<f32>::multiply(mtx, mtx, m_OffsetMtx);
    m_Model.SetMatrix(mtx);
    m_Model.SetScale(scale);
    m_Model.Update();
}

// 0x001CC150
bool NpcAccessoryModel::ReadTransform(g3d::Model* model, const char* name, nn::math::VEC3* translate, nn::math::VEC3* rotate,
                           f32* scale)
{
    bool found = false;
    sead::FormatFixedSafeString<32> nameT("%s_t", name);
    sead::FormatFixedSafeString<32> nameR("%s_r", name);
    sead::FormatFixedSafeString<32> nameS("%s_s", name);
    nw::gfx::Model* node = model->GetModel();
    nw::gfx::res::ResSceneObject object(NULL);
    if (node != NULL) {
        object = node->GetResSceneObject();
    }
    s32 index = object.GetUserDataIndex(nameT.getCString());
    if (index >= 0 && object.GetUserDataCount() > index) {
        const f32* data = static_cast<const f32*>(object.GetUserData(index));
        if (reinterpret_cast<const s32*>(data)[2] == 0) {
            found = true;
            translate->x = data[4];
            translate->y = data[5];
            translate->z = data[6];
        }
    }
    index = object.GetUserDataIndex(nameR.getCString());
    if (index >= 0 && object.GetUserDataCount() > index) {
        const f32* data = static_cast<const f32*>(object.GetUserData(index));
        if (reinterpret_cast<const s32*>(data)[2] == 0) {
            found = true;
            rotate->x = data[4];
            rotate->y = data[5];
            rotate->z = data[6];
        }
    }
    index = object.GetUserDataIndex(nameS.getCString());
    if (index >= 0 && object.GetUserDataCount() > index) {
        const f32* data = static_cast<const f32*>(object.GetUserData(index));
        if (reinterpret_cast<const s32*>(data)[2] == 0) {
            found = true;
            *scale = data[4];
        }
    }
    return found;
}

// 0x001CC2EC
bool NpcAccessoryModel::Setup(u16 kind, ssys::ma::Allocator* allocator, g3d::SkeletalModel* parent,
                              const stage::NpcInfo* info)
{
    BsNpcMgr* mgr = BsNpcMgr::s_pInstance;
    if (mgr == NULL || !mgr->m_LoadFlags.Test(BsNpcMgr::LOAD_ACCESSORY) || mgr->m_AccessoryKind != kind) {
        return false;
    }
    if (m_State == kind) {
        return true;
    }
    if (m_State != STATE_NONE) {
        return false;
    }
    const char* name = Func117820(kind);
    u32 texture = 0;
    u32 model = 0;
    if (info->kind == stage::NpcInfo::KIND_ID) {
        const u32 value = Func71C420(reinterpret_cast<const u16*>(&info->value));
        if (kind == ACCESSORY_NEW_YEARS_HAT) {
            const SvFgName* hats = &GetGardenPlus()->data.townData.unk_0xEE2C.unk_0x17A24;
            if (hats != NULL) {
                SvFgName hat = Func71CE28(hats, value <= 3);
                switch (GetFgNameId(&hat)) {
                case SvFgName::NAME_RED_NEW_YEARS_HAT:
                    break;
                case SvFgName::NAME_Y_NEW_YEARS_HAT:
                    texture = 1;
                    model = 1;
                    break;
                case SvFgName::NAME_BLUE_NEW_YEARS_HAT:
                    texture = 2;
                    model = 2;
                    break;
                case SvFgName::NAME_GREEN_NEW_YEARS_HAT:
                    texture = 3;
                    model = 3;
                    break;
                }
            }
        } else if (kind == ACCESSORY_KIND_1 && value < 8) {
            // 0x008821D8
            static const AccessoryVariant s_Variants[8] = {
                {0, 3}, {0, 1}, {0, 0}, {0, 2}, {0, 3}, {0, 1}, {0, 2}, {0, 0},
            };
            texture = s_Variants[value].texture;
            model = s_Variants[value].model;
        }
    }
    const nw::gfx::res::ResModelData* res =
        static_cast<const nw::gfx::res::ResModelData*>(mgr->m_AccessoryLoader.m_GraphicsFile.GetModels(model));
    if (kind < 2 && res != NULL && texture <= 4) {
        sead::FormatFixedSafeString<32> textureName("%s.0", name);
        sead::FormatFixedSafeString<32> variantName("%s.%d", name, texture);
        void* resTexture = NULL;
        if (variantName.getCString() != NULL) {
            resTexture = mgr->m_AccessoryLoader.m_GraphicsFile.ptr()->textures.GetData(variantName.getCString());
        }
        void* material = NULL;
        if (name != NULL) {
            material = res->materials.GetData(name);
        }
        if (material != NULL && resTexture != NULL) {
            g3d::SetTexture(nw::gfx::res::ResMaterial(material), textureName.getCString(),
                            nw::gfx::res::ResTexture(resTexture));
        }
    }
    if (!m_Model.Create(nw::gfx::res::ResSceneObject(res), allocator, allocator, 0x1FFF, true, 1)) {
        return false;
    }
    nn::math::VEC3 translate(0.0f, 0.0f, 0.0f);
    nn::math::VEC3 rotate(0.0f, 0.0f, 0.0f);
    f32 scale = 1.0f;
    if (m_Loader.IsSucceeded()) {
        Func1AAFF8(&m_Loader.m_pFileEC, name, &translate, &rotate, &scale);
    } else {
        ReadTransform(parent, name, &translate, &rotate, &scale);
    }
    m_Bone = parent->GetBoneIndex(HumanModel::s_BoneNames[HumanModel::BONE_NAME_HEAD]);
    const s16 z = static_cast<s32>(rotate.z * s_DegreeToAngle16);
    const s16 y = static_cast<s32>(rotate.y * s_DegreeToAngle16);
    const s16 x = static_cast<s32>(rotate.x * s_DegreeToAngle16);
    ssys::ma::SetRotateZYX(&m_OffsetMtx, x, y, z);
    nn::math::MTX34 move;
    move.m[0][0] = 1.0f;
    move.m[0][1] = 0.0f;
    move.m[0][2] = 0.0f;
    move.m[0][3] = translate.x;
    move.m[1][0] = 0.0f;
    move.m[1][1] = 1.0f;
    move.m[1][2] = 0.0f;
    move.m[1][3] = translate.y;
    move.m[2][0] = 0.0f;
    move.m[2][1] = 0.0f;
    move.m[2][2] = 1.0f;
    move.m[2][3] = translate.z;
    sead::Matrix34CalcCtr<f32>::multiply(m_OffsetMtx, m_OffsetMtx, move);
    m_State = kind;
    m_Scale = scale;
    return true;
}

// 0x004EE3F4
NpcAccessoryModel::~NpcAccessoryModel()
{
    // (only the members are destroyed)
}
