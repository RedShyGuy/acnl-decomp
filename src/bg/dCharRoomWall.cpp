#include "bg/dCharRoom.h"
#include "Camera/dCameraMgr.h"
#include "Other/dHeapInit.h"
#include "Sv/dSvBgName.h"
#include "field/dFdInfo.h"
#include "field/dField.h"
#include "g3d/g3d_System.h"
#include "nw/gfx/gfx_Api.h"
#include "nw/gfx/gfx_Model.h"
#include "nw/gfx/res/gfx_ResModel.h"
#include "nw/gfx/res/gfx_ResSkeleton.h"
#include "ssys/ma/ma_Math.h"
#include "stage/stage_Api.h"

// the models and the bones of the walls by direction
// 0x0094FDFC
const char* s_WallModelNames[CharRoomWall::PART_NUM] = {"wall_s", "wall_e", "wall_n", "wall_w"};
// 0x0094FE0C
const char* s_WallBoneNames[CharRoomWall::PART_NUM] = {"pos_wallS", "pos_wallE", "pos_wallN", "pos_wallW"};

namespace {
// the normals of the walls by direction
// 0x008473E0
const nn::math::VEC3_ s_WallNormals[CharRoomWall::PART_NUM] = {
    {0.0f, 0.0f, -1.0f},
    {-1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 1.0f},
    {1.0f, 0.0f, 0.0f},
};
// the grounds whose east wall model is turned around
// 0x00847420
const u16 s_TurnedEastWallGrounds[] = {
    BG_NAME_IDR_ROOM_1_C_8_SEU,   BG_NAME_IDR_ROOM_1_C_8_SWEU,  BG_NAME_IDR_ROOM_1_C_8_NSEU,   BG_NAME_IDR_ROOM_1_C_8_SEUD,
    BG_NAME_IDR_ROOM_1_C_8_NSWEU, BG_NAME_IDR_ROOM_1_C_8_SWEUD, BG_NAME_IDR_ROOM_1_C_8_NSEUD,  BG_NAME_IDR_ROOM_1_C_8_NSWEUD,
    BG_NAME_IDR_ROOM_1_W_4_E,     BG_NAME_IDR_ROOM_1_W_6_E,     BG_NAME_IDR_ROOM_1_W_8_E,     BG_NAME_IDR_MUSEUM_P_1,
    BG_NAME_IDR_MUSEUM_P_4,
};
const u32 TURNED_EAST_WALL_GROUND_NUM = sizeof(s_TurnedEastWallGrounds) / sizeof(s_TurnedEastWallGrounds[0]);

const u32 DIRECTION_EAST = 1;
const u32 DIRECTION_WEST = 3;
// the alpha of the material m_wall
const s32 WALL_ALPHA = 220;
} // namespace

// 0x0052B734 (name is ours)
void CharRoomWall::Part::Initialize(const u32* direction, g3d::ResourceLoader* room, f32 height)
{
    m_Allocator.CreateExpHeap(12288, HeapInit::s_pStageHeap, sead::SafeStringBase<char>("WallParts"),
                              sead::Heap::HEAP_DIRECTION_FORWARD, false);
    const void* const* file = reinterpret_cast<const void* const*>(&room->m_GraphicsFile);
    const nw::gfx::res::ResSkeletalModelData* position =
        static_cast<const nw::gfx::res::ResSkeletalModelData*>(nw::gfx::GetModel(file, "position"));
    // (a guarded static of four s16 {-0x8000, 0, 0, 0} at 0x0094FDF4, guard 0x0094FDF0, is set up here and not read)
    u16 ground = BG_NAME_IDR_ROOM_1_C_6_S;
    Field* field = FdInfo::GetCurrentField();
    if (field != NULL) {
        const BgInfo* bg = field->GetBg(0, 0);
        if (bg != NULL) {
            ground = bg->GetName();
        }
    }
    const s32 bone = g3d::GetBoneIndex(position, s_WallBoneNames[*direction]);
    if (bone >= 0) {
        m_Model.Create(nw::gfx::res::ResSceneObject(nw::gfx::GetModel(file, s_WallModelNames[*direction])), &m_Allocator,
                       &m_Allocator, 0, true, 1);
        m_Model.SortMeshes();
        const nw::gfx::res::ResSkeletonData* skeleton =
            static_cast<const nw::gfx::res::ResSkeletonData*>(position->skeleton.ToPtr());
        nw::ut::ResDicPatricia bones = {skeleton->boneDic.To<nw::ut::ResDicPatricia::Data>()};
        const nw::gfx::res::ResBoneData* boneData = static_cast<const nw::gfx::res::ResBoneData*>(bones.GetDataByIndex(bone));
        const nn::math::MTX34 local = boneData->localMatrix;
        s16 angles[3] = {0, 0, 0};
        Func750BD8(&local, angles);
        m_Angle = angles[1];
        const u32 copy = *direction;
        m_FadeAngle = Func6F9090(&copy) - 0x8000;
        if (*direction == DIRECTION_EAST || *direction == DIRECTION_WEST) {
            if (stage::GetCurrent() == stage::NAME_MUSEUM_SHOWROOM_2 || stage::GetCurrent() == stage::NAME_MUSEUM_SHOWROOM_4) {
                m_FadeAngle -= 0x8000;
            }
            if (*direction == DIRECTION_EAST) {
                for (u32 i = 0; i < TURNED_EAST_WALL_GROUND_NUM; i++) {
                    if (s_TurnedEastWallGrounds[i] == ground) {
                        m_Angle -= 0x8000;
                        break;
                    }
                }
            }
        }
        m_Position.x = local.m[0][3];
        m_Position.y = local.m[1][3];
        m_Position.z = local.m[2][3];
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
        ssys::ma::RotateY(&matrix, m_Angle);
        m_Model.SetMatrix(matrix);
        m_Model.Update();
    }
    m_Normal.x = s_WallNormals[*direction].x;
    m_Normal.y = s_WallNormals[*direction].y;
    m_Normal.z = s_WallNormals[*direction].z;
    nw::gfx::Model* model = m_Model.GetModel();
    if (model != NULL) {
        const nw::gfx::res::ResModel res = model->GetResModel();
        if (res.IsValid()) {
            g3d::SetConstantColor(res.GetMaterial("m_wall").ptr(), 0, -1, -1, -1, WALL_ALPHA);
        }
    }
    model = m_Model.GetModel();
    g3d::GetTextureTranslate(&m_TexTranslate, model != NULL ? model->GetResModel().ptr() : NULL, "m_wall", 0);
    m_pFadeMaterial = m_Model.GetModel()->GetResModel().GetMaterial("m_fade").ptr();
}

