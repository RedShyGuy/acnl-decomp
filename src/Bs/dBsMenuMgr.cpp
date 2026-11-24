#include "Bs/dBsMenuMgr.h"

#include "Ac/dAcPlayer.h"
#include "Player/dPlayerApi.h"
#include "Audio/bgm/dBgmTrackMixer.h"
#include "Item/item_Api.h"
#include "Bs/dBsCommonDialog.h"
#include "Bs/dBsHandCursor.h"
#include "Bs/dBsMenuBg.h"
#include "Bs/dBsMenuEdit.h"
#include "Bs/dBsMenuMain.h"
#include "Bs/dBsMenuMap.h"
#include "Bs/dBsMenuMgrExtern.h"
#include "Bs/dBsMenuRandomHeadsUp.h"
#include "Bs/dBsMenuRoomMgr.h"
#include "Bs/dBsMenuTab.h"
#include "Bs/dBsScene.h"
#include "Bs/dBsSceneExtern.h"
#include "Bs/dBsSvMgr.h"
#include "Other/dMenuBase.h"
#include "Other/dProcMgr.h"
#include "Other/dTouchState.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvCensus.h"
#include "Sv/dSvPlayer.h"
#include "demo/demo_Api.h"
#include "font/dMgr.h"
#include "g3d/g3d_System.h"
#include "netgame/dNetGameMgr.h"
#include "netgame/netgame_Api.h"
#include "oml/framework/dProcessManager.h"
#include "script/dScriptExtern.h"
#include "sead/seadControllerMgr.h"
#include "sead/seadHeapMgr.h"
#include "ssys/ma/dAllocator.h"
#include "ssys/ma/lyt/lyt_Api.h"
#include "ssys/st/dList.h"
#include "stage/stage_Api.h"

// the data of the menus (one block in the original, names are ours)
// 0x00950D1C
u8 BsMenuMgr::s_Unknown950D1C = 1;
// 0x00950D1D
u8 BsMenuMgr::s_IsPocketSlotSet;
// 0x00950D1E
u8 BsMenuMgr::s_RequestState;
// 0x00950D1F
u8 BsMenuMgr::s_RequestedMenu = MENU_NONE;
// 0x00950D20
u8 BsMenuMgr::s_RequestParam;
// 0x00950D21
u8 BsMenuMgr::s_Unknown950D21;
// 0x00950D22
u8 BsMenuMgr::s_Unknown950D22 = 1;
// 0x00950D23
u8 BsMenuMgr::s_Unknown950D23;
// 0x00950D24
u8 BsMenuMgr::s_Unknown950D24 = 11;
// 0x00950D25
u8 BsMenuMgr::s_Unknown950D25 = 2;
// 0x00950D26
u8 BsMenuMgr::s_Unknown950D26;
// 0x00950D27
u8 BsMenuMgr::s_Unknown950D27;
// 0x00950D28
u8 BsMenuMgr::s_Unknown950D28;
// 0x00950D29
u8 BsMenuMgr::s_RoomMgrState = 2;
// 0x00950D2A
u8 BsMenuMgr::s_RoomMgrKind;
// 0x00950D2B
u8 BsMenuMgr::s_Unknown950D2B;
// 0x00950D2C
u8 BsMenuMgr::s_Unknown950D2C;
// 0x00950D2D
u8 BsMenuMgr::s_Unknown950D2D;
// 0x00950D2E
u8 BsMenuMgr::s_Unknown950D2E;
// 0x00950D2F
u8 BsMenuMgr::s_Unknown950D2F;
// 0x00950D30
u8 BsMenuMgr::s_Unknown950D30;
// 0x00950D31
u8 BsMenuMgr::s_Unknown950D31;
// 0x00950D32
u8 BsMenuMgr::s_Unknown950D32;
// 0x00950D33
u8 BsMenuMgr::s_Unknown950D33;
// 0x00950D34
u8 BsMenuMgr::s_Unknown950D34;
// 0x00950D35
u8 BsMenuMgr::s_SelectedSlot = 0xFF;
// 0x00950D36
u8 BsMenuMgr::s_Unknown950D36;
// 0x00950D37
u8 BsMenuMgr::s_Unknown950D37;
// 0x00950D38
u8 BsMenuMgr::s_Unknown950D38;
// 0x00950D39
u8 BsMenuMgr::s_Unknown950D39;
// 0x00950D3A
u8 BsMenuMgr::s_Unknown950D3A;
// 0x00950D3B
u8 BsMenuMgr::s_Result;
// 0x00950D3C
u8 BsMenuMgr::s_Unknown950D3C;
// 0x00950D3D
u8 BsMenuMgr::s_Unknown950D3D;
// 0x00950D3E
u8 BsMenuMgr::s_Unknown950D3E;
// 0x00950D3F
u8 BsMenuMgr::s_Unknown950D3F;
// 0x00950D40
u8 BsMenuMgr::s_Unknown950D40;
// 0x00950D42
s16 BsMenuMgr::s_Unknown950D42;
// 0x00950D44
u16 BsMenuMgr::s_PocketItemMask;
// 0x00950D48
u32 BsMenuMgr::s_Unknown950D48;
// 0x00950D4C
BsMenuMgr* BsMenuMgr::s_pInstance;
// 0x00950D50
s32 BsMenuMgr::s_Unknown950D50 = -1;
// 0x00950D54
u32 BsMenuMgr::s_Unknown950D54;
// 0x00950D58
u32 BsMenuMgr::s_Unknown950D58;
// 0x00950D5C
u32 BsMenuMgr::s_Unknown950D5C;
// 0x00950D60
s32 BsMenuMgr::s_PocketSlot;
// 0x00950D64
u32 BsMenuMgr::s_Unknown950D64;
// 0x00950D68
u32 BsMenuMgr::s_Flags;
// 0x00950D6C
s32 BsMenuMgr::s_MenuArg;
// 0x00950D70
u32 BsMenuMgr::s_Unknown950D70;
// 0x00950D74
SvFgName BsMenuMgr::s_Item;
// 0x00950D78
u32 BsMenuMgr::s_Unknown950D78;
// 0x00950D7C
u32 BsMenuMgr::s_Unknown950D7C;
// 0x00950D80
u32 BsMenuMgr::s_Unknown950D80;
// (initial value from .data)
// 0x00950D8C
u32 BsMenuMgr::s_MenuParam = 0x4F;
// 0x00950D94
u32 BsMenuMgr::s_Unknown950D94;
// 0x00950DB0
u32 BsMenuMgr::s_Unknown950DB0;

