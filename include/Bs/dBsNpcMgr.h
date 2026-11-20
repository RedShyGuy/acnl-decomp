#pragma once

#include "decomp.h"
#include "bgcheck/dGuardBox.h"
#include "field/dFieldRect.h"
#include "g3d/dResourceLoader.h"
#include "nn/math/math_Vector3.h"
#include "Npc/dNpcBankTables.h"
#include "Other/dBase.h"
#include "Other/dBitFlag8.h"
#include "Other/dHeapInit.h"
#include "ssys/ma/dHeapAllocator.h"
#include "stage/stage_Api.h"

class AcNpc;
class Actor;
struct BitSet32;

// The manager of the NPCs of a stage: the banks and the common resources of their models, the list of the NPC actors,
// the special NPCs of the stage definitions and of the modules, and the broadcasts of events to all NPCs (the functor
// classes NpcOnBuildFunc etc. call one slot of each NPC). Class names after the RTTI, m_Npcs and s_pInstance after
// libgarden, the rest is ours.
// RTTI 8BsNpcMgr @ 0x008CD4A4
// vtable 0x008F9A08 (vptr 0x008F9A10), offset_to_top 0, 16 entries
class BsNpcMgr : public ::Base
{
public:
    class NpcForeachFunction;
    class NpcForeachWithIndexFunction;
    class NpcPosGetCB;
    class SearchFunction;

    // a list in a buffer that the owner hands over (no growth; names are ours, first/last after libgarden NpcList)
    template <typename T>
    struct Vector
    {
        Vector() : first(NULL), last(NULL), capacity(0) {}
        Vector(T* buffer, u32 num) : first(buffer), last(buffer), capacity(num) {}

        u32 GetSize() const { return last - first; }
        bool IsFull() const { return GetSize() >= capacity; }
        void Clear() { last = first; }
        // (no test of the capacity)
        void PushBackFast(const T& value)
        {
            new (last) T(value);
            last++;
        }

        T* first;      // 0x0
        T* last;       // 0x4
        u32 capacity;  // 0x8 in elements
    };

    // a vector with its buffer behind it
    template <typename T, u32 N>
    struct InlineVector : public Vector<T>
    {
        InlineVector() : Vector<T>(m_Buffer, N) {}

        T m_Buffer[N];
    };

    // what BsNpcMgr loads; m_LoadFlags has one bit per item
    enum LoadFlag
    {
        LOAD_SETUP = 0,      // the first step of Initialize is done (heaps, villagers, guards)
        LOAD_CMN = 1,        // the common resources of the villagers ("Npc/Normal/CMN.bcres") are loaded and set up
        LOAD_PKN = 2,        // the model "pkn" (Halloween)
        LOAD_MKA = 3,        // the model "mka" (April Fools)
        LOAD_ACCESSORY = 4,  // the accessories of the season (LoadAccessory)
        LOAD_NPCS = 5,       // the NPCs of the stage are created
        LOAD_GUEST = 6,      // the model of a guest (LoadGuestModel)
        LOAD_GUEST_SEO = 7,  // ... and it is the model "seo"
    };

    // an object whose level adds to m_Level (the type is not known yet)
    struct LevelSource
    {
        u8 _0[0xC];
        nn::math::VEC3_ direction; // 0x0C
        f32 level;                 // 0x18
    };

    // an actor and its unit (m_ActorUnits)
    struct ActorUnit
    {
        Actor* actor; // 0x0
        s32 x;        // 0x4
        s32 z;        // 0x8
    };

    // an NPC whose names are words of the speech recognition (the word list g_AsrWordList; names are ours): the ids of its
    // words and the kind of the entry (s_AsrSpNpcs; meaning of the kinds unknown)
    struct AsrNpc
    {
        AcNpc* actor; // 0x0
        u16 from;     // 0x4 the first word
        u16 to;       // 0x6 the last word
        u32 kind;     // 0x8
    };
    // a special character in the word list: its model, the index of its names in STR_SPNpc_name_ASR, the kind of the
    // entry (3: only with isAlways) and whether it is added without being in the stage
    struct AsrSpNpc
    {
        const char* model; // 0x0
        u32 textIndex;     // 0x4
        u8 kind;           // 0x8
        bool isAlways;     // 0x9
    };
    static const s32 ASR_SP_NPC_NUM = 42;

