#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "state/dMode.h"

// RTTI 9BsMenuTab @ 0x008CD728
// vtable 0x008FA42C (vptr 0x008FA434), offset_to_top 0, 16 entries
// vtable 0x008FA474 (vptr 0x008FA47C), offset_to_top -20, 3 entries
class BsMenuTab : public ::Base, public ::state::Mode<BsMenuTab>
{
public:
    virtual ~BsMenuTab(); // 0x006D9AB8 slot 0x00 | libgarden
    // 0x006D9AA8 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x006D968C slot 0x0C | libgarden
    virtual void Finalize(); // 0x006D98C0 slot 0x18 | libgarden
    virtual void Calc(); // 0x006D978C slot 0x24 | libgarden
    virtual void Draw(); // 0x006D939C slot 0x30 | libgarden
    void UpdateIn(); // 0x006D59E4 | libgarden [tier A]
    void CheckInput(); // 0x006D5EF0 | libgarden [tier A]
    void State_MenuIn(); // 0x006D5F34 | libgarden [tier A]
    void State_TabSelection(); // 0x006D5F80 | libgarden [tier A]
    void State_MenuOut(); // 0x006D6690 | libgarden [tier A]
    void Trans_MenuOut(); // 0x006D671C | libgarden [tier A]
    void State_TopTabOut(); // 0x006D6E20 | libgarden [tier A]
    void UpdateUpperTabAppearance(); // 0x006D6F54 | libgarden [tier A]
    void BindIn(); // 0x006D7050 | libgarden [tier A]
    void AnimateIn(); // 0x006D7380 | libgarden [tier A]
    void UpdateLowerTabAppearance(); // 0x006D74A4 | libgarden [tier A]
    void BindOut(); // 0x006D75DC | libgarden [tier A]
    void State_SwitchMenu(); // 0x006D7894 | libgarden [tier A]
    void State_TabChatExit(); // 0x006D7A48 | libgarden [tier A]
    void Trans_SwitchMenu(); // 0x006D7AE0 | libgarden [tier A]
    void AnimateOut(); // 0x006D7D5C | libgarden [tier A]
    void State_ImageUp(); // 0x006D80D4 | libgarden [tier A]
    void State_ExitIslandPressed(); // 0x006D8480 | libgarden [tier A]
    void State_WaitMenuEnd(); // 0x006D89C8 | libgarden [tier A]
    void CheckFriendOnline(); // 0x006D8D38 | libgarden [tier A]
    void State_Wait(); // 0x006D8F5C | libgarden [tier A]
    BsMenuTab(); // 0x006D98E0 | libgarden [tier A]
};
