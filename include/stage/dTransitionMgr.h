#pragma once

#include "decomp.h"
#include "stage/stage_Api.h"

namespace stage {
// a place to come back to: the stage and the position of the player (name is ours)
struct ReturnPoint
{
    ReturnPoint()
    {
        Clear();
    }
    DECOMP_EMIT_COPY void Clear() // 0x005B4C18
    {
        stage = NAME_NONE;
        action = 0;
        isFromPlayer = false;
    }

    Name stage;                     // 0x00 NAME_NONE: none
    nn::math::VEC3_ position;       // 0x04
    math::Angle16Vec3 rotation;     // 0x10
    u8 _pad16[2];
    u32 action;                     // 0x18 the player action on arrival
    bool isFromPlayer;              // 0x1C the position of the player (no stored return position was found)
};
ASSERT_SIZE(ReturnPoint, 0x20);

// the place in front of an entrance of the current stage (name is ours)
struct ExitPoint
{
    u8 structure;                   // 0x00
    nn::math::VEC3_ position;       // 0x04
    math::Angle16Vec3 rotation;     // 0x10
};

// The transitions between the stages: the next stage with the position and action of the player there, the return
// position, and the processes that hold back a transition (class, Transition and SetReturnParam after libgarden; the
// other names are ours). The typeA/faderA/typeB/faderB arguments are the fades of the scene change.
class TransitionMgr
{
public:
    TransitionMgr()
        : m_Entrance()
        , m_IsWithoutPosition(false)
        , m_IsToStructure(false)
        , m_Action(0)
        , m_HandleCount(0)
        , m_Structure(0xFC)
    {
        for (s32 i = 0; i < HANDLE_MAX; i++) {
            m_Handles[i] = -1;
        }
    }

    // starts the scene change to the stage; false while another transition runs
    DECOMP_NOINLINE bool Transition(Name stage, u8 typeA, u8 faderA, u8 typeB, u8 faderB); // 0x005B5864 | libgarden [tier A]
    // stores the position of the player (or the entrance it came from) as the return point
    void SetReturnParam(); // 0x005B64AC | libgarden [tier A]