    // a placement method of the walking villagers: the position (out), the units of the other villagers, their number
    typedef bool (BsNpcMgr::*PlaceMethod)(nn::math::VEC3* position, s8 (*units)[2], s32 num);

    static const u32 DAILY_ORDER_NUM = 4;

    // the accessory resources of the season (m_AccessoryKind; NONE: nothing loaded)
    static const u16 ACCESSORY_NONE = 3;

    BsNpcMgr(); // 0x006A27F0
    virtual ~BsNpcMgr(); // 0x0011817C slot 0x00 (Thumb)
    // 0x00118170 (deleting dtor)
    virtual oml::framework::Result Initialize(); // 0x00117C3C slot 0x0C (Thumb)
    virtual oml::framework::Result Finalize(); // 0x00100684 slot 0x18 (Thumb)
    virtual oml::framework::Result Calc(); // 0x006A26E8 slot 0x24
    virtual oml::framework::Result Draw(); // 0x006A266C slot 0x30

    // the number of NPCs of the list
    u32 GetNpcNum() const { return m_Npcs.GetSize(); }
    // calls func for every NPC of the list (inline; the copy lies in the shared code)
    DECOMP_EMIT_COPY DECOMP_NOINLINE inline void Foreach(NpcForeachFunction& func); // 0x00317044
    DECOMP_ALWAYS_INLINE void ForeachImpl(NpcForeachFunction& func);

    // the first NPC with the info (NULL: none)
    AcNpc* FindNpc(const stage::NpcInfo* info); // 0x006962E0
    // the first special NPC with the model (three letters)
    AcNpc* FindNpcByModel(const char* model); // 0x006969B0
    // the NPC of the list with the actor id: its slot 0xE8 with arg (0: no such NPC)
    s32 PassToNpc(u32 actorId, const void* arg); // 0x006A2778
    // the first NPC of the kind (byte 0x95E): its slot 0xEC with arg (false: no such NPC)
    bool PassToNpcOfKind(u8 kind, const void* arg); // 0x006963CC
    // removes npc from the list (its place stays empty)
    void RemoveNpc(AcNpc* npc); // 0x00696274

    // the broadcasts to all NPCs (and to the snowballs): something is built / previewed on the units of the rectangle
    // x0..x1, z0..z1
    void OnBuild(s32 x0, s32 z0, s32 x1, s32 z1, u8 flag); // 0x00696124
    void OnPreview(s32 x0, s32 z0, s32 x1, s32 z1, u8 flag); // 0x00696438
    void OnPreviewEnd(); // 0x00697BCC

    // creates the special NPCs of a stage (command 3 of a stage definition; a name of PROC_NAME_NUM keeps a place empty);
    // false: the banks for them could not be created
    bool CreateSpecialNpcs(const stage::Command* command); // 0x006967BC
    // queues a special NPC at the center of the unit x, z, turned by angleY, for CreateQueuedNpcs (false: the queue is
    // full)
    bool QueueNpc(u16 name, const stage::NpcInfo* info, u16 angleY, s32 x, s32 z); // 0x006A2590
    // swaps two queued NPCs
    void SwapQueuedNpcs(s32 a, s32 b); // 0x00696CA8
    // lets the module of the stage queue its NPCs and creates the queued ones (false: none)
    bool CreateQueuedNpcs(); // 0x00696D04

