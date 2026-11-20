// The Thumb part of BsNpcMgr (Initialize, Finalize, the destructor and the creation of the NPCs of a stage).
#include "Bs/dBsNpcMgr.h"
#include "Ac/dAcNpc.h"
#include "Bs/dBsNpcMgrExtern.h"
#include "Bs/dBsNpcMgr_NpcPosGetCB.h"
#include "event/event_Api.h"
#include "field/dFdInfo.h"
#include "g3d/g3d_System.h"
#include "Bank/bank_Extern.h"
#include "math/dAngle16.h"
#include "netgame/dGameFunctions.h"
#include "netgame/netgame_Api.h"
#include "Other/dHeapInit.h"
#include "Other/dPlHouseGhostSetForeachCB.h"
#include "Other/dProcMgr.h"
#include "Other/dVisitorLocalInfo.h"
#include "stage/stage_Api.h"
#include "stage/stage_Extern.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvGardenPlus.h"
#include "Sv/dSvPlayer.h"
#include "Bs/dBsSvDemo.h"

// 0x0011817C
// 0x00118170 (deleting dtor)
BsNpcMgr::~BsNpcMgr()
{
    // (only the members are destroyed)
}

// 0x00117C3C
oml::framework::Result BsNpcMgr::Initialize()
{
    const bool isPkn = (event::IsActive(event::EVENT_ID_HALLOWEEN_PREPARATION) || event::IsActive(event::EVENT_ID_HALLOWEEN)) &&
                       stage::IsKind(stage::KIND_VILLAGE, stage::GetCurrent());
    const bool isMka =
        event::IsActive(event::EVENT_ID_APRIL_FOOLS_DAY) && stage::IsKind(stage::KIND_NPC_HOUSE, stage::GetCurrent());
    const bool isGuest = stage::IsKind(stage::KIND_PLAYER_HOUSE, stage::GetCurrent()) && !netgame::IsMultiPlayer();

    // the first step: the heaps, the villagers of the day and the guards
    if (!m_LoadFlags.Test(LOAD_SETUP)) {
        s_pInstance = this;
        s_Unknown951180 = 0;
        s_State.unk11 = 0;
        Func5D4C4C();
        s_CounterPeriod = 0;
        s_Counter = 0;
        s_State.unk10 = 0;
        m_Npcs.Clear();
        Func24FAA4();
        BuildRoomFloor();
        m_CmnAllocator.CreateExpHeap(0x1400, HeapInit::s_pStageHeap, sead::SafeStringBase<char>("NPCCmnResAllocator"),
                                     sead::Heap::HEAP_DIRECTION_FORWARD, false);
        SetupAccessory();
        if (isPkn) {
            m_PknAllocator.CreateExpHeap(0x15E00, HeapInit::s_pVillageSeasonHeap,
                                         sead::SafeStringBase<char>("NPCPknResAllocator"),
                                         sead::Heap::HEAP_DIRECTION_FORWARD, false);
        }
        PickDailyOrder();
        if (isMka) {
            m_MkaAllocator.CreateExpHeap(0x15E00, HeapInit::s_pStageHeap, sead::SafeStringBase<char>("NPCMkaResAllocator"),
                                         sead::Heap::HEAP_DIRECTION_FORWARD, false);
        }
        if (isGuest) {
            m_GstAllocator.CreateExpHeap(0x15E00, HeapInit::s_pStageHeap, sead::SafeStringBase<char>("NPCGstResAllocator"),
                                         sead::Heap::HEAP_DIRECTION_FORWARD, false);
        }
        SvNpc* npcs = GetGardenData()->GetNpcData()->npcs;
        const stage::Name current = stage::GetCurrent();
        if (current == stage::NAME_DOWNTOWN && !netgame::IsMultiPlayer() && netgame::IsHost()) {
            Func30B458(0);
        }
        Func111064(npcs);
        Func699540();
        SetupStageVillagers();
        if (stage::IsKind(stage::KIND_INDOOR, current)) {
            Func51DB48();
        }
        m_Timer = 5400;
        for (s32 i = 0; i < 10; i++) {
            u8* extra = static_cast<u8*>(Func2FE4A0(i));
            if (extra != NULL) {
                const u8 value = extra[28];
                extra[28] = 165;
                extra[29] = value;
            }
        }
        m_LoadFlags.Set(LOAD_SETUP, true);
        if (stage::IsKind(stage::KIND_VILLAGE, stage::GetCurrent())) {
            SetupVillageGuards();
        } else if (stage::IsKind(stage::KIND_DOWNTOWN, stage::GetCurrent())) {
            Func14FD38(this);
        } else if (stage::IsKind(stage::KIND_EXHIBITION, stage::GetCurrent())) {
            Func14FD40(this);
        }
    }

    // the resources, one step per frame
    bool isDone = true;
    if (!m_LoadFlags.Test(LOAD_CMN)) {
        if (!m_CmnLoader.LoadAsync(sead::SafeStringBase<char>("Npc/Normal/CMN.bcres"), m_CmnAllocator.m_pHeap,
                                   g3d::ResourceLoader::CGFX_ALIGNMENT)) {
            isDone = false;
        } else {
            m_CmnLoader.InitializeCgfx(&m_CmnAllocator, NULL, true);
            m_LoadFlags.Set(LOAD_CMN, true);
        }
    }
    if (isPkn && !m_LoadFlags.Test(LOAD_PKN)) {
        if (!m_PknLoader.LoadAsync(sead::SafeStringBase<char>("Npc/Special/Model/pkn.bcres"), m_PknAllocator.m_pHeap,
                                   g3d::ResourceLoader::CGFX_ALIGNMENT)) {
            isDone = false;
        } else if (!m_LoadFlags.Test(LOAD_CMN)) {
            isDone = false;
        } else {
            m_PknLoader.InitializeCgfx(&m_PknAllocator, NULL, true);
            m_PknLoader.InitializeCgfx(&m_PknAllocator, &m_CmnLoader, true);
            const u32 num = m_PknLoader.m_GraphicsFile.ptr()->models.count;
            for (u32 i = 0; i < num; i++) {
                g3d::SetLightSetIndex(static_cast<const nw::gfx::res::ResModelData*>(m_PknLoader.m_GraphicsFile.GetModels(i)),
                                 Func1E891C());
            }
            m_LoadFlags.Set(LOAD_PKN, true);
        }
    }
    if (isMka && !m_LoadFlags.Test(LOAD_MKA)) {
        if (!m_MkaLoader.LoadAsync(sead::SafeStringBase<char>("Npc/Special/Model/mka.bcres"), m_MkaAllocator.m_pHeap,
                                   g3d::ResourceLoader::CGFX_ALIGNMENT)) {
            isDone = false;
        } else if (!m_LoadFlags.Test(LOAD_CMN)) {
            isDone = false;
        } else {
            m_MkaLoader.InitializeCgfx(&m_MkaAllocator, NULL, true);
            m_MkaLoader.InitializeCgfx(&m_MkaAllocator, &m_CmnLoader, true);
            const u32 num = m_MkaLoader.m_GraphicsFile.ptr()->models.count;
            for (u32 i = 0; i < num; i++) {
                g3d::SetLightSetIndex(static_cast<const nw::gfx::res::ResModelData*>(m_MkaLoader.m_GraphicsFile.GetModels(i)),
                                 Func1E891C());
            }
            m_LoadFlags.Set(LOAD_MKA, true);
        }
    }
    if (!LoadAccessory(static_cast<u8>(m_AccessoryKind))) {
        isDone = false;
    }
    if (netgame::IsMultiPlayer()) {
        Func29D0E0(netgame::g_pUnknown954884);
    }
    // the NPCs of the stage
    if (!m_LoadFlags.Test(LOAD_NPCS)) {
        if (!CreateStageNpcs()) {
            return oml::framework::RESULT_FAIL;
        }
        m_LoadFlags.Set(LOAD_NPCS, true);
        if (stage::GetCurrent() == stage::NAME_TITLE) {
            Func68FCFC();
        }
    }
    return isDone ? oml::framework::RESULT_SUCCESS : oml::framework::RESULT_RETRY;
}