// 0x0098D34C
u8 BsMenuMgr::s_Unknown98D34C[0x98];
// the process classes of the menus (the static initializer 0x00788868 fills the table from the constants)
// 0x0098D3E4
u16 BsMenuMgr::s_MenuProcs[MENU_NUM] = {
    PROC_BS_MENU_ITEM, PROC_BS_MENU_MY_DESIGN, PROC_BS_MENU_BOOK, PROC_BS_MENU_PASSPORT, PROC_BS_MENU_BEST_FRIEND_LIST_MGR,
    PROC_BS_MENU_CHAT, PROC_BS_MENU_REACTION, PROC_BS_MENU_BEST_FRIEND_REGISTER,
    // 8..45
    PROC_BS_MENU_ITEM_SELECT, PROC_BS_MENU_ITEM_SELECT, PROC_BS_MENU_ITEM_SELECT, PROC_BS_MENU_ITEM_SELECT,
    PROC_BS_MENU_ITEM_SELECT, PROC_BS_MENU_ITEM_SELECT, PROC_BS_MENU_ITEM_SELECT, PROC_BS_MENU_ITEM_SELECT,
    PROC_BS_MENU_ITEM_SELECT, PROC_BS_MENU_ITEM_SELECT, PROC_BS_MENU_ITEM_SELECT, PROC_BS_MENU_ITEM_SELECT,
    PROC_BS_MENU_ITEM_SELECT, PROC_BS_MENU_ITEM_SELECT, PROC_BS_MENU_ITEM_SELECT, PROC_BS_MENU_ITEM_SELECT,
    PROC_BS_MENU_ITEM_SELECT, PROC_BS_MENU_ITEM_SELECT, PROC_BS_MENU_ITEM_SELECT, PROC_BS_MENU_ITEM_SELECT,
    PROC_BS_MENU_ITEM_SELECT, PROC_BS_MENU_ITEM_SELECT, PROC_BS_MENU_ITEM_SELECT, PROC_BS_MENU_ITEM_SELECT,
    PROC_BS_MENU_ITEM_SELECT, PROC_BS_MENU_ITEM_SELECT, PROC_BS_MENU_ITEM_SELECT, PROC_BS_MENU_ITEM_SELECT,
    PROC_BS_MENU_ITEM_SELECT, PROC_BS_MENU_ITEM_SELECT, PROC_BS_MENU_ITEM_SELECT, PROC_BS_MENU_ITEM_SELECT,
    PROC_BS_MENU_ITEM_SELECT, PROC_BS_MENU_ITEM_SELECT, PROC_BS_MENU_ITEM_SELECT, PROC_BS_MENU_ITEM_SELECT,
    PROC_BS_MENU_ITEM_SELECT, PROC_BS_MENU_ITEM_SELECT,
    // 46..49, 50..54, 55..60
    PROC_BS_MENU_SETUP_TIME, PROC_BS_MENU_SETUP_TIME, PROC_BS_MENU_SETUP_TIME, PROC_BS_MENU_SETUP_TIME,
    PROC_BS_MENU_NUMBER_INPUT, PROC_BS_MENU_NUMBER_INPUT, PROC_BS_MENU_NUMBER_INPUT, PROC_BS_MENU_NUMBER_INPUT,
    PROC_BS_MENU_NUMBER_INPUT, PROC_BS_MENU_ATM, PROC_BS_MENU_ATM, PROC_BS_MENU_ATM, PROC_BS_MENU_ATM, PROC_BS_MENU_ATM,
    PROC_BS_MENU_ATM,
    // 61..77
    PROC_BS_MENU_CHEST, PROC_BS_MENU_COOLBOX, PROC_BS_MENU_COOLBOX, PROC_BS_MENU_RENTAL_HANIWA, PROC_BS_MENU_MAILBOX,
    PROC_BS_MENU_SEND_MAIL_LAND, PROC_BS_MENU_SEND_MAIL_ME, PROC_BS_MENU_LETTER_STOCK, PROC_BS_MENU_LETTER,
    PROC_BS_MENU_LETTER_MGR, PROC_BS_MENU_BBS_MGR, PROC_BS_MENU_BULLETIN_BOARD, PROC_BS_MENU_ITEM_CHANGE,
    PROC_BS_MENU_ITEM_CHANGE, PROC_BS_MENU_ITEM_CHANGE, PROC_BS_MENU_ITEM_CHANGE, PROC_BS_MENU_ITEM_MANNEQUIN,
    // 78..92
    PROC_BS_MENU_MY_DESIGN_SELECT, PROC_BS_MENU_MY_DESIGN_SELECT, PROC_BS_MENU_MY_DESIGN_SELECT,
    PROC_BS_MENU_MY_DESIGN_SELECT, PROC_BS_MENU_MY_DESIGN_SELECT, PROC_BS_MENU_MY_DESIGN_SELECT,
    PROC_BS_MENU_MY_DESIGN_SELECT, PROC_BS_MENU_MY_DESIGN_SELECT, PROC_BS_MENU_MY_DESIGN_SELECT,
    PROC_BS_MENU_MY_DESIGN_SELECT, PROC_BS_MENU_MY_DESIGN_SELECT, PROC_BS_MENU_MY_DESIGN_SELECT,
    PROC_BS_MENU_MY_DESIGN_SELECT, PROC_BS_MENU_MY_DESIGN_SELECT, PROC_BS_MENU_MY_DESIGN_SELECT,
    // 93..
    PROC_BS_MENU_MY_DESIGN_KEEP, PROC_BS_MENU_MY_DESIGN_DREAM_PRESENT, PROC_BS_MENU_MAIL_SELECT, PROC_BS_MENU_MII_SELECT,
    PROC_BS_SKB, PROC_BS_MENU_AUDIO_PLAYER_MGR, PROC_BS_MENU_AUDIO_PLAYER, PROC_BS_MENU_CREDIT,
    PROC_BS_MENU_WHAT_TREE_IS_THIS_TREE, PROC_BS_MENU_SANTA_BAG_SELECT, PROC_BS_MENU_SANTA_BAG_VIEW,
    PROC_BS_MENU_BINGO_CARD, PROC_BS_MENU_BINGO_CARD, PROC_BS_MENU_EXPLAIN_PLATE, PROC_BS_MENU_EXPLAIN_PLATE,
    PROC_BS_MENU_BORROW_THING_LIST, PROC_BS_MENU_SIGN_LIST, PROC_BS_MENU_SIGN_LIST, PROC_BS_MENU_ROOM_LIGHT_SWITCH,
    PROC_BS_MENU_INPUT_COMMENT, PROC_BS_MENU_PASSPORT_FOR_PHOTO, PROC_BS_MENU_PASSPORT_FOR_GHOST,
    PROC_BS_MENU_PASSPORT_FOR_FRIEND, PROC_BS_MENU_BEST_FRIEND_LIST, PROC_BS_MENU_FRIEND_CHAT,
    PROC_BS_MENU_FRIEND_CHAT_LOG, PROC_BS_TRIP_LIST, PROC_BS_TRIP_LIST, PROC_BS_MENU_MELODY, PROC_BS_MENU_MELODY,
    PROC_BS_MENU_EDIT, PROC_BS_MENU_CATALOG, PROC_BS_MENU_TOUR_SELECT, PROC_BS_MENU_AREA_SELECT, PROC_BS_MENU_AREA_RESULT,
    PROC_BS_MENU_QR_ENCODER, PROC_BS_MENU_QR_READER, PROC_BS_MENU_MODEL_HOME_BOARD, PROC_BS_MENU_NFC_READER,
    PROC_BS_MENU_INTERIOR_EDITOR, PROC_BS_MENU_AMIIBO_PHOTO, PROC_BS_MENU_CAMPER, PROC_BS_MENU_TICKET_EXCHANGE,
    PROC_BS_MENU_APPRAISER, PROC_BS_MENU_CHEST, PROC_BS_COMMON_DIALOG, PROC_NAME_NUM,
};
// the list of the open menus (MenuBase::List nodes)
// 0x0098D4FC
ssys::st::List g_MenuList;
// 0x0098D508 (name is ours)
u32 g_Unknown98D508[3];
// 0x0098D514
SvFgName BsMenuMgr::s_PocketItems[16];
// 0x0098D554
u32 BsMenuMgr::s_Unknown98D554[3];
// 0x0098D560
u32 BsMenuMgr::s_Unknown98D560[3];

namespace {
// the size of a menu argument that marks "no argument"
const s32 NO_MENU_ARG = -1;
// the value of g_Unknown98D508 that marks "unset"
const u32 UNKNOWN98D508_UNSET = 584;
// the slots >= 20 are no pocket slots
const s32 POCKET_SLOT_NUM = 20;
// the frames of the repeat of the menu input
const u32 PAD_REPEAT_MASK = 0x77F00;

// the bits of the CTR controller for the bits of the menu input (sead::ControllerWrapper::setPadConfig)
// 0x0084C678
extern const u8 s_PadConfig[19] = {
    0x00, 0x01, 0x00, 0x01, 0x03, 0x04, 0x0B, 0x0C, 0x0D, 0x0E, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x15, 0x16, 0x17,
};

// the parent of a new menu: the last open menu (else the given process)
inline oml::framework::Process* GetMenuParent(oml::framework::Process* parent)
{
    ssys::st::ListNode* last = g_MenuList.GetLast();
    if (last != NULL) {
        return static_cast<MenuBase::List*>(last)->m_pMenu;
    }
    return parent;
}
} // namespace

// the states with their names (debug); the machine starts waiting
// 0x0084C68C
static const BsMenuMgr::Info s_BsMenuMgrStateInfos[2] = {
    {"待機", &BsMenuMgr::Enter_Wait, &BsMenuMgr::State_Wait},
    {"通常", &BsMenuMgr::Enter_Normal, &BsMenuMgr::State_Normal},
};

// 0x006D2810
bool BsMenuMgr::OpenMenu(u32 menu, u32 arg)
{
    const bool result = CreateMenuProcess(static_cast<Menu>(menu), 0);
    if (result) {
        s_MenuArg = arg;
    }
    return result;
}

// 0x006D2834
bool BsMenuMgr::IsCancelPressed()
{
    return s_pInstance->IsPressed(INPUT_B);
}

// 0x006D2848
void AddToMenuList(ssys::st::ListNode* node)
{
    if (g_MenuList.Contains(node)) {
        return;
    }
    g_MenuList.PushBack(node);
    if (BsMenuMgr::s_RequestState == BsMenuMgr::REQUEST_CREATING) {
        BsMenuMgr::s_RequestState = BsMenuMgr::REQUEST_NONE;
    }
}

// 0x006D51AC
// 0x006D2890 (copy)
bool BsMenuMgr::IsIdle()
{
    return s_RequestState == REQUEST_NONE && g_MenuList.GetFirst() == NULL;
}

// 0x006D28C4
void BsMenuMgr::State_Normal()
{
    UpdateRequest(this);
}

// 0x006D28C8
void BsMenuMgr::UpdateRequest(oml::framework::Process* parent)
{
    if (s_RequestState != REQUEST_PENDING) {
        return;
    }
    if (applet::ErrEulaMgr::IsInputBlocked()) {
        s_RequestState = REQUEST_NONE;
        return;
    }
    if ((s_Flags & FLAG_MAP_4) == 0 && stage::GetCurrent() != stage::NAME_AMIIBO_CAMERA) {
        return;
    }
    if (s_RequestedMenu >= MENU_NUM) {
        return;
    }
    parent = GetMenuParent(parent);
    s_RequestState = REQUEST_CREATING;
    sead::ScopedCurrentHeapSetter setter(ssys::ma::lyt::GetAllocator()->m_pHeap);
    if (ProcMgr::Create(static_cast<ProcName>(s_MenuProcs[s_RequestedMenu]), parent, 0, PROC_TYPE_BASE) == NULL &&
        s_RequestState == REQUEST_CREATING) {
        s_RequestState = REQUEST_NONE;
    }
}