    // the player of the town has unlocked Kapp'n (the host in online play)
    static bool IsKappnUnlocked(); // 0x00696358
    // the level of source is added (the dot product of its direction and the direction of the guard index with index);
    // true: above 12
    bool AddLevel(const LevelSource* source); // 0x0069639C
    bool AddLevel(const LevelSource* source, s32 index); // 0x00698AA4
    // the guard boxes touch the group 1 too (false: there is an owner already)
    bool SetGuardOwner(AcNpc* owner); // 0x00696A88
    // the three villagers who walk in the town get random positions (the methods by weight)
    void PlaceWalkingVillagers(); // 0x006964F4
    // a new position for the walking villager index
    void PlaceWalkingVillager(nn::math::VEC3* out, s32 index); // 0x00698B08
    DECOMP_ALWAYS_INLINE bool PlaceByMethods(nn::math::VEC3* position, s8 (*units)[2], s32 num, const u32* defaultWeights,
                                             const PlaceMethod* methods);
    // the placement methods (see PlaceMethod)
    bool PlaceMethod0(nn::math::VEC3* position, s8 (*units)[2], s32 num); // 0x0069B324
    bool PlaceMethod1(nn::math::VEC3* position, s8 (*units)[2], s32 num); // 0x00698D44
    bool PlaceMethod2(nn::math::VEC3* position, s8 (*units)[2], s32 num); // 0x0069B104
    // the model of a guest of the house ("Npc/Normal/Model/%s.bcres", the special ones after Func20601C); false: still
    // loading
    bool LoadGuestModel(const char* model, bool isSpecial); // 0x00696F78
    void UnloadGuestModel(); // 0x006989D8
    // m_DailyOrder: 1..4 in a random order of the day
    void PickDailyOrder(); // 0x006973C4
    // the entry of m_DailyOrder for an index (taken modulo 4)
    DECOMP_EMIT_COPY u8 GetDailyOrder(s32 index) const // 0x00766FFC
    {
        const s32 i = index % static_cast<s32>(DAILY_ORDER_NUM);
        return m_DailyOrder[static_cast<u32>(i) < DAILY_ORDER_NUM ? i : 0];
    }
    // the player has the lamp (kind 149) somewhere: the town, pockets, island box, dressers, house, own fields
    static bool HasYutaroLamp(); // 0x00697454
    // (false: never; the actor is not added twice)
    bool AddActorUnit(Actor* actor); // 0x00697B0C
    bool RemoveActorUnit(Actor* actor); // 0x00698960
    // (only ModuleIndoor; meaning unknown) not online, no special character 0x2000/0x2010 in the right camping car, the
    // player has a name and finished the first day, and flag 65 of SvPlayerUnk8F77 is not set
    static bool Func697C40(); // 0x00697C40
    // a value (71, 73, 76 or slot 0x9C of an NPC) for a value in the range of one of m_AsrNpcs
    u32 Func698894(u16 value); // 0x00698894
    // the unit x, z is at least 25 units (x + z) from each of the units
    static bool IsFarFromUnits(const s8 (*units)[2], s32 num, s32 x, s32 z); // 0x006990C8
    // the value 0x938 of the first NPC of the kind (false: none)
    bool SetValueOfKind(u8 kind, u32 value); // 0x00699064
    // (meaning unknown) not online, the flag 0x12.6 of the player is not set and 0x13.5 is
    static bool Func6991AC(); // 0x006991AC
    // the lost child (AcNpcSpMaiko, model "lom") if the save says so (false: the banks failed)
    bool CreateMaiko(); // 0x00699200
    // a unit of the block of the house of the player that lies outside of the rectangle (its center into out; the
    // global 0x0095488C remembers it); false: none
    static bool FindUnitOutside(nn::math::VEC3* out, s32 minX, s32 minZ, s32 maxX, s32 maxZ); // 0x00699318
    // the end of an emoticon of the player (only player 1 offline: the NPCs get 0)
    void OnPlayerEndEmoticon(u32 emoticon, s32 player); // 0x006994B8
    // (meaning unknown; the system 0x0051Dxxx)
    static void Func699540(); // 0x00699540
    // (meaning unknown) bits 0 and 1 of SvTownData::unk_0x1DAC7 and April Fools
    static bool Func699598(); // 0x00699598
    // the banks for the NPCs of a stage command (the villagers count as kind 0, the special characters as kind 1;
    // CreateSpecialNpcs inlines it); false: no banks
    bool PrepareBanks(const stage::Command* command); // 0x00699650
    DECOMP_ALWAYS_INLINE bool PrepareBanksImpl(const stage::Command* command);
    // the guard boxes of the town: in front of the train station, the second one where Func6FA888 says, two through a
    // module
    void SetupVillageGuards(); // 0x0069976C
    // (meaning unknown) two bytes of netgame::g_pUnknown954884 become stage::NAME_MAX
    static void Func69993C(); // 0x0069993C
    // the villagers who walk in the town (at most five, three in the Dream Suite): their positions of the last time,
    // a random unit, the countdown of the new year, else in front of their houses (false: no banks)
    bool CreateWalkingVillagers(); // 0x0069995C
    // the same in other situations (s_State.unk0E / unk0F; meanings unknown)
    bool CreateWalkingVillagersE(); // 0x0069EB60
    bool CreateWalkingVillagersF(); // 0x0069D368
    // an NPC as a child of the manager
    oml::framework::Process* CreateNpc(ProcName name, u32 arg, const nn::math::VEC3_* position,
                                       const math::Angle16Vec3* rotation); // 0x0069A230
    // an NPC stands on the unit of position (Thumb)
    bool IsNpcAt(const nn::math::VEC3* position); // 0x0011782C (Thumb)
    // the NPCs of the current stage by its kind (the modules, the shops, the village, the houses ...; false: not all of
    // them could be created)
    bool CreateStageNpcs(); // 0x0011788C (Thumb)
    // (meaning unknown) in a player house Func1E29AC on the flags of the player
    static void Func117AD8(); // 0x00117AD8 (Thumb)
    // (in a dream town) the resident who owns the player house stands in its main room as AcNpcPlayerGhost, on a free
    // unit found by a road search (not when the bit of the house in VisitorLocalInfo::GetByte42 is set; meaning open)
    bool CreatePlayerGhostInHouse(); // 0x00117B00 (Thumb)
    // an actor of the process class with arg as a child, added to m_Npcs (the other places have it inline)
    void CreateNpc(ProcName name, u32 arg, const nn::math::VEC3* position, const math::Angle16Vec3* rotation); // 0x0069605C
    // the end of a fall into a pitfall / the start of an emoticon of the player (only player 1 offline)
    void OnFallToPitfall(); // 0x0069A254
    void OnPlayerStartEmoticon(u8 emoticon, s32 player); // 0x0069A2C4
    // the villagers who come along into the new stage (by the stages before and now)
    void SetupStageVillagers(); // 0x0069A34C
    // (the cases of SetupStageVillagers; meanings unknown yet)
    bool Func69ED50(BitSet32* set, bool isControlled); // 0x0069ED50
    void Func6A0F9C(BitSet32* set); // 0x006A0F9C
    void Func6A2314(BitSet32* set); // 0x006A2314
    void Func6A1F94(BitSet32* set); // 0x006A1F94
    void Func6A0C98(BitSet32* set); // 0x006A0C98
    void Func6A10D8(BitSet32* set); // 0x006A10D8
    void Func6A19A4(BitSet32* set); // 0x006A19A4
    void Func6A0964(BitSet32* set); // 0x006A0964
    // the map of the ways between the quarters of the blocks of the town (g_WalkMap)
    static void BuildWalkMap(); // 0x0069A674
    // the doors of the houses follow their villagers (one villager per frame) while this console controls the stage
    void CalcStageControl(); // 0x0069ACEC
    // the special character of an event or a day of today (s_EventNpcs; false: none)
    bool CreateEventNpc(); // 0x0069AECC
    // the broadcasts of the pitfalls (offline)
    void OnMoveOutPitfall(); // 0x0069B094
    void OnStrugglePitfall(); // 0x0069B2B4
    // the bounds of the free floor of the room (BuildRoomFloor)
    static void GetRoomBounds(s32* minX, s32* minZ, s32* maxX, s32* maxZ); // 0x0069B11C
    // an NPC at the center of the unit x, z turned by angleY
    void CreateNpcAt(ProcName name, u32 arg, u16 angleY, s32 x, s32 z); // 0x0069B148
    // the queued NPCs are created, the queue stays (false: none)
    bool CreateQueuedNpcsKeep(); // 0x0069B1CC
    // the first of the first ten NPCs of the process class with the value 0x1030
    AcNpc* FindNpcByName(u32 value, u16 name); // 0x0069B23C
    // (only ModuleOutdoor) the units of the area of g_Unknown994484 from dx / dz on are marked (Func7CFF58)
    static void Func69B33C(u32 dx, u32 dz); // 0x0069B33C
    // the first NPC of the kind: its slot 0xF0 (0: none)
    s32 PassToNpcOfKindF0(u8 kind); // 0x0069B3F4
    // the slot 0xC8 of the owner of the guards (offline; 0: none)
    s32 PassToGuardOwner(u32 a, u32 b); // 0x0069B44C
    // the free units of the floor of the room (height 0; g_RoomFloor, one bit per unit) and their bounds
    static void BuildRoomFloor(); // 0x0069B49C
    // "END" is appended to the word list, its line breaks become ends of strings and s_AsrWordListSize its size
    static void FinishAsrWordList(); // 0x0069B60C
    // the lines of text are appended to the word list as "<id>:<word>" (ids counted by s_AsrWordId); the ids of the first
    // and the last word (0xFFFF and false without text)
    static bool AddAsrWords(u16* first, u16* last, const char* text); // 0x006A1CB0
    // the word list of the speech recognition: the names of the villagers and of the special characters (in the Reset
    // Centre the two answers of STR_Reset_ASR)
    bool SetupAsrWords(); // 0x0069F958
    // the heaps of all banks are destroyed (true: all of them; then the NPCs are forgotten)
    bool FinalizeBanks(); // 0x0069F7B4
    // AcNpcSpCleaningVisit comes (a flag of the town after the Weeding Day; the flag is cleared when the host lacks the
    // flags of the player for it)
    static bool HasCleaningVisit(); // 0x0069FD44
    // an event secretary is in the stage (EventSecretarySearchFunc; the static version is false without the manager)
    static bool SIsEventSecretaryPresent(); // 0x0069FE20
    DECOMP_NOINLINE bool IsEventSecretaryPresent(); // 0x0069FE30
    // the villagers whose state belongs to the current stage (a shop, the club, a room of the museum) as AcNpcDt
    void CreateStageVillagers(); // 0x0069FEA8
    // (with bankNum banks of villagers) the NPCs of Func51D990 in the shops, the club and the museum rooms as AcNpcDt,
    // then CreateStageVillagers (false: no banks)
    bool CreateShopNpcs(u32 bankNum); // 0x00317158
    // (with the banks for two NPCs) the secretary of the anniversary beside the unit of Func6FB2BC, a special character
    // (s_AnniversaryModels) and up to two villagers (false: no banks)
    bool CreateAnniversaryNpcs(); // 0x006A0194
    // the player is in one of the rectangles m_UnknownF0F4 (12 more on each side)
    bool IsPlayerInUnknownRects(); // 0x006A082C
    // the secretary (AcNpcSpSecretaryOut) in front of the station (not in the screen stages)
    void CreateSecretaryAtStation(); // 0x006A1320
    // (with the banks for two NPCs) Mr. Resetti and/or Don Resetti in the Reset Centre, by the value 0x6.1 of the player
    // flags (from 4 on at random); s_State.unk12 is set when both come (false: no banks or no player)
    bool CreateResetCenterNpcs(); // 0x006A1470
    // (with the banks for two NPCs) the special character who sits in the cafe today (AcNpcSpCafeTalk, the first one
    // of s_CafeGuests for which Func720B4C holds; the same one again after the part-time job)
    bool CreateCafeGuest(); // 0x00697820
    // the NPCs of the village: Kapp'n or Tortimer at the dock, the secretary (at the station, at a ceremony, at an event,
    // for the public works) or AcNpcSpRakosuke, then the requested/event/special-day/visitor NPCs
    bool CreateVillageNpcs(); // 0x00697CD0
    // April Fools: one of the villagers is chosen (at least three; the smallest value of Func119FFC); false: not today
    static bool PickAprilFoolsVillager(); // 0x0069B71C
    // (only ModulePoliceBox and 0x0064AD48; meaning unknown) offline, a test of the date, from 6:00 (Sundays 12:00), and
    // a test of the badges of the player
    static bool Func69B99C(); // 0x0069B99C
    // (meaning unknown) the structure 4 lies in the rectangle of Func6FBF80
    static bool Func69BA70(); // 0x0069BA70
    // the characters of the first days: Isabelle at the tree, Tom Nook at the places for the house, Isabelle at the
    // station
    void CreatePrologueNpcs(); // 0x0069BB84
    // Resetti (model "mob" after a value of the player, else "mol")
    void CreateResetsan(); // 0x0069BEC8
    // the special character that s_State requests (in front of the structure of Func106314), or the villager who asked
    // for a visit; the requests are cleared (false: nothing)
    bool CreateRequestedNpc(); // 0x0069C078
    // the visiting special character of today (s_VisitorNpcs; false: none)
    bool CreateVisitorNpc(); // 0x0069C764
    // the special character of a special day (the first one whose conditions hold; false: none)
    bool CreateSpecialDayNpc(); // 0x0069C944
    // the first NPC of the kind: its function 0x0057A600 (0: none)
    s32 PassToNpcOfKind57A600(u8 kind, u32 arg); // 0x0069D290
    // the same for AcNpc::Func57A8FC (false: none)
    bool PassToNpcOfKind57A8FC(u8 kind); // 0x006A22C4
    // two of the file globals are reset
    static void ClearState2(); // 0x0069D2F4
    // the time of AcNpcSpPumpkingPre (Func3189EC 14 and the flags of the player)
    static bool IsPumpkingPreTime(); // 0x0069D310
    // queues a special NPC at a position (false: the queue is full)
    bool QueueNpc(u16 name, const stage::NpcInfo* info, const nn::math::VEC3_* position,
                  const math::Angle16Vec3* rotation); // 0x0069EAC0
    // (only ModuleOutdoor and ModuleSummer) the house of key in g_Unknown9516C0 gets on (and !on unless keep)
    static void Func698A38(u32 key, bool on, bool keep); // 0x00698A38

