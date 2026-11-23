#include "Bs/dBsFtrMgr.h"
#include "Other/dActor.h"
#include "Ac/dAcPlayer.h"
#include "field/dFdInfo.h"
#include <cfloat>
#include <cmath>
#include <cstring>
#include "Bs/dBsFtrMgrExtern.h"
#include "Bs/dBsFtrMgr_CommonResLoader.h"
#include "Item/item_Api.h"
#include "Other/dHeapInit.h"
#include "Sound/dSoundMgr.h"
#include "fgobj/fgobj_Api.h"
#include "fgobj/fgobj_Extern.h"
#include "g3d/dSkeletalAnim.h"
#include "g3d/dSkeletalModel.h"
#include "net/dNet.h"
#include "sead/seadExpHeap.h"
#include "ssys/ma/ma_Math.h"
#include "stage/stage_Api.h"
#include "Sv/dSvHouse.h"
#include "Sv/dSvMyDesign.h"
#include "Sc/dScStageExtern.h"
#include "demo/demo_Extern.h"
#include "List/dSelectBase.h"
#include "Other/dOrderList.h"
#include "Other/dProcMgr.h"
#include "Sound/dSoundObjFurniture.h"
#include "Sv/dSvShopZakkaExtern.h"
#include "bg/bg_Extern.h"
#include "field/dField.h"
#include "field/field_Extern.h"
#include "time/time_Api.h"
#include "netgame/netgame_Api.h"
#include "Other/dController.h"
#include "Ac/dAcFtr.h"
#include "Bs/dBsSvDemo.h"
#include "Bs/dBsNpcMgrExtern.h"
#include "Other/dCatalogList.h"
#include "field/dFieldAttribute.h"
#include "oml/framework/dProcess.h"
#include "Sv/dSvFgNameExtern.h"
#include "Other/dTourData.h"
#include "Other/dToolBankExtern.h"
#include "demo/demo_Api.h"
#include "Sv/dSvMail.h"

extern const nn::math::VEC3 VEC3_ZERO;

// the frames an item flies (a constant of the file of the player; ThrowItem)
extern const u32 g_ThrowItemFrames;

namespace {
// the units of a block of the field along x and z
const s32 UNITS_PER_BLOCK = 16;
// the sizes of the heap of the common resources and of the heaps of the mannequins
const u32 COMMON_HEAP_SIZE = 0x8C00;
const u32 MANNEQUIN_HEAP_SIZE = 0x80000;
// the number of songs (GetSongItem)
const u32 SONG_NUM = 91;
} // namespace

// (the globals of the file; the static initializer builds them)
// 0x0078656C (static initializer)
// 0x0095031C
u8 s_Unknown95031C;
// 0x00950320
BsFtrMgr* BsFtrMgr::s_pInstance;
// the item of the furniture that talks after A and the other item of the search (UpdateTalk)
// 0x0095032C
SvFgName s_TalkItem(SvFgName::s_Empty);
// 0x00950330
SvFgName s_TalkOtherItem(SvFgName::s_Empty);
// 0x00950334
f32 s_Unknown950334 = static_cast<f32>(static_cast<u16>(FdInfo::UNIT_WORLD_SIZE_XF * 0.2f));
// the furniture shown in the menus
// 0x00950338
FtrPreview s_FtrPreview;
// 0x00950340
f32 s_Unknown950340[2] = {FdInfo::UNIT_WORLD_SIZE_XF * 1.5f, FdInfo::UNIT_WORLD_SIZE_XF * 1.55f};
// the heap of the common resources of the furniture
// 0x00950348
ssys::ma::HeapAllocator s_CommonAllocator;
// the gravity of the flying items (set by an initializer of another file)
// 0x00953A24
f32 s_FlyingItemGravity;
// 0x0098BF48
BsFtrMgr::CommonModel BsFtrMgr::s_CommonModel;
// 0x0098BF78
BsFtrMgr::CommonResLoader s_CommonResLoader;
// 0x0098C080
OrderItemList g_OrderItems;

// 0x00694048
void ClearOrderItems()
{
    for (u32 i = 0; i < OrderItemList::ITEM_MAX; i++) {
        g_OrderItems.items[i] = SvFgName::s_Empty;
    }
    g_OrderItems.num = 0;
}
// the heaps of the mannequins
// 0x0098BF38
sead::ExpHeap* s_MannequinHeaps[BsFtrMgr::MANNEQUIN_HEAP_NUM];

// 0x00694F08
BsFtrMgr::BsFtrMgr()
    : m_MusicFtrIndex(-1), m_SongIndex(-1), m_Unknown860(false), m_Unknown862(false), m_pWallHangBack(NULL),
      m_pWallHangUnitCursor(NULL)
{
    for (FlyingItem* it = m_FlyingItems; it < m_FlyingItems + FLYING_ITEM_NUM; it++) {
        it->item = SvFgName::s_Empty;
    }
}

// 0x006950A0
// 0x00695004 (deleting dtor)
BsFtrMgr::~BsFtrMgr()
{
    // (only the members are destroyed)
}

// 0x00694800
oml::framework::Result BsFtrMgr::Calc()
{
    // the flying items
    const f32 step = 1.0f / static_cast<f32>(g_ThrowItemFrames);
    for (FlyingItem* it = m_FlyingItems; it < m_FlyingItems + FLYING_ITEM_NUM; it++) {
        if (it->item.IsEmpty()) {
            continue;
        }
        ssys::ma::Chase(&it->scale, 1.0f, step);
        it->position.x += it->velocity.x;
        it->position.y += it->velocity.y;
        it->position.z += it->velocity.z;
        if (it->velocity.y < 0.0f && it->position.y < it->target.y) {
            it->position.y = it->target.y;
        }
        it->velocity.y -= s_FlyingItemGravity;
        if (it->timer == 0) {
            it->item = SvFgName::s_Empty;
        }
        it->timer--;
    }
    // the music of the furniture
    if (IsMusicStage()) {
        void* actor = NULL;
        if (m_MusicFtrIndex >= 0) {
            actor = Func4E9C14(Func4EA6D0(), m_MusicFtrIndex);
        }
        if (actor != NULL) {
            const f32 frame = Func5711E4(actor);
            m_MusicAudio.Update(Func7519D4(actor), frame);
        } else {
            m_MusicAudio.Update(&VEC3_ZERO, 0.0f);
        }
        SoundMgr::SUpdateRoomMusic();
        if (m_MusicAudio.IsMusicLoaded()) {
            u32 time = 0;
            const u64 start = static_cast<u64>(m_MusicStartTime[0]) | (static_cast<u64>(m_MusicStartTime[1]) << 32);
            if (start != 0) {
                time = static_cast<u32>(net::Net::Get()->GetSessionTime()) - m_MusicStartTime[0];
            }
            m_MusicAudio.PlayMusic(m_Unknown860 == false, time);
        }
    }
    // the TV programs and the theremin of the nearest furniture
    m_ThereminSound.SoundObjTheremin::StopSound();
    nn::math::VEC3 tvPosition(0.0f, 0.0f, 0.0f);
    nn::math::VEC3 thereminPosition(0.0f, 0.0f, 0.0f);
    u8 channel;
    f32 distance;
    s32 thereminIndex;
    FindNearestTvAndTheremin(&tvPosition, &channel, &thereminPosition, &distance, &thereminIndex);
    if (m_IsTvEnabled) {
        m_TvMgr.Calc(channel, &tvPosition);
    }
    if (thereminIndex != -1) {
        m_ThereminSound.Update(&thereminPosition, distance);
    }
    // the clocks
    m_ClockDisplay.UpdateDigits();
    m_ClockDisplay.UpdateBlink();
    m_Frame60 = static_cast<f32>((static_cast<s32>(m_Frame60) + 1) % 60);
    UpdateTalk();
    return oml::framework::RESULT_SUCCESS;
}

// 0x006946D4
oml::framework::Result BsFtrMgr::Draw()
{
    for (FlyingItem* it = m_FlyingItems; it < m_FlyingItems + FLYING_ITEM_NUM; it++) {
        if (it->item.IsEmpty()) {
            continue;
        }
        const nn::math::VEC3 scale(it->scale, it->scale, it->scale);
        fgobj::PlaceOther(it->item, &it->position, &scale, reinterpret_cast<s32>(it), 8.0f, true, false, 0, false);
    }
    return oml::framework::RESULT_SUCCESS;
}

// 0x00691044
oml::framework::Result BsFtrMgr::UpdateInitialize()
{
    sead::FixedSafeString<64> path;
    path.format("Ftr/Common/%s", "FtrCommon.bcres");
    const bool isLoaded = s_CommonResLoader.LoadAsync(path, s_CommonAllocator.m_pHeap, g3d::ResourceLoader::CGFX_ALIGNMENT);
    // (no TV in the Reset Centre)
    m_IsTvEnabled = true;
    if (stage::GetCurrent() == stage::NAME_RESET_MONITORING_CENTER) {
        m_IsTvEnabled = false;
    }
    const bool isTvLoaded = m_IsTvEnabled ? m_TvMgr.Load() : true;
    if (!(isTvLoaded & isLoaded)) {
        return oml::framework::RESULT_RETRY;
    }
    if (s_CommonResLoader.IsLoadFailed()) {
        return oml::framework::RESULT_FAIL;
    }
    return oml::framework::RESULT_SUCCESS;
}

