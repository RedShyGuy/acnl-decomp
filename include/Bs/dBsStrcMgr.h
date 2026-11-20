#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"
#include "g3d/dResourceLoader.h"
#include "mutil/dBaseResourceMgr.h"
#include "nn/math/math_Vector3.h"
#include "sead/seadPtrArrayImpl.h"
#include "ssys/ma/dHeapAllocator.h"

class Actor;
class AcStrc;
namespace g3d {
class TransformNode;
}

// The fading value between day and night of the structures (0x1C; code at 0x006AA388..0x006AA640 of another file, not
// decompiled yet; names are ours).
struct StrcDayNight
{
    StrcDayNight(); // 0x006AA614
    // the target (night: 1) is set (fade: in steps of 0.2 per call of Update, else at once; meanings of the flags unknown)
    bool Set(bool isNight, bool fade, bool flag); // 0x006AA388
    void Reset(bool isNight); // 0x006AA444
    void Update(); // 0x006AA558

    u8 m_State;     // 0x00
    u8 _01[3];
    f32 m_Target;   // 0x04
    f32 m_Value;    // 0x08 0..1
    f32 m_Unknown0C; // 0x0C
    f32 m_Step;     // 0x10
    s16 m_Frame;    // 0x14
    s16 m_Frames;   // 0x16
    bool m_IsNight; // 0x18
    u8 _19[3];
};
ASSERT_SIZE(StrcDayNight, 0x1C);

// The structures of the stage (class name from the RTTI, s_pInstance after libgarden bs/BsStrcMgr.hpp, all other names are
// ours): the structure actors of the field (AcStrc and others of the modules), their common resources (the lookup tables of
// hobj/sobj/fobj/dobj and the door animations), the resources of the field objects (Fobj) and fences in own heaps, the time
// of day and the fading between day and night, the positions of the NPCs near the structures.
// RTTI 9BsStrcMgr @ 0x008CD754
// vtable 0x008FA4D0 (vptr 0x008FA4D8), offset_to_top 0, 22 entries
class BsStrcMgr : public ::UtlBase<Base>
{
public:
    typedef mutil::BaseResourceMgr<g3d::ResourceLoader, 13u> CommonResMgr;
    typedef mutil::BaseResourceMgr<g3d::ResourceLoader, 64u> FobjResMgr;
    typedef mutil::BaseResourceMgr<g3d::ResourceLoader, 15u> FenceResMgr;

    // the common resources (the order of m_CommonKeys)
    enum CommonRes
    {
        COMMON_RES_LUT_HOUSE = 0,
        COMMON_RES_LUT_SOBJ = 1,
        COMMON_RES_LUT_FIELDOBJ = 2,
        COMMON_RES_LUT_BUILDING = 3,
        COMMON_RES_HOBJ_DOOR_IN = 4,
        COMMON_RES_HOBJ_DOOR_OUT = 5,
        COMMON_RES_DOBJ_DOOR_A_IN = 6,
        COMMON_RES_DOBJ_DOOR_A_OUT = 7,
        COMMON_RES_DOBJ_DOOR_B = 8,
        COMMON_RES_DOBJ_DOOR_C_IN = 9,
        COMMON_RES_DOBJ_DOOR_C_OUT = 10,
        COMMON_RES_DOBJ_DOOR_D = 11,
        COMMON_RES_DOBJ_DOOR_E = 12,
        COMMON_RES_NUM = 13,
    };
    // the state of the initialization (s_InitState)
    static const u8 INIT_STATE_READY = 255;

    // models drawn in this frame (Draw draws them in scene 0 and clears the list)
    struct DrawList
    {
        static const s32 MODEL_NUM = 4;

        g3d::TransformNode* models[MODEL_NUM]; // 0x00
        u32 num;                               // 0x10
    };
    // a bridge of the town with its kind and position (EndInitialize)
    struct Bridge
    {
        Bridge(); // 0x006DB3AC (inline copy)

        u8 kind;                 // 0x0 (252: none)
        u8 _1[3];
        nn::math::VEC3 position; // 0x4
    };
    static const s32 BRIDGE_NUM = 3;
    static const s32 NPC_POSITION_NUM = 10;

    BsStrcMgr(); // 0x006E0E74
    virtual ~BsStrcMgr(); // 0x006E12B8 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x006E12A8 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual oml::framework::Result Calc() override; // 0x006E0620 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual oml::framework::Result Draw() override; // 0x006DF8E0 slot 0x30 | slot vf_0x30 of oml::framework::Process
    bool StartInitialize() override; // 0x006DC4A0 slot 0x40
    oml::framework::Result UpdateInitialize() override; // 0x006DB884 slot 0x44
    bool EndInitialize() override; // 0x006DB3B0 slot 0x48
    bool StartFinalize() override; // 0x006DD610 slot 0x4C
    oml::framework::Result UpdateFinalize() override; // 0x006DD45C slot 0x50
    bool EndFinalize() override; // 0x006DCD90 slot 0x54