    // the accessory resources of the season or an event (Initialize)
    void SetupAccessory(); // 0x00696B3C
    // loads the accessory resources of kind (false: still loading)
    bool LoadAccessory(u8 kind); // 0x00696DEC

    // the state of the file globals is reset
    static void ClearState(); // 0x006A2674

    // the bank tables (Thumb ctors)
    NpcNmlBankTables m_NmlBanks;                       // 0x00014
    NpcSpBankTables m_SpBanks;                         // 0x072F0
    // the resource files and their heaps
    g3d::ResourceLoader m_CmnLoader;                   // 0x0E5CC "Npc/Normal/CMN.bcres"
    ssys::ma::HeapAllocator m_CmnAllocator;            // 0x0E6D4
    g3d::ResourceLoader m_PknLoader;                   // 0x0E6DC "Npc/Special/Model/pkn.bcres"
    ssys::ma::HeapAllocator m_PknAllocator;            // 0x0E7E4
    g3d::ResourceLoader m_MkaLoader;                   // 0x0E7EC "Npc/Special/Model/mka.bcres"
    ssys::ma::HeapAllocator m_MkaAllocator;            // 0x0E8F4
    g3d::ResourceLoader m_GstLoader;                   // 0x0E8FC (the guests: player houses and online play)
    ssys::ma::HeapAllocator m_GstAllocator;            // 0x0EA04
    g3d::ResourceLoader m_AccessoryLoader;             // 0x0EA0C
    ssys::ma::HeapAllocator m_AccessoryAllocator;      // 0x0EB14
    AcNpc* m_NpcBuffer[14];                            // 0x0EB1C (10 used, 14 in the amiibo camera)
    Vector<AcNpc*> m_Npcs;                             // 0x0EB54
    InlineVector<stage::NpcEntry, 10> m_QueuedNpcs;    // 0x0EB60
    InlineVector<ActorUnit, 10> m_ActorUnits;          // 0x0ECAC
    InlineVector<AsrNpc, 50> m_AsrNpcs;       // 0x0ED30
    bgcheck::GuardBox m_GuardBoxA;                     // 0x0EF94
    bgcheck::GuardBox m_GuardBoxB;                     // 0x0EFE0
    bgcheck::GuardBox m_GuardBoxes[2];                 // 0x0F02C
    nn::math::VEC3 m_GuardDirections[2];               // 0x0F0C4
    nn::math::VEC3 m_UnknownF0DC[2];                   // 0x0F0DC
    FieldRect m_UnknownF0F4[2];                        // 0x0F0F4
    f32 m_Level;                                       // 0x0F114 decays by 10% per frame (meaning unknown)
    u32 m_Timer;                                       // 0x0F118 counts down to 0
    s32 m_UnknownF11C;                                 // 0x0F11C
    u32 m_Frame;                                       // 0x0F120 counts 0..9 while the stage is controlled here
    u8 m_UnknownF124;                                  // 0x0F124
    u8 m_UnknownF125;                                  // 0x0F125
    BitFlag8 m_LoadFlags;                              // 0x0F126 LoadFlag
    u8 m_DailyOrder[DAILY_ORDER_NUM];                  // 0x0F127
    u8 _F12B;
    u16 m_AccessoryKind;                               // 0x0F12C
    u16 m_UnknownF12E;                                 // 0x0F12E
    AcNpc* m_GuardOwner;                               // 0x0F130

