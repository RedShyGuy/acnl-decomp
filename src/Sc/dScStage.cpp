#include <cstring>

#include "Sc/dScStage.h"
#include "Bs/dBsCharRoomViewMgr.h"
#include "Bs/dBsSvMgr.h"
#include "Human/human_Extern.h"
#include "Npc/npc_Extern.h"
#include "Other/dHeapInit.h"
#include "Other/dSaveProcess.h"
#include "Other/dTicketList.h"
#include "Other/dUlcdFramework.h"
#include "Sc/dScStageExtern.h"
#include "Sound/sound_Extern.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvFgBlock.h"
#include "Sv/dSvFgName.h"
#include "Sv/dSvGardenPlus.h"
#include "Sv/dSvHolder.h"
#include "Sv/dSvNpc.h"
#include "Sv/dSvPlayer.h"
#include "Sv/dSvProcExtern.h"
#include "Sv/dSvTown.h"
#include "demo/demo_Api.h"
#include "event/event_Extern.h"
#include "fgobj/fgobj_Extern.h"
#include "field/field_Extern.h"
#include "netgame/dGameFunctions.h"
#include "netgame/dNetGameMgr.h"
#include "netgame/netgame_Api.h"
#include "nn/gx/gx_Api.h"
#include "script/dScriptExtern.h"
#include "script/dStrMgr.h"
#include "stage/stage_Api.h"
#include "stage/stage_Extern.h"
#include "time/time_Api.h"

// 0x00952684
s32 g_StageFrameCounter;

namespace {
// the parameters of nngxGetCmdlistParameteri that EndInitialize reads (as in gx_Cmdlist.cpp)
const GLenum CMDLIST_USED_SIZE = 0x202;
const GLenum CMDLIST_REQUEST_COUNT = 0x203;
const GLenum CMDLIST_BUFFER_SIZE = 0x204;
const GLenum CMDLIST_MAX_REQUEST_COUNT = 0x205;
const GLenum CMDLIST_NAME = 0x207;
const GLenum CMDLIST_REQUEST_ADDRESS = 0x20B;

// the event of the fishing tourney part of SvTownDataUnk1C7A8 (Func309C6C) and the initiative its participants get
const u8 EVENT_FISHING = 8;
const s32 INITIATIVE_FISHING = 84;

// the entries of the tables of the villagers (g_UnknownA84AEC, g_UnknownA8449C)
const u16 VILLAGER_NUM = 399;

// the item of a camping car info when there is no other (GetRandomRequestedItem)
const SvFgName::Name DEFAULT_REQUESTED_ITEM = SvFgName::NAME_EXOTIC_CHAIR;

// the parts of a camping car info for GetRandomRequestedItem: the wallpaper, the carpet, the 16 items of the second
// list and the 24 placed items
const u32 REQUESTED_ITEM_SECOND_LIST = 2;
const u32 REQUESTED_ITEM_PLACED = REQUESTED_ITEM_SECOND_LIST + TicketList::LIST1_NUM;

// the campground part of the shops of the town (inline; the original tests the address for NULL; EndInitialize reads it
// through GetGardenData, the others through GetGardenPlus)
inline SvTownDataUnkEE2C* GetCampground()
{
    return &GetGardenPlus()->data.townData.unk_0xEE2C;
}
inline SvTownDataUnkEE2C* GetCampgroundOfData()
{
    return &GetGardenData()->townData.unk_0xEE2C;
}

// (inline) the weather values that set SvTownFlags::unk_0xF_4 (meaning of the values unknown)
inline bool IsWeather3Or4()
{
    const u8 weather = Func62FB14();
    return weather == 3 || weather == 4;
}
inline bool IsWeather5Or6()
{
    const u8 weather = Func62FB14();
    return weather == 5 || weather == 6;
}

// (inline) the entry of the villager npcId in the table of the characters (NULL: none); its model name is the five
// characters at +10
inline u8* GetNpcEntry(u16 npcId)
{
    if (event::g_UnknownA84AEC.table == NULL || npcId >= VILLAGER_NUM) {
        return NULL;
    }
    return event::g_UnknownA84AEC.table + npcId * 34;
}

// (inline) item index of the requested camping car info (see GetRandomRequestedItem); the empty item without one
inline SvFgName GetRequestedItem(u32 index)
{
    if (index < TicketList::ITEM_MAX) {
        ScStage* scene = stage::GetScene();
        if (scene != NULL) {
            const void* info = scene->m_RequestedLoader.GetData();
            if (info != NULL) {
                if (index < REQUESTED_ITEM_SECOND_LIST) {
                    if (index == 0) {
                        return Func2FCC0C(info);
                    }
                    return Func7202FC(info);
                }
                if (index < REQUESTED_ITEM_PLACED) {
                    return Func720184(info, index - REQUESTED_ITEM_SECOND_LIST);
                }
                SvFgName item(Func71FF94(info, index - REQUESTED_ITEM_PLACED).item);
                item.PrepareForInventory();
                return item;
            }
        }
    }
    return SvFgName::s_Empty;
}
} // namespace

