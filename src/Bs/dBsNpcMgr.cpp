#include "Bs/dBsNpcMgr.h"
#include "script/dStrMgr.h"
#include "Ac/dAcNpc.h"
#include "Bs/dBsNpcMgrExtern.h"
#include "Bs/dBsSnowBallMgr.h"
#include "Bs/dBsNpcMgr_NpcPosGetCB.h"
#include "Bs/dBsNpcMgr_SearchFunction.h"
#include "Bank/bank_Extern.h"
#include "event/event_Api.h"
#include "event/event_Extern.h"
#include "field/dFdInfo.h"
#include "g3d/g3d_System.h"
#include "netgame/netgame_Api.h"
#include "Npc/dNpcOnBuildFunc.h"
#include "Npc/dNpcOnPreviewEndFunc.h"
#include "Npc/dNpcOnPreviewFunc.h"
#include "oml/framework/dProcess.h"
#include "Other/dActor.h"
#include "Other/dHeapInit.h"
#include "Other/dProcMgr.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvGardenPlus.h"
#include "time/time_Api.h"
#include "field/dField.h"
#include "Item/item_Api.h"
#include "math/dRandom.h"
#include "sead/seadRandom.h"
#include "netgame/dGameFunctions.h"
#include "Sound/sound_Extern.h"
#include "stage/stage_Data.h"
#include "stage/stage_Extern.h"
#include "Other/dFdBkSearchCand.h"
#include "Search/dSearchCand.h"
#include "Other/dEventSecretarySearchFunc.h"
#include "Other/dForeachOnMoveOutPitfall.h"
#include "Other/dForeachOnStrugglePitfall.h"
#include "demo/demo_Api.h"
#include "time/time_Extern.h"
#include "sead/seadMathCalcCommon.h"
#include "Bs/dBsSvDemo.h"
#include "Other/dMailLoadAdapt.h"
#include "Other/dForeachOnFallToPitfall.h"
#include "Other/dForeachOnPlayerStartEmoticon.h"
#include "ssys/ma/ma_Math.h"
#include "Search/dSearchCandXZ.h"
#include "Sound/sound_Extern.h"
#include "Other/dBkUnitSearchCandCb.h"
#include "Other/dForeachOnPlayerEndEmoticon.h"
#include "Sv/dSvFgBlock.h"
#include "Sv/dSvInsectConventionExtern.h"
#include "Sv/dSvPlayer.h"
#include "Sv/dSvProcExtern.h"
#include "field/field_Extern.h"
#include "bg/bg_Extern.h"
#include <new>