    // a special character of an event: the event or day (kind 0: Func309C6C on the date, 1: event::IsActive), its
    // process, its model and its placement function (s_PlaceFuncs)
    struct EventNpc
    {
        u8 eventOrDay;    // 0x0
        u8 _1;
        u16 name;         // 0x2 ProcName
        u8 kind;          // 0x4
        const char* model; // 0x8
        u8 place;         // 0xC
    };
    static const u32 EVENT_NPC_NUM = 10;
    // a special character that visits the town (event::Func58F754 offline, event::Func58EE68 online if allowed)
    struct VisitorNpc
    {
        u8 visitor;        // 0x0
        u8 _1;
        u16 name;          // 0x2 ProcName
        const char* model; // 0x4
        u8 place;          // 0x8 s_PlaceFuncs
        bool isOnline;     // 0x9
    };
    static const u32 VISITOR_NUM = 5;
    // a position and rotation for a special character (false: none; the functions are game code)
    typedef bool (*PlaceFunc)(nn::math::VEC3* position, math::Angle16Vec3* rotation, stage::NpcInfo* info);

    static f32 s_Unknown9511A4;
    static const PlaceFunc s_PlaceFuncs[8];
    static VisitorNpc s_VisitorNpcs[VISITOR_NUM];
    static EventNpc s_EventNpcs[EVENT_NPC_NUM];
    static u32 s_Unknown951168;
    static s32 s_CafeGuest;
    static s32 s_AsrWordListSize;
    static s32 s_AsrWordId;
    static s32 s_AsrResetWordNum[2];
    static AsrSpNpc s_AsrSpNpcs[ASR_SP_NPC_NUM];
    static const char* s_AnniversaryModels[6];
    static const char* s_ResetCenterModels[7];
    static const char* s_ResetCenterModels2[3];
    static s32 s_RoomMinX;
    static s32 s_RoomMinZ;
    static s32 s_RoomMaxX;
    static s32 s_RoomMaxZ;