// 0x006D29C4
bool BsMenuMgr::OpenEdit(u32 slot, u32 patternType)
{
    const bool result = CreateMenuProcess(MENU_EDIT, 0);
    if (result) {
        s_Result = 2;
        BsMenuEdit::SetSettings(slot, patternType);
    }
    return result;
}

// 0x006D2A08
bool BsMenuMgr::OpenMenuForResult(u32 menu)
{
    const bool result = CreateMenuProcess(static_cast<Menu>(menu), 0);
    if (result) {
        s_MenuArg = NO_MENU_ARG;
        s_Result = 2;
    }
    return result;
}

// 0x006D2A38
s32 BsMenuMgr::GetPocketSlot()
{
    s32 slot = s_PocketSlot;
    if (slot >= POCKET_SLOT_NUM) {
        slot -= POCKET_SLOT_NUM;
    }
    return slot;
}

// 0x006D2A50
SvFgName BsMenuMgr::GetPocketSlotItem()
{
    return *SvGetCurrentPlayer()->inventory.GetItem(s_PocketSlot, NULL);
}

// 0x006D2A88
bool BsMenuMgr::IsMenuOpen(u8 kind)
{
    for (ssys::st::ListNode* node = g_MenuList.GetFirst(); node != NULL; node = node->m_pNext) {
        if (static_cast<MenuBase::List*>(node)->m_pMenu->m_Kind == kind) {
            return true;
        }
    }
    return false;
}

// 0x006D2AC8
u16 BsMenuMgr::GetPocketSlotMask(InventoryFilter filter)
{
    const SvPlayerInventory& inventory = SvGetCurrentPlayer()->inventory;
    u16 mask = 0;
    for (u32 i = 0; i < 16; i++) {
        const u32 appearance = inventory.GetAppearance(i);
        if (filter(inventory.GetItem(i, NULL), appearance)) {
            mask |= 1 << i;
        }
    }
    return mask;
}

// 0x006D2B40
bool BsMenuMgr::OpenInteriorEditor()
{
    return CreateMenuProcess(MENU_INTERIOR_EDITOR, 0);
}

// 0x005C4B04 | libgarden [tier A]
bool BsMenuMgr::CanOpenMenuThink()
{
    const AcPlayer* player = PlayerGetActor(netgame::PLAYER_NO_MY_NO, true);
    if (player == NULL) {
        return false;
    }
    if ((player->m_Unknown0x8C7 & 0x20) != 0 || (player->m_Unknown0x8C7 & 1) != 0) {
        return false;
    }
    const PlayerActionInfo* info = player->GetActionInfo();
    if (info == NULL) {
        return false;
    }
    return (info->flags & 0x40) != 0;
}

// 0x006D2B4C | libgarden [tier A]
bool BsMenuMgr::CreateMenuProcess(Menu menu, u8 param)
{
    if (s_RequestState != REQUEST_NONE || g_MenuList.GetFirst() != NULL) {
        return false;
    }
    if (s_pInstance != NULL && s_pInstance->m_IsLocked) {
        return false;
    }
    s_RequestState = REQUEST_PENDING;
    s_RequestedMenu = menu;
    s_RequestParam = param;
    // the music of the field gets the mix of the menu
    if (stage::GetCurrent() != stage::NAME_CLUB444_DJKK && menu != MENU_CHAT) {
        switch (menu) {
        case MENU_REACTION:
        case MENU_CATALOG:
        case MENU_CREDIT:
        case MENU_APPRAISER:
            break;
        case MENU_AUDIO_PLAYER_MGR:
        case MENU_AUDIO_PLAYER:
            audio::bgm::GetTrackMixer()->AddMix(31);
            break;
        case MENU_EDIT:
        case MENU_QR_ENCODER:
        case MENU_QR_READER:
            audio::bgm::GetTrackMixer()->AddMix(30);
            break;
        default:
            audio::bgm::GetTrackMixer()->AddMix(33);
            break;
        }
    }
    return true;
}

// 0x006D2C44
void BsMenuMgr::UpdateMap()
{
    if (!Func566E5C() || m_IsMapShown != true) {
        return;
    }
    sead::ScopedCurrentHeapSetter setter(ssys::ma::lyt::GetAllocator()->m_pHeap);
    if ((s_Flags & FLAG_KEEP_MAP) == 0) {
        if (stage::IsKind(stage::KIND_INDOOR, stage::GetCurrent())) {
            if (m_pBsMenuMap != NULL && g_Unknown950C30 != 0 && !m_Unknown0x3B) {
                m_pBsMenuMap->Func6D2370();
                m_Unknown0x3B = true;
            }
            s_Unknown950D25 = 2;
            return;
        }
        if (!demo::CheckOrder(demo::REVOKED_PLAYER_CONTROL, false)) {
            SvPlayer* player = g_UnkAAFF18.pPlayer;
            if (m_Unknown0x3A) {
                if (m_pBsMenuMap == NULL && !m_Unknown0x3C && m_pRoomMgr == NULL) {
                    s_Unknown950D25 = 1;
                    m_Unknown0x3C = true;
                    s_Unknown950D34 = 0;
                }
            } else if ((player->playerMii.unk_0x70[0] & 0x10) == 0) {
                s_Unknown950D25 = 2;
            } else {
                m_Unknown0x3A = true;
                s_Flags &= ~FLAG_KEEP_MAP;
            }
        }
    }
    switch (s_Unknown950D25) {
    case 0:
        if (m_pBsMenuMap != NULL && g_Unknown950C30 != 0) {
            m_pBsMenuMap->Func6D2370();
            s_Unknown950D25 = 2;
            if ((s_Flags & FLAG_KEEP_MAP) != 0) {
                s_Flags &= ~FLAG_KEEP_MAP;
            }
        } else {
            s_Flags = (s_Flags & ~FLAG_MAP_2) | FLAG_MAP_4;
        }
        break;
    case 1:
        if (m_pBsMenuMap != NULL) {
            if (m_pRoomMgr == NULL) {
                s_Unknown950D25 = 2;
                break;
            }
        } else if (m_pRoomMgr == NULL) {
            if (stage::IsKind(stage::KIND_AUTO_CAMP, stage::GetCurrent())) {
                s_Flags = (s_Flags & ~FLAG_MAP_1) | FLAG_MAP_2;
            } else {
                m_pBsMenuMap = static_cast<BsMenuMap*>(ProcMgr::Create(PROC_BS_MENU_MAP, this, 0, PROC_TYPE_BASE));
                m_Unknown0x3C = true;
            }
            s_Unknown950D25 = 2;
            break;
        }
        m_pRoomMgr->Func2258A8();
        m_Unknown0x3D = true;
        s_Flags = (s_Flags & ~FLAG_MAP_1) | FLAG_MAP_2;
        break;
    }
}

// 0x006D2EAC
void BsMenuMgr::State_Wait()
{
    Func566B2C();
    if (Func566AD0()) {
        ChangeInline(&BsMenuMgr::State_Normal);
    }
}

// 0x006D2FDC
void BsMenuMgr::Enter_Normal()
{
    // empty (in the original too)
}

// 0x006D2FE0
void BsMenuMgr::Enter_Wait()
{
    // empty (in the original too)
}

// 0x006D2FE4
bool BsMenuMgr::OpenNextAudioPlayer()
{
    const bool result = CreateNextMenuProcess(MENU_AUDIO_PLAYER);
    if (result) {
        s_MenuArg = NO_MENU_ARG;
    }
    return result;
}

// 0x006D300C
bool BsMenuMgr::OpenChest()
{
    const bool result = CreateMenuProcess(MENU_CHEST, 0);
    if (result) {
        s_MenuArg = NO_MENU_ARG;
    }
    return result;
}

// 0x006D3038
bool BsMenuMgr::OpenSkb(bool isNext)
{
    const bool result = isNext ? CreateNextMenuProcess(MENU_SKB) : CreateMenuProcess(MENU_SKB, 0);
    if (result) {
        s_MenuArg = NO_MENU_ARG;
    }
    return result;
}

// 0x006D3078
void BsMenuMgr::SetPocketSlot(s32 slot)
{
    s_PocketSlot = slot;
    if (slot >= POCKET_SLOT_NUM) {
        s_IsPocketSlotSet = false;
        return;
    }
    // (the item is copied; the call that used it was removed by armlink)
    const SvFgName item = *SvGetCurrentPlayer()->inventory.GetItem(s_PocketSlot, NULL);
    (void)item;
    s_IsPocketSlotSet = true;
}

// 0x006D30D4
bool BsMenuMgr::IsDemoRunning()
{
    if ((s_Flags & FLAG_NO_DEMO_CHECK) != 0) {
        return false;
    }
    return demo::CheckOrder(demo::ORDER_5, false) || demo::CheckOrder(demo::CREDITS, false) ||
           demo::CheckOrder(demo::ORDER_0, false) || demo::CheckOrder(demo::ORDER_3, false) ||
           demo::CheckOrder(demo::WARP, false) || demo::CheckOrder(demo::ORDER_9, false) ||
           demo::CheckOrder(demo::SAVE, false) || demo::CheckOrder(demo::ORDER_11, false) ||
           demo::CheckOrder(demo::NETGAME_LOCK, false);
}