// 0x0069114C
bool BsFtrMgr::StartInitialize()
{
    s_Unknown95031C = 0;
    sead::Heap* heap = HeapInit::s_pStageHeap;
    s_CommonAllocator.CreateExpHeap(COMMON_HEAP_SIZE, heap, sead::SafeStringBase<char>("FtrCommon"),
                                    sead::Heap::HEAP_DIRECTION_FORWARD, false);
    const stage::Name current = stage::GetCurrent();
    // (the stages with mannequins)
    if (stage::IsKind(stage::KIND_PLAYER_HOUSE, current) || stage::IsKind(stage::KIND_MUSEUM_SHOWROOM, current) ||
        stage::IsKind(stage::KIND_MODEL_HOUSE, current) || current == stage::NAME_DEPARTMENT_STORE_3F) {
        sead::Heap* parent = HeapInit::s_pStageHeap;
        for (u32 i = 0; i < MANNEQUIN_HEAP_NUM; i++) {
            s_MannequinHeaps[i] = sead::ExpHeap::create(MANNEQUIN_HEAP_SIZE, sead::SafeStringBase<char>("AcFtrMannequinHeap"),
                                                        parent, sead::Heap::HEAP_DIRECTION_FORWARD, false);
        }
    }
    const u32 num = stage::GetFurnitureCapacity(current);
    s_WorkPool.Initialize(num);
    const bool result = m_ModelBanks.Initialize(heap, num, "BsFtrMgr");
    s_pInstance = this;

    // (a debug placement of test furniture in the player houses; armlink replaced the calls of the tiny function that
    // gives the mode by "mov r0, #0", so it never runs)
    Field* field = FdInfo::GetCurrentField();
    if (stage::IsKind(stage::KIND_PLAYER_HOUSE, current)) {
        const SvFgName classicTable(static_cast<SvFgName::ID>(SvFgName::NAME_CLASSIC_TABLE));
        const SvFgName cabinLowTable(static_cast<SvFgName::ID>(SvFgName::NAME_CABIN_LOW_TABLE));
        const SvFgName kiddieDresser(static_cast<SvFgName::ID>(SvFgName::NAME_KIDDIE_DRESSER));
        const SvFgName sweetsPlayer(static_cast<SvFgName::ID>(SvFgName::NAME_SWEETS_PLAYER));
        const SvFgName paveEndTable(static_cast<SvFgName::ID>(SvFgName::NAME_PAVE_END_TABLE));
        const SvFgName labChair(static_cast<SvFgName::ID>(SvFgName::NAME_LAB_CHAIR));
        SvFgName labChairTurned3;
        Func311860(&labChairTurned3, labChair, 3);
        SvFgName labChairTurned1;
        Func311860(&labChairTurned1, labChair, 1);
        const SvFgName amazingMachine(static_cast<SvFgName::ID>(SvFgName::NAME_AMAZING_MACHINE));
        const SvFgName lavaLamp(static_cast<SvFgName::ID>(SvFgName::NAME_LAVA_LAMP));
        const SvFgName grapefruitTable(static_cast<SvFgName::ID>(SvFgName::NAME_GRAPEFRUIT_TABLE));
        const SvFgName hourglass(static_cast<SvFgName::ID>(SvFgName::NAME_HOURGLASS));
        const SvFgName hologramMachine(static_cast<SvFgName::ID>(SvFgName::NAME_HOLOGRAM_MACHINE));
        const SvFgName smallSpaceConsole(static_cast<SvFgName::ID>(SvFgName::NAME_SMALL_SPACE_CONSOLE));
        const SvFgName mannequin1(static_cast<SvFgName::ID>(SvFgName::NAME_MANNEQUIN_1));
        const SvFgName mannequin2(static_cast<SvFgName::ID>(SvFgName::NAME_MANNEQUIN_2));
        const s32 mode = 0;
        if (mode != 0) {
            if (mode == 1) {
                for (s32 x = 4; x <= 11; x++) {
                    for (s32 z = 11; z <= 12; z++) {
                        if (x < 5 || x > 8) {
                            field->PlaceItem(hologramMachine, x, z, 0);
                        }
                    }
                }
                field->PlaceItem(mannequin1, 5, 11, 0);
                field->PlaceItem(mannequin2, 8, 11, 0);
                field->PlaceItem(labChairTurned1, 5, 12, 0);
                field->PlaceItem(labChairTurned3, 8, 12, 0);
                for (s32 x = 4; x <= 11; x++) {
                    if (x == 6 || x == 7) {
                        continue;
                    }
                    field->PlaceItem(smallSpaceConsole, x, 10, 0);
                }
                field->PlaceItem(labChair, 6, 10, 0);
                field->PlaceItem(labChair, 7, 10, 0);
                for (s32 x = 4; x < 11; x += 2) {
                    for (s32 z = 6; z <= 8; z++) {
                        field->PlaceItem(amazingMachine, x, z, 0);
                    }
                }
                for (s32 x = 4; x <= 11; x++) {
                    field->PlaceItem(hologramMachine, x, 5, 0);
                    field->PlaceItem(hourglass, x, 9, 0);
                }
            }
            if (mode == 2) {
                const SvFgName tables[3] = {paveEndTable, classicTable, grapefruitTable};
                const SvFgName dressers[2] = {kiddieDresser, cabinLowTable};
                s32 i = 0;
                for (s32 x = 4; x <= 11; x++) {
                    for (s32 z = 5; z <= 7; z += 2) {
                        field->PlaceItem(dressers[i % 2], x, z, 0);
                        i++;
                    }
                }
                (void)tables;
            }
        }
        (void)sweetsPlayer;
        (void)lavaLamp;
    }
    return result;
}

