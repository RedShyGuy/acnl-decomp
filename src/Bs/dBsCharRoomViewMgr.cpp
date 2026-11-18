#include <cfloat>

#include "Bs/dBsCharRoomViewMgr.h"
#include "Other/dHeapInit.h"
#include "Other/dTourData.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvBgName.h"
#include "Sv/dSvGardenPlus.h"
#include "Sv/dSvHouse.h"
#include "Sv/dSvMuseum.h"
#include "field/dFdInfo.h"
#include "field/dField.h"
#include "g3d/g3d_System.h"
#include "hobj/hobj_Extern.h"
#include "isfactory/isfactory_Api.h"
#include "netgame/dGameFunctions.h"
#include "netgame/netgame_Api.h"
#include "nw/gfx/gfx_Api.h"
#include "stage/stage_Api.h"
#include "time/time_Api.h"

// 0x0094FDEC
BsCharRoomViewMgr* BsCharRoomViewMgr::s_pInstance = NULL;

namespace {
// the size of the heap of the manager, of the model heap and of the heap of the room resources
const u32 HEAP_SIZE = 25600;
const u32 MODEL_HEAP_SIZE = 102400;
const u32 RESOURCE_HEAP_SIZE = 102400;

// the packets of a new wallpaper / flooring (names are ours)
const u8 PACKET_ROOM_WALLPAPER = 120;
const u8 PACKET_ROOM_FLOORING = 121;

// the grounds with the sunshine for the date tests A / B (A: at the bone pos_wallW, B: at pos_wallE turned around)
// 0x0084743A
const u16 s_SunshineGroundsA[] = {
    BG_NAME_IDR_ROOM_1_C_4_S,  BG_NAME_IDR_ROOM_1_C_6_S,   BG_NAME_IDR_ROOM_1_C_8_S,   BG_NAME_IDR_ROOM_1_C_8_SU,
    BG_NAME_IDR_ROOM_1_C_8_SEU, BG_NAME_IDR_ROOM_1_C_8_NSU, BG_NAME_IDR_ROOM_1_C_8_SUD, BG_NAME_IDR_ROOM_1_C_8_NSEU,
    BG_NAME_IDR_ROOM_1_C_8_SEUD, BG_NAME_IDR_ROOM_1_C_8_NSUD, BG_NAME_IDR_ROOM_1_C_8_NSEUD, BG_NAME_IDR_ROOM_1_N_4_S,
    BG_NAME_IDR_ROOM_1_N_6_S,  BG_NAME_IDR_ROOM_1_N_8_S,   BG_NAME_IDR_ROOM_1_W_4_E,   BG_NAME_IDR_ROOM_1_W_6_E,
    BG_NAME_IDR_ROOM_1_W_8_E,  BG_NAME_IDR_ROOM_2_C_4_D,   BG_NAME_IDR_ROOM_2_C_6_D,   BG_NAME_IDR_ROOM_2_C_8_D,
    BG_NAME_IDR_ROOM_N_P_C,
};
// 0x00847464
const u16 s_SunshineGroundsB[] = {
    BG_NAME_IDR_ROOM_1_C_4_S,  BG_NAME_IDR_ROOM_1_C_6_S,   BG_NAME_IDR_ROOM_1_C_8_S,   BG_NAME_IDR_ROOM_1_C_8_SU,
    BG_NAME_IDR_ROOM_1_C_8_SWU, BG_NAME_IDR_ROOM_1_C_8_NSU, BG_NAME_IDR_ROOM_1_C_8_SUD, BG_NAME_IDR_ROOM_1_C_8_NSWU,
    BG_NAME_IDR_ROOM_1_C_8_SWUD, BG_NAME_IDR_ROOM_1_C_8_NSUD, BG_NAME_IDR_ROOM_1_C_8_NSWUD, BG_NAME_IDR_ROOM_1_N_4_S,
    BG_NAME_IDR_ROOM_1_N_6_S,  BG_NAME_IDR_ROOM_1_N_8_S,   BG_NAME_IDR_ROOM_1_E_4_W,   BG_NAME_IDR_ROOM_1_E_6_W,
    BG_NAME_IDR_ROOM_1_E_8_W,  BG_NAME_IDR_ROOM_2_C_4_D,   BG_NAME_IDR_ROOM_2_C_6_D,   BG_NAME_IDR_ROOM_2_C_8_D,
    BG_NAME_IDR_ROOM_N_P_C,
};
const u32 SUNSHINE_GROUND_NUM = sizeof(s_SunshineGroundsA) / sizeof(s_SunshineGroundsA[0]);

// (inline) whether a ground is in a table
inline bool IsInGrounds(const u16* grounds, u16 ground)
{
    for (u32 i = 0; i < SUNSHINE_GROUND_NUM; i++) {
        if (grounds[i] == ground) {
            return true;
        }
    }
    return false;
}

// (inline) a packet to the other players
inline void SendCoveringPacket(u8 type, const RoomCovering* covering)
{
    if (netgame::IsMultiPlayer()) {
        netgame::EnqueuePacket(type, netgame::PLAYER_NO_TARGET_ALL, covering, sizeof(*covering));
    }
}

// (inline) the covering of the room of the current stage (a room of an own house or a museum room)
inline void SetRoomWallpaper(const RoomCovering* covering)
{
    if (stage::IsKind(stage::KIND_PLAYER_HOUSE, stage::GetCurrent())) {
        SvRoom* room = Func6FA758(stage::GetCurrent());
        if (room != NULL) {
            room->SetWallpaper(covering->item);
            room->flags.unk_0x20 = covering->param;
        }
    } else if (stage::IsKind(stage::KIND_MUSEUM_SHOWROOM, stage::GetCurrent())) {
        const u8 index = stage::GetCurrent() - stage::NAME_MUSEUM_SHOWROOM_1;
        GetGardenData()->townData.museum.GetExhibit(index)->museumRoom.SetWallpaper(covering->item);
        GetGardenData()->townData.museum.GetExhibit(index)->museumRoom.flags.unk_0x20 = covering->param;
    }
}
inline void SetRoomFlooring(const RoomCovering* covering)
{
    if (stage::IsKind(stage::KIND_PLAYER_HOUSE, stage::GetCurrent())) {
        SvRoom* room = Func6FA758(stage::GetCurrent());
        if (room != NULL) {
            room->SetFlooring(covering->item);
            room->flags.unk_0x21 = covering->param;
        }
    } else if (stage::IsKind(stage::KIND_MUSEUM_SHOWROOM, stage::GetCurrent())) {
        const u8 index = stage::GetCurrent() - stage::NAME_MUSEUM_SHOWROOM_1;
        GetGardenData()->townData.museum.GetExhibit(index)->museumRoom.SetFlooring(covering->item);
        GetGardenData()->townData.museum.GetExhibit(index)->museumRoom.flags.unk_0x21 = covering->param;
    }
}
} // namespace