// 0x0061373C slot 0x00
// 0x006136B0 (deleting dtor)
ScStage::~ScStage()
{
    // (only the members are destroyed)
}

// 0x006134F4 slot 0x24
oml::framework::Result ScStage::Calc()
{
    if (stage::IsKind(stage::KIND_INDOOR, stage::GetCurrent()) && !m_HasArea && BsCharRoomViewMgr::s_pInstance != NULL) {
        BsCharRoomViewMgr::s_pInstance->GetRoomRect(&m_AreaMin.x, &m_AreaMin.z, &m_AreaMax.x, &m_AreaMax.z);
        m_AreaCenter.x = (m_AreaMin.x + m_AreaMax.x) * 0.5f;
        m_AreaCenter.z = (m_AreaMin.z + m_AreaMax.z) * 0.5f;
        m_HasArea = true;
    }
    if (stage::IsKind(stage::KIND_INDOOR, stage::GetCurrent())) {
        if (m_LoadingNpcId != NO_NPC) {
            if (m_RequestedLoader.LoadForNpc(m_LoadingNpcId, HeapInit::s_pStageHeap, 1)) {
                m_LoadedNpcId = m_LoadingNpcId;
                m_LoadingNpcId = NO_NPC;
            }
        } else if (m_RequestedNpcId != NO_NPC) {
            if (m_LoadedNpcId == NO_NPC) {
                m_LoadingNpcId = m_RequestedNpcId;
                m_LoadedNpcId = NO_NPC;
            } else if (m_LoadedNpcId != m_RequestedNpcId && m_RequestedLoader.Cancel()) {
                m_LoadingNpcId = m_RequestedNpcId;
                m_LoadedNpcId = NO_NPC;
            }
        }
    }
    Func52E654();
    Func4DEC78();
    std::memset(g_UnknownAAEAF4.data, 0, sizeof(g_UnknownAAEAF4.data));
    g_UnknownAAEAF4.flag = 0;
    g_StageFrameCounter++;
    time::Update();
    if (m_IsGameStage) {
        Func1E6488(Func6311CC());
    }
    return oml::framework::RESULT_SUCCESS;
}

// 0x006134EC slot 0x30
oml::framework::Result ScStage::Draw()
{
    return oml::framework::RESULT_SUCCESS;
}

// 0x0075EAE4 slot 0x40
oml::framework::Result ScStage::vf_0x40()
{
    return oml::framework::RESULT_SUCCESS;
}

