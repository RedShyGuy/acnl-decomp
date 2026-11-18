#include <cstring>

#include "bg/dCharRoom.h"
#include "Camera/dCameraMgr.h"
#include "Other/dHeapInit.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvGardenPlus.h"
#include "Sv/dSvHouse.h"
#include "field/field_Extern.h"
#include "g3d/g3d_System.h"
#include "nw/gfx/gfx_Api.h"
#include "nw/gfx/gfx_Material.h"
#include "nw/gfx/gfx_Model.h"
#include "nw/gfx/res/gfx_ResModel.h"
#include "ssys/ma/ma_Math.h"
#include "stage/stage_Api.h"
#include "stage/stage_Extern.h"

// the models and the bones of the landings by direction
// 0x0094FE1C
const char* s_LandingModelNames[CharRoomLanding::PART_NUM] = {"stairs_s", "stairs_e", "stairs_n", "stairs_w"};
// 0x0094FE2C
const char* s_LandingBoneNames[CharRoomLanding::PART_NUM] = {"pos_stairsS", "pos_stairsE", "pos_stairsN",
                                                             "pos_stairsW"};

namespace {
// the alpha of the material m_fade_wall
const s32 FADE_WALL_ALPHA = 220;
// the house size (SvHouseExterior::houseSize) whose landings get the texture landing1 of the extra file
const u8 HOUSE_SIZE_EXTRA_LANDING = 7;
// the room size (SvRoomState::roomSize) whose wallpaper on the landings is moved by half a texture
const u8 ROOM_SIZE_SHIFTED = 3;

// (inline) the position of a material in the materials of a model (-1: none)
inline s32 GetMaterialIndex(const nw::gfx::res::ResModelData* model, const char* name)
{
    return model->materials.GetDic().GetIndex(name);
}

// (inline) the alpha of material index of a landing by the camera
inline void FadeMaterial(CharRoomLanding::Part* part, s32 index)
{
    if (index < 0) {
        return;
    }
    nw::gfx::res::ResMaterialData* material = part->m_Model.GetModel()->GetMaterial(index)->GetBuffer().ptr();
    const camera::Mgr* camera;
    if (material == NULL || (camera = camera::Mgr::s_pInstance) == NULL) {
        return;
    }
    const nn::math::MTX34& world = part->m_Model.GetWorldTransform().m_TransformMatrix;
    const nn::math::VEC3 position(world.m[0][3], world.m[1][3], world.m[2][3]);
    const u8 alpha = static_cast<u32>(Func766E80(camera, &position, part->m_FadeAngle) * 255.0f);
    g3d::SetConstantColor(material, 1, -1, -1, -1, alpha);
}
} // namespace

// 0x0060DD94 (name is ours)
void CharRoomLanding::Part::Initialize(const u32* direction, g3d::ResourceLoader* room, f32 height)
{
    m_Allocator.CreateExpHeap(12288, HeapInit::s_pStageHeap, sead::SafeStringBase<char>("LandingParts"),
                              sead::Heap::HEAP_DIRECTION_FORWARD, false);
    const void* const* file = reinterpret_cast<const void* const*>(&room->m_GraphicsFile);
    const nw::gfx::res::ResSkeletalModelData* position =
        static_cast<const nw::gfx::res::ResSkeletalModelData*>(nw::gfx::GetModel(file, "position"));
    const nw::gfx::res::ResModelData* model =
        static_cast<const nw::gfx::res::ResModelData*>(nw::gfx::GetModel(file, s_LandingModelNames[*direction]));
    if (model == NULL) {
        return;
    }
    const s32 bone = g3d::GetBoneIndex(position, s_LandingBoneNames[*direction]);
    if (bone >= 0) {
        m_Model.Create(nw::gfx::res::ResSceneObject(model), &m_Allocator, &m_Allocator, 4, true, 1);
        m_Model.SortMeshes();
        g3d::GetBoneTranslate(&m_Position, position, bone);
        nn::math::MTX34 matrix;
        matrix.m[0][0] = 1.0f;
        matrix.m[0][1] = 0.0f;
        matrix.m[0][2] = 0.0f;
        matrix.m[0][3] = m_Position.x;
        matrix.m[1][0] = 0.0f;
        matrix.m[1][1] = 1.0f;
        matrix.m[1][2] = 0.0f;
        matrix.m[1][3] = m_Position.y + height;
        matrix.m[2][0] = 0.0f;
        matrix.m[2][1] = 0.0f;
        matrix.m[2][2] = 1.0f;
        matrix.m[2][3] = m_Position.z;
        const u32 copy = *direction;
        ssys::ma::RotateY(&matrix, Func6F9090(&copy));
        m_Model.SetMatrix(matrix);
        m_Model.Update();
    }
    m_FadeIndex = GetMaterialIndex(model, "m_fade");
    m_FadeWallIndex = GetMaterialIndex(model, "m_fade_wall");
    const u32 copy = *direction;
    m_FadeAngle = Func6F9090(&copy) - 0x8000;
    g3d::SetConstantColor(nw::gfx::res::ResModel(model).GetMaterial("m_fade_wall").ptr(), 0, -1, -1, -1, FADE_WALL_ALPHA);
    nw::gfx::Model* gfxModel = m_Model.GetModel();
    if (gfxModel != NULL) {
        g3d::GetTextureTranslate(&m_TexTranslate, gfxModel->GetResModel().ptr(), "m_fade_wall", 0);
    }
}