    // a structure of a kind is placed at a unit and the field gets it; if one is placed already, it is kept for Calc
    // (m_Unknown6B51..m_Unknown6B5C; meaning of the kinds unknown)
    void Func6E0A74(s32 x, s32 z, u8 kind); // 0x006E0A74
    // the structure actors of the field and of the stage (UpdateInitialize, state 2)
    void CreateStrcs(); // 0x006DF9CC
    // the structure actor of a kind is sorted into the arrays (inline in CreateStrcs; name is ours)
    void AddStrc(Actor* actor);
    // the structures of Main Street / the island / the tours / the outdoor and exhibition stages (CreateStrcs)
    void CreateDowntownStrcs(); // 0x006DE08C
    void CreateIslandStrcs(); // 0x006DED44
    void CreateTourStrcs(); // 0x006DEB3C
    void CreateOutdoorStrcs(); // 0x006DE378
    // (inline in CreateOutdoorStrcs; name is ours)
    DECOMP_ALWAYS_INLINE void CreateShowcaseHouses(u32 type, u32 section);
    // (meaning unknown; returns 0xFF)
    static u8 Func6DED28(); // 0x006DED28
    // the door sounds of the current stage (house of a player or villager, or a stage with its own door): knocking, opening,
    // closing (0xFFFFFFFF: none), the melody of the door and a flag (tent and automatic doors; meaning unknown);
    // false: no door known
    static bool GetDoorSounds(u32* knock, u32* open, u32* close, u32* melody, bool* flag); // 0x006DCD98
    // the town plaza gets a call of its module function (meanings unknown)
    void Func6DCD74(); // 0x006DCD74
    void Func6DD878(); // 0x006DD878
    // the structure actor at a unit (the object index of the field at the unit; NULL: none)
    Actor* FindStrcAt(s32 x, s32 z); // 0x006DB728
    // the position is in front of or far behind the point of the camera, else Func68F5E8 if test (unused: not read)
    static bool IsOutOfCameraDepth(const nn::math::VEC3* position, bool test, f32 unused, f32 depth); // 0x006DD68C
    // the byte of an index of g_Unknown998C70: low 4 bits value, bits 4.. flags (meanings unknown)
    static void SetUnknown998C70(u32 index, u32 bit, u32 value); // 0x006DD700
    static void SetUnknown998C70Bit(u32 index, u32 bit); // 0x006DD7C8
    // (ModuleOutdoor) a test of the town, the player and the hour (after 19 or 21 o'clock; meaning unknown)
    static bool Func6DCC34(); // 0x006DCC34
    // a value of the module function of the structure of a kind (-1: none; meaning unknown)
    f32 Func6DD968(u8 kind); // 0x006DD968
    // (StageOpusVillageDowntown) the town plaza gets a call of its module function
    void Func6DDC18(); // 0x006DDC18
    static void GetUnknown998D68(nn::math::VEC3* out); // 0x006DDC34
    // (ModulePrologue/ModuleRealVillage) a structure of a kind is placed at a unit (kinds of Func5CAF84, 76..79 and the
    // player houses; in m_Unknown67BC; nothing if one is placed) / it is removed again
    void PlaceStrc(s32 x, s32 z, u8 kind); // 0x006DDA04
    void RemovePlacedStrc(); // 0x006DDB48
    static void ClearUnknown998C70Head(); // 0x006DDE84
    // (SubState of the BGM) the town plaza gets a call of its module function
    void Func6DDE90(); // 0x006DDE90
    static void GetUnknown998D80(nn::math::VEC3* out); // 0x006DDEAC
    static void GetUnknown998D74(nn::math::VEC3* out); // 0x006DDED0
    // the structure of a kind (all arrays; in m_Strcs only 105; NULL: none) / of a DemoActor id (the player houses by kind)
    AcStrc* FindStrcOfKind(u8 kind); // 0x006DDC58
    AcStrc* FindStrcById(u32 id); // 0x006DF300
    // the byte +0x89 of the structure of an object index becomes 0 / 1 (meaning unknown)
    void ClearStrcFlag89(s32 index); // 0x006DDEF4
    void SetStrcFlag89(s32 index); // 0x006DDFC0
    void SetStrcFlag89(s32 index, u8 value);
    // the structure of an object index is removed (m_Unknown68C0: stopped and erased; m_Strcs: flag cleared, slot 0x98)
    void RemoveStrcAt(s32 index); // 0x006DF480
    // the structure nearest to a position (the units; NULL: none)
    AcStrc* FindNearestStrc(const nn::math::VEC3* position); // 0x006DEEE8
    template <typename T>
    void FindNearestStrcIn(T* const* begin, T* const* end, s32 x, s32 z, const nn::math::VEC3* position, bool height,
                           AcStrc** nearest, s32* distance);
    // the villager house of an index (the structure kind index + 8 in m_Unknown6774; NULL: none)
    AcStrc* FindVillagerHouse(u32 index); // 0x006DD804
    // Franklin's Table in m_Unknown67A8 (NULL: none)
    AcStrc* FindFranklinsTable(); // 0x006DD8FC
    bool IsUnknown67BCEmpty(); // 0x006DD94C
    // the position of a bridge (NULL: none)
    const nn::math::VEC3* GetBridgePosition(u32 index); // 0x006DE338
    // the boat when there is exactly one (NULL: none)
    Actor* GetBoat(); // 0x006E0938
    // (ModulePrologue) the house of m_Unknown67BC gets a call of its module function (meaning unknown)
    void Func6E0E24(); // 0x006E0E24
    // (static helpers of the file; meanings unknown unless named)
    static u32 Func6DB524(); // 0x006DB524
    static void GetUnknown998DA4(nn::math::VEC3* out); // 0x006DD618
    static f32 Func6DD63C(); // 0x006DD63C
    static u8 GetUnknown998C70(u32 index); // 0x006DD648
    static void GetUnknown998D5C(nn::math::VEC3* out); // 0x006DD65C
    static f32 Func6DD680(); // 0x006DD680
    static void Func6DD7E4(); // 0x006DD7E4
    DECOMP_NOINLINE static void ClearUnknown998C70(); // 0x006DD7F4
    static void ClearUnknown998C70Bit(u32 index, u32 bit); // 0x006DD85C
    static u32 Func6DD894(); // 0x006DD894
    static u8 Func6DE370(); // 0x006DE370
    static u32 Func6E0958(); // 0x006E0958
    // (ModuleIndoor/ModuleOutdoor) the multiplayer
    static bool IsMultiplayer(); // 0x006DED30