// 0x00612394 slot 0x44
bool ScStage::StartInitialize()
{
    if (!m_IsNextApplied) {
        stage::ApplyNext();
        m_IsNextApplied = true;
    }
    if (!demo::CheckOrder(demo::ORDER_9, false)) {
        time::Update();
    }
    // the fruit of the town to the players (when the current one has none, or in the village or the station)
    if (!netgame::IsMultiplePlayer() && netgame::GetNetgameType() != netgame::TYPE_DREAM) {
        const SvFgName* fruit = &GetGardenData()->townData.townFruit;
        if (fruit->IsUsual() && Func5C3750()) {
            SvPlayer* current = SvGetCurrentPlayer();
            if (current != NULL &&
                (!current->unk_0xA47A.item.IsUsual() || stage::IsKind(stage::KIND_VILLAGE, stage::GetCurrent()) ||
                 stage::GetCurrent() == stage::NAME_STATION)) {
                SvPlayerData* players = &GetGardenData()->players;
                for (u32 i = 0; i < 4; i++) {
                    SvPlayer* player = players->Get(i);
                    if (player != NULL && player->playerInfo.IsValid()) {
                        player->unk_0xA47A.item = *fruit;
                    }
                }
            }
        }
    }

    m_IsGameStage = !stage::IsKind(stage::KIND_INCLUDE_SAVE | stage::KIND_SCREEN, stage::GetCurrent());
    if (m_IsGameStage) {
        Func1E8AAC(Func6311CC());
    }

    const stage::Name current = stage::GetCurrent();
    if (stage::GetCurrent() == stage::NAME_DOWNTOWN) {
        Func52E1B8(1);
    } else {
        u32 mode;
        if (stage::IsKind(stage::KIND_EXHIBITION, stage::GetCurrent())) {
            mode = 2;
        } else if (stage::GetCurrent() == stage::NAME_LOBBY_ISLAND) {
            mode = 4;
        } else {
            mode = stage::IsKind(stage::KIND_ISLAND_DEMO, stage::GetCurrent()) ? 3 : 0;
        }
        Func52E1B8(mode);
    }

    // the heaps of the stage
    sead::Heap* heap = HeapInit::s_pStageHeap;
    if (heap != NULL) {
        if (stage::IsKind(stage::KIND_EXHIBITION, stage::GetCurrent()) ||
            stage::IsKind(stage::KIND_MODEL_HOUSE, stage::GetCurrent()) || current == stage::NAME_SAVE ||
            current == stage::NAME_PLAYER_SELECT) {
            if (HeapInit::s_pExhibitionParkHeap == NULL) {
                HeapInit::CreateExhibitionParkHeap(heap);
            }
        } else if (HeapInit::s_pExhibitionParkHeap != NULL) {
            g_SvExhibitionHolder.Free();
            HeapInit::DestroyExhibitionParkHeap();
        }

        if (stage::IsKind(stage::KIND_KOTOBUKI, stage::GetCurrent()) ||
            stage::IsKind(stage::KIND_ISLAND_DEMO, stage::GetCurrent())) {
            if (HeapInit::s_pIslandFieldHeap == NULL) {
                CreateIslandFieldHeap(heap);
            }
        } else if (HeapInit::s_pIslandFieldHeap != NULL) {
            netgame::Func6A7C4C();
            netgame::Func6A7620();
            HeapInit::DestroyIslandFieldHeap();
        }

        if (stage::IsKind(stage::KIND_OUTDOOR, stage::GetCurrent())) {
            if (stage::IsKind(stage::KIND_VILLAGE, stage::GetCurrent())) {
                HeapInit::CreateStrcHeap(heap, 0x500000);
            } else if (stage::IsKind(stage::KIND_EXHIBITION, stage::GetCurrent())) {
                HeapInit::CreateStrcHeap(heap, 0x44B000);
            } else if (stage::IsKind(stage::KIND_AUTO_CAMP, stage::GetCurrent())) {
                HeapInit::CreateStrcHeap(heap, 0x200000);
            } else {
                HeapInit::CreateStrcHeap(heap, 0x400000);
            }
        }
        if (stage::IsKind(stage::KIND_VILLAGE, stage::GetCurrent())) {
            HeapInit::CreateMyDesignWorkHeap(heap, 76800);
        }

        if (HeapInit::s_pVillageSeasonHeap != NULL) {
            HeapInit::DestroyVillageSeasonHeap();
        }
        if (stage::IsKind(stage::KIND_VILLAGE, stage::GetCurrent())) {
            HeapInit::CreateVillageSeasonHeap(heap);
        }
        if (HeapInit::s_pVrBoxHeap != NULL) {
            HeapInit::DestroyVrBoxHeap();
        }
        if (stage::IsKind(stage::KIND_OUTDOOR, stage::GetCurrent())) {
            HeapInit::CreateVrBoxHeap(heap);
        }

        if (HeapInit::s_pSoundExtStageHeap != NULL) {
            HeapInit::DestroySoundExtStageHeap();
        }
        if (stage::IsKind(stage::KIND_INDOOR, stage::GetCurrent())) {
            if (stage::IsKind(stage::KIND_PLAYER_HOUSE, stage::GetCurrent()) ||
                stage::IsKind(stage::KIND_NPC_HOUSE, stage::GetCurrent()) ||
                stage::IsKind(stage::KIND_MODEL_HOUSE, stage::GetCurrent()) ||
                stage::IsKind(stage::KIND_ISLAND_HUT, stage::GetCurrent()) ||
                stage::IsKind(stage::KIND_MUSEUM_SHOWROOM, stage::GetCurrent()) ||
                stage::IsKind(stage::KIND_RV_CAR_ENVIRONMENT, stage::GetCurrent()) ||
                stage::IsKind(stage::KIND_CLUB, stage::GetCurrent()) ||
                current == stage::NAME_RESET_MONITORING_CENTER) {
                HeapInit::CreateSoundExtStageHeap(heap, 0);
            }
        } else if (stage::IsKind(stage::KIND_WORLD_CURVE | stage::KIND_OUTDOOR | stage::KIND_SCREEN |
                                     stage::KIND_ISLAND_DEMO,
                                 stage::GetCurrent())) {
            HeapInit::CreateSoundExtStageHeap(heap, 0);
        } else if (stage::IsKind(stage::KIND_MINI_GAME, stage::GetCurrent())) {
            HeapInit::CreateSoundExtStageHeap(heap, 0x100000);
        }
    }

    if (stage::IsKind(stage::KIND_VILLAGE, stage::GetCurrent()) ||
        stage::IsKind(stage::KIND_AUTO_CAMP, stage::GetCurrent()) ||
        stage::GetCurrent() == stage::NAME_RESET_MONITORING_CENTER) {
        if (!netgame::IsMultiplePlayer()) {
            Func4DF0D0();
        }
    }
    if (stage::IsKind(stage::KIND_OUTDOOR, stage::GetCurrent())) {
        Func4E9C78();
    }
    if (heap != NULL) {
        // (the result is not used)
        m_FtrSoundBanks.Initialize(heap, stage::GetFurnitureCapacity(current), "FtrSoundObject");
    }
    Func5C0664();
    if (stage::GetCurrent() == stage::NAME_TITLE) {
        Func5C06A4();
        Func4EA450();
        Func25D018();
    }
    m_InitFlags = 0;
    m_NpcIndex = 0;
    return true;
}

