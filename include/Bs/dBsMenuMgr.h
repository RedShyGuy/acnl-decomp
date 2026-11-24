#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "state/dMode.h"
#include "applet/dErrEulaMgr.h"
#include "sead/seadControllerWrapper.h"
#include "Sv/dSvFgName.h"

class BsHandCursor;
class BsMenuBg;
class BsMenuMain;
class BsMenuMap;
class BsMenuRandomHeadsUp;
class BsMenuRoomMgr;
class BsMenuTab;
namespace ssys {
namespace st {
class List;
class ListNode;
} // namespace st
} // namespace ssys

// The manager of the menus (class name from the RTTI; class, member and function names after libgarden where it has them,
// all other names are ours): it opens one menu process at a time (CreateMenuProcess: requested, then created by the
// state "通常" as a child of the last open menu), keeps the lasting menu processes (main, background, tabs, hand cursor,
// map and room manager), the input of the menus and the data the menus are opened with (the statics behind 0x00950D1C).
// RTTI 9BsMenuMgr @ 0x008CD708
// vtable 0x008FA3D0 (vptr 0x008FA3D8), offset_to_top 0, 16 entries
// vtable 0x008FA418 (vptr 0x008FA420), offset_to_top -20, 3 entries
class BsMenuMgr : public ::Base, public ::state::Mode<BsMenuMgr>
{
public:
    // the menus: the index into the process table s_MenuProcs (enum name after libgarden; the names after the process
    // classes, several menus share a class)
    enum Menu : u8
    {
        MENU_ITEM = 0,
        MENU_MY_DESIGN = 1,
        MENU_BOOK = 2,
        MENU_PASSPORT = 3,
        MENU_BEST_FRIEND_LIST_MGR = 4,
        MENU_CHAT = 5,
        MENU_REACTION = 6,
        MENU_BEST_FRIEND_REGISTER = 7,
        MENU_ITEM_SELECT_FIRST = 8, // 8..45 BsMenuItemSelect
        MENU_ITEM_SELECT_LAST = 45,
        MENU_SETUP_TIME_FIRST = 46, // 46..49 BsMenuSetupTime
        MENU_NUMBER_INPUT_FIRST = 50, // 50..54 BsMenuNumberInput
        MENU_ATM_FIRST = 55, // 55..60 BsMenuAtm
        MENU_CHEST = 61,
        MENU_COOLBOX_0 = 62,
        MENU_COOLBOX_1 = 63,
        MENU_RENTAL_HANIWA = 64, // (Haniwa = gyroids)
        MENU_MAILBOX = 65,
        MENU_SEND_MAIL_LAND = 66,
        MENU_SEND_MAIL_ME = 67,
        MENU_LETTER_STOCK = 68,
        MENU_LETTER = 69,
        MENU_LETTER_MGR = 70,
        MENU_BBS_MGR = 71,
        MENU_BULLETIN_BOARD = 72,
        MENU_ITEM_CHANGE_0 = 73, // 73..76 BsMenuItemChange
        MENU_ITEM_CHANGE_1 = 74,
        MENU_ITEM_CHANGE_2 = 75,
        MENU_ITEM_CHANGE_3 = 76,
        MENU_ITEM_MANNEQUIN = 77,
        MENU_MY_DESIGN_SELECT_FIRST = 78, // 78..92 BsMenuMyDesignSelect
        MENU_MY_DESIGN_KEEP = 93,
        MENU_MY_DESIGN_DREAM_PRESENT = 94,
        MENU_MAIL_SELECT = 95,
        MENU_MII_SELECT = 96,
        MENU_SKB = 97,
        MENU_AUDIO_PLAYER_MGR = 98,
        MENU_AUDIO_PLAYER = 99,
        MENU_CREDIT = 100,
        MENU_WHAT_TREE_IS_THIS_TREE = 101,
        MENU_SANTA_BAG_SELECT = 102,
        MENU_SANTA_BAG_VIEW = 103,
        MENU_BINGO_CARD_0 = 104,
        MENU_BINGO_CARD_1 = 105,
        MENU_EXPLAIN_PLATE_0 = 106,
        MENU_EXPLAIN_PLATE_1 = 107,
        MENU_BORROW_THING_LIST = 108,
        MENU_SIGN_LIST_0 = 109,
        MENU_SIGN_LIST_1 = 110,
        MENU_ROOM_LIGHT_SWITCH = 111,
        MENU_INPUT_COMMENT = 112,
        MENU_PASSPORT_FOR_PHOTO = 113,
        MENU_PASSPORT_FOR_GHOST = 114,
        MENU_PASSPORT_FOR_FRIEND = 115,
        MENU_BEST_FRIEND_LIST = 116,
        MENU_FRIEND_CHAT = 117,
        MENU_FRIEND_CHAT_LOG = 118,
        MENU_TRIP_LIST_0 = 119,
        MENU_TRIP_LIST_1 = 120,
        MENU_MELODY_0 = 121,
        MENU_MELODY_1 = 122,
        MENU_EDIT = 123,
        MENU_CATALOG = 124,
        MENU_TOUR_SELECT = 125,
        MENU_AREA_SELECT = 126,
        MENU_AREA_RESULT = 127,
        MENU_QR_ENCODER = 128,
        MENU_QR_READER = 129,
        MENU_MODEL_HOME_BOARD = 130,
        MENU_NFC_READER = 131,
        MENU_INTERIOR_EDITOR = 132,
        MENU_AMIIBO_PHOTO = 133,
        MENU_CAMPER = 134,
        MENU_TICKET_EXCHANGE = 135,
        MENU_APPRAISER = 136,
        MENU_CHEST_1 = 137,
        MENU_COMMON_DIALOG = 138,
        MENU_NUM = 140, // (libgarden: COUNT; the table has an entry 139 = PROC_NAME_NUM)
        MENU_NONE = 140,
    };
    // s_RequestState
    enum RequestState : u8
    {
        REQUEST_NONE = 1,     // (0 before Initialize)
        REQUEST_PENDING = 2,  // the menu is created by the state "通常"
        REQUEST_CREATING = 3, // created; AddToMenuList ends the request
    };
    // the bits of the controller wrapper of the menus (m_Controller): the constructor maps them with the pad config
    // 0x0084C678 to the bits of the CTR controller (Controller::Pad); names are ours after that mapping
    enum Input : u32
    {
        INPUT_A_0 = 1 << 0,        // (A as well)
        INPUT_B_1 = 1 << 1,        // (B as well)
        INPUT_A = 1 << 2,
        INPUT_B = 1 << 3,
        INPUT_X = 1 << 4,
        INPUT_Y = 1 << 5,
        INPUT_START = 1 << 6,
        INPUT_SELECT = 1 << 7,
        INPUT_L = 1 << 8,
        INPUT_R = 1 << 9,
        INPUT_DPAD_UP = 1 << 10,
        INPUT_DPAD_DOWN = 1 << 11,
        INPUT_DPAD_LEFT = 1 << 12,
        INPUT_DPAD_RIGHT = 1 << 13,
        INPUT_CPAD_UP = 1 << 14,
        INPUT_CPAD_DOWN_15 = 1 << 15,  // (circle pad down as well)
        INPUT_CPAD_DOWN = 1 << 16,
        INPUT_CPAD_LEFT = 1 << 17,
        INPUT_CPAD_RIGHT = 1 << 18,
    };
    // the bits of s_Flags (meanings only as far as the code shows them)
    enum Flag : u32
    {
        FLAG_MAP_1 = 1 << 0,
        FLAG_MAP_2 = 1 << 1,
        FLAG_MAP_4 = 1 << 2,         // (also: a menu may be created on stage 164)
        FLAG_CLOSE_DIALOG = 1 << 5,  // the common dialog goes out
        FLAG_KEEP_MAP = 1 << 9,      // the map and room manager are not updated
        FLAG_NO_DEMO_CHECK = 1 << 10, // IsDemoRunning is false
        FLAG_RESET_MASK = 0x3C00,
    };

