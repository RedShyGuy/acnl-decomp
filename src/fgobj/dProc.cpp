#include "Other/dBase.h"
#include "fgobj/dProc.h"
#include "fgobj/dObjectBase.h"
#include "fgobj/fgobj_Globals.h"
#include <cstring>
#include "fgobj/fgobj_Api.h"
#include "fgobj/fgobj_Extern.h"
#include "sead/seadStringFormatter.h"
#include "sead/seadFixedSafeString.h"
#include "time/time_Api.h"
#include "Other/dWorldCurve.h"
#include "Player/dPlayerApi.h"
#include "script/dScriptExtern.h"
#include "Sv/dSvMail.h"
#include "Sv/dSvShopZakkaExtern.h"
#include "fgobj/dObjectExe.h"
#include "Sound/dSoundMgr.h"
#include "fgobj/dPlace.h"
#include "Sv/dSvFgNameExtern.h"
#include "fgobj/dObjectFruit.h"
#include "fgobj/dWaterPacket.h"
#include "Bs/dBsSvMgr.h"
#include "netgame/netgame_Api.h"
#include "stage/stage_Api.h"
#include "fgobj/dObjectSwing.h"
#include "Other/dHeapInit.h"
#include "Sv/dSvFgName.h"
#include "sead/seadFrameHeap.h"
#include "sead/seadExpHeap.h"
#include "Camera/dCameraMgr.h"
#include "field/dField.h"
#include "field/dFdInfo.h"
#include "g3d/g3d_System.h"
#include "graphic/dScreenCamera.h"
#include "fgobj/dObjectDeco.h"
#include "fgobj/dDrawListNode.h"
#include "nw/gfx/gfx_MeshRenderer.h"
#include "nw/gfx/gfx_Api.h"
#include "nw/gfx/res/gfx_ResModel.h"
#include "nw/gfx/res/gfx_ResMaterial.h"
#include "stage/stage_Extern.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvGardenPlus.h"
#include "nw/gfx/res/gfx_ResTexture.h"
#include "nn/gx/gx_Api.h"

