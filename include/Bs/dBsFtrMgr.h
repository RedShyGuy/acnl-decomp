#pragma once

#include "decomp.h"
#include "Bank/dBank.h"
#include "Bank/dBankTable.h"
#include "Bs/dBsFtrMgr_TvMgr.h"
#include "Bs/dBsFtrMgr_ClockDisplay.h"
#include "Other/dBase.h"
#include "Audio/dAudioObjFurnitureAudio.h"
#include "Sound/dSoundObjTheremin.h"
#include "Sv/dSvFgName.h"
#include "Bs/dBsFtrMgr_Preview.h"
#include "Utl/dUtlBase.h"
#include "math/dAngle16.h"
#include "g3d/dSkeletalAnim.h"
#include "g3d/dSkeletalModel.h"
#include "nn/math/math_Vector3.h"

class Actor;
namespace sead {
class ExpHeap;
}
namespace stage {
enum Name : u8;
}

// the bank of a furniture model (name is ours)
typedef Bank<96000u, 1u, false, BankVramConfig<20500u, 15u, 5500u, 15u, true> > FtrModelBank;

// The furniture of the stage (class name from the RTTI; CreateFtrArg, CreateFtrActor and s_pInstance after libgarden
// bs/BsFtrMgr.hpp, the other names are ours): the banks of the furniture models, the common resources of all furniture
// (FtrCommon.bcres with the model of the push/pull animations), the TV programs, the music of the furniture, the clock
// of the furniture and the items that fly from the player to a place.
// RTTI 8BsFtrMgr @ 0x008CD46C
// vtable 0x008F9904 (vptr 0x008F990C), offset_to_top 0, 22 entries
class BsFtrMgr : public ::UtlBase<Base>
{
public:
    class CommonResLoader;
    struct WorkPool;

    // the theremin sound (SoundObjTheremin) with a started flag (code at 0x002AA658..; names are ours)
    struct ThereminSound : public ::SoundObjTheremin
    {
        ThereminSound(); // 0x002AA6D0
        void Start(); // 0x002AA658
        // the listener of the sound is at the place of the player
        void Update(const nn::math::VEC3* position, f32 distance); // 0x002AA670
        void Stop(); // 0x002AA6A8

        bool m_IsStarted; // 0x98
        u8 _99[3];
    };

    // the furniture actor of an item at a unit of the stage (libgarden)
    struct CreateFtrArg
    {
        enum PlaceType : u8
        {
            PLACE_TYPE_DEFAULT = 0,
            PLACE_TYPE_DISPLAY = 1,
            PLACE_TYPE_UNKNOWN_INVISIBLE = 2, // (created even if the item has no furniture actor of its own)
            PLACE_TYPE_DISPLAY_2 = 3,
            PLACE_TYPE_UNKNOWN = 4,
        };

        SvFgName item;  // 0x0
        u8 x;           // 0x4
        u8 y;           // 0x5
        u8 rotation;    // 0x6
        u8 layer;       // 0x7
        u8 placeType;   // 0x8 PlaceType
        u8 pad[3];      // 0x9

        CreateFtrArg() {}
        // (the orientation of furniture is cleared from the item; name is ours)
        CreateFtrArg(const SvFgName& item, u8 x, u8 y, u8 rotation, u8 layer, u8 placeType); // 0x0069201C
    };

    // an item that flies from the player to a place in an arc, growing on the way (Calc/Draw; names are ours)
    struct FlyingItem
    {
        DECOMP_EMIT_COPY FlyingItem() {} // 0x0069456C
        DECOMP_EMIT_COPY ~FlyingItem() {} // 0x00694580

        nn::math::VEC3 position; // 0x00
        nn::math::VEC3 target;   // 0x0C (its y is the lowest the item falls to)
        nn::math::VEC3 velocity; // 0x18
        f32 scale;               // 0x24 grows from 0 to 1
        SvFgName item;           // 0x28 (empty: unused)
        u8 timer;                // 0x2C frames to fly
        u8 _2D[3];
    };
    static const s32 FLYING_ITEM_NUM = 3;