    BsMenuMgr(); // 0x006D52B4
    // 0x006D5378 (deleting dtor)
    ~BsMenuMgr() override; // 0x006D53A4 slot 0x00
    oml::framework::Result Initialize() override; // 0x006D4978 slot 0x0C
    oml::framework::Result Finalize() override; // 0x006D5158 slot 0x18
    oml::framework::Result Calc() override; // 0x006D4CE0 slot 0x24
    oml::framework::Result Draw() override; // 0x006D4970 slot 0x30

    // the player may open a menu (the menus test it before they open; the meaning of the flags is unknown)
    DECOMP_NOINLINE static bool CanOpenMenuThink(); // 0x005C4B04 | libgarden [tier A]
    // the menu is requested (false: a menu is open or requested, or the manager is locked); param: the argument of the
    // process
    DECOMP_NOINLINE static bool CreateMenuProcess(Menu menu, u8 param = 1); // 0x006D2B4C | libgarden [tier A]
    // the menu is requested as the next one while a menu is open (no music change)
    DECOMP_NOINLINE static bool CreateNextMenuProcess(Menu menu); // 0x006D31AC
    DECOMP_NOINLINE bool IsPressed(unsigned long input); // 0x006D4C24 | libgarden [tier A]

    // (libgarden; the original calls the copy instead of inlining it)
    DECOMP_EMIT_COPY DECOMP_NOINLINE DECOMP_NOIPA static BsMenuMgr* Get() { return s_pInstance; } // 0x006D29B4