// 0x00100684
oml::framework::Result BsNpcMgr::Finalize()
{
    s_pInstance = NULL;
    m_CmnLoader.Unload();
    m_CmnAllocator.DestroyHeap();
    Func12DD40(this);
    if (m_PknAllocator.m_pHeap != NULL) {
        m_PknLoader.Unload();
        m_PknAllocator.DestroyHeap();
    }
    if (m_MkaAllocator.m_pHeap != NULL) {
        m_MkaLoader.Unload();
        m_MkaAllocator.DestroyHeap();
    }
    if (m_GstAllocator.m_pHeap != NULL) {
        m_GstLoader.Unload();
        m_GstAllocator.DestroyHeap();
    }
    s_Unknown951168 = 0;
    if (m_GuardBoxA.IsActive()) {
        m_GuardBoxA.Remove();
    }
    if (m_GuardBoxB.IsActive()) {
        m_GuardBoxB.Remove();
    }
    for (s32 i = 0; i < 2; i++) {
        if (m_GuardBoxes[i].IsActive()) {
            m_GuardBoxes[i].Remove();
        }
    }
    Func12DAE4();
    if (stage::IsKind(stage::KIND_NPC_HOUSE, stage::GetCurrent())) {
        Func14FCB0();
    }
    return oml::framework::RESULT_SUCCESS;
}