// 0x006906C8
bool BsFtrMgr::EndInitialize()
{
    // the music of the furniture: the furniture object of the Robo Stereo plays it
    if (IsMusicStage()) {
        SoundMgr::SCreateRoomMusic();
        SoundMgr::SInitializeRoomMusic();
    }
    // 0x00950328 (guard 0x00950324)
    static const SvFgName s_MusicFtr(static_cast<SvFgName::ID>(SvFgName::NAME_ROBO_STEREO));
    const item::ParamItemEntry* entry = item::GetParamItemEntry(s_MusicFtr);
    if (entry != NULL) {
        u32 kind = entry->ftrSoundKind;
        if (kind >= SoundObjFurniture::KIND_NUM) {
            kind = 0;
        }
        m_MusicAudio.Start(&VEC3_ZERO, kind, false);
    }
    m_ThereminSound.Start();

    // the common model of the furniture with its push/pull animations
    s_CommonResLoader.InitializeCgfx(&s_CommonAllocator, NULL, true);
    void* model = s_CommonResLoader.GetGraphicsFile().GetModels("int_trans_dummy");
    void* pushAnim = s_CommonResLoader.GetGraphicsFile().ptr()->skeletalAnims.GetData("int_trans_push");
    void* pullAnim = s_CommonResLoader.GetGraphicsFile().ptr()->skeletalAnims.GetData("int_trans_pull");
    s_CommonModel.model.Create(nw::gfx::res::ResSceneObject(model), &s_CommonAllocator, &s_CommonAllocator, 0, true, 1);
    s_CommonModel.m_pPushAnim = pushAnim;
    s_CommonModel.m_pPullAnim = pullAnim;
    s_CommonModel.anim.Build(&s_CommonModel.model, nw::anim::res::ResAnim(pushAnim), &s_CommonAllocator, 0);
    s_CommonModel.anim.SetPlayMode(g3d::FrameController::PLAY_MODE_ONETIME);

    // (a model house entered from an exhibition: the active flags of its rooms come from the saved rooms)
    const stage::Name current = stage::GetCurrent();
    if (stage::IsKind(stage::KIND_EXHIBITION, stage::GetPrevious()) && stage::IsKind(stage::KIND_MODEL_HOUSE, current)) {
        for (u32 stage = current; stage < current + 6u; stage++) {
            const SvRoom* room = Func6FA758(static_cast<u8>(stage));
            if (room == NULL) {
                continue;
            }
            const SvRoomActiveLayers saved = room->activeStates;
            SvRoomActiveLayers* layers = Func5831D0(static_cast<u8>(stage));
            if (layers == NULL) {
                continue;
            }
            for (u32 layer = 0; layer < 2; layer++) {
                Func5831B8(layers, &saved.layers[layer & 1], layer & 0xFF);
            }
        }
    }

    // the furniture actors of the items of the field (in a model house the items that can be ordered are collected)
    ClearOrderItems();
    const Field* field = FdInfo::GetCurrentField();
    if (field != NULL) {
        const s32 width = field->GetBlockWidth();
        const s32 height = field->GetBlockHeight();
        for (u32 layer = 0; layer < 2; layer++) {
            for (s32 blockZ = 0; blockZ < height; blockZ++) {
                for (s32 blockX = 0; blockX < width; blockX++) {
                    for (s32 unitZ = 0; unitZ < UNITS_PER_BLOCK; unitZ++) {
                        for (s32 unitX = 0; unitX < UNITS_PER_BLOCK; unitX++) {
                            const SvFgName* item = field->GetFg(blockX, blockZ, unitX, unitZ, layer);
                            if (item != NULL && Func30B658(item)) {
                                CreateFtrArg arg;
                                s32 x, z;
                                FdInfo::ToUnit(x, z, blockX, blockZ, unitX, unitZ);
                                const u8 rotation = Func76A388(item);
                                SvFgName name(*item);
                                name.ClearOrientationIfFurniture();
                                arg.item = name;
                                arg.x = x;
                                arg.y = z;
                                arg.rotation = rotation;
                                arg.layer = layer;
                                arg.placeType = CreateFtrArg::PLACE_TYPE_DEFAULT;
                                CreateFtrActor(arg);
                                if (stage::IsKind(stage::KIND_MODEL_HOUSE, current)) {
                                    const item::Kind kind = item->GetKind();
                                    if (kind != item::KIND_MANNEQUIN && kind != item::KIND_MT_FRUIT &&
                                        kind != item::KIND_GOOD_MT_FRUIT && kind != item::KIND_SHELL) {
                                        // (walls and carpets that stand as furniture are ordered as the wall/carpet)
                                        SvFgName order(*item);
                                        if (Func76B6A0(&order)) {
                                            order.ConvertFromWallKind();
                                        } else if (Func76BE54(&order)) {
                                            order.Func6BBCF0();
                                        } else if (Func76C828(&order)) {
                                            order.ConvertCustomDesignFromWall();
                                        }
                                        g_OrderItems.Add(order);
                                    }
                                }
                            } else if (stage::IsKind(stage::KIND_MODEL_HOUSE, current) && item != NULL &&
                                       Func76910C(item)) {
                                const item::Kind kind = item->GetKind();
                                if (kind == item::KIND_DUMMY_WALL || kind == item::KIND_CARPET) {
                                    g_OrderItems.Add(*item);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    // (and the wallpaper and the flooring of the model house)
    if (stage::IsKind(stage::KIND_MODEL_HOUSE, current)) {
        const SvRoom* room = Func6FA758(current);
        if (room != NULL && room->flags.roomSize != 0) {
            const SvFgName wallpaper(*room->GetWallpaper());
            if (wallpaper.GetKind() == item::KIND_DUMMY_WALL) {
                g_OrderItems.Add(wallpaper);
            }
            const SvFgName flooring(*room->GetFlooring());
            if (flooring.GetKind() == item::KIND_CARPET) {
                g_OrderItems.Add(flooring);
            }
        }
    }

    // the frame of the clocks: two seconds are 60 frames
    const nn::fnd::DateTimeParameters* date = time::GetCurrentDate();
    m_Frame60 = static_cast<f32>(date->milliSecond) * 0.03f + 15.0f;
    if (date->second % 2 != 0) {
        m_Frame60 += 30.0f;
    }

    if (stage::IsKind(stage::KIND_INDOOR, stage::GetCurrent())) {
        m_pWallHangBack = Actor::Create(PROC_AC_OBJ_WALL_HANG_BACK, this, 0, NULL, NULL);
        m_pWallHangUnitCursor = Actor::Create(PROC_AC_OBJ_WALL_HANG_UNIT_CURSOR, this, 0, NULL, NULL);
    }
    return true;
}

// 0x006921E0
bool BsFtrMgr::StartFinalize()
{
    s_CommonModel.anim.Finalize();
    s_CommonModel.model.Finalize();
    m_ThereminSound.Stop();
    m_MusicAudio.Stop();
    if (IsMusicStage()) {
        SoundMgr::SDestroyRoomMusic();
    }
    return true;
}

// 0x006921C0
oml::framework::Result BsFtrMgr::UpdateFinalize()
{
    s_pInstance = NULL;
    Func4E9C78();
    return oml::framework::RESULT_SUCCESS;
}

// 0x00691E6C
bool BsFtrMgr::EndFinalize()
{
    const stage::Name current = stage::GetCurrent();
    // (leaving a model house into an exhibition: the mannequins of the room are active again)
    if (stage::IsKind(stage::KIND_MODEL_HOUSE, current) && stage::IsKind(stage::KIND_EXHIBITION, stage::GetNext())) {
        SvRoomActiveLayers* layers = Func5831D0(current);
        if (layers != NULL) {
            layers->SetAllActive();
        }
    }
    m_ModelBanks.FinalizeBanks();
    s_CommonResLoader.Unload();
    if (m_IsTvEnabled) {
        m_TvMgr.Unload();
    }
    s_WorkPool.Finalize();
    for (s32 i = 0; i < MANNEQUIN_HEAP_NUM; i++) {
        if (s_MannequinHeaps[i] != NULL) {
            s_MannequinHeaps[i]->destroy();
            s_MannequinHeaps[i] = NULL;
        }
    }
    m_pWallHangBack = NULL;
    m_pWallHangUnitCursor = NULL;
    s_CommonAllocator.DestroyHeap();
    return true;
}

// 0x00690F8C | libgarden [tier A]
Actor* BsFtrMgr::CreateFtrActor(const CreateFtrArg& arg)
{
    BsFtrMgr* mgr = s_pInstance;
    const item::ParamItemEntry* entry = item::GetParamItemEntry(arg.item);
    const bool isInvisible = arg.placeType == CreateFtrArg::PLACE_TYPE_UNKNOWN_INVISIBLE;
    if (entry == NULL || !(Func749394(entry) | isInvisible)) {
        return NULL;
    }
    if (stage::IsPolice(stage::GetCurrent())) {
        return NULL;
    }
    if (!Func746234(Func4EA6D0()) && arg.placeType != CreateFtrArg::PLACE_TYPE_UNKNOWN_INVISIBLE) {
        return NULL;
    }
    u16 name = GetFtrProcName(1);
    if (entry->ftr < 86 && entry->ftr != 0) {
        name = GetFtrProcNameOfEntry(entry);
    }
    return static_cast<Actor*>(Actor::Create(static_cast<ProcName>(name), mgr, reinterpret_cast<u32>(&arg), NULL, NULL));
}

namespace {
// the distance of a player to the place on the ground (x and z), squared
inline f32 GetDistanceSquareXZ(const nn::math::VEC3* player, const nn::math::VEC3& position)
{
    const f32 dx = player->x - position.x;
    const f32 dz = player->z - position.z;
    return dx * dx + dz * dz;
}
} // namespace

// the directions of the search in front of the player, in 1/16 turns added to the angle (SearchFtr; step 1 is the own unit)
// 0x00847CB8
extern const u16 s_FtrSearchDirections[5] = {0, 0, 2, static_cast<u16>(-2), 0};
// the unit next to (and two units from) a unit in each of the 16 directions
struct FtrUnitOffset
{
    s32 x;
    s32 z;
};
// 0x00847CC4
extern const FtrUnitOffset s_FtrNearOffsets[16] = {
    {0, 1}, {1, 1}, {1, 1}, {1, 0}, {1, 0}, {1, -1}, {1, -1}, {0, -1},
    {0, -1}, {-1, -1}, {-1, -1}, {-1, 0}, {-1, 0}, {-1, 1}, {-1, 1}, {0, 1},
};
// 0x00847D44
extern const FtrUnitOffset s_FtrFarOffsets[16] = {
    {0, 2}, {2, 2}, {2, 2}, {2, 0}, {2, 0}, {2, -2}, {2, -2}, {0, -2},
    {0, -2}, {-2, -2}, {-2, -2}, {-2, 0}, {-2, 0}, {-2, 2}, {-2, 2}, {0, 2},
};

// 0x00691A90
s32 BsFtrMgr::SearchFtr(SearchResult* out, const nn::math::VEC3* position, math::Angle16 angle, u8 mode)
{
    if (Func4E2B6C()) {
        return -1;
    }
    for (u32 step = 0; step < 5; step++) {
        for (s32 layer = 1; layer >= 0; layer--) {
            // (the unit two in front only on the lower layer)
            if (step == 4 && layer == 0) {
                break;
            }
            s32 x, z;
            FdInfo::ToUnit(x, z, *position);
            if (step != 1) {
                if (step >= 5) {
                    continue;
                }
                const u32 direction = (s_FtrSearchDirections[step] + (angle.value >> 12)) & 15;
                FtrUnitOffset offset = s_FtrNearOffsets[direction];
                if (step == 4) {
                    offset = s_FtrFarOffsets[direction];
                }
                x += offset.x;
                z += offset.z;
            }
            const s32 index = SearchFtrAtUnit(out, x, z, layer, position, angle, step, mode);
            if (index != -1) {
                return index;
            }
        }
        if (step != 0) {
            continue;
        }
        // nothing in front: an item on the ground of the unit in front without a field object
        s32 x, z;
        Func5C3200(&x, &z, position, angle);
        void* object = Func4EA0B0(Func4E1338(), x, z, 0);
        Field* field = FdInfo::GetCurrentField();
        if (field == NULL || object != NULL) {
            continue;
        }
        const SvFgName* item = field->GetFg(x, z, 0);
        if (item == NULL) {
            continue;
        }
        if (!item->IsUsual() && !Func769190(item)) {
            continue;
        }
        nn::math::VEC3 center;
        FdInfo::ToCenterWorld(&center, x, z);
        out->position = center;
        out->item = *item;
        out->baseItem = *item;
        out->radius = 20.0f;
        out->unk18 = 0;
        out->unk19 = 0;
        return -2;
    }
    return -1;
}

// the radius of the body of the movement test of IsUnitReachable (only read there)
// 0x00882BA8
extern const f32 s_FtrReachRadius = 15.0f;

// 0x00692654
bool BsFtrMgr::IsUnitReachable(s32 fromX, s32 fromZ, s32 x, s32 z)
{
    s32 dx = fromX - x;
    if (dx < 0) {
        dx = -dx;
    }
    s32 dz = fromZ - z;
    if (dz < 0) {
        dz = -dz;
    }
    if (dx + dz == 0 || dx + dz == 1) {
        return true;
    }
    // along x first, then along z
    nn::math::VEC3 from;
    nn::math::VEC3 to;
    FdInfo::ToCenterWorldWithUnitHeight(&from, fromX, fromZ);
    FdInfo::ToCenterWorldWithUnitHeight(&to, x, fromZ);
    {
        bgcheck::MoveTest test;
        test.Test(&to, &from, 0, s_FtrReachRadius, 1, bgcheck::MoveTest::GetDefaultMask(), 0);
        if (!test.IsHit(15)) {
            return true;
        }
    }
    FdInfo::ToCenterWorldWithUnitHeight(&to, fromX, z);
    {
        bgcheck::MoveTest test;
        test.Test(&to, &from, 0, s_FtrReachRadius, 1, bgcheck::MoveTest::GetDefaultMask(), 0);
        if (!test.IsHit(15)) {
            return true;
        }
    }
    return false;
}

// 0x00692C38
s32 BsFtrMgr::SearchFtrAtUnit(SearchResult* out, s32 x, s32 z, u8 layer, const nn::math::VEC3* position, math::Angle16 angle,
                              u32 step, u8 mode)
{
    if (out == NULL) {
        return -1;
    }
    nn::math::VEC3 center;
    FdInfo::ToCenterWorld(&center, x, z);
    f32 maxDistance = s_Unknown950340[layer & 1];
    if (step == 4) {
        maxDistance += FdInfo::UNIT_WORLD_SIZE_XF;
    }
    const f32 dx = center.x - position->x;
    const f32 dz = center.z - position->z;
    if (std::sqrt(dx * dx + dz * dz) > maxDistance) {
        return -1;
    }
    AcFtr* actor = static_cast<AcFtr*>(Func4EA0B0(Func4E1338(), x, z, layer));
    s32 playerX, playerZ;
    FdInfo::ToUnit(playerX, playerZ, *position);
    if (layer == 0 && !IsUnitReachable(playerX, playerZ, x, z)) {
        return -1;
    }
    if (actor != NULL && Func7510D0(actor)) {
        // (furniture with a front: only from the front, the player looking at it)
        const u32 facing = GetQuarterOfAngle(&actor->m_Rotation.y.value);
        const u32 looking = (GetQuarterOfAngle(&angle.value) + 2) & 3;
        if (looking != facing) {
            return -1;
        }
        const s32 offsetX = playerX - x;
        const s32 offsetZ = playerZ - z;
        switch (facing) {
        case 0:
            if (offsetZ <= 0) {
                return -1;
            }
            break;
        case 1:
            if (offsetX <= 0) {
                return -1;
            }
            break;
        case 2:
            if (offsetZ >= 0) {
                return -1;
            }
            break;
        case 3:
            if (offsetX >= 0) {
                return -1;
            }
            break;
        }
    }
    if (step == 4 && !Func17C5C0(x, z, &center, position)) {
        return -1;
    }
    if (actor != NULL && (actor->vf_0x90() || mode != 0) && !Func751EEC(actor)) {
        // a furniture actor
        Func75242C(actor, &center, 0);
        out->item = *Func752074(actor);
        out->baseItem = Func6BB164(actor);
        out->unk18 = layer;
        if (step == 4) {
            out->unk19 = 2;
        } else if (Func752D50(actor) == 4 || layer == 1) {
            out->unk19 = 1;
        } else {
            out->unk19 = 0;
        }
        out->position = center;
        out->radius = 0.0f;
        if (layer == 0) {
            out->radius = 20.0f;
        }
        return Func746284(Func4EA6D0(), actor);
    }
    // an item on the upper layer without a furniture actor
    Field* field = FdInfo::GetCurrentField();
    if (!(layer == 1 && actor == NULL) || field == NULL) {
        return -1;
    }
    const SvFgName* item = field->GetFg(x, z, layer);
    if (item == NULL) {
        return -1;
    }
    if (!item->IsUsual() && !Func769190(item)) {
        return -1;
    }
    if (step == 4 && !Func17C5C0(x, z, &center, position)) {
        return -1;
    }
    out->position = center;
    out->item = *item;
    out->baseItem = *item;
    out->radius = 0.0f;
    out->unk18 = layer;
    out->unk19 = (step == 4) ? 2 : 1;
    return -2;
}

// 0x0069278C
void BsFtrMgr::UpdateTalk()
{
    const stage::Name current = stage::GetCurrent();
    if (!stage::IsKind(stage::KIND_MODEL_HOUSE, current) &&
        !(stage::IsKind(stage::KIND_MUSEUM_SHOWROOM, current) && !stage::IsOwnShowroom(current))) {
        if (!Func4E79CC(stage::GetCurrent())) {
            return;
        }
        const TourData* tour = TourData::s_pInstance;
        if (tour != NULL && Func76F7E4(&tour->m_Seed) == 2) {
            return;
        }
    }
    if (!Controller::IsTrig(Controller::A)) {
        return;
    }
    s_TalkItem = s_TalkOtherItem = SvFgName::s_Empty;
    SearchResult result;
    AcPlayer* player = demo::PlayerGetActorEntry(static_cast<netgame::PlayerNo>(4), false);
    if (player == NULL) {
        return;
    }
    const s32 index = SearchFtr(&result, static_cast<const nn::math::VEC3*>(&player->m_Position),
                                player->m_Rotation.y, 1);
    if (index == -2) {
        s_TalkOtherItem = result.baseItem;
        DemoActor* object = static_cast<DemoActor*>(const_cast<void*>(Func4EA078(Func4E1338(), &result.position, 0)));
        if (object != NULL) {
            s_TalkItem = *Func752074(object);
            demo::MakeOrderDefaultTalk(object);
        }
        return;
    }
    if (index < 0) {
        return;
    }
    DemoActor* actor = static_cast<DemoActor*>(Func4E9C14(Func4EA6D0(), index));
    if (actor == NULL) {
        return;
    }
    const u8 kind = Func752A3C(actor);
    if (kind == 1) {
        s_TalkOtherItem = *Func752074(actor);
        void* object = const_cast<void*>(Func4EA078(Func4E1338(), &result.position, 0));
        if (object != NULL) {
            s_TalkItem = *Func752074(object);
        }
    } else if (kind == 0) {
        s_TalkItem = *Func752074(actor);
    }
    demo::MakeOrderDefaultTalk(actor);
}

// 0x00691CE8
s32 BsFtrMgr::SearchFtrOfPlayer(SearchResult* out, u8 mode)
{
    AcPlayer* player = demo::PlayerGetActorEntry(static_cast<netgame::PlayerNo>(4), false);
    if (player == NULL) {
        return -1;
    }
    return SearchFtr(out, static_cast<const nn::math::VEC3*>(&player->m_Position),
                     player->m_Rotation.y, mode);
}

namespace {
// the states of the furniture actors with Func7529D0 that Func691D34/Func692088 change (meanings unknown)
inline bool IsChangingState(u8 state)
{
    return state == 182 || state == 184 || state == 189 || state == 191 || state == 192 || state == 195 ||
           state == 196 || state == 197 || state == 198;
}
} // namespace

// 0x00691D34
void BsFtrMgr::Func691D34(const nn::math::VEC3* position, bool flag, u32 value)
{
    void* actor = const_cast<void*>(Func4EA078(Func4E1338(), position, 0));
    if (actor == NULL) {
        return;
    }
    if (Func7529D0(actor)) {
        if (IsChangingState(Func752840(actor))) {
            if (flag) {
                Func56F190(actor, 186);
            } else {
                Func56F190(actor, Func752840(actor));
            }
        }
    } else if (Func75285C(actor) && stage::IsKind(stage::KIND_OUTDOOR, stage::GetCurrent()) &&
               !Func752074(actor)->IsName(SvFgName::NAME_DREAM_SUITE_BED_INVISIBLE)) {
        Func575054(actor, value);
        Func574AF4(actor);
        return;
    }
    Func575054(actor, value);
    Func57298C(actor);
}

// 0x00692088
void BsFtrMgr::Func692088(const nn::math::VEC3* position, bool flag, u32 value)
{
    void* actor = const_cast<void*>(Func4EA078(Func4E1338(), position, 0));
    if (actor == NULL) {
        return;
    }
    if (Func7529D0(actor)) {
        if (IsChangingState(Func752840(actor))) {
            if (flag) {
                Func56F190(actor, 186);
            } else {
                Func56F190(actor, Func752840(actor));
            }
        }
    } else if (Func75285C(actor) && stage::IsKind(stage::KIND_OUTDOOR, stage::GetCurrent()) &&
               !Func752074(actor)->IsName(SvFgName::NAME_DREAM_SUITE_BED_INVISIBLE)) {
        Func575054(actor, value);
        Func574C50(actor);
        return;
    }
    Func575054(actor, value);
    Func572FD4(actor);
}

// 0x00692010
BsFtrMgr::WorkPool* BsFtrMgr::GetWorkPool()
{
    return &s_WorkPool;
}

// 0x0069201C
BsFtrMgr::CreateFtrArg::CreateFtrArg(const SvFgName& item, u8 x, u8 y, u8 rotation, u8 layer, u8 placeType)
{
    SvFgName name(item);
    name.ClearOrientationIfFurniture();
    this->item = name;
    this->x = x;
    this->rotation = rotation;
    this->y = y;
    this->layer = layer;
    this->placeType = placeType;
}

// 0x00692070
u32 GetQuarterOfAngle(const u16* angle)
{
    return (((*angle >> 12) + 2) & 15) >> 2;
}

// 0x0069222C
s32 BsFtrMgr::FindPlaceOfPlayer(CreateFtrArg* out, const SvFgName* item, u32 unused)
{
    AcPlayer* player = demo::PlayerGetActorEntry(static_cast<netgame::PlayerNo>(4), false);
    if (player == NULL) {
        return 4;
    }
    return FindPlace(out, item, static_cast<const nn::math::VEC3*>(&player->m_Position),
                     player->m_Rotation.y, unused);
}

// 0x00692294
SvFgName BsFtrMgr::GetSongItem()
{
    BsFtrMgr* mgr = s_pInstance;
    if (mgr != NULL && static_cast<u32>(mgr->m_SongIndex) < SONG_NUM) {
        return SvFgName::MakeOfKind(item::KIND_MUSIC, mgr->m_SongIndex);
    }
    return SvFgName(SvFgName::s_Empty);
}

// 0x006922C8
g3d::ResourceLoader* BsFtrMgr::GetTvReceptorLoader()
{
    if (!m_IsTvEnabled) {
        return NULL;
    }
    return &m_TvMgr.m_ReceptorLoader;
}

// 0x006922E0
void* BsFtrMgr::Func6922E0(void* actor)
{
    if (actor != NULL) {
        const u8 kind = Func75288C(actor);
        if (kind == 30) {
            return Func150308(actor);
        }
        if (kind == 31) {
            return Func150310(actor);
        }
    }
    return NULL;
}

// 0x00692320
sead::ExpHeap* BsFtrMgr::GetMannequinHeap(sead::ExpHeap* const* heaps, u32 index)
{
    if (index < MANNEQUIN_HEAP_NUM) {
        return heaps[index];
    }
    return NULL;
}

// 0x00692330
bool FtrPreview::Show(const SvFgName& name)
{
    SvFgName item(name);
    const item::Kind kind = name.GetKind();
    if (kind == item::KIND_DUMMY_WALL || kind == item::KIND_CARPET || kind == item::KIND_PAPER) {
        item.SetName(SvFgName::NAME_UNKNOWN_WALLPAPER);
    }
    if (m_pProcess == NULL && (Func30B658(&item) || Func76D474(&item))) {
        const BsFtrMgr::CreateFtrArg arg(item, 0, 0, 0, 0, BsFtrMgr::CreateFtrArg::PLACE_TYPE_UNKNOWN_INVISIBLE);
        m_Name = name;
        m_pProcess = BsFtrMgr::CreateFtrActor(arg);
    }
    if (m_pProcess != NULL && !Func76BA20(&m_Name, &name)) {
        Hide();
    }
    return true;
}

// 0x006925C0
bool FtrPreview::Hide()
{
    if (m_pProcess != NULL) {
        m_pProcess->StopSelf();
        m_pProcess = NULL;
    }
    m_Name = SvFgName::s_Empty;
    return true;
}

// 0x006925F8
FtrPreview::~FtrPreview()
{
    // empty (in the original too: the destructor of m_Name is a removed call)
}

// 0x0069260C
u32 BsFtrMgr::SearchPointOfPlayer(PointResult* out, bool once)
{
    AcPlayer* player = demo::PlayerGetActorEntry(static_cast<netgame::PlayerNo>(4), false);
    if (player == NULL) {
        return 0;
    }
    return SearchPoint(out, static_cast<const nn::math::VEC3*>(&player->m_Position),
                       player->m_Rotation.y, once);
}

// the steps of SearchPoint (indices into the directions of SearchFtr)
// 0x00847CB4
extern const u8 s_FtrPointSteps[3] = {0, 2, 3};

// 0x006929C0
u32 BsFtrMgr::SearchPoint(PointResult* out, const nn::math::VEC3* position, math::Angle16 angle, bool once)
{
    out->item = SvFgName::s_Empty;
    out->unk4 = -1;
    out->unk8 = -1;
    for (u32 i = 0; i < 3; i++) {
        const u8 step = s_FtrPointSteps[i];
        s32 x, z;
        FdInfo::ToUnit(x, z, *position);
        bool isValid = true;
        if (step != 1) {
            if (step >= 5) {
                isValid = false;
            } else {
                const u32 direction = (s_FtrSearchDirections[step] + (angle.value >> 12)) & 15;
                FtrUnitOffset offset = s_FtrNearOffsets[direction];
                if (step == 4) {
                    offset = s_FtrFarOffsets[direction];
                }
                x += offset.x;
                z += offset.z;
            }
        }
        if (isValid && out != NULL) {
            const nn::math::VEC3 from = *position;
            nn::math::VEC3 center;
            FdInfo::ToCenterWorld(&center, x, z);
            const f32 dx = center.x - from.x;
            const f32 dz = center.z - from.z;
            if (!(std::sqrt(dx * dx + dz * dz) > s_Unknown950340[0])) {
                AcFtr* actor = static_cast<AcFtr*>(Func4EA0B0(Func4E1338(), x, z, 0));
                s32 fromX, fromZ;
                FdInfo::ToUnit(fromX, fromZ, from);
                if (IsUnitReachable(fromX, fromZ, x, z) && actor != NULL && actor->vf_0x90()) {
                    out->item = Func6BB164(actor);
                    s32 a, b;
                    s16 c = 0;
                    Func752298(actor, &a, &b, 0, &c);
                    out->unk4 = a;
                    out->unk8 = b;
                    return Func751EEC(actor) ? 1 : 2;
                }
            }
        }
        if (once && i == 0) {
            break;
        }
    }
    return 0;
}

// 0x00692BEC
void* BsFtrMgr::Func692BEC(void* actor)
{
    if (actor != NULL) {
        const u8 kind = Func75288C(actor);
        if (kind == 30) {
            return Func150318(actor);
        }
        if (kind == 31) {
            return Func150320(actor);
        }
    }
    return NULL;
}

// 0x00692C2C
BsFtrMgr::CommonResLoader* BsFtrMgr::GetCommonResLoader()
{
    return &s_CommonResLoader;
}

// 0x00692FD0
FtrPreview* GetFtrPreview()
{
    return &s_FtrPreview;
}

// the units of the shapes of the furniture (FindPlace): two by two, two by three and one by three
// 0x00847DC4
extern const FtrUnitOffset s_FtrShapeUnits4[4] = {{0, 0}, {-1, 0}, {0, -1}, {-1, -1}};
// 0x00847DE4
extern const FtrUnitOffset s_FtrShapeUnits6[6] = {{0, 0}, {0, -1}, {-1, 0}, {1, 0}, {-1, -1}, {1, -1}};
// 0x00847E14
extern const FtrUnitOffset s_FtrShapeUnits3[3] = {{0, -1}, {-1, -1}, {1, -1}};
// the order of the rotations tried (added to the direction toward the player)
// 0x00847E2C
extern const u32 s_FtrPlaceTurns[4] = {0, 2, 1, 3};

// 0x00692FDC
s32 BsFtrMgr::FindPlace(CreateFtrArg* out, const SvFgName* item, const nn::math::VEC3* position, math::Angle16 angle, u32 unused)
{
    (void)unused;
    const item::ParamItemEntry* entry = item::GetParamItemEntry(*item);
    if (entry == NULL || !Func749394(entry)) {
        return 4;
    }
    if (!Func746234(Func4EA6D0())) {
        return 1;
    }
    if (!Func4E7890() && !stage::IsCampingCar(stage::GetCurrent())) {
        return 4;
    }
    if (Func303BD4(item) && stage::IsTent(stage::GetCurrent())) {
        return 2;
    }
    // the shape of the item: 0/1 one unit (1: turned toward the player), 2 four units, 3 six units, 4 three units
    u32 shape = entry->shape & 15;
    if (shape >= 5) {
        shape = 0;
    }
    s32 playerX, playerZ;
    FdInfo::ToUnit(playerX, playerZ, *position);
    const u32 direction = GetQuarterOfAngle(&angle.value);
    u32 wallMissing = 0;
    u32 wallNotFront = 0;
    u32 unitNum = 0;
    switch (shape) {
    case 0:
    case 1:
        unitNum = 1;
        break;
    case 2:
        unitNum = 4;
        break;
    case 3:
        unitNum = 6;
        break;
    case 4:
        unitNum = 3;
        break;
    }
    // the units of the shapes 3 and 4 behind the unit, turned like the player (1.1 to round the half units)
    s32 units6[6][2];
    s32 units3[3][2];
    const s16 back = static_cast<s16>(direction * 0x4000 + 0x8000);
    for (u32 i = 0; i < 6; i++) {
        nn::math::VEC3 v(static_cast<f32>(s_FtrShapeUnits6[i].x) * 1.1f, 0.0f,
                         static_cast<f32>(s_FtrShapeUnits6[i].z) * 1.1f);
        ssys::ma::RotateY(&v, back);
        units6[i][0] = static_cast<s32>(v.x);
        units6[i][1] = static_cast<s32>(v.z);
    }
    for (u32 i = 0; i < 3; i++) {
        nn::math::VEC3 v(static_cast<f32>(s_FtrShapeUnits3[i].x) * 1.1f, 0.0f,
                         static_cast<f32>(s_FtrShapeUnits3[i].z) * 1.1f);
        ssys::ma::RotateY(&v, back);
        units3[i][0] = static_cast<s32>(v.x);
        units3[i][1] = static_cast<s32>(v.z);
    }
    const u32 facing = (direction + 2) & 3;
    s32 result = 4;
    for (u32 step = 0; step < 5; step++) {
        for (s32 layer = 1; layer >= 0; layer--) {
            if (step == 4 && layer == 0) {
                break;
            }
            s32 x, z;
            FdInfo::ToUnit(x, z, *position);
            if (step != 1) {
                if (step >= 5) {
                    continue;
                }
                const u32 offsetDirection = (s_FtrSearchDirections[step] + (angle.value >> 12)) & 15;
                FtrUnitOffset offset = s_FtrNearOffsets[offsetDirection];
                if (step == 4) {
                    offset = s_FtrFarOffsets[offsetDirection];
                }
                x += offset.x;
                z += offset.z;
            }
            if (layer != 1 && !IsUnitReachable(playerX, playerZ, x, z)) {
                continue;
            }
            for (u16 turn = 0; turn < 4; turn++) {
                u32 rotation = (s_FtrPlaceTurns[turn] + facing) & 3;
                SvFgName rotated;
                Func311860(&rotated, *item, rotation);
                const FtrUnits units(&rotated, 4);
                const s16 rotationAngle = static_cast<s16>(rotation << 14);
                for (u32 k = 0; k < unitNum; k++) {
                    s32 unitX, unitZ;
                    if (shape == 3) {
                        unitX = units6[k][0] + x;
                        unitZ = units6[k][1] + z;
                    } else if (shape == 4) {
                        unitX = units3[k][0] + x;
                        unitZ = units3[k][1] + z;
                    } else {
                        unitX = s_FtrShapeUnits4[k].x + x;
                        unitZ = s_FtrShapeUnits4[k].z + z;
                        if (shape == 1) {
                            // (turned toward the player)
                            nn::math::VEC3 center;
                            units.GetCenter(&center, unitX, unitZ);
                            const math::Angle16 toPlayer =
                                math::Angle16::Atan2(position->x - center.x, position->z - center.z);
                            s32 difference = static_cast<s16>(toPlayer.value - rotationAngle);
                            if (difference < 0) {
                                difference = -difference;
                            }
                            if (difference > 0x4000) {
                                unitX += units.GetOffset(1)[0];
                                unitZ += units.GetOffset(1)[1];
                                rotation = (rotation + 2) & 3;
                            }
                        }
                    }
                    const s32 check = CheckPlace(out, unitX, unitZ, rotation, item, layer, playerX, playerZ,
                                                 direction, step);
                    if (!Func303BD4(item)) {
                        result = check;
                    } else if (layer == 0) {
                        if (check == 3) {
                            wallNotFront++;
                            continue;
                        }
                        if (check == 5) {
                            wallMissing++;
                            continue;
                        }
                    }
                    if (check == 0) {
                        return 0;
                    }
                }
            }
        }
    }
    if (Func303BD4(item) && (wallMissing | wallNotFront) != 0) {
        return wallNotFront > wallMissing ? 3 : 5;
    }
    return result;
}

// 0x006935A0
sead::ExpHeap** BsFtrMgr::GetMannequinHeaps()
{
    return s_MannequinHeaps;
}

// 0x006935AC
void BsFtrMgr::GetTalkItems(SvFgName* item, SvFgName* otherItem)
{
    *item = s_TalkItem;
    *otherItem = s_TalkOtherItem;
}

// 0x006935D4
s32 BsFtrMgr::CheckPlace(CreateFtrArg* out, s32 x, s32 z, u8 rotation, const SvFgName* item, u8 layer, s32 playerX,
                         s32 playerZ, u32 direction, u32 step)
{
    const item::ParamItemEntry* entry = item::GetParamItemEntry(*item);
    const u8* roomParam = Func5360C8(item);
    if (entry == NULL) {
        return 4;
    }
    if (stage::IsKind(stage::KIND_INDOOR, stage::GetCurrent()) && roomParam == NULL) {
        return 4;
    }
    const u8 type = Func7493C4(entry);
    bool isAtPlayer = false;
    if (roomParam != NULL) {
        isAtPlayer = (roomParam[4] >> 2) & 1;
    }
    SvFgName rotated;
    Func311860(&rotated, *item, rotation);
    const FtrUnits units(&rotated, 4);
    const u32 num = units.GetNum();
    nn::math::VEC3 playerCenter;
    FdInfo::ToCenterWorld(&playerCenter, playerX, playerZ);
    Field* field = FdInfo::GetCurrentField();
    const bool hasPlayer = playerX >= 0 && playerZ >= 0;
    s32 group = -1;
    const u32 facing = (direction + 2) & 3;
    for (u32 i = 0; i < num; i++) {
        const s32* offset = units.GetOffset(i);
        const s32 unitX = offset[0] + x;
        const s32 unitZ = offset[1] + z;
        nn::math::VEC3 center;
        FdInfo::ToCenterWorld(&center, unitX, unitZ);
        const SvFgName* fg = field->GetFg(unitX, unitZ, layer);
        if (Func7491EC(entry)) {
            // on a wall: the unit in front of the wall, free, the item turned like the wall
            const u8 attribute = GetAttribute95EC3BAt(unitX, unitZ, FIELD_NAME_CURRENT);
            if (hasPlayer && unitX == playerX && unitZ == playerZ) {
                return 5;
            }
            if (fg == NULL || !fg->IsEmpty()) {
                return 5;
            }
            if (attribute == 0) {
                return 3;
            }
            if (attribute >= 5) {
                return 4;
            }
            if (Func7493C4(entry) == 5 && !FdInfo::Is95EC3BWithout7C_FB(unitX, unitZ, FIELD_NAME_CURRENT)) {
                return 5;
            }
            u32 wall = 3;
            if (attribute == 3) {
                wall = 0;
            } else if (attribute == 4) {
                wall = 1;
            } else if (attribute == 1) {
                wall = 2;
            }
            if (wall != rotation) {
                return 5;
            }
            if (hasPlayer) {
                if (facing != wall) {
                    return 3;
                }
                const s32 dx = playerX - unitX;
                const s32 dz = playerZ - unitZ;
                switch (wall) {
                case 0:
                    if (dz <= 0) {
                        return 3;
                    }
                    break;
                case 1:
                    if (dx <= 0) {
                        return 3;
                    }
                    break;
                case 2:
                    if (dz >= 0) {
                        return 3;
                    }
                    break;
                case 3:
                    if (dx >= 0) {
                        return 3;
                    }
                    break;
                }
            }
            if (!isAtPlayer) {
                // the unit in front of the wall is free and no furniture next to it at the corner
                nn::math::VEC3 wallCenter;
                FdInfo::ToCenterWorld(&wallCenter, unitX, unitZ);
                nn::math::VEC3 front(0.0f, 0.0f, FdInfo::UNIT_WORLD_SIZE_ZF);
                s16 wallAngle = 0;
                if (wall == 1) {
                    wallAngle = 0x4000;
                } else if (wall == 2) {
                    wallAngle = static_cast<s16>(0x8000);
                } else if (wall == 3) {
                    wallAngle = static_cast<s16>(0xC000);
                }
                ssys::ma::RotateY(&front, wallAngle);
                const nn::math::VEC3 frontPosition(wallCenter.x + front.x, wallCenter.y + front.y,
                                                   wallCenter.z + front.z);
                s32 frontX, frontZ;
                FdInfo::ToUnit(frontX, frontZ, frontPosition);
                SvFgName frontItem(*Func4E1374(frontX, frontZ, 0, FdInfo::GetCurrentField(), 0));
                if (Func30B658(&frontItem) && Func303C2C(&frontItem)) {
                    return 5;
                }
                s32 minX, minZ, maxX, maxZ;
                Func2854A0(&minX, &minZ, &maxX, &maxZ);
                s32 cornerX = 0;
                s32 cornerZ = 0;
                if (minX == unitX || maxX == unitX) {
                    if (minZ - 1 == unitZ) {
                        cornerZ = minZ;
                    } else if (maxZ + 1 == unitZ) {
                        cornerZ = maxZ;
                    }
                    cornerX = (minX == unitX) ? minX - 1 : maxX + 1;
                }
                if (minZ == unitZ || maxZ == unitZ) {
                    if (minX - 1 == unitX) {
                        cornerX = minX;
                    } else if (maxX + 1 == unitX) {
                        cornerX = maxX;
                    }
                    cornerZ = (minZ == unitZ) ? minZ - 1 : maxZ + 1;
                }
                void* corner = Func4EA0B0(Func4E1338(), cornerX, cornerZ, 0);
                if (corner != NULL && Func7510D0(corner) && !Func7518B0(corner)) {
                    return 5;
                }
            }
        } else {
            // on the floor: a free unit (not the one of the player)
            if (hasPlayer && !isAtPlayer && unitX == playerX && unitZ == playerZ) {
                return 4;
            }
            if (fg == NULL || !fg->IsEmpty()) {
                return 4;
            }
            if (!Func3000F0(unitX, unitZ, 0)) {
                return 4;
            }
            if (entry->unk_0x1B[1] & 4) {
                // (next to the walls: no furniture in front of the unit along the walls)
                s32 minX, minZ, maxX, maxZ;
                Func2854A0(&minX, &minZ, &maxX, &maxZ);
                if (minX == unitX || maxX == unitX || minZ == unitZ || maxZ == unitZ) {
                    s32 besideX = unitX;
                    s32 besideZ = unitZ;
                    if (minZ == unitZ) {
                        besideZ = unitZ - 1;
                    } else if (maxZ == unitZ) {
                        besideZ = unitZ + 1;
                    }
                    void* beside = Func4EA0B0(Func4E1338(), besideX, besideZ, 0);
                    if (beside != NULL && Func7510D0(beside) && !Func7518B0(beside)) {
                        return 4;
                    }
                    if (minX == unitX) {
                        besideX = unitX - 1;
                    } else if (maxX == unitX) {
                        besideX = unitX + 1;
                    }
                    beside = Func4EA0B0(Func4E1338(), besideX, unitZ, 0);
                    if (beside != NULL && Func7510D0(beside) && !Func7518B0(beside)) {
                        return 4;
                    }
                }
            }
        }
        if (layer == 1) {
            // on top of furniture that carries items (all units on the same furniture)
            AcFtr* base = static_cast<AcFtr*>(Func4EA0B0(Func4E1338(), unitX, unitZ, 0));
            if (base == NULL) {
                return 4;
            }
            if (type != 2 && type != 3) {
                return 4;
            }
            if (!base->vf_0x90()) {
                return 4;
            }
            if (Func752D50(base) != 1) {
                return 4;
            }
            const s32 baseGroup = *reinterpret_cast<const s32*>(reinterpret_cast<const u8*>(base) + 0x304);
            if (group >= 0 && baseGroup != group) {
                return 4;
            }
            group = baseGroup;
            if (hasPlayer && step == 4 && !Func17C5C0(unitX, unitZ, &center, &playerCenter)) {
                return 4;
            }
        }
    }
    u8 placeType = CreateFtrArg::PLACE_TYPE_DISPLAY;
    BsFtrMgr* mgr = s_pInstance;
    if (mgr != NULL && mgr->m_Unknown862 && item->IsName(SvFgName::NAME_LANTERN)) {
        placeType = CreateFtrArg::PLACE_TYPE_UNKNOWN;
    }
    *out = CreateFtrArg(*item, x, z, rotation, layer, placeType);
    return 0;
}

// 0x00693E34
bool BsFtrMgr::IsTalkStage(stage::Name stage)
{
    if (stage::IsKind(stage::KIND_MODEL_HOUSE, stage)) {
        return true;
    }
    if (stage::IsKind(stage::KIND_MUSEUM_SHOWROOM, stage) && !stage::IsOwnShowroom(stage)) {
        return true;
    }
    if (!Func4E79CC(stage::GetCurrent())) {
        return false;
    }
    const TourData* tour = TourData::s_pInstance;
    if (tour != NULL && Func76F7E4(&tour->m_Seed) == 2) {
        return false;
    }
    return true;
}

// 0x00693EE8
bool BsFtrMgr::IsHouseStage()
{
    const stage::Name current = stage::GetCurrent();
    const u8* npcHouse = static_cast<const u8*>(Func2FE4A0(stage::GetNpcHouseIndex(stage::GetCurrent())));
    if (stage::IsKind(stage::KIND_PLAYER_HOUSE, current) || stage::IsKind(stage::KIND_MODEL_HOUSE, current) ||
        stage::IsKind(stage::KIND_MUSEUM_SHOWROOM, current)) {
        return true;
    }
    if (stage::IsKind(stage::KIND_NPC_HOUSE, current) && npcHouse != NULL &&
        (*reinterpret_cast<const u32*>(npcHouse + 0x44) & 0x20)) {
        return true;
    }
    return false;
}

// 0x00693F78
bool BsFtrMgr::IsMusicStage()
{
    const stage::Name current = stage::GetCurrent();
    return stage::IsKind(stage::KIND_PLAYER_HOUSE, current) || stage::IsKind(stage::KIND_NPC_HOUSE, current) ||
           stage::IsKind(stage::KIND_MUSEUM_SHOWROOM, current) || stage::IsKind(stage::KIND_MODEL_HOUSE, current) ||
           stage::IsKind(stage::KIND_RV_CAR_ENVIRONMENT, current) || stage::IsHutOfBorrowingRace(current);
}

// 0x00694000
bool BsFtrMgr::IsUnitFree(s32 x, s32 z)
{
    if (Func3000F0(x, z, 0)) {
        return true;
    }
    const u8 attribute = GetAttribute95EC3BAt(x, z, FIELD_NAME_CURRENT);
    return attribute != 0 && attribute != 5;
}

// 0x00694080
s32 BsFtrMgr::FindPlaceOfMyDesign(CreateFtrArg* out, u32 item, u32 design, u32 unused, bool isWall)
{
    AcPlayer* player = demo::PlayerGetActorEntry(static_cast<netgame::PlayerNo>(4), false);
    SvFgName probe(static_cast<SvFgName::ID>(SvFgName::NAME_CUSTOM_SHIRT));
    if (player == NULL || !Func304E74(&probe, design)) {
        return 4;
    }
    const SvMyDesign* data = static_cast<const SvMyDesign*>(Func76AD48(&probe));
    if (data == NULL) {
        return 4;
    }
    u32 id = item;
    if (data->IsWearable()) {
        // the clothes and hats of the shape of the design (the wall versions follow 7 later)
        if (data->patternType <= 5) {
            switch (data->GetClothesType()) {
            case 0:
                id = SvFgName::NAME_CUSTOM_DESIGN_LONG_SLEEVE_DRESS;
                if (isWall) {
                    id += 7;
                }
                break;
            case 1:
                id = SvFgName::NAME_CUSTOM_DESIGN_SLEEVE_DRESS;
                if (isWall) {
                    id += 7;
                }
                break;
            case 2:
                id = SvFgName::NAME_CUSTOM_DESIGN_TANK_DRESS;
                if (isWall) {
                    id += 7;
                }
                break;
            case 3:
                id = SvFgName::NAME_CUSTOM_DESIGN_LONG_SLEEVE_SHIRT;
                if (isWall) {
                    id += 7;
                }
                break;
            case 4:
                id = SvFgName::NAME_CUSTOM_DESIGN_T_SHIRT_DISTORTED_SLEEVES;
                if (isWall) {
                    id += 7;
                }
                break;
            case 5:
                id = SvFgName::NAME_CUSTOM_DESIGN_TANK_SHIRT;
                if (isWall) {
                    id += 7;
                }
                break;
            }
        } else if (static_cast<u32>(data->patternType - 6) <= 1) {
            const u32 hat = data->GetHatType();
            if (hat == 6) {
                id = SvFgName::NAME_CUSTOM_DESIGN_HORNED_HAT;
            } else if (hat == 7) {
                id = SvFgName::NAME_CUSTOM_DESIGN_KNIT_HAT;
            } else if (hat == 11) {
                id = SvFgName::NAME_CUSTOM_DESIGN_FEMALE_HAT;
            }
        }
    }
    SvFgName name(static_cast<SvFgName::ID>(id));
    Func304E74(&name, design);
    return FindPlace(out, &name, static_cast<const nn::math::VEC3*>(&player->m_Position),
                     player->m_Rotation.y, unused);
}

// 0x00694250
void BsFtrMgr::ClearTalkItems()
{
    s_TalkItem = s_TalkOtherItem = SvFgName::s_Empty;
}

// 0x0069453C
s32 BsFtrMgr::FindPlaceOfMyDesignCanvas(CreateFtrArg* out, u32 design, bool isWallCanvas)
{
    return FindPlaceOfMyDesign(out, isWallCanvas ? SvFgName::NAME_PATTERN_WALL_CANVAS : SvFgName::NAME_PATTERN_EASEL,
                               design, 1, false);
}

// 0x00694770
void BsFtrMgr::StopMusic(s32 fadeFrames)
{
    if (IsMusicStage()) {
        m_MusicAudio.StopMusic(fadeFrames);
    }
}

// 0x006947A0
u16 GetAngleOfQuarter(u32 quarter)
{
    switch (quarter) {
    case 1:
        return 0x4000;
    case 2:
        return 0x8000;
    case 3:
        return 0xC000;
    }
    return 0;
}

// 0x00694AC8
void BsFtrMgr::LoadMusic(u32 song)
{
    if (IsMusicStage() && song < Func5360A8(item::KIND_MUSIC)->count) {
        m_MusicAudio.LoadMusic(song);
    }
}

namespace {
// the model at position turned by rotation (the matrix of the common model)
inline void SetCommonModelMatrix(g3d::SkeletalModel* model, const nn::math::VEC3* position,
                                 const math::Angle16* rotation)
{
    nn::math::MTX34 mtx;
    mtx.m[0][0] = 1.0f;
    mtx.m[0][1] = 0.0f;
    mtx.m[0][2] = 0.0f;
    mtx.m[0][3] = position->x;
    mtx.m[1][0] = 0.0f;
    mtx.m[1][1] = 1.0f;
    mtx.m[1][2] = 0.0f;
    mtx.m[1][3] = position->y;
    mtx.m[2][0] = 0.0f;
    mtx.m[2][1] = 0.0f;
    mtx.m[2][2] = 1.0f;
    mtx.m[2][3] = position->z;
    ssys::ma::RotateY(&mtx, static_cast<s16>(rotation->value));
    model->SetMatrix(mtx);
}
} // namespace

// 0x00694CC4
bool BsFtrMgr::CommonModel::StartPull(const nn::math::VEC3* position, const math::Angle16* rotation, const void* owner)
{
    if (m_pOwner != NULL) {
        return false;
    }
    anim.ChangeAnim(nw::anim::res::ResAnim(m_pPullAnim));
    anim.SetSpeed(1.0f);
    model.SetSkeletalAnim(&anim);
    m_pOwner = owner;
    SetCommonModelMatrix(&model, position, rotation);
    return true;
}

// 0x00694D94
bool BsFtrMgr::CommonModel::StartPush(const nn::math::VEC3* position, const math::Angle16* rotation, const void* owner)
{
    if (m_pOwner != NULL) {
        return false;
    }
    anim.ChangeAnim(nw::anim::res::ResAnim(m_pPushAnim));
    anim.SetSpeed(1.0f);
    model.SetSkeletalAnim(&anim);
    m_pOwner = owner;
    SetCommonModelMatrix(&model, position, rotation);
    return true;
}

// 0x00694E64
bool BsFtrMgr::CommonModel::Update(nn::math::VEC3* out, const void* owner)
{
    if (m_pOwner == NULL || m_pOwner != owner) {
        return false;
    }
    model.UpdateAnims();
    model.Update();
    nn::math::VEC3 position;
    model.GetBoneWorldPosition(&position, 0);
    *out = position;
    if (!anim.IsFinished()) {
        return false;
    }
    m_pOwner = NULL;
    return true;
}

// 0x00694EF0
BsFtrMgr::CommonModel::~CommonModel()
{
    // (only the members are destroyed)
}

// 0x0069427C
void BsFtrMgr::FindNearestTvAndTheremin(nn::math::VEC3* tvPosition, u8* channel, nn::math::VEC3* thereminPosition,
                                        f32* distance, s32* thereminIndex)
{
    tvPosition->x = 0.0f;
    tvPosition->y = 0.0f;
    tvPosition->z = 0.0f;
    *channel = 0;
    thereminPosition->x = 0.0f;
    thereminPosition->y = 0.0f;
    thereminPosition->z = 0.0f;
    *distance = 0.0f;
    *thereminIndex = -1;
    const u32 num = stage::GetMaxFurniture(stage::GetCurrent());
    const nn::math::VEC3* player = Func5C2308(4, true);
    if (player == NULL) {
        return;
    }
    nn::math::VEC3 tv(0.0f, 0.0f, 0.0f);
    u8 tvChannel = 0;
    s32 nearest = -1;
    f32 nearestSquare = 0.0f;
    nn::math::VEC3 theremin;
    for (u32 i = 0; i < num; i++) {
        void* actor = Func4E9C14(Func4EA6D0(), i);
        if (actor == NULL || !Func7510AC(actor)) {
            continue;
        }
        if (Func752824(actor)) {
            // the TV nearest to the player (on the ground)
            const nn::math::VEC3* position = Func7519D4(actor);
            if (!(std::fabs(tv.x * tv.x + tv.y * tv.y + tv.z * tv.z) <= FLT_EPSILON)) {
                const f32 current = std::sqrt(GetDistanceSquareXZ(player, tv));
                const f32 next = std::sqrt(GetDistanceSquareXZ(player, *position));
                if (current <= next) {
                    continue;
                }
            }
            tv = *position;
            tvChannel = Func752840(actor);
        } else if (Func7510B8(actor)) {
            // the theremin nearest to any player
            nn::math::VEC3 position;
            Func75242C(actor, &position, 0);
            if (netgame::IsMultiPlayer()) {
                for (u32 p = 0; p < 4; p++) {
                    const nn::math::VEC3* other = Func5C2308(p, true);
                    if (other == NULL) {
                        continue;
                    }
                    const f32 square = GetDistanceSquareXZ(other, position);
                    if (nearest == -1 || nearestSquare > square) {
                        theremin = position;
                        nearestSquare = square;
                        nearest = i;
                    }
                }
            } else {
                const f32 square = GetDistanceSquareXZ(player, position);
                if (nearest == -1 || nearestSquare > square) {
                    theremin = position;
                    nearestSquare = square;
                    nearest = i;
                }
            }
        }
    }
    *tvPosition = tv;
    *channel = tvChannel;
    *thereminPosition = theremin;
    *distance = std::sqrt(nearestSquare);
    *thereminIndex = Func5C1B04(4) ? -1 : nearest;
}

// 0x00691CC0
void BsFtrMgr::ThrowItem(const SvFgName& item, const nn::math::VEC3& target)
{
    if (s_pInstance == NULL) {
        return;
    }
    s_pInstance->StartFlyingItem(s_pInstance->m_FlyingItems, item, target);
}

// 0x00694594
void BsFtrMgr::StartFlyingItem(FlyingItem* items, const SvFgName& item, const nn::math::VEC3& target)
{
    AcPlayer* player = demo::PlayerGetActorEntry(static_cast<netgame::PlayerNo>(4), false);
    if (player == NULL) {
        return;
    }
    const f32 frames = static_cast<f32>(g_ThrowItemFrames);
    for (FlyingItem* it = items; it < items + FLYING_ITEM_NUM; it++) {
        if (!it->item.IsEmpty()) {
            continue;
        }
        it->timer = g_ThrowItemFrames;
        const nn::math::VEC3_& from = player->m_Position;
        it->position.x = from.x;
        it->position.y = from.y;
        it->position.z = from.z;
        it->position.y = FdInfo::UNIT_WORLD_SIZE_HALF_XF;
        it->target = target;
        it->velocity.x = (target.x - from.x) / frames;
        it->velocity.z = (target.z - from.z) / frames;
        it->scale = 0.0f;
        // the fall of the flight is added up frame by frame
        f32 fall = 0.0f;
        f32 velocity = 0.0f;
        for (u32 i = 0; static_cast<f32>(i) < frames; i++) {
            fall += velocity;
            velocity -= s_FlyingItemGravity;
        }
        it->velocity.y = (target.y - it->position.y - fall) / frames;
        it->item = item;
        return;
    }
}


// the sizes of the furniture actors that the pool holds (the largest decides the block size)
// 0x00847E3C
extern const u32 s_FtrActorSizes[40] = {
    2104, 2112, 2272, 2272, 2112, 2112, 2112, 2288, 2104, 2104, 2128, 2240, 2112, 2288, 2272, 2440, 2104, 2184, 2112, 2368,
    2936, 2184, 2104, 2104, 2120, 2104, 2640, 2384, 2272, 2112, 2112, 2104, 2112, 2104, 2128, 2104, 2368, 2368, 2112, 2896,
};

// 0x0098BF24
BsFtrMgr::WorkPool BsFtrMgr::s_WorkPool;

// 0x00694B74
bool BsFtrMgr::WorkPool::Initialize(u32 num)
{
    if (m_Num != 0) {
        return true;
    }
    m_BlockSize = 0;
    m_pBuffer = NULL;
    std::memset(m_Bits, 0, sizeof(m_Bits));
    for (u32 i = 0; i < sizeof(s_FtrActorSizes) / sizeof(s_FtrActorSizes[0]); i++) {
        if (s_FtrActorSizes[i] > m_BlockSize) {
            m_BlockSize = s_FtrActorSizes[i];
        }
    }
    m_BlockSize = (m_BlockSize + 31) & ~31;
    const u32 size = m_BlockSize * num;
    if (size == 0) {
        return true;
    }
    sead::Heap* heap = HeapInit::s_pStageHeap;
    if (heap == NULL) {
        return false;
    }
    m_pBuffer = static_cast<u8*>(heap->tryAlloc(size, 4));
    if (m_pBuffer == NULL) {
        return false;
    }
    m_Num = num;
    return true;
}

// 0x00694C50
void* BsFtrMgr::WorkPool::Alloc()
{
    for (u32 i = 0; i < m_Num; i++) {
        if ((m_Bits[i >> 3] >> (i & 7)) & 1) {
            continue;
        }
        m_Bits[i >> 3] |= 1 << (i & 7);
        return m_pBuffer + m_BlockSize * i;
    }
    return NULL;
}

// 0x00694B0C
bool BsFtrMgr::WorkPool::Free(void* block)
{
    for (u32 i = 0; i < m_Num; i++) {
        if (m_pBuffer + m_BlockSize * i == block) {
            m_Bits[i >> 3] &= ~(1 << (i & 7));
            return true;
        }
    }
    return false;
}

void BsFtrMgr::WorkPool::Finalize()
{
    if (m_pBuffer == NULL) {
        return;
    }
    HeapInit::s_pStageHeap->free(m_pBuffer);
    m_BlockSize = 0;
    m_Num = 0;
    m_pBuffer = NULL;
    std::memset(m_Bits, 0, sizeof(m_Bits));
}