// the globals of the file (names are ours)
// 0x00951140
BsNpcMgr::State BsNpcMgr::s_State = {
    stage::NAME_MAX, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xFC, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
// 0x00951158
BsNpcMgr* BsNpcMgr::s_pInstance;
// 0x0095115C
u32 BsNpcMgr::s_Unknown95115C;
// 0x00951160
u32 BsNpcMgr::s_CounterPeriod;
// 0x00951164
u32 BsNpcMgr::s_Counter;
// 0x00951180
u32 BsNpcMgr::s_Unknown951180;

// (the blob initializer 0x0078676C writes the process names)
// 0x009511A4
f32 BsNpcMgr::s_Unknown9511A4 = static_cast<f32>(static_cast<u16>(FdInfo::UNIT_WORLD_SIZE_XF * 0.2f));
// 0x009511B0
const BsNpcMgr::PlaceFunc BsNpcMgr::s_PlaceFuncs[8] = {
    &Func4DC498, &Func4DCBB0, &Func4DC9A4, &Func4DCED8, &Func4DCE84, &Func4DCAF4, &Func10BCDC, &Func10BB3C,
};
// 0x009511D0
BsNpcMgr::VisitorNpc BsNpcMgr::s_VisitorNpcs[VISITOR_NUM] = {
    {1, 0, PROC_AC_NPC_SP_KABURIBA, "boa", 1, true},
    {2, 0, PROC_AC_NPC_SP_GRACE_OUT, "grf", 2, true},
    {3, 0, PROC_AC_NPC_SP_JONNY, "seg", 0, true},
    {5, 0, PROC_AC_NPC_SP_ROLLAN, "cml", 1, false},
    {6, 0, PROC_AC_NPC_SP_PERIO_NORMAL, "plb", 1, false},
};
// 0x0095120C
BsNpcMgr::EventNpc BsNpcMgr::s_EventNpcs[EVENT_NPC_NUM] = {
    {event::EVENT_ID_TOY_DAY, 0, PROC_AC_NPC_SP_JINGLE, 1, "snt", 1},
    {event::EVENT_ID_HARVEST_FESTIVAL, 0, PROC_AC_NPC_SP_FRANKRIN, 1, "tuk", 2},
    {event::EVENT_ID_HALLOWEEN, 0, PROC_AC_NPC_SP_PUMPKING, 1, "pkn", 1},
    {event::EVENT_ID_BUNNY_DAY, 0, PROC_AC_NPC_SP_PYONTAROU, 1, "pyn", 2},
    {event::EVENT_ID_COUNTDOWN, 0, PROC_AC_NPC_SP_TUNEKICHI_EVENT, 1, "fob", 3},
    {event::EVENT_ID_FIREWORKS_SHOW, 0, PROC_AC_NPC_SP_TUNEKICHI_EVENT, 1, "fob", 3},
    {event::EVENT_ID_WEEDING_DAY, 0, PROC_AC_NPC_SP_CLEANING, 0, "slo", 2},
    {event::EVENT_ID_FISHING_TOURNEY, 0, PROC_AC_NPC_SP_UOMASA, 1, "bev", 2},
    {event::EVENT_ID_BUG_OFF, 0, PROC_AC_NPC_SP_KAMEYAMA, 1, "chm", 2},
    {event::EVENT_ID_FESTIVALE, 0, PROC_AC_NPC_SP_BERLINA, 1, "pck", 4},
};
// the guest of the cafe (index into s_CafeGuests, -1: none)
// 0x00951168
u32 BsNpcMgr::s_Unknown951168;
// 0x0095116C
s32 BsNpcMgr::s_CafeGuest = -1;
// 0x00951170
s32 BsNpcMgr::s_AsrWordId;
// 0x00951174
s32 BsNpcMgr::s_AsrWordListSize;
// the numbers of words of the two answers in the Reset Centre
// 0x00951178
s32 BsNpcMgr::s_AsrResetWordNum[2];
// the bounds of the free floor of the room (BuildRoomFloor)
// 0x00951194
s32 BsNpcMgr::s_RoomMinX;
// 0x00951198
s32 BsNpcMgr::s_RoomMinZ;
// 0x0095119C
s32 BsNpcMgr::s_RoomMaxX;
// 0x009511A0
s32 BsNpcMgr::s_RoomMaxZ;

namespace {
// the accessories of the seasons (BsNpcMgr::SetupAccessory)
// 0x00854E4A
const AccessoryEntry s_AccessoryEntries[2] = {
    {event::EVENT_ID_COUNTDOWN, 1, 0},
    {event::EVENT_ID_FESTIVALE, 1, 1},
};
} // namespace

// 0x006A27F0
BsNpcMgr::BsNpcMgr()
    : m_Level(0.0f), m_UnknownF11C(-1), m_Frame(0), m_UnknownF124(0), m_UnknownF125(1),
      m_AccessoryKind(ACCESSORY_NONE), m_UnknownF12E(0)
{
    const u32 num = stage::GetCurrent() == stage::NAME_AMIIBO_CAMERA ? 14 : 10;
    m_Npcs.first = m_NpcBuffer;
    m_Npcs.last = m_NpcBuffer;
    m_Npcs.capacity = num;
    m_LoadFlags.m_Bits = 0;
    g_Unknown994484.SetLimits(16, 16, 83, 83);
}

// 0x006A266C slot 0x30
oml::framework::Result BsNpcMgr::Draw()
{
    return oml::framework::RESULT_SUCCESS;
}

// 0x006A2674 (name is ours)
void BsNpcMgr::ClearState()
{
    s_Unknown951180 = 0;
    s_Unknown95115C = 0;
    s_State.unk00 = stage::NAME_MAX;
    s_State.unk01 = 0;
    s_State.unk02 = 0;
    s_State.unk03 = 0;
    s_State.unk04 = 0;
    s_State.unk06 = 0;
    s_State.unk07 = 0;
    s_State.unk08 = 0;
    s_State.unk09 = 0;
    s_State.unk0A = 0;
    s_State.unk05 = 0;
    s_State.unk0D = 0xFC;
    s_State.unk0C = 0;
    s_State.unk0E = 0;
    s_State.unk13 = 0;
    s_State.unk14 = 0;
    s_State.unk15 = 0;
    s_State.unk0B = 0;
    s_State.unk0F = 0;
    s_State.unk16 = 0;
    s_State.unk17 = 0;
}

// 0x006A26E8 slot 0x24
oml::framework::Result BsNpcMgr::Calc()
{
    if (m_Timer != 0) {
        m_Timer--;
    }
    s_State.unk10 = 0;
    if (netgame::IsStageControl()) {
        CalcStageControl();
        m_Frame++;
        if (m_Frame >= 10) {
            m_Frame = 0;
        }
    }
    m_Level *= 0.9f;
    s_Counter++;
    if (s_Counter >= s_CounterPeriod) {
        s_Counter = 0;
    }
    return oml::framework::RESULT_SUCCESS;
}

// 0x006962E0
AcNpc* BsNpcMgr::FindNpc(const stage::NpcInfo* info)
{
    SearchFunction func(info);
    ForeachImpl(func);
    return func.m_pResult;
}

// 0x006969B0
AcNpc* BsNpcMgr::FindNpcByModel(const char* model)
{
    stage::NpcInfo info(stage::NpcInfo::KIND_MODEL, 0, model);
    SearchFunction func(&info);
    ForeachImpl(func);
    return func.m_pResult;
}

// 0x006A2778
s32 BsNpcMgr::PassToNpc(u32 actorId, const void* arg)
{
    for (AcNpc** it = m_Npcs.first; it != m_Npcs.last; it++) {
        AcNpc* npc = *it;
        if (npc != NULL && npc->GetId() == actorId) {
            return npc->vf_0xE8(arg);
        }
    }
    return 0;
}

// 0x006963CC
bool BsNpcMgr::PassToNpcOfKind(u8 kind, const void* arg)
{
    for (AcNpc** it = m_Npcs.first; it != m_Npcs.last; it++) {
        AcNpc* npc = *it;
        if (npc != NULL && npc->m_Kind == kind) {
            return npc->vf_0xEC(arg);
        }
    }
    return false;
}

// 0x00696274
void BsNpcMgr::RemoveNpc(AcNpc* npc)
{
    const s32 num = m_Npcs.GetSize();
    AcNpc** it = m_Npcs.first;
    for (s32 i = 0; i < num; i++, it++) {
        if (*it == npc) {
            *it = NULL;
            return;
        }
    }
}

// 0x00696124
void BsNpcMgr::OnBuild(s32 x0, s32 z0, s32 x1, s32 z1, u8 flag)
{
    NpcOnBuildFunc func(x0, z0, x1, z1, flag);
    ForeachImpl(func);
    if (BsSnowBallMgr::s_pInstance != NULL) {
        BsSnowBallMgr::s_pInstance->OnBuild(x0, z0, x1, z1, flag);
    }
}

// 0x00696438
void BsNpcMgr::OnPreview(s32 x0, s32 z0, s32 x1, s32 z1, u8 flag)
{
    NpcOnPreviewFunc func(x0, z0, x1, z1, flag);
    ForeachImpl(func);
    if (BsSnowBallMgr::s_pInstance != NULL) {
        BsSnowBallMgr::s_pInstance->OnPreview(x0, z0, x1, z1, flag);
    }
}

// 0x00697BCC
void BsNpcMgr::OnPreviewEnd()
{
    NpcOnPreviewEndFunc func;
    ForeachImpl(func);
    if (BsSnowBallMgr::s_pInstance != NULL) {
        BsSnowBallMgr::s_pInstance->OnPreviewEnd();
    }
}

// 0x006967BC
bool BsNpcMgr::CreateSpecialNpcs(const stage::Command* command)
{
    if (!PrepareBanksImpl(command)) {
        return false;
    }
    const stage::NpcEntry* entries = static_cast<const stage::NpcEntry*>(command->data);
    for (s32 i = 0; i < command->count; i++) {
        const stage::NpcEntry& entry = entries[i];
        if (entry.name == PROC_NAME_NUM) {
            m_Npcs.PushBackFast(NULL);
            continue;
        }
        nn::math::VEC3_ position = entry.position;
        math::Angle16Vec3 rotation = entry.rotation;
        oml::framework::Process* npc = Actor::Create(static_cast<ProcName>(entry.name), this,
                                                     reinterpret_cast<u32>(&entry.info), &position, &rotation);
        if (npc == NULL) {
            return false;
        }
        m_Npcs.PushBackFast(static_cast<AcNpc*>(npc));
    }
    return true;
}

// 0x006A2590
bool BsNpcMgr::QueueNpc(u16 name, const stage::NpcInfo* info, u16 angleY, s32 x, s32 z)
{
    nn::math::VEC3 position;
    FdInfo::ToCenterWorldWithUnitHeight(&position, x, z);
    if (m_QueuedNpcs.IsFull()) {
        return false;
    }
    stage::NpcEntry entry;
    entry.name = name;
    entry.position = position;
    entry.rotation.x.value = 0;
    entry.rotation.y.value = angleY;
    entry.rotation.z.value = 0;
    entry.info = *info;
    m_QueuedNpcs.PushBackFast(entry);
    return true;
}

// 0x00696CA8
void BsNpcMgr::SwapQueuedNpcs(s32 a, s32 b)
{
    const s32 max = a < b ? b : a;
    if (static_cast<u32>(max) < m_QueuedNpcs.GetSize()) {
        const stage::NpcEntry entry = m_QueuedNpcs.first[a];
        m_QueuedNpcs.first[a] = m_QueuedNpcs.first[b];
        m_QueuedNpcs.first[b] = entry;
    }
}

// 0x00696D04
bool BsNpcMgr::CreateQueuedNpcs()
{
    // the module of the stage queues its NPCs (static.crs patches the module functions into the table; the original
    // copies it from 0x00854F6C)
    struct StageFunc
    {
        u8 stage;
        void (*func)();
    };
    const StageFunc funcs[22] = {
        {stage::NAME_SEA_DEPARTURE, QueueSeaNpcs},
        {stage::NAME_SEA_RETURN, QueueSeaNpcs},
        {stage::NAME_STATION_ENTRY_DEMO, QueueStationEntryNpcs},
        {stage::NAME_STATION_RETIRE_DEMO, QueueStationRetireNpcs},
        {stage::NAME_STATION_COME_HOME_DEMO, QueueStationComeHomeNpcs},
        {stage::NAME_STATION_PROLOGUE_DEMO, QueueStationPrologueNpcs},
        {stage::NAME_ISLAND_HIDE_AND_SEEK_RESULT, QueueTourResultNpcs},
        {stage::NAME_ISLAND_FLOWER_PICKING_RESULT, QueueTourResultNpcs},
        {stage::NAME_ISLAND_FISHING_RESULT, QueueTourResultNpcs},
        {stage::NAME_ISLAND_INSECT_CATCHING_RESULT, QueueTourResultNpcs},
        {stage::NAME_ISLAND_BALLOON_RESULT, QueueTourResultNpcs},
        {stage::NAME_ISLAND_TREASURE_HUNT_RESULT, QueueTourResultNpcs},
        {stage::NAME_ISLAND_PIKO_HAN_RESULT, QueueTourResultNpcs},
        {stage::NAME_ISLAND_MAZE_RESULT, QueueTourResultNpcs},
        {stage::NAME_ISLAND_MINING_RESULT, QueueTourResultNpcs},
        {stage::NAME_ISLAND_FOSSIL_DIG_RESULT, QueueTourResultNpcs},
        {stage::NAME_ISLAND_DIVING_RESULT, QueueTourResultNpcs},
        {stage::NAME_ISLAND_BORROWING_RACE_RESULT, QueueTourResultNpcs},
        {stage::NAME_FISH_AWARD_TENT_DEMO, QueueFishAwardNpcs},
        {stage::NAME_INSECT_AWARD_TENT_DEMO, QueueInsectAwardNpcs},
        {stage::NAME_CAFE_ARBEIT, QueueCafeArbeitNpcs},
        {stage::NAME_AMIIBO_CAMERA, QueueAmiiboCameraNpcs},
    };
    if (!stage::IsKind(stage::KIND_SCREEN, stage::GetCurrent())) {
        return false;
    }
    for (u32 i = 0; i < 22; i++) {
        if (stage::GetCurrent() == funcs[i].stage) {
            funcs[i].func();
        }
    }
    if (m_QueuedNpcs.last == m_QueuedNpcs.first) {
        return false;
    }
    stage::Command command = {3, 0, NULL};
    command.count = m_QueuedNpcs.GetSize();
    command.data = m_QueuedNpcs.first;
    CreateSpecialNpcs(&command);
    Func2C51B0();
    m_QueuedNpcs.Clear();
    return true;
}

// 0x00696B3C
void BsNpcMgr::SetupAccessory()
{
    m_AccessoryAllocator.CreateExpHeap(30720, HeapInit::s_pStageHeap, sead::SafeStringBase<char>("NPCAccResAllocator"),
                                       sead::Heap::HEAP_DIRECTION_FORWARD, false);
    for (u32 i = 0; i < 2; i++) {
        const AccessoryEntry& entry = s_AccessoryEntries[i];
        if ((entry.kind == 0 && Func309C6C(entry.eventOrDay, time::GetCurrentDate())) ||
            (entry.kind == 1 && event::IsActive(entry.eventOrDay))) {
            m_AccessoryKind = entry.accessory;
            break;
        }
    }
    // the birthday party of the player in the house of a villager
    if (stage::IsKind(stage::KIND_NPC_HOUSE, stage::GetCurrent()) && event::IsActive(event::EVENT_ID_PLAYER_BIRTHDAY)) {
        const s32 index = stage::GetNpcHouseIndex(stage::GetCurrent());
        void* npc = event::Func300928(GetGardenPlus()->data.npcData.npcs, index);
        if (npc != NULL && Func11A3A8(npc)) {
            m_AccessoryKind = 0;
        }
    }
    if (stage::IsKind(stage::KIND_MINI_GAME, stage::GetCurrent())) {
        m_AccessoryKind = 2;
    }
}

// 0x00696DEC
bool BsNpcMgr::LoadAccessory(u8 kind)
{
    if (kind == ACCESSORY_NONE || m_LoadFlags.Test(LOAD_ACCESSORY)) {
        return true;
    }
    if (!m_AccessoryLoader.LoadAsync(sead::SafeStringBase<char>(Func10BB30(kind)), m_AccessoryAllocator.m_pHeap,
                                     g3d::ResourceLoader::CGFX_ALIGNMENT)) {
        return false;
    }
    if (!m_LoadFlags.Test(LOAD_CMN)) {
        return false;
    }
    m_AccessoryLoader.InitializeCgfx(&m_AccessoryAllocator, NULL, true);
    m_AccessoryLoader.InitializeCgfx(&m_AccessoryAllocator, &m_CmnLoader, true);
    m_AccessoryLoader.InitializeCgfx(&m_AccessoryAllocator, Func536700(), true);
    m_AccessoryLoader.InitializeCgfx(&m_AccessoryAllocator, Func5C22D8(), true);
    const u32 num = m_AccessoryLoader.GetGraphicsFile().ptr()->models.count;
    for (u32 i = 0; i < num; i++) {
        g3d::SetLightSetIndex(static_cast<const nw::gfx::res::ResModelData*>(
                                  m_AccessoryLoader.GetGraphicsFile().GetModels(i)),
                              Func1E891C());
    }
    m_LoadFlags.Set(LOAD_ACCESSORY, true);
    m_AccessoryKind = kind;
    return true;
}

// the factory of the process table
// 0x007F34A8
template oml::framework::Process* oml::framework::Process::Create<BsNpcMgr>();

namespace {
// a unit of the block a villager may stand on: not more than 2 below the reference height and free (a callback of the
// unit searches; name is ours)
// 0x006961E0
bool IsStandableUnit(s32 blockX, s32 blockZ, s32 unitX, s32 unitZ, u32 /* arg */)
{
    s32 x;
    s32 z;
    FdInfo::ToUnit(x, z, blockX, blockZ, unitX, unitZ);
    const f32 limit = Func6FA0C4() - 2.0f;
    const bool isHigh = FdInfo::GetUnitHeight(x, z, FIELD_NAME_CURRENT) >= limit;
    if (!Func51FDB8(x, z, true, true)) {
        return false;
    }
    return Func3000F0(x, z, 0) && isHigh;
}
} // namespace

// 0x00696358
bool BsNpcMgr::IsKappnUnlocked()
{
    SvPlayer* player = netgame::IsMultiplePlayer() ? netgame::GetSvPlayer(netgame::PLAYER_NO_0) : SvGetCurrentPlayer();
    if (player == NULL) {
        return false;
    }
    return player->playerFlags.unlockedKappn;
}

// 0x0069639C
bool BsNpcMgr::AddLevel(const LevelSource* source)
{
    m_Level += source->level;
    return m_Level > 12.0f;
}

// 0x00698AA4
bool BsNpcMgr::AddLevel(const LevelSource* source, s32 index)
{
    const nn::math::VEC3& direction = m_GuardDirections[index];
    const f32 dot = source->direction.x * direction.x + source->direction.y * direction.y +
                    source->direction.z * direction.z;
    m_Level += __builtin_fabsf(dot * source->level);
    return m_Level > 12.0f;
}

// 0x00696A88
bool BsNpcMgr::SetGuardOwner(AcNpc* owner)
{
    if (m_GuardOwner != 0) {
        return false;
    }
    m_GuardOwner = owner;
    m_Level = 0.0f;
    if (m_GuardBoxB.IsActive()) {
        m_GuardBoxB.SetMask(m_GuardBoxB.GetMask() | 1);
    }
    m_GuardBoxA.SetMask(m_GuardBoxA.GetMask() | 1);
    for (s32 i = 0; i < 2; i++) {
        m_GuardBoxes[i].SetMask(m_GuardBoxes[i].GetMask() | 1);
    }
    return true;
}

// the weighted choice of the placement methods (inline in PlaceWalkingVillagers / PlaceWalkingVillager): weights in percent
// of 100; a method that fails is left out of the next draws
DECOMP_ALWAYS_INLINE bool BsNpcMgr::PlaceByMethods(nn::math::VEC3* position, s8 (*units)[2], s32 num,
                                                    const u32* defaultWeights, const PlaceMethod* methods)
{
    u32 weights[3] = {defaultWeights[0], defaultWeights[1], defaultWeights[2]};
    u32 total = 100;
    do {
        u32 value = Func2FF638(100);
        u32* weight = weights;
        s32 method;
        if (weight[0] != 0 && weight[0] > value) {
            method = 0;
        } else {
            if (weight[0] != 0) {
                value -= weight[0];
            }
            weight++;
            if (weight[0] != 0 && weight[0] > value) {
                method = 1;
            } else {
                if (weight[0] != 0) {
                    value -= weight[0];
                }
                weight++;
                if (weight[0] == 0 || weight[0] <= value) {
                    return false;
                }
                method = 2;
            }
        }
        if ((this->*methods[method])(position, units, num)) {
            return true;
        }
        if (*weight > total) {
            return false;
        }
        total -= *weight;
        *weight = 0;
    } while (total != 0);
    return false;
}

// 0x006964F4
void BsNpcMgr::PlaceWalkingVillagers()
{
    s8 units[3][2] = {};
    s32 num = 0;
    void* npcs = GetGardenPlus()->data.npcData.npcs;
    for (s32 i = 0; i < 3; i++) {
        const s32 index = Func118F30(npcs, i);
        if (index == -1) {
            continue;
        }
        u8* npc = static_cast<u8*>(event::Func300928(npcs, index));
        if (npc == NULL || npc[0x250F] != 0) {
            continue;
        }
        // 0x00854F18
        static const PlaceMethod s_Methods[3] = {&BsNpcMgr::PlaceMethod0, &BsNpcMgr::PlaceMethod1,
                                                 &BsNpcMgr::PlaceMethod2};
        const u32 weights[3] = {40, 40, 20};
        nn::math::VEC3 position = VEC3_ZERO;
        if (PlaceByMethods(&position, units, num, weights, s_Methods)) {
            NpcPositionEntry& entry = GetNpcPositions()->GetEntry(index);
            entry.Set(static_cast<u32>(position.x), static_cast<u32>(position.z));
            entry.stage = stage::GetCurrent();
            if (num < 3) {
                s32 x;
                s32 z;
                FdInfo::ToUnit(x, z, position);
                units[num][0] = x;
                units[num][1] = z;
                num++;
            }
        }
    }
}

// 0x00698B08
void BsNpcMgr::PlaceWalkingVillager(nn::math::VEC3* out, s32 index)
{
    s8 units[3][2] = {};
    s32 num = 0;
    void* npcs = GetGardenPlus()->data.npcData.npcs;
    for (s32 i = 0; i < 3; i++) {
        if (i == index) {
            continue;
        }
        const s32 other = Func118F30(npcs, i);
        if (other == -1) {
            continue;
        }
        const NpcPositionEntry& entry = GetNpcPositions()->GetEntry(other);
        nn::math::VEC3 position(static_cast<f32>(entry.GetX()), 0.0f, static_cast<f32>(entry.GetZ()));
        if (num >= 2) {
            continue;
        }
        s32 x;
        s32 z;
        Func307340(&x, &z, &position);
        units[num][0] = x;
        units[num][1] = z;
        num++;
    }
    if (Func118F30(npcs, index) == -1) {
        return;
    }
    // 0x00854F3C
    static const PlaceMethod s_Methods2[3] = {&BsNpcMgr::PlaceMethod0, &BsNpcMgr::PlaceMethod1,
                                              &BsNpcMgr::PlaceMethod2};
    const u32 weights[3] = {40, 40, 20};
    nn::math::VEC3 position = VEC3_ZERO;
    if (PlaceByMethods(&position, units, num, weights, s_Methods2)) {
        *out = position;
    }
}

// 0x00696F78
bool BsNpcMgr::LoadGuestModel(const char* model, bool isSpecial)
{
    if (m_LoadFlags.Test(LOAD_GUEST)) {
        return true;
    }
    sead::FixedSafeString<64> path;
    if (isSpecial) {
        SvFgName item;
        sead::FixedSafeString<8> name;
        name.copy(sead::SafeStringBase<char>(model));
        Func20601C(&name, &item, 0);
        path.format("Npc/Special/Model/%s.bcres", name.getCString());
    } else {
        path.format("Npc/Normal/Model/%s.bcres", model);
    }
    if (!m_GstLoader.LoadAsync(path, m_GstAllocator.m_pHeap, g3d::ResourceLoader::CGFX_ALIGNMENT)) {
        return false;
    }
    if (!m_LoadFlags.Test(LOAD_CMN)) {
        return false;
    }
    m_GstLoader.InitializeCgfx(&m_GstAllocator, NULL, true);
    m_GstLoader.InitializeCgfx(&m_GstAllocator, &m_CmnLoader, true);
    const u32 num = m_GstLoader.GetGraphicsFile().ptr()->models.count;
    for (u32 i = 0; i < num; i++) {
        g3d::SetLightSetIndex(
            static_cast<const nw::gfx::res::ResModelData*>(m_GstLoader.GetGraphicsFile().GetModels(i)),
            Func1E891C());
    }
    m_LoadFlags.Set(LOAD_GUEST, true);
    if (sead::FixedSafeString<32>(sead::SafeStringBase<char>("seo")).isEqual(sead::SafeStringBase<char>(model))) {
        m_LoadFlags.Set(LOAD_GUEST_SEO, true);
    }
    return true;
}

// 0x006989D8
void BsNpcMgr::UnloadGuestModel()
{
    if (m_LoadFlags.Test(LOAD_GUEST) && m_GstAllocator.m_pHeap != NULL) {
        m_GstLoader.Unload();
        m_LoadFlags.Set(LOAD_GUEST, false);
        m_LoadFlags.Set(LOAD_GUEST_SEO, false);
    }
}

// 0x006973C4
void BsNpcMgr::PickDailyOrder()
{
    const nn::fnd::DateTimeParameters date = time::GetSavedTime();
    math::Random random((date.year << 24) + (date.month << 16) + date.day);
    BitSet32 set;
    set.Init(DAILY_ORDER_NUM, true);
    for (u32 i = 0; i < DAILY_ORDER_NUM; i++) {
        const s32 index = set.PickRandom(&random);
        set.Remove(index);
        m_DailyOrder[i] = index + 1;
    }
}

namespace {
// (inline) the item is the lamp
inline bool IsYutaroLamp(const SvFgName* item)
{
    return item->GetKind() == item::KIND_YUTARO_LAMP;
}

// (inline) a lamp lies on the field of the stage
inline bool HasYutaroLampOnField(Field* field)
{
    for (s32 z = 0; z < 16; z++) {
        for (s32 x = 0; x < 16; x++) {
            const SvFgName* item = field->GetFg(x, z, 0);
            if (item != NULL && IsYutaroLamp(item)) {
                return true;
            }
            item = field->GetFg(x, z, 1);
            if (item != NULL && IsYutaroLamp(item)) {
                return true;
            }
        }
    }
    return false;
}
} // namespace

// 0x00697454
bool BsNpcMgr::HasYutaroLamp()
{
    // the items of the town
    SvTownDataUnk5C* map = &GetGardenData()->townData.unk_0x5C;
    for (s32 z = 0; z < 4; z++) {
        for (s32 x = 0; x < 5; x++) {
            SvFgBlock* block = Func2FAECC(map, x, z);
            if (block == NULL) {
                continue;
            }
            for (s32 unitZ = 0; unitZ < 16; unitZ++) {
                for (s32 unitX = 0; unitX < 16; unitX++) {
                    if (IsYutaroLamp(block->GetFgChecked(unitX, unitZ))) {
                        return true;
                    }
                }
            }
        }
    }
    // the pockets, the island box, the dressers and the house of the player
    SvPlayer* player = SvGetCurrentPlayer();
    if (player->playerInfo.IsValid()) {
        for (u32 i = 0; i < 16; i++) {
            if (IsYutaroLamp(player->inventory.GetItem(i))) {
                return true;
            }
        }
        for (u32 i = 0; i < 40; i++) {
            if (IsYutaroLamp(Func720E88(player->islandBox, i))) {
                return true;
            }
        }
        for (u32 a = 0; a < 3; a++) {
            for (u32 b = 0; b < 6; b++) {
                for (u32 c = 0; c < 10; c++) {
                    if (IsYutaroLamp(Func76FEE0(player->dressers, a, b, c))) {
                        return true;
                    }
                }
            }
        }
        void* house = Func70FA3C(reinterpret_cast<u8*>(GetGardenPlus()) + 0x71900, SvGetPlayerSaveNo());
        for (u32 room = 0; room < 4; room++) {
            for (u32 z = 0; z < 9; z++) {
                for (u32 x = 0; x < 10; x++) {
                    if (IsYutaroLamp(Func770034(house, room, z, x))) {
                        return true;
                    }
                }
            }
        }
    }
    // the fields of the stages of the player
    for (s32 i = 2; i < 26; i++) {
        const FieldName name = static_cast<FieldName>(static_cast<u8>(i));
        if (Func6FB278(name) != SvGetPlayerSaveNo()) {
            continue;
        }
        Field* field = stage::GetField(name);
        if (field != NULL && HasYutaroLampOnField(field)) {
            return true;
        }
    }
    for (s32 i = 0; i < 4; i++) {
        const s8* owner = Func76DE58(reinterpret_cast<u8*>(GetGardenPlus()) + 0x80 + 0x6AE38, static_cast<u8>(i));
        if (*owner != SvGetPlayerSaveNo()) {
            continue;
        }
        Field* field = stage::GetField(static_cast<FieldName>(static_cast<u8>(i + 26)));
        if (field != NULL && HasYutaroLampOnField(field)) {
            return true;
        }
    }
    return false;
}

// 0x00697B0C
bool BsNpcMgr::AddActorUnit(Actor* actor)
{
    const s32 num = m_ActorUnits.GetSize();
    ActorUnit* unit = m_ActorUnits.first;
    for (s32 i = 0; i < num; i++, unit++) {
        if (unit->actor == actor) {
            return true;
        }
    }
    s32 x;
    s32 z;
    FdInfo::ToUnit(x, z, actor->m_Position);
    ActorUnit entry = {actor, x, z};
    m_ActorUnits.PushBackFast(entry);
    return true;
}

// 0x00698960
bool BsNpcMgr::RemoveActorUnit(Actor* actor)
{
    for (ActorUnit* unit = m_ActorUnits.first; unit != m_ActorUnits.last; unit++) {
        if (unit->actor == actor) {
            m_ActorUnits.last--;
            for (; unit != m_ActorUnits.last; unit++) {
                unit[0] = unit[1];
            }
            return true;
        }
    }
    return false;
}

// 0x00697C40
bool BsNpcMgr::Func697C40()
{
    if (netgame::IsMultiPlayer()) {
        return false;
    }
    SvTownDataUnkEE2C* camp = &GetGardenPlus()->data.townData.unk_0xEE2C;
    // (the original tests the address of the items for NULL)
    const SvFgName* items = camp->campgroundShopItems;
    if (items != NULL) {
        const u16 id = camp->campgroundCampingCar[1];
        if (id == 0x2000 || id == 0x2010) {
            return false;
        }
    }
    SvPlayer* player = SvGetCurrentPlayer();
    if (player == NULL) {
        return false;
    }
    if (!player->playerFlags.playerSetNameAndTownName || !player->playerFlags.finishedFirstDay) {
        return false;
    }
    return !(reinterpret_cast<const u8*>(&player->unk_0x8F77)[8] & 2);
}

// 0x00698894
u32 BsNpcMgr::Func698894(u16 value)
{
    for (AsrNpc* entry = m_AsrNpcs.first; entry != m_AsrNpcs.last; entry++) {
        if (value < entry->from || value > entry->to) {
            continue;
        }
        if (Func309C6C(2, time::GetCurrentDate())) {
            return 73;
        }
        switch (entry->kind) {
        case 0:
        case 1:
        case 4:
            if (entry->actor != NULL) {
                return entry->actor->vf_0x9C();
            }
            break;
        case 2:
            if (entry->actor != NULL) {
                return 73;
            }
            break;
        case 5:
            return 76;
        default:
            break;
        }
    }
    return 71;
}

// 0x00698A38
void BsNpcMgr::Func698A38(u32 key, bool on, bool keep)
{
    if (g_Unknown9516C0.mode != 255 || g_Unknown9516C0.p4 == NULL) {
        return;
    }
    void* house = Func6DD804(g_Unknown9516C0.p4, key);
    if (house == NULL) {
        return;
    }
    Func150350(house, on);
    if (!keep) {
        Func150358(house, !on);
    }
}

// 0x0069B324
bool BsNpcMgr::PlaceMethod0(nn::math::VEC3* position, s8 (*units)[2], s32 num)
{
    return Func180784(position, units, num, reinterpret_cast<const void*>(&Func17C1F8));
}

// 0x0069B104
bool BsNpcMgr::PlaceMethod2(nn::math::VEC3* position, s8 (*units)[2], s32 num)
{
    return Func180784(position, units, num, reinterpret_cast<const void*>(&Func175178));
}

// 0x00698D44
bool BsNpcMgr::PlaceMethod1(nn::math::VEC3* position, s8 (*units)[2], s32 num)
{
    // in front of (z - 1) a grown tree of a random block, beside it (half a unit to the left or right)
    Field* field = stage::GetField(static_cast<FieldName>(1));
    if (field == NULL) {
        return false;
    }
    FdBkSearchCand cand(field->GetBlockWidth(), field->GetBlockHeight());
    cand.Reset();
    cand.RemoveEdges();
    bool isFound = false;
    while (true) {
        const s32 index = cand.Pick();
        if (index < 0) {
            return false;
        }
        const s32 blockZ = index / cand.m_Width;
        const s32 blockX = index % cand.m_Width;
        BgInfo* bg = field->GetBg(blockX, blockZ);
        if (bg != NULL) {
            s32 found = 0;
            for (s32 unitZ = 0; unitZ < 16; unitZ++) {
                for (s32 unitX = 0; unitX < 16; unitX++) {
                    const SvFgName* item = static_cast<const BgInfo*>(bg)->GetFg(unitX, unitZ, 0);
                    if (item == NULL || item->IsEmpty() || !item->IsFg()) {
                        continue;
                    }
                    const item::ParamFgEntry* param = item::GetParamFgEntry(*item);
                    if (param == NULL || param->size != 4) {
                        continue;
                    }
                    s32 x;
                    s32 z;
                    FdInfo::ToUnit(x, z, blockX, blockZ, unitX, unitZ);
                    z--;
                    if (!Func51FDB8(x, z, false, true) || !IsFarFromUnits(units, num, x, z)) {
                        continue;
                    }
                    // each new place replaces the last one with a chance of 1 / n
                    if (math::GetRandomF32(static_cast<f32>(found + 1)) >= 1.0f) {
                        continue;
                    }
                    nn::math::VEC3 center;
                    FdInfo::ToCenterWorld(&center, x, z);
                    *position = center;
                    BitSet32 sides;
                    sides.Init(2, false);
                    if (Func51FDB8(x - 1, z, false, true)) {
                        sides.Add(0);
                    }
                    if (Func51FDB8(x + 1, z, false, true)) {
                        sides.Add(1);
                    }
                    if (sides.count <= 0) {
                        continue;
                    }
                    found++;
                    const bool isLeft = sides.PickRandom() == 0;
                    FdInfo::ToCenterWorld(position, x, z);
                    if (isLeft) {
                        position->x -= FdInfo::UNIT_WORLD_SIZE_XF * 0.45f;
                    } else {
                        position->x += FdInfo::UNIT_WORLD_SIZE_XF * 0.45f;
                    }
                    isFound = true;
                }
            }
            if (isFound) {
                return true;
            }
        }
        cand.Remove(blockZ * cand.m_Width + blockX);
    }
}

// 0x006990C8
bool BsNpcMgr::IsFarFromUnits(const s8 (*units)[2], s32 num, s32 x, s32 z)
{
    for (s32 i = 0; i < num; i++) {
        s32 dx = x - units[i][0];
        if (dx < 0) {
            dx = -dx;
        }
        s32 dz = z - units[i][1];
        if (dz < 0) {
            dz = -dz;
        }
        if (dx + dz < 25) {
            return false;
        }
    }
    return true;
}

// 0x00699064
bool BsNpcMgr::SetValueOfKind(u8 kind, u32 value)
{
    for (AcNpc** it = m_Npcs.first; it != m_Npcs.last; it++) {
        AcNpc* npc = *it;
        if (npc != NULL && npc->m_Kind == kind) {
            npc->m_Unknown938 = value;
            return true;
        }
    }
    return false;
}

// 0x006991AC
bool BsNpcMgr::Func6991AC()
{
    if (netgame::IsMultiPlayer()) {
        return false;
    }
    SvPlayer* player = SvGetCurrentPlayer();
    if (player == NULL) {
        return false;
    }
    if (player->playerFlags.unk_0x12_6) {
        return false;
    }
    return player->playerFlags.unk_0x13_5;
}

// 0x00699200
bool BsNpcMgr::CreateMaiko()
{
    if (!m_SpBanks.Create(HeapInit::s_pStageHeap, 2)) {
        return false;
    }
    if (::Func2FC0D0()[21] == 0) {
        return true;
    }
    stage::NpcInfo info(stage::NpcInfo::KIND_MODEL, 0, "lom");
    math::Angle16Vec3 rotation = {0, 0x8000, 0};
    oml::framework::Process* npc =
        Actor::Create(PROC_AC_NPC_SP_MAIKO, this, reinterpret_cast<u32>(&info), &stage::detail::s_NpcPosition7, &rotation);
    if (npc != NULL) {
        m_Npcs.PushBackFast(static_cast<AcNpc*>(npc));
    }
    return true;
}

// 0x00699318
bool BsNpcMgr::FindUnitOutside(nn::math::VEC3* out, s32 minX, s32 minZ, s32 maxX, s32 maxZ)
{
    // the units of the block of the house of the player, tested by Func174164
    // 0x00854E42
    static const u8 s_HouseStructures[4] = {0, 1, 2, 3};
    nn::math::VEC3_ house;
    const u8 saveNo = SvGetPlayerSaveNo();
    if (saveNo < 4) {
        GetStructurePosition(&house, s_HouseStructures[saveNo]);
    } else {
        GetStructurePosition(&house, 0);
    }
    s32 block[2];
    Func307340(&block[0], &block[1], static_cast<const nn::math::VEC3*>(&house));
    BkUnitSearchCandCb cand(block[0], block[1], 0, reinterpret_cast<BkUnitSearchCandCb::Callback>(&Func174164));
    cand.Clear();
    cand.AddCandidates();
    while (cand.m_RestNum > 0) {
        const s32 index = cand.Pick();
        if (index < 0) {
            continue;
        }
        const s32 unitZ = index / cand.m_Width;
        const s32 unitX = index % cand.m_Width;
        s32 x;
        s32 z;
        FdInfo::ToUnit(x, z, block[0], block[1], unitX, unitZ);
        if (x < minX || maxX < x || z < minZ || maxZ < z) {
            FdInfo::ToCenterWorld(out, block[0], block[1], unitX, unitZ);
            u8* data = static_cast<u8*>(netgame::g_pUnknown95488C);
            if (data != NULL) {
                data[0x9F] |= 2;
                data[0x9E] = unitX | (unitZ << 4);
            }
            return true;
        }
        cand.Remove(unitZ * cand.m_Width + unitX);
    }
    return false;
}

// 0x006994B8
void BsNpcMgr::OnPlayerEndEmoticon(u32 /* emoticon */, s32 player)
{
    if (netgame::IsMultiplePlayer() || player != 1) {
        return;
    }
    ForeachOnPlayerEndEmoticon func(0);
    ForeachImpl(func);
}

// 0x00699540
void BsNpcMgr::Func699540()
{
    Func51D35C(0);
    const s32 index = Func51E3DC();
    if (index < 0) {
        return;
    }
    void* npcs = GetGardenPlus()->data.npcData.npcs;
    const s32 house = stage::GetNpcHouseIndex(stage::GetCurrent());
    if (house != Func119244(npcs) && index < 2) {
        Func51E3EC(index + 1);
    }
}

// 0x00699598
bool BsNpcMgr::Func699598()
{
    SvPlayer* player = SvGetCurrentPlayer();
    if (player != NULL) {
        if (reinterpret_cast<const u8*>(&player->unk_0x8A3C)[0x67] & 8) {
            GetGardenData()->townData.unk_0x1DAC7 |= 2;
            return false;
        }
        if (!player->playerFlags.finishedFirstDay) {
            return false;
        }
    }
    if (event::IsActive(event::EVENT_ID_APRIL_FOOLS_DAY)) {
        return false;
    }
    if (!(GetGardenPlus()->data.townData.unk_0x1DAC7 & 1)) {
        return false;
    }
    if (GetGardenPlus()->data.townData.unk_0x1DAC7 & 2) {
        return false;
    }
    return !netgame::IsMultiPlayer();
}

// 0x00699650
bool BsNpcMgr::PrepareBanks(const stage::Command* command)
{
    return PrepareBanksImpl(command);
}

// 0x0069976C
void BsNpcMgr::SetupVillageGuards()
{
    const f32 unit = FdInfo::UNIT_WORLD_SIZE_XF;
    // in front of the train station
    nn::math::VEC3 size(unit * 3.0f, unit * 2.0f, unit * 5.0f);
    nn::math::VEC3_ position;
    GetStructurePosition(&position, GetStationStructure());
    position.x += unit * 6.0f;
    position.z -= unit * 4.0f;
    m_GuardBoxA.Set(position, size, 0x100, 130, &VillageStationGuard);
    if (Func6FA888()) {
        position = *Func6A76A0();
        size.x = unit * 4.0f;
        size.y = unit * 10.0f;
        size.z = unit * 11.0f;
        if (GetGardenPlus()->data.townData.unk_0x5C.oceanSide == 0) {
            position.x += FdInfo::UNIT_WORLD_SIZE_HALF_XF;
        } else {
            position.x -= FdInfo::UNIT_WORLD_SIZE_HALF_XF;
        }
        m_GuardBoxB.Set(position, size, 0x100, 130, &VillageGuardB);
    }
    s32 found = 0;
    for (u32 a = 1; a < 6; a++) {
        for (u32 b = 1; b < 5; b++) {
            if (Func6D0084(a, b, 0)) {
                Func150360(this, a, b, found);
                found++;
                if (found == 2) {
                    break;
                }
            }
        }
    }
}

// 0x0069993C
void BsNpcMgr::Func69993C()
{
    u8* data = static_cast<u8*>(netgame::g_pUnknown954884);
    data[0x174] = stage::NAME_MAX;
    data[0x184] = stage::NAME_MAX;
}

// 0x0069995C
bool BsNpcMgr::CreateWalkingVillagers()
{
    SvPlayer* player = g_pSvCurrentPlayer;
    if (player != NULL && player->playerFlags.unk_0x2B_7) {
        return true;
    }
    // at most five villagers walk in the town (three in the Dream Suite)
    const s32 num = netgame::GetNetgameType() == netgame::TYPE_DREAM ? 3 : 5;
    if (!m_NmlBanks.Create(HeapInit::s_pStageHeap, num)) {
        return false;
    }
    SvNpc* npcs = GetGardenData()->GetNpcData()->npcs;
    if (s_State.unk0E) {
        CreateWalkingVillagersE(); // 0x0069EB60
        return true;
    }
    if (s_State.unk0F) {
        CreateWalkingVillagersF(); // 0x0069D368
        return true;
    }

    // the villagers that are in the town (online: those of the host)
    BitSet32 set;
    set.Init(10, false);
    const u8* online = static_cast<const u8*>(netgame::g_pUnknown95488C);
    for (s32 i = 0; i < 10; i++) {
        SvNpc* npc = Func305AC0(npcs, i);
        if (npc == NULL || npc->unk_0x250E != 0) {
            continue;
        }
        if (netgame::IsMultiPlayer()) {
            if (online == NULL || static_cast<u32>(i) >= 10 || !((online[0x98 + i / 8] >> (i % 8)) & 1)) {
                continue;
            }
        }
        set.Add(i);
    }
    while (set.count > num) {
        const s32 index = set.PickRandom();
        SvNpc* npc = Func305AC0(npcs, index);
        if (npc != NULL) {
            Func113A14(npc, 2);
        }
        set.Remove(index);
    }

    for (u32 k = 0; k < 5; k++) {
        const s32 index = set.GetNth(k);
        SvNpc* npc = Func305AC0(npcs, index);
        if (npc == NULL) {
            continue;
        }
        math::Angle16Vec3 rotation = {0, 0, 0};
        nn::math::VEC3 position;
        bool isPlaced = false;
        // the position of the last time on this stage
        const NpcPositionEntry& entry = GetNpcPositions()->GetEntry(index);
        if (stage::GetCurrent() == entry.stage) {
            position.x = static_cast<f32>(entry.GetX());
            position.y = 96.0f;
            position.z = static_cast<f32>(entry.GetZ());
            position.y = FdInfo::GetHeight(position, FIELD_NAME_CURRENT);
            rotation.y.value = entry.packed[3] << 8;
            isPlaced = true;
        }
        // the chosen villager stands on a random unit of a random block away from the structure 4 (meaning unknown)
        if (m_UnknownF11C == index) {
            const nn::math::VEC3* structure = Func5C37B0(4, true);
            Field* field = stage::GetField(static_cast<FieldName>(1));
            if (structure != NULL && field != NULL) {
                s32 avoidX;
                s32 avoidZ;
                Func307340(&avoidX, &avoidZ, structure);
                FdBkSearchCand blocks(field->GetBlockWidth(), field->GetBlockHeight());
                blocks.Reset();
                blocks.RemoveEdges();
                blocks.Remove(avoidZ * blocks.m_Width + avoidX);
                while (blocks.m_RestNum > 0) {
                    const s32 blockIndex = blocks.Pick();
                    if (blockIndex < 0) {
                        continue;
                    }
                    const s32 blockZ = blockIndex / blocks.m_Width;
                    const s32 blockX = blockIndex % blocks.m_Width;
                    BkUnitSearchCandCb units(blockX, blockZ, 0, &IsStandableUnit);
                    units.Reset();
                    units.AddCandidates();
                    if (units.m_RestNum > 0) {
                        const s32 unitIndex = units.Pick();
                        if (unitIndex >= 0) {
                            FdInfo::ToCenterWorld(&position, blockX, blockZ, unitIndex % units.m_Width,
                                                  unitIndex / units.m_Width);
                            isPlaced = true;
                            break;
                        }
                    }
                    blocks.Remove(blockZ * blocks.m_Width + blockX);
                }
            }
        }
        // the countdown to the new year (Dec 31 from 23:50 to Jan 1 2:00): near the countdown
        if (!isPlaced) {
            const nn::fnd::DateTimeParameters* date = time::GetCurrentDate();
            const s32 day = date->month * 100 + date->day;
            const s32 clock = date->hour * 100 + date->minute;
            const bool isCountdown = day == 1231 ? clock >= 2350 : (day == 101 && clock < 200);
            SvPlayer* current = g_pSvCurrentPlayer;
            if (isCountdown && current != NULL && current->playerFlags.finishedFirstDay) {
                s32 blockX;
                s32 blockZ;
                Func6FB074(&blockX, &blockZ);
                s32 baseX;
                s32 baseZ;
                FdInfo::ToUnit(baseX, baseZ, blockX, blockZ, 0, 0);
                SearchCandXZ<16, 16> units;
                units.Clear();
                for (s32 x = 0; x < 16; x++) {
                    for (s32 z = 0; z < 16; z++) {
                        if (FdInfo::IsAttribute11(baseX + x, baseZ + z, FIELD_NAME_CURRENT) &&
                            Func51FDB8(baseX + x, baseZ + z, true, true)) {
                            units.Add(z * units.m_Width + x);
                        }
                    }
                }
                // not where another NPC stands
                NpcPosGetCB positions(NULL);
                ForeachImpl(positions);
                const s32 posNum = positions.m_Num;
                for (s32 i = 0; i < posNum; i++) {
                    nn::math::VEC3 other;
                    if (i < positions.m_Num) {
                        ssys::ma::CopyVec3(&other, &positions.m_Positions[i]);
                    } else {
                        other = VEC3_ZERO;
                    }
                    s32 otherBlockX;
                    s32 otherBlockZ;
                    s32 otherUnitX;
                    s32 otherUnitZ;
                    FdInfo::ToBlockAndLocalUnit(otherBlockX, otherBlockZ, otherUnitX, otherUnitZ, other);
                    if (otherBlockX == blockX && otherBlockZ == blockZ) {
                        units.Remove(otherUnitZ * units.m_Width + otherUnitX);
                    }
                }
                const s32 unitIndex = units.Pick();
                if (unitIndex >= 0) {
                    FdInfo::ToCenterWorld(&position, baseX + unitIndex % units.m_Width, baseZ + unitIndex / units.m_Width);
                    isPlaced = true;
                }
            }
        }
        // while this console controls the stage, a villager at home or on the way must stand on a free unit
        if (isPlaced && netgame::IsStageControl()) {
            const u8 state = npc->unk_0x250F;
            if (state == 0 || state == 1) {
                if (!Func51FC50(&position, true, true) || IsNpcAt(&position) || !FdInfo::HasEqualHeights(position, static_cast<FieldName>(1))) {
                    isPlaced = Func51CCF4(&position, &position, 2, reinterpret_cast<const void*>(&Func51D0FC), 0, true, 0);
                }
            }
        }
        // else in front of its house
        if (!isPlaced) {
            GetStructurePosition(&position, static_cast<u8>(index + 8));
            position.z += FdInfo::UNIT_WORLD_SIZE_XF;
        }
        stage::NpcInfo info;
        info.kind = stage::NpcInfo::KIND_ID;
        info.flag = 0;
        info.value = npc->npc.villagerId | (static_cast<u8>(index) << 16);
        oml::framework::Process* actor =
            Actor::Create(PROC_AC_NPC_OUT, this, reinterpret_cast<u32>(&info), &position, &rotation);
        if (actor != NULL) {
            m_Npcs.PushBackFast(static_cast<AcNpc*>(actor));
        }
    }
    return true;
}

// 0x0069A230
oml::framework::Process* BsNpcMgr::CreateNpc(ProcName name, u32 arg, const nn::math::VEC3_* position,
                                             const math::Angle16Vec3* rotation)
{
    return Actor::Create(name, this, arg, position, rotation);
}

// 0x0069A254
void BsNpcMgr::OnFallToPitfall()
{
    if (netgame::IsMultiplePlayer()) {
        return;
    }
    ForeachOnFallToPitfall func;
    ForeachImpl(func);
}

// 0x0069A2C4
void BsNpcMgr::OnPlayerStartEmoticon(u8 emoticon, s32 player)
{
    if (netgame::IsMultiplePlayer() || player != 1) {
        return;
    }
    ForeachOnPlayerStartEmoticon func(emoticon);
    ForeachImpl(func);
}

// 0x0069A34C
void BsNpcMgr::SetupStageVillagers()
{
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return;
    }
    const stage::Name current = stage::GetCurrent();
    const stage::Name previous = stage::GetPrevious();
    BitSet32 set;
    set.Init(10, false);
    if (netgame::IsMultiPlayer() || netgame::GetMyPlayerNo() != 0) {
        if (netgame::IsStageControl() && current == stage::NAME_VILLAGE) {
            Func69ED50(&set, true);
        }
        return;
    }
    m_UnknownF11C = -1;
    // nobody follows between the cafe and the work in the cafe
    if (current == stage::NAME_CAFE) {
        if (previous == stage::NAME_CAFE_ARBEIT) {
            return;
        }
    } else if (current == stage::NAME_CAFE_ARBEIT && previous == stage::NAME_CAFE) {
        return;
    }
    GetGardenData()->GetNpcData();
    if (Func69ED50(&set, false)) {
        return;
    }
    if (stage::IsKind(stage::KIND_VILLAGE, current)) {
        if (previous >= stage::NAME_MAX) {
            goto downtown;
        }
        if (stage::IsKind(stage::KIND_INDOOR, previous)) {
            if (Func118E74(GetGardenData()->GetNpcData()->npcs) <= 3) {
                Func6A2314(&set);
            } else {
                Func6A1F94(&set);
            }
            return;
        }
    } else if (previous >= stage::NAME_MAX) {
        goto downtown;
    }
    if (stage::IsKind(stage::KIND_VILLAGE, previous) && stage::IsOwnHouseRoom(stage::GetCurrent())) {
        Func6A0C98(&set);
        return;
    }
    if (stage::IsKind(stage::KIND_VILLAGE, previous) &&
        (current == stage::NAME_DOWNTOWN || current == stage::NAME_RECYCLE_SHOP || current == stage::NAME_CAFE)) {
        Func6A10D8(&set);
        return;
    }
    if (stage::IsKind(stage::KIND_DOWNTOWN, previous) &&
        (current == stage::NAME_CONVENIENCE_STORE || current == stage::NAME_SUPERMARKET ||
         current == stage::NAME_HOME_CENTER || current == stage::NAME_DEPARTMENT_STORE_1F ||
         current == stage::NAME_CLUB444_DJKK || current == stage::NAME_CLUB444_LIVE || current == stage::NAME_TAILOR ||
         current == stage::NAME_MUSEUM_ENTRANCE)) {
        Func6A19A4(&set);
        return;
    }
downtown:
    if (stage::IsKind(stage::KIND_DOWNTOWN, current)) {
        switch (previous) {
        case stage::NAME_FLORISTS:
        case stage::NAME_FLORISTS_HOME_IMPROVEMENT_CENTER:
        case stage::NAME_DREAM:
        case stage::NAME_PHOTO_BOX:
        case stage::NAME_BEAUTY_SALON:
        case stage::NAME_POST_OFFICE:
        case stage::NAME_FORTUNE:
        case stage::NAME_REAL_ESTATE_AGENCY:
        case stage::NAME_SHOE_SHOP:
        case stage::NAME_SHOP:
        case stage::NAME_CONVENIENCE_STORE:
        case stage::NAME_SUPERMARKET:
        case stage::NAME_HOME_CENTER:
        case stage::NAME_DEPARTMENT_STORE_1F:
        case stage::NAME_CLUB444:
        case stage::NAME_CLUB444_DJKK:
        case stage::NAME_CLUB444_LIVE:
        case stage::NAME_TAILOR:
        case stage::NAME_ACCESSORY_SHOP:
        case stage::NAME_MUSEUM_ENTRANCE:
            Func150368(this, &set);
            break;
        default:
            break;
        }
        return;
    }
    if (previous == stage::NAME_MUSEUM_ENTRANCE && stage::IsKind(stage::KIND_MUSEUM_SHOWROOM, current)) {
        Func6A0964(&set);
    }
}