// 0x002C6FAC (name is ours)
BsCharRoomViewMgr::BsCharRoomViewMgr()
    : m_IsResourceReady(false), m_IsFloorReady(false), m_IsWallReady(false), m_IsLandingReady(false),
      m_BgName(BG_NAME_NONE), m_LightTimer(0)
{
}

// 0x002C70D8 slot 0x00
// 0x002C70C8 (deleting dtor)
BsCharRoomViewMgr::~BsCharRoomViewMgr()
{
    // (only the members are destroyed)
}

// (inline)
DECOMP_ALWAYS_INLINE void BsCharRoomViewMgr::SetFixedCoverings()
{
    if (stage::IsKind(stage::KIND_ISLAND_HUT, stage::GetCurrent())) {
        const s32 tour = netgame::Func6C4DD0(TourData::s_pInstance->m_Tour);
        if (tour == isfactory::TOUR_TYPE_BORROWING_RACE) {
            m_Wallpaper.item = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_WOOD_PANELING));
            m_Wallpaper.param = 0;
            m_Flooring.item = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_PLANK_FLOORING));
            m_Flooring.param = 0;
        } else if (tour == isfactory::TOUR_TYPE_FOSSIL_DIG) {
            m_Wallpaper.item = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_MODERN_WALL));
            m_Wallpaper.param = 0;
            m_Flooring.item = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_MODERN_TILE));
            m_Flooring.param = 0;
        }
    } else if (stage::GetCurrent() == stage::NAME_CHIP_TUTORIAL) {
        m_Wallpaper.item = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_BASIC_WALL));
        m_Wallpaper.param = 0;
        m_Flooring.item = SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_PANEL_CARPET_FLOOR));
        m_Flooring.param = 0;
    }
}