    void AddHandle(s32 handle); // 0x005B57D0
    void RemoveHandle(s32 handle); // 0x005B5998
    bool TransitionToReturnPoint(u8 typeA, u8 faderA, u8 typeB, u8 faderB); // 0x005B57EC
    void SetReturnPoint(const nn::math::VEC3_& position, u32 action, u16 rotationY); // 0x005B5A5C
    // the stage behind the entrance of the current stage (NAME_NONE: no such entrance)
    Name GetEntranceTarget(u32 index) const; // 0x005B5AC8
    void ClearReturnPoint(); // 0x005B5B50
    // the place a block in front of the entrance of the current stage
    bool GetExitPoint(ExitPoint* pPoint, u32 index) const; // 0x005B5B68
    bool TransitionByEntrance(u32 index, u8 typeA, u8 faderA, u8 typeB, u8 faderB); // 0x005B5D14
    // out to the stage of the structure, in front of it (offset added)
    DECOMP_NOINLINE bool TransitionToStructure(u8 structure, const nn::math::VEC3_* offset, const math::Angle16Vec3* rotation, u8 typeA,
                               u8 faderA, u8 typeB, u8 faderB); // 0x005B5D58
    // out of the building of the stage (NAME_NONE: the current one) in front of its structure
    DECOMP_NOINLINE bool TransitionFromStructure(u8 typeA, u8 faderA, u8 typeB, u8 faderB, Name stage); // 0x005B6054
    DECOMP_NOINLINE bool TransitionTo(Name stage, const nn::math::VEC3_* position, const math::Angle16Vec3* rotation, u32 action, u8 typeA,
                      u8 faderA, u8 typeB, u8 faderB); // 0x005B6660
    // out in front of the house of a village player
    DECOMP_NOINLINE bool TransitionToPlayerHouse(u8 owner, u8 typeA, u8 faderA, u8 typeB, u8 faderB); // 0x005B66FC
    // the stage with the start positions of the four players (the own player starts at its position)
    bool TransitionWithStartPositions(Name stage, u32 action, u32 startAction, const nn::math::VEC3_* position0,
                                      const math::Angle16Vec3* rotation0, const nn::math::VEC3_* position1,
                                      const math::Angle16Vec3* rotation1, const nn::math::VEC3_* position2,
                                      const math::Angle16Vec3* rotation2, const nn::math::VEC3_* position3,
                                      const math::Angle16Vec3* rotation3, u8 typeA, u8 faderA, u8 typeB,
                                      u8 faderB); // 0x005B6820
    DECOMP_NOINLINE bool TransitionWithStartPositions(Name stage, u32 hostAction, u32 startHostAction, u32 guestAction,
                                      u32 startGuestAction, const nn::math::VEC3_* position0,
                                      const math::Angle16Vec3* rotation0, const nn::math::VEC3_* position1,
                                      const math::Angle16Vec3* rotation1, const nn::math::VEC3_* position2,
                                      const math::Angle16Vec3* rotation2, const nn::math::VEC3_* position3,
                                      const math::Angle16Vec3* rotation3, u8 typeA, u8 faderA, u8 typeB,
                                      u8 faderB); // 0x005B6848
    // out of the hut of a tour back to the field of the tour
    DECOMP_NOINLINE bool TransitionFromTourHut(u8 typeA, u8 faderA, u8 typeB, u8 faderB); // 0x005B6944
    // to the return point, or without one into the own house; both reset the wait of the network game
    bool ReturnOrGoHome(u8 typeA, u8 faderA, u8 typeB, u8 faderB); // 0x005B6AA0
    // the same, but going home does not reset the wait
    bool ReturnOrGoHomeKeepWait(u8 typeA, u8 faderA, u8 typeB, u8 faderB); // 0x005B6BCC
    // out in front of the structure that was entered last
    DECOMP_NOINLINE bool TransitionToLastStructure(u8 typeA, u8 faderA, u8 typeB, u8 faderB); // 0x005B6D30
    DECOMP_NOINLINE bool TransitionByEntrance(Name stage, u32 index, u8 typeA, u8 faderA, u8 typeB, u8 faderB); // 0x005B6E60

    static const s32 HANDLE_MAX = 4;

    Entrance m_Entrance;            // 0x00 the entrance of the next stage
    ReturnPoint m_Return;           // 0x24
    bool m_IsWithoutPosition;       // 0x44 the player starts at the start of the stage definition
    bool m_IsToStructure;           // 0x45 the transition leaves a building to its structure
    u8 _pad46[2];
    nn::math::VEC3_ m_Position;     // 0x48 the start of the player in the next stage
    math::Angle16Vec3 m_Rotation;   // 0x54
    u8 _pad5A[2];
    u32 m_Action;                   // 0x5C
    s32 m_HandleCount;              // 0x60
    s32 m_Handles[HANDLE_MAX];      // 0x64 (-1: free)
    u8 m_Structure;                 // 0x74 the structure the transition leaves to (0xFC: none)
};
ASSERT_SIZE(TransitionMgr, 0x78);

// the transition manager and the default start in the own house (names are ours)
extern TransitionMgr s_TransitionMgr;
extern nn::math::VEC3 s_HouseStartPosition;
extern math::Angle16Vec3 s_ZeroRotation;

// libgarden (copies in the shared area)
DECOMP_EMIT_COPY inline TransitionMgr* GetTransitionMgr() // 0x00308154 | libgarden [tier A]
{
    return &s_TransitionMgr;
}
DECOMP_EMIT_COPY inline bool Transition(Name stage, u8 typeA, u8 faderA, u8 typeB, u8 faderB) // 0x00304A60 | libgarden [tier A]
{
    return s_TransitionMgr.Transition(stage, typeA, faderA, typeB, faderB);
}
// (name is ours) the transition into the first mini game; the first two arguments are unused
bool TransitionToMiniGame0(u32 arg0, u32 arg1, u8 type, u8 typeB, u8 faderB); // 0x005B6CF0
} // namespace stage