// 0x006D31AC
bool BsMenuMgr::CreateNextMenuProcess(Menu menu)
{
    if (s_pInstance != NULL && s_pInstance->m_IsLocked) {
        return false;
    }
    if (s_RequestState != REQUEST_NONE || g_MenuList.GetFirst() == NULL) {
        return false;
    }
    s_RequestState = REQUEST_PENDING;
    s_RequestedMenu = menu;
    return true;
}

// 0x006D3200
bool BsMenuMgr::OpenCredit()
{
    const bool result = CreateMenuProcess(MENU_CREDIT, 0);
    if (result) {
        s_MenuArg = NO_MENU_ARG;
    }
    return result;
}

// 0x006D322C
bool BsMenuMgr::Func6D322C(u32 value, u32 arg, u8 kind)
{
    s_pInstance->m_Unknown0x200 = value;
    s_Unknown950D3A = kind;
    s_Unknown950D1C = 1;
    s_Unknown950D64 = arg;
    const bool result = CreateMenuProcess(MENU_LETTER, 0);
    if (!result) {
        s_pInstance->m_Unknown0x200 = 0;
    }
    return result;
}

// 0x006D3274
bool BsMenuMgr::Func6D3274()
{
    const bool result = CreateMenuProcess(MENU_MELODY_0, 0);
    if (result) {
        s_Result = 2;
    }
    return result;
}

// 0x006D32A0
void BsMenuMgr::Func6D32A0()
{
    g_Unknown98D508[0] = UNKNOWN98D508_UNSET;
    g_Unknown98D508[1] = UNKNOWN98D508_UNSET;
    g_Unknown98D508[2] = UNKNOWN98D508_UNSET;
}

// 0x006D32BC
s32 BsMenuMgr::Func6D32BC()
{
    return static_cast<s32>(s_Unknown950D48) >> 16;
}

// 0x006D32D0
void BsMenuMgr::Func6D32D0()
{
    g_CommonDialogArg.kind = 0;
    g_CommonDialogArg.icon = 0;
    g_CommonDialogArg.labels[0] = -1;
    g_CommonDialogArg.labels[1] = -1;
    g_CommonDialogArg.labels[2] = -1;
    g_CommonDialogArg.labels[3] = -1;
    g_CommonDialogArg.okSe = static_cast<SeID>(-1);
    g_CommonDialogArg.cancelSe = static_cast<SeID>(-1);
    g_CommonDialogArg.number = 0;
    g_CommonDialogArg.wordKind = -1;
    g_CommonDialogArg.pFile = NULL;
    g_CommonDialogArg.pPerson = NULL;
    g_CommonDialogArg.pItem = NULL;
    g_CommonDialogArg.pWord = NULL;
    g_CommonDialogArg.callback = NULL;
    g_CommonDialogArg.isCentered = true;
    g_CommonDialogArg.isLast = true;
    g_CommonDialogArg.isIconShown = false;
    g_CommonDialogArg.input = false;
}

// 0x006D3334
s8 BsMenuMgr::Func6D3334()
{
    return s_pInstance->m_pBsMenuTab->m_Unknown0x1916;
}

// 0x006D3350
void RemoveFromMenuList(ssys::st::ListNode* node)
{
    if (!g_MenuList.Contains(node)) {
        return;
    }
    g_MenuList.Erase(node);
    if (g_MenuList.GetFirst() != NULL) {
        return;
    }
    // the last menu has closed: the music of the field gets its mix back
    if (BsMenuMgr::s_RequestParam == 0) {
        audio::bgm::GetTrackMixer()->RemoveMix(33);
        audio::bgm::GetTrackMixer()->RemoveMix(31);
        audio::bgm::GetTrackMixer()->RemoveMix(30);
    }
    if (BsMenuMgr::s_pInstance != NULL) {
        BsMenuMgr::s_pInstance->m_IsCancel = false;
    }
}

// 0x006D33D8
bool BsMenuMgr::OpenCatalog(u32 kind)
{
    const bool result = kind == 4 || kind == 5 ? CreateNextMenuProcess(MENU_CATALOG) : CreateMenuProcess(MENU_CATALOG, 0);
    if (result) {
        audio::bgm::GetTrackMixer()->AddMix(kind == 3 ? 33 : 30);
        g_Unknown9507F0 = kind;
        s_Result = 2;
        s_Unknown950D23 = 0;
    }
    return result;
}

// 0x006D3468
bool BsMenuMgr::OpenMailbox()
{
    const bool result = CreateMenuProcess(MENU_MAILBOX, 0);
    if (result) {
        s_MenuArg = NO_MENU_ARG;
    }
    return result;
}

// 0x006D3494
u8 BsMenuMgr::GetOpenMenuKind()
{
    ssys::st::ListNode* node = g_MenuList.GetFirst();
    if (node == NULL) {
        return MENU_NONE;
    }
    return static_cast<MenuBase::List*>(node)->m_pMenu->m_Kind;
}

// 0x006D34B4
void BsMenuMgr::CollectPocketItems()
{
    for (u32 i = 0; i < 16; i++) {
        if ((s_PocketItemMask & (1 << i)) != 0) {
            s_PocketItems[i] = SvFgName::s_Empty;
        }
    }
    const SvPlayerInventory& inventory = SvGetCurrentPlayer()->inventory;
    u32 count = 0;
    for (u32 i = 0; i < 16; i++) {
        if ((s_PocketItemMask & (1 << i)) != 0) {
            s_PocketItems[count] = *inventory.GetItem(i, NULL);
            count++;
        }
    }
}

// 0x006D3550
bool BsMenuMgr::OpenQrReader(u32 arg)
{
    const bool result = CreateMenuProcess(MENU_QR_READER, 0);
    if (result) {
        s_Unknown950D3C = 0;
        s_Unknown950D78 = arg;
        s_Result = 2;
    }
    return result;
}

// 0x006D3590
bool BsMenuMgr::OpenReaction(u8 arg, bool isNext)
{
    const bool result = isNext ? CreateNextMenuProcess(MENU_REACTION) : CreateMenuProcess(MENU_REACTION, 0);
    if (result) {
        s_Unknown950D22 = arg;
        s_MenuArg = NO_MENU_ARG;
    }
    return result;
}

// 0x006D35D8
bool BsMenuMgr::OpenNextMenu(Menu menu)
{
    const bool result = CreateNextMenuProcess(menu);
    if (result) {
        s_MenuArg = NO_MENU_ARG;
    }
    return result;
}

// 0x006D35FC
bool BsMenuMgr::OpenTripList(bool isSecond)
{
    const bool result = CreateMenuProcess(isSecond ? MENU_TRIP_LIST_1 : MENU_TRIP_LIST_0, 0);
    if (result) {
        s_Result = 2;
    }
    return result;
}

// 0x006D3630
void BsMenuMgr::UpdateRoomMgr()
{
    if (!Func566E5C() || m_IsMapShown != true) {
        return;
    }
    sead::ScopedCurrentHeapSetter setter(ssys::ma::lyt::GetAllocator()->m_pHeap);
    if ((s_Flags & FLAG_KEEP_MAP) == 0) {
        if (!stage::IsKind(stage::KIND_INDOOR, stage::GetCurrent())) {
            if (!stage::IsKind(stage::KIND_MODEL_HOUSE, stage::GetCurrent()) &&
                !stage::IsKind(stage::KIND_RV_CAR_ENVIRONMENT, stage::GetCurrent()) && m_pRoomMgr != NULL &&
                !m_Unknown0x3D) {
                m_pRoomMgr->Func2258A8();
                m_Unknown0x3D = true;
            }
            s_RoomMgrState = 2;
            return;
        }
        bool isCreateWanted;
        if (stage::IsKind(stage::KIND_RV_CAR_ENVIRONMENT, stage::GetCurrent())) {
            if (m_pRoomMgr != NULL && g_pUnknown951038 == NULL) {
                m_pRoomMgr = NULL;
                m_Unknown0x3E = false;
            }
        }
        if (m_pRoomMgr == NULL && !m_Unknown0x3E && m_pBsMenuMap == NULL) {
            isCreateWanted = true;
        } else {
            isCreateWanted = stage::IsKind(stage::KIND_RV_CAR_ENVIRONMENT, stage::GetCurrent()) && g_pUnknown951038 == NULL;
        }
        if (isCreateWanted) {
            if (!IsDemoRunning() && !demo::IsTalking()) {
                s_RoomMgrState = 1;
                m_Unknown0x3E = true;
            } else if (stage::IsKind(stage::KIND_RV_CAR_ENVIRONMENT, stage::GetCurrent())) {
                s_RoomMgrState = 0;
            }
        }
    }
    switch (s_RoomMgrState) {
    case 0:
        if (m_pRoomMgr != NULL && s_Unknown950D34 != 0) {
            m_pRoomMgr->Func2258A8();
            s_RoomMgrState = 2;
        }
        break;
    case 1:
        if (m_pRoomMgr != NULL || m_pBsMenuMap != NULL || IsDemoRunning() || demo::IsTalking()) {
            if (m_pBsMenuMap != NULL && g_Unknown950C30 != 0) {
                if ((s_Flags & FLAG_KEEP_MAP) != 0) {
                    s_RoomMgrState = 2;
                }
            } else {
                if (!IsDemoRunning()) {
                    demo::IsTalking();
                }
                s_RoomMgrState = 2;
            }
            break;
        }
        if (stage::IsKind(stage::KIND_MODEL_HOUSE, stage::GetCurrent())) {
            s_RoomMgrKind = 2;
            const u8 structure = stage::GetLastStructure();
            s_Unknown950D40 = Func6CC9F8(structure);
            s_Unknown950D80 = Func6CCA38(structure);
            s_Unknown950D3F = 0;
        } else if (stage::IsKind(stage::KIND_RV_CAR_ENVIRONMENT, stage::GetCurrent())) {
            s_RoomMgrKind = 3;
        } else {
            s_RoomMgrKind = 1;
        }
        m_pRoomMgr = static_cast<BsMenuRoomMgr*>(ProcMgr::Create(PROC_BS_MENU_ROOM_MGR, this, 0, PROC_TYPE_BASE));
        s_Unknown950D34 = 0;
        m_Unknown0x3E = true;
        s_RoomMgrState = 2;
        break;
    }
}