namespace fgobj {
// 0x0059F2F8 (name is ours)
fgobj::Proc::Proc()
{
    m_Glitter.m_NearNum = 0;
    m_Glitter.m_Num = 0;
    std::memset(m_Glitter.m_Timers, FlowerGlitter::TIMER_FRAMES, sizeof(m_Glitter.m_Timers));
    m_Glitter.m_Delay = 0;
    m_QueuedUnitNum = 0;
    std::memset(m_QueuedUnitX, 0, sizeof(m_QueuedUnitX));
    std::memset(m_QueuedUnitZ, 0, sizeof(m_QueuedUnitZ));
    for (u32 i = 0; i < 100; i++) {
        m_Unknown0x321C[i] = 0;
    }
    for (u32 i = 0; i < 100; i++) {
        m_Unknown0x33AC[i] = 0;
    }
    for (u32 i = 0; i < 3; i++) {
        UnitArea& area = m_Areas[i];
        area.scale = 0.0f;
        area.minX = 0;
        area.minZ = 0;
        area.maxX = 0;
        area.maxZ = 0;
    }
    m_CameraUnitX = 0;
    m_CameraUnitZ = 0;
    m_FieldObjectCounter = -1000;
    m_RemovedObjectCounter = 0;
    m_LoadState = 0;
    m_UnitMinX = 0;
    m_UnitMinZ = 0;
    m_UnitMaxX = 0;
    m_UnitMaxZ = 0;
    m_pSettingModels[1] = NULL;
    m_pSettingModels[0] = NULL;
}

// 0x0059F64C slot 0x00 | slot vf_0x00 of oml::framework::Process
// 0x0059F57C (deleting dtor)
fgobj::Proc::~Proc()
{
    // (only the members are destroyed)
}

// 0x0059573C (name is ours)
bool fgobj::Proc::UnitArea::Contains(s32 x, s32 z) const
{
    if (x < minX || x > maxX) {
        return false;
    }
    if (z < minZ || z > maxZ) {
        return false;
    }
    const s32 narrowing = static_cast<s32>(scale * static_cast<f32>(z - minZ));
    if (narrowing != 0 && !(minX + narrowing <= x && x <= maxX - narrowing)) {
        return false;
    }
    return true;
}

// 0x005957BC (name is ours)
void fgobj::Proc::UnitArea::Set(const nn::math::VEC3* center, f32 halfWidth, f32 back, f32 front, f32 narrowing)
{
    nn::math::VEC3 min = *center;
    nn::math::VEC3 max;
    min.x = center->x - halfWidth;
    min.z = center->z - back;
    max.x = center->x + halfWidth;
    max.y = center->y;
    max.z = center->z + front;
    FdInfo::ToUnit(minX, minZ, min);
    FdInfo::ToUnit(maxX, maxZ, max);
    if (maxZ == minZ) {
        scale = 0.0f;
    } else {
        scale = narrowing / static_cast<f32>(maxZ - minZ);
    }
}

namespace {
// the narrowing of the shown area
const f32 AREA_NARROWING = 4.5f;
const f32 AREA_NARROWING_WIDE = 4.2f;
} // namespace

extern const f32 s_AreaUnitSizeX;
extern const f32 s_AreaUnitSizeZ;

// 0x0059C730 (name is ours)
bool fgobj::Proc::UpdateAreas()
{
    if (s_IsIndoor) {
        UnitArea& area = m_Areas[0];
        area.minX = m_UnitMinX;
        area.minZ = m_UnitMinZ;
        area.maxX = m_UnitMaxX;
        area.maxZ = m_UnitMaxZ;
        if (area.maxZ == area.minZ) {
            area.scale = 0.0f;
        } else {
            area.scale = 0.0f / static_cast<f32>(area.maxZ - area.minZ);
        }
        return true;
    }
    camera::Mgr* camera = camera::Mgr::s_pInstance;
    if (camera == NULL) {
        return false;
    }
    nn::math::VEC3 center;
    center.x = camera->m_pUnk0x1A4->m_Point.x;
    center.y = camera->m_pUnk0x1A4->m_Point.y;
    center.z = camera->m_pUnk0x1A4->m_Point.z;
    FdInfo::ToUnit(m_CameraUnitX, m_CameraUnitZ, center);
    f32 narrowing = AREA_NARROWING;
    const f32 sizeZ = s_AreaUnitSizeZ;
    const f32 createFront = sizeZ * 3.2f;
    const f32 showFront = sizeZ * 1.9f;
    if (stage::IsKind(stage::KIND_DOWNTOWN, stage::GetCurrent())) {
        const f32 halfWidth = s_AreaUnitSizeX * 10.1f;
        m_Areas[0].Set(&center, halfWidth, s_AreaUnitSizeX * 15.1f, showFront, narrowing);
        m_Areas[1].Set(&center, halfWidth, s_AreaUnitSizeZ * 15.1f, createFront, 3.2f);
    } else {
        const bool isExhibition = stage::IsKind(stage::KIND_EXHIBITION, stage::GetCurrent());
        const f32 createHalfWidth = s_AreaUnitSizeX * 8.5f;
        const f32 createBack = s_AreaUnitSizeZ * 13.6f;
        const f32 createNarrowing = 4.8f;
        if (isExhibition) {
            m_Areas[0].Set(&center, s_AreaUnitSizeX * 7.5f, s_AreaUnitSizeX * 12.6f, s_AreaUnitSizeZ * 2.1f, narrowing);
            m_Areas[1].Set(&center, createHalfWidth, createBack, createFront, createNarrowing);
        } else {
            // the wide areas on some ground of the block of the camera
            bool isWide = true;
            if (!s_Unk94FE5C) {
                s32 blockX;
                s32 blockZ;
                FdInfo::ToBlock(blockX, blockZ, m_CameraUnitX, m_CameraUnitZ);
                Field* field = FdInfo::GetCurrentField();
                isWide = false;
                if (field != NULL) {
                    BgInfo* bg = field->GetBg(blockX, blockZ);
                    if (bg != NULL) {
                        switch (Func6060F8(bg->GetName())) {
                        case 11:
                        case 12:
                        case 13:
                        case 16:
                        case 17:
                            isWide = true;
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
            if (isWide) {
                narrowing = AREA_NARROWING_WIDE;
                m_Areas[0].Set(&center, s_AreaUnitSizeX * 8.25f, s_AreaUnitSizeX * 17.2f, s_AreaUnitSizeZ * 4.0f, narrowing);
                m_Areas[1].Set(&center, s_AreaUnitSizeX * 10.0f, s_AreaUnitSizeZ * 19.4f, s_AreaUnitSizeZ * 5.6f, 6.0f);
            } else {
                m_Areas[0].Set(&center, s_AreaUnitSizeX * 8.15f, s_AreaUnitSizeX * 13.8f, showFront, narrowing);
                m_Areas[1].Set(&center, createHalfWidth, createBack, createFront, createNarrowing);
            }
        }
    }
    // the shown area one unit smaller
    UnitArea& inner = m_Areas[2];
    const UnitArea& shown = m_Areas[0];
    inner.maxZ = shown.maxZ - 1;
    inner.minZ = shown.minZ;
    inner.maxX = shown.maxX - 1;
    inner.minX = shown.minX + 1;
    if (inner.maxZ == inner.minZ) {
        inner.scale = 0.0f;
    } else {
        inner.scale = narrowing / static_cast<f32>(inner.maxZ - inner.minZ);
    }
    return true;
}

namespace {
// the pool heaps: the number of small heaps of the auto camp / indoors, their size there / outdoors, the size of a large
// heap and the size of the management of a heap
const u32 SMALL_HEAP_NUM_AUTO_CAMP = 200;
const u32 SMALL_HEAP_NUM_INDOOR = 100;
const u32 POOL_HEAP_SIZE_SMALL = 1712;
const u32 POOL_HEAP_SIZE_SMALL_OUTDOOR = 1552;
const u32 POOL_HEAP_SIZE_LARGE = 0x1ADC;
const u32 HEAP_HEADER_SIZE = 16;
const u32 OBJECT_HEAP_EXTRA_SIZE = 220;
} // namespace

// 0x0059CC8C (name is ours)
void fgobj::Proc::CreateHeaps()
{
    if (m_Allocator3.m_pHeap != NULL) {
        return;
    }
    sead::Heap* parent = m_Allocator2.m_pHeap;
    u32 smallNum;
    u32 largeNum;
    u32 smallSize;
    if (stage::IsKind(stage::KIND_AUTO_CAMP, stage::GetCurrent())) {
        smallNum = SMALL_HEAP_NUM_AUTO_CAMP;
        largeNum = LARGE_HEAP_NUM;
        smallSize = POOL_HEAP_SIZE_SMALL;
    } else if (s_IsOutdoor) {
        smallNum = SMALL_HEAP_NUM;
        largeNum = LARGE_HEAP_NUM;
        smallSize = POOL_HEAP_SIZE_SMALL_OUTDOOR;
    } else {
        smallNum = SMALL_HEAP_NUM_INDOOR;
        largeNum = 0;
        smallSize = POOL_HEAP_SIZE_SMALL;
    }
    m_Allocator3.CreateExpHeap(smallNum * (smallSize + HEAP_HEADER_SIZE) + largeNum * (POOL_HEAP_SIZE_LARGE + HEAP_HEADER_SIZE) +
                                   OBJECT_HEAP_EXTRA_SIZE,
                               parent, sead::SafeStringBase<char>("BsFgObject"), sead::Heap::HEAP_DIRECTION_FORWARD, false);
    sead::Heap* heap = m_Allocator3.m_pHeap;
    const u32 size = (stage::IsKind(stage::KIND_AUTO_CAMP, stage::GetCurrent()) || s_IsIndoor) ? POOL_HEAP_SIZE_SMALL : POOL_HEAP_SIZE_SMALL_OUTDOOR;
    u32 i;
    for (i = 0; i < smallNum; i++) {
        s_SmallHeaps[i] = sead::FrameHeap::create(size, sead::SafeStringBase<char>("heapArrayS"), heap, sead::Heap::HEAP_DIRECTION_FORWARD, false);
    }
    for (; i < SMALL_HEAP_NUM; i++) {
        s_SmallHeaps[i] = NULL;
    }
    for (i = 0; i < largeNum; i++) {
        s_LargeHeaps[i] = sead::ExpHeap::create(POOL_HEAP_SIZE_LARGE, sead::SafeStringBase<char>("heapArrayL"), heap, sead::Heap::HEAP_DIRECTION_FORWARD, false);
    }
    for (; i < LARGE_HEAP_NUM; i++) {
        s_LargeHeaps[i] = NULL;
    }
}

namespace {
// the sizes of the heaps of the Proc (outdoors / indoors) and of the design textures
const u32 SETUP_HEAP_SIZE_OUTDOOR = 0x1E000;
const u32 SETUP_HEAP_SIZE_INDOOR = 0xDC00;
const u32 RESOURCE_HEAP_SIZE_OUTDOOR = 0x20E000;
const u32 RESOURCE_HEAP_SIZE_INDOOR = 0x100400;
const u32 MY_DESIGN_HEAP_SIZE = 0x19800;
// the number of free large pool heaps a swinging object needs
const u32 SWING_FREE_HEAP_NUM = 4;

// whether more than SWING_FREE_HEAP_NUM large pool heaps are free
inline bool HasFreeLargeHeaps()
{
    u32 num = 0;
    for (u32 i = 0; i < LARGE_HEAP_NUM; i++) {
        if (s_LargeHeaps[i] != NULL) {
            num++;
        }
    }
    return num > SWING_FREE_HEAP_NUM;
}
} // namespace

// 0x0059D05C (name is ours)
bool SwingBushObject(const SvFgName* fg, s32 x, s32 z)
{
    if (s_pProc == NULL || !FdInfo::GetCurrentField()->IsInside(x, z) || !HasFreeLargeHeaps()) {
        return false;
    }
    ObjectSwing* object = ObjectSwing::Create(*fg, x, z, ObjectSwing::KIND_SHAKE_S, false);
    if (object == NULL) {
        return false;
    }
    s_pProc->RemoveObjectsAt(x, z, Proc::REMOVE_MODE_NO_FLAG_4);
    Proc* proc = s_pProc;
    ObjectBase* other = static_cast<ObjectBase*>(proc->m_ExeObjects.GetFirst());
    while (other != NULL) {
        ObjectBase* next = static_cast<ObjectBase*>(other->m_pNext);
        if (other->m_X == x && other->m_Z == z) {
            proc->RemoveObject(other, false, true);
        }
        other = next;
    }
    s_pProc->m_ExeObjects.InsertByPriority(object);
    s_pProc->m_FieldBit.Set(x, z);
    return true;
}

// 0x0059D18C (name is ours)
bool SwingTreeObject(const SvFgName* fg, s32 x, s32 z, u8 kind)
{
    if (s_pProc == NULL || !FdInfo::GetCurrentField()->IsInside(x, z) || !HasFreeLargeHeaps()) {
        return false;
    }
    for (ObjectBase* object = static_cast<ObjectBase*>(s_pProc->m_ExeObjects.GetFirst()); object != NULL;
         object = static_cast<ObjectBase*>(object->m_pNext)) {
        if (object->m_X == x && object->m_Z == z && !object->vf_0x10()) {
            return false;
        }
    }
    return PlaceSwing(*fg, SvFgName::s_Empty, x, z, NULL, kind, false);
}

// 0x0059D2C8 (name is ours)
void fgobj::Proc::CreateResourceHeaps()
{
    if (m_Allocator2.m_pHeap != NULL) {
        return;
    }
    m_LoadState = LOAD_STATE_NONE;
    s_TreeTextureAddresses[0] = 0;
    s_TreeTextureAddresses[1] = 0;
    u32 setupSize;
    u32 resourceSize;
    sead::Heap* heap = HeapInit::s_pStageHeap;
    if (s_IsOutdoor) {
        setupSize = SETUP_HEAP_SIZE_OUTDOOR;
        resourceSize = RESOURCE_HEAP_SIZE_OUTDOOR;
        s_pMyDesignTextures = NULL;
        if (m_MyDesignTextures.m_Allocator.m_pHeap == NULL) {
            m_MyDesignTextures.m_Allocator.CreateExpHeap(MY_DESIGN_HEAP_SIZE, heap, sead::SafeStringBase<char>("BsFgProc::MyDsnMgr"),
                                                        sead::Heap::HEAP_DIRECTION_FORWARD, false);
        }
    } else {
        setupSize = SETUP_HEAP_SIZE_INDOOR;
        resourceSize = RESOURCE_HEAP_SIZE_INDOOR;
    }
    m_Allocator.CreateExpHeap(setupSize, heap, sead::SafeStringBase<char>("BsFgProc::Setup"), sead::Heap::HEAP_DIRECTION_FORWARD, false);
    m_Allocator2.CreateExpHeap(resourceSize, heap, sead::SafeStringBase<char>("BsFgProc::Resource"), sead::Heap::HEAP_DIRECTION_FORWARD,
                               false);
    m_LoadState = LOAD_STATE_LOAD;
}

// 0x0059D3E0 (name is ours)
fgobj::ObjectBase* fgobj::Proc::FindFieldObject(s32 x, s32 z) const
{
    for (ObjectBase* object = static_cast<ObjectBase*>(m_FieldObjects.GetFirst()); object != NULL;
         object = static_cast<ObjectBase*>(object->m_pNext)) {
        if (object->m_X == x && object->m_Z == z) {
            return object;
        }
    }
    return NULL;
}

namespace {
// the frames of the sway of a field object
const u8 SWAY_FRAMES = 10;
} // namespace

// 0x0059D41C (name is ours)
void SwayFieldObject(s32 x, s32 z)
{
    if (s_pProc == NULL || !FdInfo::GetCurrentField()->IsInside(x, z)) {
        return;
    }
    ObjectBase* object = s_pProc->FindFieldObject(x, z);
    if (object != NULL && (object->m_Flags & ObjectBase::FLAG_UNK_8)) {
        object->m_Unk0x11C = SWAY_FRAMES;
    }
}

// 0x0059AB58 (name is ours)
bool fgobj::Proc::LoadSeasonResources()
{
    switch (m_LoadState) {
    case LOAD_STATE_LOAD: {
        sead::Heap* heap = m_Allocator2.m_pHeap;
        if (!s_IsOutdoor) {
            m_LoadState = LOAD_STATE_DONE;
            return true;
        }
        bool isLoaded = m_FgModelLoad.Read(sead::SafeStringBase<char>("Fg/Param/FgShape.bin"), heap, 4);
        const nn::fnd::DateTimeParameters date = time::GetSavedTime();
        sead::FixedSafeString<32> path;
        {
            sead::StringFormatter formatter(&path);
            formatter.append("Fg/Bcres/Season%02d.bcres").appendValue(static_cast<s32>(time::GetSeasonPart(date))).flush();
        }
        if (!(m_FgLoader.LoadAsync(path, heap, 128) & isLoaded)) {
            return false;
        }
        if (!s_IsOutdoor) {
            m_LoadState = LOAD_STATE_DONE;
            return true;
        }
        m_FgLoader.Read(0x20000, 0x20000, 0x30000);
        m_FgLoader.InitializeCgfx(&m_Allocator, GetFgLut(), true);
        m_FgLoader.InitializeCgfx(&m_Allocator, NULL, true);
        m_LoadState = LOAD_STATE_SETUP;
        return false;
    }
    case LOAD_STATE_SETUP:
        m_LoadState = LOAD_STATE_FINISH;
        return false;
    case LOAD_STATE_FINISH:
        Func179330(&m_FgLoader, m_Allocator2.m_pHeap);
        m_LoadState = LOAD_STATE_DONE;
        SetTreeTextureAddress(0, "treeA");
        SetTreeTextureAddress(1, "treeA2");
        return true;
    default:
        return true;
    }
}

// 0x0059B12C (name is ours)
bool fgobj::Proc::LoadItemResources()
{
    switch (m_LoadState) {
    case LOAD_STATE_DONE: {
        sead::Heap* heap = m_Allocator2.m_pHeap;
        bool isLoaded;
        if (stage::IsKind(stage::KIND_AUTO_CAMP, stage::GetCurrent())) {
            isLoaded = m_ItemLoader.LoadAsync(sead::SafeStringBase<char>("Fg/Bcres/FgItemAutoCamp.bcres"), heap, 128);
        } else if (s_IsOutdoor) {
            isLoaded = m_ItemLoader.LoadAsync(sead::SafeStringBase<char>("Fg/Bcres/FgItemOutside.bcres"), heap, 128);
        } else {
            isLoaded = m_ItemLoader.LoadAsync(sead::SafeStringBase<char>("Fg/Bcres/FgItemInside.bcres"), heap, 128);
        }
        if (!isLoaded) {
            return false;
        }
        if (s_IsOutdoor) {
            m_ItemLoader.Read(0x10000, 0x10000, 0x10000);
        } else {
            m_ItemLoader.Read(0x30000, 0x30000, 0x20000);
        }
        m_ItemLoader.InitializeCgfx(&m_Allocator, NULL, true);
        m_ItemLoader.InitializeCgfx(&m_Allocator, GetFgLut(), true);
        if (stage::IsKind(stage::KIND_AUTO_CAMP, stage::GetCurrent())) {
            ReplaceRegionTexture("fg_soldout", s_SoldOutTextureNames[0], s_SoldOutTextureNames);
            for (u32 i = 0; i < FOG_MODEL_NUM; i++) {
                g3d::EnableFog(static_cast<const nw::gfx::res::ResModelData*>(
                    m_ItemLoader.m_GraphicsFile.ptr()->models.GetData(s_FogModelNames[i])));
            }
        } else if (!s_IsOutdoor) {
            m_LoadState = LOAD_STATE_ITEM_SETUP;
            return false;
        }
        m_LoadState = LOAD_STATE_ITEM_DONE;
        return true;
    }
    case LOAD_STATE_ITEM_SETUP:
        m_LoadState = LOAD_STATE_ITEM_FINISH;
        return false;
    case LOAD_STATE_ITEM_FINISH:
        if (!s_IsOutdoor) {
            Func179330(&m_ItemLoader, m_Allocator2.m_pHeap);
            ReplaceRegionTexture("fg_soldout", s_SoldOutTextureNames[0], s_SoldOutTextureNames);
            ReplaceRegionTexture("fg_grace_soldout", s_GraceSoldOutTextureNames[0], s_GraceSoldOutTextureNames);
            ReplaceRegionTexture("fg_wall_soldout", s_WallSoldOutTextureNames[0], s_WallSoldOutTextureNames);
            ReplaceRegionTexture("fg_wall_g_soldout", s_WallGraceSoldOutTextureNames[0], s_WallGraceSoldOutTextureNames);
        }
        m_LoadState = LOAD_STATE_ITEM_DONE;
        return true;
    default:
        return true;
    }
}

// 0x0059D5D0 (name is ours)
void fgobj::Proc::ReplaceRegionTexture(const char* modelName, const char* textureName, const char* const* names)
{
    const nw::gfx::res::ResModelData* model =
        static_cast<const nw::gfx::res::ResModelData*>(nw::gfx::GetModel(reinterpret_cast<const void* const*>(&m_ItemLoader.m_GraphicsFile), modelName));
    nw::gfx::res::ResMaterial material(model->materials.GetData(0));
    const char* name = names[GetGardenPlus()->data.townData.unk_0xEDAA & 0xF];
    void* texture = m_ItemLoader.m_GraphicsFile.ptr()->textures.GetData(name);
    if (texture == NULL) {
        return;
    }
    g3d::SetTexture(material, textureName, nw::gfx::res::ResTexture(texture));
}

namespace {
// all hashes of g3d::ResetRenderState
const s32 RESET_ALL_HASHES = 0x1FFF;
// the radius of a tree for the visibility test
const f32 TREE_VISIBLE_RADIUS = 8.0f;
// the draw mode of the top screen (ssys::ma::lyt::Layout)
const u32 DRAW_MODE_TOP_LEFT = 0x400;
} // namespace

// 0x0059BDAC (name is ours)
void fgobj::Proc::PlaceDeco(const SvFgName& item, s32 x, s32 z)
{
    if (!item.IsName(SvFgName::NAME_CEDAR_XMAS)) {
        return;
    }
    ObjectDeco* object = NewObject<ObjectDeco>(s_LargeHeaps, LARGE_HEAP_NUM);
    if (object == NULL) {
        return;
    }
    object->Setup(item, &x, &z, false);
    m_FieldObjects.InsertByPriority(object);
    object->m_Flags |= ObjectBase::FLAG_UNK_400 | ObjectBase::FLAG_UNK_2000;
    m_FieldObjectCounter++;
}

// 0x0059BEB8 (name is ours)
void fgobj::Proc::AddToDrawLists(ObjectBase* first)
{
    for (ObjectBase* object = first; object != NULL; object = static_cast<ObjectBase*>(object->m_pNext)) {
        if ((object->m_Flags & (ObjectBase::FLAG_UNK_4 | ObjectBase::FLAG_GROWN_TREE)) != 0 &&
            Func68F5A8(&object->m_UnitPosition, TREE_VISIBLE_RADIUS)) {
            continue;
        }
        if (!object->IsVisible()) {
            continue;
        }
        DrawList& list = (object->m_Flags & ObjectBase::FLAG_UNK_20000) ? m_DrawLists[1] : m_DrawLists[0];
        list.PushBack(&object->m_DrawNode);
        if ((object->m_Flags & ObjectBase::FLAG_BLOCK_HANDLE) && (object->m_Flags & ObjectBase::FLAG_BLOCK_UNK)) {
            continue;
        }
        if (!(object->m_Flags & ObjectBase::FLAG_HAS_SHADOW)) {
            continue;
        }
        if (!(object->m_Flags & ObjectBase::FLAG_SHADOW_SET)) {
            object->m_Shadow.Set(&object->m_DrawPosition, object->m_CurveAngle.value, true, object->m_ShadowKind, 0, object->m_ShadowSize,
                                 object->m_ShadowSize, 0.0f);
            object->m_Flags |= ObjectBase::FLAG_SHADOW_SET;
        }
        if (s_Unk94FE5D) {
            Func6EFBF0(&object->m_Shadow);
        }
    }
}

// 0x0059BFC8 (name is ours)
void fgobj::Proc::DrawOpaque(nw::gfx::MeshRenderer* renderer, void* proc)
{
    Proc* self = static_cast<Proc*>(proc);
    self->DrawObjects(renderer, &self->m_DrawLists[0]);
}

// 0x0059BFE0 (name is ours)
void fgobj::Proc::DrawObjects(nw::gfx::MeshRenderer* renderer, DrawList* list)
{
    s_LastModelKey = -1;
    s_pLastMesh = NULL;
    s_LastMaterialKey = -1;
    g3d::ResetRenderState(RESET_ALL_HASHES);
    const nn::math::MTX34 view = graphic::ScreenCamera::s_Instance.GetViewMatrix(DRAW_MODE_TOP_LEFT);
    ssys::st::ListNode* node = list->m_pFirst;
    while (node != NULL) {
        ObjectBase* object = static_cast<DrawListNode*>(node)->m_pObject;
        node = node->m_pNext;
        (object->*s_DrawMethods[object->m_Unk0x11D])(renderer, &view);
    }
    if (s_pLastMesh != NULL) {
        renderer->DeactivateVertexAttribute(nw::gfx::res::ResMesh(s_pLastMesh));
        s_pLastMesh = NULL;
    }
    list->Clear();
    s_LastModelKey = -1;
    s_LastMaterialKey = -1;
    s_pLastMesh = NULL;
    g3d::ResetRenderState(RESET_ALL_HASHES);
}

// 0x0059D6B4 (name is ours)
void fgobj::Proc::DrawTranslucent(nw::gfx::MeshRenderer* renderer, void* proc)
{
    Proc* self = static_cast<Proc*>(proc);
    self->DrawObjects(renderer, &self->m_DrawLists[1]);
}

// 0x0059DFC4 slot 0x30 | slot vf_0x30 of oml::framework::Process
oml::framework::Result fgobj::Proc::Draw()
{
    if (s_IsOutdoor) {
        AddToDrawListsOutdoor(static_cast<ObjectBase*>(m_FieldObjects.GetFirst()));
    } else {
        AddToDrawLists(static_cast<ObjectBase*>(m_FieldObjects.GetFirst()));
    }
    AddToDrawLists(static_cast<ObjectBase*>(m_ExeObjects.GetFirst()));
    m_Func.AddToScene(0);
    s_Unk94FE5D = 1;
    s_Unk94FE5C = 0;
    return oml::framework::RESULT_SUCCESS;
}

// 0x0059AF08 (name is ours)
void fgobj::Proc::CreateQueuedFieldObjects()
{
    if (s_IsIndoor) {
        while (m_QueuedUnitNum != 0) {
            m_QueuedUnitNum--;
            const u8 x = m_QueuedUnitX[m_QueuedUnitNum];
            const u8 z = m_QueuedUnitZ[m_QueuedUnitNum];
            if (m_FieldBit.IsSet(x, z)) {
                continue;
            }
            if (CreateFieldObject(x, z, true)) {
                continue;
            }
            CreateFieldObject(x, z, false);
        }
        Func150218(this);
        return;
    }
    while (m_QueuedUnitNum != 0) {
        m_QueuedUnitNum--;
        const u8 x = m_QueuedUnitX[m_QueuedUnitNum];
        const u8 z = m_QueuedUnitZ[m_QueuedUnitNum];
        if (m_FieldBit.IsSet(x, z)) {
            continue;
        }
        CreateFieldObject(x, z, false);
    }
    // the units of the area that are still missing, from the back row to the front
    const UnitArea& area = m_Areas[1];
    const s32 maxZ = area.maxZ > m_UnitMaxZ ? m_UnitMaxZ : area.maxZ;
    const s32 minZ = area.minZ < m_UnitMinZ ? m_UnitMinZ : area.minZ;
    const s32 maxX = area.maxX > m_UnitMaxX ? m_UnitMaxX : area.maxX;
    const s32 minX = area.minX < m_UnitMinX ? m_UnitMinX : area.minX;
    for (s32 z = maxZ; z >= minZ; z--) {
        for (s32 x = minX; x <= maxX; x++) {
            if (m_FieldBit.IsSet(x, z)) {
                continue;
            }
            CreateFieldObject(x, z, false);
            if (m_FieldObjectCounter >= 10) {
                return;
            }
        }
    }
}

// 0x0059D99C (name is ours)
void fgobj::Proc::SetTreeTextureAddress(u32 index, const char* name)
{
    const void* data = m_FgLoader.m_GraphicsFile.ptr()->textures.GetData(name);
    const nw::gfx::res::ResImageTextureData* texture =
        nw::gfx::res::DynamicCastData<nw::gfx::res::ResImageTextureData>(data, nw::gfx::res::ResTexture::TYPE_IMAGE);
    const nw::gfx::res::ResImageData* image = texture->image.To<nw::gfx::res::ResImageData>();
    const void* address = image->pLocationAddress != NULL ? image->pLocationAddress : image->data.ToPtr();
    s_TreeTextureAddresses[index] = nngxGetPhysicalAddr(reinterpret_cast<uptr>(address));
}

extern const f32 s_PalmFruitAngles[2];
extern const f32 s_BananaFruitAngles[2];
extern const nn::math::VEC3_ s_FruitOffsets[3];

namespace {
// the fruits are a little further in front on the island
const f32 ISLAND_FRUIT_OFFSET_Z = 2.0f;
// the flags of a fruit among the field objects
const u32 FRUIT_FLAGS = ObjectBase::FLAG_UNK_4 | ObjectBase::FLAG_UNK_200 | ObjectBase::FLAG_UNK_400;

// a fruit object goes to the field objects
inline void AddFruit(Proc* proc, ObjectBase* object)
{
    proc->m_FieldObjects.InsertByPriority(object);
    object->m_Flags = (object->m_Flags & ~ObjectBase::FLAG_HAS_SHADOW) | FRUIT_FLAGS;
    proc->m_FieldObjectCounter++;
}
} // namespace

// 0x0059D4AC (name is ours)
void fgobj::Proc::PlaceFruits(const SvFgName* tree, const SvFgName* fruit, s32 x, s32 z)
{
    SvFgName item;
    for (u32 i = 0; i < 3; i++) {
        item = *fruit;
        if (tree->HasPerfectFruitAt(i)) {
            item.ToPerfectFruit();
        }
        nn::math::VEC3 offset;
        offset.x = s_FruitOffsets[i].x;
        offset.y = s_FruitOffsets[i].y;
        offset.z = s_FruitOffsets[i].z;
        if (static_cast<s32>(i) < 2 && stage::IsOnIsland()) {
            offset.z += ISLAND_FRUIT_OFFSET_Z;
        }
        ObjectFruit* object = ObjectFruit::Create(item, x, z, &offset, 0.0f, i, false);
        if (object == NULL) {
            return;
        }
        AddFruit(this, object);
    }
}

// 0x0059DA74 (name is ours)
void fgobj::Proc::PlacePalmFruits(const SvFgName* tree, const SvFgName* fruit, s32 x, s32 z)
{
    // 0x0098B428 (guard 0x0094FE84)
    static const nn::math::VEC3 s_PalmFruitOffsets[2] = {nn::math::VEC3(9.5f, 34.0f, 6.0f), nn::math::VEC3(-6.0f, 32.0f, 8.0f)};
    // 0x0098B440 (guard 0x0094FE80)
    static const nn::math::VEC3 s_BananaFruitOffsets[2] = {nn::math::VEC3(8.0f, 36.0f, 6.5f), nn::math::VEC3(-6.0f, 32.5f, 6.0f)};
    const nn::math::VEC3* offsets;
    const f32* angles;
    if (fruit->IsName(SvFgName::NAME_BANANA)) {
        offsets = s_BananaFruitOffsets;
        angles = s_BananaFruitAngles;
    } else {
        offsets = s_PalmFruitOffsets;
        angles = s_PalmFruitAngles;
    }
    for (u32 i = 0; i < 2; i++) {
        ObjectFruit* object = ObjectFruit::Create(*fruit, x, z, &offsets[i], angles[i], i, i == 0);
        if (object == NULL) {
            return;
        }
        AddFruit(this, object);
    }
}

namespace {
// the frames between the effects of two watered flowers
const u8 WATER_EFFECT_DELAY_STEP = 5;
// the height that means: no height test
const f32 WATER_EFFECT_NO_HEIGHT = -1000.0f;
} // namespace

// 0x0059DC4C (name is ours)
void StartWaterEffects(s32 x, s32 z, f32 height, s32 area, bool isMine)
{
    if (s_pProc == NULL || !FdInfo::GetCurrentField()->IsInside(x, z)) {
        return;
    }
    u16 skipMask = 0;
    if (height != WATER_EFFECT_NO_HEIGHT) {
        skipMask = GetHeightDifferenceMask(x, z, stage::GetCurrent(), area, height);
    }
    s32 num = 1;
    if (area == WATER_AREA_SQUARE) {
        num = 9;
    } else if (area == WATER_AREA_CROSS) {
        num = 5;
    }
    u8 delay = 0;
    for (s32 i = 0; i < num; i++) {
        if (skipMask & (1 << i)) {
            continue;
        }
        ObjectBase* object = s_pProc->FindFieldObject(x + s_WaterEffectUnitOffsets[0][i], z + s_WaterEffectUnitOffsets[1][i]);
        if (object == NULL || !object->m_Item.IsFlowerOrWilted()) {
            continue;
        }
        if (isMine) {
            Func6F0C80(9, 1);
        }
        if (object->m_Flags & ObjectBase::FLAG_EFFECT) {
            continue;
        }
        if (netgame::IsMultiplePlayer() && area != WATER_AREA_UNIT) {
            object->m_Flags |= ObjectBase::FLAG_UNK_80;
        } else {
            object->m_Flags &= ~ObjectBase::FLAG_UNK_80;
        }
        if (delay != 0) {
            object->m_Unk0x11A = delay;
            object->m_Flags |= ObjectBase::FLAG_EFFECT_DELAYED;
        } else {
            object->StartEffect();
            object->m_Unk0x11A = 0;
            object->m_Flags &= ~ObjectBase::FLAG_EFFECT_DELAYED;
        }
        delay += WATER_EFFECT_DELAY_STEP;
    }
}

namespace {
// the object goes to its draw list and its shadow is set
inline void AddToDrawList(Proc* proc, ObjectBase* object)
{
    DrawList& list = (object->m_Flags & ObjectBase::FLAG_UNK_20000) ? proc->m_DrawLists[1] : proc->m_DrawLists[0];
    list.PushBack(&object->m_DrawNode);
    if ((object->m_Flags & ObjectBase::FLAG_BLOCK_HANDLE) && (object->m_Flags & ObjectBase::FLAG_BLOCK_UNK)) {
        return;
    }
    if (!(object->m_Flags & ObjectBase::FLAG_HAS_SHADOW)) {
        return;
    }
    if (!(object->m_Flags & ObjectBase::FLAG_SHADOW_SET)) {
        object->m_Shadow.Set(&object->m_DrawPosition, object->m_CurveAngle.value, true, object->m_ShadowKind, 0, object->m_ShadowSize,
                             object->m_ShadowSize, 0.0f);
        object->m_Flags |= ObjectBase::FLAG_SHADOW_SET;
    }
    if (s_Unk94FE5D) {
        Func6EFBF0(&object->m_Shadow);
    }
}
} // namespace

// 0x0059DE40 (name is ours)
void fgobj::Proc::AddToDrawListsOutdoor(ObjectBase* first)
{
    for (ObjectBase* object = first; object != NULL; object = static_cast<ObjectBase*>(object->m_pNext)) {
        if ((object->m_Flags & (ObjectBase::FLAG_UNK_4 | ObjectBase::FLAG_GROWN_TREE)) != 0 &&
            Func68F5A8(&object->m_UnitPosition, TREE_VISIBLE_RADIUS)) {
            continue;
        }
        // the objects in the inner area are always drawn
        if (!m_Areas[2].Contains(object->m_X, object->m_Z) && !object->IsVisible()) {
            continue;
        }
        AddToDrawList(this, object);
    }
}

// 0x005A15A0 (name is ours)
void FreeUnitLocksOfType23Or24()
{
    for (s32 i = 0; i < static_cast<s32>(UNIT_LOCK_NUM); i++) {
        UnitLock& lock = s_UnitLocks[i];
        if (lock.stage == stage::NAME_NONE) {
            continue;
        }
        if (lock.type == PLACE_TYPE_UNKNOWN_23) {
            lock.stage = stage::NAME_NONE;
        } else if (lock.type == PLACE_TYPE_UNKNOWN_24) {
            Func168E20(lock.pos.x, lock.pos.y, 0, lock.stage, SvFgName::s_Empty);
            lock.stage = stage::NAME_NONE;
        }
    }
}

namespace {
// the frames of the sway of a field object at most
const u8 SWAY_FRAMES_MAX = 10;
} // namespace

// 0x005A1610 (name is ours)
void fgobj::FlowerGlitter::Update(ObjectList* objects)
{
    m_Num = 0;
    m_NearNum = 0;
    long playerX;
    long playerZ;
    const bool hasPlayer = PlayerGetPosition(playerX, playerZ, netgame::PLAYER_NO_MY_NO, true);
    s32 num = 0;
    const s32 minX = playerX - 1;
    const s32 maxX = playerX + 1;
    const s32 minZ = playerZ - 1;
    const s32 maxZ = playerZ + 1;
    for (ObjectBase* object = static_cast<ObjectBase*>(objects->GetFirst()); object != NULL;
         object = static_cast<ObjectBase*>(object->m_pNext)) {
        if (object->m_Flags & ObjectBase::FLAG_UNK_2000) {
            static_cast<ObjectExe*>(object)->Execute();
        }
        if ((object->m_Flags & ObjectBase::FLAG_UNK_8) && object->m_Unk0x11C != 0) {
            if (object->m_Unk0x11C > SWAY_FRAMES_MAX) {
                object->m_Unk0x11C = SWAY_FRAMES_MAX;
            }
            const u8 frame = object->m_Unk0x11C;
            object->m_Unk0x11C = frame - 1;
            const nn::math::VEC3 scale(s_SwayScales[SWAY_FRAMES_MAX - frame][0], s_SwayScales[SWAY_FRAMES_MAX - frame][1], 1.0f);
            object->m_Model.SetScale(scale);
            object->m_Model.g3d::SkeletalModel::Update();
        }
        if (object->m_Flags & ObjectBase::FLAG_EFFECT_DELAYED) {
            if (object->m_Unk0x11A == 0 || --object->m_Unk0x11A == 0) {
                object->m_Flags &= ~ObjectBase::FLAG_EFFECT_DELAYED;
                object->StartEffect();
            }
        }
        if (!(object->m_Flags & ObjectBase::FLAG_UNK_10)) {
            continue;
        }
        if (object->m_Unk0x11B != 0) {
            if (--object->m_Unk0x11B == 0) {
                object->m_Flags &= ~ObjectBase::FLAG_EFFECT;
            }
            continue;
        }
        num++;
        if (hasPlayer && object->m_X >= minX && maxX >= object->m_X && minZ <= object->m_Z && object->m_Z <= maxZ) {
            object->m_Flags &= ~ObjectBase::FLAG_GLITTERED;
            m_NearNum++;
        }
        if (!(object->m_Flags & ObjectBase::FLAG_GLITTERED)) {
            m_Num++;
        }
    }
    // all flowers have glittered: they may again
    if (m_Num == 0 && num != 0) {
        m_Num = num;
        for (ObjectBase* object = static_cast<ObjectBase*>(objects->GetFirst()); object != NULL;
             object = static_cast<ObjectBase*>(object->m_pNext)) {
            object->m_Flags &= ~ObjectBase::FLAG_GLITTERED;
        }
    }
}

namespace {
// the effect of the glitter
const u32 GLITTER_EFFECT_ID = 250;
} // namespace

// 0x005A1824 (name is ours)
void fgobj::FlowerGlitter::Start(ObjectList* objects, s32 timer, bool nearPlayer)
{
    m_Delay = DELAY_FRAMES;
    m_Timers[timer] = TIMER_FRAMES;
    const u32 target = GetRandom(nearPlayer ? m_NearNum : m_Num);
    ObjectBase* object = static_cast<ObjectBase*>(objects->GetFirst());
    u32 index = 0;
    long playerX;
    long playerZ;
    if (!PlayerGetPosition(playerX, playerZ, netgame::PLAYER_NO_MY_NO, true)) {
        nearPlayer = false;
        playerX = 0;
        playerZ = 0;
    }
    const s32 minX = playerX - 1;
    const s32 minZ = playerZ - 1;
    const s32 maxX = playerX + 1;
    const s32 maxZ = playerZ + 1;
    for (; object != NULL; object = static_cast<ObjectBase*>(object->m_pNext)) {
        if (!(object->m_Flags & ObjectBase::FLAG_UNK_10) || (object->m_Flags & ObjectBase::FLAG_GLITTERED) || object->m_Unk0x11B != 0) {
            continue;
        }
        if (nearPlayer && !(object->m_X >= minX && maxX >= object->m_X && minZ <= object->m_Z && object->m_Z <= maxZ)) {
            continue;
        }
        if (index == target) {
            break;
        }
        index++;
    }
    if (object == NULL) {
        return;
    }
    if (m_Handle.GetEmitterSet() == NULL) {
        Func207998(&m_Handle, GLITTER_EFFECT_ID, &VEC3_ZERO, &ANGLE16_VEC3_ZERO, &VEC3_ONE, true, 0);
        m_Handle.Func2E6848(1);
    }
    const nn::math::VEC3 position = object->m_Position;
    nn::math::MTX34 matrix;
    MakeWorldCurveMatrix(&matrix, &position, math::Angle16());
    m_Handle.SetMatrix(&matrix);
    m_Handle.Func2E64A4(&VEC3_ZERO);
    object->m_Unk0x11B = FLOWER_GLITTER_FRAMES;
    object->m_Flags |= ObjectBase::FLAG_GLITTERED;
}

// 0x005A3238 (name is ours)
void InitializeMyDesignTextures(MyDesignTextures* textures)
{
    if (textures->m_Allocator.m_pHeap == NULL) {
        return;
    }
    for (u32 i = 0; i < MyDesignTextures::PLAYER_NUM * MyDesignTextures::DESIGN_NUM; i++) {
        const void* data = textures->m_Loaders[i].m_GraphicsFile.ptr()->textures.GetData(0);
        const nw::gfx::res::ResImageTextureData* texture =
            nw::gfx::res::DynamicCastData<nw::gfx::res::ResImageTextureData>(data, nw::gfx::res::ResTexture::TYPE_IMAGE);
        const nw::gfx::res::ResImageData* image = texture->image.To<nw::gfx::res::ResImageData>();
        textures->m_Addresses[i] = nngxGetPhysicalAddr(reinterpret_cast<uptr>(image->data.ToPtr()));
    }
}

// 0x005A32EC (name is ours)
const void* GetMyDesignTexture(u32 player, u32 design)
{
    if (s_pMyDesignTextures == NULL) {
        return NULL;
    }
    return s_pMyDesignTextures->m_Loaders[player * MyDesignTextures::DESIGN_NUM + design].m_GraphicsFile.ptr()->textures.GetData(0);
}

// 0x0059E5A0 (name is ours)
bool fgobj::Proc::CreateFieldObject(s32 x, s32 z, bool isUpper)
{
    if (s_IsOutdoor && !m_FieldBit2.IsSet(x, z) && !m_Areas[1].Contains(x, z)) {
        return false;
    }
    Field* field = FdInfo::GetCurrentField();
    SvFgName item;
    const SvFgName* fg = field->GetFg(x, z, isUpper ? 1 : 0);
    if (fg == NULL) {
        if (!isUpper) {
            m_FieldBit.Set(x, z);
        }
        return false;
    }
    if (!fg->IsFg()) {
        if (fg->IsUsual()) {
            // indoors the items of Func30B658 are only shown in the police station
            if (s_IsIndoor && Func30B658(fg) && !stage::IsPolice(stage::GetCurrent())) {
                if (!isUpper) {
                    m_FieldBit.Set(x, z);
                }
                return false;
            }
            if (stage::IsKind(stage::KIND_AUTO_CAMP, stage::GetCurrent()) && Func30B658(fg)) {
                m_FieldBit.Set(x, z);
                return false;
            }
        } else if (!Func769190(fg)) {
            if (!isUpper) {
                m_FieldBit.Set(x, z);
            }
            return false;
        }
    }
    item = *fg;
    if (s_IsOutdoor) {
        // the units of the trees and bushes are only created in the inner area
        if (!m_FieldBit2.IsSet(x, z)) {
            const u8 fgKind = item.GetFgKind();
            if (!SvFgName::IsGrownTreeFgKind(fgKind) && !SvFgName::IsBushFgKind(fgKind)) {
                m_FieldBit2.Set(x, z);
            }
        }
        if (m_FieldBit2.IsSet(x, z) && !m_Areas[0].Contains(x, z)) {
            return false;
        }
    }
    ObjectBase* object = NULL;
    for (u32 i = 0; i < SMALL_HEAP_NUM; i++) {
        sead::Heap* heap = s_SmallHeaps[i];
        if (heap != NULL) {
            s_SmallHeaps[i] = NULL;
            object = new (heap, 4) ObjectBase();
            object->m_Allocator.SetHeap(heap);
            object->Setup(item, x, z, isUpper);
            object->InitializeShadow();
            break;
        }
    }
    if (object == NULL) {
        return false;
    }
    if (s_IsOutdoor && item.IsName(SvFgName::NAME_MAGIC_LAMP)) {
        s_pMagicLampObject = object;
    }
    m_FieldObjects.InsertByPriority(object);
    m_FieldBit.Set(x, z);
    if (s_IsOutdoor) {
        if (item.IsFg() && item.IsFruitRipe()) {
            SvFgName fruit;
            if (item.GetFruit(&fruit)) {
                // the fruits are made unless they are there already
                bool hasFruits = false;
                for (ObjectBase* other = static_cast<ObjectBase*>(m_FieldObjects.GetFirst()); other != NULL;
                     other = static_cast<ObjectBase*>(other->m_pNext)) {
                    if (other->m_X == x && other->m_Z == z && (other->m_Flags & ObjectBase::FLAG_UNK_4) && other->GetFallDirection() == 0) {
                        hasFruits = true;
                        break;
                    }
                }
                if (!hasFruits) {
                    if (item.GetFgKind() == SvFgName::FG_KIND_PALM_TREE) {
                        PlacePalmFruits(&item, &fruit, x, z);
                    } else {
                        PlaceFruits(&item, &fruit, x, z);
                    }
                }
            }
        }
        PlaceDeco(item, x, z);
    } else if (s_IsIndoor) {
        const s32 handle = Func150230(object);
        object->UpdateBlockHandle(isUpper);
        object->m_Unk0x114 = handle;
        object->m_Flags |= ObjectBase::FLAG_FTR;
    }
    m_FieldObjectCounter++;
    return true;
}

// 0x0059E4F0 (name is ours)
void ClearUnit(s32 x, s32 z)
{
    if (s_pProc == NULL) {
        return;
    }
    s_pProc->RemoveObjectsAt(x, z, Proc::REMOVE_MODE_ALL);
    Proc* proc = s_pProc;
    ObjectBase* object = static_cast<ObjectBase*>(proc->m_ExeObjects.GetFirst());
    while (object != NULL) {
        ObjectBase* next = static_cast<ObjectBase*>(object->m_pNext);
        if (object->m_X == x && object->m_Z == z) {
            proc->RemoveObject(object, false, true);
        }
        object = next;
    }
    s_pProc->m_FieldBit.Set(x, z);
}

// 0x0059E03C slot 0x0C | slot vf_0x0C of oml::framework::Process
oml::framework::Result fgobj::Proc::Initialize()
{
    s_pProc = NULL;
    s_IsIndoor = stage::IsKind(stage::KIND_INDOOR, stage::GetCurrent());
    s_IsOutdoor = stage::IsKind(stage::KIND_OUTDOOR, stage::GetCurrent());
    s_pMagicLampObject = NULL;
    s_Unk94FE64 = 0;
    s_Unk94FE78 = 0.0f;
    s_Unk94FE60 = 0;
    s_Unk94FE7C = 0.0f;
    s_Unk94FE61 = 0;
    s_Unk94FE62 = 0;
    CreateResourceHeaps();
    if (!LoadSeasonResources()) {
        return oml::framework::RESULT_RETRY;
    }
    if (!LoadItemResources()) {
        return oml::framework::RESULT_RETRY;
    }
    // the animations of the field objects
    switch (m_LoadState) {
    case LOAD_STATE_ITEM_DONE:
        if (s_IsOutdoor) {
            if (!m_AnimLoader.LoadAsync(sead::SafeStringBase<char>("Fg/Bcres/FgObjAnim.bcres"), m_Allocator2.m_pHeap, 128)) {
                return oml::framework::RESULT_RETRY;
            }
            m_AnimLoader.Read(0x30000, 0x30000, 0x20000);
            m_AnimLoader.InitializeCgfx(&m_Allocator, GetFgLut(), true);
            m_AnimLoader.InitializeCgfx(&m_Allocator, &m_FgLoader, true);
            m_AnimLoader.InitializeCgfx(&m_Allocator, NULL, true);
            m_LoadState = LOAD_STATE_ANIM_SETUP;
        }
        break;
    case LOAD_STATE_ANIM_SETUP:
        m_LoadState = LOAD_STATE_ANIM_FINISH;
        return oml::framework::RESULT_RETRY;
    case LOAD_STATE_ANIM_FINISH:
        Func179330(&m_AnimLoader, m_Allocator2.m_pHeap);
        m_LoadState = LOAD_STATE_ANIM_DONE;
        break;
    default:
        break;
    }
    // the textures of the designs of all players
    MyDesignTextures& textures = m_MyDesignTextures;
    if (textures.m_Allocator.m_pHeap != NULL) {
        sead::Heap* heap = textures.m_Allocator.m_pHeap;
        bool isLoaded = true;
        SvFgName pattern(static_cast<SvFgName::ID>(SvFgName::NAME_CUSTOM_SHIRT));
        u32 index = 0;
        for (u32 player = 0; player < MyDesignTextures::PLAYER_NUM; player++) {
            for (u32 design = 0; design < MyDesignTextures::DESIGN_NUM; design++) {
                Func304EA0(&pattern, 0, player, design);
                isLoaded &= textures.m_Loaders[index].Func536B74(&pattern, heap, 4, 1, 0, 1, 0);
                index++;
            }
        }
        if (!isLoaded) {
            return oml::framework::RESULT_RETRY;
        }
        s_pMyDesignTextures = &textures;
    }
    CreateHeaps();
    s_pProc = this;
    s_Unk94FE5D = 0;
    s_Unk94FE5C = 0;
    s_pParticleHandles = m_ParticleHandles;
    m_QueuedUnitNum = 0;
    std::memset(m_QueuedUnitX, 0, sizeof(m_QueuedUnitX));
    std::memset(m_QueuedUnitZ, 0, sizeof(m_QueuedUnitZ));
    for (u32 i = 0; i < 100; i++) {
        m_Unknown0x321C[i] = 0;
    }
    for (u32 i = 0; i < 100; i++) {
        m_Unknown0x33AC[i] = 0;
    }
    m_ExeObjects.Clear();
    m_FieldObjects.Clear();
    m_DrawLists[0].Clear();
    m_DrawLists[1].Clear();
    m_FieldBit.Clear();
    m_FieldBit3.Clear();
    m_FieldBit2.Clear();
    m_Glitter.m_NearNum = 0;
    m_Glitter.m_Num = 0;
    std::memset(m_Glitter.m_Timers, FlowerGlitter::TIMER_FRAMES, sizeof(m_Glitter.m_Timers));
    m_Glitter.m_Delay = 0;
    InitializeMyDesignTextures(&m_MyDesignTextures);
    Field* field = FdInfo::GetCurrentField();
    m_UnitMinX = 0;
    m_UnitMaxX = field->GetBlockWidth() << 4;
    m_UnitMinZ = 0;
    m_UnitMaxZ = field->GetBlockHeight() << 4;
    m_FieldObjectCounter = -1000;
    m_RemovedObjectCounter = 0;
    ResetPlaceRequests();
    if (s_IsOutdoor) {
        for (u32 i = 0; i < 2; i++) {
            const void* model = s_pProc != NULL ? nw::gfx::GetModel(reinterpret_cast<const void* const*>(&s_pProc->m_ItemLoader.m_GraphicsFile),
                                                                    s_SettingModelNames[i])
                                                : NULL;
            if (model == NULL) {
                break;
            }
            g3d::Model& object = m_SettingModelObjects[i];
            object.Create(nw::gfx::res::ResSceneObject(model), &m_Allocator, &m_Allocator, 0, true, 1);
            m_pSettingModels[i] = object.GetModel();
        }
    } else {
        m_pSettingModels[0] = NULL;
        m_pSettingModels[1] = NULL;
    }
    m_Func.Initialize(&m_Allocator);
    m_Func.SetFunctions(DrawOpaque, DrawTranslucent, this);
    return oml::framework::RESULT_SUCCESS;
}

// 0x0059E588 (name is ours)
bool fgobj::Proc::Exists()
{
    return s_pProc != NULL;
}

// 0x0059F240 (name is ours)
void fgobj::Proc::RemoveObject(ObjectBase* object, bool isFieldObject, bool removeFromList)
{
    m_FieldBit.Reset(object->m_X, object->m_Z);
    if (removeFromList) {
        (isFieldObject ? m_FieldObjects : m_ExeObjects).Erase(object);
    }
    if (m_DrawLists[0].Contains(&object->m_DrawNode)) {
        m_DrawLists[0].Erase(&object->m_DrawNode);
    }
    if (m_DrawLists[1].Contains(&object->m_DrawNode)) {
        m_DrawLists[1].Erase(&object->m_DrawNode);
    }
    object->Finalize();
}

// 0x005A3158 (name is ours)
void fgobj::Proc::RemoveObjectsAt(s32 x, s32 z, u32 mode)
{
    ObjectBase* object = static_cast<ObjectBase*>(m_FieldObjects.GetFirst());
    while (object != NULL) {
        ObjectBase* next = static_cast<ObjectBase*>(object->m_pNext);
        if (object->m_X == x && object->m_Z == z) {
            if (mode == REMOVE_MODE_ALL) {
                RemoveObject(object, true, true);
            } else if (object->m_Flags & ObjectBase::FLAG_UNK_4) {
                if (mode == REMOVE_MODE_FLAG_4) {
                    RemoveObject(object, true, true);
                }
            } else if (mode == REMOVE_MODE_NO_FLAG_4) {
                RemoveObject(object, true, true);
            }
        }
        object = next;
    }
    m_FieldBit.Reset(x, z);
}

// 0x0059EF40 slot 0x18 | slot vf_0x18 of oml::framework::Process
oml::framework::Result fgobj::Proc::Finalize()
{
    FinishCoinStone();
    FinishPlaceActions();
    FreeUnitLocksOfType23Or24();
    m_SettingModelObjects[0].g3d::Model::Finalize();
    m_SettingModelObjects[1].g3d::Model::Finalize();
    m_Func.g3d::TransformNode::Finalize();
    ObjectBase* object = static_cast<ObjectBase*>(m_FieldObjects.GetFirst());
    while (object != NULL) {
        ObjectBase* next = static_cast<ObjectBase*>(object->m_pNext);
        RemoveObject(object, true, true);
        object = next;
    }
    object = static_cast<ObjectBase*>(m_ExeObjects.GetFirst());
    while (object != NULL) {
        ObjectBase* next = static_cast<ObjectBase*>(object->m_pNext);
        RemoveObject(object, false, true);
        object = next;
    }
    ObjectBase** objects = reinterpret_cast<ObjectBase**>(m_Unknown0x321C);
    for (u32 n = 0; n < OBJECT_ARRAY_NUM; n++) {
        u32 i;
        for (i = 0; i < OBJECT_ARRAY_NUM; i++) {
            if (objects[i] != NULL) {
                break;
            }
        }
        if (i == OBJECT_ARRAY_NUM) {
            break;
        }
        ObjectBase* taken = objects[i];
        objects[i] = NULL;
        RemoveObject(taken, false, false);
    }
    s_pProc = NULL;
    s_pMagicLampObject = NULL;
    s_Unk94FE64 = 0;
    s_Unk94FE78 = 0.0f;
    s_Unk94FE60 = 0;
    s_Unk94FE7C = 0.0f;
    s_Unk94FE61 = 0;
    s_Unk94FE62 = 0;
    for (u32 i = 0; i < MyDesignTextures::PLAYER_NUM * MyDesignTextures::DESIGN_NUM; i++) {
        m_MyDesignTextures.m_Loaders[i].Release();
    }
    m_MyDesignTextures.m_Allocator.DestroyHeap();
    s_pMyDesignTextures = NULL;
    s_pParticleHandles = NULL;
    m_FgModelLoad.Reset();
    m_AnimLoader.Unload();
    m_ItemLoader.Unload();
    s_TreeTextureAddresses[0] = 0;
    s_TreeTextureAddresses[1] = 0;
    m_FgLoader.Unload();
    for (u32 i = 0; i < SMALL_HEAP_NUM; i++) {
        if (s_SmallHeaps[i] != NULL) {
            s_SmallHeaps[i]->destroy();
            s_SmallHeaps[i] = NULL;
        }
    }
    m_Allocator3.DestroyHeap();
    m_Allocator2.DestroyHeap();
    s_IsIndoor = false;
    s_IsOutdoor = false;
    return oml::framework::RESULT_SUCCESS;
}

// 0x0059EAA4 slot 0x24 | slot vf_0x24 of oml::framework::Process
oml::framework::Result fgobj::Proc::Calc()
{
    g_UnitBitsAAF736.Clear();
    m_DrawLists[0].Clear();
    m_DrawLists[1].Clear();
    UpdatePlaceActions();
    UpdateCoinStone();
    if (!UpdateAreas()) {
        return oml::framework::RESULT_SUCCESS;
    }
    // the running objects: finished ones leave the list and their unit gets its field objects again
    ObjectExe* object = static_cast<ObjectExe*>(m_ExeObjects.GetFirst());
    while (object != NULL) {
        ObjectExe* next = static_cast<ObjectExe*>(object->m_pNext);
        const s32 result = object->Execute();
        SoundMgr::SSetSharedSoundPosition(&object->m_Position, object);
        if (result != ObjectExe::EXECUTE_CONTINUE &&
            (result == ObjectExe::EXECUTE_FINISHED || result == ObjectExe::EXECUTE_FINISHED_KEEP_REQUEST)) {
            if (FdInfo::GetCurrentField()->IsInside(object->m_X, object->m_Z)) {
                m_FieldBit.Reset(object->m_X, object->m_Z);
                if (result == ObjectExe::EXECUTE_FINISHED) {
                    FinishPlaceRequest(object->m_X, object->m_Z, stage::GetCurrent());
                }
                const s32 x = object->m_X;
                const s32 z = object->m_Z;
                if (static_cast<u32>(x) < 256 && z >= 0 && z <= 255 && m_QueuedUnitNum < QUEUED_UNIT_NUM) {
                    m_QueuedUnitX[m_QueuedUnitNum] = x;
                    m_QueuedUnitZ[m_QueuedUnitNum] = z;
                    m_QueuedUnitNum++;
                }
            }
            m_ExeObjects.Erase(object);
            object->Finalize();
        }
        object = next;
    }
    if (s_IsOutdoor) {
        // the field objects that left their area (at most 20 per frame)
        ObjectBase* field = static_cast<ObjectBase*>(m_FieldObjects.GetFirst());
        while (field != NULL) {
            ObjectBase* next = static_cast<ObjectBase*>(field->m_pNext);
            bool isOutside;
            if (field->m_Flags & ObjectBase::FLAG_UNK_200) {
                isOutside = !m_Areas[1].Contains(field->m_X, field->m_Z);
            } else {
                isOutside = !m_Areas[0].Contains(field->m_X, field->m_Z);
            }
            if (isOutside) {
                RemoveObject(field, true, true);
                if (++m_RemovedObjectCounter >= 20) {
                    break;
                }
            }
            field = next;
        }
    } else {
        for (ObjectBase* field = static_cast<ObjectBase*>(m_FieldObjects.GetFirst()); field != NULL;
             field = static_cast<ObjectBase*>(field->m_pNext)) {
            if (!(field->m_Flags & ObjectBase::FLAG_MODULE_SCALE)) {
                continue;
            }
            if (g_pUnk7B2C88 == NULL) {
                field->m_Flags &= ~ObjectBase::FLAG_MODULE_SCALE;
                continue;
            }
            f32 scale = 1.0f;
            if (!Func150238(g_pUnk7B2C88, &scale, field->m_Unk0x110)) {
                field->m_Flags &= ~ObjectBase::FLAG_MODULE_SCALE;
            }
            const nn::math::VEC3 scales(scale, scale, scale);
            field->m_Model.SetScale(scales);
            field->UpdateMatrix(false, 0.0f);
        }
    }
    CreateQueuedFieldObjects();
    if (s_IsOutdoor) {
        // the rows near the camera are curved from the camera
        const s32 limit = m_CameraUnitZ - 8;
        for (ObjectBase* field = static_cast<ObjectBase*>(m_FieldObjects.GetFirst()); field != NULL;
             field = static_cast<ObjectBase*>(field->m_pNext)) {
            const u32 flags = field->m_Flags;
            if (!(flags & ObjectBase::FLAG_UNK_400)) {
                continue;
            }
            if (field->m_Z <= limit) {
                field->UpdateMatrix(true, 0.0f);
                field->m_Flags |= ObjectBase::FLAG_CURVED;
            } else if (flags & ObjectBase::FLAG_CURVED) {
                field->UpdateMatrix(true, 0.0f);
                field->m_Flags &= ~ObjectBase::FLAG_CURVED;
            }
        }
    }
    if (s_IsOutdoor) {
        // the glitter of the watered flowers
        m_Glitter.Update(&m_FieldObjects);
        s32 free = -1;
        for (u32 i = 0; i < FlowerGlitter::TIMER_NUM; i++) {
            if (m_Glitter.m_Timers[i] != 0) {
                m_Glitter.m_Timers[i]--;
            } else {
                free = i;
            }
        }
        if (m_Glitter.m_Delay != 0) {
            m_Glitter.m_Delay--;
            free = -1;
        }
        if (m_Glitter.m_Num != 0 && free != -1) {
            if ((free & 1) && m_Glitter.m_NearNum != 0) {
                m_Glitter.Start(&m_FieldObjects, free, true);
            } else {
                m_Glitter.Start(&m_FieldObjects, free, false);
            }
        }
    }
    ObjectBase* wobbling = s_pMagicLampObject;
    if (wobbling != NULL &&
        UpdateWobble(reinterpret_cast<s16*>(&wobbling->m_CurveAngle), &wobbling->m_RotateY, &wobbling->m_RotateZ)) {
        wobbling->UpdateMatrix(false, s_Unk94FE7C);
    }
    m_FieldObjectCounter = 0;
    m_RemovedObjectCounter = 0;
    return oml::framework::RESULT_SUCCESS;
}

} // namespace fgobj

// the factory of the process class
// 0x007F3C50
template oml::framework::Process* oml::framework::Process::Create<fgobj::Proc>();