// 0x0060E0BC (name is ours)
bool CharRoomLanding::Request(const RoomCovering* covering)
{
    bool isChanged = false;
    const s32 result = m_Bank.Exchange(Bank::Keyword(covering->item));
    if (result == 2) {
        item::ResLoader* loader = m_Bank.GetLoader(m_Bank.GetCurrentSlot(), 0);
        for (u32 i = 0; i < PART_NUM; i++) {
            Part& part = m_Parts[i];
            f32 shift = 0.0f;
            if (stage::IsKind(stage::KIND_PLAYER_HOUSE, stage::GetCurrent()) ||
                stage::IsKind(stage::KIND_MODEL_HOUSE, stage::GetCurrent())) {
                if (covering->item.GetKind() != item::KIND_MY_DESIGN_TEXTURE) {
                    const SvRoom* room = Func6FA758(stage::GetCurrent());
                    if (room != NULL && room->flags.roomSize == ROOM_SIZE_SHIFTED) {
                        shift = 0.5f;
                    }
                }
            } else if (stage::IsKind(stage::KIND_NPC_HOUSE, stage::GetCurrent())) {
                shift = 0.5f;
            }
            const nn::math::VEC2 translate(part.m_TexTranslate.x + shift, part.m_TexTranslate.y + 0.0f);
            Func52985C(&part.m_Model, loader, covering, "m_fade_wall", &translate);
            Func529EF8(&part.m_Model, covering->param, "m_fade_wall", covering);
        }
        isChanged = true;
    } else if (result == 1) {
        for (u32 i = 0; i < PART_NUM; i++) {
            Func529EF8(&m_Parts[i].m_Model, covering->param, "m_fade_wall", covering);
        }
    }
    return isChanged;
}

// 0x0060E340 (name is ours)
void CharRoomLanding::Initialize(g3d::ResourceLoader* room, g3d::ResourceLoader* extra, f32 height)
{
    m_Bank.CreateHeaps(HeapInit::s_pStageHeap, "踊り場の壁紙", 0);
    m_Bank.m_Alignment = 1;
    for (u32 i = 0; i < PART_NUM; i++) {
        m_Parts[i].Initialize(&i, room, height);
    }
    if (stage::IsKind(stage::KIND_PLAYER_HOUSE, stage::GetCurrent()) ||
        stage::IsKind(stage::KIND_MODEL_HOUSE, stage::GetCurrent())) {
        bool isExtra = false;
        if (stage::IsKind(stage::KIND_PLAYER_HOUSE, stage::GetCurrent())) {
            const u8 index = Func6FB278(GetFieldName(stage::GetCurrent()));
            if (GetGardenPlus()->data.townData.playerHouses.Get(index)->exterior1.houseSize == HOUSE_SIZE_EXTRA_LANDING) {
                isExtra = true;
            }
        }
        if (stage::IsKind(stage::KIND_MODEL_HOUSE, stage::GetCurrent())) {
            const u8 structure = stage::GetLastStructure();
            if (Func6CB818(structure)) {
                const u8* object = static_cast<const u8*>(Func6CC658(structure));
                if (object != NULL &&
                    reinterpret_cast<const SvPlayerHouse*>(object + 16)->exterior1.houseSize == HOUSE_SIZE_EXTRA_LANDING) {
                    isExtra = true;
                }
            }
        }
        if (isExtra) {
            for (u32 i = 0; i < PART_NUM; i++) {
                Part& part = m_Parts[i];
                if (extra == NULL) {
                    continue;
                }
                const nw::gfx::res::ResTexture texture(extra->GetGraphicsFile().ptr()->textures.GetData("landing1"));
                if (!texture.IsValid() || part.m_Model.GetModel() == NULL) {
                    continue;
                }
                const nw::gfx::res::ResMaterial material = part.m_Model.GetModel()->GetResModel().GetMaterial("m_stairs");
                if (material.IsValid()) {
                    g3d::SetTexture(material, "landing0", texture);
                }
            }
        }
    }
    m_IsInitialized = true;
}

// 0x0060E61C (name is ours)
void CharRoomLanding::Calc()
{
    for (u32 i = 0; i < PART_NUM; i++) {
        Part& part = m_Parts[i];
        if (part.m_Model.GetModel() == NULL) {
            continue;
        }
        FadeMaterial(&part, part.m_FadeIndex);
        FadeMaterial(&part, part.m_FadeWallIndex);
    }
}

// 0x0060E7D4 (name is ours)
void CharRoomLanding::Finalize()
{
    for (u32 i = 0; i < PART_NUM; i++) {
        Part& part = m_Parts[i];
        part.m_Model.Finalize();
        part.m_Allocator.DestroyHeap();
    }
    m_Bank.FinalizeBank();
}
