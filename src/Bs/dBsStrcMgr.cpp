#include "Bs/dBsStrcMgr.h"

#include "Ac/dAcStrc.h"
#include "Bs/dBsNpcMgr.h"
#include "Bs/dBsNpcMgrExtern.h"
#include "Bs/dBsNpcMgr_NpcPosGetCB.h"
#include "Bs/dBsStrcMgrExtern.h"
#include "Other/dHeapInit.h"
#include "g3d/dTransformNode.h"
#include "mutil/dResourceLoaderMgr.h"
#include "net/dNet.h"
#include "netgame/netgame_Api.h"
#include "sead/seadExpHeap.h"
#include "stage/stage_Api.h"
#include "time/time_Api.h"
#include "stage/stage_Extern.h"
#include "Sv/dSvApi.h"
#include "camera/dCameraMgr.h"
#include "field/field_Extern.h"
#include "field/dFdInfo.h"
#include "field/dFieldAttribute.h"
#include "netgame/dGameFunctions.h"
#include "Other/dProcMgr.h"
#include "Sv/dSvGardenPlus.h"
#include "field/dFieldRect.h"
#include "Bs/dBsFtrMgrExtern.h"
#include "sead/seadHeapMgr.h"
#include "Sound/dSoundLabel.h"
#include "Sv/dSvNpc.h"
#include "event/event_Extern.h"
#include "Sv/dSvProcExtern.h"
#include <cmath>
#include <cstring>

// (the globals of the file)
// 0x009516C0
u8 BsStrcMgr::s_InitState;
// 0x009516C4
BsStrcMgr* BsStrcMgr::s_pInstance;

namespace {
// the files of the common resources (the order of BsStrcMgr::CommonRes)
const char* const COMMON_RES_FILES[BsStrcMgr::COMMON_RES_NUM] = {
    "Strc/hobj/lut/Lut_house.bcres",
    "Strc/sobj/lut/Lut_sobj.bcres",
    "Strc/fobj/lut/Lut_fieldobj.bcres",
    "Strc/dobj/lut/Lut_building.bcres",
    "Strc/hobj/door/hobj_door/Animation/hobj_door_in.bcres",
    "Strc/hobj/door/hobj_door/Animation/hobj_door_out.bcres",
    "Strc/dobj/dobj_door_a/animation/dobj_door_in.bcres",
    "Strc/dobj/dobj_door_a/animation/dobj_door_out.bcres",
    "Strc/dobj/dobj_door_b/animation/dobj_door_b.bcres",
    "Strc/dobj/dobj_door_c/animation/dobj_door_c_in1.bcres",
    "Strc/dobj/dobj_door_c/animation/dobj_door_c_out1.bcres",
    "Strc/dobj/dobj_door_d/animation/dobj_door_d.bcres",
    "Strc/dobj/dobj_door_e/animation/dobj_door_e.bcres",
};
// the sizes of the heaps
const u32 ALLOCATOR_SIZE = 0x2000;
const u32 COMMON_RES_HEAP_SIZE = 0x5400;
const u32 VILLAGE_FOBJ_HEAP_SIZE = 0x50000;
const u32 VILLAGE_FOBJ_RES_HEAP_SIZE = 0x15B800;
const u32 FENCE_HEAP_SIZE = 0xC800;
const u32 OTHER_FOBJ_HEAP_SIZE = 0xC800;
const u32 OTHER_FOBJ_RES_HEAP_SIZE = 0x4B000;
// the values of Func63070C that make it day / night
const u32 DAY_NIGHT_DAY = 50;
const u32 DAY_NIGHT_NIGHT = 51;
} // namespace

// 0x006E0E74
BsStrcMgr::BsStrcMgr()
    : m_CommonResMgr(), m_FobjResMgr(), m_Hour(0), m_Minute(0), m_Second(0), m_Unknown6A98(0), m_Unknown6A9C(0),
      m_Unknown6AA0(0), m_Unknown6AA4(0), m_LastSecond(0), m_DayNightValue(0), m_IsNight(false), m_Unknown6ACD(0),
      m_Unknown6ACE(0), m_pTownPlaza(NULL), m_Unknown6B50(1), m_Unknown6B51(false), m_Unknown6B54(-1),
      m_Unknown6B58(-1), m_Unknown6B5C(STRUCTURE_EMPTY_SPACE), m_pUnknown6B90(NULL), m_pUnknown6B94(NULL)
{
    for (s32 i = 0; i < 2; i++) {
        for (s32 j = 0; j < DrawList::MODEL_NUM; j++) {
            m_DrawLists[i].models[j] = NULL;
        }
        m_DrawLists[i].num = 0;
    }
}

// 0x006E12B8
// 0x006E12A8 (deleting dtor)
BsStrcMgr::~BsStrcMgr()
{
    // (only the members are destroyed)
}

// 0x006DB3AC
BsStrcMgr::Bridge::Bridge()
{
    // empty (in the original too)
}

// 0x006DC4A0
bool BsStrcMgr::StartInitialize()
{
    sead::Heap* heap = HeapInit::s_pStrcHeap;
    m_Allocator.CreateExpHeap(ALLOCATOR_SIZE, heap, sead::SafeStringBase<char>("BsStrcMgr::Allocator"),
                              sead::Heap::HEAP_DIRECTION_FORWARD, false);
    m_CommonResMgr.Initialize(heap, COMMON_RES_HEAP_SIZE);
    const stage::Name current = stage::GetCurrent();
    if (current == stage::NAME_TITLE || current == stage::NAME_VILLAGE || current == stage::NAME_VILLAGE_PROLOGUE) {
        m_FobjAllocator.CreateExpHeap(VILLAGE_FOBJ_HEAP_SIZE, heap,
                                      sead::SafeStringBase<char>("BsStrcMgr(Village)::FobjAllocator"),
                                      sead::Heap::HEAP_DIRECTION_FORWARD, false);
        m_FobjAllocator.m_pHeap->mFlag |= sead::Heap::FLAG_ENABLE_LOCK;
        m_FobjResMgr.Initialize(heap, VILLAGE_FOBJ_RES_HEAP_SIZE);
        m_FenceAllocator.CreateExpHeap(FENCE_HEAP_SIZE, heap, sead::SafeStringBase<char>("BsStrcMgr::FenceAllocator"),
                                       sead::Heap::HEAP_DIRECTION_FORWARD, false);
    } else if (stage::IsKind(stage::KIND_ISLAND, stage::GetCurrent()) ||
               stage::IsKind(stage::KIND_TOUR, stage::GetCurrent())) {
        m_FobjAllocator.CreateExpHeap(OTHER_FOBJ_HEAP_SIZE, heap,
                                      sead::SafeStringBase<char>("BsStrcMgr(Island)::FobjAllocator"),
                                      sead::Heap::HEAP_DIRECTION_FORWARD, false);
        m_FobjResMgr.Initialize(heap, OTHER_FOBJ_RES_HEAP_SIZE);
    } else if (stage::IsKind(stage::KIND_AUTO_CAMP, stage::GetCurrent())) {
        m_FobjAllocator.CreateExpHeap(OTHER_FOBJ_HEAP_SIZE, heap,
                                      sead::SafeStringBase<char>("BsStrcMgr(AutoCamp)::FobjAllocator"),
                                      sead::Heap::HEAP_DIRECTION_FORWARD, false);
        m_FobjResMgr.Initialize(heap, OTHER_FOBJ_RES_HEAP_SIZE);
    }

    // the time of day (in the multiplayer from the time of the session)
    const nn::fnd::DateTimeParameters* date = time::GetCurrentDate();
    const s32 hour = date->hour;
    if (netgame::IsMultiplePlayer()) {
        const u64 seconds = static_cast<u64>(net::Net::Get()->GetSessionTime()) / 1000;
        m_Second = static_cast<s32>(seconds % 60);
        m_Minute = static_cast<s32>(seconds / 60 % 60);
        m_Hour = static_cast<s32>(seconds / 60 / 60 % 60);
    } else {
        m_Hour = hour;
        m_Minute = date->minute;
        m_Second = date->second;
    }
    m_IsNight = static_cast<u32>(hour - 6) >= 12;
    if (!stage::IsKind(stage::KIND_ISLAND_DEMO, stage::GetCurrent())) {
        if (Func63070C() == DAY_NIGHT_DAY) {
            m_IsNight = false;
        } else if (Func63070C() == DAY_NIGHT_NIGHT) {
            m_IsNight = true;
        }
    }
    for (s32 i = 0; i < BRIDGE_NUM; i++) {
        m_Bridges[i].kind = STRUCTURE_EMPTY_SPACE;
        m_Bridges[i].position.x = 0.0f;
        m_Bridges[i].position.y = 0.0f;
        m_Bridges[i].position.z = 0.0f;
    }
    return true;
}