// 0x00611ECC slot 0x48
oml::framework::Result ScStage::UpdateInitialize()
{
    if (!(m_InitFlags & INIT_SCENE_RESOURCES) && LoadSceneResources()) {
        m_InitFlags = (m_InitFlags | INIT_SCENE_RESOURCES) &
                      ~(INIT_CAMPING_CAR_INFO | INIT_ITEM_INDOOR | INIT_NPC_INFO | INIT_OFFSET_X | INIT_CAMPING_CAR_ITEMS);
        m_NpcIndex = 0;
        if (demo::IsFlag1()) {
            Func17D478();
            Func114D18(0);
        }
    }
    if (m_InitFlags & INIT_SCENE_RESOURCES) {
        const stage::Name current = stage::GetCurrent();
        const bool isIndoor = stage::IsKind(stage::KIND_INDOOR, current);
        const bool isAutoCamp = stage::IsKind(stage::KIND_AUTO_CAMP, current);
        const bool isRvCar = stage::IsKind(stage::KIND_RV_CAR_ENVIRONMENT, current);

        if (!(m_InitFlags & INIT_ITEM_INDOOR)) {
            if (!isIndoor && !isAutoCamp) {
                m_InitFlags |= INIT_ITEM_INDOOR;
            } else if (m_ItemIndoor.Read(sead::SafeStringBase<char>("Item/Param/ItemIndoor.bin"),
                                         HeapInit::s_pStageHeap)) {
                m_InitFlags |= INIT_ITEM_INDOOR;
            }
        }
        // (the same block twice in the original)
        if (!(m_InitFlags & INIT_CAMPING_CAR_INFO)) {
            if (!isAutoCamp && !isRvCar) {
                m_InitFlags |= INIT_CAMPING_CAR_INFO;
            } else if (LoadCampingCarData()) {
                m_InitFlags |= INIT_CAMPING_CAR_INFO;
            }
        }
        if (!(m_InitFlags & INIT_CAMPING_CAR_INFO)) {
            if (!isAutoCamp && !isRvCar) {
                m_InitFlags |= INIT_CAMPING_CAR_INFO;
            } else if (LoadCampingCarData()) {
                m_InitFlags |= INIT_CAMPING_CAR_INFO;
            }
        }

        // the items of the camping cars are placed into their stages
        if (!(m_InitFlags & INIT_CAMPING_CAR_ITEMS)) {
            bool isLoaded = true;
            if (stage::GetOffsetZ() != 0) {
                SvTownDataUnkEE2C* camp = GetCampground();
                if (camp != NULL) {
                    const bool isLoaded0 = LoadCampingCarInfo(camp->campgroundCampingCar[0], 0);
                    const bool isLoaded1 = LoadCampingCarInfo(camp->campgroundCampingCar[1], 1);
                    isLoaded = isLoaded1 & isLoaded0;
                }
                if (isLoaded) {
                    for (u32 i = 0; i < CAMPING_CAR_NUM; i++) {
                        const void* info = GetCampingCarInfoImpl(i);
                        if (info == NULL) {
                            continue;
                        }
                        for (u32 j = 0; j < CAMPING_CAR_ITEM_NUM; j++) {
                            const FgPlacement placement = Func71FF94(info, j);
                            if (placement.item.IsUsual()) {
                                const u32 stage = i == 0 ? stage::NAME_CAMPING_CAR_A : stage::NAME_CAMPING_CAR_B;
                                Func319770(placement.x, placement.z, placement.layer, Func319750(&placement.item),
                                           stage);
                            }
                        }
                    }
                }
            }
            if (isLoaded) {
                m_InitFlags |= INIT_CAMPING_CAR_ITEMS;
            }
        }

        if (!(m_InitFlags & INIT_OFFSET_X)) {
            if (stage::GetOffsetX() != 0) {
                Func1193A0(GetGardenPlus()->data.players.players[3].unk_0xA284);
            }
            m_InitFlags |= INIT_OFFSET_X;
        }

        // the camping car infos of the villagers
        if (!(m_InitFlags & INIT_NPC_INFO)) {
            if (stage::IsKind(stage::KIND_VILLAGE, stage::GetCurrent()) ||
                stage::IsKind(stage::KIND_NPC_HOUSE, stage::GetCurrent())) {
                SvNpc* npc = Func305AC0(GetGardenData()->GetNpcData()->npcs, m_NpcIndex);
                if (npc != NULL && !npc->status.unk_0x2_2) {
                    u8* entry = GetNpcEntry(npc->npc.villagerId);
                    if (entry != NULL) {
                        entry[15] = '\0';
                        const char* model = reinterpret_cast<const char*>(entry + 10);
                        if (m_NpcLoader.Load(model != NULL ? model : "wol04", NULL, 1)) {
                            Func1134AC(npc, m_NpcLoader.m_pData);
                        }
                    }
                } else if (m_NpcLoader.Cancel()) {
                    m_NpcIndex++;
                }
                if (m_NpcIndex >= NPC_NUM) {
                    m_InitFlags |= INIT_NPC_INFO;
                }
            } else {
                m_InitFlags |= INIT_NPC_INFO;
            }
        }
    }
    if ((m_InitFlags & INIT_CAMPING_CAR_INFO) && (m_InitFlags & INIT_NPC_INFO) && (m_InitFlags & INIT_ITEM_INDOOR) &&
        (m_InitFlags & INIT_OFFSET_X) && (m_InitFlags & INIT_CAMPING_CAR_ITEMS)) {
        return oml::framework::RESULT_SUCCESS;
    }
    return oml::framework::RESULT_RETRY;
}