    // the inline input tests of the menus (names are ours; no input while the error applet blocks it)
    bool IsTrigOrRepeat(u32 input) const
    {
        return !applet::ErrEulaMgr::IsInputBlocked() && ((m_Controller.mPadTrig | m_Controller.mPadRepeat) & input) != 0;
    }
    bool IsHold(u32 input) const
    {
        return !applet::ErrEulaMgr::IsInputBlocked() && !m_IsCancel && (m_Controller.mPadHold & input) != 0;
    }
    // (m_IsCancel counts as a release of INPUT_A; gui::RepeatButton tests the bit with other inputs)
    bool IsRelease(u32 input) const
    {
        return !applet::ErrEulaMgr::IsInputBlocked() && (((input & INPUT_A) != 0 && m_IsCancel) || (m_Controller.mPadRelease & input) != 0);
    }
    // the circle pad (zero while the input is blocked)
    const sead::Vector2<f32>& GetLeftStick() const
    {
        if (applet::ErrEulaMgr::IsInputBlocked()) {
            // 0x00983614 (guard 0x00983610)
            static const sead::Vector2<f32> s_ZeroStick(0.0f, 0.0f);
            return s_ZeroStick;
        }
        return m_Controller.mLeftStick;
    }

    // the states (the names in the comments are the debug names of the binary)
    void State_Wait();   // 0x006D2EAC 待機 (wait: until the resident files are loaded)
    void Enter_Wait();   // 0x006D2FE0
    void State_Normal(); // 0x006D28C4 通常 (normal)
    void Enter_Normal(); // 0x006D2FDC
    // the requested menu is created as a child of the last open menu (or of parent)
    DECOMP_NOINLINE static void UpdateRequest(oml::framework::Process* parent); // 0x006D28C8
    // the map and the room manager of the lower screen are created and shown
    void UpdateMap(); // 0x006D2C44
    void UpdateRoomMgr(); // 0x006D3630
    // the open menus and the data of the menus are reset
    DECOMP_NOINLINE void Reset(); // 0x006D4144
    // m_IsCancel is set (the menus treat it as a release of INPUT_A)
    bool RequestCancel(); // 0x006D46D0

    // whether no menu is open or requested
    // 0x006D2890 (copy)
    DECOMP_NOINLINE static bool IsIdle(); // 0x006D51AC
    // (its code lies in front of IsIdle and falls into it)
    static bool IsMenuIdle(); // 0x006D51A8
    static bool IsCancelPressed(); // 0x006D2834
    // a menu of the kind (MenuBase::m_Kind) is open
    static bool IsMenuOpen(u8 kind); // 0x006D2A88
    // the kind of the first open menu (MENU_NONE: none)
    static u8 GetOpenMenuKind(); // 0x006D3494
    // the requested menu (open or not) is an item selection
    static bool IsItemSelectRequested(); // 0x006D39A4
    static bool IsPassportForPhotoOpen(); // 0x006D44F8
    // the open menus are stopped (not the lasting ones); true when nothing was left to stop
    static bool CloseMenus(); // 0x006D4574
    // all lasting menus have their archives
    static bool AreArchivesLoaded(); // 0x006D4224
    // a demo that blocks the menus runs
    static bool IsDemoRunning(); // 0x006D30D4
    // the manager blocks new menus (m_IsLocked) / the menus are reset
    static void Lock(); // 0x006D4128
    static void ResetMenus(); // 0x006D401C