// 0x006D3984
void BsMenuMgr::GetUnknown98D554(u32* out)
{
    out[0] = s_Unknown98D554[0];
    out[1] = s_Unknown98D554[1];
    out[2] = s_Unknown98D554[2];
}

// 0x006D39A4
bool BsMenuMgr::IsItemSelectRequested()
{
    if (s_RequestState == REQUEST_NONE && g_MenuList.GetFirst() == NULL) {
        return false;
    }
    return static_cast<u32>(s_RequestedMenu - MENU_ITEM_SELECT_FIRST) <
           static_cast<u32>(MENU_ITEM_SELECT_LAST - MENU_ITEM_SELECT_FIRST + 1);
}

// 0x006D39E8
bool BsMenuMgr::Func6D39E8()
{
    return g_Unknown98D508[0] != UNKNOWN98D508_UNSET;
}

// 0x006D3A04
bool BsMenuMgr::OpenAppraiser(bool isNext)
{
    const bool result = isNext ? CreateNextMenuProcess(MENU_APPRAISER) : CreateMenuProcess(MENU_APPRAISER, 0);
    if (result) {
        s_MenuArg = NO_MENU_ARG;
    }
    return result;
}

// 0x006D3A44
bool BsMenuMgr::OpenBingoCard()
{
    s_Unknown950D2E = 0;
    const bool result = CreateMenuProcess(MENU_BINGO_CARD_0, 0);
    if (result) {
        s_MenuArg = NO_MENU_ARG;
    }
    return result;
}

// 0x006D3A74
bool BsMenuMgr::Func6D3A74(u8 specialSlot, u32 slot)
{
    const bool result = CreateMenuProcess(MENU_EDIT, 0);
    if (result) {
        s_Result = 2;
        BsMenuEdit::SetSpecialSettings(specialSlot, slot);
    }
    return result;
}

// 0x006D3AB8
bool BsMenuMgr::OpenMiiSelect()
{
    const bool result = CreateMenuProcess(MENU_MII_SELECT, 0);
    if (result) {
        s_MenuArg = NO_MENU_ARG;
    }
    return result;
}

// 0x006D3AE4
bool BsMenuMgr::OpenQrEncoder(u32 arg)
{
    const bool result = CreateMenuProcess(MENU_QR_ENCODER, 0);
    if (result) {
        Func1503E0(arg);
        s_Result = 2;
    }
    return result;
}

// 0x006D3B20
bool BsMenuMgr::Func6D3B20(u8 menu, u32 arg)
{
    const bool result = CreateMenuProcess(static_cast<Menu>(menu), 0);
    if (result) {
        s_MenuArg = arg;
    }
    return result;
}

// 0x006D3B44
bool BsMenuMgr::OpenChest1()
{
    const bool result = CreateMenuProcess(MENU_CHEST_1, 0);
    if (result) {
        s_MenuArg = NO_MENU_ARG;
    }
    return result;
}

// 0x006D3B70
void BsMenuMgr::UsePocketSlotPaper()
{
    SvPlayerInventory* inventory = &SvGetCurrentPlayer()->inventory;
    SvFgName item = *inventory->GetItem(s_PocketSlot, NULL);
    const s32 count = item.DecreasePaperCount();
    if (count != 0) {
        item.SetPaperCount(count);
        Func2BFF98(inventory, s_PocketSlot, &item, 0, 1);
    } else {
        Func2BFF98(inventory, s_PocketSlot, &SvFgName::s_Empty, 0, 1);
    }
}

// 0x006D3C14
void BsMenuMgr::SetUnknown98D554(const u32* value)
{
    s_Unknown98D554[0] = value[0];
    s_Unknown98D554[1] = value[1];
    s_Unknown98D554[2] = value[2];
}

// 0x006D3C28
u8* BsMenuMgr::GetUnknown98D34C()
{
    return s_Unknown98D34C;
}

// 0x006D3C34
bool BsMenuMgr::OpenAreaResult()
{
    const bool result = CreateMenuProcess(MENU_AREA_RESULT, 0);
    if (result) {
        s_Result = 2;
        s_Unknown950D48 = 0xFFFFFFFF;
    }
    return result;
}

// 0x006D3C68
bool BsMenuMgr::OpenAreaSelect()
{
    const bool result = CreateMenuProcess(MENU_AREA_SELECT, 0);
    if (result) {
        s_Result = 2;
        s_Unknown950D48 = 0;
        Func2AE4B4(0);
    }
    return result;
}

// 0x006D3CA8
bool BsMenuMgr::OpenItemChange1(const SvFgName* item)
{
    const bool result = CreateMenuProcess(MENU_ITEM_CHANGE_1, 0);
    if (result) {
        s_Item = *item;
    }
    return result;
}

// 0x006D3CDC
bool BsMenuMgr::OpenItemChange3(const SvFgName* item, const u32* words)
{
    const bool result = CreateMenuProcess(MENU_ITEM_CHANGE_3, 0);
    if (result) {
        s_Item = *item;
        s_Unknown98D560[0] = words[0];
        s_Unknown98D560[1] = words[1];
        s_Unknown98D560[2] = words[2];
    }
    return result;
}

// 0x006D3D34
bool BsMenuMgr::OpenItemChange0(const SvFgName* item)
{
    const bool result = CreateMenuProcess(MENU_ITEM_CHANGE_0, 0);
    if (result) {
        s_Item = *item;
    }
    return result;
}

// 0x006D3D68
bool BsMenuMgr::OpenNextFriendChat()
{
    const bool result = CreateNextMenuProcess(MENU_FRIEND_CHAT);
    if (result) {
        s_MenuArg = NO_MENU_ARG;
    }
    return result;
}

// 0x006D3D90
bool BsMenuMgr::Func6D3D90(u16 value, u8 menu, u32 arg)
{
    const bool result = CreateMenuProcess(static_cast<Menu>(menu), 0);
    if (result) {
        s_Unknown950D42 = value;
        s_MenuArg = arg;
        s_SelectedSlot = 0xFF;
        s_Unknown950D36 = 0;
    }
    return result;
}

// 0x006D3DD8
bool BsMenuMgr::OpenLetterMgr()
{
    s_Unknown950D3A = MENU_LETTER;
    s_Unknown950D1C = 0;
    return CreateMenuProcess(MENU_LETTER_MGR, 0);
}

// 0x006D3DF8
bool BsMenuMgr::Func6D3DF8()
{
    const bool result = CreateMenuProcess(MENU_MAIL_SELECT, 0);
    if (result) {
        s_MenuArg = NO_MENU_ARG;
    }
    return result;
}

// 0x006D3E24
bool BsMenuMgr::Func6D3E24(u32 a, u32 b)
{
    BsMenuMap* map = s_pInstance->m_pBsMenuMap;
    if (map == NULL) {
        return false;
    }
    return map->Func6D1FAC(a, b) != false;
}

// 0x006D3E5C
bool BsMenuMgr::OpenTourSelect()
{
    const bool result = CreateMenuProcess(MENU_TOUR_SELECT, 0);
    if (result) {
        s_Result = 2;
        s_Unknown950D48 = 0xFFFFFFFF;
    }
    return result;
}

// 0x006D3E90
u32 BsMenuMgr::CountPocketItems()
{
    u32 count = 0;
    for (u32 i = 0; i < 16; i++) {
        if ((s_PocketItemMask & (1 << i)) != 0) {
            count++;
        }
    }
    return count;
}

// 0x006D3ECC
u16 BsMenuMgr::Func6D3ECC()
{
    const u32 value = s_Unknown950D48;
    if ((value & 0xFFFF) != 0) {
        return value;
    }
    return 1;
}

// 0x006D3EEC
bool BsMenuMgr::OpenAudioPlayerMgr()
{
    const bool result = CreateMenuProcess(MENU_AUDIO_PLAYER_MGR, 0);
    if (result) {
        s_MenuArg = NO_MENU_ARG;
    }
    return result;
}