// 0x006DB884
oml::framework::Result BsStrcMgr::UpdateInitialize()
{
    stage::GetCurrent();
    switch (s_InitState) {
    case 0: {
        // the common resources are loaded
        for (s32 i = 0; i < COMMON_RES_NUM; i++) {
            m_CommonKeys[i] = m_CommonResMgr.Load(COMMON_RES_FILES[i]);
        }
        bool isLoaded = true;
        for (s32 i = 0; i < COMMON_RES_NUM; i++) {
            isLoaded &= m_CommonKeys[i].IsValid();
        }
        if (isLoaded) {
            s_InitState++;
        }
        break;
    }
    case 1:
        for (s32 i = 0; i < COMMON_RES_NUM; i++) {
            m_CommonResMgr.Get(m_CommonKeys[i])->InitializeCgfx(&m_Allocator, NULL, true);
        }
        s_pInstance = this;
        s_InitState++;
        break;
    case 2:
        CreateStrcs();
        s_InitState++;
        break;
    case 3: {
        // the structures have started
        bool isReady = true;
        for (s32 i = 0; i < m_Strcs.size(); i++) {
            AcStrc* strc = i < m_Strcs.size() ? m_Strcs.at(i) : NULL;
            isReady &= strc->vf_0x78();
        }
        if (isReady) {
            s_InitState = INIT_STATE_READY;
            return oml::framework::RESULT_SUCCESS;
        }
        break;
    }
    }
    return oml::framework::RESULT_RETRY;
}

// 0x006DB3B0
bool BsStrcMgr::EndInitialize()
{
    m_DayNight.Reset(false);
    if (!netgame::IsMultiPlayer()) {
        std::memset(g_Unknown998C70, 0, sizeof(g_Unknown998C70));
    }
    // up to three bridges with their positions (the N-S and W-E bridges at the corner of their units)
    u32 num = 0;
    for (AcStrc** it = m_Strcs.begin(); it != m_Strcs.end(); it++) {
        if (!IsBridgeStructure(Func1500B0(*it)) || num >= BRIDGE_NUM) {
            continue;
        }
        Bridge& bridge = m_Bridges[num];
        bridge.kind = Func1500B0(*it);
        bridge.position = (*it)->m_Position;
        switch (bridge.kind - STRUCTURE_BRIDGE_FIRST) {
        case 0:
        case 3:
        case 4:
        case 7:
        case 8:
        case 11:
        case 12:
        case 15:
        case 16:
        case 19:
        case 20:
        case 23:
            bridge.position.x += 16.0f;
            bridge.position.z += 16.0f;
            break;
        }
        num++;
    }
    return true;
}

// 0x006DD610
bool BsStrcMgr::StartFinalize()
{
    return true;
}

// 0x006DD45C
oml::framework::Result BsStrcMgr::UpdateFinalize()
{
    m_CommonResMgr.Finalize();
    m_Allocator.DestroyHeap();
    m_FobjResMgr.Finalize();
    m_FobjAllocator.DestroyHeap();
    s_pInstance = NULL;
    s_InitState = 0;
    return oml::framework::RESULT_SUCCESS;
}

// 0x006DCD90
bool BsStrcMgr::EndFinalize()
{
    return true;
}

// 0x006DF8E0
oml::framework::Result BsStrcMgr::Draw()
{
    for (s32 i = 0; i < 2; i++) {
        DrawList& list = m_DrawLists[i];
        for (s32 scene = 0; scene < 3; scene++) {
            if (((1 >> scene) & 1) == 0) {
                continue;
            }
            for (u32 j = 0; j < list.num; j++) {
                if (list.models[j] != NULL) {
                    list.models[j]->AddToScene(scene);
                }
            }
        }
        for (s32 j = 0; j < DrawList::MODEL_NUM; j++) {
            list.models[j] = NULL;
        }
        list.num = 0;
    }
    return oml::framework::RESULT_SUCCESS;
}

// 0x006E0620
oml::framework::Result BsStrcMgr::Calc()
{
    if (m_Unknown6B51) {
        Func6E0A74(m_Unknown6B54, m_Unknown6B58, m_Unknown6B5C);
    }
    // the hour and the minute of the last change
    if (m_Minute < m_Unknown6AA4) {
        m_Unknown6AA0 = m_Unknown6A98;
        m_Unknown6A98 = m_Hour;
    }
    if (m_Second < m_LastSecond) {
        m_Unknown6AA4 = m_Unknown6A9C;
        m_Unknown6A9C = m_Minute;
    }
    m_LastSecond = m_Second;
    const nn::fnd::DateTimeParameters* date = time::GetCurrentDate();
    m_Hour = date->hour;
    m_Minute = date->minute;
    m_Second = date->second;
    m_IsNight = static_cast<u32>(m_Hour - 6) >= 12;
    if (!stage::IsKind(stage::KIND_ISLAND_DEMO, stage::GetCurrent())) {
        if (Func63070C() == DAY_NIGHT_DAY) {
            m_IsNight = false;
        } else if (Func63070C() == DAY_NIGHT_NIGHT) {
            m_IsNight = true;
        }
    }
    if (m_IsNight) {
        m_DayNight.Set(true, true, false);
    } else {
        m_DayNight.Set(false, true, false);
    }
    m_DayNight.Update();
    m_DayNightValue = static_cast<s32>(m_DayNight.m_Value * 255.0f);

    // the positions of the NPCs
    if (stage::IsKind(static_cast<stage::Kind>(stage::KIND_OUTDOOR | stage::KIND_EXHIBITION), stage::GetCurrent()) ||
        stage::GetCurrent() == stage::NAME_DOWNTOWN) {
        BsNpcMgr* npcMgr = BsNpcMgr::s_pInstance;
        if (npcMgr != NULL) {
            BsNpcMgr::NpcPosGetCB positions(NULL);
            npcMgr->ForeachImpl(positions);
            m_NpcPositionNum = static_cast<u8>(positions.m_Num);
            for (s32 i = 0; i < m_NpcPositionNum; i++) {
                m_NpcPositions[i] = positions.GetPosition(i);
            }
        }
    }
    m_Unknown6B5D = g_UnknownAAFF90.unk204 > 0.0f;
    return oml::framework::RESULT_SUCCESS;
}

// 0x006DB530
g3d::ResourceLoader* BsStrcMgr::GetBuildingLut()
{
    return m_CommonResMgr.Get(m_CommonKeys[COMMON_RES_LUT_BUILDING]);
}

// 0x006DB5A4
g3d::ResourceLoader* BsStrcMgr::GetFieldObjLut()
{
    return m_CommonResMgr.Get(m_CommonKeys[COMMON_RES_LUT_FIELDOBJ]);
}

// 0x006DB618
g3d::ResourceLoader* BsStrcMgr::GetHouseLut()
{
    return m_CommonResMgr.Get(m_CommonKeys[COMMON_RES_LUT_HOUSE]);
}

// 0x006DB68C
void BsStrcMgr::GetStrcInfo(void* out, u32 index)
{
    AcStrc* strc = index < static_cast<u32>(m_Strcs.size()) ? m_Strcs.at(index) : NULL;
    strc->vf_0x9C(out);
}

// 0x006DB6B4
g3d::ResourceLoader* BsStrcMgr::GetSobjLut()
{
    return m_CommonResMgr.Get(m_CommonKeys[COMMON_RES_LUT_SOBJ]);
}

// 0x006DF244
g3d::ResourceLoader* BsStrcMgr::GetHobjDoorInAnim()
{
    return m_CommonResMgr.Get(m_CommonKeys[COMMON_RES_HOBJ_DOOR_IN]);
}

// 0x006DF40C
g3d::ResourceLoader* BsStrcMgr::GetHobjDoorOutAnim()
{
    return m_CommonResMgr.Get(m_CommonKeys[COMMON_RES_HOBJ_DOOR_OUT]);
}

// 0x006DF5B4
g3d::ResourceLoader* BsStrcMgr::GetDobjDoorBAnim()
{
    return m_CommonResMgr.Get(m_CommonKeys[COMMON_RES_DOBJ_DOOR_B]);
}

// 0x006DF628
g3d::ResourceLoader* BsStrcMgr::GetDobjDoorDAnim()
{
    return m_CommonResMgr.Get(m_CommonKeys[COMMON_RES_DOBJ_DOOR_D]);
}

// 0x006DF69C
g3d::ResourceLoader* BsStrcMgr::GetDobjDoorEAnim()
{
    return m_CommonResMgr.Get(m_CommonKeys[COMMON_RES_DOBJ_DOOR_E]);
}

// 0x006DF710
g3d::ResourceLoader* BsStrcMgr::GetDoorInAnim()
{
    return m_CommonResMgr.Get(m_CommonKeys[COMMON_RES_HOBJ_DOOR_IN]);
}

// 0x006DF784
g3d::ResourceLoader* BsStrcMgr::GetDobjDoorCInAnim()
{
    return m_CommonResMgr.Get(m_CommonKeys[COMMON_RES_DOBJ_DOOR_C_IN]);
}

// 0x006DF7F8
g3d::ResourceLoader* BsStrcMgr::GetDoorOutAnim()
{
    return m_CommonResMgr.Get(m_CommonKeys[COMMON_RES_HOBJ_DOOR_OUT]);
}

// 0x006DF86C
g3d::ResourceLoader* BsStrcMgr::GetDobjDoorCOutAnim()
{
    return m_CommonResMgr.Get(m_CommonKeys[COMMON_RES_DOBJ_DOOR_C_OUT]);
}