    // the menu process with its argument (s_MenuArg)
    static bool OpenMenu(u32 menu, u32 arg); // 0x006D2810
    // (copies of OpenMenu)
    static bool Func6D3B20(u8 menu, u32 arg); // 0x006D3B20
    static bool Func6D448C(u32 menu, u32 arg); // 0x006D448C
    // the menu with s_MenuArg = -1 and s_Result reset
    static bool OpenMenuForResult(u32 menu); // 0x006D2A08
    static bool OpenNextMenu(Menu menu); // 0x006D35D8
    static bool Func6D40AC(u32 menu); // 0x006D40AC
    static bool Func6D3F8C(u32 menu, u32 arg, bool isNext); // 0x006D3F8C

    // the pocket slot the menus work on (>= 20: none) and its item
    static void SetPocketSlot(s32 slot); // 0x006D3078
    static s32 GetPocketSlot(); // 0x006D2A38
    static SvFgName GetPocketSlotItem(); // 0x006D2A50
    // the paper (letter set) of the pocket slot is used once
    static void UsePocketSlotPaper(); // 0x006D3B70
    // a filter of an inventory slot (the item and its appearance); true: the slot is meant
    typedef bool (*InventoryFilter)(const SvFgName* item, u32 appearance);
    // the mask of the 16 pocket slots of the current player that the filter accepts
    static u16 GetPocketSlotMask(InventoryFilter filter); // 0x006D2AC8
    // the items of the slots in s_PocketItemMask, packed into s_PocketItems / their number
    static void CollectPocketItems(); // 0x006D34B4
    static u32 CountPocketItems(); // 0x006D3E90
    // the census counts a planted flower or tree
    static void CountPlanted(const SvFgName* item); // 0x006D4334
    // the four decimal digits of value into s_Digits
    static void SetDigits(u32 value); // 0x006D5274

    // the menus with their data
    static bool OpenInteriorEditor(); // 0x006D2B40
    static bool OpenEdit(u32 slot, u32 patternType); // 0x006D29C4
    static bool Func6D3A74(u8 specialSlot, u32 slot); // 0x006D3A74
    static bool OpenNextAudioPlayer(); // 0x006D2FE4
    static bool OpenChest(); // 0x006D300C
    static bool OpenChest1(); // 0x006D3B44
    static bool OpenSkb(bool isNext); // 0x006D3038
    static bool OpenCredit(); // 0x006D3200
    static bool Func6D322C(u32 value, u32 arg, u8 kind); // 0x006D322C
    static bool Func6D3F18(u32 value, u32 arg, u8 kind); // 0x006D3F18
    static bool Func6D3274(); // 0x006D3274
    static bool OpenCatalog(u32 kind); // 0x006D33D8
    static bool OpenMailbox(); // 0x006D3468
    static bool OpenQrReader(u32 arg); // 0x006D3550
    static bool OpenQrEncoder(u32 arg); // 0x006D3AE4
    static bool OpenReaction(u8 arg, bool isNext); // 0x006D3590
    static bool OpenTripList(bool isSecond); // 0x006D35FC
    static bool OpenAppraiser(bool isNext); // 0x006D3A04
    static bool OpenBingoCard(); // 0x006D3A44
    static bool OpenNextBingoCard(); // 0x006D425C
    static bool OpenMiiSelect(); // 0x006D3AB8
    static bool OpenAreaResult(); // 0x006D3C34
    static bool OpenAreaSelect(); // 0x006D3C68
    static bool OpenTourSelect(); // 0x006D3E5C
    static bool OpenItemChange0(const SvFgName* item); // 0x006D3D34
    static bool OpenItemChange1(const SvFgName* item); // 0x006D3CA8
    static bool OpenItemChange2(const SvFgName* item); // 0x006D4038
    static bool OpenItemChange3(const SvFgName* item, const u32* words); // 0x006D3CDC
    static bool OpenNextFriendChat(); // 0x006D3D68
    static bool OpenNextFriendChatLog(); // 0x006D42E0
    static bool OpenNextBestFriendList(); // 0x006D4394
    static bool Func6D3D90(u16 value, u8 menu, u32 arg); // 0x006D3D90
    static bool Func6D46E0(u16 value, u8 menu, u32 arg); // 0x006D46E0
    static bool Func6D4724(u16 value, u8 menu, u32 arg); // 0x006D4724
    static bool Func6D3DF8(); // 0x006D3DF8
    static bool OpenLetterMgr(); // 0x006D3DD8
    static bool OpenAudioPlayerMgr(); // 0x006D3EEC
    static bool Func6D3F60(); // 0x006D3F60
    static bool Func6D40D4(); // 0x006D40D4
    static bool OpenNextSantaBagView(); // 0x006D4100
    static bool Func6D44B0(); // 0x006D44B0
    static bool OpenBbsMgr(u32 value); // 0x006D4288
    static bool Func6D4308(); // 0x006D4308
    // the common dialog with g_CommonDialogArg (isNext: as the next menu)
    static bool Func6D406C(bool isNext); // 0x006D406C
    // ... with its button in s_Result
    DECOMP_NOINLINE static bool OpenCommonDialogForResult(bool isNext); // 0x006D43C4
    // (its code lies in front of OpenCommonDialogForResult and falls into it)
    static bool OpenNextCommonDialogForResult(); // 0x006D43BC
    static bool OpenNextMelody1(); // 0x006D4418
    static bool OpenModelHomeBoard(u8 a, u32 b, u8 c); // 0x006D4440
    static bool OpenTicketExchange(u8 arg); // 0x006D44E0
    static bool OpenNextBorrowThingList(); // 0x006D454C
    static bool OpenPassportForPhoto(); // 0x006D476C
    static bool OpenNextPassportForPhoto(); // 0x006D4914
    static bool OpenPassportForGhost(u32 arg); // 0x006D47F8
    static bool OpenNextPassportForGhost(u32 arg); // 0x006D493C
    static bool OpenNextPassportForFriend(); // 0x006D48C0
    static bool OpenCoolbox0(); // 0x006D4894
    static bool OpenCoolbox1(); // 0x006D4798
    static bool OpenItemMannequin(u32 a, u32 b); // 0x006D47C4
    static bool OpenWhatTreeIsThisTree(); // 0x006D48E8