// 0x002C6A84 slot 0x24
oml::framework::Result BsCharRoomViewMgr::Calc()
{
    const nn::fnd::DateTimeParameters date = *time::GetCurrentDate();
    if (CharRoomSunshine::IsShown(&date)) {
        if (CharRoomSunshine::IsSideA(&date)) {
            if (IsInGrounds(s_SunshineGroundsA, m_BgName)) {
                const s16 angle = 0;
                m_Sunshine.Calc(&date, &m_WallWPosition, &angle);
            }
        } else if (CharRoomSunshine::IsSideB(&date)) {
            if (IsInGrounds(s_SunshineGroundsB, m_BgName)) {
                const s16 angle = -0x8000;
                m_Sunshine.Calc(&date, &m_WallEPosition, &angle);
            }
        }
    }
    if (++m_LightTimer >= LIGHT_UPDATE_FRAMES) {
        UpdateLights();
        m_LightTimer = 0;
    }
    m_Wall.Calc();
    m_Landing.Calc();
    m_Wallpaper = Func529BE8();
    m_Flooring = Func5299E0();
    SetFixedCoverings();
    return oml::framework::RESULT_SUCCESS;
}

// 0x002C6898 slot 0x30
oml::framework::Result BsCharRoomViewMgr::Draw()
{
    m_Floor.Request(&m_Flooring);
    m_Wall.Request(&m_Wallpaper);
    m_Landing.Request(&m_Wallpaper);
    for (u32 i = 0; i < CharRoomLanding::PART_NUM; i++) {
        CharRoomLanding::Part& part = m_Landing.m_Parts[i];
        if (part.m_Model.GetModel() != NULL) {
            part.m_Model.AddToScene(0);
        }
    }
    m_Floor.m_Model.AddToScene(0);
    for (u32 i = 0; i < CharRoomWall::PART_NUM; i++) {
        CharRoomWall::Part& part = m_Wall.m_Parts[i];
        if (part.m_IsVisible) {
            part.m_Model.AddToScene(0);
        }
    }
    const nn::fnd::DateTimeParameters date = *time::GetCurrentDate();
    if (!stage::IsKind(stage::KIND_ISLAND_HUT, stage::GetCurrent()) && CharRoomSunshine::IsShown(&date)) {
        if (CharRoomSunshine::IsSideA(&date)) {
            if (IsInGrounds(s_SunshineGroundsA, m_BgName)) {
                m_Sunshine.Draw();
            }
        } else if (CharRoomSunshine::IsSideB(&date)) {
            if (IsInGrounds(s_SunshineGroundsB, m_BgName)) {
                m_Sunshine.Draw();
            }
        }
    }
    return oml::framework::RESULT_SUCCESS;
}

// 0x002C5C34 slot 0x40
bool BsCharRoomViewMgr::StartInitialize()
{
    sead::Heap* heap = HeapInit::s_pStageHeap;
    m_BgName = BG_NAME_IDR_ROOM_1_C_6_S;
    Field* field = FdInfo::GetCurrentField();
    if (field != NULL) {
        const BgInfo* bg = field->GetBg(0, 0);
        if (bg != NULL) {
            m_BgName = bg->GetName();
        }
    }
    m_Allocator.CreateExpHeap(HEAP_SIZE, heap, sead::SafeStringBase<char>("BsCharRoomViewMgr"),
                              sead::Heap::HEAP_DIRECTION_FORWARD, false);
    m_BgModels.CreateHeap(heap, MODEL_HEAP_SIZE);
    m_ResourceMgr.InitializeImpl(heap, RESOURCE_HEAP_SIZE);
    m_Wallpaper = Func529BE8();
    m_Flooring = Func5299E0();
    SetFixedCoverings();
    return true;
}

// 0x002C58EC slot 0x44
oml::framework::Result BsCharRoomViewMgr::UpdateInitialize()
{
    m_RoomTexturesKey = m_ResourceMgr.Load("Bg/Indoor/RoomTextures/room.bcres");
    bool isLoaded = m_RoomTexturesKey.IsValid();
    m_CommonKey = m_ResourceMgr.Load("Bg/Indoor/Common_idr/Common_idr.bcres");
    isLoaded &= m_CommonKey.IsValid();
    if (!(m_BgModels.Load(0, 0, m_BgName) & isLoaded)) {
        return oml::framework::RESULT_RETRY;
    }
    if (!m_IsResourceReady) {
        g3d::ResourceLoader* roomTextures = m_ResourceMgr.Get(m_RoomTexturesKey);
        roomTextures->Read(0x20000, 0x20000, 0x30000);
        roomTextures->InitializeCgfx(&m_Allocator, NULL, true);
        g3d::ResourceLoader* common = m_ResourceMgr.Get(m_CommonKey);
        common->Read(0x20000, 0x20000, 0x30000);
        common->InitializeCgfx(&m_Allocator, NULL, true);
        g3d::ResourceLoader* model = m_BgModels.GetLoader(0, 0);
        model->Read(0x20000, 0x20000, 0x30000);
        model->InitializeCgfx(&m_Allocator, NULL, true);
        model->InitializeCgfx(&m_Allocator, roomTextures, true);
        model->InitializeCgfx(&m_Allocator, common, true);
        const f32 height = Func6FA938();
        m_Floor.Initialize(model, &m_Flooring, height);
        m_Wall.Initialize(model, &m_Wallpaper, height);
        m_Landing.Initialize(model, roomTextures, height);
        m_IsResourceReady = true;
        return oml::framework::RESULT_RETRY;
    }
    if (!m_IsFloorReady) {
        m_IsFloorReady = m_Floor.Request(&m_Flooring);
    }
    if (!m_IsWallReady) {
        m_IsWallReady = m_Wall.Request(&m_Wallpaper);
    }
    if (!m_IsLandingReady) {
        m_IsLandingReady = m_Landing.Request(&m_Wallpaper);
    }
    if (m_IsFloorReady & m_IsWallReady & m_IsLandingReady) {
        return oml::framework::RESULT_SUCCESS;
    }
    return oml::framework::RESULT_RETRY;
}