namespace {
// the place of the train at the station
const nn::math::VEC3 TRAIN_POSITION(0.0f, 120.0f, 446.0f);
// the structures of the prologue only in the rows above this one
const s32 PROLOGUE_MAX_UNIT_Z = 25;
// the height of the bridges above the ground
const f32 BRIDGE_HEIGHT = 32.0f;
// the stage of the structures that are on every field (GetStageOfStructure; meaning unknown)
const u8 STAGE_ANY = 165;

// the position of a field object: the center of its unit on the ground
inline void GetStrcPosition(nn::math::VEC3* position, s32 unitX, s32 unitZ)
{
    UnitToWorld(position, unitX, unitZ);
    position->y = FdInfo::GetBgHeight(*position, FIELD_NAME_CURRENT);
}

// the structure belongs to the field of the town
inline bool IsStrcOfStage(u8 kind)
{
    const u8 stage = GetStageOfStructure(kind);
    return stage == stage::NAME_VILLAGE || stage == STAGE_ANY;
}
} // namespace

inline void BsStrcMgr::AddStrc(Actor* actor)
{
    AcStrc* strc = static_cast<AcStrc*>(actor);
    const u8 kind = Func1500B0(strc);
    if (IsPlayerHouseStructure(kind)) {
        if (Func150420(strc)) {
            m_PlayerHouses.pushBack(actor);
        } else {
            m_Strcs.pushBack(strc);
        }
    } else if (Func6CB77C(kind)) {
        m_Unknown6774.pushBack(actor);
    } else if (!Func6CD470(kind) && !IsBridgeStructure(kind) && kind != STRUCTURE_PICNIC_BLANKET) {
        m_Unknown68C0.pushBack(actor);
    } else {
        m_Strcs.pushBack(strc);
    }
}

// 0x006DF9CC
void BsStrcMgr::CreateStrcs()
{
    const stage::Name current = stage::GetCurrent();
    if (current == stage::NAME_VILLAGE_PROLOGUE) {
        // (the prologue: the train and the structures of the lower part of the town)
        Actor* train = static_cast<Actor*>(Actor::Create(PROC_AC_STRC_TRAIN, this, 0, &TRAIN_POSITION, NULL));
        FieldObjectList* list = GetFieldObjectList(0);
        const u32 num = list->GetNum();
        for (u32 i = 0; i < num; i++) {
            const FieldObjectEntry entry = list->Get(i);
            if (entry.unitZ >= PROLOGUE_MAX_UNIT_Z) {
                continue;
            }
            nn::math::VEC3 position;
            GetStrcPosition(&position, entry.unitX, entry.unitZ);
            if (IsBridgeStructure(entry.kind)) {
                position.y += BRIDGE_HEIGHT;
            }
            if (entry.kind == STRUCTURE_KAPPNS_BOAT) {
                Actor* boat = static_cast<Actor*>(Actor::Create(PROC_AC_STRC_BOAT, this, 0, &position, NULL));
                if (boat != NULL) {
                    m_Boats.pushBack(boat);
                }
            } else if (entry.kind == STRUCTURE_RAILROAD_CROSSING_ARMS) {
                Actor* actor = static_cast<Actor*>(
                    Actor::Create(static_cast<ProcName>(Func5CA378(entry.kind)), this, 0, &position, NULL));
                if (actor != NULL) {
                    *reinterpret_cast<Actor**>(reinterpret_cast<u8*>(actor) + 0x5BC) = train;
                }
            } else {
                Actor* actor = static_cast<Actor*>(
                    Actor::Create(static_cast<ProcName>(Func5CA378(entry.kind)), this, entry.kind, &position, NULL));
                if (actor != NULL) {
                    AddStrc(actor);
                }
            }
        }
        FieldObjectList* list1 = GetFieldObjectList(1);
        if (list1->GetNum() != 0) {
            const FieldObjectIterator end = GetFieldObjectEnd(list1);
            for (FieldObjectIterator it(list1); it != end; ++it) {
                if (it.unitZ >= PROLOGUE_MAX_UNIT_Z) {
                    continue;
                }
                nn::math::VEC3 position;
                GetStrcPosition(&position, it.unitX, it.unitZ);
                const u8 kind = it.kind;
                Actor* actor = static_cast<Actor*>(
                    Actor::Create(static_cast<ProcName>(Func5CA378(it.kind)), this, kind, &position, NULL));
                if (actor != NULL) {
                    m_Unknown67A8.pushBack(actor);
                }
            }
        }
        return;
    }
    if (!stage::IsKind(stage::KIND_VILLAGE, stage::GetCurrent())) {
        if (current == stage::NAME_DOWNTOWN) {
            CreateDowntownStrcs();
        } else if (stage::IsKind(stage::KIND_ISLAND_DEMO, stage::GetCurrent())) {
            // (the pier of the island demo and the boat)
            const u32 pier =
                stage::GetCurrent() == stage::NAME_SEA_DEPARTURE ? STRUCTURE_DOCK : STRUCTURE_ISLAND_STORAGE_BIN;
            Actor* actor = static_cast<Actor*>(Actor::Create(PROC_AC_STRC_DEMO_PIER, this, pier, &VEC3_ZERO, NULL));
            if (actor != NULL) {
                m_DemoPier.pushBack(actor);
            }
            Actor* boat = static_cast<Actor*>(Actor::Create(PROC_AC_STRC_BOAT, this, 0, &VEC3_ZERO, NULL));
            if (boat != NULL) {
                m_Boats.pushBack(boat);
            }
        } else if (stage::IsKind(stage::KIND_ISLAND, stage::GetCurrent())) {
            CreateIslandStrcs();
        } else if (stage::IsKind(stage::KIND_TOUR, stage::GetCurrent())) {
            CreateTourStrcs();
        } else if (stage::IsKind(static_cast<stage::Kind>(stage::KIND_OUTDOOR | stage::KIND_EXHIBITION),
                                 stage::GetCurrent())) {
            CreateOutdoorStrcs();
        } else if (stage::IsKind(stage::KIND_AUTO_CAMP, stage::GetCurrent())) {
            FieldObjectList* list = GetFieldObjectList(13);
            for (u32 i = 0; i < 5; i++) {
                const FieldObjectEntry* entry = Func7128EC(list, i);
                if (entry->kind >= STRUCTURE_EMPTY_SPACE) {
                    continue;
                }
                nn::math::VEC3 position;
                UnitToWorld(&position, entry->unitX, entry->unitZ);
                if (!IsStrcOfStage(entry->kind)) {
                    continue;
                }
                position.y = FdInfo::GetBgHeight(position, FIELD_NAME_CURRENT);
                Actor* actor = static_cast<Actor*>(Actor::Create(static_cast<ProcName>(Func5CA378(entry->kind)), this,
                                                                 entry->kind, &position, NULL));
                if (actor != NULL) {
                    if (entry->kind == STRUCTURE_FIRE_PIT) {
                        m_pUnknown6B94 = actor;
                    }
                    m_Unknown6A6C.pushBack(actor);
                }
            }
        }
        return;
    }

    // the town: the train, the structures of the town, the objects of the lists 1 and 12
    Actor* train = static_cast<Actor*>(Actor::Create(PROC_AC_STRC_TRAIN, this, 0, &TRAIN_POSITION, NULL));
    FieldObjectList* list = GetFieldObjectList(0);
    const u32 num = list->GetNum();
    for (u32 i = 0; i < num; i++) {
        const FieldObjectEntry entry = list->Get(i);
        if (!IsStrcOfStage(entry.kind)) {
            continue;
        }
        nn::math::VEC3 position;
        GetStrcPosition(&position, entry.unitX, entry.unitZ);
        if (IsBridgeStructure(entry.kind)) {
            position.y += BRIDGE_HEIGHT;
        }
        if (entry.kind == STRUCTURE_KAPPNS_BOAT) {
            Actor* boat = static_cast<Actor*>(Actor::Create(PROC_AC_STRC_BOAT, this, 0, &position, NULL));
            if (boat != NULL) {
                m_Boats.pushBack(boat);
            }
            continue;
        }
        if (entry.kind == STRUCTURE_RAILROAD_CROSSING_ARMS) {
            Actor* actor = static_cast<Actor*>(
                Actor::Create(static_cast<ProcName>(Func5CA378(entry.kind)), this, 0, &position, NULL));
            if (actor != NULL) {
                *reinterpret_cast<Actor**>(reinterpret_cast<u8*>(actor) + 0x5BC) = train;
            }
            continue;
        }
        Actor* actor = static_cast<Actor*>(
            Actor::Create(static_cast<ProcName>(Func5CA378(entry.kind)), this, entry.kind, &position, NULL));
        if (entry.kind == STRUCTURE_TOWN_PLAZA) {
            m_pTownPlaza = actor;
        }
        if (IsTownHallStructure(entry.kind)) {
            // (the ticket machine beside the town hall)
            nn::math::VEC3 machine = position;
            machine.x += Func150430()->x;
            machine.z += Func150430()->z;
            Actor::Create(PROC_AC_HOME_TICKET_MACHINE, this, 0, &machine, NULL);
        }
        if (actor != NULL) {
            AddStrc(actor);
        }
    }
    FieldObjectList* list1 = GetFieldObjectList(1);
    if (list1->GetNum() != 0) {
        const FieldObjectIterator end = GetFieldObjectEnd(list1);
        for (FieldObjectIterator it(list1); it != end; ++it) {
            nn::math::VEC3 position;
            UnitToWorld(&position, it.unitX, it.unitZ);
            if (!IsStrcOfStage(it.kind)) {
                continue;
            }
            position.y = FdInfo::GetBgHeight(position, FIELD_NAME_CURRENT);
            const u8 kind = it.kind;
            Actor* actor = static_cast<Actor*>(
                Actor::Create(static_cast<ProcName>(Func5CA378(it.kind)), this, kind, &position, NULL));
            if (actor != NULL) {
                m_Unknown67A8.pushBack(actor);
            }
        }
    }
    FieldObjectList* list12 = GetFieldObjectList(12);
    const u32 num12 = list12->GetNum();
    for (u32 i = 0; i < num12; i++) {
        const FieldObjectEntry* entry = Func7191A8(list12, i);
        if (entry->kind >= STRUCTURE_EMPTY_SPACE) {
            continue;
        }
        nn::math::VEC3 position;
        UnitToWorld(&position, entry->unitX, entry->unitZ);
        if (!IsStrcOfStage(entry->kind)) {
            continue;
        }
        position.y = FdInfo::GetBgHeight(position, FIELD_NAME_CURRENT);
        Actor* actor = static_cast<Actor*>(
            Actor::Create(static_cast<ProcName>(Func5CA378(entry->kind)), this, entry->kind, &position, NULL));
        if (actor != NULL) {
            m_Unknown6A5C.pushBack(actor);
        }
    }
}

