#pragma once

#include "decomp.h"
#include "state/dBase.h"

namespace state {
// Instantiations found in the binary:
//   state::Mode<AddressSelectWindow>  typeinfo 0x008D3428
//   state::Mode<AtmWindow>  typeinfo 0x008D35F0
//   state::Mode<BlackToBtn>  typeinfo 0x008D314C
//   state::Mode<BlackWait>  typeinfo 0x008D35FC
//   state::Mode<BookIconBase>  typeinfo 0x008D31B8
//   state::Mode<BsCommonDialog>  typeinfo 0x008D326C
//   state::Mode<BsHandCursor>  typeinfo 0x008D31C4
//   state::Mode<BsMenuAtm>  typeinfo 0x008D3608
//   state::Mode<BsMenuBbsEditer>  typeinfo 0x008D32E4
//   state::Mode<BsMenuBbsMgr>  typeinfo 0x008D31D0
//   state::Mode<BsMenuBestFriendList>  typeinfo 0x008D3464
//   state::Mode<BsMenuBestFriendListMgr>  typeinfo 0x008D34F4
//   state::Mode<BsMenuBestFriendRegister>  typeinfo 0x008D350C
//   state::Mode<BsMenuBg>  typeinfo 0x008D35A8
//   state::Mode<BsMenuBingoCard>  typeinfo 0x008D32F0
//   state::Mode<BsMenuBook>  typeinfo 0x008D3158
//   state::Mode<BsMenuBorrowThingList>  typeinfo 0x008D34B8
//   state::Mode<BsMenuBulletinBoard>  typeinfo 0x008D3434
//   state::Mode<BsMenuCatalog>  typeinfo 0x008D320C
//   state::Mode<BsMenuChat>  typeinfo 0x008D3164
//   state::Mode<BsMenuChest>  typeinfo 0x008D31A0
//   state::Mode<BsMenuCoolbox>  typeinfo 0x008D3218
//   state::Mode<BsMenuEdit>  typeinfo 0x008D3170
//   state::Mode<BsMenuExplainPlate>  typeinfo 0x008D33D4
//   state::Mode<BsMenuFriendChatLog>  typeinfo 0x008D3440
//   state::Mode<BsMenuItem>  typeinfo 0x008D317C
//   state::Mode<BsMenuItemChange>  typeinfo 0x008D3338
//   state::Mode<BsMenuItemSelect>  typeinfo 0x008D3344
//   state::Mode<BsMenuLetter>  typeinfo 0x008D31DC
//   state::Mode<BsMenuLetterEditer>  typeinfo 0x008D33E0
//   state::Mode<BsMenuLetterMgr>  typeinfo 0x008D32FC
//   state::Mode<BsMenuLetterStock>  typeinfo 0x008D3398
//   state::Mode<BsMenuMailSelect>  typeinfo 0x008D3350
//   state::Mode<BsMenuMailbox>  typeinfo 0x008D3224
//   state::Mode<BsMenuMain>  typeinfo 0x008D3188
//   state::Mode<BsMenuMap>  typeinfo 0x008D3614
//   state::Mode<BsMenuMapVillage>  typeinfo 0x008D335C
//   state::Mode<BsMenuMelody>  typeinfo 0x008D31E8
//   state::Mode<BsMenuMgr>  typeinfo 0x008D3620
//   state::Mode<BsMenuMiiSelect>  typeinfo 0x008D3308
//   state::Mode<BsMenuModelHomeBoard>  typeinfo 0x008D3470
//   state::Mode<BsMenuMyDesign>  typeinfo 0x008D3278
//   state::Mode<BsMenuMyDesignDreamPresent>  typeinfo 0x008D3524
//   state::Mode<BsMenuMyDesignKeep>  typeinfo 0x008D33EC
//   state::Mode<BsMenuMyDesignSelect>  typeinfo 0x008D347C
//   state::Mode<BsMenuNumberInput>  typeinfo 0x008D33A4
//   state::Mode<BsMenuPassport>  typeinfo 0x008D3284
//   state::Mode<BsMenuPassportForFriend>  typeinfo 0x008D3500
//   state::Mode<BsMenuPassportForGhost>  typeinfo 0x008D34DC
//   state::Mode<BsMenuPassportForPhoto>  typeinfo 0x008D34E8
//   state::Mode<BsMenuRandomHeadsUp>  typeinfo 0x008D344C
//   state::Mode<BsMenuReaction>  typeinfo 0x008D3290
//   state::Mode<BsMenuRentalHaniwa>  typeinfo 0x008D33F8
//   state::Mode<BsMenuRoomLightSwitch>  typeinfo 0x008D34C4
//   state::Mode<BsMenuRoomMgr>  typeinfo 0x008D3230
//   state::Mode<BsMenuSantaBagSelect>  typeinfo 0x008D3488
//   state::Mode<BsMenuSendMailLand>  typeinfo 0x008D3404
//   state::Mode<BsMenuSendMailMe>  typeinfo 0x008D3368
//   state::Mode<BsMenuSetupTime>  typeinfo 0x008D3314
//   state::Mode<BsMenuSignList>  typeinfo 0x008D329C
//   ... 49 more
// a state machine that calls one member function of T per frame (names after libgarden; the GetName slot of all
// instances is one folded function returning NULL)
template <typename T>
class Mode : public ::state::Base<T>
{
public:
    typedef typename ::state::Base<T>::Func Func;

    // a named state (debug)
    struct Info
    {
        const char* names;
        Func transition;
        Func state;
    };

    Mode() : m_Callback(NULL), m_CallbackCounter(0), m_Calling(false), m_pInfos(NULL), m_Count(0) {}
    virtual const char* GetName(Func) const { return NULL; }

    void Change(Func func);
    // calls the current state; counts the frames it stays the same
    void Call()
    {
        const Func func = m_Callback;
        if (func != NULL) {
            (static_cast<T*>(this)->*func)();
        }
        if (func == m_Callback) {
            m_CallbackCounter++;
        } else {
            m_CallbackCounter = 0;
        }
    }
    Func GetState() const { return m_Callback; }

    Func m_Callback;          // 0x0C
    u32 m_CallbackCounter;    // 0x14
    bool m_Calling;           // 0x18
    const Info* m_pInfos;     // 0x1C
    u32 m_Count;              // 0x20
};
} // namespace state