// 0x002C56C4 slot 0x48
bool BsCharRoomViewMgr::EndInitialize()
{
    const f32 height = Func6FA938();
    g3d::ResourceLoader* model = m_BgModels.GetLoader(0, 0);
    g3d::ResourceLoader* common = m_ResourceMgr.Get(m_CommonKey);
    const void* const* file = reinterpret_cast<const void* const*>(&model->m_GraphicsFile);
    const nw::gfx::res::ResSkeletalModelData* position =
        static_cast<const nw::gfx::res::ResSkeletalModelData*>(nw::gfx::GetModel(file, "position"));
    if (IsInGrounds(s_SunshineGroundsA, m_BgName) || IsInGrounds(s_SunshineGroundsB, m_BgName)) {
        g3d::GetBoneTranslate(&m_WallEPosition, position, g3d::GetBoneIndex(position, "pos_wallE"));
        g3d::GetBoneTranslate(&m_WallWPosition, position, g3d::GetBoneIndex(position, "pos_wallW"));
        const s16 angle = -0x8000;
        const nn::math::VEC3 scale(1.0f, 1.0f, 1.0f);
        m_Sunshine.Initialize(nw::gfx::GetModel(file, "sunshine"), common, &m_Allocator, &m_WallEPosition, &scale,
                                &angle, height);
    }
    if (stage::GetCurrent() == stage::NAME_CHIP_TUTORIAL) {
        Func1E6844(1, stage::GetCurrent());
    }
    UpdateLights();
    s_pInstance = this;
    return true;
}

// 0x002C645C slot 0x4C
bool BsCharRoomViewMgr::StartFinalize()
{
    s_pInstance = NULL;
    return true;
}

// 0x002C6288 slot 0x50
oml::framework::Result BsCharRoomViewMgr::UpdateFinalize()
{
    m_Sunshine.Finalize();
    m_Floor.Finalize();
    m_Wall.Finalize();
    m_Landing.Finalize();
    m_BgModels.Finalize();
    m_ResourceMgr.Finalize();
    m_Allocator.DestroyHeap();
    return oml::framework::RESULT_SUCCESS;
}

// 0x002C60AC slot 0x54
bool BsCharRoomViewMgr::EndFinalize()
{
    return true;
}

// 0x002C5FA8 (name is ours)
void BsCharRoomViewMgr::GetRoomRect(f32* minX, f32* minZ, f32* maxX, f32* maxZ)
{
    g3d::ResourceLoader* model = m_BgModels.GetLoader(0, 0);
    if (model == NULL) {
        return;
    }
    const nw::gfx::res::ResSkeletalModelData* position = static_cast<const nw::gfx::res::ResSkeletalModelData*>(
        nw::gfx::GetModel(reinterpret_cast<const void* const*>(&model->m_GraphicsFile), "position"));
    // (an initialized local array: ARMCC copies it from 0x00847410)
    const char* const bones[] = {"pos_wallN", "pos_wallE", "pos_wallW", "pos_wallS"};
    f32 left = FLT_MAX;
    f32 top = FLT_MAX;
    f32 right = -FLT_MAX;
    f32 bottom = -FLT_MAX;
    for (u32 i = 0; i < 4; i++) {
        nn::math::VEC3 translate;
        g3d::GetBoneTranslate(&translate, position, g3d::GetBoneIndex(position, bones[i]));
        if (left >= translate.x) {
            left = translate.x;
        }
        if (top >= translate.z) {
            top = translate.z;
        }
        right = right > translate.x ? right : translate.x;
        bottom = bottom > translate.z ? bottom : translate.z;
    }
    *minX = left;
    *minZ = top;
    *maxX = right;
    *maxZ = bottom;
}