    // (meanings unknown; 0x006D2718/0x006D2744 lie in front of the block of BsMenuMgr)
    static bool Func6D2718(); // 0x006D2718
    static void Func6D2744(); // 0x006D2744
    static s32 Func6D32BC(); // 0x006D32BC
    static u16 Func6D3ECC(); // 0x006D3ECC
    static s8 Func6D3334(); // 0x006D3334
    static bool Func6D39E8(); // 0x006D39E8
    static bool Func6D3E24(u32 a, u32 b); // 0x006D3E24
    static void Func6D3FC8(); // 0x006D3FC8
    static void Func6D4CC4(); // 0x006D4CC4
    static void Func6D51E4(); // 0x006D51E4
    static u32 Func6D4824(u16 id); // 0x006D4824
    static void GetUnknown98D554(u32* out); // 0x006D3984
    static void SetUnknown98D554(const u32* value); // 0x006D3C14
    static u8* GetUnknown98D34C(); // 0x006D3C28
    // (names are ours) g_CommonDialogArg is reset / g_Unknown98D508 is set to 584
    DECOMP_NOINLINE static void Func6D32D0(); // 0x006D32D0
    static void Func6D32A0(); // 0x006D32A0
    // (the common dialog closes when it is true; name is ours)
    static bool Func6D520C(); // 0x006D520C

    // the data of the menus (the block 0x00950D1C; names are ours)
    static u8 s_Unknown950D1C;
    static u8 s_IsPocketSlotSet;   // SetPocketSlot: the slot is a pocket slot
    static u8 s_RequestState;      // RequestState
    static u8 s_RequestedMenu;     // Menu
    static u8 s_RequestParam;
    static u8 s_Unknown950D21;
    static u8 s_Unknown950D22;
    static u8 s_Unknown950D23;
    static u8 s_Unknown950D24;
    static u8 s_Unknown950D25;     // the state of the map: 0 shown, 1 created, 2 nothing to do
    static u8 s_Unknown950D26;     // (2: the editor does not change the background)
    static u8 s_Unknown950D27;
    static u8 s_Unknown950D28;
    static u8 s_RoomMgrState;      // like s_Unknown950D25, for the room manager
    static u8 s_RoomMgrKind;       // 1 a room, 2 the model house, 3 the RV
    static u8 s_Unknown950D2B;
    static u8 s_Unknown950D2C;
    static u8 s_Unknown950D2D;
    static u8 s_Unknown950D2E;
    static u8 s_Unknown950D2F;
    static u8 s_Unknown950D30;
    static u8 s_Unknown950D31;
    static u8 s_Unknown950D32;
    static u8 s_Unknown950D33;
    static u8 s_Unknown950D34;
    static u8 s_SelectedSlot;      // the pocket slot of the item menu
    static u8 s_Unknown950D36;
    static u8 s_Unknown950D37;
    static u8 s_Unknown950D38;
    static u8 s_Unknown950D39;
    static u8 s_Unknown950D3A;
    static u8 s_Result;            // the result of the last menu (2: none)
    static u8 s_Unknown950D3C;
    static u8 s_Unknown950D3D;
    static u8 s_Unknown950D3E;
    static u8 s_Unknown950D3F;
    static u8 s_Unknown950D40;
    static s16 s_Unknown950D42;
    static u16 s_PocketItemMask;   // the pocket slots marked in the item menu
    static u32 s_Unknown950D48;
    static BsMenuMgr* s_pInstance;
    static s32 s_Unknown950D50;
    static u32 s_Unknown950D54;
    static u32 s_Unknown950D58;
    static u32 s_Unknown950D5C;
    static s32 s_PocketSlot;
    static u32 s_Unknown950D64;
    static u32 s_Flags;            // Flag
    static s32 s_MenuArg;
    static u32 s_Unknown950D70;    // the four digits of SetDigits
    static SvFgName s_Item;
    static u32 s_Unknown950D78;
    static u32 s_Unknown950D7C;
    static u32 s_Unknown950D80;
    static u32 s_MenuParam;
    static u32 s_Unknown950D94;
    static u32 s_Unknown950DB0;