// 0x006DE08C
void BsStrcMgr::CreateDowntownStrcs()
{
    FieldObjectList* list = GetFieldObjectList(2);
    const u32 num = list->GetNum();
    // (the state of the shops decides which buildings are opened: the structures of m_Strcs)
    bool isNooklingOpen = false;
    bool isLeifOpen = false;
    const SvShopZakka* shop = &GetGardenPlus()->data.townData.unk_0xEE2C.shopZakka;
    const SvTownDataUnkEE2C* shops = &GetGardenPlus()->data.townData.unk_0xEE2C;
    const s32* leif = &shops->unk_0x132C0;
    if (shop != NULL && leif != NULL) {
        if (!shop->IsRenovationDay()) {
            if (shop->nooklingState == 1 || shop->nooklingState == 2) {
                isNooklingOpen = true;
            }
        }
        if (shop->nooklingState == 4) {
            const nn::math::VEC3 position(1354.0f, 112.0f, 368.0f);
            Actor::Create(PROC_AC_STRC_DOWNTOWN_LAMP, this, STRUCTURE_MAIN_STREET_LAMP, &position, NULL);
        }
        if (shops->leifUnlockStatus == 3) {
            isLeifOpen = true;
        }
    }
    bool isShampoodleOpen = false;
    const u8* shampoodle = &GetGardenPlus()->data.townData.unk_0xEE2C.shampoodleUnlockStatus;
    if (shampoodle != NULL && *shampoodle != 1) {
        isShampoodleOpen = true;
    }
    for (u32 i = 0; i < num; i++) {
        const FieldObjectEntry entry = list->Get(i);
        nn::math::VEC3 position;
        GetStrcPosition(&position, entry.unitX, entry.unitZ);
        AcStrc* strc = static_cast<AcStrc*>(
            Actor::Create(static_cast<ProcName>(Func5CA378(entry.kind)), this, entry.kind, &position, NULL));
        if (strc == NULL) {
            continue;
        }
        bool isOpen;
        switch (Func1500B0(strc)) {
        case STRUCTURE_SHAMPOODLE:
            isOpen = isShampoodleOpen;
            break;
        case STRUCTURE_DREAM_SUITE:
        case STRUCTURE_CLUB_LOL:
        case STRUCTURE_MUSEUM:
            isOpen = true;
            break;
        case STRUCTURE_NOOKLING_STORES:
            isOpen = isNooklingOpen;
            break;
        case STRUCTURE_GARDENING_STORE:
            isOpen = isLeifOpen;
            break;
        default:
            isOpen = false;
            break;
        }
        if (isOpen) {
            m_Strcs.pushBack(strc);
        } else if (!Func150410(strc)) {
            m_Unknown68C0.pushBack(strc);
        }
    }
    Actor::Create(PROC_AC_STRC_DOWNTOWN_ATT, this, 0, &VEC3_ZERO, NULL);
}

// 0x006DEB3C
void BsStrcMgr::CreateTourStrcs()
{
    FieldObjectList* list = GetFieldObjectList(4);
    const u32 num = list->GetNum();
    for (u32 i = 0; i < num; i++) {
        const FieldObjectEntry entry = list->Get(i);
        nn::math::VEC3 position;
        GetStrcPosition(&position, entry.unitX, entry.unitZ);
        AcStrc* strc = static_cast<AcStrc*>(
            Actor::Create(static_cast<ProcName>(Func5CA378(entry.kind)), this, entry.kind, &position, NULL));
        if (strc == NULL) {
            continue;
        }
        const u8 kind = Func1500B0(strc);
        if (IsPlayerHouseStructure(kind)) {
            if (Func150420(strc)) {
                m_PlayerHouses.pushBack(strc);
            } else {
                m_Strcs.pushBack(strc);
            }
        } else if (Func6CB77C(kind)) {
            m_Unknown6774.pushBack(strc);
        } else if (Func6CD470(kind)) {
            m_Strcs.pushBack(strc);
        } else if (IsBridgeStructure(kind)) {
            m_Strcs.pushBack(strc);
        } else {
            m_Unknown68C0.pushBack(strc);
        }
    }
}

// 0x006DED28
u8 BsStrcMgr::Func6DED28()
{
    return 0xFF;
}

// 0x006DED30
bool BsStrcMgr::IsMultiplayer()
{
    return netgame::IsMultiplePlayer();
}

// 0x006DED44
void BsStrcMgr::CreateIslandStrcs()
{
    FieldObjectList* list = GetFieldObjectList(3);
    const u32 num = list->GetNum();
    for (u32 i = 0; i < num; i++) {
        const FieldObjectEntry entry = list->Get(i);
        nn::math::VEC3 position;
        GetStrcPosition(&position, entry.unitX, entry.unitZ);
        AcStrc* strc = static_cast<AcStrc*>(
            Actor::Create(static_cast<ProcName>(Func5CA378(entry.kind)), this, entry.kind, &position, NULL));
        if (strc == NULL) {
            continue;
        }
        const u8 kind = Func1500B0(strc);
        if (IsPlayerHouseStructure(kind)) {
            if (Func150420(strc)) {
                m_PlayerHouses.pushBack(strc);
            } else {
                m_Strcs.pushBack(strc);
            }
        } else if (Func6CB77C(kind)) {
            m_Unknown6774.pushBack(strc);
        } else {
            m_Unknown68C0.pushBack(strc);
        }
    }
}

namespace {
// the houses of the players of a section of the Happy Home Showcase (Func71E484; 16 entries of 0x7DD8 bytes, bit 0 of
// +0x7DD6: the house is there)
const u32 SHOWCASE_HOUSE_NUM = 16;
const u32 SHOWCASE_HOUSE_SIZE = 0x7DD8;
const u32 SHOWCASE_HOUSE_FLAG_OFFSET = 0x7DD6;
// the birds of the bulletin boards: three pairs on free places of the field
const s32 BIRD_PAIR_NUM = 3;
const u32 BIRD_UNIT_RANGE = 27;
const s32 BIRD_UNIT_MIN = 18;
const f32 BIRD_HEIGHT = 112.0f;
const f32 BIRD_PAIR_OFFSET = 32.0f;

// the actors of a list of the field (they are not kept)
inline void CreateListActors(BsStrcMgr* mgr, u32 type)
{
    FieldObjectList* list = GetFieldObjectList(type);
    const u32 num = list->GetNum();
    for (u32 i = 0; i < num; i++) {
        const FieldObjectEntry entry = list->Get(i);
        nn::math::VEC3 position;
        GetStrcPosition(&position, entry.unitX, entry.unitZ);
        Actor::Create(static_cast<ProcName>(Func5CA378(entry.kind)), mgr, entry.kind, &position, NULL);
    }
}

// the unit is free for a bird (the attributes of the units of the bird)
inline bool IsBirdUnit(s32 x, s32 z)
{
    const FieldAttribute attribute = FdInfo::GetAttribute(x, z, FIELD_NAME_CURRENT);
    return attribute == FIELD_ATTRIBUTE_SOIL_14 || attribute == FIELD_ATTRIBUTE_STONE_11;
}
} // namespace

// the houses of a section of the Happy Home Showcase (the model houses go into m_Unknown68C0, the others into m_Strcs)
DECOMP_ALWAYS_INLINE void BsStrcMgr::CreateShowcaseHouses(u32 type, u32 section)
{
    FieldObjectList* list = GetFieldObjectList(type);
    const u8* houses = Func71E484(Func537D80(), section);
    for (u32 i = 0; i < SHOWCASE_HOUSE_NUM; i++) {
        if ((houses[i * SHOWCASE_HOUSE_SIZE + SHOWCASE_HOUSE_FLAG_OFFSET] & 1) == 0) {
            continue;
        }
        const FieldObjectEntry entry = list->Get(i);
        nn::math::VEC3 position;
        GetStrcPosition(&position, entry.unitX, entry.unitZ);
        AcStrc* strc = static_cast<AcStrc*>(
            Actor::Create(static_cast<ProcName>(Func5CA378(entry.kind)), this, entry.kind, &position, NULL));
        if (strc == NULL) {
            continue;
        }
        if (Func150418(strc)) {
            m_Unknown68C0.pushBack(strc);
        } else {
            m_Strcs.pushBack(strc);
        }
    }
}