    // the common resources (the dobj_door_a files have no getter; 0x006DF710/0x006DF7F8 return the hobj door animations)
    g3d::ResourceLoader* GetHouseLut(); // 0x006DB618
    // the structure of an index of m_Strcs fills out through its slot 0x9C (the module structures write their model
    // first; the original calls it on NULL too when the index is too big)
    void GetStrcInfo(void* out, u32 index); // 0x006DB68C
    g3d::ResourceLoader* GetSobjLut(); // 0x006DB6B4
    g3d::ResourceLoader* GetFieldObjLut(); // 0x006DB5A4
    g3d::ResourceLoader* GetBuildingLut(); // 0x006DB530
    g3d::ResourceLoader* GetHobjDoorInAnim(); // 0x006DF244
    g3d::ResourceLoader* GetHobjDoorOutAnim(); // 0x006DF40C
    g3d::ResourceLoader* GetDoorInAnim(); // 0x006DF710
    g3d::ResourceLoader* GetDoorOutAnim(); // 0x006DF7F8
    g3d::ResourceLoader* GetDobjDoorBAnim(); // 0x006DF5B4
    g3d::ResourceLoader* GetDobjDoorCInAnim(); // 0x006DF784
    g3d::ResourceLoader* GetDobjDoorCOutAnim(); // 0x006DF86C
    g3d::ResourceLoader* GetDobjDoorDAnim(); // 0x006DF628
    g3d::ResourceLoader* GetDobjDoorEAnim(); // 0x006DF69C

    // libgarden
    static BsStrcMgr* s_pInstance;
    static u8 s_InitState;