// 0x0011782C
bool BsNpcMgr::IsNpcAt(const nn::math::VEC3* position)
{
    s32 x;
    s32 z;
    FdInfo::ToUnit(x, z, *position);
    NpcPosGetCB func(NULL);
    Foreach(func);
    const s32 num = func.m_Num;
    for (s32 i = 0; i < num; i++) {
        const nn::math::VEC3 npcPosition = func.GetPosition(i);
        s32 npcX;
        s32 npcZ;
        FdInfo::ToUnit(npcX, npcZ, npcPosition);
        if (x == npcX && z == npcZ) {
            return true;
        }
    }
    return false;
}

// 0x0011788C
bool BsNpcMgr::CreateStageNpcs()
{
    const stage::Name current = stage::GetCurrent();
    if (stage::IsKind(stage::KIND_MINI_GAME, current)) {
        return true;
    }
    if (stage::IsKind(stage::KIND_SCREEN, stage::GetCurrent()) && CreateQueuedNpcs()) {
        ClearState2();
        return true;
    }
    bool isCreated;
    if (current == stage::NAME_CHIP_TUTORIAL) {
        isCreated = Func14FCE0(this);
    } else if (current == stage::NAME_ISLAND_HIDE_AND_SEEK) {
        isCreated = m_NmlBanks.Create(HeapInit::s_pStageHeap, 4) && Func14FCE8(this);
    } else if (current == stage::NAME_CAMP_NPC) {
        isCreated = Func14FCF0(this);
    } else if (current == stage::NAME_CAFE) {
        isCreated = Func14FCF8(this) && CreateCafeGuest();
    } else if (stage::IsKind(stage::KIND_TRAIN_STATION_ENVIRONMENT, current) && !stage::IsKind(stage::KIND_SCREEN, current)) {
        isCreated = CreateMaiko();
    } else if (current == stage::NAME_RECYCLE_SHOP) {
        isCreated = Func14FD00(this);
    } else if (stage::IsKind(stage::KIND_DOWNTOWN, current)) {
        isCreated = Func14FCD0(this) && Func14FCD8(this);
    } else if (current == 56 || current == stage::NAME_CONVENIENCE_STORE || current == stage::NAME_SUPERMARKET ||
               current == stage::NAME_HOME_CENTER || current == stage::NAME_DEPARTMENT_STORE_1F ||
               current == stage::NAME_DEPARTMENT_STORE_2F) {
        isCreated = CreateShopNpcs(2);
    } else if (current == stage::NAME_DEPARTMENT_STORE_3F) {
        isCreated = CreateShopNpcs(2);
    } else if (current == stage::NAME_CLUB444_DJKK || current == stage::NAME_CLUB444_LIVE) {
        isCreated = CreateShopNpcs(3);
    } else if (current == stage::NAME_TAILOR) {
        isCreated = CreateShopNpcs(2);
    } else if (stage::IsKind(stage::KIND_MUSEUM, current)) {
        isCreated = CreateShopNpcs(2);
    } else if (stage::IsKind(stage::KIND_VILLAGE, current)) {
        s_State.unk17 = 0;
        if (s_State.unk0B) {
            isCreated = CreateAnniversaryNpcs();
        } else {
            isCreated = CreateVillageNpcs() && CreateWalkingVillagers();
        }
    } else if (stage::IsKind(stage::KIND_NPC_HOUSE, current)) {
        isCreated = Func14FD08(this);
    } else if (stage::IsKind(stage::KIND_PLAYER_HOUSE, current)) {
        // (in the Dream Suite the owner stands in the house as a ghost)
        if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
            return CreatePlayerGhostInHouse();
        }
        isCreated = Func14FD10(this) || (Func14FD18(this) && Func14FD20(this));
    } else if (stage::IsKind(stage::KIND_EXHIBITION, current)) {
        Func14FD28(this);
        isCreated = true;
    } else if (current == stage::NAME_RESET_MONITORING_CENTER) {
        isCreated = CreateResetCenterNpcs();
    } else if (current == stage::NAME_CAMPING_CAR_A || current == stage::NAME_CAMPING_CAR_B) {
        isCreated = Func14FD30(this);
    } else {
        isCreated = true;
    }
    if (!isCreated) {
        return false;
    }
    ClearState2();
    return true;
}