    static const s32 MANNEQUIN_HEAP_NUM = 4;

    BsFtrMgr(); // 0x00694F08
    virtual ~BsFtrMgr(); // 0x006950A0 slot 0x00
    virtual oml::framework::Result Calc() override; // 0x00694800 slot 0x24
    virtual oml::framework::Result Draw() override; // 0x006946D4 slot 0x30
    bool StartInitialize() override; // 0x0069114C slot 0x40
    oml::framework::Result UpdateInitialize() override; // 0x00691044 slot 0x44
    bool EndInitialize() override; // 0x006906C8 slot 0x48
    bool StartFinalize() override; // 0x006921E0 slot 0x4C
    oml::framework::Result UpdateFinalize() override; // 0x006921C0 slot 0x50
    bool EndFinalize() override; // 0x00691E6C slot 0x54

    // the furniture actor of an item (NULL: the item has none or is not shown here; libgarden)
    static Actor* CreateFtrActor(const CreateFtrArg& arg); // 0x00690F8C | libgarden [tier A]
    // an item flies from the player to target (nothing without a manager)
    static void ThrowItem(const SvFgName& item, const nn::math::VEC3& target); // 0x00691CC0
    void StartFlyingItem(FlyingItem* items, const SvFgName& item, const nn::math::VEC3& target); // 0x00694594

    // the nearest TV (its position and channel) and the nearest theremin to the players (position, distance and index of
    // its furniture actor; -1: none)
    static void FindNearestTvAndTheremin(nn::math::VEC3* tvPosition, u8* channel, nn::math::VEC3* thereminPosition,
                                         f32* distance, s32* thereminIndex); // 0x0069427C
    // the result of a search for an item in front of the player (names are ours)
    struct SearchResult
    {
        nn::math::VEC3 position; // 0x00 the center of the unit
        SvFgName item;           // 0x0C
        SvFgName baseItem;       // 0x10 (the item the found one stands on; else the item itself)
        f32 radius;              // 0x14
        u8 unk18;                // 0x18
        u8 unk19;                // 0x19
        u8 _1A[2];
    };
    // the furniture in front of the player at position looking at angle: the units in front, the own unit, the units
    // diagonally in front and the unit two in front are searched (index of the furniture actor; -2: an item on the
    // ground of the unit in front (no furniture actor there), -1: nothing)
    DECOMP_NOINLINE static s32 SearchFtr(SearchResult* out, const nn::math::VEC3* position, math::Angle16 angle, u8 mode); // 0x00691A90
    // the search at one unit (step: the index of the search order; -1: nothing)
    static s32 SearchFtrAtUnit(SearchResult* out, s32 x, s32 z, u8 layer, const nn::math::VEC3* position, math::Angle16 angle,
                               u32 step, u8 mode); // 0x00692C38
    // the unit x, z can be reached from the unit fromX, fromZ (next to it, or nothing in the way along x or along z)
    static bool IsUnitReachable(s32 fromX, s32 fromZ, s32 x, s32 z); // 0x00692654
    // (Calc; in model houses and other players' showrooms, else where the stage allows it) on A the furniture in front
    // of the player talks (its item and the item it stands on are kept in s_TalkItem/s_TalkBaseItem)
    static void UpdateTalk(); // 0x0069278C
    // the stage where furniture talks after A (model houses, other players' showrooms, else Func4E79CC and not on
    // tour 2)
    static bool IsTalkStage(stage::Name stage); // 0x00693E34
    // the items of the last talk / they are cleared
    static void GetTalkItems(SvFgName* item, SvFgName* otherItem); // 0x006935AC
    static void ClearTalkItems(); // 0x00694250
    // SearchFtr for the player (-1 without a player)
    static s32 SearchFtrOfPlayer(SearchResult* out, u8 mode); // 0x00691CE8