// 0x00611B8C slot 0x4C
bool ScStage::EndInitialize()
{
    stage::ClearOffset();
    if (!netgame::IsMultiPlayer() && Func5C3750() &&
        !stage::IsKind(stage::KIND_INCLUDE_SAVE, stage::GetCurrent())) {
        SvTownDataUnkEE2C* camp = GetCampgroundOfData();
        if (camp != NULL && Func1B6350(camp->campgroundCampingCar[1])) {
            u32 npcId = camp->campgroundCampingCar[1];
            switch (npcId) {
            case 0x2001:
            case 0x2002:
            case 0x2003:
            case 0x2006:
            case 0x2008:
            case 0x200A:
            case 0x200B:
            case 0x200D:
            case 0x200E:
                if (g_UnkAAFF18.pPlayer != NULL) {
                    camp->unk_0x17A38[2] = !Func1B5CEC(&npcId);
                }
                break;
            default:
                camp->unk_0x17A38[2] = false;
                break;
            }
        }
        const bool isWeather = IsWeather3Or4() || IsWeather5Or6();
        GetGardenData()->townData.townFlags.unk_0xF_4 = isWeather;
    }

    // the initiative of the players of the fishing tourney
    if (!stage::IsKind(stage::KIND_INCLUDE_SAVE, stage::GetCurrent()) &&
        Func309C6C(EVENT_FISHING, time::GetCurrentDate()) && Func5C3750()) {
        for (s32 i = 0; i < Func725C1C(&GetGardenPlus()->data.townData.unk_0x1C7A8); i++) {
            if (!Func725CB4(&GetGardenPlus()->data.townData.unk_0x1C7A8, i)) {
                continue;
            }
            const u32 slot = Func725C48(&GetGardenPlus()->data.townData.unk_0x1C7A8, i);
            SvPlayer* player = GetGardenData()->players.Get(slot & 0xFF);
            if (player != NULL && player->playerInfo.IsValid()) {
                Func24BA04(&player->initiative, INITIATIVE_FISHING, 1);
            }
        }
    }

    stage::GetCurrent(); // (the result is not used)
    stage::Build(this);
    Func60D024();
    demo::StartFirstOrder();
    // (the values are not used)
    GLint name;
    GLint value;
    nngxGetCmdlistParameteri(CMDLIST_NAME, &name);
    nngxGetCmdlistParameteri(CMDLIST_BUFFER_SIZE, &value);
    nngxGetCmdlistParameteri(CMDLIST_MAX_REQUEST_COUNT, &value);
    nngxGetCmdlistParameteri(CMDLIST_REQUEST_ADDRESS, &value);
    nngxGetCmdlistParameteri(CMDLIST_USED_SIZE, &value);
    nngxGetCmdlistParameteri(CMDLIST_REQUEST_COUNT, &value);

    if (stage::IsKind(stage::KIND_OUTDOOR, stage::GetCurrent())) {
        if (stage::GetCurrent() != stage::NAME_TITLE) {
            UlcdFramework::GetInstance()->m_IsRendering3D = false;
        }
    } else {
        UlcdFramework::GetInstance()->m_IsRendering3D = true;
    }
    return true;
}