// 0x006D3F18
bool BsMenuMgr::Func6D3F18(u32 value, u32 arg, u8 kind)
{
    s_pInstance->m_Unknown0x200 = value;
    s_Unknown950D3A = kind;
    s_Unknown950D1C = 1;
    s_Unknown950D64 = arg;
    const bool result = CreateNextMenuProcess(MENU_LETTER_MGR);
    if (!result) {
        s_pInstance->m_Unknown0x200 = 0;
        s_Unknown950D28 = 0;
    }
    return result;
}

// 0x006D3F60
bool BsMenuMgr::Func6D3F60()
{
    const bool result = CreateMenuProcess(MENU_LETTER_STOCK, 0);
    if (result) {
        s_Result = 2;
    }
    return result;
}

// 0x006D3F8C
bool BsMenuMgr::Func6D3F8C(u32 menu, u32 arg, bool isNext)
{
    const bool result =
        isNext ? CreateNextMenuProcess(static_cast<Menu>(menu)) : CreateMenuProcess(static_cast<Menu>(menu), 0);
    if (result) {
        s_MenuArg = arg;
    }
    return result;
}

// 0x006D3FC8
void BsMenuMgr::Func6D3FC8()
{
    for (ssys::st::ListNode* node = g_MenuList.GetFirst(); node != NULL; node = node->m_pNext) {
        MenuBase* menu = static_cast<MenuBase::List*>(node)->m_pMenu;
        if (menu->m_Index == PROC_BS_MENU_ITEM) {
            Func26D8E4(reinterpret_cast<u8*>(menu) + 0x1EC0);
            return;
        }
    }
}

// 0x006D401C
void BsMenuMgr::ResetMenus()
{
    if (s_pInstance != NULL) {
        s_pInstance->Reset();
    }
}

// 0x006D4038
bool BsMenuMgr::OpenItemChange2(const SvFgName* item)
{
    const bool result = CreateMenuProcess(MENU_ITEM_CHANGE_2, 0);
    if (result) {
        s_Item = *item;
    }
    return result;
}

// 0x006D406C
bool BsMenuMgr::Func6D406C(bool isNext)
{
    const bool result =
        isNext ? CreateNextMenuProcess(MENU_COMMON_DIALOG) : CreateMenuProcess(MENU_COMMON_DIALOG, 0);
    if (result) {
        s_MenuArg = NO_MENU_ARG;
    }
    return result;
}

// 0x006D40AC
bool BsMenuMgr::Func6D40AC(u32 menu)
{
    s_Unknown950D2C = 0;
    const bool result = CreateMenuProcess(static_cast<Menu>(menu), 0);
    if (result) {
        s_MenuArg = NO_MENU_ARG;
    }
    return result;
}

// 0x006D40D4
bool BsMenuMgr::Func6D40D4()
{
    const bool result = CreateMenuProcess(MENU_RENTAL_HANIWA, 0);
    if (result) {
        s_MenuArg = NO_MENU_ARG;
    }
    return result;
}

// 0x006D4100
bool BsMenuMgr::OpenNextSantaBagView()
{
    const bool result = CreateNextMenuProcess(MENU_SANTA_BAG_VIEW);
    if (result) {
        s_MenuArg = NO_MENU_ARG;
    }
    return result;
}

// 0x006D4128
void BsMenuMgr::Lock()
{
    if (s_pInstance != NULL) {
        s_pInstance->m_IsLocked = true;
    }
}

// 0x006D4144
void BsMenuMgr::Reset()
{
    if (m_pBsMenuMain != NULL) {
        m_pBsMenuMain->Func1A0E74();
    }
    s_Unknown950D50 = -1;
    s_RequestParam = 0;
    s_Unknown950D21 = 0;
    s_Unknown950D22 = 1;
    s_Unknown950D23 = 0;
    s_Unknown950D2C = 0;
    s_Unknown950D2D = 0;
    s_Unknown950D2E = 0;
    s_Unknown950D2B = 0;
    s_Unknown950D2F = 0;
    s_Unknown950D30 = 0;
    s_Unknown950D54 = 0;
    s_Flags &= ~FLAG_RESET_MASK;
    s_Unknown950D1C = 1;
    s_IsPocketSlotSet = 0;
    s_Item = SvFgName::s_Empty;
    s_Unknown950D58 = 0;
    s_Unknown950D37 = 0;
    s_Unknown950D38 = 0;
    s_Unknown950D31 = 0;
    s_Unknown950D32 = 0;
    s_Unknown950D39 = 0;
    s_RequestState = REQUEST_NONE;
    s_RequestedMenu = MENU_NONE;
    s_pInstance->m_Unknown0x41 = 0;
    s_Unknown950D26 = 0;
    s_Unknown950D27 = 0;
    s_Unknown950D24 = 11;
    if (s_pInstance->m_pBsMenuTab != NULL) {
        s_pInstance->m_pBsMenuTab->Func6D93D0();
    }
    if (m_pBsMenuBg != NULL) {
        m_pBsMenuBg->Reset();
    }
    g3d::EndRender();
    m_IsLocked = false;
}

// 0x006D4224
bool BsMenuMgr::AreArchivesLoaded()
{
    bool loaded = CommonButtonSingle::LoadArchive();
    loaded &= Func6E7090();
    loaded &= Func209D2C();
    loaded &= Func294D68();
    loaded &= Func217678();
    return Func60C8A8() & loaded;
}

// 0x006D425C
bool BsMenuMgr::OpenNextBingoCard()
{
    s_Unknown950D2E = 0;
    const bool result = CreateNextMenuProcess(MENU_BINGO_CARD_1);
    if (result) {
        s_MenuArg = NO_MENU_ARG;
    }
    return result;
}

// 0x006D4288
bool BsMenuMgr::OpenBbsMgr(u32 value)
{
    s_pInstance->m_Unknown0x204 = value;
    const bool result = CreateMenuProcess(MENU_BBS_MGR, 0);
    if (!result) {
        s_pInstance->m_Unknown0x204 = 0;
    } else {
        SoundMgr::PlaySound(SE_SYS_BBS_CALL, 0.0f);
        s_Unknown950D31 = 0;
        s_Unknown950D32 = 0;
    }
    return result;
}

// 0x006D42E0
bool BsMenuMgr::OpenNextFriendChatLog()
{
    const bool result = CreateNextMenuProcess(MENU_FRIEND_CHAT_LOG);
    if (result) {
        s_MenuArg = NO_MENU_ARG;
    }
    return result;
}

// 0x006D4308
bool BsMenuMgr::Func6D4308()
{
    const bool result = CreateMenuProcess(MENU_MY_DESIGN_DREAM_PRESENT, 0);
    if (result) {
        s_Result = 2;
    }
    return result;
}

// 0x006D4334
void BsMenuMgr::CountPlanted(const SvFgName* item)
{
    if (!Func76D5AC(item)) {
        return;
    }
    SvCensus* census = GetCensus();
    switch (item->GetKind()) {
    case item::KIND_SEED:
        census->AddFlowersPlanted(1);
        break;
    case item::KIND_SEEDLING:
    case item::KIND_SEEDLING_BUSH:
    case item::KIND_PLANT:
        census->AddTreesPlanted(1);
        break;
    default:
        break;
    }
}

// 0x006D4394
bool BsMenuMgr::OpenNextBestFriendList()
{
    const bool result = CreateNextMenuProcess(MENU_BEST_FRIEND_LIST);
    if (result) {
        s_MenuArg = NO_MENU_ARG;
    }
    return result;
}

// 0x006D43BC
bool BsMenuMgr::OpenNextCommonDialogForResult()
{
    return OpenCommonDialogForResult(true);
}

// 0x006D43C4
bool BsMenuMgr::OpenCommonDialogForResult(bool isNext)
{
    s_Result = 2;
    BsCommonDialog::s_IsResultUsed = true;
    const bool result =
        isNext ? CreateNextMenuProcess(MENU_COMMON_DIALOG) : CreateMenuProcess(MENU_COMMON_DIALOG, 0);
    if (result) {
        s_MenuArg = NO_MENU_ARG;
    }
    return result;
}

// 0x006D4418
bool BsMenuMgr::OpenNextMelody1()
{
    const bool result = CreateNextMenuProcess(MENU_MELODY_1);
    if (result) {
        s_Result = 2;
    }
    return result;
}

// 0x006D4440
bool BsMenuMgr::OpenModelHomeBoard(u8 a, u32 b, u8 c)
{
    const bool result = CreateMenuProcess(MENU_MODEL_HOME_BOARD, 0);
    if (result) {
        s_Unknown950D3F = c;
        s_Unknown950D40 = a;
        s_Unknown950D80 = b;
        s_Unknown950D37 = 0;
        s_Unknown950D38 = 0;
    }
    return result;
}

// 0x006D448C
bool BsMenuMgr::Func6D448C(u32 menu, u32 arg)
{
    const bool result = CreateMenuProcess(static_cast<Menu>(menu), 0);
    if (result) {
        s_MenuArg = arg;
    }
    return result;
}

// 0x006D44B0
bool BsMenuMgr::Func6D44B0()
{
    s_Unknown950D5C = 0;
    const bool result = CreateMenuProcess(MENU_SANTA_BAG_SELECT, 0);
    if (result) {
        s_MenuArg = NO_MENU_ARG;
    }
    return result;
}