    // the result of SearchPoint: the item of the furniture and a point of it (meanings unknown; names are ours)
    struct PointResult
    {
        SvFgName item; // 0x0
        s32 unk4;      // 0x4
        s32 unk8;      // 0x8
    };
    // the furniture in front of position that can be used (Func752298 gives the point; once: only the unit in front;
    // 0: none, 1/2: found, 2 if Func751EEC is false)
    static u32 SearchPoint(PointResult* out, const nn::math::VEC3* position, math::Angle16 angle, bool once); // 0x006929C0
    // (ModuleIndoor) SearchPoint for the player (nothing without a player)
    static u32 SearchPointOfPlayer(PointResult* out, bool once); // 0x0069260C

    // where an item can stand at the unit x, z with rotation on layer, the player at playerX, playerZ looking in
    // direction (step: the index of the search order): 0 and out is filled, else the reason (3: the player is not in
    // front of the wall, 4: not possible there, 5: the wall or the unit is not free)
    static s32 CheckPlace(CreateFtrArg* out, s32 x, s32 z, u8 rotation, const SvFgName* item, u8 layer, s32 playerX,
                          s32 playerZ, u32 direction, u32 step); // 0x006935D4
    // a place for the item in front of position looking at angle (the units like SearchFtr, all units of its shape and
    // four rotations; 0: out is filled, 1: no furniture actor can be added, 2: not in a tent, 3/5: wall items: the
    // reason of most of the walls, 4: nowhere)
    static s32 FindPlace(CreateFtrArg* out, const SvFgName* item, const nn::math::VEC3* position, math::Angle16 angle,
                         u32 unused); // 0x00692FDC
    // FindPlace in front of the player (4 without a player)
    static s32 FindPlaceOfPlayer(CreateFtrArg* out, const SvFgName* item, u32 unused); // 0x0069222C
    // FindPlace in front of the player for a custom design: the item of the shape of the design (clothes and hats; the
    // wall versions with isWall), else item
    static s32 FindPlaceOfMyDesign(CreateFtrArg* out, u32 item, u32 design, u32 unused, bool isWall); // 0x00694080
    // the same for a pattern wall canvas or a pattern easel
    static s32 FindPlaceOfMyDesignCanvas(CreateFtrArg* out, u32 design, bool isWallCanvas); // 0x0069453C

    // the furniture actor at position gets a value and changes its state (player actions; meanings unknown; flag: a
    // fixed state 186 for the actors with Func7529D0)
    static void Func691D34(const nn::math::VEC3* position, bool flag, u32 value); // 0x00691D34
    static void Func692088(const nn::math::VEC3* position, bool flag, u32 value); // 0x00692088
    // (ModuleShop) the furniture actors of kind 30/31 (AcFtrCloth/AcFtrStand of ModuleFtr; meanings unknown)
    static void* Func6922E0(void* actor); // 0x006922E0
    static void* Func692BEC(void* actor); // 0x00692BEC
    // (ModuleFtr) the heaps of the mannequins / one of them (NULL out of range)
    static sead::ExpHeap** GetMannequinHeaps(); // 0x006935A0
    static sead::ExpHeap* GetMannequinHeap(sead::ExpHeap* const* heaps, u32 index); // 0x00692320
    // (ModuleFtr) a unit of the current field is free (Func3000F0, else attribute not 0 or 5)
    static bool IsUnitFree(s32 x, s32 z); // 0x00694000
    // the stages with furniture of a house (player houses, model houses, showrooms, NPC houses with flag 0x20)
    static bool IsHouseStage(); // 0x00693EE8
    // the room music and the stereos play in this stage
    static bool IsMusicStage(); // 0x00693F78
    static WorkPool* GetWorkPool(); // 0x00692010
    // (ModuleIndoor) the item of the song of the furniture music (empty: none)
    static SvFgName GetSongItem(); // 0x00692294
    // (ModuleFtr) the loader of the TV receptor model (NULL without TVs in this stage)
    g3d::ResourceLoader* GetTvReceptorLoader(); // 0x006922C8
    // the music of the furniture stops / loads the song (in the stages with music; songs below the number of songs)
    void StopMusic(s32 fadeFrames); // 0x00694770
    void LoadMusic(u32 song); // 0x00694AC8