// 0x00612E18 slot 0x50
bool ScStage::StartFinalize()
{
    Func139580(Func3044DC());
    Func6D4128();
    return true;
}

// 0x00612D48 slot 0x54
oml::framework::Result ScStage::UpdateFinalize()
{
    bool isDone = true;
    if (!netgame::NetGameMgr::s_pInstance->ProcessLeave()) {
        isDone = false;
    }
    if (!Func6D4574()) {
        isDone = false;
    }
    for (s32 i = 0; i < static_cast<s32>(CAMPING_CAR_NUM); i++) {
        isDone &= m_CampingCarLoaders[i].Cancel();
    }
    isDone &= m_NpcLoader.Cancel();
    isDone &= m_SeatInfo.RequestCancel();
    isDone &= m_ConpaneInfo.RequestCancel();
    isDone &= m_ContainerString.RequestCancel();
    isDone &= m_BankonString.RequestCancel();
    isDone &= m_RequestedLoader.RequestCancel();
    isDone &= m_ItemIndoor.RequestCancel();
    return isDone ? oml::framework::RESULT_SUCCESS : oml::framework::RESULT_RETRY;
}

// 0x00612B20 slot 0x58
bool ScStage::EndFinalize()
{
    stage::GetCurrent(); // (the result is not used)
    Func6E57E4(&g_HumanAnimTable);
    Func4E188C();
    Func62FD0C();
    Func1E83DC();
    Func1E8A48();
    if (stage::IsKind(stage::KIND_EXHIBITION, stage::GetNext()) || HeapInit::s_pExhibitionParkHeap == NULL) {
        stage::SetLastStructure(STRUCTURE_EMPTY_SPACE);
    }
    if (stage::IsKind(stage::KIND_INDOOR, stage::GetCurrent())) {
        m_IsUnitCursorLoaded = false;
        m_UnitCursorResource.Unload();
        m_UnitCursorAllocator.DestroyHeap();
    }
    // (like BankTable::FinalizeTable, without the test of the handed-out banks)
    u32 index = 0;
    for (FtrSoundBank* bank = m_FtrSoundBanks.m_Banks; bank < m_FtrSoundBanks.m_Banks + m_FtrSoundBanks.m_Size;
         bank++, index++) {
        m_FtrSoundBanks.CheckBit(index); // (the result is not used)
        bank->FinalizeBank();
    }
    m_FtrSoundBanks.m_Size = 0;
    stage::Destroy();
    Func133CB8(script::StrMgr::s_pInstance);
    Func4DEDC4();

    // the play time of the stage
    SvPlayer* player = SvGetCurrentPlayer();
    const s64 time = time::GetPlayTimeSeconds();
    if (player != NULL) {
        player->AddPlaytime(time);
    }
    GetGardenData()->AddPlaytime(time);
    time::StartPlayTimer();

    if (!netgame::IsMultiPlayer() && stage::GetNext() == stage::NAME_RECYCLE_SHOP) {
        Func20D838(GetGardenData()->townData.unk_0xEE2C.unk_0x13428);
    }
    Func6D401C();
    if (HeapInit::s_pMyDesignWorkHeap != NULL) {
        HeapInit::DestroyMyDesignWorkHeap();
    }
    if (HeapInit::s_pStrcHeap != NULL) {
        HeapInit::DestroyStrcHeap();
    }
    return true;
}