// 0x006D44E0
bool BsMenuMgr::OpenTicketExchange(u8 arg)
{
    s_Unknown950D33 = arg;
    return CreateMenuProcess(MENU_TICKET_EXCHANGE, 0);
}

// 0x006D44F8
bool BsMenuMgr::IsPassportForPhotoOpen()
{
    bool isOpen = false;
    for (ssys::st::ListNode* node = g_MenuList.GetFirst(); node != NULL; node = node->m_pNext) {
        if (static_cast<MenuBase::List*>(node)->m_pMenu->m_Index == PROC_BS_MENU_PASSPORT_FOR_PHOTO) {
            isOpen = true;
            break;
        }
    }
    return isOpen;
}

// 0x006D454C
bool BsMenuMgr::OpenNextBorrowThingList()
{
    const bool result = CreateNextMenuProcess(MENU_BORROW_THING_LIST);
    if (result) {
        s_MenuArg = NO_MENU_ARG;
    }
    return result;
}

// 0x006D4574
bool BsMenuMgr::CloseMenus()
{
    if (!Func1FE500()) {
        return false;
    }
    bool isDone = true;
    ssys::st::ListNode* node = g_MenuList.GetFirst();
    while (node != NULL) {
        MenuBase* menu = static_cast<MenuBase::List*>(node)->m_pMenu;
        node = node->m_pNext;
        if (menu == NULL) {
            continue;
        }
        // the lasting menus stay
        const u16 name = menu->m_Index;
        if (name == PROC_BS_HAND_CURSOR || name == PROC_BS_MENU_MAIN || name == PROC_BS_MENU_BG ||
            name == PROC_BS_MENU_TAB || name == PROC_BS_MENU_MAP || name == PROC_BS_MENU_ROOM_LIGHT_SWITCH ||
            name == PROC_BS_MENU_ROOM_MGR || name == PROC_BS_MENU_MODEL_HOME_BOARD) {
            continue;
        }
        const u8 state = menu->GetState();
        if (state == oml::framework::Process::STATE_CREATED ||
            oml::framework::ProcessManager::Get()->HasUninitializedDescendant(menu) ||
            state == oml::framework::Process::STATE_STOPPING || !menu->FUN_00767b30()) {
            isDone = false;
            continue;
        }
        menu->Stop();
        isDone = false;
    }
    if (s_RequestState != REQUEST_NONE) {
        isDone = false;
    }
    return isDone;
}

// 0x006D46D0
bool BsMenuMgr::RequestCancel()
{
    m_IsCancel = true;
    return true;
}

// 0x006D46E0
bool BsMenuMgr::Func6D46E0(u16 value, u8 menu, u32 arg)
{
    const bool result = CreateNextMenuProcess(static_cast<Menu>(menu));
    if (result) {
        s_Unknown950D42 = value;
        s_MenuArg = arg;
        s_SelectedSlot = 0xFF;
        s_Unknown950D36 = 0;
    }
    return result;
}

// 0x006D4724
bool BsMenuMgr::Func6D4724(u16 value, u8 menu, u32 arg)
{
    const bool result = CreateMenuProcess(static_cast<Menu>(menu), 0);
    s_PocketItemMask = 0;
    if (result) {
        s_Unknown950D42 = value;
        s_MenuArg = arg;
        s_Unknown950D36 = 1;
    }
    return result;
}

// 0x006D476C
bool BsMenuMgr::OpenPassportForPhoto()
{
    const bool result = CreateMenuProcess(MENU_PASSPORT_FOR_PHOTO, 0);
    if (result) {
        s_MenuArg = NO_MENU_ARG;
    }
    return result;
}

// 0x006D4798
bool BsMenuMgr::OpenCoolbox1()
{
    const bool result = CreateMenuProcess(MENU_COOLBOX_1, 0);
    if (result) {
        s_MenuArg = NO_MENU_ARG;
    }
    return result;
}

// 0x006D47C4
bool BsMenuMgr::OpenItemMannequin(u32 a, u32 b)
{
    const bool result = CreateMenuProcess(MENU_ITEM_MANNEQUIN, 0);
    if (result) {
        s_Unknown950D64 = a;
        s_MenuArg = b;
    }
    return result;
}

// 0x006D47F8
bool BsMenuMgr::OpenPassportForGhost(u32 arg)
{
    s_Unknown950D54 = arg;
    const bool result = CreateMenuProcess(MENU_PASSPORT_FOR_GHOST, 0);
    if (result) {
        s_MenuArg = NO_MENU_ARG;
    }
    return result;
}

// 0x006D4824
u32 BsMenuMgr::Func6D4824(u16 id)
{
    for (u32 i = 0; i < 118; i++) {
        const Unknown851F14& entry = g_Unknown851F14[i];
        if (entry.id != id) {
            continue;
        }
        switch (entry.type) {
        case 0:
            return 0;
        case 1:
            return 1;
        case 2:
        case 4:
            return 2;
        case 3:
            return 3;
        default:
            break;
        }
    }
    return 0;
}

// 0x006D4894
bool BsMenuMgr::OpenCoolbox0()
{
    const bool result = CreateMenuProcess(MENU_COOLBOX_0, 0);
    if (result) {
        s_MenuArg = NO_MENU_ARG;
    }
    return result;
}

// 0x006D48C0
bool BsMenuMgr::OpenNextPassportForFriend()
{
    const bool result = CreateNextMenuProcess(MENU_PASSPORT_FOR_FRIEND);
    if (result) {
        s_MenuArg = NO_MENU_ARG;
    }
    return result;
}

// 0x006D48E8
bool BsMenuMgr::OpenWhatTreeIsThisTree()
{
    const bool result = CreateMenuProcess(MENU_WHAT_TREE_IS_THIS_TREE, 0);
    if (result) {
        s_MenuArg = NO_MENU_ARG;
    }
    return result;
}

// 0x006D4914
bool BsMenuMgr::OpenNextPassportForPhoto()
{
    const bool result = CreateNextMenuProcess(MENU_PASSPORT_FOR_PHOTO);
    if (result) {
        s_MenuArg = NO_MENU_ARG;
    }
    return result;
}

// 0x006D493C
bool BsMenuMgr::OpenNextPassportForGhost(u32 arg)
{
    s_Unknown950D54 = arg;
    const bool result = CreateNextMenuProcess(MENU_PASSPORT_FOR_GHOST);
    if (result) {
        s_Unknown950D2B = 0;
        s_MenuArg = NO_MENU_ARG;
    }
    return result;
}

// 0x006D4970 slot 0x30
oml::framework::Result BsMenuMgr::Draw()
{
    return oml::framework::RESULT_SUCCESS;
}

// 0x006D4978 slot 0x0C
oml::framework::Result BsMenuMgr::Initialize()
{
    if (font::Mgr::s_pInstance == NULL || font::Mgr::s_pInstance->GetFont(static_cast<font::FontID>(0)) == NULL) {
        return oml::framework::RESULT_RETRY;
    }
    s_pInstance = this;
    s_RequestedMenu = MENU_NONE;
    s_RequestState = REQUEST_NONE;
    m_IsLocked = false;
    sead::Heap* heap = sead::HeapMgr::s_pInstance->getCurrentHeap();
    {
        sead::ScopedCurrentHeapSetter setter(ssys::ma::lyt::GetAllocator()->m_pHeap);
        m_pBsHandCursor = static_cast<BsHandCursor*>(ProcMgr::Create(PROC_BS_HAND_CURSOR, this, 0, PROC_TYPE_BASE));
        m_pBsMenuMain = static_cast<BsMenuMain*>(ProcMgr::Create(PROC_BS_MENU_MAIN, this, 0, PROC_TYPE_BASE));
        m_pBsMenuBg = static_cast<BsMenuBg*>(ProcMgr::Create(PROC_BS_MENU_BG, this, 0, PROC_TYPE_BASE));
        m_pBsMenuTab = static_cast<BsMenuTab*>(ProcMgr::Create(PROC_BS_MENU_TAB, this, 0, PROC_TYPE_BASE));
        m_pBsMenuMap = NULL;
        m_pRoomMgr = NULL;
        m_Unk2[0] = 0;
        m_Unk2[1] = 0;
        m_pBsMenuRandomHeadsUp = NULL;
        s_Unknown950D3D = 0;
        s_Unknown950D3E = 0;
        s_Unknown950D7C = 0;
        {
            sead::ScopedCurrentHeapSetter setter2(heap);
            m_Controller.setPadConfig(sizeof(s_PadConfig), s_PadConfig, false);
            m_Controller.setController(sead::ControllerMgr::s_pInstance->getController(0), true);
            m_Controller.setPadRepeat(PAD_REPEAT_MASK, 20, 5);
            m_HandMode = 0;
            m_HandGrabTimer = 14;
            m_CurrentActionNodeIndex = 0;
            m_Unknown0x42 = 0;
            m_Unknown0x41 = 0;
            m_Unknown0x3A = false;
            m_Unknown0x3B = false;
            m_Unknown0x3C = false;
            m_Unknown0x3D = false;
            m_Unknown0x3E = false;
            s_Flags = (s_Flags & ~FLAG_MAP_2) | FLAG_MAP_4;
            g_Unknown98D508[0] = UNKNOWN98D508_UNSET;
            g_Unknown98D508[1] = UNKNOWN98D508_UNSET;
            g_Unknown98D508[2] = UNKNOWN98D508_UNSET;
            Func6D32D0();
            Start("BsMenuMgr", s_BsMenuMgrStateInfos, 2);
        }
    }
    return oml::framework::RESULT_SUCCESS;
}