// 0x006DE378
void BsStrcMgr::CreateOutdoorStrcs()
{
    const stage::Name current = stage::GetCurrent();
    switch (current) {
    case stage::NAME_EXHIBITION_NORTH:
        CreateShowcaseHouses(6, 2);
        CreateListActors(this, 9);
        break;
    case stage::NAME_EXHIBITION_EAST:
        CreateShowcaseHouses(7, 0);
        CreateListActors(this, 10);
        break;
    case stage::NAME_EXHIBITION_WEST:
        CreateShowcaseHouses(8, 1);
        CreateListActors(this, 11);
        break;
    case stage::NAME_EXHIBITION_ENTRANCE:
        CreateListActors(this, 5);
        break;
    default:
        break;
    }
    if (m_IsNight || current == stage::NAME_EXHIBITION_ENTRANCE) {
        return;
    }
    // (by day) three pairs of birds on random places whose 3x3 units are free
    s32 unitXs[BIRD_PAIR_NUM] = {-5, 0, 0};
    s32 unitZs[BIRD_PAIR_NUM] = {-5, 0, 0};
    u32 arg = 0;
    for (s32 pair = 0; pair < BIRD_PAIR_NUM;) {
        const s32 x = netgame::Func2FF63C(BIRD_UNIT_RANGE) + BIRD_UNIT_MIN;
        const s32 z = netgame::Func2FF63C(BIRD_UNIT_RANGE) + BIRD_UNIT_MIN;
        for (s32 i = 0; i < pair; i++) {
            // (the test against the other pairs is empty)
        }
        if (!IsBirdUnit(x - 1, z - 1) || !IsBirdUnit(x - 1, z + 2) || !IsBirdUnit(x + 2, z - 1) ||
            !IsBirdUnit(x + 2, z + 2) || !IsBirdUnit(x, z) || !IsBirdUnit(x + 1, z + 1) || !IsBirdUnit(x, z - 1)) {
            continue;
        }
        unitXs[pair] = x;
        unitZs[pair] = z;
        nn::math::VEC3 position;
        UnitToWorld(&position, x, z);
        position.y = BIRD_HEIGHT;
        Actor* bird = static_cast<Actor*>(Actor::Create(PROC_AC_STRC_BBS_BIRD, this, arg, &position, NULL));
        *reinterpret_cast<nn::math::VEC3*>(reinterpret_cast<u8*>(bird) + 0x3E0) = VEC3_ZERO;
        position.x += BIRD_PAIR_OFFSET;
        position.z += BIRD_PAIR_OFFSET;
        bird = static_cast<Actor*>(Actor::Create(PROC_AC_STRC_BBS_BIRD, this, arg + 1, &position, NULL));
        *reinterpret_cast<nn::math::VEC3*>(reinterpret_cast<u8*>(bird) + 0x3E0) = VEC3_ZERO;
        pair++;
        arg += 2;
    }
    (void)unitXs;
    (void)unitZs;
}

// 0x006DB524
u32 BsStrcMgr::Func6DB524()
{
    // (the item NAME_MARINE_SONG_2001 by the item list; meaning unknown)
    return SvFgName::NAME_MARINE_SONG_2001;
}

// 0x006DCD74
void BsStrcMgr::Func6DCD74()
{
    if (m_pTownPlaza != NULL) {
        Func1503E8(m_pTownPlaza);
    }
}

// 0x006DD41C
const char* GetRegionSuffix()
{
    const char* const suffixes[10] = {"_JP",     "_US_ENG", "_US_SPA", "_US_FRA", "_EU_ENG",
                                      "_EU_SPA", "_EU_FRA", "_ITA",    "_GER",    "_KR"};
    return suffixes[GetGardenData()->townData.unk_0xEDAA & 15];
}

// 0x006DD618
void BsStrcMgr::GetUnknown998DA4(nn::math::VEC3* out)
{
    *out = g_Unknown998DA4;
}

// 0x006DD63C
f32 BsStrcMgr::Func6DD63C()
{
    return 0.0f;
}

// 0x006DD648
u8 BsStrcMgr::GetUnknown998C70(u32 index)
{
    return g_Unknown998C70[index] & 15;
}

// 0x006DD65C
void BsStrcMgr::GetUnknown998D5C(nn::math::VEC3* out)
{
    *out = g_Unknown998D5C;
}

// 0x006DD680
f32 BsStrcMgr::Func6DD680()
{
    return 5.0f;
}

// 0x006DD7E4
void BsStrcMgr::Func6DD7E4()
{
    Func3080D0();
    ClearUnknown998C70();
}

// 0x006DD7F4
void BsStrcMgr::ClearUnknown998C70()
{
    std::memset(g_Unknown998C70, 0, sizeof(g_Unknown998C70));
}

// 0x006DD804
AcStrc* BsStrcMgr::FindVillagerHouse(u32 index)
{
    const u8 kind = index + 8;
    for (Actor** it = m_Unknown6774.begin(); it != m_Unknown6774.end(); it++) {
        if (Func1500B0(static_cast<AcStrc*>(*it)) == kind) {
            return static_cast<AcStrc*>(*it);
        }
    }
    return NULL;
}

// 0x006DD85C
void BsStrcMgr::ClearUnknown998C70Bit(u32 index, u32 bit)
{
    g_Unknown998C70[index] &= ~(16 << bit);
}

// 0x006DD878
void BsStrcMgr::Func6DD878()
{
    if (m_pTownPlaza != NULL) {
        Func1503F0(m_pTownPlaza);
    }
}

// 0x006DD894
u32 BsStrcMgr::Func6DD894()
{
    return 0xDC64;
}

// 0x006DD8A0
const char* GetShopName()
{
    const char* const names[7] = {"shop", "conveni", "super", "homecenter", "depart", "rebuilding_02", "rebuilding_03"};
    const SvShopZakka* shop = &GetGardenPlus()->data.townData.unk_0xEE2C.shopZakka;
    if (shop->IsRenovationDay()) {
        return shop->nooklingStateUnknown <= 2 ? names[5] : names[6];
    }
    return names[shop->nooklingState];
}

// 0x006DD8FC
AcStrc* BsStrcMgr::FindFranklinsTable()
{
    for (Actor** it = m_Unknown67A8.begin(); it != m_Unknown67A8.end(); it++) {
        if (Func1500B0(static_cast<AcStrc*>(*it)) == STRUCTURE_FRANKLINS_TABLE) {
            return static_cast<AcStrc*>(*it);
        }
    }
    return NULL;
}

// 0x006DD94C
bool BsStrcMgr::IsUnknown67BCEmpty()
{
    return m_Unknown67BC.size() == 0;
}

// 0x006DE304
const char* GetShampoodleName()
{
    const char* const names[3] = {"cut", "rebuilding_01", "cut"};
    return names[GetGardenPlus()->data.townData.unk_0xEE2C.shampoodleUnlockStatus];
}

// 0x006DE338
const nn::math::VEC3* BsStrcMgr::GetBridgePosition(u32 index)
{
    if (index < BRIDGE_NUM && IsBridgeStructure(m_Bridges[index].kind)) {
        return &m_Bridges[index].position;
    }
    return NULL;
}

// 0x006DE370
u8 BsStrcMgr::Func6DE370()
{
    return 0xFF;
}

// 0x006E0938
Actor* BsStrcMgr::GetBoat()
{
    if (m_Boats.size() == 1) {
        return m_Boats.at(0);
    }
    return NULL;
}

// 0x006E0958
u32 BsStrcMgr::Func6E0958()
{
    return 0;
}

// 0x006E0E24
void BsStrcMgr::Func6E0E24()
{
    if (m_Unknown67BC.size() != 1 || !IsPlayerHouseStructure(Func1500B0(static_cast<AcStrc*>(m_Unknown67BC.at(0))))) {
        return;
    }
    Func150438(static_cast<AcStrc*>(m_Unknown67BC.size() > 0 ? m_Unknown67BC.at(0) : NULL));
}

namespace {
// the object index of a structure actor (a field of AcStrc at 0x80, whose layout is not known yet)
inline s32 GetStrcObjectIndex(const Actor* actor)
{
    return *reinterpret_cast<const s32*>(reinterpret_cast<const u8*>(actor) + 0x80);
}
} // namespace

// 0x006DB728
Actor* BsStrcMgr::FindStrcAt(s32 x, s32 z)
{
    if (!FdInfo::IsAttribute02_03_04_A2_A5_F8(x, z, FIELD_NAME_CURRENT)) {
        return NULL;
    }
    const s32 index = Func24063C(x, z, 0);
    if (index == -1) {
        return NULL;
    }
    for (Actor** it = m_Unknown68C0.begin(); it != m_Unknown68C0.end(); it++) {
        if (GetStrcObjectIndex(*it) == index && Func5CAF84(Func1500B0(static_cast<AcStrc*>(*it)))) {
            return *it;
        }
    }
    for (Actor** it = m_Unknown67A8.begin(); it != m_Unknown67A8.end(); it++) {
        if (GetStrcObjectIndex(*it) == index && Func5CAD4C(Func1500B0(static_cast<AcStrc*>(*it))) == 10) {
            return *it;
        }
    }
    if (stage::IsKind(stage::KIND_AUTO_CAMP, stage::GetCurrent())) {
        for (Actor** it = m_Unknown6A6C.begin(); it != m_Unknown6A6C.end(); it++) {
            if (GetStrcObjectIndex(*it) == index) {
                return *it;
            }
        }
    }
    return NULL;
}

