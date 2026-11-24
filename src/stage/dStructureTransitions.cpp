#include "stage/stage_Extern.h"

#include "math/dAngle16.h"
#include "netgame/dGameFunctions.h"
#include "stage/dTransitionMgr.h"

// Two transitions out to a building of the town (between BsFtrMgr and BsMenuBg in the binary; called through the copies
// 0x0030C368/0x0030C36C, names are ours).

// 0x00695138
void TransitionToTownHall()
{
    // 0x00950378 (guard 0x00950370)
    static const math::Angle16Vec3 s_Rotation = {0, 0, 0};
    nn::math::VEC3_ position;
    GetStructurePosition(&position, static_cast<u32>(GetTownHallStructure()));
    position.z -= 16.0f;
    stage::GetTransitionMgr()->TransitionTo(stage::NAME_VILLAGE, &position, &s_Rotation, PLAYER_ACTION_HIDE, 0, 0, 1, 1);
}

// 0x006951F4
void TransitionToPostOffice()
{
    // 0x0095037E (guard 0x00950374)
    static const math::Angle16Vec3 s_Rotation = {0, 0, 0};
    nn::math::VEC3_ position;
    GetStructurePosition(&position, static_cast<u32>(STRUCTURE_POST_OFFICE));
    position.x -= 24.0f;
    stage::GetTransitionMgr()->TransitionTo(stage::NAME_DOWNTOWN, &position, &s_Rotation, PLAYER_ACTION_HIDE, 0, 0, 1, 1);
}