    // the common model of the furniture with the animations of pushing and pulling: one furniture actor at a time
    // moves its model along the bone of the animation (names are ours)
    struct CommonModel
    {
        ~CommonModel(); // 0x00694EF0

        // the animation starts at position with the rotation for owner (false: another owner moves)
        bool StartPull(const nn::math::VEC3* position, const math::Angle16* rotation, const void* owner); // 0x00694CC4
        bool StartPush(const nn::math::VEC3* position, const math::Angle16* rotation, const void* owner); // 0x00694D94
        // the animation of owner goes on, out: the position of the bone (true: finished, the model is free)
        bool Update(nn::math::VEC3* out, const void* owner); // 0x00694E64

        g3d::SkeletalModel model;     // 0x00
        g3d::SkeletalAnim anim;       // 0x0C
        void* m_pPushAnim;            // 0x24
        void* m_pPullAnim;            // 0x28
        const void* m_pOwner;         // 0x2C
    };
    static CommonModel s_CommonModel;
    static CommonResLoader* GetCommonResLoader(); // 0x00692C2C

    // a pool of equal blocks for the furniture actors (the size is the largest of the table 0x00847E3C, rounded to 32;
    // names are ours)
    struct WorkPool
    {
        // num blocks from the stage heap (true: done or already there)
        bool Initialize(u32 num); // 0x00694B74
        void* Alloc(); // 0x00694C50
        bool Free(void* block); // 0x00694B0C
        // (inline in EndFinalize)
        DECOMP_ALWAYS_INLINE void Finalize();
        DECOMP_EMIT_COPY ~WorkPool() {} // 0x00694CC0

        u32 m_BlockSize;  // 0x00
        u32 m_Num;        // 0x04
        u8* m_pBuffer;    // 0x08
        u8 m_Bits[7];     // 0x0C the blocks in use
        u8 _13;
    };
    static WorkPool s_WorkPool;

    static BsFtrMgr* s_pInstance; // libgarden

    BankTable<FtrModelBank, 50u> m_ModelBanks;          // 0x014
    FlyingItem m_FlyingItems[FLYING_ITEM_NUM];          // 0x410
    FtrTvMgr m_TvMgr;                                   // 0x4A0
    AudioObjFurnitureAudio m_MusicAudio;                // 0x71C the music of the furniture (stereos)
    ThereminSound m_ThereminSound;                      // 0x7B4
    s32 m_MusicFtrIndex;                                // 0x850 the furniture actor that plays the music (-1: none)
    s32 m_SongIndex;                                    // 0x854 the song of the music (GetSongItem; -1: none)
    u32 m_MusicStartTime[2];                            // 0x858 (64 bits; 0: not started)
    bool m_Unknown860;                                  // 0x860 (false: the music of the furniture plays from the start)
    bool m_IsTvEnabled;                                 // 0x861
    bool m_Unknown862;                                  // 0x862
    u8 _863;
    f32 m_Frame60;                                      // 0x864 counts 0..59 (from the time at the start)
    FtrClockDisplay m_ClockDisplay;                     // 0x868
    // (indoor stages) the actors of the items on the walls
    oml::framework::Process* m_pWallHangBack;           // 0x8B8 PROC_AC_OBJ_WALL_HANG_BACK
    oml::framework::Process* m_pWallHangUnitCursor;     // 0x8BC PROC_AC_OBJ_WALL_HANG_UNIT_CURSOR
};
ASSERT_OFFSET(BsFtrMgr, m_ModelBanks, 0x14);
ASSERT_OFFSET(BsFtrMgr, m_FlyingItems, 0x410);
ASSERT_OFFSET(BsFtrMgr, m_TvMgr, 0x4A0);
ASSERT_OFFSET(BsFtrMgr, m_MusicAudio, 0x71C);
ASSERT_OFFSET(BsFtrMgr, m_ThereminSound, 0x7B4);
ASSERT_OFFSET(BsFtrMgr, m_MusicFtrIndex, 0x850);
ASSERT_OFFSET(BsFtrMgr, m_ClockDisplay, 0x868);
ASSERT_SIZE(BsFtrMgr, 0x8C0);