// 0x006DD68C
bool BsStrcMgr::IsOutOfCameraDepth(const nn::math::VEC3* position, bool test, f32 unused, f32 depth)
{
    (void)unused;
    const camera::Mgr* camera = camera::Mgr::s_pInstance;
    if (camera == NULL) {
        return false;
    }
    const f32 offset = position->z - camera->m_pUnk0x1A4->m_Point.z;
    if (offset < -depth || offset > depth * 0.5f) {
        return true;
    }
    return test && Func68F5E8();
}

// 0x006DD700
void BsStrcMgr::SetUnknown998C70(u32 index, u32 bit, u32 value)
{
    if (index == 18) {
        return;
    }
    switch (index) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 15:
    case 16:
    case 17:
        if (value == 4) {
            value = 0;
            bit = 5;
        }
        g_Unknown998C70[index] = (value & 15) | (16 << bit);
        break;
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
        if (value == 1 || value == 2) {
            g_Unknown998C70[index] |= 16 << bit;
        } else if (value == 4 || value == 0) {
            g_Unknown998C70[index] &= ~(16 << bit);
        }
        break;
    }
}

// 0x006DD7C8
void BsStrcMgr::SetUnknown998C70Bit(u32 index, u32 bit)
{
    g_Unknown998C70[index] |= 16 << bit;
}

// 0x006DCC34
bool BsStrcMgr::Func6DCC34()
{
    const SvFgName* items = GetGardenPlus()->data.townData.unk_0xEE2C.campgroundShopItems;
    if (items != NULL && GetGardenPlus()->data.townData.unk_0xEE2C.campgroundCampingCar[1] == SvFgName::NAME_LYCHEE) {
        return false;
    }
    const u8* player = static_cast<const u8*>(g_UnknownAAFF18.unk234);
    if (player == NULL) {
        return false;
    }
    if (netgame::IsMultiPlayer()) {
        return false;
    }
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return false;
    }
    if ((player[0x5706] & 1) == 0) {
        return false;
    }
    const u8* town = reinterpret_cast<const u8*>(GetGardenData()) + 0x6214F;
    if (town[11] >> 7) {
        return false;
    }
    if (!Func2FD624(reinterpret_cast<const u8*>(GetGardenPlus()) + 0x80 + 0x6214F + 99, 4)) {
        return false;
    }
    if (Func630854()) {
        return false;
    }
    if (Func630428()) {
        return false;
    }
    const s32 hour = time::GetCurrentDate()->hour;
    const u8 flags = (reinterpret_cast<const u8*>(GetGardenPlus()) + 0x80)[0x6214F];
    if (flags & 2) {
        return hour >= 19;
    }
    if (flags & 4) {
        return static_cast<u32>(hour - 2) >= 19;
    }
    return hour >= 21;
}

// 0x006DD968
f32 BsStrcMgr::Func6DD968(u8 kind)
{
    for (AcStrc** it = m_Strcs.begin(); it != m_Strcs.end(); it++) {
        if (Func1500B0(*it) == kind) {
            return Func1503F8(*it);
        }
    }
    for (Actor** it = m_PlayerHouses.begin(); it != m_PlayerHouses.end(); it++) {
        if (Func1500B0(static_cast<AcStrc*>(*it)) == kind) {
            return Func1503F8(static_cast<AcStrc*>(*it));
        }
    }
    return -1.0f;
}

// 0x006DDC18
void BsStrcMgr::Func6DDC18()
{
    if (m_pTownPlaza != NULL) {
        Func150400(m_pTownPlaza);
    }
}

// 0x006DDC34
void BsStrcMgr::GetUnknown998D68(nn::math::VEC3* out)
{
    *out = g_Unknown998D68;
}

// 0x006DDA04
void BsStrcMgr::PlaceStrc(s32 x, s32 z, u8 kind)
{
    if (m_Unknown67BC.size() != 0) {
        return;
    }
    if (!Func5CAF84(kind) && kind != 76 && kind != 77 && kind != 78 && kind != 79 && kind != 0 && kind != 1 &&
        kind != 2 && kind != 3) {
        return;
    }
    sead::ScopedCurrentHeapSetter setter(m_Allocator.m_pHeap);
    nn::math::VEC3 position;
    GetStrcPosition(&position, x, z);
    Actor* actor = static_cast<Actor*>(
        Actor::Create(static_cast<ProcName>(Func5CA378(kind)), this, kind | 0x80000000, &position, NULL));
    if (actor != NULL) {
        m_Unknown67BC.pushBack(actor);
    }
    Func6CBFB8(x, z, kind);
    if (IsPlayerHouseStructure(kind)) {
        Func6CCB88(x, z, kind);
    }
}

// 0x006DDB48
void BsStrcMgr::RemovePlacedStrc()
{
    if (m_Unknown67BC.size() != 1) {
        return;
    }
    s32 x, z;
    FdInfo::ToUnit(x, z, static_cast<const nn::math::VEC3&>(m_Unknown67BC.at(0)->m_Position));
    Func6CC320(x, z, Func1500B0(static_cast<AcStrc*>(m_Unknown67BC.size() > 0 ? m_Unknown67BC.at(0) : NULL)));
    if (IsPlayerHouseStructure(Func1500B0(static_cast<AcStrc*>(m_Unknown67BC.size() > 0 ? m_Unknown67BC.at(0) : NULL)))) {
        Func6CCED8(x, z, Func1500B0(static_cast<AcStrc*>(m_Unknown67BC.size() > 0 ? m_Unknown67BC.at(0) : NULL)));
    }
    (m_Unknown67BC.size() > 0 ? m_Unknown67BC.at(0) : NULL)->Stop();
    if (m_Unknown67BC.size() > 0) {
        m_Unknown67BC.mPtrNum--;
    }
}

// 0x006DDDE4
const char* GetLeifName()
{
    const char* const names[6] = {"garden_grs", "rebuilding_00", "garden", "garden_hc", "dummy", "dummy"};
    const SvTownDataUnkEE2C* shops = &GetGardenPlus()->data.townData.unk_0xEE2C;
    if (!Func71D010(&shops->unk_0x132C0)) {
        return names[shops->leifUnlockStatus];
    }
    return shops->leifUnlockStatus == 1 ? names[1] : names[5];
}

// 0x006DDE48
const char* GetKicksName()
{
    const char* const names[3] = {"shoeshop_grs", "rebuilding_00", "shoeshop"};
    return names[GetGardenPlus()->data.townData.unk_0xEE2C.kickUnlockStatus];
}

// 0x006DDE84
void BsStrcMgr::ClearUnknown998C70Head()
{
    std::memset(g_Unknown998C70, 0, 10);
}

// 0x006DDE90
void BsStrcMgr::Func6DDE90()
{
    if (m_pTownPlaza != NULL) {
        Func150408(m_pTownPlaza);
    }
}

// 0x006DDEAC
void BsStrcMgr::GetUnknown998D80(nn::math::VEC3* out)
{
    *out = g_Unknown998D80;
}

// 0x006DDED0
void BsStrcMgr::GetUnknown998D74(nn::math::VEC3* out)
{
    *out = g_Unknown998D74;
}

// 0x006DDC58
AcStrc* BsStrcMgr::FindStrcOfKind(u8 kind)
{
    for (Actor** it = m_PlayerHouses.begin(); it != m_PlayerHouses.end(); it++) {
        if (Func1500B0(static_cast<AcStrc*>(*it)) == kind) {
            return static_cast<AcStrc*>(*it);
        }
    }
    for (Actor** it = m_Unknown6774.begin(); it != m_Unknown6774.end(); it++) {
        if (Func1500B0(static_cast<AcStrc*>(*it)) == kind) {
            return static_cast<AcStrc*>(*it);
        }
    }
    for (Actor** it = m_Unknown68C0.begin(); it != m_Unknown68C0.end(); it++) {
        if (Func1500B0(static_cast<AcStrc*>(*it)) == kind) {
            return static_cast<AcStrc*>(*it);
        }
    }
    // (in m_Strcs only the kind 105)
    for (AcStrc** it = m_Strcs.begin(); it != m_Strcs.end(); it++) {
        if (kind == STRUCTURE_ISLAND_HUT && Func1500B0(*it) == STRUCTURE_ISLAND_HUT) {
            return *it;
        }
    }
    for (Actor** it = m_Unknown67A8.begin(); it != m_Unknown67A8.end(); it++) {
        if (Func1500B0(static_cast<AcStrc*>(*it)) == kind) {
            return static_cast<AcStrc*>(*it);
        }
    }
    for (Actor** it = m_Unknown6A6C.begin(); it != m_Unknown6A6C.end(); it++) {
        if (Func1500B0(static_cast<AcStrc*>(*it)) == kind) {
            return static_cast<AcStrc*>(*it);
        }
    }
    return NULL;
}

// the byte +0x89 of the structure of an object index in m_Unknown68C0 or m_Strcs (kinds of Func5CAF84; inline)
inline void BsStrcMgr::SetStrcFlag89(s32 index, u8 value)
{
    for (Actor** it = m_Unknown68C0.begin(); it != m_Unknown68C0.end(); it++) {
        if (GetStrcObjectIndex(*it) == index) {
            if (Func5CAF84(Func1500B0(static_cast<AcStrc*>(*it)))) {
                reinterpret_cast<u8*>(*it)[0x89] = value;
            }
            return;
        }
    }
    for (AcStrc** it = m_Strcs.begin(); it != m_Strcs.end(); it++) {
        if (GetStrcObjectIndex(*it) == index) {
            if (Func5CAF84(Func1500B0(*it))) {
                reinterpret_cast<u8*>(*it)[0x89] = value;
            }
            return;
        }
    }
}