// 0x00117AD8
void BsNpcMgr::Func117AD8()
{
    if (!stage::IsKind(stage::KIND_PLAYER_HOUSE, stage::GetCurrent())) {
        return;
    }
    SvPlayer* player = SvGetCurrentPlayer();
    if (player != NULL) {
        Func1E29AC(&player->playerFlags, 1);
    }
}

// 0x00117B00
bool BsNpcMgr::CreatePlayerGhostInHouse()
{
    if (VisitorLocalInfo::s_pInstance == NULL) {
        return true;
    }
    const u8* flags = VisitorLocalInfo::s_pInstance->GetByte42();
    // the main rooms of the four player houses (the bits 1..4: the owner was met already)
    u32 saveNo;
    switch (stage::GetCurrent()) {
    case 3:
        if ((flags[0] >> 1) & 1) {
            return true;
        }
        saveNo = 0;
        break;
    case 9:
        if ((flags[0] >> 2) & 1) {
            return true;
        }
        saveNo = 1;
        break;
    case 15:
        if ((flags[0] >> 3) & 1) {
            return true;
        }
        saveNo = 2;
        break;
    case 21:
        if ((flags[0] >> 4) & 1) {
            return true;
        }
        saveNo = 3;
        break;
    default:
        return true;
    }
    SvPlayerData* players = &GetGardenPlus()->data.players;
    if (!SvIsAnyPlayerSaveNo(static_cast<PlayerSaveNumber>(saveNo))) {
        return true;
    }
    SvPlayer* owner = players->Get(saveNo);
    if (owner != NULL && !owner->playerInfo.IsValid()) {
        return true;
    }
    // a free unit of the room by the road search
    HouseGhostSearch search;
    Func7D05DC(&search);
    PlHouseGhostSetForeachCB func(&search);
    Func7CF740(&search);
    Func7CF8F0(&search, &func);
    Func7D2F64(&search, 7, 13, 0);
    s32 x = 0;
    s32 z = 0;
    if (!Func826F84(&search, &x, &z)) {
        return true;
    }
    if ((x == 7 || x == 8) && z == 13) {
        return true;
    }
    nn::math::VEC3 position;
    FdInfo::ToCenterWorldWithUnitHeight(&position, x, z);
    u32 value;
    Func29CF18(&value, owner);
    stage::NpcInfo info;
    Func29CF30(&info, &value);
    CreateNpc(PROC_AC_NPC_PLAYER_GHOST, reinterpret_cast<u32>(&info), &position, &ANGLE16_VEC3_ZERO);
    return true;
}