    // the bytes the NPCs share (cleared by ClearState; meanings unknown)
    struct State
    {
        u8 unk00; // 0x00 (stage::NAME_MAX: none)
        s8 unk01; // 0x01 requests Resetti
        u8 unk02;
        u8 unk03;
        u8 unk04;
        u8 unk05;
        u8 unk06;
        u8 unk07;
        u8 unk08;
        u8 unk09;
        u8 unk0A;
        u8 unk0B;
        u8 unk0C;
        u8 unk0D; // 0x0D (0xFC: none)
        u8 unk0E;
        u8 unk0F;
        u8 unk10; // 0x10 cleared in each frame
        u8 unk11;
        u8 unk12;
        u8 unk13;
        u8 unk14;
        u8 unk15;
        u8 unk16;
        u8 unk17;
    };

    static State s_State;
    static BsNpcMgr* s_pInstance;
    static u32 s_Unknown95115C;
    // a counter that runs from 0 to the period (Calc)
    static u32 s_CounterPeriod;
    static u32 s_Counter;
    static u32 s_Unknown951180;
};
ASSERT_OFFSET(BsNpcMgr, m_SpBanks, 0x72F0);
ASSERT_OFFSET(BsNpcMgr, m_CmnLoader, 0xE5CC);
ASSERT_OFFSET(BsNpcMgr, m_NpcBuffer, 0xEB1C);
ASSERT_OFFSET(BsNpcMgr, m_QueuedNpcs, 0xEB60);
ASSERT_OFFSET(BsNpcMgr, m_ActorUnits, 0xECAC);
ASSERT_OFFSET(BsNpcMgr, m_AsrNpcs, 0xED30);
ASSERT_OFFSET(BsNpcMgr, m_GuardBoxA, 0xEF94);
ASSERT_OFFSET(BsNpcMgr, m_UnknownF0F4, 0xF0F4);
ASSERT_OFFSET(BsNpcMgr, m_LoadFlags, 0xF126);
ASSERT_SIZE(BsNpcMgr, 0xF134);