    // the process classes of the menus (filled from the ProcName constants by the static initializer 0x00788868)
    static u16 s_MenuProcs[MENU_NUM];
    static u8 s_Unknown98D34C[0x98];
    static SvFgName s_PocketItems[16];
    static u32 s_Unknown98D554[3];
    static u32 s_Unknown98D560[3];

    bool m_IsBgShown;                   // 0x38 the background of the menus is shown (set every frame)
    bool m_IsMapShown;                  // 0x39 the map / room of the lower screen is shown (set every frame)
    bool m_Unknown0x3A;                 // 0x3A
    bool m_Unknown0x3B;                 // 0x3B
    bool m_Unknown0x3C;                 // 0x3C
    bool m_Unknown0x3D;                 // 0x3D
    bool m_Unknown0x3E;                 // 0x3E
    bool m_IsRandomHeadsUpCreated;      // 0x3F
    u8 m_HandMode;                      // 0x40 1: the hand cursor is used (name after libgarden)
    u8 m_Unknown0x41;                   // 0x41
    u8 m_Unknown0x42;                   // 0x42 (0..3)
    sead::ControllerWrapper m_Controller; // 0x44 (name after libgarden)
    s32 m_Unk1;                         // 0x1D4 (name after libgarden)
    BsMenuMain* m_pBsMenuMain;          // 0x1D8 (names after libgarden)
    BsMenuBg* m_pBsMenuBg;              // 0x1DC
    BsMenuTab* m_pBsMenuTab;            // 0x1E0
    BsMenuMap* m_pBsMenuMap;            // 0x1E4 (libgarden: m_pProcess, the menu process; the code creates BsMenuMap there)
    BsMenuRoomMgr* m_pRoomMgr;          // 0x1E8
    s32 m_Unk2[2];                      // 0x1EC
    BsHandCursor* m_pBsHandCursor;      // 0x1F4
    BsMenuRandomHeadsUp* m_pBsMenuRandomHeadsUp; // 0x1F8
    // the cursor id of the ButtonActionControl that owns the cursor (14 at the start; 12/13 = cancelled)
    u8 m_HandGrabTimer;                 // 0x1FC (names after libgarden)
    u8 m_CurrentActionNodeIndex;        // 0x1FD the node index of that control
    bool m_IsLocked;                    // 0x1FE no new menu (Lock; cleared by Reset)
    u32 m_Unknown0x200;                 // 0x200
    u32 m_Unknown0x204;                 // 0x204
    bool m_IsCancel;                    // 0x208 (name after libgarden)
};
// (names are ours) parameters of the software keyboard (the editor sets the first two)
extern u32 g_Unknown98D508[3];

ASSERT_OFFSET(BsMenuMgr, m_IsBgShown, 0x38);
ASSERT_OFFSET(BsMenuMgr, m_HandMode, 0x40);
ASSERT_OFFSET(BsMenuMgr, m_Controller, 0x44);
ASSERT_OFFSET(BsMenuMgr, m_pBsMenuMain, 0x1D8);
ASSERT_OFFSET(BsMenuMgr, m_pBsHandCursor, 0x1F4);
ASSERT_OFFSET(BsMenuMgr, m_IsCancel, 0x208);
ASSERT_SIZE(BsMenuMgr, 0x20C);