// 0x002C67B4 (name is ours)
void BsCharRoomViewMgr::UpdateLights()
{
    u8 window0[4];
    Func1E63B0(window0, 12);
    u8 window1[4];
    Func1E63B0(window1, 13);
    CharRoomWall& wall = m_Wall;
    if (wall.m_pWindowMaterialE != NULL) {
        g3d::SetConstantColor(wall.m_pWindowMaterialE, 0, window0[3], window0[2], window0[1], -1);
        g3d::SetConstantColor(wall.m_pWindowMaterialE, 1, window1[3], window1[2], window1[1], -1);
    }
    if (wall.m_pWindowMaterialW != NULL) {
        g3d::SetConstantColor(wall.m_pWindowMaterialW, 0, window0[3], window0[2], window0[1], -1);
        g3d::SetConstantColor(wall.m_pWindowMaterialW, 1, window1[3], window1[2], window1[1], -1);
    }
    u8 object[4];
    Func1E63B0(object, 1);
    m_Sunshine.SetColor(object, Func1E839C());
}

// 0x002C60B4 (name is ours)
bool BsCharRoomViewMgr::SetWallpaper(const SvFgName* item, bool playSound)
{
    if (!stage::IsInOwnHouse()) {
        return false;
    }
    if (item->GetKind() != item::KIND_DUMMY_WALL && !Func76BBF0(item)) {
        return false;
    }
    RoomCovering covering;
    covering.item = *item;
    covering.param = 0;
    if (netgame::IsMultiPlayer()) {
        SendCoveringPacket(PACKET_ROOM_WALLPAPER, &covering);
    }
    if (covering.item.GetKind() != item::KIND_DUMMY_WALL && !Func76BBF0(&covering.item)) {
        return false;
    }
    if (playSound) {
        SoundMgr::PlaySound(SE_ACT_WALLPAPER_CHANGE);
    }
    SetRoomWallpaper(&covering);
    return true;
}

// 0x002C6614 (name is ours)
bool BsCharRoomViewMgr::SetWallpaper(const SvFgName* item, u8 param, bool playSound)
{
    if (!stage::IsInOwnHouse()) {
        return false;
    }
    RoomCovering covering;
    covering.item = *item;
    covering.param = param;
    if (netgame::IsMultiPlayer()) {
        SendCoveringPacket(PACKET_ROOM_WALLPAPER, &covering);
    }
    if (covering.item.GetKind() != item::KIND_DUMMY_WALL && !Func76BBF0(&covering.item)) {
        return false;
    }
    if (playSound) {
        SoundMgr::PlaySound(SE_ACT_WALLPAPER_CHANGE);
    }
    SetRoomWallpaper(&covering);
    return true;
}

// 0x002C6DD8 (name is ours)
bool BsCharRoomViewMgr::SetFlooring(const SvFgName* item, bool playSound)
{
    if (!stage::IsInOwnHouse()) {
        return false;
    }
    if (item->GetKind() != item::KIND_CARPET && !Func76BBF0(item)) {
        return false;
    }
    RoomCovering covering;
    covering.item = *item;
    covering.param = 0;
    if (netgame::IsMultiPlayer()) {
        SendCoveringPacket(PACKET_ROOM_FLOORING, &covering);
    }
    if (covering.item.GetKind() != item::KIND_CARPET && !Func76BBF0(&covering.item)) {
        return false;
    }
    if (playSound) {
        SoundMgr::PlaySound(SE_ACT_CARPET_CHANGE);
    }
    SetRoomFlooring(&covering);
    return true;
}

// 0x002C6474 (name is ours)
bool BsCharRoomViewMgr::SetFlooring(const SvFgName* item, u8 param, bool playSound)
{
    if (!stage::IsInOwnHouse()) {
        return false;
    }
    RoomCovering covering;
    covering.item = *item;
    covering.param = param;
    if (netgame::IsMultiPlayer()) {
        SendCoveringPacket(PACKET_ROOM_FLOORING, &covering);
    }
    if (covering.item.GetKind() != item::KIND_CARPET && !Func76BBF0(&covering.item)) {
        return false;
    }
    if (playSound) {
        SoundMgr::PlaySound(SE_ACT_CARPET_CHANGE);
    }
    SetRoomFlooring(&covering);
    return true;
}