// 0x006DDEF4
void BsStrcMgr::ClearStrcFlag89(s32 index)
{
    SetStrcFlag89(index, 0);
}

// 0x006DDFC0
void BsStrcMgr::SetStrcFlag89(s32 index)
{
    SetStrcFlag89(index, 1);
}

// 0x006DF300
AcStrc* BsStrcMgr::FindStrcById(u32 id)
{
    for (Actor** it = m_PlayerHouses.begin(); it != m_PlayerHouses.end(); it++) {
        if (Func1500B0(static_cast<AcStrc*>(*it)) == id) {
            return static_cast<AcStrc*>(*it);
        }
    }
    for (Actor** it = m_Unknown6774.begin(); it != m_Unknown6774.end(); it++) {
        if (static_cast<AcStrc*>(*it)->GetId() == id) {
            return static_cast<AcStrc*>(*it);
        }
    }
    for (Actor** it = m_Unknown68C0.begin(); it != m_Unknown68C0.end(); it++) {
        if (static_cast<AcStrc*>(*it)->GetId() == id) {
            return static_cast<AcStrc*>(*it);
        }
    }
    for (AcStrc** it = m_Strcs.begin(); it != m_Strcs.end(); it++) {
        if ((*it)->GetId() == id) {
            return *it;
        }
    }
    return NULL;
}

// 0x006DF480
void BsStrcMgr::RemoveStrcAt(s32 index)
{
    for (Actor** it = m_Unknown68C0.begin(); it != m_Unknown68C0.end(); it++) {
        if (GetStrcObjectIndex(*it) == index) {
            if (!Func5CAF84(Func1500B0(static_cast<AcStrc*>(*it)))) {
                return;
            }
            Actor* actor = *it;
            s32 position = -1;
            for (s32 i = 0; i < m_Unknown68C0.size(); i++) {
                if (m_Unknown68C0.at(i) == actor) {
                    position = i;
                    break;
                }
            }
            actor->Stop();
            m_Unknown68C0.erase(position, 1);
            return;
        }
    }
    for (AcStrc** it = m_Strcs.begin(); it != m_Strcs.end(); it++) {
        if (GetStrcObjectIndex(*it) == index) {
            if (!Func5CAF84(Func1500B0(*it))) {
                return;
            }
            reinterpret_cast<u8*>(*it)[0x89] = 0;
            (*it)->vf_0x98();
            return;
        }
    }
}

// 0x006E0960
bool IsAloneInRoom()
{
    if (Func4E2B6C() || Func4E2B7C()) {
        return false;
    }
    if (BsNpcMgr::s_pInstance != NULL) {
        // (the collector is made but not filled)
        BsNpcMgr::NpcPosGetCB positions(NULL);
        if (positions.GetNum() > 0) {
            return false;
        }
    }
    // the players in the room: the bits 4.. of the byte of the house in g_Unknown998C70
    u32 index;
    switch (stage::GetStructure(stage::GetCurrent())) {
    case STRUCTURE_PLAYER_1_HOUSE:
        index = 10;
        break;
    case STRUCTURE_PLAYER_2_HOUSE:
        index = 11;
        break;
    case STRUCTURE_PLAYER_3_HOUSE:
        index = 12;
        break;
    case STRUCTURE_PLAYER_4_HOUSE:
        index = 13;
        break;
    case STRUCTURE_MUSEUM:
        index = 14;
        break;
    default:
        return true;
    }
    u32 bits = g_Unknown998C70[index] >> 4;
    u32 num = 0;
    while (bits != 0) {
        if (bits & 1) {
            num++;
        }
        bits >>= 1;
    }
    return num <= 1;
}

// 0x006E0A74
void BsStrcMgr::Func6E0A74(s32 x, s32 z, u8 kind)
{
    if (m_Unknown67BC.size() != 0 || m_Unknown6B50 == 0) {
        // (not now: Calc tries again)
        m_Unknown6B51 = true;
        m_Unknown6B54 = x;
        m_Unknown6B58 = z;
        m_Unknown6B5C = kind;
        return;
    }
    if (Func5CAF84(kind) || IsPlayerHouseStructure(kind) || kind == STRUCTURE_POLICE_STATION_MODERN ||
        kind == STRUCTURE_POLICE_STATION_CLASSIC || kind == STRUCTURE_CAFE || kind == STRUCTURE_RESET_CENTER ||
        kind == STRUCTURE_CAMPING_CAR_1 || kind == STRUCTURE_CAMPING_CAR_2) {
        sead::ScopedCurrentHeapSetter setter(m_Allocator.m_pHeap);
        nn::math::VEC3 position;
        UnitToWorld(&position, x, z);
        if ((kind == STRUCTURE_CUSTOM_DESIGN_SIGN || kind == STRUCTURE_FACE_CUTOUT_STANDEE) &&
            !Func27A878(reinterpret_cast<u8*>(GetGardenData()) + 0x4BEF0, x, z)) {
            return;
        }
        position.y = FdInfo::GetBgHeight(position, FIELD_NAME_CURRENT);
        Actor* actor = static_cast<Actor*>(
            Actor::Create(static_cast<ProcName>(Func5CA378(kind)), this, kind | 0x40000000, &position, NULL));
        if (actor != NULL) {
            AcStrc* strc = static_cast<AcStrc*>(actor);
            if (IsPlayerHouseStructure(Func1500B0(strc))) {
                if (Func150420(strc)) {
                    m_PlayerHouses.pushBack(actor);
                } else {
                    m_Strcs.pushBack(strc);
                }
            } else if (Func5CB000(Func1500B0(strc))) {
                m_Unknown68C0.pushBack(actor);
            }
            m_Unknown67BC.pushBack(actor);
            m_pUnknown6B90 = actor;
        }
        if (IsPlayerHouseStructure(kind)) {
            Func6CD1A4(x, z, kind);
        }
        if (IsPlayerHouseStructure(kind)) {
            Func242E44(x, z, kind);
        } else {
            Func2425D8(x, z, kind);
        }
        Func242D00();
        if (IsPlayerHouseStructure(kind)) {
            // (the area of the house on the field)
            const s8* area = Func5CAD04(kind);
            const Vector2i min(area[0], area[1]);
            const Vector2i max(area[2], area[3]);
            const Vector2i offset(x, z);
            FieldRect rect(min, max);
            rect += offset;
            Func6FB77C(&rect, true);
        }
    }
    m_Unknown6B51 = false;
    m_Unknown6B54 = -1;
    m_Unknown6B58 = -1;
    m_Unknown6B5C = STRUCTURE_EMPTY_SPACE;
}

namespace {
// the distance of the units of a structure to a unit (x and z added)
inline s32 GetUnitDistance(AcStrc* strc, s32 x, s32 z)
{
    s32 strcX, strcZ;
    FdInfo::ToUnit(strcX, strcZ, *Func150428(strc));
    s32 dx = x - strcX;
    if (dx < 0) {
        dx = -dx;
    }
    s32 dz = z - strcZ;
    if (dz < 0) {
        dz = -dz;
    }
    return dx + dz;
}
} // namespace

// the nearest structure of an array (with height: only within 32 in y; inline)
template <typename T>
inline void BsStrcMgr::FindNearestStrcIn(T* const* begin, T* const* end, s32 x, s32 z, const nn::math::VEC3* position,
                                         bool height, AcStrc** nearest, s32* distance)
{
    for (T* const* it = begin; it != end; it++) {
        AcStrc* strc = static_cast<AcStrc*>(*it);
        const s32 d = GetUnitDistance(strc, x, z);
        if (height) {
            if (d < *distance && std::fabs(Func150428(strc)->y - position->y) < 32.0f) {
                *nearest = strc;
                *distance = d;
            }
        } else if (d < *distance) {
            *nearest = strc;
            *distance = d;
        }
    }
}

// 0x006DEEE8
AcStrc* BsStrcMgr::FindNearestStrc(const nn::math::VEC3* position)
{
    s32 x = 0;
    s32 z = 0;
    FdInfo::ToUnit(x, z, *position);
    AcStrc* nearest = NULL;
    s32 distance = 10000;
    FindNearestStrcIn(m_PlayerHouses.begin(), m_PlayerHouses.end(), x, z, position, false, &nearest, &distance);
    FindNearestStrcIn(m_Unknown6774.begin(), m_Unknown6774.end(), x, z, position, false, &nearest, &distance);
    FindNearestStrcIn(m_Unknown68C0.begin(), m_Unknown68C0.end(), x, z, position, true, &nearest, &distance);
    FindNearestStrcIn(m_Strcs.begin(), m_Strcs.end(), x, z, position, true, &nearest, &distance);
    FindNearestStrcIn(m_Unknown67A8.begin(), m_Unknown67A8.end(), x, z, position, false, &nearest, &distance);
    FindNearestStrcIn(m_Unknown6A6C.begin(), m_Unknown6A6C.end(), x, z, position, false, &nearest, &distance);
    return nearest;
}