// the functions all NPCs are called with (the derived functors are classes of the users)
// RTTI N8BsNpcMgr18NpcForeachFunctionE @ 0x008D3FF4
class BsNpcMgr::NpcForeachFunction
{
public:
    NpcForeachFunction() {}
    virtual void Invoke(AcNpc* npc) = 0; // slot 0x00
};

inline void BsNpcMgr::ForeachImpl(NpcForeachFunction& func)
{
    for (AcNpc** it = m_Npcs.first; it != m_Npcs.last; it++) {
        if (*it != NULL) {
            func.Invoke(*it);
        }
    }
}

inline void BsNpcMgr::Foreach(NpcForeachFunction& func)
{
    ForeachImpl(func);
}

inline bool BsNpcMgr::PrepareBanksImpl(const stage::Command* command)
{
    if (command == NULL) {
        return false;
    }
    const stage::NpcEntry* entries = static_cast<const stage::NpcEntry*>(command->data);
    u32 nmlNum = 0;
    u32 spNum = 0;
    for (s32 i = 0; i < command->count; i++) {
        const u8 kind = entries[i].info.kind;
        if (kind == stage::NpcInfo::KIND_ID) {
            nmlNum++;
        } else if (kind == stage::NpcInfo::KIND_MODEL) {
            spNum++;
        }
    }
    // the train station shows one more special character, Puzzle League two and a villager
    const stage::Name current = stage::GetCurrent();
    if (stage::IsKind(stage::KIND_TRAIN_STATION_ENVIRONMENT, stage::GetCurrent()) &&
        !stage::IsKind(stage::KIND_SCREEN, stage::GetCurrent())) {
        spNum++;
    }
    if (current == stage::NAME_MINI_GAME_1) {
        spNum += 2;
        nmlNum++;
    }
    if (m_NmlBanks.m_Num < nmlNum && !m_NmlBanks.Create(HeapInit::s_pStageHeap, nmlNum)) {
        return false;
    }
    if (m_SpBanks.m_Num < spNum && !m_SpBanks.Create(HeapInit::s_pStageHeap, spNum)) {
        return false;
    }
    return true;
}
