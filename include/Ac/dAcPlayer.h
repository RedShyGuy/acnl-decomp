#pragma once

#include "decomp.h"
#include "Other/dDemoActor.h"
#include "Player/dPlayerModel.h"
#include "Player/dPlayerSimpleMessage.h"
#include "Utl/dUtlBase.h"

// RTTI 8AcPlayer @ 0x008CD434
// vtable 0x008F96D8 (vptr 0x008F96E0), offset_to_top 0, 36 entries
// vtable 0x008F9770 (vptr 0x008F9778), offset_to_top -104, 63 entries
// the entry of a player action in the table 0x00843C20 (235 entries of 10 bytes; names are ours)
struct PlayerActionInfo
{
    u8 _unknown0x0[9];
    u8 flags; // 0x9 (bit 6: see BsMenuMgr::CanOpenMenuThink)
};
ASSERT_SIZE(PlayerActionInfo, 10);

// the number of player actions (entries of the table)
const u32 PLAYER_ACTION_NUM = 235;

// the table entry of an action (NULL for an invalid action; name is ours)
const PlayerActionInfo* GetPlayerActionInfo(u8 action); // 0x0064FE68

class AcPlayer : public ::UtlBase<DemoActor>, public ::PlayerSimpleMessage
{
public:
    class ToolFunctor;
    class ActionPosition;
    AcPlayer(); // ctor candidate(s) 0x0068EC58 (unverified)
    virtual ~AcPlayer(); // 0x0068F210 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x0068F200 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void HandleInitializationResult(oml::framework::Result); // 0x001170A5 slot 0x10 | slot vf_0x10 of oml::framework::Process
    virtual bool CanCalc() const; // 0x0064D6FC slot 0x20 | slot vf_0x20 of oml::framework::Process
    virtual oml::framework::Result Calc(); // 0x0068DB7C slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual oml::framework::Result Draw(); // 0x0068D44C slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x40(); // 0x00650CFC slot 0x40 | virtual slot, introduced by Actor
    virtual void vf_0x5C(); // 0x007631D4 slot 0x5C | virtual slot, introduced by DemoActor
    bool StartInitialize() override; // 0x0064DD30 slot 0x78
    oml::framework::Result UpdateInitialize() override; // 0x001170B9 slot 0x7C
    bool EndInitialize() override; // 0x0064D0D0 slot 0x80
    bool StartFinalize() override; // 0x00117175 slot 0x84
    oml::framework::Result UpdateFinalize() override; // 0x0011715D slot 0x88
    bool EndFinalize() override; // 0x00117125 slot 0x8C
    void DoAction(PlayerAction::Name, PlayerState const*, bool); // 0x0064DB90 | libgarden [tier A]
    void GetScratchState() const; // 0x006576F8 | libgarden [tier A]
    // (the float of the fishing rod of this player landed at position; name after the address)
    void Func67867C(const nn::math::VEC3* position); // 0x0067867C
    // the table entry of the current action (name is ours)
    DECOMP_NOINLINE DECOMP_NOIPA inline const PlayerActionInfo* GetActionInfo() const; // 0x00764834

    // (the layout so far as the tools show it; the constructor 0x0068EC58 builds the model at 0x1B4; names are ours)
    u32 m_UnknownE4[(0x1A8 - 0xE4) / 4]; // 0xE4 (words: ARMCC does not use the tail padding of the bases)
    u8 m_Unknown0x1A8;
    u8 m_Action;                  // 0x1A9 (index of the action table)
    u8 m_Unknown0x1AA[0x1B4 - 0x1AA];
    PlayerModel m_Model;          // 0x1B4
    u8 m_Unknown0x424[0x8C7 - 0x424];
    u8 m_Unknown0x8C7;            // 0x8C7 (bits 0 and 5: no menu, see BsMenuMgr::CanOpenMenuThink)
};
ASSERT_OFFSET(AcPlayer, m_Model, 0x1B4);
ASSERT_OFFSET(AcPlayer, m_Action, 0x1A9);
ASSERT_OFFSET(AcPlayer, m_Unknown0x8C7, 0x8C7);

inline const PlayerActionInfo* AcPlayer::GetActionInfo() const
{
    if (m_Action >= PLAYER_ACTION_NUM) {
        return NULL;
    }
    return GetPlayerActionInfo(m_Action);
}