// 0x0052B404 (name is ours)
void CharRoomWall::Initialize(g3d::ResourceLoader* room, const RoomCovering* covering, f32 height)
{
    (void)covering;
    m_Bank.CreateHeaps(HeapInit::s_pStageHeap, "壁紙", 0);
    m_Bank.m_Alignment = 1;
    for (u32 i = 0; i < PART_NUM; i++) {
        m_Parts[i].Initialize(&i, room, height);
    }
    const void* const* file = reinterpret_cast<const void* const*>(&room->m_GraphicsFile);
    m_pWindowMaterialE = nw::gfx::res::ResModel(nw::gfx::GetModel(file, "wall_e")).GetMaterial("m_window").ptr();
    m_pWindowMaterialW = nw::gfx::res::ResModel(nw::gfx::GetModel(file, "wall_w")).GetMaterial("m_window").ptr();
    m_IsInitialized = true;
}

// 0x0052B284 (name is ours)
bool CharRoomWall::Request(const RoomCovering* covering)
{
    bool isChanged = false;
    const s32 result = m_Bank.Exchange(Bank::Keyword(covering->item));
    if (result == 2) {
        item::ResLoader* loader = m_Bank.GetLoader(m_Bank.GetCurrentSlot(), 0);
        for (u32 i = 0; i < PART_NUM; i++) {
            Part& part = m_Parts[i];
            Func52985C(&part.m_Model, loader, covering, "m_wall", &part.m_TexTranslate);
            Func529EF8(&part.m_Model, covering->param, "m_wall", covering);
        }
        isChanged = true;
    } else if (result == 1) {
        for (u32 i = 0; i < PART_NUM; i++) {
            Func529EF8(&m_Parts[i].m_Model, covering->param, "m_wall", covering);
        }
    }
    return isChanged;
}

// 0x0052B4E0 (name is ours)
void CharRoomWall::Calc()
{
    for (u32 i = 0; i < PART_NUM; i++) {
        Part& part = m_Parts[i];
        const camera::Mgr* camera;
        if (part.m_pFadeMaterial == NULL || (camera = camera::Mgr::s_pInstance) == NULL) {
            continue;
        }
        const nn::math::MTX34& world = part.m_Model.GetWorldTransform().m_TransformMatrix;
        const nn::math::VEC3 position(world.m[0][3], world.m[1][3], world.m[2][3]);
        const u8 alpha = static_cast<u32>(Func766E80(camera, &position, part.m_FadeAngle) * 255.0f);
        g3d::SetConstantColor(part.m_pFadeMaterial, 1, -1, -1, -1, alpha);
        part.m_IsVisible = true;
        const nn::math::VEC3_& eye = camera->m_pUnk0x1A4->m_Eye;
        nn::math::VEC3 toEye(eye.x - part.m_Position.x, eye.y - part.m_Position.y, eye.z - part.m_Position.z);
        ssys::ma::NormalizeIfPossible(&toEye);
        const f32 dot = part.m_Normal.x * toEye.x + part.m_Normal.y * toEye.y + part.m_Normal.z * toEye.z;
        if (alpha == 0 && dot < 0.001f) {
            part.m_IsVisible = false;
        }
    }
}

// 0x0052B630 (name is ours)
void CharRoomWall::Finalize()
{
    m_Bank.FinalizeBank();
    for (u32 i = 0; i < PART_NUM; i++) {
        Part& part = m_Parts[i];
        part.m_Model.Finalize();
        part.m_Allocator.DestroyHeap();
    }
}