// 0x00612E30 (name is ours)
bool ScStage::LoadSceneResources()
{
    sead::Heap* heap = HeapInit::s_pStageHeap;
    bool isLoaded = Func6E59D0(&g_HumanAnimTable, heap);
    if (stage::IsKind(stage::KIND_INDOOR, stage::GetCurrent()) || stage::GetCurrent() == stage::NAME_PLAYER_SELECT ||
        demo::IsFlag1()) {
        if (HeapInit::s_pStageHeap != NULL) {
            isLoaded &= Func1C24D8(Func3044DC(), HeapInit::s_pStageHeap);
        }
    }
    isLoaded &= script::StrMgr::s_pInstance->LoadScene(this);
    if (stage::IsKind(stage::KIND_INDOOR, stage::GetCurrent())) {
        if (m_UnitCursorAllocator.m_pHeap == NULL) {
            m_UnitCursorAllocator.CreateExpHeap(20480, heap, sead::SafeStringBase<char>("UnitCursorRes"),
                                                sead::Heap::HEAP_DIRECTION_FORWARD, false);
        }
        if (!m_IsUnitCursorLoaded) {
            m_IsUnitCursorLoaded = m_UnitCursorResource.LoadAsync(
                sead::SafeStringBase<char>("Ftr/Chip/UnitCursor.bcres"), m_UnitCursorAllocator.m_pHeap, 128);
            if (m_IsUnitCursorLoaded) {
                m_UnitCursorResource.InitializeCgfx(&m_UnitCursorAllocator, NULL, true);
            }
        }
        isLoaded &= m_IsUnitCursorLoaded;
    }
    return isLoaded;
}

// 0x00612F94 (name is ours)
bool ScStage::LoadCampingCarInfo(u16 npcId, u32 index)
{
    if (!Func1B6350(npcId) || index >= CAMPING_CAR_NUM) {
        return true;
    }
    const char* model = Func1B6330(npcId);
    return m_CampingCarLoaders[index].Load(model != NULL ? model : "wol04", NULL, 1);
}

// 0x00613000 (name is ours)
void ScStage::Func613000()
{
    // empty (in the original too)
}