namespace {
// (inline) a unit that cannot be walked on
inline bool IsBlockedUnit(s32 x, s32 z)
{
    if (FdInfo::HasPart95E93E(x, z, static_cast<FieldName>(1))) {
        return true;
    }
    if (!FdInfo::IsFlat(x, z, static_cast<FieldName>(1)) && !FdInfo::HasPartIn891697(x, z, static_cast<FieldName>(1))) {
        return true;
    }
    if (FdInfo::IsAttribute02_03_04_A2_A5_F8(x, z, static_cast<FieldName>(1))) {
        return true;
    }
    return FdInfo::HasPartIn89166C(x, z, static_cast<FieldName>(1));
}
} // namespace

// 0x0069A674
void BsNpcMgr::BuildWalkMap()
{
    Field* field = stage::GetField(static_cast<FieldName>(1));
    if (field == NULL) {
        return;
    }
    for (u32 z = 0; z < WALK_MAP_HEIGHT; z++) {
        for (u32 x = 0; x < WALK_MAP_WIDTH; x++) {
            WalkMapEntry& entry = g_WalkMap[z * WALK_MAP_WIDTH + x];
            entry.dirs.m_Bits = 0;
            entry.kind = 0;
        }
    }
    GetGardenPlus();
    for (s32 blockZ = 0; static_cast<u32>(blockZ) < static_cast<u32>(field->GetBlockHeight()); blockZ++) {
        for (s32 blockX = 0; static_cast<u32>(blockX) < static_cast<u32>(field->GetBlockWidth()); blockX++) {
            const bool isTown = Func6141C0(blockX, blockZ);
            // which quarters of the block are connected (the kinds 0..3 of the units): 1 = 0-1, 2 = 0-2, 4 = 1-3,
            // 8 = 2-3
            u8 links = 0;
            const s32 steps[2][2] = {{1, 0}, {0, 1}};
            for (s32 unitZ = 0; unitZ < 15; unitZ++) {
                for (s32 unitX = 0; unitX < 15; unitX++) {
                    s32 x;
                    s32 z;
                    FdInfo::ToUnit(x, z, blockX, blockZ, unitX, unitZ);
                    if (IsBlockedUnit(x, z)) {
                        continue;
                    }
                    const u8 kind = FdInfo::GetKind(blockX, blockZ, unitX, unitZ, static_cast<FieldName>(1));
                    for (s32 i = 0; i < 2; i++) {
                        const s32 nextUnitX = steps[i][0] + unitX;
                        const s32 nextUnitZ = steps[i][1] + unitZ;
                        s32 nextX;
                        s32 nextZ;
                        FdInfo::ToUnit(nextX, nextZ, blockX, blockZ, nextUnitX, nextUnitZ);
                        if (IsBlockedUnit(nextX, nextZ)) {
                            continue;
                        }
                        const u8 next =
                            FdInfo::GetKind(blockX, blockZ, nextUnitX, nextUnitZ, static_cast<FieldName>(1));
                        if (kind == next) {
                            continue;
                        }
                        switch (kind) {
                        case 0:
                            if (next == 1) {
                                links |= 1;
                            } else if (next == 2) {
                                links |= 2;
                            }
                            break;
                        case 1:
                            if (next == 3) {
                                links |= 4;
                            } else if (next == 0) {
                                links |= 1;
                            }
                            break;
                        case 2:
                            if (next == 3) {
                                links |= 8;
                            } else if (next == 0) {
                                links |= 2;
                            }
                            break;
                        case 3:
                            if (next == 2) {
                                links |= 8;
                            } else if (next == 1) {
                                links |= 4;
                            }
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
            // the four quarters of the block in the map (directions 0 -x, 1 +x, 2 -z, 3 +z)
            for (s32 z = blockZ * 2; z < blockZ * 2 + 2; z++) {
                for (s32 x = blockX * 2; x < blockX * 2 + 2; x++) {
                    WalkMapEntry* entry = &g_WalkMap[z * WALK_MAP_WIDTH + x];
                    if (isTown) {
                        const bool isInnerX = static_cast<u32>(x - 3) < 8;
                        const bool isInnerZ = static_cast<u32>(z - 3) < 6;
                        const bool isOddZ = z & 1;
                        const bool isOddX = x & 1;
                        // the way out of the block is open in the inner part of the town, the way to the next quarter
                        // if the quarters are connected
                        InlineVector<u8, 2> inner;
                        if (isOddZ) {
                            entry->dirs.Set(3, isInnerZ);
                            inner.PushBackFast(2);
                        } else {
                            entry->dirs.Set(2, isInnerZ);
                            inner.PushBackFast(3);
                        }
                        if (isOddX) {
                            entry->dirs.Set(1, isInnerX);
                            inner.PushBackFast(0);
                        } else {
                            entry->dirs.Set(0, isInnerX);
                            inner.PushBackFast(1);
                        }
                        for (const u8* dir = inner.first; dir != inner.last; dir++) {
                            bool isLinked;
                            if (*dir == 0 || *dir == 1) {
                                isLinked = isOddZ ? (links & 8) : (links & 1);
                            } else {
                                isLinked = isOddX ? (links & 4) : (links & 2);
                            }
                            if (isLinked) {
                                entry->dirs.Set(*dir, true);
                            }
                        }
                    } else {
                        // no way into the block: the ways of the neighbours into it are closed too
                        const s32 neighbours[8][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {1, 1}, {1, -1}, {-1, 1}};
                        entry->kind = 3;
                        for (u8 i = 0; i < 4; i++) {
                            const s32 nx = neighbours[i][0] + x;
                            const s32 nz = neighbours[i][1] + z;
                            if (static_cast<u32>(nx) >= WALK_MAP_WIDTH || nz < 0 || nz >= static_cast<s32>(WALK_MAP_HEIGHT)) {
                                continue;
                            }
                            WalkMapEntry* next = &g_WalkMap[nz * WALK_MAP_WIDTH + nx];
                            if (next == NULL) {
                                continue;
                            }
                            const u8 opposite = (i & 1) ? i - 1 : i + 1;
                            entry->dirs.Set(i, false);
                            next->dirs.Set(opposite, false);
                        }
                    }
                }
            }
        }
    }
}

// 0x0069ACEC
void BsNpcMgr::CalcStageControl()
{
    // one villager per frame (m_Frame): the door of its house follows its state at home (2) / leaving it (23)
    if (!stage::IsKind(stage::KIND_VILLAGE, stage::GetCurrent())) {
        return;
    }
    if (demo::CheckOrder(static_cast<demo::OrderID>(3), false)) {
        return;
    }
    SvNpc* npcs = GetGardenData()->GetNpcData()->npcs;
    SvNpc* npc = Func305AC0(npcs, m_Frame);
    if (npc == NULL) {
        return;
    }
    if (g_Unknown9516C0.mode != 255 || g_Unknown9516C0.p4 == NULL) {
        return;
    }
    void* house = Func6DD804(g_Unknown9516C0.p4, m_Frame);
    if (Func52CF24() == house) {
        return;
    }
    u8 message[2];
    switch (npc->unk_0x250F) {
    case 2:
        if (!Func309B70(npc, 0) || Func11972C(npcs, m_Frame, 0)) {
            return;
        }
        Func113A14(npc, 23);
        Func698A38(m_Frame, true, false);
        message[0] = m_Frame;
        message[1] = 0;
        Func7D2190(4, message);
        break;
    case 23:
        if (Func309B70(npc, 0)) {
            return;
        }
        Func113A14(npc, 2);
        Func698A38(m_Frame, false, false);
        message[0] = m_Frame;
        message[1] = 1;
        Func7D2190(4, message);
        break;
    default:
        break;
    }
}

// 0x0069AECC
bool BsNpcMgr::CreateEventNpc()
{
    SvPlayer* player = SvGetCurrentPlayer();
    if (player == NULL || !player->playerFlags.finishedFirstDay) {
        return false;
    }
    for (u32 i = 0; i < EVENT_NPC_NUM; i++) {
        const EventNpc& entry = s_EventNpcs[i];
        const u8 kind = entry.kind;
        if (netgame::IsMultiplePlayer()) {
            if (!event::IsActive(entry.eventOrDay)) {
                continue;
            }
        } else if (kind == 0) {
            if (!Func309C6C(entry.eventOrDay, time::GetCurrentDate())) {
                continue;
            }
        } else if (kind == 1) {
            if (!event::IsActive(entry.eventOrDay)) {
                continue;
            }
        } else {
            continue;
        }
        if (m_SpBanks.m_Model.m_Size == m_SpBanks.m_Model.m_UsedNum) {
            continue;
        }
        math::Angle16Vec3 rotation = {0, 0, 0};
        stage::NpcInfo info(stage::NpcInfo::KIND_MODEL, 0, entry.model);
        PlaceFunc place = s_PlaceFuncs[entry.place];
        nn::math::VEC3 position;
        if (place == NULL || !place(&position, &rotation, &info)) {
            continue;
        }
        oml::framework::Process* npc =
            Actor::Create(static_cast<ProcName>(entry.name), this, reinterpret_cast<u32>(&info), &position, &rotation);
        if (npc != NULL) {
            m_Npcs.PushBackFast(static_cast<AcNpc*>(npc));
        }
        return true;
    }
    return false;
}

// 0x0069B094
void BsNpcMgr::OnMoveOutPitfall()
{
    if (netgame::IsMultiplePlayer()) {
        return;
    }
    ForeachOnMoveOutPitfall func;
    ForeachImpl(func);
}

// 0x0069B2B4
void BsNpcMgr::OnStrugglePitfall()
{
    if (netgame::IsMultiplePlayer()) {
        return;
    }
    ForeachOnStrugglePitfall func;
    ForeachImpl(func);
}

// 0x0069B11C
void BsNpcMgr::GetRoomBounds(s32* minX, s32* minZ, s32* maxX, s32* maxZ)
{
    *minX = s_RoomMinX;
    *minZ = s_RoomMinZ;
    *maxX = s_RoomMaxX;
    *maxZ = s_RoomMaxZ;
}

// 0x0069B148
void BsNpcMgr::CreateNpcAt(ProcName name, u32 arg, u16 angleY, s32 x, s32 z)
{
    math::Angle16Vec3 rotation;
    rotation.x.value = 0;
    rotation.y.value = angleY;
    rotation.z.value = 0;
    nn::math::VEC3 position;
    FdInfo::ToCenterWorldWithUnitHeight(&position, x, z);
    oml::framework::Process* npc = Actor::Create(name, this, arg, &position, &rotation);
    if (npc != NULL) {
        m_Npcs.PushBackFast(static_cast<AcNpc*>(npc));
    }
}

// 0x0069B1CC
bool BsNpcMgr::CreateQueuedNpcsKeep()
{
    if (m_QueuedNpcs.last == m_QueuedNpcs.first) {
        return false;
    }
    stage::Command command = {3, 0, NULL};
    command.count = m_QueuedNpcs.GetSize();
    command.data = m_QueuedNpcs.first;
    CreateSpecialNpcs(&command);
    return true;
}

// 0x0069B23C
AcNpc* BsNpcMgr::FindNpcByName(u32 value, u16 name)
{
    for (u32 i = 0; i < 10; i++) {
        if (i >= m_Npcs.GetSize()) {
            continue;
        }
        AcNpc* npc = m_Npcs.first[i];
        if (npc == NULL || npc->m_Index != name) {
            continue;
        }
        if (npc->m_Unknown1030 == value) {
            return npc;
        }
    }
    return NULL;
}

// 0x0069B33C
void BsNpcMgr::Func69B33C(u32 dx, u32 dz)
{
    const s32 x0 = g_Unknown994484.m_X;
    const s32 z0 = g_Unknown994484.m_Z;
    const s32 x1 = x0 + 16;
    const s32 z1 = z0 + 16;
    if (dx < 16) {
        for (s32 x = x0 + dx; x < x1; x++) {
            for (s32 z = z0; z < z1; z++) {
                g_Unknown994484.Func7CFF58(x, z);
            }
        }
    }
    if (dz < 16) {
        for (s32 x = x0; x < x1; x++) {
            for (s32 z = z0 + dz; z < z1; z++) {
                g_Unknown994484.Func7CFF58(x, z);
            }
        }
    }
}

// 0x0069B3F4
s32 BsNpcMgr::PassToNpcOfKindF0(u8 kind)
{
    for (AcNpc** it = m_Npcs.first; it != m_Npcs.last; it++) {
        AcNpc* npc = *it;
        if (npc != NULL && npc->m_Kind == kind) {
            return npc->vf_0xF0();
        }
    }
    return 0;
}

// 0x0069B44C
s32 BsNpcMgr::PassToGuardOwner(u32 a, u32 b)
{
    if (netgame::IsMultiplePlayer()) {
        return 0;
    }
    if (m_GuardOwner == NULL) {
        return 0;
    }
    return m_GuardOwner->vf_0xC8(a, b);
}

// 0x0069B49C
void BsNpcMgr::BuildRoomFloor()
{
    for (s32 z = 0; z < 16; z++) {
        g_RoomFloor[z] = 0;
    }
    s_RoomMinX = 16;
    s_RoomMinZ = 16;
    s_RoomMaxX = 0;
    s_RoomMaxZ = 0;
    stage::GetCurrent();
    // the rooms of the player houses keep two units free
    s32 freeX0 = -1;
    s32 freeX1 = -1;
    s32 freeZ0 = -1;
    s32 freeZ1 = -1;
    const stage::Name current = stage::GetCurrent();
    if (current == 3 || current == 9 || current == 15 || current == 21) {
        freeZ0 = 14;
        freeX0 = 6;
        freeX1 = 9;
        freeZ1 = 14;
    }
    if (!stage::IsKind(stage::KIND_INDOOR, stage::GetCurrent())) {
        return;
    }
    for (s32 x = 0; x < 16; x++) {
        const u16 bit = 1 << x;
        for (s32 z = 0; z < 16; z++) {
            if (FdInfo::GetUnitHeight(x, z, FIELD_NAME_CURRENT) != 0.0f) {
                continue;
            }
            if ((x == freeX0 && z == freeZ0) || (x == freeX1 && z == freeZ1)) {
                continue;
            }
            g_RoomFloor[z < 16 ? z : 0] |= bit;
            if (s_RoomMinX > x) {
                s_RoomMinX = x;
            }
            if (s_RoomMaxX < x) {
                s_RoomMaxX = x;
            }
            if (s_RoomMinZ > z) {
                s_RoomMinZ = z;
            }
            if (s_RoomMaxZ < z) {
                s_RoomMaxZ = z;
            }
        }
    }
}

// 0x0069B60C
void BsNpcMgr::FinishAsrWordList()
{
    g_AsrWordList.append(sead::SafeStringBase<char>("END"), -1);
    const s32 length = g_AsrWordList.getLength();
    const s32 num = g_AsrWordList.getLength();
    char* text = g_AsrWordList.getBuffer();
    for (s32 i = num; i > 0; i--, text++) {
        if (*text == '\n') {
            *text = '\0';
        }
    }
    s_AsrWordListSize = length + 1;
}

// 0x0069B71C
bool BsNpcMgr::PickAprilFoolsVillager()
{
    if (netgame::IsMultiplePlayer()) {
        return false;
    }
    if (!event::IsActive(event::EVENT_ID_APRIL_FOOLS_DAY)) {
        return false;
    }
    SvPlayer* host = SvGetNetPlayer(netgame::PLAYER_NO_0);
    if (host == NULL || !host->playerFlags.finishedFirstDay) {
        return false;
    }
    const u8* flags = reinterpret_cast<const u8*>(&host->unk_0x8F77) + 17;
    if ((*flags & 0x40) && ((*flags >> 2) & 0xF) < 10 && !netgame::IsMultiPlayer()) {
        return false;
    }
    if ((*flags & 0x40) && !(*flags >> 7)) {
        return true;
    }
    if (time::GetCurrentDate()->day != 1) {
        return false;
    }
    if (*flags & 0x40) {
        return true;
    }
    BitSet32 set;
    set.Init(10, false);
    Func18D83C(&set, true);
    if (set.count < 3) {
        return false;
    }
    // the villager with the smallest value (a personality before none; ties at random)
    void* npcs = GetGardenPlus()->data.npcData.npcs;
    bool isChosen = false;
    s32 best = -1;
    u32 bestValue = 6400;
    s32 ties = 0;
    u32 bestPersonality = 8;
    for (s32 i = 0; i < 10; i++) {
        if (!set.Test(i)) {
            continue;
        }
        SvNpc* npc = static_cast<SvNpc*>(event::Func300928(npcs, i));
        if (npc != NULL && npc->unk_0x250F == 2) {
            return true;
        }
        if (isChosen) {
            continue;
        }
        u8* extra = static_cast<u8*>(Func2FE4A0(i));
        if (extra == NULL) {
            continue;
        }
        if (*reinterpret_cast<u32*>(extra + 68) & 0x80) {
            isChosen = true;
            continue;
        }
        const u32 value = Func119FFC(npc, 1);
        if (value < bestValue) {
            bestValue = value;
            best = i;
            ties = 0;
            bestPersonality = Func2FE510(npc);
            continue;
        }
        if (value != bestValue) {
            continue;
        }
        const u32 personality = Func2FE510(npc);
        if (bestPersonality == 0) {
            if (personality != 0) {
                best = i;
                bestPersonality = personality;
                ties = 0;
            }
        } else if (personality == 0) {
            continue;
        }
        ties++;
        if (Func2FF638(ties) == 0) {
            best = i;
        }
    }
    if (isChosen) {
        return true;
    }
    SvNpc* npc = Func305AC0(GetGardenData()->GetNpcData()->npcs, best);
    u8* extra = static_cast<u8*>(Func2FE4A0(best));
    if (npc != NULL && extra != NULL) {
        Func113A14(npc, 2);
        *reinterpret_cast<u32*>(extra + 68) |= 0x80;
    }
    return true;
}

// 0x0069B99C
bool BsNpcMgr::Func69B99C()
{
    if (netgame::IsMultiPlayer()) {
        return false;
    }
    nn::fnd::DateTimeParameters now;
    time::Func3004A8(&now);
    nn::fnd::DateTimeParameters date = now;
    if (!Func751014(&date, 0)) {
        return false;
    }
    // from 6:00 (on Sundays from 12:00)
    if (time::GetCurrentDate()->hour < 6) {
        return false;
    }
    if (time::GetCurrentDate()->dayOfWeek == nn::fnd::DAY_OF_WEEK_SUNDAY && time::GetCurrentDate()->hour < 12) {
        return false;
    }
    if (Func630428()) {
        return false;
    }
    SvPlayer* player = SvGetCurrentPlayer();
    if (player == NULL) {
        return true;
    }
    if (!player->playerFlags.finishedFirstDay) {
        return false;
    }
    return Func716C78(&player->badges);
}

// 0x0069BA70
bool BsNpcMgr::Func69BA70()
{
    // the structure 4 lies in the rectangle of units of Func6FBF80 (12 around)
    const nn::math::VEC3* structure = Func5C37B0(4, true);
    if (structure == NULL) {
        return false;
    }
    const f32 x = structure->x;
    const f32 z = structure->z;
    FieldRect rect;
    Func6FBF80(&rect);
    const f32 unit = FdInfo::UNIT_WORLD_SIZE_XF;
    const f32 minX = rect.min.x * unit - 12.0f;
    const f32 minZ = rect.min.y * unit - 12.0f;
    const f32 maxX = (rect.max.x + 1) * unit + 12.0f;
    const f32 maxZ = (rect.max.y + 1) * unit + 12.0f;
    return x > minX && x < maxX && z > minZ && z < maxZ;
}

// 0x0069BB84
void BsNpcMgr::CreatePrologueNpcs()
{
    if (stage::IsKind(stage::KIND_SCREEN, stage::GetCurrent())) {
        return;
    }
    SvPlayer* player = SvGetCurrentPlayer();
    if (player == NULL) {
        return;
    }
    if (player->playerFlags.houseLocationChosen) {
        // Isabelle plants the tree of the town
        if (!s_State.unk0E) {
            return;
        }
        s32 x;
        s32 z;
        Func6FB2BC(&x, &z);
        math::Angle16Vec3 rotation = {0, 0xC000, 0};
        nn::math::VEC3 center;
        FdInfo::ToCenterWorld(&center, x, z);
        nn::math::VEC3_ position;
        position.x = center.x + g_Unknown8822A4;
        position.y = center.y;
        position.z = center.z - g_Unknown8822A8;
        stage::NpcInfo info(stage::NpcInfo::KIND_MODEL, 0, "sza");
        oml::framework::Process* npc = Actor::Create(PROC_AC_NPC_SP_SECRETARY_PLANT_TREE, this,
                                                     reinterpret_cast<u32>(&info), &position, &rotation);
        if (npc != NULL) {
            m_Npcs.PushBackFast(static_cast<AcNpc*>(npc));
        }
        return;
    }
    if (player->playerFlags.chooseHouseLocation) {
        // Tom Nook shows the places for the house
        nn::math::VEC3 position;
        FdInfo::ToCenterWorldWithUnitHeight(&position, 59, 6);
        math::Angle16Vec3 rotation = {0, 0, 0};
        stage::NpcInfo info(stage::NpcInfo::KIND_MODEL, 0, "rco");
        oml::framework::Process* npc = Actor::Create(PROC_AC_NPC_SP_PROLOGUE_TANUKICHI, this,
                                                     reinterpret_cast<u32>(&info), &position, &rotation);
        if (npc != NULL) {
            m_Npcs.PushBackFast(static_cast<AcNpc*>(npc));
        }
        return;
    }
    if (player->playerFlags.mainStreetUnlocked || !player->playerFlags.playerSetNameAndTownName) {
        return;
    }
    // Isabelle waits in front of the train station
    nn::math::VEC3_ position;
    GetStructurePosition(&position, GetStationStructure());
    const f32 unit = FdInfo::UNIT_WORLD_SIZE_XF;
    position.z += unit * 2.5f;
    position.x += unit * 2.0f;
    math::Angle16Vec3 rotation = {0, 0x8000, 0};
    stage::NpcInfo info(stage::NpcInfo::KIND_MODEL, 0, "sza");
    oml::framework::Process* npc =
        Actor::Create(PROC_AC_NPC_SP_SECRETARY_OUT, this, reinterpret_cast<u32>(&info), &position, &rotation);
    if (npc != NULL) {
        m_Npcs.PushBackFast(static_cast<AcNpc*>(npc));
    }
    Func5C933C();
}

// 0x0069BEC8
void BsNpcMgr::CreateResetsan()
{
    SvPlayer* player = g_pSvCurrentPlayer;
    if (player == NULL) {
        return;
    }
    nn::math::VEC3_ position;
    GetStructurePosition(&position, Func106314());
    position.x += 64.0f;
    position.z += 48.0f;
    math::Angle16Vec3 rotation = {0, 0x4000, 0};
    const char* model = player->playerFlags.unk_0x0_0 == 3 ? "mob" : "mol";
    stage::NpcInfo info(stage::NpcInfo::KIND_MODEL, 0, model);
    oml::framework::Process* npc =
        Actor::Create(PROC_AC_NPC_SP_RESETSAN, this, reinterpret_cast<u32>(&info), &position, &rotation);
    if (npc != NULL) {
        m_Npcs.PushBackFast(static_cast<AcNpc*>(npc));
    }
}

namespace {
// (inline) the special character with the model in front of the structure of Func106314
inline oml::framework::Process* CreateNpcAtStructure(BsNpcMgr* mgr, ProcName name, const char* model,
                                                   const nn::math::VEC3_* position, const math::Angle16Vec3* rotation)
{
    stage::NpcInfo info(stage::NpcInfo::KIND_MODEL, 0, model);
    return Actor::Create(name, mgr, reinterpret_cast<u32>(&info), position, rotation);
}
} // namespace

// 0x0069C078
bool BsNpcMgr::CreateRequestedNpc()
{
    if (demo::CheckOrder(static_cast<demo::OrderID>(9), false)) {
        return false;
    }
    // the villager who asked (flag 0x20 of its extra data) and up to two others (flag 0x40)
    s32 villagers[3] = {-1, -1, -1};
    s32 num = 1;
    SvNpc* npcs = GetGardenData()->GetNpcData()->npcs;
    for (s32 i = 0; i < 10; i++) {
        u32* extra = static_cast<u32*>(Func2FE4A0(i));
        if (extra == NULL) {
            continue;
        }
        const u32 flags = extra[17];
        if (flags & 0x20) {
            villagers[0] = i;
            extra[17] = flags | 0x100;
            SvNpc* npc = Func305AC0(npcs, i);
            if (npc == NULL || !Func309B70(npc, 0)) {
                continue;
            }
            if (Func11972C(npcs, i, 0)) {
                Func113A14(npc, 2);
            } else {
                Func113A14(npc, 23);
            }
        } else if (flags & 0x40) {
            if (num < 3) {
                villagers[num] = i;
                num++;
            }
        }
    }
    SvPlayer* player = g_pSvCurrentPlayer;
    if (player == NULL) {
        return false;
    }
    nn::math::VEC3_ position;
    if (!GetStructurePosition(&position, Func106314())) {
        return false;
    }
    position.x += 48.0f;
    position.z += 48.0f;
    math::Angle16Vec3 rotation = {0, 0xC000, 0};
    oml::framework::Process* npc = NULL;
    bool isCreated = false;
    if (s_State.unk03) {
        npc = CreateNpcAtStructure(this, PROC_AC_NPC_SP_PERIO_TUTORIAL, "plb", &position, &rotation);
    } else if (villagers[0] != -1) {
        u32* extra = static_cast<u32*>(Func2FE4A0(villagers[0]));
        if (extra != NULL) {
            extra[17] |= 0x20;
            SvNpc* villager = Func305AC0(npcs, villagers[0]);
            if (villager != NULL) {
                Func113A0C(villager, 0);
            }
        }
        for (s32 i = 1; i < 3; i++) {
            extra = static_cast<u32*>(Func2FE4A0(villagers[i]));
            if (extra != NULL) {
                extra[17] |= 0x40;
            }
        }
        isCreated = true;
        goto clear;
    } else if (event::g_Unknown94EDF8) {
        npc = CreateNpcAtStructure(this, PROC_AC_NPC_SP_SECRETARY_ANNIVERSARY, "sza", &position, &rotation);
    } else if (s_State.unk06) {
        npc = CreateNpcAtStructure(this, PROC_AC_NPC_SP_PERIO_SPECIAL, "plb", &position, &rotation);
    } else {
        const s8 resetti = s_State.unk01;
        if (resetti != 0 && !player->playerFlags.unk_0xF_2) {
            CreateResetsan();
            isCreated = true;
            goto clear;
        }
        if (s_State.unk09) {
            npc = CreateNpcAtStructure(this, PROC_AC_NPC_SP_HONMA, "ott", &position, &rotation);
        } else if (resetti != 0) {
            CreateResetsan();
            isCreated = true;
            goto clear;
        } else if (s_State.unk04) {
            npc = CreateNpcAtStructure(this, PROC_AC_NPC_SP_PERIO_WARNING, "plb", &position, &rotation);
        } else if (s_State.unk08) {
            npc = CreateNpcAtStructure(this, PROC_AC_NPC_SP_PRE_SISYO, "upa", &position, &rotation);
        } else if (s_State.unk0A) {
            g_KotobukiAnnounceState = 3;
            npc = CreateNpcAtStructure(this, PROC_AC_NPC_SP_KOTOBUKI_ANNOUNCE, "ttl", &position, &rotation);
        } else {
            goto clear;
        }
    }
    if (npc != NULL) {
        m_Npcs.PushBackFast(static_cast<AcNpc*>(npc));
    }
    isCreated = true;
clear:
    s_State.unk01 = 0;
    s_State.unk02 = 0;
    s_State.unk03 = 0;
    s_State.unk04 = 0;
    s_State.unk06 = 0;
    s_State.unk07 = 0;
    s_State.unk08 = 0;
    s_State.unk09 = 0;
    s_State.unk0A = 0;
    s_State.unk05 = 0;
    return isCreated;
}

// 0x0069C764
bool BsNpcMgr::CreateVisitorNpc()
{
    SvPlayer* player = SvGetCurrentPlayer();
    if (player == NULL || !player->playerFlags.finishedFirstDay) {
        return false;
    }
    for (u32 i = 0; i < VISITOR_NUM; i++) {
        const VisitorNpc& entry = s_VisitorNpcs[i];
        if (netgame::IsMultiPlayer()) {
            if (!entry.isOnline || event::Func58EE68() != entry.visitor) {
                continue;
            }
        } else if (!event::Func58F754(entry.visitor)) {
            continue;
        }
        if (m_SpBanks.m_Model.m_Size == m_SpBanks.m_Model.m_UsedNum) {
            continue;
        }
        math::Angle16Vec3 rotation = {0, 0, 0};
        stage::NpcInfo info(stage::NpcInfo::KIND_MODEL, 0, entry.model);
        PlaceFunc place = s_PlaceFuncs[entry.place];
        // Gracie does not come once the flag 1.5 of the town is set
        if (entry.name == PROC_AC_NPC_SP_GRACE_OUT && GetGardenPlus()->data.townData.townFlags.unk_0x1_5) {
            return false;
        }
        nn::math::VEC3 position;
        if (place == NULL || !place(&position, &rotation, &info)) {
            continue;
        }
        oml::framework::Process* npc =
            Actor::Create(static_cast<ProcName>(entry.name), this, reinterpret_cast<u32>(&info), &position, &rotation);
        if (npc != NULL) {
            m_Npcs.PushBackFast(static_cast<AcNpc*>(npc));
        }
        return true;
    }
    return false;
}

namespace {
// (inline) a special character with the model placed by s_PlaceFuncs[place]
inline bool CreatePlacedNpc(BsNpcMgr* mgr, ProcName name, const char* model, u32 place)
{
    if (mgr->m_SpBanks.m_Model.m_Size == mgr->m_SpBanks.m_Model.m_UsedNum) {
        return false;
    }
    math::Angle16Vec3 rotation = {0, 0, 0};
    stage::NpcInfo info(stage::NpcInfo::KIND_MODEL, 0, model);
    BsNpcMgr::PlaceFunc func = BsNpcMgr::s_PlaceFuncs[place];
    nn::math::VEC3 position;
    if (func == NULL || !func(&position, &rotation, &info)) {
        return false;
    }
    oml::framework::Process* npc = Actor::Create(name, mgr, reinterpret_cast<u32>(&info), &position, &rotation);
    if (npc != NULL) {
        mgr->m_Npcs.PushBackFast(static_cast<AcNpc*>(npc));
    }
    return true;
}
} // namespace

// 0x0069FD44
DECOMP_EMIT_COPY inline bool BsNpcMgr::HasCleaningVisit()
{
    SvPlayer* host = SvGetNetPlayer(netgame::PLAYER_NO_0);
    if (host == NULL) {
        GetGardenData()->townData.townFlags.unk_0xA_6 = false;
        return false;
    }
    if (event::IsActive(event::EVENT_ID_WEEDING_DAY)) {
        return false;
    }
    const u8 bits = host->playerFlags.unk_0x19_1 | host->playerFlags.unk_0x19_2 | host->playerFlags.unk_0x19_3;
    if (!(bits & (reinterpret_cast<const u8*>(&host->playerFlags)[1] >> 1))) {
        GetGardenData()->townData.unk_0x1CA8C[4] &= ~0x80;
        GetGardenData()->townData.townFlags.unk_0xA_6 = false;
    }
    return (GetGardenPlus()->data.townData.unk_0x1CA8C[4] & 0x80) != 0;
}

// 0x0069C944
bool BsNpcMgr::CreateSpecialDayNpc()
{
    SvPlayer* player = SvGetCurrentPlayer();
    if (player == NULL || !player->playerFlags.finishedFirstDay) {
        return false;
    }
    // April Fools: AcNpcSpMysteryCat (else the stored position of a villager is updated)
    if (PickAprilFoolsVillager()) {
        if (CreatePlacedNpc(this, PROC_AC_NPC_SP_MYSTERY_CAT, "mka", 2)) {
            return true;
        }
    } else {
        const u16 index = Func724650(&g_UnknownA8513C, "mka");
        if (index != 0xFFFF && g_UnknownA8513C.entries != NULL && index < 72) {
            const UnknownA8513C::Entry* entry = &g_UnknownA8513C.entries[index];
            if (entry != NULL && entry->villager >= 0) {
                Func2069EC(&GetNpcPositions()->GetEntry(entry->villager + 25));
            }
        }
    }
    // the time of Func3189EC 14: AcNpcSpPumpkingPre
    if (IsPumpkingPreTime()) {
        if (CreatePlacedNpc(this, PROC_AC_NPC_SP_PUMPKING_PRE, "pkn", 1)) {
            return true;
        }
    }
    // AcNpcSpCleaningVisit (a flag of the town that the Weeding Day sets)
    if (HasCleaningVisit() && m_SpBanks.m_Model.m_Size != m_SpBanks.m_Model.m_UsedNum) {
        math::Angle16Vec3 rotation = {0, 0, 0};
        nn::math::VEC3 position;
        GetStructurePosition(&position, Func106314());
        stage::NpcInfo info(stage::NpcInfo::KIND_MODEL, 0, "slo");
        PlaceFunc func = s_PlaceFuncs[6];
        if (func != NULL && func(&position, &rotation, &info)) {
            oml::framework::Process* npc =
                Actor::Create(PROC_AC_NPC_SP_CLEANING_VISIT, this, reinterpret_cast<u32>(&info), &position, &rotation);
            if (npc != NULL) {
                m_Npcs.PushBackFast(static_cast<AcNpc*>(npc));
            }
            return true;
        }
    }
    // AcNpcSpMaiko
    if (Func699598()) {
        if (CreatePlacedNpc(this, PROC_AC_NPC_SP_MAIKO, "lom", 1)) {
            return true;
        }
    }
    // AcNpcSpPalloncino (the time of Func69B99C)
    if (Func69B99C()) {
        if (CreatePlacedNpc(this, PROC_AC_NPC_SP_PALLONCINO, "bln", 1)) {
            return true;
        }
    }
    // AcNpcSpYutarouVisit while the player has no lamp (not on Sunday mornings)
    if (netgame::IsMultiPlayer()) {
        return false;
    }
    if (time::GetCurrentDate()->dayOfWeek == nn::fnd::DAY_OF_WEEK_SUNDAY && time::GetCurrentDate()->hour >= 6 &&
        time::GetCurrentDate()->hour < 12) {
        return false;
    }
    player = SvGetCurrentPlayer();
    if (player != NULL && (!player->playerFlags.finishedFirstDay || !player->playerFlags.unk_0x12_1)) {
        return false;
    }
    if (GetGardenPlus()->data.townData.townFlags.unk_0xE_0 || GetGardenPlus()->data.townData.townFlags.unk_0xD_6) {
        return false;
    }
    if (HasYutaroLamp()) {
        return false;
    }
    return CreatePlacedNpc(this, PROC_AC_NPC_SP_YUTAROU_VISIT, "gst", 7);
}

// 0x0069D290
s32 BsNpcMgr::PassToNpcOfKind57A600(u8 kind, u32 arg)
{
    for (AcNpc** it = m_Npcs.first; it != m_Npcs.last; it++) {
        AcNpc* npc = *it;
        if (npc != NULL && npc->m_Kind == kind) {
            return npc->Func57A600(arg);
        }
    }
    return 0;
}

// 0x0069D2F4
void BsNpcMgr::ClearState2()
{
    s_Unknown95115C = 0;
    s_State.unk00 = stage::NAME_MAX;
}

// 0x0069D310
bool BsNpcMgr::IsPumpkingPreTime()
{
    SvPlayer* player = SvGetCurrentPlayer();
    if (Func3189EC() != 14) {
        return false;
    }
    if (player == NULL || (reinterpret_cast<const u8*>(player)[0x8A6B] & 1)) {
        if (!(reinterpret_cast<const u8*>(&player->unk_0x8F77)[6] & 4)) {
            return false;
        }
    }
    return true;
}

// 0x0069EAC0
bool BsNpcMgr::QueueNpc(u16 name, const stage::NpcInfo* info, const nn::math::VEC3_* position,
                        const math::Angle16Vec3* rotation)
{
    if (m_QueuedNpcs.IsFull()) {
        return false;
    }
    stage::NpcEntry entry;
    entry.name = name;
    entry.position = *position;
    entry.rotation = *rotation;
    entry.info = *info;
    m_QueuedNpcs.PushBackFast(entry);
    return true;
}

// 0x0069EB60
bool BsNpcMgr::CreateWalkingVillagersE()
{
    // five villagers stand around the place where the tree is planted and look at it
    const s32 offsets[5][2] = {{-4, -1}, {-3, -2}, {-2, -3}, {3, -2}, {4, -1}};
    s32 x;
    s32 z;
    Func6FB2BC(&x, &z);
    void* npcs = GetGardenPlus()->data.npcData.npcs;
    const f32 distance = FdInfo::UNIT_WORLD_SIZE_XF * 3.5f;
    for (s32 i = 0, num = 0; num < 5 && i < 10; i++) {
        SvNpc* npc = static_cast<SvNpc*>(event::Func300928(npcs, i));
        if (npc == NULL) {
            continue;
        }
        const s32 dx = offsets[num][0];
        const s32 dz = offsets[num][1];
        const math::Angle16 angle = math::Angle16::Atan2(static_cast<f32>(-dx), static_cast<f32>(-dz));
        stage::NpcInfo info;
        info.kind = stage::NpcInfo::KIND_ID;
        info.flag = 0;
        info.value = npc->npc.villagerId | (static_cast<u8>(i) << 16);
        math::Angle16Vec3 rotation;
        rotation.x.value = 0;
        rotation.y = angle;
        rotation.z.value = 0;
        nn::math::VEC3 position;
        FdInfo::ToCenterWorldWithUnitHeight(&position, x, z);
        nn::math::VEC3 offset(static_cast<f32>(dx), 0.0f, static_cast<f32>(dz));
        if (ssys::ma::Normalize(&offset)) {
            sead::Vector3CalcCtr<f32>::multScalar(offset, offset, distance);
        }
        position.x += offset.x;
        position.y += offset.y;
        position.z += offset.z;
        oml::framework::Process* actor =
            Actor::Create(PROC_AC_NPC_DEMO_DOLL, this, reinterpret_cast<u32>(&info), &position, &rotation);
        if (actor != NULL) {
            m_Npcs.PushBackFast(static_cast<AcNpc*>(actor));
        }
        num++;
    }
    return true;
}

namespace {
// (inline) a unit into the list of candidates (copy constructed in place)
inline void PushUnit(BsNpcMgr::Vector<Vector2i>& units, s32 x, s32 z)
{
    units.PushBackFast(Vector2i(x, z));
}
} // namespace

// 0x0069D368
bool BsNpcMgr::CreateWalkingVillagersF()
{
    // the units around the structure of Func30CEDC where the villagers may stand, by the kind of the structure
    void* object = Func30CEDC();
    const u32 kind = Func302704(object);
    const s32 x = Func718E6C(object);
    const s32 z = Func718E90(object);
    alignas(4) u8 buffer[16 * sizeof(Vector2i)];
    Vector<Vector2i> units(reinterpret_cast<Vector2i*>(buffer), 16);
    if (Func6CD484(kind)) {
        switch (Func6CC2E0(kind)) {
        case 0:
            PushUnit(units, x - 1, z - 3);
            PushUnit(units, x + 2, z - 3);
            PushUnit(units, x - 1, z + 2);
            PushUnit(units, x + 2, z + 2);
            PushUnit(units, x, z - 3);
            PushUnit(units, x + 1, z - 3);
            break;
        case 1:
            PushUnit(units, x + 2, z - 2);
            PushUnit(units, x - 2, z + 2);
            PushUnit(units, x + 3, z - 1);
            PushUnit(units, x - 3, z + 1);
            PushUnit(units, x + 3, z - 2);
            PushUnit(units, x - 3, z + 2);
            break;
        case 2:
            PushUnit(units, x - 2, z - 2);
            PushUnit(units, x + 2, z + 2);
            PushUnit(units, x - 3, z - 1);
            PushUnit(units, x + 3, z + 1);
            PushUnit(units, x - 3, z - 2);
            PushUnit(units, x + 3, z + 2);
            break;
        case 3:
            PushUnit(units, x - 1, z - 1);
            PushUnit(units, x + 1, z - 1);
            PushUnit(units, x - 2, z);
            PushUnit(units, x + 2, z);
            PushUnit(units, x - 2, z - 1);
            PushUnit(units, x + 2, z - 1);
            break;
        default:
            break;
        }
    } else if (Func5CAF48() || IsStationStructure(kind)) {
        PushUnit(units, x + 3, z + 1);
        PushUnit(units, x - 3, z + 1);
        PushUnit(units, x + 2, z + 2);
        PushUnit(units, x - 2, z + 2);
        PushUnit(units, x + 3, z + 3);
        PushUnit(units, x - 3, z + 3);
        PushUnit(units, x + 2, z + 1);
        PushUnit(units, x - 2, z + 1);
        PushUnit(units, x + 3, z + 2);
        PushUnit(units, x - 3, z + 2);
        PushUnit(units, x + 2, z + 3);
        PushUnit(units, x - 2, z + 3);
    } else {
        // in front of the sides of the rectangle of the structure
        const s8* rect = Func5CAD04(kind);
        const s32 left = rect[0] + x;
        const s32 right = rect[2] + x;
        const s32 front = rect[3] + z;
        const s32 width = right - left;
        if (width <= 2) {
            PushUnit(units, left - 2, front);
            PushUnit(units, right + 2, front);
            PushUnit(units, left - 2, front - 2);
            PushUnit(units, right + 2, front - 2);
            PushUnit(units, left - 1, front - 1);
            PushUnit(units, right + 1, front - 1);
            PushUnit(units, left - 1, front - 3);
            PushUnit(units, right + 1, front - 3);
            PushUnit(units, left - 1, front);
            PushUnit(units, right + 1, front);
            PushUnit(units, left - 1, front - 2);
            PushUnit(units, right + 1, front - 2);
            PushUnit(units, left - 2, front - 1);
            PushUnit(units, right + 2, front - 1);
            PushUnit(units, left - 2, front - 3);
            PushUnit(units, right + 2, front - 3);
        } else if (width > 5) {
            PushUnit(units, left, front);
            PushUnit(units, right, front);
            PushUnit(units, left, front - 1);
            PushUnit(units, right, front - 1);
            PushUnit(units, left, front - 2);
            PushUnit(units, right, front - 2);
            PushUnit(units, left, front - 3);
            PushUnit(units, right, front - 3);
            PushUnit(units, left - 1, front);
            PushUnit(units, right + 1, front);
            PushUnit(units, left - 1, front - 2);
            PushUnit(units, right + 1, front - 2);
            PushUnit(units, left - 1, front - 1);
            PushUnit(units, right + 1, front - 1);
            PushUnit(units, left - 1, front - 3);
            PushUnit(units, right + 1, front - 3);
        } else {
            PushUnit(units, left - 1, front);
            PushUnit(units, right + 1, front);
            PushUnit(units, left - 1, front - 2);
            PushUnit(units, right + 1, front - 2);
            PushUnit(units, left, front - 1);
            PushUnit(units, right, front - 1);
            PushUnit(units, left, front - 3);
            PushUnit(units, right, front - 3);
            PushUnit(units, left, front);
            PushUnit(units, right, front);
            PushUnit(units, left, front - 2);
            PushUnit(units, right, front - 2);
            PushUnit(units, left - 1, front - 1);
            PushUnit(units, right + 1, front - 1);
            PushUnit(units, left - 1, front - 3);
            PushUnit(units, right + 1, front - 3);
        }
    }

    // up to four villagers at home (not the walking ones) stand on the first free candidates and look at the
    // structure
    void* npcs = GetGardenPlus()->data.npcData.npcs;
    s32 num = 0;
    Vector2i* it = units.first;
    Vector2i* end = units.last;
    nn::math::VEC3 center = VEC3_ZERO;
    Func3361FC(&center, kind);
    u8 walking[2] = {0, 0};
    for (s32 i = 0; i < 3; i++) {
        const s32 index = Func118F30(npcs, i);
        if (index != -1 && static_cast<u32>(index) < 10) {
            walking[index / 8] |= 1 << (index % 8);
        }
    }
    const f32 limit = Func6FA0C4() - FdInfo::UNIT_WORLD_SIZE_ZF;
    for (s32 i = 0; i < 10; i++) {
        SvNpc* npc = static_cast<SvNpc*>(event::Func300928(npcs, i));
        if (npc == NULL) {
            continue;
        }
        if (static_cast<u32>(i) < 10 && ((walking[i / 8] >> (i % 8)) & 1)) {
            continue;
        }
        if (npc->unk_0x250F != 0) {
            continue;
        }
        for (; it != end; it++) {
            const s32 unitX = it->x;
            const s32 unitZ = it->y;
            const f32 height = FdInfo::GetUnitHeight(unitX, unitZ, FIELD_NAME_CURRENT);
            if (!Func51D0FC(unitX, unitZ)) {
                continue;
            }
            if (!FdInfo::IsFlat(unitX, unitZ, FIELD_NAME_CURRENT) &&
                !FdInfo::HasPartIn891697(unitX, unitZ, FIELD_NAME_CURRENT)) {
                continue;
            }
            if (!(height > limit)) {
                continue;
            }
            const s32 dx = static_cast<s32>((unitX - x) * FdInfo::UNIT_WORLD_SIZE_XF);
            const s32 dz = static_cast<s32>((unitZ - z) * FdInfo::UNIT_WORLD_SIZE_XF);
            nn::math::VEC3 offset = VEC3_ZERO;
            Func150370(&offset, Func302704(object));
            const math::Angle16 angle =
                math::Angle16::Atan2(center.x + static_cast<f32>(-dx), center.z + static_cast<f32>(-dz));
            stage::NpcInfo info;
            info.kind = stage::NpcInfo::KIND_ID;
            info.flag = 0;
            info.value = npc->npc.villagerId | (static_cast<u8>(i) << 16);
            math::Angle16Vec3 rotation;
            rotation.x.value = 0;
            rotation.y = angle;
            rotation.z.value = 0;
            nn::math::VEC3 position;
            FdInfo::ToCenterWorldWithUnitHeight(&position, unitX, unitZ);
            oml::framework::Process* actor =
                Actor::Create(PROC_AC_NPC_DEMO_DOLL, this, reinterpret_cast<u32>(&info), &position, &rotation);
            if (actor != NULL) {
                m_Npcs.PushBackFast(static_cast<AcNpc*>(actor));
            }
            it++;
            num++;
            break;
        }
        if (it == end || num >= 4) {
            break;
        }
    }
    return true;
}

namespace {
// (inline) a villager whose state is not one of the steady ones goes home (or to state 0 without unk_0x250E)
inline void SettleVillager(SvNpc* npc)
{
    switch (npc->unk_0x250F) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 23:
        break;
    default:
        Func113A14(npc, npc->unk_0x250E ? 2 : 0);
        break;
    }
}
} // namespace

// 0x0069ED50
bool BsNpcMgr::Func69ED50(BitSet32* set, bool isControlled)
{
    SvNpc* npcs = GetGardenData()->GetNpcData()->npcs;
    const s32 pairA = Func119244(npcs);
    const s32 pairB = Func119378(npcs);
    if (Func51E3DC() >= 2) {
        Func51E3EC(-1);
        u32* extraA = static_cast<u32*>(Func2FE4A0(pairA));
        if (extraA != NULL) {
            extraA[17] &= ~0x10;
        }
        u32* extraB = static_cast<u32*>(Func2FE4A0(pairB));
        if (extraB != NULL) {
            SvNpc* npc = Func305AC0(npcs, pairB);
            extraB[17] &= ~8;
            if (npc != NULL) {
                Func113A0C(npc, 2);
            }
        }
    } else {
        if (pairA != -1) {
            set->Add(pairA);
        }
        if (pairB != -1) {
            set->Add(pairB);
        }
    }

    // the villagers go in and out of their houses
    if (!(netgame::IsMultiplePlayer() && !isControlled)) {
        const u8* online = static_cast<const u8*>(netgame::g_pUnknown95488C);
        for (s32 i = 0; i < 10; i++) {
            SvNpc* npc = Func305AC0(npcs, i);
            if (npc == NULL) {
                continue;
            }
            const u8 state = npc->unk_0x250F;
            bool isDone = true;
            switch (state) {
            case 5:
                isDone = Func112104();
                break;
            case 6:
                isDone = Func111FA8();
                break;
            case 8:
            case 22:
                isDone = Func112004();
                break;
            case 21:
                isDone = Func1120B8();
                break;
            default:
                break;
            }
            if (!isDone) {
                Func113A14(npc, 2);
            }
            if (Func309B70(npc, 0)) {
                if (Func11972C(npcs, i, 0)) {
                    if (online != NULL && !(static_cast<u32>(i) < 10 && ((online[0x98 + i / 8] >> (i % 8)) & 1))) {
                        Func113A14(npc, 2);
                    }
                } else {
                    if (i == pairA || i == pairB) {
                        SvNpc* npcA = Func305AC0(npcs, pairA);
                        SvNpc* npcB = Func305AC0(npcs, pairB);
                        u32* extraA = static_cast<u32*>(Func2FE4A0(pairA));
                        u32* extraB = static_cast<u32*>(Func2FE4A0(pairB));
                        if (npcA != NULL && npcB != NULL && extraA != NULL && extraB != NULL) {
                            Func113A14(npcA, 2);
                            Func113A14(npcB, 2);
                            extraA[17] &= ~0x10;
                            extraB[17] &= ~8;
                            set->Remove(pairA);
                            set->Remove(pairB);
                        }
                    }
                    Func113A14(npc, 23);
                    u8 message[2] = {static_cast<u8>(i), 0};
                    Func7D2190(4, message);
                }
                if (Func51D8A4() == i) {
                    Func2FE520(-1);
                }
            } else if (npc->unk_0x250F == 23) {
                Func113A14(npc, 2);
                u8 message[2] = {static_cast<u8>(i), 1};
                Func7D2190(4, message);
            }
        }
    }
    if (isControlled) {
        Func6A0F9C(set);
        return true;
    }

    // the villagers of the club (meaning of Func7179B8 / Func717DA4 unknown)
    SvTownDataUnkEE2C* shops = &GetGardenPlus()->data.townData.unk_0xEE2C;
    u8* club = &shops->clubLolUnlockState;
    if (club != NULL && Func7179B8(club)) {
        for (s32 i = 0; i < 10; i++) {
            SvNpc* npc = Func305AC0(npcs, i);
            if (npc == NULL || Func717DA4(club, npc->npc.villagerId)) {
                continue;
            }
            SettleVillager(npc);
            set->Add(i);
        }
    }
    // in some stages a visiting villager (state 5) comes back half of the time
    const stage::Name current = stage::GetCurrent();
    if (current == 38 || current == 2 || current == 1 || stage::IsOwnHouseRoom(stage::GetCurrent())) {
        if (Func2FF638(100) < 50) {
            for (s32 i = 0; i < 10; i++) {
                SvNpc* npc = Func305AC0(npcs, i);
                if (npc == NULL || npc->unk_0x250F != 5) {
                    continue;
                }
                if (npc->unk_0x250E != 0) {
                    Func113A14(npc, 2);
                } else {
                    Func113A14(npc, 0);
                    nn::math::VEC3_ position;
                    if (GetStructurePosition(&position, 88)) {
                        NpcPositionEntry& entry = GetNpcPositions()->GetEntry(i);
                        entry.Set(static_cast<u32>(position.x), static_cast<u32>(position.z + FdInfo::UNIT_WORLD_SIZE_XF));
                        entry.stage = 0;
                    }
                }
                break;
            }
        }
    }
    // the cafe: the villagers who worked there (state 6) go home on the days of Func3094C0
    if (current == stage::NAME_CAFE) {
        const nn::fnd::DateTimeParameters* date = time::GetCurrentDate();
        if (Func3094C0(reinterpret_cast<u8*>(GetGardenPlus()) + 0x70EE0, date)) {
            for (s32 i = 0; i < 10; i++) {
                SvNpc* npc = Func305AC0(npcs, i);
                if (npc != NULL && npc->unk_0x250F == 6) {
                    Func113A0C(npc, 2);
                }
            }
        }
    }
    // the villagers with a wish of the host (Func30E90C)
    SvPlayer* host = SvGetNetPlayer(netgame::PLAYER_NO_0);
    u8* wish = reinterpret_cast<u8*>(npcs) + 0x173C6;
    for (s32 i = 0; i < 10; i++) {
        SvNpc* npc = Func305AC0(npcs, i);
        if (npc == NULL) {
            continue;
        }
        const Bits64 copy = Func30E90C(npcs, npc, 2, 1, 0, 0);
        s32 count = 0;
        for (u32 bit = 0; bit < 33; bit++) {
            if (((copy.bytes[bit >> 3] >> (bit & 7)) & 1) == 1) {
                count++;
            }
        }
        if (count == 0) {
            if (host == NULL || Func11A7DC(npc, &host->playerInfo)) {
                continue;
            }
        }
        set->Add(i);
        SettleVillager(npc);
        if (!(copy.bytes[2] & 1)) {
            continue;
        }
        if (Func309DDC(wish)) {
            Func113A14(npc, 2);
            wish[31] = 2;
        } else if (wish[31] == 0 && Func75863C(wish, time::GetCurrentDate(), 15)) {
            wish[31] = 2;
        }
    }
    // April Fools: the chosen villager stays at home
    if (event::IsActive(event::EVENT_ID_APRIL_FOOLS_DAY)) {
        const s8 chosen = static_cast<const s8*>(netgame::g_pUnknown95488C)[0x9A];
        if (chosen == -1) {
            for (s32 i = 0; i < 10; i++) {
                u32* extra = static_cast<u32*>(Func2FE4A0(i));
                if (extra == NULL || !(extra[17] & 0x80)) {
                    continue;
                }
                SvNpc* npc = Func305AC0(npcs, i);
                if (npc != NULL && npc->unk_0x250F != 23) {
                    Func113A14(npc, 2);
                    set->Add(i);
                }
                break;
            }
        } else {
            set->Add(chosen);
            SvNpc* npc = Func305AC0(npcs, chosen);
            if (npc != NULL) {
                Func113A0C(npc, 2);
            }
        }
    }
    // the villagers take part in some events
    if (event::IsActive(event::EVENT_ID_HALLOWEEN) || event::IsActive(event::EVENT_ID_HARVEST_FESTIVAL) ||
        event::IsActive(event::EVENT_ID_TOY_DAY) || event::IsActive(event::EVENT_ID_FESTIVALE)) {
        for (s32 i = 0; i < 10; i++) {
            SvNpc* npc = Func305AC0(npcs, i);
            if (npc == NULL) {
                continue;
            }
            SettleVillager(npc);
            set->Add(i);
        }
    }
    // the villager who asked for a visit (flag 0x20) is at home in its house, else outside
    if (Func30FCCC() != -1) {
        for (s32 i = 0; i < 10; i++) {
            u32* extra = static_cast<u32*>(Func2FE4A0(i));
            if (extra == NULL || !(extra[17] & 0x20)) {
                continue;
            }
            SvNpc* npc = Func305AC0(npcs, i);
            if (npc != NULL) {
                if (stage::GetNpcHouseIndex(stage::GetCurrent()) == i) {
                    Func113A14(npc, 2);
                } else {
                    Func113A14(npc, 0);
                }
            }
            set->Add(i);
        }
        return true;
    }
    Func6A0F9C(set);
    if (m_Timer != 0 || netgame::IsMultiplePlayer()) {
        return true;
    }
    SvNpcData* data = GetGardenData()->GetNpcData();
    if (!Func309DC8(reinterpret_cast<u8*>(data) + 0x17436)) {
        return false;
    }
    return reinterpret_cast<const u8*>(&data->unk_0x173F0)[79] < 4;
}

// the special characters in the word list (built by a static initializer of several files)
// 0x0099413C
BsNpcMgr::AsrSpNpc BsNpcMgr::s_AsrSpNpcs[ASR_SP_NPC_NUM] = {
    {"sza", 0, 1, false},  {"rco", 2, 1, false},  {"fob", 3, 1, false},  {"owl", 5, 3, false},  {"pkn", 6, 2, false},
    {"end", 7, 3, false},  {"alw", 12, 3, false}, {"alp", 13, 3, false}, {"ows", 14, 3, false}, {"LRN", 15, 3, false},
    {"lrn", 16, 1, false}, {"mol", 17, 3, false}, {"mob", 18, 3, false}, {"mnk", 19, 3, false}, {"kpp", 20, 1, false},
    {"seg", 21, 4, false}, {"wrl", 22, 1, false}, {"pge", 23, 3, false}, {"ott", 24, 1, false}, {"lom", 25, 1, false},
    {"slo", 26, 1, false}, {"pyn", 27, 1, false}, {"seo", 28, 3, false}, {"cml", 29, 1, false}, {"tuk", 30, 1, false},
    {"grf", 31, 1, false}, {"upa", 32, 1, false}, {"bpt", 34, 3, false}, {"plb", 35, 1, false}, {"snt", 36, 1, false},
    {"bev", 37, 1, false}, {"chm", 42, 1, false}, {"pck", 43, 1, false}, {"tap", 47, 1, false}, {"bln", 51, 1, false},
    {"boa", 53, 1, false}, {"plc", 54, 3, false}, {"pla", 55, 3, false}, {"ttl", 64, 1, false}, {"dmy", 67, 5, true},
    {"spn", 73, 1, false}, {"lrm", 16, 1, false},
};

// 0x0069F7B4
bool BsNpcMgr::FinalizeBanks()
{
    bool isAll = m_NmlBanks.Finalize();
    isAll &= m_SpBanks.Finalize();
    if (isAll) {
        m_Npcs.Clear();
    }
    return isAll;
}

// 0x0069F958
bool BsNpcMgr::SetupAsrWords()
{
    m_AsrNpcs.Clear();
    g_AsrWordList.clear();
    s_AsrWordId = 0;
    if (stage::GetCurrent() == stage::NAME_RESET_MONITORING_CENTER) {
        u16 first = 0;
        u16 last = 0;
        AddAsrWords(&first, &last, static_cast<const char*>(script::StrMgr::s_pInstance->GetText("STR_Reset_ASR", 0)));
        s_AsrResetWordNum[0] = last - first + 1;
        AddAsrWords(&first, &last, static_cast<const char*>(script::StrMgr::s_pInstance->GetText("STR_Reset_ASR", 1)));
        s_AsrResetWordNum[1] = last - first + 1;
        FinishAsrWordList();
        return true;
    }
    // the villagers
    void* npcs = GetGardenPlus()->data.npcData.npcs;
    for (s32 i = 0; i < 10; i++) {
        SvNpc* npc = static_cast<SvNpc*>(event::Func300928(npcs, i));
        if (npc == NULL) {
            continue;
        }
        const u16 id = npc->npc.villagerId;
        stage::NpcInfo info;
        info.kind = stage::NpcInfo::KIND_ID;
        info.flag = 0;
        info.value = id | (static_cast<u8>(i) << 16);
        SearchFunction func(&info);
        ForeachImpl(func);
        AcNpc* actor = func.m_pResult;
        u8* table = event::g_UnknownA84AEC.table;
        if (table == NULL || id >= 399) {
            continue;
        }
        u8* entry = table + id * 34;
        if (entry == NULL) {
            continue;
        }
        // the label of the villager (5 characters at 10)
        entry[15] = 0;
        u16 first;
        u16 last;
        const char* text = static_cast<const char*>(
            script::StrMgr::s_pInstance->GetTextByLabel("STR_NNpc_name_ASR", reinterpret_cast<const char*>(entry + 10)));
        if (AddAsrWords(&first, &last, text)) {
            AsrNpc asr = {actor, first, last, 0};
            m_AsrNpcs.PushBackFast(asr);
        }
    }
    // the special characters
    for (s32 i = 0; i < ASR_SP_NPC_NUM; i++) {
        const AsrSpNpc& sp = s_AsrSpNpcs[i];
        stage::NpcInfo info(stage::NpcInfo::KIND_MODEL, 0, sp.model);
        SearchFunction func(&info);
        ForeachImpl(func);
        AcNpc* actor = func.m_pResult;
        const u8 kind = sp.kind;
        if (kind == 3) {
            if (!sp.isAlways) {
                continue;
            }
        } else if (!sp.isAlways && (actor == NULL || actor->m_UnknownA8 == 27)) {
            continue;
        }
        u16 first;
        u16 last;
        const char* text =
            static_cast<const char*>(script::StrMgr::s_pInstance->GetText("STR_SPNpc_name_ASR", sp.textIndex));
        if (AddAsrWords(&first, &last, text)) {
            AsrNpc asr = {actor, first, last, kind};
            m_AsrNpcs.PushBackFast(asr);
        }
    }
    FinishAsrWordList();
    return true;
}

// 0x006A1CB0
bool BsNpcMgr::AddAsrWords(u16* first, u16* last, const char* text)
{
    if (text == NULL) {
        *first = 0xFFFF;
        *last = 0xFFFF;
        return false;
    }
    const u16 firstId = s_AsrWordId + 1;
    u16 lastId = s_AsrWordId + 1;
    s32 index;
    do {
        const sead::SafeStringBase<char> line(text);
        if (line.isEmpty()) {
            break;
        }
        g_AsrWordList.appendWithFormat("%d:", s_AsrWordId + 1);
        const sead::SafeStringBase<char> newline("\n");
        index = line.findIndex(newline);
        if (index >= 0) {
            g_AsrWordList.append(line, index + 1);
            lastId++;
            text += index + 1;
        } else {
            g_AsrWordList.append(line, -1);
            g_AsrWordList.append(newline, -1);
        }
        s_AsrWordId++;
    } while (index > 0);
    *first = firstId;
    *last = lastId;
    return true;
}

// 0x0069FE20
bool BsNpcMgr::SIsEventSecretaryPresent()
{
    if (s_pInstance == NULL) {
        return false;
    }
    return s_pInstance->IsEventSecretaryPresent();
}

// 0x0069FE30
bool BsNpcMgr::IsEventSecretaryPresent()
{
    EventSecretarySearchFunc func;
    ForeachImpl(func);
    return func.m_IsFound;
}

// 0x0069FEA8
void BsNpcMgr::CreateStageVillagers()
{
    void* npcs = GetGardenPlus()->data.npcData.npcs;
    const stage::Name current = stage::GetCurrent();
    // the state of the villagers who are in the stage
    u8 state;
    switch (current) {
    case stage::NAME_DOWNTOWN:
        state = 7;
        break;
    case stage::NAME_RECYCLE_SHOP:
        state = 5;
        break;
    case stage::NAME_CAFE:
        state = 6;
        break;
    case stage::NAME_CONVENIENCE_STORE:
    case stage::NAME_SUPERMARKET:
    case stage::NAME_HOME_CENTER:
    case stage::NAME_DEPARTMENT_STORE_1F:
        state = 8;
        break;
    case stage::NAME_TAILOR:
        state = 21;
        break;
    case stage::NAME_CLUB444_DJKK:
    case stage::NAME_CLUB444_LIVE:
        state = 9;
        break;
    case stage::NAME_MUSEUM_FISH_0:
        state = 10;
        break;
    case 75:
        state = 11;
        break;
    case 76:
        state = 12;
        break;
    case 77:
        state = 13;
        break;
    case 78:
        state = 14;
        break;
    case 79:
        state = 15;
        break;
    case 80:
        state = 16;
        break;
    case 81:
        state = 17;
        break;
    case 82:
        state = 18;
        break;
    case 83:
        state = 19;
        break;
    case 84:
        state = 20;
        break;
    default:
        return;
    }
    for (s32 i = 0; i < 10; i++) {
        SvNpc* npc = static_cast<SvNpc*>(event::Func300928(npcs, i));
        if (npc == NULL) {
            continue;
        }
        // (in the T&T Emporium also the villagers in the state 22)
        if (npc->unk_0x250F != state && !(current == stage::NAME_DEPARTMENT_STORE_1F && npc->unk_0x250F == 22)) {
            continue;
        }
        nn::math::VEC3 position;
        math::Angle16Vec3 rotation;
        rotation.x.value = 0;
        rotation.y.value = 0;
        rotation.z.value = 0;
        bool isPlaced = false;
        if (netgame::IsMultiplePlayer()) {
            // the position the host sent
            const NpcPositionEntry& entry = GetNpcPositions()->GetEntry(i);
            if (stage::GetCurrent() == entry.stage) {
                position.x = static_cast<f32>(entry.GetX());
                position.y = 96.0f;
                position.z = static_cast<f32>(entry.GetZ());
                position.y = FdInfo::GetUnitHeight(position, FIELD_NAME_CURRENT);
                rotation.y.value = entry.packed[3] << 8;
                isPlaced = true;
            }
        }
        if (!isPlaced) {
            Func150158(&position, npc->unk_0x250F == 22);
        }
        stage::NpcInfo info;
        info.kind = stage::NpcInfo::KIND_ID;
        info.flag = 0;
        info.value = npc->npc.villagerId | (static_cast<u8>(i) << 16);
        oml::framework::Process* actor =
            Actor::Create(PROC_AC_NPC_DT, this, reinterpret_cast<u32>(&info), &position, &rotation);
        if (actor != NULL) {
            m_Npcs.PushBackFast(static_cast<AcNpc*>(actor));
        }
    }
}

// the models of the special character who stands beside the secretary (CreateAnniversaryNpcs)
// 0x009512E0
const char* BsNpcMgr::s_AnniversaryModels[6] = {"rci", "owl", "hgh", "mnk", "alw", "lrd"};

namespace {
// (inline) the villagers who may take part: not Func11B244/Func11A1E0 and not the one of Func30977C
inline bool IsAnniversaryVillager(void* npcs, SvNpc* npc, s32 index)
{
    if (Func11B244(npc)) {
        return false;
    }
    if (Func11A1E0(npc)) {
        return false;
    }
    return Func30977C(npcs, 0) != index;
}
} // namespace

// 0x006A0194
bool BsNpcMgr::CreateAnniversaryNpcs()
{
    if (!m_SpBanks.Create(HeapInit::s_pStageHeap, 2)) {
        return false;
    }
    if (!m_NmlBanks.Create(HeapInit::s_pStageHeap, 2)) {
        return false;
    }
    // the secretary (AcNpcSpSecretaryAnniversary)
    {
        s32 x = 0;
        s32 z = 0;
        Func6FB2BC(&x, &z);
        nn::math::VEC3 position;
        FdInfo::ToCenterWorld(&position, x, z);
        position.x += 24.0f;
        position.z += FdInfo::UNIT_WORLD_SIZE_ZF * 2.0f;
        math::Angle16Vec3 rotation;
        rotation.x.value = 0;
        rotation.y.value = 0xE71C;
        rotation.z.value = 0;
        stage::NpcInfo info(stage::NpcInfo::KIND_MODEL, 0, "sza");
        oml::framework::Process* actor =
            Actor::Create(PROC_AC_NPC_SP_SECRETARY_ANNIVERSARY, this, reinterpret_cast<u32>(&info), &position, &rotation);
        if (actor != NULL) {
            m_Npcs.PushBackFast(static_cast<AcNpc*>(actor));
        }
    }
    // a special character (AcNpcDemoDollSp), on the side of Func30BA74
    {
        s32 x = 0;
        s32 z = 0;
        Func6FB2BC(&x, &z);
        nn::math::VEC3 position;
        FdInfo::ToCenterWorld(&position, x, z);
        const u8 value = Func30BA74();
        if (value == 15 || value == 30 || value == 28) {
            position.x += FdInfo::UNIT_WORLD_SIZE_XF * 2.5f;
        } else {
            position.x -= FdInfo::UNIT_WORLD_SIZE_XF * 2.5f;
        }
        position.z += FdInfo::UNIT_WORLD_SIZE_ZF * 2.0f;
        math::Angle16Vec3 rotation;
        rotation.x.value = 0;
        rotation.y.value = 0x2000;
        rotation.z.value = 0;
        const nn::math::VEC3* target = Func5C37B0(4, true);
        if (target != NULL) {
            rotation.y = Actor::CalcAngleY(&position, target);
        }
        sead::Random random;
        random.init();
        const char* model = s_AnniversaryModels[random.getU32(6)];
        stage::NpcInfo info(stage::NpcInfo::KIND_MODEL, 0, model);
        oml::framework::Process* actor =
            Actor::Create(PROC_AC_NPC_DEMO_DOLL_SP, this, reinterpret_cast<u32>(&info), &position, &rotation);
        if (actor != NULL) {
            m_Npcs.PushBackFast(static_cast<AcNpc*>(actor));
            static_cast<AcNpc*>(actor)->m_UnknownDA5 |= 4;
        }
    }
    // a villager: of the ones that may take part, those with a date (0x250A) not before the date of the first
    void* npcs = GetGardenPlus()->data.npcData.npcs;
    s32 chosen = -1;
    {
        SearchCand<10> cand;
        cand.Clear();
        const SvDate* first = NULL;
        for (s32 i = 0; i < 10; i++) {
            SvNpc* npc = static_cast<SvNpc*>(event::Func300928(npcs, i));
            if (npc == NULL || !IsAnniversaryVillager(npcs, npc, i)) {
                continue;
            }
            const SvDate* date = &npc->date2;
            if (first == NULL) {
                cand.Add(i);
                first = date;
            } else if (event::CompareSvDate(date, first->year, first->month, first->day) != -1) {
                cand.Add(i);
            }
        }
        if (cand.m_RestNum != 0) {
            s32 x = 0;
            s32 z = 0;
            Func6FB2BC(&x, &z);
            x += 2;
            z += 1;
            nn::math::VEC3 position;
            FdInfo::ToCenterWorld(&position, x, z);
            math::Angle16Vec3 rotation;
            rotation.x.value = 0;
            rotation.y.value = 0xE16C;
            rotation.z.value = 0;
            chosen = cand.Pick();
            SvNpc* npc = static_cast<SvNpc*>(event::Func300928(npcs, chosen));
            stage::NpcInfo info;
            info.kind = stage::NpcInfo::KIND_ID;
            info.flag = 0;
            info.value = npc->npc.villagerId | (static_cast<u8>(chosen) << 16);
            oml::framework::Process* actor =
                Actor::Create(PROC_AC_NPC_DEMO_DOLL, this, reinterpret_cast<u32>(&info), &position, &rotation);
            if (actor != NULL) {
                m_Npcs.PushBackFast(static_cast<AcNpc*>(actor));
                static_cast<AcNpc*>(actor)->m_UnknownB1E = true;
            }
        }
    }
    // another villager of the ones that may take part
    {
        SearchCand<10> cand;
        cand.Clear();
        for (s32 i = 0; i < 10; i++) {
            SvNpc* npc = static_cast<SvNpc*>(event::Func300928(npcs, i));
            if (npc == NULL || !IsAnniversaryVillager(npcs, npc, i)) {
                continue;
            }
            cand.Add(i);
        }
        cand.Remove(chosen);
        if (cand.m_RestNum != 0) {
            s32 x = 0;
            s32 z = 0;
            Func6FB2BC(&x, &z);
            x -= 2;
            z += 1;
            nn::math::VEC3 position;
            FdInfo::ToCenterWorld(&position, x, z);
            math::Angle16Vec3 rotation;
            rotation.x.value = 0;
            rotation.y.value = 0xEAAB;
            rotation.z.value = 0;
            const nn::math::VEC3* target = Func5C37B0(4, true);
            if (target != NULL) {
                rotation.y = Actor::CalcAngleY(&position, target);
            }
            const s32 index = cand.Pick();
            SvNpc* npc = static_cast<SvNpc*>(event::Func300928(npcs, index));
            stage::NpcInfo info;
            info.kind = stage::NpcInfo::KIND_ID;
            info.flag = 0;
            info.value = npc->npc.villagerId | (static_cast<u8>(index) << 16);
            oml::framework::Process* actor =
                Actor::Create(PROC_AC_NPC_DEMO_DOLL, this, reinterpret_cast<u32>(&info), &position, &rotation);
            if (actor != NULL) {
                m_Npcs.PushBackFast(static_cast<AcNpc*>(actor));
            }
        }
    }
    return true;
}

// 0x006A082C
bool BsNpcMgr::IsPlayerInUnknownRects()
{
    const nn::math::VEC3* player = Func5C37B0(4, true);
    if (player == NULL) {
        return false;
    }
    const f32 x = player->x;
    const f32 z = player->z;
    for (s32 i = 0; i < 2; i++) {
        FieldRect rect(m_UnknownF0F4[i]);
        const Vector2i& min = rect.min;
        const Vector2i& max = rect.max;
        const f32 margin = 12.0f;
        const f32 left = static_cast<f32>(min.x) * FdInfo::UNIT_WORLD_SIZE_XF - margin;
        const f32 top = static_cast<f32>(min.y) * FdInfo::UNIT_WORLD_SIZE_XF - margin;
        const f32 right = margin + static_cast<f32>(max.x + 1) * FdInfo::UNIT_WORLD_SIZE_XF;
        const f32 bottom = margin + static_cast<f32>(max.y + 1) * FdInfo::UNIT_WORLD_SIZE_XF;
        if (x > left && x < right && z > top && z < bottom) {
            return true;
        }
    }
    return false;
}

namespace {
// (inline) one of the rooms first..last of the museum with a donation, as the state of a villager in it (24: none)
inline u8 PickMuseumRoomState(SvMuseum* museum, u8 first, u8 last, u8 firstState)
{
    BitSet32 rooms;
    rooms.Init(last - first + 1, false);
    for (u32 room = first; room <= last; room++) {
        if (museum->HasDonationInRoom(room)) {
            rooms.Add(room - first);
        }
    }
    if (rooms.count <= 0) {
        return 24;
    }
    return rooms.PickRandom() + firstState;
}
} // namespace

// 0x006A0964
void BsNpcMgr::Func6A0964(BitSet32* set)
{
    if (Func2FF638(100) >= 30) {
        return;
    }
    if (!Func51D0CC()) {
        return;
    }
    SvNpc* npcs = GetGardenData()->GetNpcData()->npcs;
    BitSet32 candidates;
    candidates.Init(10, false);
    // a villager in a museum room goes to another room with a donation (the states 10..20 are the rooms)
    u8 state = 24;
    const stage::Name current = stage::GetCurrent();
    SvMuseum* museum = &GetGardenPlus()->data.townData.museum;
    switch (current) {
    case stage::NAME_MUSEUM_FISH_0:
    case 75:
    case 76:
        state = PickMuseumRoomState(museum, 74, 76, 10);
        if (state == 24) {
            return;
        }
        break;
    case 77:
    case 78:
    case 79:
        state = PickMuseumRoomState(museum, 77, 79, 13);
        if (state == 24) {
            return;
        }
        break;
    case 80:
    case 81:
    case 82:
        state = PickMuseumRoomState(museum, 80, 82, 16);
        if (state == 24) {
            return;
        }
        break;
    case 83:
    case 84:
        state = PickMuseumRoomState(museum, 83, 84, 19);
        if (state == 24) {
            return;
        }
        break;
    default:
        break;
    }
    if (state == 24) {
        return;
    }
    for (s32 i = 0; i < 10; i++) {
        SvNpc* npc = Func305AC0(npcs, i);
        const u8* extra = static_cast<const u8*>(Func2FE4A0(i));
        if (npc == NULL || set->Test(i) || extra == NULL) {
            continue;
        }
        if (Func11A1E0(npc) || Func119900(npc)) {
            continue;
        }
        const u8 own = npc->unk_0x250F;
        if (own == state) {
            // (someone is in the room already)
            candidates.bits = 0;
            candidates.count = 0;
            return;
        }
        if (static_cast<u32>(own - 8) < 15 && extra[29] == 165) {
            candidates.Add(i);
        }
    }
    if (candidates.count > 0) {
        SvNpc* npc = Func305AC0(npcs, candidates.PickRandom());
        if (npc != NULL) {
            Func113A0C(npc, state);
        }
    }
}

// 0x006A0C98
void BsNpcMgr::Func6A0C98(BitSet32* set)
{
    if (!Func51D0CC()) {
        return;
    }
    SvNpc* npcs = GetGardenData()->GetNpcData()->npcs;
    BitSet32 villagers;
    villagers.Init(10, false);
    // the states a villager may go to (the ones of Func30A270 that nobody is in)
    BitSet32 states;
    states.Init(24, false);
    Func30A270(&states, 2);
    SvPlayer* player = g_UnkAAFF18.pPlayer;
    if (player == NULL || !(reinterpret_cast<const u8*>(player)[0x5701] & 2)) {
        Func51DD94(&states);
    }
    for (s32 i = 0; i < 10; i++) {
        SvNpc* npc = Func305AC0(npcs, i);
        if (npc == NULL || Func11A1E0(npc) || Func119900(npc)) {
            continue;
        }
        states.Remove(npc->unk_0x250F);
        if (set->Test(i)) {
            continue;
        }
        const u8* extra = static_cast<const u8*>(Func2FE4A0(i));
        if (extra == NULL) {
            continue;
        }
        if (extra[28] != 165 || extra[29] != 165) {
            continue;
        }
        if (Func11A084(npc) != 2) {
            continue;
        }
        if (Func309B70(npc, 0) && !Func11972C(npcs, i, 0)) {
            continue;
        }
        villagers.Add(i);
    }
    if (!Func112104()) {
        states.Remove(5);
    }
    if (!Func111FA8()) {
        states.Remove(6);
    }
    if (villagers.count <= 0 || states.count <= 0) {
        return;
    }
    if (Func2FF638(100) >= 30) {
        return;
    }
    const s32 index = villagers.PickRandom();
    SvNpc* npc = Func305AC0(npcs, index);
    BitSet32 choices = states;
    if (npc == NULL) {
        return;
    }
    if (player != NULL) {
        const u8* data = Func30A1C4(npc, &player->playerInfo);
        if (data == NULL || !(data[188] >> 7) || Func11A668(npc)) {
            Func51DD94(&choices);
        }
    }
    u8 state;
    if (choices.Test(5)) {
        BitSet32 others = choices;
        if (!Func11AB1C(npc, index)) {
            others.Remove(5);
        }
        state = others.PickRandom();
    } else {
        state = choices.PickRandom();
    }
    if (state >= 24) {
        return;
    }
    Func113A14(npc, state);
    if (Func51D8A4() == index) {
        Func2FE520(-1);
    }
    states.Remove(state);
}

// 0x006A0F9C
void BsNpcMgr::Func6A0F9C(BitSet32* set)
{
    SvNpc* npcs = GetGardenData()->GetNpcData()->npcs;
    if (Func119474(npcs, -1)) {
        for (u32 i = 0; i < 2; i++) {
            const s32 index = Func1194F8(npcs, i);
            SvNpc* npc = Func305AC0(npcs, index);
            if (npc != NULL) {
                Func113A14(npc, 2);
                set->Add(index);
            }
        }
    }
    for (s32 i = 0; i < 10; i++) {
        SvNpc* npc = Func305AC0(npcs, i);
        if (npc == NULL) {
            continue;
        }
        void* extra = Func2FE4A0(i);
        if (extra == NULL) {
            continue;
        }
        const s32 other = Func309A70(extra);
        if (other < 0) {
            continue;
        }
        SvNpc* otherNpc = Func305AC0(npcs, other);
        if (!netgame::IsMultiPlayer() && otherNpc != NULL && Func11A3A8(otherNpc)) {
            Func113A14(npc, 3);
            set->Add(i);
        } else {
            Func113A14(npc, 2);
        }
        if (Func51D8A4() == i) {
            Func2FE520(-1);
        }
    }
}

// 0x006A10D8
void BsNpcMgr::Func6A10D8(BitSet32* set)
{
    if (!Func51D0CC()) {
        return;
    }
    SvNpc* npcs = GetGardenData()->GetNpcData()->npcs;
    BitSet32 villagers;
    villagers.Init(10, false);
    // the state of the villagers in the stage (Main Street, Retail, the cafe)
    u8 state = 24;
    switch (stage::GetCurrent()) {
    case stage::NAME_DOWNTOWN: {
        state = 7;
        SvPlayer* player = g_UnkAAFF18.pPlayer;
        if (player == NULL || !(reinterpret_cast<const u8*>(player)[0x5701] & 2)) {
            return;
        }
        break;
    }
    case stage::NAME_RECYCLE_SHOP:
        state = 5;
        if (!Func112104()) {
            return;
        }
        break;
    case stage::NAME_CAFE:
        state = 6;
        if (!Func111FA8()) {
            return;
        }
        break;
    default:
        break;
    }
    SvPersonalId* playerInfo = &SvGetCurrentPlayer()->playerInfo;
    for (s32 i = 0; i < 10; i++) {
        SvNpc* npc = Func305AC0(npcs, i);
        if (npc == NULL) {
            continue;
        }
        if (npc->unk_0x250F == state) {
            return;
        }
        if (set->Test(i) || Func11A1E0(npc) || Func119900(npc)) {
            continue;
        }
        if (Func309B70(npc, 0) && !Func11972C(npcs, i, 0)) {
            continue;
        }
        const u8* extra = static_cast<const u8*>(Func2FE4A0(i));
        if (extra == NULL || extra[28] != 165 || extra[29] != 165) {
            continue;
        }
        Func309D90(npc, playerInfo);
        if (state != 6 && state == 5 && !Func11AB1C(npc, i)) {
            continue;
        }
        villagers.Add(i);
    }
    if (villagers.count <= 0) {
        return;
    }
    if (Func2FF638(100) >= 10) {
        return;
    }
    const s32 index = villagers.PickRandom();
    SvNpc* npc = Func305AC0(npcs, index);
    if (npc == NULL) {
        return;
    }
    Func113A14(npc, state);
    if (Func51D8A4() == index) {
        Func2FE520(-1);
    }
}

// 0x006A1320
void BsNpcMgr::CreateSecretaryAtStation()
{
    if (stage::IsKind(stage::KIND_SCREEN, stage::GetCurrent())) {
        return;
    }
    if (SvGetCurrentPlayer() == NULL) {
        return;
    }
    nn::math::VEC3 position;
    GetStructurePosition(&position, GetStationStructure());
    position.z += FdInfo::UNIT_WORLD_SIZE_XF * 2.5f;
    position.x += FdInfo::UNIT_WORLD_SIZE_XF * 2.0f;
    math::Angle16Vec3 rotation;
    rotation.x.value = 0;
    rotation.y.value = 0x8000;
    rotation.z.value = 0;
    stage::NpcInfo info(stage::NpcInfo::KIND_MODEL, 0, "sza");
    oml::framework::Process* actor =
        Actor::Create(PROC_AC_NPC_SP_SECRETARY_OUT, this, reinterpret_cast<u32>(&info), &position, &rotation);
    if (actor != NULL) {
        m_Npcs.PushBackFast(static_cast<AcNpc*>(actor));
    }
}

// the models of Mr. Resetti and Don Resetti in the Reset Centre (CreateResetCenterNpcs)
// 0x009512B8
const char* BsNpcMgr::s_ResetCenterModels[7] = {"mof", "mof", "mof", "mof", "mof", "moo", "mof"};
// 0x009512D4
const char* BsNpcMgr::s_ResetCenterModels2[3] = {"mod", "moc", "moc"};

// 0x006A1470
bool BsNpcMgr::CreateResetCenterNpcs()
{
    if (!m_SpBanks.Create(HeapInit::s_pStageHeap, 2)) {
        return false;
    }
    // the places of Mr. Resetti (by the value of the player) and of Don Resetti
    // 0x009940B4 (guard 0x00951190)
    static const nn::math::VEC3 s_Positions[7] = {
        nn::math::VEC3(FdInfo::UNIT_WORLD_SIZE_XF * 9.5f, 0.0f, FdInfo::UNIT_WORLD_SIZE_XF * 2.5f),
        nn::math::VEC3(FdInfo::UNIT_WORLD_SIZE_XF * 10.5f, 0.0f, FdInfo::UNIT_WORLD_SIZE_XF * 3.5f),
        nn::math::VEC3(FdInfo::UNIT_WORLD_SIZE_XF * 6.5f, 0.0f, FdInfo::UNIT_WORLD_SIZE_XF * 2.5f),
        nn::math::VEC3(FdInfo::UNIT_WORLD_SIZE_XF * 7.5f, 0.0f, FdInfo::UNIT_WORLD_SIZE_XF * 2.5f),
        nn::math::VEC3(FdInfo::UNIT_WORLD_SIZE_XF * 6.5f, 0.0f, FdInfo::UNIT_WORLD_SIZE_XF * 3.5f),
        nn::math::VEC3(272.0f, 0.0f, 44.0f),
        nn::math::VEC3(FdInfo::UNIT_WORLD_SIZE_XF * 1.5f, 0.0f, FdInfo::UNIT_WORLD_SIZE_XF * 3.5f),
    };
    // 0x00994108 (guard 0x0095118C)
    static const nn::math::VEC3 s_Positions2[3] = {
        nn::math::VEC3(FdInfo::UNIT_WORLD_SIZE_XF * 9.5f, 0.0f, FdInfo::UNIT_WORLD_SIZE_XF * 3.5f),
        nn::math::VEC3(FdInfo::UNIT_WORLD_SIZE_XF * 7.5f, 0.0f, FdInfo::UNIT_WORLD_SIZE_XF * 1.5f),
        nn::math::VEC3(FdInfo::UNIT_WORLD_SIZE_XF * 10.5f, 0.0f, FdInfo::UNIT_WORLD_SIZE_XF * 2.5f),
    };
    // 0x0099412C (guard 0x00951188)
    static const u16 s_Angles[7] = {0x58E4, 0x4000, 0xA000, 0x2E39, 0xC000, 0x0000, 0x8000};
    // 0x009511A8 (guard 0x00951184)
    static const u16 s_Angles2[3] = {0xAAAB, 0x8000, 0xA000};

    s_State.unk12 = false;
    bool isCreated = false;
    SvPlayer* player = SvGetCurrentPlayer();
    if (player == NULL) {
        return false;
    }
    u32 index = 0;
    u32 index2 = 0;
    bool withDon = false;
    const u8 value = player->playerFlags.unk_0x6_1;
    if (value == 0 || value == 1 || value == 2) {
        index = value;
    } else if (value == 3) {
        withDon = true;
        index = 3;
    } else {
        const u32 random = netgame::Func2FF63C(100);
        if (random < 30) {
            index = 4;
        } else if (random < 60) {
            // only Don Resetti
            index2 = 1;
            goto don;
        } else if (random >= 90) {
            index = 6;
        } else {
            withDon = true;
            s_State.unk12 = true;
            index = 5;
            index2 = 2;
        }
    }
    if (index < 7) {
        nn::math::VEC3 position = s_Positions[index];
        math::Angle16Vec3 rotation;
        rotation.x.value = 0;
        rotation.y.value = s_Angles[index];
        rotation.z.value = 0;
        stage::NpcInfo info(stage::NpcInfo::KIND_MODEL, 0, s_ResetCenterModels[index]);
        oml::framework::Process* actor =
            Actor::Create(PROC_AC_NPC_SP_RESETSAN_IN, this, reinterpret_cast<u32>(&info), &position, &rotation);
        if (actor != NULL) {
            m_Npcs.PushBackFast(static_cast<AcNpc*>(actor));
        }
        isCreated = true;
    }
    if (!withDon || index2 >= 3) {
        return isCreated;
    }
don:
    {
        nn::math::VEC3 position = s_Positions2[index2];
        math::Angle16Vec3 rotation;
        rotation.x.value = 0;
        rotation.y.value = s_Angles2[index2];
        rotation.z.value = 0;
        stage::NpcInfo info(stage::NpcInfo::KIND_MODEL, 0, s_ResetCenterModels2[index2]);
        oml::framework::Process* actor =
            Actor::Create(PROC_AC_NPC_SP_RACKETSAN_IN, this, reinterpret_cast<u32>(&info), &position, &rotation);
        if (actor != NULL) {
            m_Npcs.PushBackFast(static_cast<AcNpc*>(actor));
        }
    }
    return true;
}

// 0x006A19A4
void BsNpcMgr::Func6A19A4(BitSet32* set)
{
    if (!Func51D0CC()) {
        return;
    }
    SvPlayer* player = g_UnkAAFF18.pPlayer;
    if (player == NULL || !(reinterpret_cast<const u8*>(player)[0x5701] & 2)) {
        return;
    }
    SvNpc* npcs = GetGardenData()->GetNpcData()->npcs;
    BitSet32 villagers;
    villagers.Init(10, false);
    u8 state = 24;
    // the museum rooms with a donation
    SvMuseum* museum = &GetGardenPlus()->data.townData.museum;
    BitSet32 rooms;
    rooms.Init(11, false);
    for (u32 room = 74; room <= 84; room++) {
        if (museum->HasDonationInRoom(room)) {
            rooms.Add(room - 74);
        }
    }
    // the state of the villagers in the stage (the shops, the club, the museum)
    const stage::Name current = stage::GetCurrent();
    switch (current) {
    case stage::NAME_TAILOR:
        state = 21;
        break;
    case stage::NAME_CONVENIENCE_STORE:
    case stage::NAME_SUPERMARKET:
    case stage::NAME_HOME_CENTER:
    case stage::NAME_DEPARTMENT_STORE_1F:
        state = 8;
        break;
    case stage::NAME_CLUB444_DJKK:
    case stage::NAME_CLUB444_LIVE:
        state = 9;
        break;
    case 73:
        state = 10;
        break;
    default:
        break;
    }
    SvPersonalId* playerInfo = &player->playerInfo;
    for (s32 i = 0; i < 10; i++) {
        SvNpc* npc = Func305AC0(npcs, i);
        if (npc == NULL) {
            continue;
        }
        if (npc->unk_0x250F == state || state == 10) {
            return;
        }
        if (set->Test(i) || Func11A1E0(npc) || Func119900(npc)) {
            continue;
        }
        if (Func309B70(npc, 0) && !Func11972C(npcs, i, 0)) {
            continue;
        }
        const u8* data = Func309D90(npc, playerInfo);
        if (data == NULL || !(data[188] >> 7) || Func11A668(npc)) {
            continue;
        }
        const u8* extra = static_cast<const u8*>(Func2FE4A0(i));
        if (extra == NULL || extra[28] != 165 || extra[29] != 165) {
            continue;
        }
        villagers.Add(i);
    }
    if (villagers.count <= 0) {
        return;
    }
    if (Func2FF638(100) >= 20) {
        return;
    }
    SvNpc* npc = Func305AC0(npcs, villagers.PickRandom());
    if (npc == NULL) {
        return;
    }
    u8 newState;
    if (current == stage::NAME_CLUB444_DJKK || current == stage::NAME_CLUB444_LIVE) {
        u8* club = &GetGardenPlus()->data.townData.unk_0xEE2C.clubLolUnlockState;
        if (club == NULL) {
            return;
        }
        if (!Func717934(club) && !Func7176C4(club)) {
            return;
        }
        newState = state;
    } else if (current == 73) {
        if (rooms.count <= 0) {
            return;
        }
        newState = rooms.PickRandom() + 10;
    } else {
        newState = state;
    }
    if (newState != 24) {
        Func113A0C(npc, newState);
    }
}

// 0x006A1F94
void BsNpcMgr::Func6A1F94(BitSet32* set)
{
    if (!Func51D0CC()) {
        return;
    }
    SvNpc* npcs = GetGardenData()->GetNpcData()->npcs;
    BitSet32 villagers;
    villagers.Init(10, false);
    BitSet32 states;
    states.Init(24, false);
    Func30A270(&states, 2);
    SvPlayer* player = g_UnkAAFF18.pPlayer;
    if (player == NULL || !(reinterpret_cast<const u8*>(player)[0x5701] & 2)) {
        Func51DD94(&states);
    }
    // not the shop the player comes from
    const stage::Name previous = stage::GetPrevious();
    if (previous == stage::NAME_RECYCLE_SHOP) {
        states.Remove(5);
    } else if (previous == stage::NAME_CAFE) {
        states.Remove(6);
    }
    if (!Func112104()) {
        states.Remove(5);
    }
    if (!Func111FA8()) {
        states.Remove(6);
    }
    // a fifth of the villagers who are out move on
    for (s32 i = 0; i < 10; i++) {
        const u32* extra = static_cast<const u32*>(Func2FE4A0(i));
        SvNpc* npc = Func305AC0(npcs, i);
        if (extra == NULL || npc == NULL) {
            continue;
        }
        const u8 state = npc->unk_0x250F;
        switch (state) {
        case 0:
        case 1:
        case 3:
        case 7:
        case 23:
            continue;
        default:
            break;
        }
        if (!set->Test(i) && !Func11A1E0(npc) && !Func119900(npc) && ((extra[8] >> 13) & 3) == 0 &&
            Func2FF638(100) < 20) {
            villagers.Add(i);
        }
        states.Remove(state);
    }
    while (villagers.count > 0 && states.count > 0) {
        const s32 index = villagers.PickRandom();
        villagers.Remove(index);
        SvNpc* npc = Func305AC0(npcs, index);
        if (npc == NULL) {
            continue;
        }
        BitSet32 choices = states;
        if (player != NULL) {
            const u8* data = Func30A1C4(npc, &player->playerInfo);
            if (data == NULL || !(data[188] >> 7) || Func11A668(npc)) {
                Func51DD94(&choices);
            }
        }
        u8 state;
        if (choices.Test(5)) {
            BitSet32 others = choices;
            if (!Func11AB1C(npc, index)) {
                others.Remove(5);
            }
            state = others.PickRandom();
        } else {
            state = choices.PickRandom();
        }
        if (state >= 24) {
            continue;
        }
        Func113A14(npc, state);
        states.Remove(state);
        if (Func51D8A4() == index) {
            Func2FE520(-1);
        }
    }
}

// 0x006A22C4
bool BsNpcMgr::PassToNpcOfKind57A8FC(u8 kind)
{
    for (AcNpc** it = m_Npcs.first; it != m_Npcs.last; it++) {
        AcNpc* npc = *it;
        if (npc != NULL && npc->m_Kind == kind) {
            return npc->Func57A8FC();
        }
    }
    return false;
}

// 0x006A2314
void BsNpcMgr::Func6A2314(BitSet32* set)
{
    SvNpc* npcs = GetGardenData()->GetNpcData()->npcs;
    BitSet32 villagers;
    villagers.Init(10, false);
    const stage::Name previous = stage::GetPrevious();
    // the villagers who are not where the player comes from
    for (s32 i = 0; i < 10; i++) {
        SvNpc* npc = Func305AC0(npcs, i);
        if (npc == NULL || set->Test(i) || npc->unk_0x250E != 0) {
            continue;
        }
        if (Func11A1E0(npc) || Func119900(npc)) {
            continue;
        }
        if (Func309B70(npc, 0) && !Func11972C(npcs, i, 0)) {
            continue;
        }
        bool isCandidate = false;
        switch (npc->unk_0x250F) {
        case 2:
            // (at home: not the villager whose house the player comes from)
            isCandidate = previous - 27 != i;
            break;
        case 5:
            isCandidate = previous != stage::NAME_RECYCLE_SHOP;
            break;
        case 6:
            isCandidate = previous != stage::NAME_CAFE && Func111FA8();
            break;
        case 8:
            isCandidate = previous != 56 && previous != stage::NAME_CONVENIENCE_STORE &&
                          previous != stage::NAME_SUPERMARKET && previous != stage::NAME_HOME_CENTER &&
                          previous != stage::NAME_DEPARTMENT_STORE_1F;
            break;
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
            isCandidate = true;
            break;
        case 21:
            isCandidate = previous != stage::NAME_TAILOR;
            break;
        case 22:
            isCandidate = previous != stage::NAME_DEPARTMENT_STORE_1F;
            break;
        default:
            break;
        }
        if (isCandidate) {
            villagers.Add(i);
        }
    }
    if (villagers.count <= 0) {
        return;
    }
    const s32 index = villagers.PickRandom();
    SvNpc* npc = Func305AC0(npcs, index);
    if (npc == NULL) {
        return;
    }
    Func113A14(npc, 0);
    Func698A38(index, true, false);
    m_UnknownF11C = index;
}

namespace {
// a special character who can sit in the cafe: the key of Func720B4C and the model (CreateCafeGuest)
struct CafeGuest
{
    u8 key;
    const char* model;
};
// 0x00854E58
const CafeGuest s_CafeGuests[14] = {
    {0, "pga"}, {1, "pgb"}, {2, "rci"}, {3, "mof"},  {4, "mod"},  {5, "kpp"},  {6, "ttl"},
    {7, "enj"}, {8, "upa"}, {9, "grf"}, {10, "skk"}, {11, "plb"}, {12, "bln"}, {13, "szo"},
};

// (inline) the guest sits at the unit (3, 12) of the cafe
inline oml::framework::Process* CreateCafeGuestActor(BsNpcMgr* mgr, const CafeGuest& guest)
{
    nn::math::VEC3 position;
    FdInfo::ToCenterWorld(&position, 3, 12);
    math::Angle16Vec3 rotation;
    rotation.x.value = 0;
    rotation.y.value = 0x8000;
    rotation.z.value = 0;
    stage::NpcInfo info(stage::NpcInfo::KIND_MODEL, 0, guest.model);
    return Actor::Create(PROC_AC_NPC_SP_CAFE_TALK, mgr, reinterpret_cast<u32>(&info), &position, &rotation);
}
} // namespace

// 0x00697820
bool BsNpcMgr::CreateCafeGuest()
{
    // (back from the part-time job: the guest of before, if there was one)
    if (stage::GetPrevious() == stage::NAME_CAFE_ARBEIT && static_cast<u32>(s_CafeGuest) >= 15) {
        return true;
    }
    if (!m_SpBanks.Create(HeapInit::s_pStageHeap, 2)) {
        return false;
    }
    if (netgame::IsMultiPlayer()) {
        return true;
    }
    SvPlayer* player = g_UnkAAFF18.pPlayer;
    if (player != NULL && !(reinterpret_cast<const u8*>(player)[0x5701] & 2)) {
        return true;
    }
    u8* data = reinterpret_cast<u8*>(GetGardenPlus()) + 0x70EE0;
    if (stage::GetPrevious() == stage::NAME_CAFE_ARBEIT && s_CafeGuest >= 0 && s_CafeGuest < 14) {
        const CafeGuest& guest = s_CafeGuests[s_CafeGuest];
        if (!Func720B4C(data, guest.key, time::GetCurrentDate())) {
            return true;
        }
        oml::framework::Process* actor = CreateCafeGuestActor(this, guest);
        if (actor != NULL) {
            m_Npcs.PushBackFast(static_cast<AcNpc*>(actor));
        }
        return true;
    }
    // the first guest of the day who can come
    for (u32 i = 0; i < 14; i++) {
        const CafeGuest& guest = s_CafeGuests[i];
        if (!Func720B4C(data, guest.key, time::GetCurrentDate())) {
            continue;
        }
        oml::framework::Process* actor = CreateCafeGuestActor(this, guest);
        if (actor == NULL) {
            continue;
        }
        m_Npcs.PushBackFast(static_cast<AcNpc*>(actor));
        s_CafeGuest = i;
        return true;
    }
    s_CafeGuest = -1;
    return true;
}

// 0x00697CD0
bool BsNpcMgr::CreateVillageNpcs()
{
    bool isNormal = true;
    if (stage::GetCurrent() == stage::NAME_TITLE) {
        return true;
    }
    u32 num;
    if (!netgame::IsMultiPlayer() && netgame::GetNetgameType() != netgame::TYPE_DREAM && !s_State.unk0E) {
        num = 4;
    } else {
        num = 3;
    }
    if (!m_SpBanks.Create(HeapInit::s_pStageHeap, num)) {
        return false;
    }
    // (in the Dream Suite the module places the NPCs)
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        return Func150328(this);
    }
    SvPlayer* player = SvGetCurrentPlayer();
    SvPlayer* host = netgame::IsMultiplePlayer() ? netgame::GetSvPlayer(netgame::PLAYER_NO_0) : SvGetCurrentPlayer();
    const f32 x = FdInfo::UNIT_WORLD_SIZE_XF * 30.0f;
    const f32 z = FdInfo::UNIT_WORLD_SIZE_XF * 70.0f;
    if (host != NULL && (reinterpret_cast<const u8*>(&host->playerFlags)[0x12] >> 7)) {
        // Kapp'n at the dock (structure 92)
        nn::math::VEC3 position(x, 0.0f, z);
        if (netgame::GetStructurePosition(&position, 92)) {
            position.x += FdInfo::UNIT_WORLD_SIZE_XF * 2.5f;
            position.z += FdInfo::UNIT_WORLD_SIZE_XF * 6.5f;
        }
        math::Angle16Vec3 rotation;
        rotation.x.value = 0;
        rotation.y.value = 0xA000;
        rotation.z.value = 0;
        stage::NpcInfo info(stage::NpcInfo::KIND_MODEL, 0, "kpp");
        oml::framework::Process* actor =
            Actor::Create(PROC_AC_NPC_SP_KAPPEI, this, reinterpret_cast<u32>(&info), &position, &rotation);
        if (actor != NULL) {
            m_Npcs.PushBackFast(static_cast<AcNpc*>(actor));
        }
        if (stage::IsKind(stage::KIND_ISLAND_DEMO, stage::GetCurrent())) {
            stage::NpcInfo tortimer(stage::NpcInfo::KIND_MODEL, 0, "ttl");
            Func2D4BC0(reinterpret_cast<u8*>(GetGardenData()) + 0x70E60, &tortimer);
        }
    } else if (!netgame::IsMultiPlayer()) {
        SvPlayer* current = SvGetCurrentPlayer();
        if (current != NULL && !(reinterpret_cast<const u8*>(&current->playerFlags)[0x12] & 0x40) &&
            (reinterpret_cast<const u8*>(&current->playerFlags)[0x13] & 0x20)) {
            // Tortimer announces the island (AcNpcSpKotobukiAnnounce)
            nn::math::VEC3 position(x, 0.0f, z);
            if (netgame::GetStructurePosition(&position, 92)) {
                position.x += FdInfo::UNIT_WORLD_SIZE_XF * 2.0f;
                position.z += FdInfo::UNIT_WORLD_SIZE_XF * 6.0f;
            }
            math::Angle16Vec3 rotation;
            rotation.x.value = 0;
            rotation.y.value = 0xA000;
            rotation.z.value = 0;
            stage::NpcInfo info(stage::NpcInfo::KIND_MODEL, 0, "ttl");
            g_KotobukiAnnounceState = 2;
            oml::framework::Process* actor =
                Actor::Create(PROC_AC_NPC_SP_KOTOBUKI_ANNOUNCE, this, reinterpret_cast<u32>(&info), &position, &rotation);
            if (actor != NULL) {
                m_Npcs.PushBackFast(static_cast<AcNpc*>(actor));
            }
            s_State.unk0A = 0;
        }
    }

    // the secretary
    bool isGpEvent = false;
    bool isEvent = false;
    bool isGpEventCreated = false;
    u8* online = static_cast<u8*>(netgame::g_pUnknown95488C);
    if (Func333628()) {
        isGpEvent = true;
    } else if (Func150330()) {
        isEvent = true;
    } else if (online[156] == 1 && netgame::IsMultiplePlayer()) {
        const u8 value = Func3189EC();
        if (value != 8 && value != 7) {
            isGpEvent = true;
        } else {
            isEvent = true;
        }
    }
    if (player != NULL && (reinterpret_cast<const u8*>(&player->playerFlags)[0x2B] >> 7)) {
        CreateSecretaryAtStation();
        isNormal = false;
    } else if (s_State.unk0F) {
        // the ceremony of a structure: the secretary and a guest of the module
        void* object = Func30CEDC();
        if (Func70BEDC(object)) {
            const s32 unitX = Func718E6C(object);
            const s32 unitZ = Func718E90(object);
            Func150338(Func302704(object), unitX, unitZ);
            math::Angle16Vec3 rotation;
            rotation.x.value = 0;
            rotation.y.value = 0xE000;
            rotation.z.value = 0;
            nn::math::VEC3 center;
            FdInfo::ToCenterWorld(&center, unitX, unitZ);
            nn::math::VEC3 position = center;
            Func150340(&position, Func302704(object));
            stage::NpcInfo info(stage::NpcInfo::KIND_MODEL, 0, "sza");
            oml::framework::Process* actor =
                Actor::Create(PROC_AC_NPC_SP_SECRETARY_CEREMONY, this, reinterpret_cast<u32>(&info), &position, &rotation);
            if (actor != NULL) {
                m_Npcs.PushBackFast(static_cast<AcNpc*>(actor));
            }
            const CeremonyGuest* guest = Func150348(Func302704(object));
            if (guest != NULL) {
                stage::NpcInfo guestInfo(stage::NpcInfo::KIND_MODEL, 0, guest->model);
                nn::math::VEC3 guestPosition;
                guestPosition.x = center.x;
                guestPosition.y = center.y;
                guestPosition.z = center.z;
                math::Angle16Vec3 guestRotation;
                guestRotation.x.value = 0;
                guestRotation.y.value = 0x2000;
                guestRotation.z.value = 0;
                guestPosition.x += guest->x;
                guestPosition.z += guest->z;
                oml::framework::Process* doll = Actor::Create(PROC_AC_NPC_DEMO_DOLL_SP, this,
                                                              reinterpret_cast<u32>(&guestInfo), &guestPosition,
                                                              &guestRotation);
                if (doll != NULL) {
                    m_Npcs.PushBackFast(static_cast<AcNpc*>(doll));
                    static_cast<AcNpc*>(doll)->m_UnknownB1E = true;
                }
            }
        }
        isNormal = false;
    } else if (player != NULL && !(reinterpret_cast<const u8*>(player)[0x5701] & 2)) {
        CreatePrologueNpcs();
    } else if (isEvent) {
        // at the place of Func6FB2BC (AcNpcSpSecretaryEvent)
        math::Angle16Vec3 rotation;
        rotation.x.value = 0;
        rotation.y.value = 0;
        rotation.z.value = 0;
        s32 unitX;
        s32 unitZ;
        Func6FB2BC(&unitX, &unitZ);
        unitX -= 1;
        unitZ += 5;
        nn::math::VEC3 position;
        FdInfo::ToCenterWorldWithUnitHeight(&position, unitX, unitZ);
        stage::NpcInfo info(stage::NpcInfo::KIND_MODEL, 0, "sza");
        oml::framework::Process* actor =
            Actor::Create(PROC_AC_NPC_SP_SECRETARY_EVENT, this, reinterpret_cast<u32>(&info), &position, &rotation);
        if (actor != NULL) {
            m_Npcs.PushBackFast(static_cast<AcNpc*>(actor));
        }
    } else if (s_State.unk0C) {
        // in front of the town hall (AcNpcSpSecretaryPWorks)
        Func5C37B0(4, true);
        nn::math::VEC3 position;
        GetStructurePosition(&position, GetTownHallStructure());
        position.z += FdInfo::UNIT_WORLD_SIZE_XF * 1.5f;
        position.x += FdInfo::UNIT_WORLD_SIZE_XF * 0.5f;
        math::Angle16Vec3 rotation;
        rotation.x.value = 0;
        rotation.y.value = 0xC000;
        rotation.z.value = 0;
        stage::NpcInfo info(stage::NpcInfo::KIND_MODEL, 0, "sza");
        oml::framework::Process* actor =
            Actor::Create(PROC_AC_NPC_SP_SECRETARY_P_WORKS, this, reinterpret_cast<u32>(&info), &position, &rotation);
        if (actor != NULL) {
            m_Npcs.PushBackFast(static_cast<AcNpc*>(actor));
        }
    } else if (isGpEvent) {
        // AcNpcSpSecretaryGpEvent: at the town hall during an event of Func308E74, else at the place of Func6FB2BC
        math::Angle16Vec3 rotation;
        rotation.x.value = 0;
        rotation.y.value = 0;
        rotation.z.value = 0;
        nn::math::VEC3 position;
        if (event::Func308E74() != event::EVENT_ID_NONE) {
            GetStructurePosition(&position, GetTownHallStructure());
            position.z += FdInfo::UNIT_WORLD_SIZE_XF * 2.0f;
            position.x += FdInfo::UNIT_WORLD_SIZE_XF * 2.0f;
        } else {
            s32 unitX;
            s32 unitZ;
            Func6FB2BC(&unitX, &unitZ);
            unitX += 1;
            unitZ += 4;
            FdInfo::ToCenterWorldWithUnitHeight(&position, unitX, unitZ);
        }
        stage::NpcInfo info(stage::NpcInfo::KIND_MODEL, 0, "sza");
        oml::framework::Process* actor =
            Actor::Create(PROC_AC_NPC_SP_SECRETARY_GP_EVENT, this, reinterpret_cast<u32>(&info), &position, &rotation);
        if (actor != NULL) {
            m_Npcs.PushBackFast(static_cast<AcNpc*>(actor));
        }
        isGpEventCreated = true;
    } else if (!netgame::IsMultiPlayer()) {
        // AcNpcSpRakosuke at a place of s_PlaceFuncs[5]
        math::Angle16Vec3 rotation;
        rotation.x.value = 0;
        rotation.y.value = 0;
        rotation.z.value = 0;
        stage::NpcInfo info(stage::NpcInfo::KIND_MODEL, 0, "seo");
        PlaceFunc func = s_PlaceFuncs[5];
        nn::math::VEC3 position;
        if (func != NULL && func(&position, &rotation, &info)) {
            oml::framework::Process* actor =
                Actor::Create(PROC_AC_NPC_SP_RAKOSUKE, this, reinterpret_cast<u32>(&info), &position, &rotation);
            if (actor != NULL) {
                m_Npcs.PushBackFast(static_cast<AcNpc*>(actor));
            }
        }
    }
    if (!(isGpEventCreated | isEvent)) {
        online[156] = 0;
    }
    if (isNormal) {
        const bool isRequested = CreateRequestedNpc();
        if (isRequested) {
            Func5C933C();
        }
        const bool isEventNpc = CreateEventNpc();
        if (!((CreateSpecialDayNpc() & isEventNpc) | isRequested)) {
            CreateVisitorNpc();
        }
    }
    return true;
}

// 0x00317158
bool BsNpcMgr::CreateShopNpcs(u32 bankNum)
{
    if (!m_NmlBanks.Create(HeapInit::s_pStageHeap, bankNum)) {
        return false;
    }
    // the kinds of up to two NPCs of Func51D990 in the stage (11: none)
    const stage::Name current = stage::GetCurrent();
    u8 kinds[4] = {11, 11, 0, 1};
    bool hasKinds = true;
    switch (current) {
    case stage::NAME_CONVENIENCE_STORE:
    case stage::NAME_SUPERMARKET:
    case stage::NAME_HOME_CENTER:
    case stage::NAME_DEPARTMENT_STORE_2F:
        kinds[0] = 4;
        break;
    case stage::NAME_DEPARTMENT_STORE_3F:
        kinds[0] = 5;
        break;
    case stage::NAME_CLUB444_DJKK:
    case stage::NAME_CLUB444_LIVE:
        kinds[0] = 6;
        kinds[1] = 7;
        break;
    case stage::NAME_TAILOR:
        kinds[0] = 8;
        break;
    case stage::NAME_MUSEUM_FISH_0:
    case 75:
    case 76:
    case 77:
    case 78:
    case 79:
    case 80:
    case 81:
    case 82:
    case 83:
    case 84:
        if (Func51DFB8(1, 0) == current) {
            kinds[0] = 9;
        }
        if (Func51DFB8(0, 0) == current) {
            kinds[1] = 10;
        }
        break;
    default:
        hasKinds = false;
        break;
    }
    if (hasKinds) {
        for (s32 i = 0; i < 2; i++) {
            const u8 kind = kinds[i];
            if (kind == 11) {
                continue;
            }
            const u16 id = Func51D990(kind, 0);
            if (id == 0xFFFF) {
                continue;
            }
            math::Angle16Vec3 rotation;
            rotation.x.value = 0;
            rotation.y.value = 0;
            rotation.z.value = 0;
            nn::math::VEC3 position;
            bool isPlaced = false;
            if (netgame::IsMultiplePlayer()) {
                // the position the host sent (entries 10 and 11)
                const NpcPositionEntry& entry = GetNpcPositions()->GetEntry(i + 10);
                if (stage::GetCurrent() == entry.stage) {
                    position.x = static_cast<f32>(entry.GetX());
                    position.y = 96.0f;
                    position.z = static_cast<f32>(entry.GetZ());
                    position.y = FdInfo::GetUnitHeight(position, FIELD_NAME_CURRENT);
                    rotation.y.value = entry.packed[3] << 8;
                    isPlaced = true;
                }
            }
            if (!isPlaced) {
                Func150158(&position, false);
            }
            stage::NpcInfo info;
            info.kind = stage::NpcInfo::KIND_ID;
            info.flag = 0;
            info.value = id | (static_cast<u8>(kind | 0x40) << 16) | (1 << 24);
            oml::framework::Process* actor =
                Actor::Create(PROC_AC_NPC_DT, this, reinterpret_cast<u32>(&info), &position, &rotation);
            if (actor != NULL) {
                m_Npcs.PushBackFast(static_cast<AcNpc*>(actor));
            }
        }
    }
    CreateStageVillagers();
    return true;
}

// 0x0069605C
void BsNpcMgr::CreateNpc(ProcName name, u32 arg, const nn::math::VEC3* position, const math::Angle16Vec3* rotation)
{
    oml::framework::Process* actor = Actor::Create(name, this, arg, position, rotation);
    if (actor != NULL) {
        m_Npcs.PushBackFast(static_cast<AcNpc*>(actor));
    }
}

// 0x007CB794
template void BsNpcMgr::Vector<AcNpc*>::Clear();