// 0x006D4C24 | libgarden [tier A]
bool BsMenuMgr::IsPressed(unsigned long input)
{
    if (applet::ErrEulaMgr::IsInputBlocked()) {
        return false;
    }
    if ((input & INPUT_B) != 0) {
        // the end of a tour cancels as well
        if ((stage::IsKind(stage::KIND_TOUR, stage::GetCurrent()) ||
             stage::IsKind(stage::KIND_ISLAND_HUT, stage::GetCurrent())) &&
            netgame::NetGameMgr::s_pInstance != NULL &&
            netgame::NetGameMgr::s_pInstance->m_LockMgr.IsWaitingForCmd(netgame::LockMgr::CMD_TOUR_END)) {
            return true;
        }
        if (m_IsCancel) {
            return true;
        }
    }
    return (m_Controller.mPadTrig & input) != 0;
}

// 0x006D4CC4
void BsMenuMgr::Func6D4CC4()
{
    if (s_pInstance != NULL) {
        s_pInstance->m_Unknown0x41 = 0;
    }
    Func6D32D0();
}

// 0x006D4CE0 slot 0x24
oml::framework::Result BsMenuMgr::Calc()
{
    if (m_IsCancel) {
        g_TouchState.Func1B78E8();
    }
    m_IsBgShown = false;
    m_IsMapShown = false;
    BsScene* scene = static_cast<BsScene*>(BsScene::s_ChangeRequest.pScene);
    if (scene != NULL && scene->vf_0x40() == oml::framework::RESULT_SUCCESS) {
        if (stage::IsKind(stage::KIND_OUTDOOR, stage::GetCurrent()) ||
            stage::IsKind(stage::KIND_INDOOR, stage::GetCurrent())) {
            m_IsBgShown = true;
            m_IsMapShown = true;
        }
        const stage::Name name = stage::GetCurrent();
        if (name == stage::NAME_TRAIN) {
            m_IsMapShown = true;
        } else if (name == stage::NAME_AMIIBO_CAMERA) {
            m_IsBgShown = false;
            m_IsMapShown = true;
        } else if (name == stage::NAME_PLAYER_SELECT || name == stage::NAME_TITLE || name == stage::NAME_UPDATE_SELECT) {
            m_IsBgShown = false;
            m_IsMapShown = false;
        }
        UpdateMap();
        UpdateRoomMgr();
        if (demo::CheckOrder(demo::INTERACT, true) || demo::CheckOrder(demo::DEFAULT_TALK, true) ||
            s_pInstance->m_Unknown0x41 == 1) {
            if (m_Unknown0x42 == 0) {
                m_Unknown0x42 = 1;
            } else if (m_Unknown0x42 == 1) {
                m_Unknown0x42 = 2;
            } else if (m_Unknown0x42 == 3) {
                m_Unknown0x42 = 0;
            }
        } else if (m_Unknown0x42 == 2) {
            m_Unknown0x42 = 3;
        } else if (m_Unknown0x42 == 3) {
            m_Unknown0x42 = 0;
        }
        if ((netgame::GetNetgameType() == netgame::TYPE_RANDOM_MATCH ||
             netgame::NetGameMgr::s_pInstance->m_IsConnectRandomMatch) &&
            stage::GetCurrent() == stage::NAME_SEA_DEPARTURE) {
            if (!m_IsRandomHeadsUpCreated) {
                sead::ScopedCurrentHeapSetter setter(ssys::ma::lyt::GetAllocator()->m_pHeap);
                m_pBsMenuRandomHeadsUp = static_cast<BsMenuRandomHeadsUp*>(
                    ProcMgr::Create(PROC_BS_MENU_RANDOM_HEADS_UP, this, 0, PROC_TYPE_BASE));
                m_IsRandomHeadsUpCreated = true;
            }
        } else if (m_pBsMenuRandomHeadsUp != NULL && !Func566E5C()) {
            if (!((g_pUnknown976400 != NULL && g_pUnknown976400->GetState() == 2) ||
                  (g_pUnknown976404 != NULL && g_pUnknown976404->GetState() == 2) ||
                  (g_pUnknown976400 != NULL && g_pUnknown976400->GetState() == 3) ||
                  (g_pUnknown976404 != NULL && g_pUnknown976404->GetState() == 3))) {
                m_pBsMenuRandomHeadsUp->StopSelf();
                m_pBsMenuRandomHeadsUp = NULL;
                m_IsRandomHeadsUpCreated = false;
            }
        }
    } else {
        // no stage: the map and the room go
        if (m_pBsMenuMap != NULL) {
            m_pBsMenuMap->StopSelf();
            s_pInstance->m_pBsMenuMap = NULL;
            s_Flags = (s_Flags & ~FLAG_MAP_2) | FLAG_MAP_4;
            m_Unknown0x3B = false;
            m_Unknown0x3C = false;
        }
        if (m_pRoomMgr != NULL) {
            m_pRoomMgr->StopSelf();
            s_pInstance->m_pRoomMgr = NULL;
            s_Flags = (s_Flags & ~FLAG_MAP_2) | FLAG_MAP_4;
            m_Unknown0x3D = false;
            m_Unknown0x3E = false;
            s_Unknown950D34 = 0;
        }
    }
    if (m_pBsMenuMap != NULL && g_Unknown950C30 != 0) {
        m_pBsMenuMap->Func6D2590(m_IsMapShown);
    }
    if (m_pRoomMgr != NULL) {
        m_pRoomMgr->Func225828(m_IsMapShown);
    }
    if (stage::GetCurrent() != stage::NAME_AMIIBO_CAMERA) {
        m_pBsMenuBg->m_IsShown = m_IsBgShown ? true : false;
    }
    m_pBsMenuTab->m_IsBgShown = m_IsBgShown;
    m_pBsHandCursor->m_HandMode = m_HandMode;
    Call();
    return oml::framework::RESULT_SUCCESS;
}

// 0x006D5158 slot 0x18
oml::framework::Result BsMenuMgr::Finalize()
{
    if (m_pBsMenuMap != NULL) {
        m_pBsMenuMap->StopSelf();
    }
    m_pBsMenuMain->StopSelf();
    m_pBsMenuBg->StopSelf();
    m_pBsHandCursor->StopSelf();
    if (m_pBsMenuRandomHeadsUp != NULL) {
        m_pBsMenuRandomHeadsUp->StopSelf();
    }
    s_pInstance = NULL;
    return oml::framework::RESULT_SUCCESS;
}

// 0x006D51A8
bool BsMenuMgr::IsMenuIdle()
{
    return IsIdle();
}

// 0x006D51E4
void BsMenuMgr::Func6D51E4()
{
    s_Unknown950D24 = 11;
    BsMenuTab* tab = s_pInstance->m_pBsMenuTab;
    if (tab != NULL) {
        tab->Func6D93D0();
    }
}

// 0x006D520C
bool BsMenuMgr::Func6D520C()
{
    if ((stage::IsKind(stage::KIND_TOUR, stage::GetCurrent()) ||
         stage::IsKind(stage::KIND_ISLAND_HUT, stage::GetCurrent())) &&
        netgame::NetGameMgr::s_pInstance != NULL &&
        netgame::NetGameMgr::s_pInstance->m_LockMgr.IsWaitingForCmd(netgame::LockMgr::CMD_TOUR_END)) {
        return true;
    }
    return false;
}

// 0x006D5274
void BsMenuMgr::SetDigits(u32 value)
{
    u8* digits = reinterpret_cast<u8*>(&s_Unknown950D70);
    digits[0] = value / 1000;
    value %= 1000;
    digits[1] = value / 100;
    value %= 100;
    digits[2] = value / 10;
    digits[3] = value % 10;
}

// 0x006D52B4
BsMenuMgr::BsMenuMgr()
    : m_IsBgShown(false), m_IsMapShown(false), m_Unknown0x3A(false), m_Unknown0x3B(false), m_Unknown0x3C(false),
      m_Unknown0x3D(false), m_Unknown0x3E(false), m_IsRandomHeadsUpCreated(false), m_HandMode(0), m_Unknown0x41(0),
      m_Unknown0x42(0), m_pBsMenuMain(NULL), m_pBsMenuBg(NULL), m_pBsMenuTab(NULL), m_pBsMenuMap(NULL), m_pRoomMgr(NULL),
      m_pBsHandCursor(NULL), m_pBsMenuRandomHeadsUp(NULL), m_HandGrabTimer(14), m_CurrentActionNodeIndex(0),
      m_IsLocked(false), m_Unknown0x200(0), m_Unknown0x204(0), m_IsCancel(false)
{
    m_Unk2[0] = 0;
    m_Unk2[1] = 0;
}

// 0x006D53A4 slot 0x00
// 0x006D5378 (deleting dtor)
// 0x00775DD0 (thunk)
// 0x00775DA4 (deleting thunk)
BsMenuMgr::~BsMenuMgr()
{
    // (only the members are destroyed)
}