    ssys::ma::HeapAllocator m_Allocator;                 // 0x0014 "BsStrcMgr::Allocator"
    CommonResMgr m_CommonResMgr;                         // 0x001C
    mutil::ResourceKey m_CommonKeys[COMMON_RES_NUM];     // 0x0EB4
    ssys::ma::HeapAllocator m_FobjAllocator;             // 0x0F1C "BsStrcMgr(Village|Island|AutoCamp)::FobjAllocator"
    FobjResMgr m_FobjResMgr;                             // 0x0F24
    ssys::ma::HeapAllocator m_Unknown5650;               // 0x5650
    FenceResMgr m_FenceResMgr;                           // 0x5658
    ssys::ma::HeapAllocator m_FenceAllocator;            // 0x6728 "BsStrcMgr::FenceAllocator"
    DrawList m_DrawLists[2];                             // 0x6730
    sead::FixedPtrArray<Actor, 4> m_PlayerHouses;        // 0x6758
    sead::FixedPtrArray<Actor, 10> m_Unknown6774;        // 0x6774 (Func6CB77C)
    sead::FixedPtrArray<Actor, 2> m_Unknown67A8;         // 0x67A8 (field object list 1)
    sead::FixedPtrArray<Actor, 1> m_Unknown67BC;         // 0x67BC
    sead::FixedPtrArray<AcStrc, 50> m_Strcs;             // 0x67CC
    sead::FixedPtrArray<Actor, 1> m_Boats;               // 0x68A0
    sead::FixedPtrArray<Actor, 1> m_DemoPier;            // 0x68B0 (the pier of the island demo)
    sead::FixedPtrArray<Actor, 100> m_Unknown68C0;       // 0x68C0
    sead::FixedPtrArray<Actor, 1> m_Unknown6A5C;         // 0x6A5C (field object list 12)
    sead::FixedPtrArray<Actor, 5> m_Unknown6A6C;         // 0x6A6C
    s32 m_Hour;                                          // 0x6A8C
    s32 m_Minute;                                        // 0x6A90
    s32 m_Second;                                        // 0x6A94
    s32 m_Unknown6A98;                                   // 0x6A98 (the hour at the last full hour)
    s32 m_Unknown6A9C;                                   // 0x6A9C (the minute at the last full minute)
    s32 m_Unknown6AA0;                                   // 0x6AA0
    s32 m_Unknown6AA4;                                   // 0x6AA4
    s32 m_LastSecond;                                    // 0x6AA8
    StrcDayNight m_DayNight;                             // 0x6AAC
    s32 m_DayNightValue;                                 // 0x6AC8 (0..255)
    bool m_IsNight;                                      // 0x6ACC
    u8 m_Unknown6ACD;                                    // 0x6ACD
    u8 m_Unknown6ACE;                                    // 0x6ACE (a closing door of the long animation stays open)
    u8 _6ACF;
    nn::math::VEC3 m_NpcPositions[NPC_POSITION_NUM];     // 0x6AD0
    u8 m_NpcPositionNum;                                 // 0x6B48
    u8 _6B49[3];
    Actor* m_pTownPlaza;                                 // 0x6B4C
    u8 m_Unknown6B50;                                    // 0x6B50
    bool m_Unknown6B51;                                  // 0x6B51 (Calc calls Func6E0A74)
    u8 _6B52[2];
    s32 m_Unknown6B54;                                   // 0x6B54
    s32 m_Unknown6B58;                                   // 0x6B58
    u8 m_Unknown6B5C;                                    // 0x6B5C
    bool m_Unknown6B5D;                                  // 0x6B5D
    u8 _6B5E[2];
    Bridge m_Bridges[BRIDGE_NUM];                        // 0x6B60
    Actor* m_pUnknown6B90;                               // 0x6B90 (the structure placed last by Func6E0A74)
    Actor* m_pUnknown6B94;                               // 0x6B94 (AutoCamp: the actor of the structure kind 201)
};
ASSERT_OFFSET(BsStrcMgr, m_CommonResMgr, 0x1C);
ASSERT_OFFSET(BsStrcMgr, m_CommonKeys, 0xEB4);
ASSERT_OFFSET(BsStrcMgr, m_FobjAllocator, 0xF1C);
ASSERT_OFFSET(BsStrcMgr, m_FobjResMgr, 0xF24);
ASSERT_OFFSET(BsStrcMgr, m_FenceResMgr, 0x5658);
ASSERT_OFFSET(BsStrcMgr, m_FenceAllocator, 0x6728);
ASSERT_OFFSET(BsStrcMgr, m_DrawLists, 0x6730);
ASSERT_OFFSET(BsStrcMgr, m_PlayerHouses, 0x6758);
ASSERT_OFFSET(BsStrcMgr, m_Strcs, 0x67CC);
ASSERT_OFFSET(BsStrcMgr, m_Unknown68C0, 0x68C0);
ASSERT_OFFSET(BsStrcMgr, m_Unknown6A6C, 0x6A6C);
ASSERT_OFFSET(BsStrcMgr, m_Hour, 0x6A8C);
ASSERT_OFFSET(BsStrcMgr, m_DayNight, 0x6AAC);
ASSERT_OFFSET(BsStrcMgr, m_IsNight, 0x6ACC);
ASSERT_OFFSET(BsStrcMgr, m_NpcPositions, 0x6AD0);
ASSERT_OFFSET(BsStrcMgr, m_pTownPlaza, 0x6B4C);
ASSERT_OFFSET(BsStrcMgr, m_Bridges, 0x6B60);
ASSERT_SIZE(BsStrcMgr, 0x6B98);
