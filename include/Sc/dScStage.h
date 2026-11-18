#pragma once

#include "decomp.h"
#include "Bank/dBank.h"
#include "Bank/dBankTable.h"
#include "Bank/dBankVramConfig.h"
#include "Bs/dBsScene.h"
#include "Npc/dNpcCampingCarInfoLoader.h"
#include "Utl/dUtlBase.h"
#include "g3d/dResourceLoader.h"
#include "nn/math/math_Vector3.h"
#include "ssys/ma/dHeapAllocator.h"
#include "ssys/ma/dLoadSplit.h"
#include "stage/stage_Api.h"

namespace sead {
class ExpHeap;
class Heap;
} // namespace sead
class SvFgName;

// The scene of the stages of the game (class name from the RTTI; all other names are ours). Its initialization loads
// the resources of a stage over several frames (m_InitFlags), makes the heaps of the stage (HeapInit) and builds the
// stage (stage::Build); its finalization frees them again. It also holds the camping car infos (NpcCampingCarInfo):
// those of the two camping cars of the campground (stages CampingCarA/B) and of the visiting characters.
// RTTI 7ScStage @ 0x008CD3E0
// vtable 0x008F919C (vptr 0x008F91A4), offset_to_top 0, 23 entries
class ScStage : public ::UtlBase<BsScene>
{
public:
    typedef Bank<1024, 1, false, BankVramConfig<0, 0, 0, 0, false> > FtrSoundBank;

    static const u32 FTR_SOUND_BANK_NUM = 50;
    // the two camping cars of the campground (left and right: SvTownDataUnkEE2C::campgroundCampingCar)
    static const u32 CAMPING_CAR_NUM = 2;
    // the placed items of a camping car info (Func71FF94)
    static const u32 CAMPING_CAR_ITEM_NUM = 24;
    // the villagers whose camping car infos are applied (Func305AC0)
    static const u32 NPC_NUM = 10;
    // no NPC (m_RequestedNpcId etc.)
    static const u32 NO_NPC = 0xFFFF;

    // the parts that UpdateInitialize has loaded (m_InitFlags)
    enum InitFlag : u8
    {
        INIT_SCENE_RESOURCES = 1 << 0,  // LoadSceneResources
        INIT_CAMPING_CAR_INFO = 1 << 1, // LoadCampingCarData (auto camps, RV stages)
        INIT_ITEM_INDOOR = 1 << 2,      // Item/Param/ItemIndoor.bin (indoor stages, auto camps)
        INIT_NPC_INFO = 1 << 3,         // the camping car infos of the villagers
        INIT_OFFSET_X = 1 << 4,         // Func1193A0 when stage::GetOffsetX is set
        INIT_CAMPING_CAR_ITEMS = 1 << 5,         // the items of the camping cars when stage::GetOffsetZ is set
    };

    // (inline, in its factory 0x007F31BC)
    ScStage()
        : m_IsNextApplied(false), m_IsGameStage(false), m_IsUnitCursorLoaded(false), m_InitFlags(0),
          m_NpcIndex(0), m_RequestedNpcId(NO_NPC), m_LoadingNpcId(NO_NPC), m_LoadedNpcId(NO_NPC), m_HasArea(false),
          m_AreaMin(0.0f, 0.0f, 0.0f), m_AreaMax(0.0f, 0.0f, 0.0f), m_AreaCenter(0.0f, 0.0f, 0.0f)
    {
    }
    virtual ~ScStage(); // 0x0061373C slot 0x00
    // 0x006136B0 slot 0x04 (deleting dtor)
    virtual oml::framework::Result Calc(); // 0x006134F4 slot 0x24
    virtual oml::framework::Result Draw(); // 0x006134EC slot 0x30
    virtual oml::framework::Result vf_0x40(); // 0x0075EAE4 slot 0x40 | virtual slot, introduced by BsScene
    bool StartInitialize() override; // 0x00612394 slot 0x44
    oml::framework::Result UpdateInitialize() override; // 0x00611ECC slot 0x48
    bool EndInitialize() override; // 0x00611B8C slot 0x4C
    bool StartFinalize() override; // 0x00612E18 slot 0x50
    oml::framework::Result UpdateFinalize() override; // 0x00612D48 slot 0x54
    bool EndFinalize() override; // 0x00612B20 slot 0x58

    // the heap "IslandFieldHeap" in parent (HeapInit::s_pIslandFieldHeap; Thumb code in dHeapInitThumb.cpp)
    static sead::ExpHeap* CreateIslandFieldHeap(sead::Heap* parent); // 0x0010C40C | libgarden [tier A]

    // the camping car info of camping car index (NULL: none; ARMCC embeds it in this file and calls its copy elsewhere)
    DECOMP_EMIT_COPY DECOMP_NOINLINE const void* GetCampingCarInfo(u32 index) const // 0x0075E984
    {
        return GetCampingCarInfoImpl(index);
    }
    // the camping car info of the camping car of the current stage (CampingCarA/B; NULL on the other stages)
    DECOMP_EMIT_COPY DECOMP_NOINLINE const void* GetCurrentCampingCarInfo() const // 0x0075E9AC
    {
        return GetCurrentCampingCarInfoImpl();
    }