// 0x00613004 (name is ours)
SvFgName ScStage::GetRandomRequestedItem()
{
    SvFgNameSet items;
    s32 num = 0;
    for (u32 i = 0; i < TicketList::ITEM_MAX; i++) {
        const SvFgName item = GetRequestedItem(i);
        if (item.IsUsual()) {
            Func302F80(&items, &item);
            num++;
        }
    }
    if (num != 0) {
        const SvFgName item = PickFgName(&items);
        if (item.IsUsual()) {
            return item;
        }
    }
    return SvFgName(static_cast<SvFgName::ID>(DEFAULT_REQUESTED_ITEM));
}

// 0x006131D0 (name is ours)
const void* ScStage::GetRequestedInfo(u32 npcId)
{
    ScStage* scene = stage::GetScene();
    if (scene == NULL) {
        return NULL;
    }
    if (scene->m_LoadedNpcId == npcId) {
        return scene->m_RequestedLoader.GetData();
    }
    if (npcId != NO_NPC) {
        scene->m_RequestedNpcId = npcId;
    }
    return NULL;
}

// 0x00613210 (name is ours)
const SvFgName* ScStage::GetCampingCarMusic()
{
    ScStage* scene = stage::GetScene();
    if (scene == NULL) {
        return &SvFgName::s_Empty;
    }
    // 0x00958250 (guard 0x0095824C)
    static SvFgName s_Music;

    u16 npcId = NO_NPC;
    SvTownDataUnkEE2C* camp = GetCampground();
    if (camp != NULL) {
        switch (stage::GetCurrent()) {
        case stage::NAME_CAMPING_CAR_A:
            npcId = camp->campgroundCampingCar[0];
            if (!Func1B6350(npcId)) {
                npcId = NO_NPC;
            }
            break;
        case stage::NAME_CAMPING_CAR_B:
            npcId = camp->campgroundCampingCar[1];
            if (!Func1B6350(npcId)) {
                npcId = NO_NPC;
            }
            break;
        }
    }
    if (Func1B5A18(npcId)) {
        // a villager: its song from the villager table
        if (g_UnknownA8449C.entries == NULL || npcId >= VILLAGER_NUM) {
            return &SvFgName::s_Empty;
        }
        const SvFgName* music = reinterpret_cast<const SvFgName*>(g_UnknownA8449C.entries[npcId].item);
        if (music->GetKind() == item::KIND_MUSIC) {
            return music;
        }
        return &SvFgName::s_Empty;
    }
    const void* info = scene->GetCurrentCampingCarInfoImpl();
    if (info == NULL) {
        return &SvFgName::s_Empty;
    }
    s_Music = Func720324(info);
    if (s_Music.GetKind() == item::KIND_MUSIC) {
        return &s_Music;
    }
    return &SvFgName::s_Empty;
}

// 0x006133BC (name is ours)
bool ScStage::RequestInfo(u32 npcId)
{
    ScStage* scene = stage::GetScene();
    if (scene == NULL) {
        return false;
    }
    if (npcId != NO_NPC) {
        scene->m_RequestedNpcId = npcId;
    }
    return true;
}

// 0x006133E4 (name is ours)
bool ScStage::LoadCampingCarData()
{
    sead::Heap* heap = HeapInit::s_pStageHeap;
    bool isLoaded = m_BankonString.Read(
        sead::SafeStringBase<char>("NpcCampingCarInfo/campingCarBankonString.bin"), heap);
    isLoaded &= m_ContainerString.Read(
        sead::SafeStringBase<char>("NpcCampingCarInfo/campingCarContainerString.bin"), heap);
    isLoaded &= m_SeatInfo.Read(sead::SafeStringBase<char>("NpcCampingCarInfo/campingCarSeatInfo.bin"), heap);
    isLoaded &= m_ConpaneInfo.Read(sead::SafeStringBase<char>("NpcCampingCarInfo/campingCarConpaneInfo.bin"), heap);
    SvTownDataUnkEE2C* camp = GetCampground();
    if (camp == NULL) {
        return isLoaded;
    }
    isLoaded = LoadCampingCarInfo(camp->campgroundCampingCar[0], 0) & isLoaded;
    return LoadCampingCarInfo(camp->campgroundCampingCar[1], 1) & isLoaded;
}