namespace {
// the kinds of doors of GetDoorSounds (names are ours): the house and villager values 0..21 select wood, steel or
// trick doors (meaning of the single values unknown), the others are set for stages
const u8 DOOR_AUTOMATIC = 30;
const u8 DOOR_NONE = 40;
const u8 DOOR_DEPARTMENT_STORE = 50;
const u8 DOOR_TENT = 100;
const u8 DOOR_UNKNOWN = 255;

// no sound
const u32 DOOR_SOUND_NONE = 0xFFFFFFFF;

// the structures of the villager houses
const u32 VILLAGER_HOUSE_FIRST = 8;
const u32 VILLAGER_HOUSE_LAST = 17;

// (inline) the entry of a villager in the table of the characters (NULL: none)
inline const u8* GetVillagerEntry(u16 villagerId)
{
    if (event::g_UnknownA84AEC.table == NULL || villagerId >= 399) {
        return NULL;
    }
    return event::g_UnknownA84AEC.table + villagerId * 34;
}
} // namespace

// 0x006DCD98
bool BsStrcMgr::GetDoorSounds(u32* knock, u32* open, u32* close, u32* melody, bool* flag)
{
    const stage::Name current = stage::GetCurrent();
    u32 melodySound = SE_MELODY_ONESEQ_DOOR_HOME;
    bool isFlag = false;
    u8 door = DOOR_UNKNOWN;
    *knock = 1;
    // the house of a player
    for (u32 i = STRUCTURE_PLAYER_1_HOUSE; i <= STRUCTURE_PLAYER_4_HOUSE; i++) {
        if (GetInsideStageOfStructure(i) != current) {
            continue;
        }
        SvPlayerHouseList* houses = &GetGardenData()->townData.playerHouses;
        door = Func3180C8(houses, i)->exterior1.houseBrick;
        if (Func3180C8(houses, i)->exterior1.houseSize == 0) {
            // (the tent)
            isFlag = true;
            melodySound = DOOR_SOUND_NONE;
            door = DOOR_TENT;
        }
    }
    // the house of a villager
    for (u32 i = VILLAGER_HOUSE_FIRST; i <= VILLAGER_HOUSE_LAST; i++) {
        if (GetInsideStageOfStructure(i) != current) {
            continue;
        }
        SvNpc* npc = static_cast<SvNpc*>(event::Func300928(GetGardenPlus()->data.npcData.npcs, i - VILLAGER_HOUSE_FIRST));
        if (npc == NULL) {
            continue;
        }
        const u8* entry = GetVillagerEntry(npc->npc.villagerId);
        if (entry != NULL) {
            door = entry[8];
        }
    }
    // the stages with their own doors (no knocking)
    switch (current) {
    case stage::NAME_TAILOR:
        melodySound = SE_MELODY_ONESEQ_DOOR_ABLE;
        door = 0;
        *knock = DOOR_SOUND_NONE;
        break;
    case stage::NAME_FORTUNE:
        melodySound = SE_MELODY_ONESEQ_DOOR_FORTUNE;
        door = 4;
        *knock = DOOR_SOUND_NONE;
        break;
    case stage::NAME_OFFICE:
        melodySound = SE_MELODY_ONESEQ_DOOR_PUB;
        door = 0;
        *knock = DOOR_SOUND_NONE;
        break;
    case stage::NAME_BEAUTY_SALON:
        door = DOOR_AUTOMATIC;
        melodySound = DOOR_SOUND_NONE;
        *knock = DOOR_SOUND_NONE;
        break;
    case stage::NAME_RECYCLE_SHOP:
        melodySound = SE_MELODY_ONESEQ_DOOR_RECYCLE;
        door = 0;
        *knock = DOOR_SOUND_NONE;
        break;
    case stage::NAME_CAFE:
    case stage::NAME_CAFE_ARBEIT:
        melodySound = SE_MELODY_ONESEQ_DOOR_CAFE;
        door = 0;
        *knock = DOOR_SOUND_NONE;
        break;
    case stage::NAME_SHOP:
        melodySound = SE_MELODY_ONESEQ_DOOR_SHOP;
        door = 0;
        *knock = DOOR_SOUND_NONE;
        break;
    case stage::NAME_CONVENIENCE_STORE:
        melodySound = SE_MELODY_ONESEQ_DOOR_CONVENI;
        door = DOOR_AUTOMATIC;
        *knock = DOOR_SOUND_NONE;
        isFlag = true;
        break;
    case stage::NAME_SUPERMARKET:
        door = DOOR_AUTOMATIC;
        melodySound = DOOR_SOUND_NONE;
        *knock = DOOR_SOUND_NONE;
        isFlag = true;
        break;
    case stage::NAME_HOME_CENTER:
    case stage::NAME_FLORISTS_HOME_IMPROVEMENT_CENTER:
        door = DOOR_AUTOMATIC;
        melodySound = DOOR_SOUND_NONE;
        isFlag = true;
        *knock = DOOR_SOUND_NONE;
        break;
    case stage::NAME_DEPARTMENT_STORE_1F:
        door = DOOR_DEPARTMENT_STORE;
        melodySound = DOOR_SOUND_NONE;
        *knock = DOOR_SOUND_NONE;
        break;
    case stage::NAME_FLORISTS:
        melodySound = SE_MELODY_ONESEQ_DOOR_GARDEN;
        door = 0;
        *knock = DOOR_SOUND_NONE;
        break;
    case stage::NAME_SHOE_SHOP:
        melodySound = SE_MELODY_ONESEQ_DOOR_SHOESHOP;
        door = 0;
        *knock = DOOR_SOUND_NONE;
        break;
    case stage::NAME_CLUB444:
    case stage::NAME_CLUB444_DJKK:
    case stage::NAME_CLUB444_LIVE:
        melodySound = SE_MELODY_ONESEQ_DOOR_CLUB;
        door = 2;
        *knock = DOOR_SOUND_NONE;
        break;
    case stage::NAME_DREAM:
        melodySound = SE_MELODY_ONESEQ_DOOR_CASE;
        door = 0;
        *knock = DOOR_SOUND_NONE;
        break;
    case stage::NAME_POST_OFFICE:
        melodySound = SE_MELODY_ONESEQ_DOOR_POST;
        door = 0;
        *knock = DOOR_SOUND_NONE;
        break;
    case stage::NAME_REAL_ESTATE_AGENCY:
        melodySound = SE_MELODY_ONESEQ_DOOR_ESTATE;
        door = 0;
        *knock = DOOR_SOUND_NONE;
        break;
    case stage::NAME_ACCESSORY_SHOP:
        melodySound = SE_MELODY_ONESEQ_DOOR_ACCESHOP;
        door = 0;
        *knock = DOOR_SOUND_NONE;
        break;
    case stage::NAME_STATION:
    case stage::NAME_INSECT_AWARD_TENT:
    case stage::NAME_FISH_AWARD_TENT:
    case stage::NAME_POLICE_BOX_A:
    case stage::NAME_POLICE_BOX_B:
    case stage::NAME_MUSEUM_ENTRANCE:
    case stage::NAME_TOUR_DESK:
        door = DOOR_NONE;
        melodySound = DOOR_SOUND_NONE;
        *knock = DOOR_SOUND_NONE;
        break;
    default:
        break;
    }
    *melody = melodySound;
    *flag = isFlag;
    if (door == DOOR_UNKNOWN) {
        return false;
    }
    switch (door) {
    case 0:
    case 1:
    case 3:
    case 6:
    case 9:
    case 10:
    case 11:
    case 12:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 20:
        if (*knock != DOOR_SOUND_NONE) {
            *knock = SE_OBJ_DOOR_WOOD_KNOCKED;
        }
        *open = SE_OBJ_DOOR_WOOD_OPEN;
        *close = SE_OBJ_DOOR_WOOD_CLOSE;
        break;
    case 2:
    case 5:
    case 21:
        if (*knock != DOOR_SOUND_NONE) {
            *knock = SE_OBJ_DOOR_STEEL_KNOCKED;
        }
        *open = SE_OBJ_DOOR_STEEL_OPEN;
        *close = SE_OBJ_DOOR_STEEL_CLOSE;
        break;
    case DOOR_AUTOMATIC:
        *knock = DOOR_SOUND_NONE;
        *open = SE_OBJ_DOOR_AUTO_OPEN;
        *close = SE_OBJ_DOOR_AUTO_CLOSE;
        break;
    case DOOR_NONE:
        *knock = DOOR_SOUND_NONE;
        *open = DOOR_SOUND_NONE;
        *close = DOOR_SOUND_NONE;
        break;
    case DOOR_DEPARTMENT_STORE:
        *knock = DOOR_SOUND_NONE;
        *open = SE_OBJ_DOWNTOWN_DEPART_DOOR_OPEN;
        *close = DOOR_SOUND_NONE;
        break;
    case DOOR_TENT:
        *knock = DOOR_SOUND_NONE;
        *open = SE_OBJ_DOOR_TENT_OPEN;
        *close = SE_OBJ_DOOR_TENT_CLOSE;
        break;
    default:
        if (*knock != DOOR_SOUND_NONE) {
            *knock = SE_OBJ_DOOR_TRICK_KNOCKED;
        }
        *open = SE_OBJ_DOOR_TRICK_OPEN;
        *close = SE_OBJ_DOOR_TRICK_CLOSE;
        break;
    }
    return true;
}