    // (called by ModuleIncSave; empty)
    static void Func613000(); // 0x00613000
    // one of the items of the requested camping car info at random (wallpaper, carpet, the 16 items of the second list
    // and the 24 placed ones; NAME_EXOTIC_CHAIR without one; ModuleIndoor)
    static SvFgName GetRandomRequestedItem(); // 0x00613004
    // the requested camping car info if it is the one of npcId; else it is requested (NULL; ModuleIndoor)
    static const void* GetRequestedInfo(u32 npcId); // 0x006131D0
    // the song (an item of KIND_MUSIC) of the camping car of the current stage: from the villager table for a villager,
    // else from its camping car info; the empty item otherwise
    static const SvFgName* GetCampingCarMusic(); // 0x00613210
    // requests the camping car info of npcId (Calc loads it; ModuleIndoor)
    static bool RequestInfo(u32 npcId); // 0x006133BC

private:
    // (the bodies of GetCampingCarInfo and GetCurrentCampingCarInfo, embedded in this file)
    DECOMP_ALWAYS_INLINE const void* GetCampingCarInfoImpl(u32 index) const
    {
        if (index >= CAMPING_CAR_NUM) {
            return NULL;
        }
        return m_CampingCarLoaders[index].GetData();
    }
    DECOMP_ALWAYS_INLINE const void* GetCurrentCampingCarInfoImpl() const
    {
        switch (stage::GetCurrent()) {
        case stage::NAME_CAMPING_CAR_A:
            return m_CampingCarLoaders[0].GetData();
        case stage::NAME_CAMPING_CAR_B:
            return m_CampingCarLoaders[1].GetData();
        default:
            return NULL;
        }
    }

    // the animations of the humans, the save data, the strings of the scene and (indoor) the unit cursor; true when
    // all are loaded
    bool LoadSceneResources(); // 0x00612E30
    // the camping car info of the camping car index of the NPC npcId (true: nothing to load or loaded)
    bool LoadCampingCarInfo(u16 npcId, u32 index); // 0x00612F94
    // the tables of the camping cars and the infos of both camping cars
    bool LoadCampingCarData(); // 0x006133E4

public:
    // (the furniture sounds of the stage: GetFurnitureCapacity banks of 1 KB, the heaps named "FtrSoundObject")
    BankTable<FtrSoundBank, FTR_SOUND_BANK_NUM> m_FtrSoundBanks; // 0x018
    bool m_IsNextApplied;                                         // 0x414 stage::ApplyNext was called
    bool m_IsGameStage;                                           // 0x415 neither KIND_INCLUDE_SAVE nor KIND_SCREEN
    bool m_IsUnitCursorLoaded;                                    // 0x416
    ssys::ma::HeapAllocator m_UnitCursorAllocator;                // 0x418 "UnitCursorRes"
    g3d::ResourceLoader m_UnitCursorResource;                     // 0x420 "Ftr/Chip/UnitCursor.bcres"
    u8 m_InitFlags;                                               // 0x528 InitFlag
    NpcCampingCarInfoLoader m_CampingCarLoaders[CAMPING_CAR_NUM];       // 0x52C
    NpcCampingCarInfoLoader m_NpcLoader;                          // 0x704 the info of villager m_NpcIndex
    u32 m_NpcIndex;                                               // 0x7F0
    ssys::ma::LoadSplit m_SeatInfo;                               // 0x7F4 campingCarSeatInfo.bin
    ssys::ma::LoadSplit m_ConpaneInfo;                            // 0x8E0 campingCarConpaneInfo.bin
    ssys::ma::LoadSplit m_BankonString;                           // 0x9CC campingCarBankonString.bin
    ssys::ma::LoadSplit m_ContainerString;                        // 0xAB8 campingCarContainerString.bin
    ssys::ma::LoadSplit m_ItemIndoor;                             // 0xBA4 Item/Param/ItemIndoor.bin
    NpcCampingCarInfoLoader m_RequestedLoader;                    // 0xC90 the info of m_RequestedNpcId
    u32 m_RequestedNpcId;                                         // 0xD7C
    u32 m_LoadingNpcId;                                           // 0xD80
    u32 m_LoadedNpcId;                                            // 0xD84
    bool m_HasArea;                                               // 0xD88 the field area below is set
    nn::math::VEC3 m_AreaMin;                                     // 0xD8C
    nn::math::VEC3 m_AreaMax;                                     // 0xD98
    nn::math::VEC3 m_AreaCenter;                                  // 0xDA4 (x and z)
};
ASSERT_OFFSET(ScStage, m_FtrSoundBanks, 0x18);
ASSERT_OFFSET(ScStage, m_IsNextApplied, 0x414);
ASSERT_OFFSET(ScStage, m_UnitCursorAllocator, 0x418);
ASSERT_OFFSET(ScStage, m_UnitCursorResource, 0x420);
ASSERT_OFFSET(ScStage, m_InitFlags, 0x528);
ASSERT_OFFSET(ScStage, m_CampingCarLoaders, 0x52C);
ASSERT_OFFSET(ScStage, m_NpcLoader, 0x704);
ASSERT_OFFSET(ScStage, m_NpcIndex, 0x7F0);
ASSERT_OFFSET(ScStage, m_SeatInfo, 0x7F4);
ASSERT_OFFSET(ScStage, m_ItemIndoor, 0xBA4);
ASSERT_OFFSET(ScStage, m_RequestedLoader, 0xC90);
ASSERT_OFFSET(ScStage, m_RequestedNpcId, 0xD7C);
ASSERT_OFFSET(ScStage, m_HasArea, 0xD88);
ASSERT_OFFSET(ScStage, m_AreaCenter, 0xDA4);
ASSERT_SIZE(ScStage, 0xDB0);

// the frames of the stage scene (ScStage counts them; the field objects start animations with it; name is ours)
extern s32 g_StageFrameCounter;
