#include "Bs/dBsSkb.h"

#include "Bs/dBsCommonDialog.h"
#include "Bs/dBsMenuMgr.h"
#include "Bs/dBsSvMgr.h"
#include "Audio/bgm/dBgmTrackMixer.h"
#include "Button/dButtonActionControl.h"
#include "Other/dComDialogA.h"
#include "Other/dComButton1Lyt.h"
#include "Other/dComButton2Lyt.h"
#include "Other/dComButton3Lyt.h"
#include "Other/dHeapInit.h"
#include "Other/dPaneVisible.h"
#include "Other/dRegion.h"
#include "Bs/dBsMenuMgrExtern.h"
#include "lyt/dTextAreaEx.h"
#include "gui/dTextAreaEx.h"
#include "Sound/dSoundMgr.h"
#include "Sound/sound_Extern.h"
#include "font/dMgr.h"
#include "gui/dButtonEx2.h"
#include "gui/dCellPhoneKeySet.h"
#include "gui/dGridKeySet.h"
#include "gui/dKanaKeySet.h"
#include "gui/dQwertyKeySet.h"
#include "gui/dRadioButton.h"
#include "gui/dRepeatButton.h"
#include "gui/dTextArea.h"
#include "gui/dTextAreaBoard.h"
#include "gui/dTextAreaMail.h"
#include "gui/dWidget.h"
#include "lyt/dAnmObject.h"
#include "lyt/dObject.h"
#include "lyt/dTextAreaBoard.h"
#include "lyt/dTextAreaMail.h"
#include "lyt/dTextAreaOneLineBirth.h"
#include "lyt/dTextAreaOneLineChat.h"
#include "lyt/dTextAreaOneLineComTitle.h"
#include "lyt/dTextAreaOneLineComment.h"
#include "lyt/dTextAreaOneLineFriend.h"
#include "netgame/netgame_Api.h"
#include "nw/lyt/lyt_Pane.h"
#include "nw/ut/ut_Api.h"
#include "script/dMgr.h"
#include "script/dTalkSystem.h"
#include "sead/seadExpHeap.h"
#include "sead/seadHeap.h"
#include "sead/seadNew.h"
#include "lyt/dObjectEx.h"
#include "sead/seadHeapMgr.h"
#include "ssys/ma/dAllocator.h"
#include "ssys/ma/lyt/lyt_Api.h"
#include "stage/stage_Api.h"
#include "sys/dSwkbd.h"
#include "sys/dTextManager.h"

// 0x00951658
BsSkb* BsSkb::s_pInstance;
// 0x0095165C
const char* BsSkb::s_GridArrowGroups[2] = {"G_arwL_00", "G_arwR_00"};
// 0x00951664
const char* BsSkb::s_PageLabels[10] = {
    "sign_keytop_00", "sign_keytop_01", "sign_keytop_02", "sign_keytop_03", "euro_keytop_00",
    "euro_keytop_01", "euro_keytop_02", "euro_keytop_03", "euro_keytop_04", "euro_keytop_05",
};

namespace {
// the flags of sys::KeyboardParam that ask for a check of the text (names are ours, after what BsSkb does)
const u32 PARAM_FLAG_CHECK_TEXT = 1 << 4;     // the UGC check of the text
const u32 PARAM_FLAG_CHECK_NUMBERS = 1 << 5;  // the numbers in the text are counted
// the label file of the dialogs and the buttons
const char* const MESSAGE_FILE = "SYS_2D_UI";
// the labels of the buttons and of the errors (SYS_2D_UI)
const s32 LABEL_CANCEL = 76;
const s32 LABEL_OK = 79;
const s32 LABEL_SEND = 82;
const s32 LABEL_PREVIEW = 70;
const s32 LABEL_REWRITE = 71;
const s32 LABEL_ERROR = 87;
const s32 LABEL_NG_NUMBERS = 349;
const s32 LABEL_NG_NUMBER_IN_COMMENT = 350;
const s32 LABEL_NG_TEXT = 399;
const s32 LABEL_CANCEL_CONFIRM = 288;
const s32 LABEL_BOARD_CANCEL_CONFIRM = 180;
// the stage on which the key sets are read into the stage heap
const stage::Name STAGE_KEY_SET_IN_STAGE_HEAP = static_cast<stage::Name>(63);
// the frames of the page arrows of the grid
const s32 GRID_ARROW_REPEAT_DELAY = 20;
const s32 GRID_ARROW_REPEAT_INTERVAL = 10;

// the key set from the mode of the settings (KeyboardSetting::mode)
inline s32 GetKeySetOfMode(s32 mode)
{
    switch (mode) {
    case 1:
        return BsSkb::KEY_SET_KANA;
    case 2:
    case 3:
        return BsSkb::KEY_SET_GRID;
    case 4:
        return BsSkb::KEY_SET_CELL_PHONE;
    default:
        return BsSkb::KEY_SET_QWERTY;
    }
}

// the letter is written on the paper with the extra frame (lyt::TextAreaMail)
inline bool IsLetterWithFrame()
{
    return sys::g_Unknown95F0EB == BsSkb::KIND_LETTER &&
           lyt::Func75910C(reinterpret_cast<SvMail*>(lyt::g_pUnknown950BD0 + 0x118));
}

// the object of the game behind lyt::g_pUnknown950BD0 (+0x50) and its methods (not decompiled; names are ours)
inline u8* GetLetterMenu()
{
    return *reinterpret_cast<u8**>(lyt::g_pUnknown950BD0 + 0x50);
}

// an object of the heap is destroyed and freed
template <typename T>
inline void DeleteFromHeap(sead::Heap* heap, T*& obj)
{
    obj->~T();
    heap->free(obj);
    obj = NULL;
}
} // namespace

// the debug object of the letter menu (game code, not decompiled; names are ours)
void Func2DA908(u8* letterMenu); // 0x002DA908
void Func2DA978(u8* letterMenu); // 0x002DA978

// the states with their names (debug); the machine starts with the key set
// 0x00855370
const BsSkb::StateInfo BsSkb::s_StateInfos[28] = {
    {"window in", &BsSkb::Enter_WindowIn, &BsSkb::State_WindowIn},
    {"window out", &BsSkb::Enter_WindowOut, &BsSkb::State_WindowOut},
    {"button in", &BsSkb::Enter_ButtonIn, &BsSkb::State_ButtonIn},
    {"button out", &BsSkb::Enter_ButtonOut, &BsSkb::State_ButtonOut},
    {"wait", &BsSkb::Enter_Wait, &BsSkb::State_Wait},
    {"return wait", &BsSkb::Enter_ReturnWait, &BsSkb::State_ReturnWait},
    {"ng check", &BsSkb::Enter_NgCheck, &BsSkb::State_NgCheck},
    {"ng check", &BsSkb::Enter_NgCheck2, &BsSkb::State_NgCheck2},
    {"ng error", &BsSkb::Enter_NgError, &BsSkb::State_NgError},
    {"ng error wait", &BsSkb::Enter_NgErrorWait, &BsSkb::State_NgErrorWait},
    {"write", &BsSkb::Enter_Write, &BsSkb::State_Write},
    {"button wait", &BsSkb::Enter_ButtonWait, &BsSkb::State_ButtonWait},
    {"waite cancel", &BsSkb::Enter_WaitCancel, &BsSkb::State_WaitCancel},
    {"waite cancel", &BsSkb::Enter_WaitCancel2, &BsSkb::State_WaitCancel2},
    {"preview1", &BsSkb::Enter_Preview1, &BsSkb::State_Preview1},
    {"preview2", &BsSkb::Enter_Preview2, &BsSkb::State_Preview2},
    {"preview3", &BsSkb::Enter_Preview3, &BsSkb::State_Preview3},
    {"preview", &BsSkb::Enter_Preview, &BsSkb::State_Preview},
    {"preview", &BsSkb::Enter_PreviewEnd, &BsSkb::State_PreviewEnd},
    {"preview", &BsSkb::Enter_PreviewCheck, &BsSkb::State_PreviewCheck},
    {"rewrite0", &BsSkb::Enter_Rewrite0, &BsSkb::State_Rewrite0},
    {"rewrite1", &BsSkb::Enter_Rewrite1, &BsSkb::State_Rewrite1},
    {"rewrite2", &BsSkb::Enter_Rewrite2, &BsSkb::State_Rewrite2},
    {"rewrite3", &BsSkb::Enter_Rewrite3, &BsSkb::State_Rewrite3},
    {"melody play", &BsSkb::Enter_MelodyPlay, &BsSkb::State_MelodyPlay},
    {"keyset load", &BsSkb::Enter_KeySetLoad, &BsSkb::State_KeySetLoad},
    {"warninig wait", &BsSkb::Enter_WarningWait, &BsSkb::State_WarningWait},
    {"idle", &BsSkb::Enter_Idle, &BsSkb::State_Idle},
};

// 0x0057C900 slot 0x44
void BsSkb::OnClose()
{
    SetInputEnabled(true);
    if (sys::g_Unknown95F0EB == KIND_LETTER) {
        sys::g_pTextManager->CommitAll(true);
        static_cast<lyt::TextAreaMail*>(m_pTextArea)->StoreText(false);
    }
    m_Unknown0x11C5 = true;
    if (::state::Mode<BsSkb>::GetState() != &BsSkb::State_KeySetLoad) {
        Change(&BsSkb::State_ButtonOut);
    }
}

// 0x0057C9A8
bool BsSkb::ChangeKeySet(s32 kind)
{
    sead::ScopedCurrentHeapSetter setter(m_pHeap);
    if (m_KeySetKind != KEY_SET_QWERTY) {
        m_IsLabel16 = false;
        m_IsLabel27 = false;
        if (sys::g_Unknown95F0EB == KIND_17 || sys::g_Unknown95F0EB == KIND_18) {
            m_IsLabel16 = true;
        }
    } else {
        const s32 label = m_pKeySet->m_Label;
        m_IsLabel16 = label == 1 || label == 6;
        m_IsLabel27 = label == 2 || label == 7;
    }
    if (m_pKeySet != NULL) {
        // (deleted with the lock of the heap held once)
        sead::CriticalSection* cs = &m_pHeap->mCS;
        cs->lock();
        const bool isEnableLock = (m_pHeap->mFlag & sead::Heap::FLAG_ENABLE_LOCK) != 0;
        m_pHeap->mFlag &= ~sead::Heap::FLAG_ENABLE_LOCK;
        gui::KeySet* keySet = m_pKeySet;
        keySet->~KeySet();
        m_pHeap->free(keySet);
        if (isEnableLock) {
            m_pHeap->mFlag |= sead::Heap::FLAG_ENABLE_LOCK;
        } else {
            m_pHeap->mFlag &= ~sead::Heap::FLAG_ENABLE_LOCK;
        }
        m_pKeySet = NULL;
        cs->unlock();
    }
    m_Unknown0x11BC = -1;
    m_PageIndex = -1;
    m_pPages = NULL;
    m_pQwerty = NULL;
    m_pKana = NULL;
    m_pGrid = NULL;
    m_pCellPhone = NULL;
    sys::KeyboardSetting& setting = sys::g_KeyboardSetting;
    switch (kind) {
    case KEY_SET_QWERTY:
        m_pQwerty = new (m_pHeap, 4) gui::QwertyKeySet(&m_KeyReader);
        if (setting.mode == 0) {
            if (m_IsLabel16) {
                m_Unknown0x11BC = 1;
            } else if (m_IsLabel27) {
                m_Unknown0x11BC = 2;
            } else {
                m_Unknown0x11BC = m_pQwerty->GetFirstLabel(setting.kanaMode);
            }
        } else if (setting.mode == 5) {
            if (m_IsLabel16) {
                m_Unknown0x11BC = 6;
            } else if (m_IsLabel27) {
                m_Unknown0x11BC = 7;
            } else {
                m_Unknown0x11BC = 5;
            }
        } else {
            setting.mode = 0;
            m_Unknown0x11BC = 0;
        }
        if (m_pQwerty->Initialize(m_Unknown0x11BC)) {
            m_pKeySet = m_pQwerty;
        }
        break;
    case KEY_SET_KANA:
        m_pKana = new (m_pHeap, 4) gui::KanaKeySet(&m_KeyReader);
        if (m_pKana->Initialize(setting.kanaKeySet)) {
            m_pKeySet = m_pKana;
        }
        break;
    case KEY_SET_GRID:
        if (setting.mode == 3) {
            m_PageIndex = setting._unknown0x14;
            m_pPages = &m_CellPhonePageNum;
        } else {
            if (setting.mode != 2) {
                setting.mode = 2;
            }
            m_pPages = &m_GridPageNum;
            m_PageIndex = setting._unknown0x18;
        }
        m_pGrid = new (m_pHeap, 4) gui::GridKeySet(&m_KeyReader);
        if (m_pGrid->Initialize(m_pPages[1 + m_PageIndex])) {
            m_pGrid->SetPage(m_PageIndex, *m_pPages);
            m_pKeySet = m_pGrid;
            m_IsShifted = false;
        }
        break;
    case KEY_SET_CELL_PHONE:
        m_pCellPhone = new (m_pHeap, 4) gui::CellPhoneKeySet(&m_KeyReader);
        if (m_pCellPhone->Initialize(m_pCellPhone->GetLowerLabel(setting.cellPhoneKeySet))) {
            m_pKeySet = m_pCellPhone;
        }
        break;
    }
    // the page arrows of the grid come in or go out
    if (m_KeySetKind == KEY_SET_GRID) {
        if (kind != KEY_SET_GRID) {
            for (s32 i = 0; i < 2; i++) {
                m_pGridArrowAnims[i]->ApplyFrame(m_pGridArrowAnims[i]->m_StartFrame);
            }
            m_pGridArrow->m_State = lyt::Object::STATE_CHANGED;
        }
    } else if (kind == KEY_SET_GRID) {
        for (s32 i = 0; i < 2; i++) {
            m_pGridArrowAnims[i]->ApplyFrame(m_pGridArrowAnims[i]->m_EndFrame);
        }
        m_pGridArrow->m_State = lyt::Object::STATE_CHANGED;
    }
    for (s32 i = 0; i < 2; i++) {
        m_pGridArrowButtons[i]->SetActive(kind == KEY_SET_GRID);
    }
    sys::g_pTextManager->m_PendingAccent = 0;
    m_KeySetKind = kind;
    if (m_pKeySet != NULL) {
        m_pKeySet->GetLayout()->m_State = lyt::Object::STATE_CHANGED;
    }
    return true;
}

// 0x0057CE3C
void BsSkb::State_Idle()
{
    // empty (in the original too)
}

// 0x0057CE40
void BsSkb::State_Wait()
{
    m_Result = -1;
    const bool isAcceptable = sys::IsTextAcceptable();
    bool isDecided = false;
    if (m_pButton1 != NULL) {
        ButtonActionControl* control = &m_pButton1->m_Control;
        if (control->IsSelectOkDone() || control->IsSelectOkOngoing()) {
            if (m_IsResultUsed) {
                BsMenuMgr::s_Result = 1;
            }
            m_Result = 0;
            sys::g_pTextManager->CommitAll(true);
            isDecided = true;
        } else if (m_IsTextAcceptable != isAcceptable) {
            m_pButton1->m_Control.SetButtonDisabled(0, !isAcceptable);
            m_IsTextAcceptable = isAcceptable;
        }
    } else if (m_pButton2 != NULL) {
        ButtonActionControl* control = &m_pButton2->m_Control;
        if (control->IsSelectOkDone() || control->IsSelectOkOngoing()) {
            switch (m_pButton2->m_Control.GetSelectedIndex()) {
            case 0:
                if (m_IsResultUsed) {
                    BsMenuMgr::s_Result = 0;
                }
                m_Result = 1;
                sys::SetFinished(true, 1);
                break;
            case 1:
                if (m_IsResultUsed) {
                    BsMenuMgr::s_Result = 1;
                }
                m_Result = 2;
                sys::g_pTextManager->CommitAll(true);
                break;
            }
            if (IsLetterWithFrame()) {
                u8* letterMenu = GetLetterMenu();
                *reinterpret_cast<u32*>(letterMenu + 0x6B4) |= 8;
                reinterpret_cast<ButtonActionControl*>(letterMenu + 0x5E0)->ResetSelectAll();
            }
            isDecided = true;
        } else if (m_IsTextAcceptable != isAcceptable) {
            m_pButton2->m_Control.SetButtonDisabled(1, !isAcceptable);
            m_IsTextAcceptable = isAcceptable;
        }
    } else if (m_pButton3 != NULL) {
        ButtonActionControl* control = &m_pButton3->m_Control;
        if (control->IsSelectOkDone() || control->IsSelectOkOngoing()) {
            if (m_IsResultUsed) {
                BsMenuMgr::s_Result = m_pButton3->m_Control.GetDecidedIndex();
            }
            if (m_pButton3->m_Control.GetDecidedIndex() == 1) {
                m_Result = 5;
                sys::g_pTextManager->CommitAll(true);
                m_pButton3->m_Control.UnselectActive();
                m_pButton3->m_Control.Reset();
            } else if (m_pButton3->m_Control.GetDecidedIndex() == 5) {
                m_Result = 4;
                m_pButton3->m_Control.UnselectActive();
                m_pButton3->m_Control.Reset();
            } else {
                m_Result = 3;
                sys::SetFinished(true, 3);
            }
            isDecided = true;
        } else if (m_IsTextAcceptable != isAcceptable) {
            m_pButton3->m_Control.SetButtonDisabled(2, !isAcceptable);
            m_IsTextAcceptable = isAcceptable;
        }
    }
    if (isDecided) {
        SetInputEnabled(true);
        Change(&BsSkb::State_ReturnWait);
    }
    if (sys::g_Unknown95F0EB == KIND_17 || sys::g_Unknown95F0EB == KIND_18) {
        m_pTextArea->vf_0x18();
    }
}

// 0x0057D1B0
void BsSkb::Enter_Idle()
{
    // empty (in the original too)
}

// 0x0057D1B4
void BsSkb::Enter_Wait()
{
    // empty (in the original too)
}

// 0x0057D1B8
void BsSkb::CreateTextArea()
{
    f32 titleFrame = -1.0f;
    sead::Heap* heap = m_pHeap;
    switch (sys::g_Unknown95F0EB) {
    case KIND_CHAT:
        m_pTextArea = new (heap, 4) lyt::TextAreaOneLineChat(0, 500, NULL);
        m_pTextArea->Setup(&m_Reader, &m_CommonReader);
        m_pTextInAnim = m_pTextArea->CreateAnmObject("key16_chat_back.bclan", "G_All");
        m_pTextOutAnim = m_pTextArea->CreateAnmObject("key16_chat_sent.bclan", "G_All");
        break;
    case KIND_BIRTHDAY:
        m_pTextArea = new (heap, 4) lyt::TextAreaOneLineBirth(0, 500, NULL);
        m_pTextArea->Setup(&m_Reader, &m_CommonReader);
        break;
    case KIND_COMMENT:
        m_pTextArea = new (heap, 4) lyt::TextAreaOneLineComment(0, 500, NULL);
        m_pTextArea->Setup(&m_Reader, &m_CommonReader);
        break;
    case KIND_FRIEND:
        m_pTextArea = new (heap, 4) lyt::TextAreaOneLineFriend(0, 500, NULL);
        m_pTextArea->Setup(&m_Reader, &m_CommonReader);
        break;
    case 4:
    case 6:
        titleFrame = 5.0f;
        break;
    case 5:
        titleFrame = 3.0f;
        break;
    case 7:
        titleFrame = 0.0f;
        break;
    case 8:
        titleFrame = 1.0f;
        break;
    case 10:
        titleFrame = 13.0f;
        break;
    case 11:
    case 12:
    case KIND_17:
        titleFrame = 17.0f;
        break;
    case 13:
        titleFrame = 6.0f;
        break;
    case 9:
    case 14:
        titleFrame = 30.0f;
        break;
    case KIND_18:
        titleFrame = 38.0f;
        break;
    case KIND_LETTER: {
        m_pTextArea = new (heap, 4) lyt::TextAreaMail(0, 500, NULL);
        m_pTextArea->Setup(&m_Reader, &m_CommonReader);
        m_pTextInAnim = m_pTextArea->CreateAnmObject("ltr_base_in.bclan", "G_ltr_base");
        m_pTextOutAnim = m_pTextArea->CreateAnmObject("ltr_base_out.bclan", "G_ltr_base");
        m_HasTextAnim = true;
        m_pArrow = new (m_pHeap, 4) lyt::Object(0, 500, NULL);
        m_pArrow->Build(&m_CommonReader, "Key_arrow.bclyt", 0x4000);
        // the melody player of the town tune
        m_pMelody = new (m_pHeap, 4) lyt::Object(0, lyt::Object::PRIORITY_NO_ANIMATE, NULL);
        m_pMelody->Build(&m_Reader, "mld_ply.bclyt", 0x4000);
        m_pMelodyInAnim = m_pMelody->CreateAnmObject("mld_ply_in.bclan", "G_inout");
        m_pMelodyOutAnim = m_pMelody->CreateAnmObject("mld_ply_out.bclan", "G_inout");
        gui::ButtonEx2::Description desc;
        desc.pLayout = m_pMelody;
        desc.groupName = "G_btn_00";
        desc.anims[gui::ButtonBase::ANIM_PRESS] = m_pMelody->CreateAnmObject("mld_ply_select.bclan", "G_btn_00");
        desc.anims[gui::ButtonBase::ANIM_SLIDE_OUT] = NULL;
        desc.anims[gui::ButtonBase::ANIM_DECIDE] = m_pMelody->CreateAnmObject("mld_ply_select_ok.bclan", "G_btn_00");
        desc.anims[gui::ButtonBase::ANIM_RELEASE] = NULL;
        desc.exAnims[0] = m_pMelody->CreateAnmObject("mld_ply_touch.bclan", "G_btn_00");
        desc.exAnims[2] = m_pMelody->CreateAnmObject("mld_ply_touch_ok.bclan", "G_btn_00");
        desc.exAnims[1] = NULL;
        desc.anims[gui::ButtonBase::ANIM_DISABLE] = m_pMelody->CreateAnmObject("mld_ply_ng.bclan", "G_btn_00");
        desc.sounds[gui::ButtonBase::SOUND_PRESS] = SE_SYS_MELODY_BTN_ACTIVE;
        desc.sounds[gui::ButtonBase::SOUND_DECIDE] = SE_SYS_MELODY_BTN_SELECTED;
        m_pMelodyButton = new (m_pHeap, 4) gui::ButtonEx2(desc);
        m_pMelodyButton->SetEnabled(false, true, false);
        m_pTextArea->AttachChild("N_btn_pos_00", m_pMelody);
        m_pMelodyInAnim->ApplyLimitFrame(true);
        break;
    }
    default: {
        m_pTextArea = new (heap, 4) lyt::TextAreaBoard(0, 500, NULL);
        m_pTextArea->Setup(&m_Reader, &m_CommonReader);
        m_pTextInAnim = m_pTextArea->CreateAnmObject("board_paper_B_in.bclan", "G_inout");
        m_pTextOutAnim = m_pTextArea->CreateAnmObject("board_paper_B_out.bclan", "G_inout");
        m_HasTextAnim = true;
        m_pArrow = new (m_pHeap, 4) lyt::Object(0, 500, NULL);
        m_pArrow->Build(&m_CommonReader, "Key_arrow.bclyt", 0x4000);
        break;
    }
    }
    if (titleFrame >= 0.0f) {
        m_pTextArea = new (m_pHeap, 4) lyt::TextAreaOneLineComTitle(0, 500, NULL);
        m_pTextArea->Setup(&m_Reader, &m_CommonReader);
        m_pTextInAnim = m_pTextArea->CreateAnmObject("cmn_title_win_In_msg_win.bclan", "A00_win_msg");
        m_pTextOutAnim = m_pTextArea->CreateAnmObject("cmn_title_win_Out_msg_win.bclan", "A00_win_msg");
        m_pTitleMsgPosAnim = m_pTextArea->CreateAnmObject("cmn_title_win_msg_pos.bclan", "G_msg");
        m_pTitleMsgPosAnim->ApplyFrame(1.0f);
        m_pTitleIconAnim = m_pTextArea->CreateAnmObject("cmn_title_win_Pattern_chara_icon.bclan", "B00_icn");
        m_pTitleIconAnim->ApplyFrame(titleFrame);
        m_HasTextAnim = true;
    }
    if (sys::g_Unknown95F0EB == KIND_LETTER) {
        gui::TextAreaMail* widget = new (m_pHeap, 4) gui::TextAreaMail;
        m_pPageWidget = widget;
        widget->Initialize(static_cast<lyt::TextAreaEx*>(m_pTextArea), m_pArrow);
    } else if (sys::g_Unknown95F0EB == KIND_BOARD) {
        gui::TextAreaBoard* widget = new (m_pHeap, 4) gui::TextAreaBoard;
        m_pPageWidget = widget;
        widget->Initialize(static_cast<lyt::TextAreaEx*>(m_pTextArea), m_pArrow);
        m_pPageWidget->DeleteBottomTouch();
    } else {
        m_pTextWidget = new (m_pHeap, 4) gui::TextArea;
        m_pTextWidget->Initialize(m_pTextArea);
    }
}

// 0x0057DCF8
void BsSkb::State_Write()
{
    const s8 isScrolling = static_cast<lyt::TextAreaEx*>(m_pTextArea)->m_IsScrolling;
    if (m_IsWriting != isScrolling) {
        SetInputEnabled(isScrolling);
        m_IsWriting = isScrolling;
        if (isScrolling) {
            if (m_pButton1 != NULL) {
                m_pButton1->m_Control.ResetSelectAll();
            }
            if (m_pButton2 != NULL) {
                m_pButton2->m_Control.ResetSelectAll();
            }
            if (m_pButton3 != NULL) {
                m_pButton3->m_Control.ResetSelectAll();
            }
        }
    }
    if (m_pButton2->m_Control.IsSelectingOk()) {
        SetInputEnabled(true);
        static_cast<lyt::TextAreaEx*>(m_pTextArea)->m_Unknown0x73A = true;
        if (IsLetterWithFrame()) {
            u8* letterMenu = GetLetterMenu();
            *reinterpret_cast<u32*>(letterMenu + 0x6B4) |= 8;
            reinterpret_cast<ButtonActionControl*>(letterMenu + 0x5E0)->ResetSelectAll();
        }
        Change(&BsSkb::State_ButtonWait);
    }
    if (sys::g_pTextManager->m_IsFull == true) {
        Change(&BsSkb::State_WarningWait);
    }
}

// 0x0057DE30
void BsSkb::Enter_Write()
{
    BsMenuMgr::s_Flags |= 0x80;
    SetInputEnabled(false);
    if (IsLetterWithFrame()) {
        u8* letterMenu = GetLetterMenu();
        *reinterpret_cast<u32*>(letterMenu + 0x6B4) &= ~8;
        reinterpret_cast<ButtonActionControl*>(letterMenu + 0x5E0)->ResetSelectAll();
    }
    m_IsWriting = static_cast<lyt::TextAreaEx*>(m_pTextArea)->m_IsScrolling;
}

// 0x0057DEBC
void BsSkb::SelectKeySetButton(s32 kind)
{
    if (GetRegionIndex() != 0) {
        switch (kind) {
        case 0:
            m_pKeySetButtons[0]->Select(false);
            break;
        case 2:
            m_pKeySetButtons[2]->Select(false);
            break;
        case 3:
            m_pKeySetButtons[1]->Select(false);
            break;
        case 4:
            m_pKeySetButtons[3]->Select(false);
            break;
        default:
            kind = 0;
            m_pKeySetButtons[0]->Select(false);
            break;
        }
    } else if (kind == 0) {
        m_pKeySetButtons[0]->Select(false);
    } else if (kind == 2) {
        m_pKeySetButtons[2]->Select(false);
    } else if (kind == 4) {
        m_pKeySetButtons[3]->Select(false);
    } else {
        if (kind != 1) {
            kind = 1;
        }
        m_pKeySetButtons[1]->Select(false);
    }
    m_pKeySetSelect->m_State = lyt::Object::STATE_CHANGED;
    sys::g_KeyboardSetting.mode = kind;
}

// 0x0057DF94
void BsSkb::State_Preview()
{
    if (m_IsMelodyPlaying == true) {
        Change(&BsSkb::State_MelodyPlay);
        return;
    }
    ButtonActionControl* control = &m_pButton2->m_Control;
    if (control->IsSelectOkDone() || (control->m_pFunc == &ButtonActionControl::State_Selecting)) {
        Change(&BsSkb::State_Rewrite0);
    }
}

// 0x0057E014
void BsSkb::Enter_Preview()
{
    // empty (in the original too)
}

// 0x0057E044
void BsSkb::Close()
{
    SetInputEnabled(true);
    BsMenuMgr::s_Flags &= ~0x40;
    m_Unknown0x11C5 = true;
    if (::state::Mode<BsSkb>::GetState() != &BsSkb::State_KeySetLoad) {
        StopSelf();
    }
}

// 0x0057E0C8
void BsSkb::State_ButtonIn()
{
    if (m_pButton1 != NULL && !m_pButton1->m_Control.IsBusy()) {
        Change(&BsSkb::State_Wait);
        BsMenuMgr::s_Flags |= 0xC0;
        SetInputEnabled(false);
    }
    if (m_pButton2 != NULL && !m_pButton2->m_Control.IsBusy()) {
        if (sys::g_Unknown95F0EB == KIND_BOARD || sys::g_Unknown95F0EB == KIND_LETTER) {
            Change(&BsSkb::State_Write);
        } else {
            Change(&BsSkb::State_Wait);
        }
        BsMenuMgr::s_Flags |= 0xC0;
        SetInputEnabled(false);
    }
    if (m_pButton3 != NULL && !m_pButton3->m_Control.IsBusy()) {
        BsMenuMgr::s_Flags |= 0xC0;
        SetInputEnabled(false);
        Change(&BsSkb::State_Wait);
    }
}

// 0x0057E1E8
void BsSkb::State_WindowIn()
{
    const nn::math::VEC3 position = m_pBg->FindPane("N_All")->m_Translate;
    if (m_pKeySet != NULL) {
        m_pKeySet->GetLayout()->GetRootPane()->SetTranslate(position);
    }
    m_pKeySetSelect->GetRootPane()->SetTranslate(position);
    if (m_HasTextAnim != true) {
        m_pTextArea->GetRootPane()->SetTranslate(position);
    }
    if (m_pArrow != NULL) {
        m_pArrow->GetRootPane()->SetTranslate(position);
        m_pArrow->m_State = lyt::Object::STATE_CHANGED;
    }
    if (m_KeySetKind == KEY_SET_GRID) {
        m_pGridArrow->GetRootPane()->SetTranslate(position);
        m_pGridArrow->m_State = lyt::Object::STATE_CHANGED;
    }
    if (m_pKeySet != NULL) {
        m_pKeySet->GetLayout()->m_State = lyt::Object::STATE_CHANGED;
    }
    m_pBg->m_State = lyt::Object::STATE_CHANGED;
    m_pKeySetSelect->m_State = lyt::Object::STATE_CHANGED;
    m_pTextArea->m_State = lyt::Object::STATE_CHANGED;
    if (m_HasTextAnim == true) {
        if (m_pBgInAnim->m_State == ut::FrameController::STATE_STOPPING &&
            m_pTextInAnim->m_State == ut::FrameController::STATE_STOPPING) {
            Change(&BsSkb::State_ButtonIn);
        }
    } else if (m_pBgInAnim->m_State == ut::FrameController::STATE_STOPPING) {
        Change(&BsSkb::State_ButtonIn);
    }
}

// 0x0057E364
void BsSkb::Enter_ButtonIn()
{
    bool isDisabled;
    if (sys::g_Unknown95F0EB == KIND_BOARD || sys::g_Unknown95F0EB == KIND_LETTER) {
        isDisabled = false;
        if (IsLetterWithFrame()) {
            u8* letterMenu = GetLetterMenu();
            *reinterpret_cast<u32*>(letterMenu + 0x6B4) |= 8;
            reinterpret_cast<ButtonActionControl*>(letterMenu + 0x5E0)->ResetSelectAll();
            Func2DA908(letterMenu);
        }
    } else {
        isDisabled = !sys::IsTextAcceptable();
    }
    if (m_pButton1 != NULL) {
        m_pButton1->m_Control.StartIn();
        m_pButton1->m_Control.SetButtonDisabled(0, isDisabled);
    }
    if (m_pButton2 != NULL) {
        m_pButton2->m_Control.StartIn();
        m_pButton2->m_Control.SetButtonDisabled(1, isDisabled);
    }
    if (m_pButton3 != NULL) {
        m_pButton3->m_Control.StartIn();
        m_pButton3->m_Control.SetButtonDisabled(2, isDisabled);
    }
    BsMenuMgr::s_Flags |= 0x10;
}

// 0x0057E490
void BsSkb::Enter_WindowIn()
{
    BsMenuMgr::s_Flags = (BsMenuMgr::s_Flags & ~0x90) | 8;
    m_pBgInAnim->Restart();
    if (m_HasTextAnim == true) {
        m_pTextInAnim->Restart();
    }
    m_pBg->m_State = lyt::Object::STATE_CHANGED;
    if (m_KeySetKind == KEY_SET_GRID) {
        for (s32 i = 0; i < 2; i++) {
            m_pGridArrowAnims[i]->ApplyFrame(m_pGridArrowAnims[i]->m_EndFrame);
        }
    }
}

// 0x0057E52C
void BsSkb::State_ButtonOut()
{
    if (m_pButton1 != NULL && !m_pButton1->m_Control.IsBusy()) {
        Change(&BsSkb::State_WindowOut);
    }
    if (m_pButton2 != NULL && !m_pButton2->m_Control.IsBusy()) {
        Change(&BsSkb::State_WindowOut);
    }
    if (m_pButton3 != NULL && !m_pButton3->m_Control.IsBusy()) {
        Change(&BsSkb::State_WindowOut);
    }
}

// 0x0057E5CC
void BsSkb::State_WindowOut()
{
    const nn::math::VEC3 position = m_pBg->FindPane("N_All")->m_Translate;
    if (m_pKeySet != NULL) {
        m_pKeySet->GetLayout()->GetRootPane()->SetTranslate(position);
    }
    m_pKeySetSelect->GetRootPane()->SetTranslate(position);
    if (m_HasTextAnim != true) {
        m_pTextArea->GetRootPane()->SetTranslate(position);
    }
    if (m_pArrow != NULL) {
        m_pArrow->GetRootPane()->SetTranslate(position);
        m_pArrow->m_State = lyt::Object::STATE_CHANGED;
    }
    if (m_KeySetKind == KEY_SET_GRID) {
        m_pGridArrow->GetRootPane()->SetTranslate(position);
        m_pGridArrow->m_State = lyt::Object::STATE_CHANGED;
    }
    if (m_pKeySet != NULL) {
        m_pKeySet->GetLayout()->m_State = lyt::Object::STATE_CHANGED;
    }
    m_pBg->m_State = lyt::Object::STATE_CHANGED;
    m_pKeySetSelect->m_State = lyt::Object::STATE_CHANGED;
    m_pTextArea->m_State = lyt::Object::STATE_CHANGED;
    if (m_HasTextAnim == true) {
        if (m_pBgOutAnim->m_State != ut::FrameController::STATE_STOPPING ||
            m_pTextOutAnim->m_State != ut::FrameController::STATE_STOPPING) {
            return;
        }
    } else if (m_pBgOutAnim->m_State != ut::FrameController::STATE_STOPPING) {
        return;
    }
    BsMenuMgr::s_Flags &= ~0x40;
    StopSelf();
}

// 0x0057E750
void BsSkb::State_NgError()
{
    if (BsCommonDialog::GetInstance() != NULL) {
        Change(&BsSkb::State_NgErrorWait);
    }
}

// 0x0057E77C
void BsSkb::Enter_ButtonOut()
{
    if (m_pButton1 != NULL) {
        m_pButton1->m_Control.StartOut();
    }
    if (m_pButton2 != NULL) {
        m_pButton2->m_Control.StartOut();
    }
    if (m_pButton3 != NULL) {
        m_pButton3->m_Control.StartOut();
    }
}

// 0x0057E7C0
void BsSkb::Enter_WindowOut()
{
    SetInputEnabled(true);
    BsMenuMgr::s_Flags &= ~0x100;
    m_pBgOutAnim->ApplyLimitFrame(true);
    m_pBgOutAnim->Restart();
    if (m_HasTextAnim == true) {
        m_pTextOutAnim->Restart();
    }
    m_pBg->m_State = lyt::Object::STATE_CHANGED;
}

// 0x0057E83C
void BsSkb::Enter_NgError()
{
    BsMenuMgr::Func6D32D0();
    g_CommonDialogArg.isCentered = true;
    g_CommonDialogArg.icon = 1;
    if (m_IsRandomMatch != true) {
        g_CommonDialogArg.kind = ComDialogA::KIND_ONE_BUTTON;
        g_CommonDialogArg.pFile = MESSAGE_FILE;
        g_CommonDialogArg.labels[3] = LABEL_ERROR;
        g_CommonDialogArg.labels[0] = m_ErrorLabel;
        g_CommonDialogArg.isLast = true;
    } else {
        DialogTextRequest request;
        request.Clear();
        request.index = m_ErrorLabel;
        g_DialogTextRequest = request;
    }
    BsMenuMgr::Func6D406C(true);
}

// 0x0057E8DC
void BsSkb::SetupPages()
{
    const s32 lastPage = GetRegionIndex() != 0 ? 3 : 9;
    m_GridPageNum = 0;
    for (s32 i = 0; i <= lastPage; i++) {
        if (sys::GetMessageText(s_PageLabels[i])[0] != 0) {
            m_GridPages[m_GridPageNum++] = i;
            if (m_GridPageNum == 10) {
                break;
            }
        }
    }
    if (GetRegionIndex() == 0) {
        m_CellPhonePageNum = 0;
    } else {
        m_CellPhonePageNum = 0;
        for (s32 i = 4; i <= 9; i++) {
            if (sys::GetMessageText(s_PageLabels[i])[0] != 0) {
                m_CellPhonePages[m_CellPhonePageNum++] = i;
                if (m_CellPhonePageNum == 10) {
                    break;
                }
            }
        }
    }
    sys::KeyboardSetting& setting = sys::g_KeyboardSetting;
    if (setting._unknown0x18 < 0 || setting._unknown0x18 >= m_GridPageNum) {
        setting._unknown0x18 = 0;
    }
    if (setting._unknown0x14 < 0 || setting._unknown0x14 >= m_CellPhonePageNum) {
        setting._unknown0x14 = 0;
    }
}

// 0x0057EA08
void BsSkb::State_ButtonWait()
{
    if (!m_pButton2->m_Control.IsSelectOkDone()) {
        return;
    }
    if (sys::g_Unknown95F0EB == KIND_BOARD) {
        static_cast<lyt::TextAreaBoard*>(m_pTextArea)->StoreText();
    } else if (sys::g_Unknown95F0EB == KIND_LETTER) {
        static_cast<lyt::TextAreaMail*>(m_pTextArea)->StoreText(false);
    }
    m_pTextArea->m_State = lyt::Object::STATE_CHANGED;
    switch (m_pButton2->m_Control.GetSelectedIndex()) {
    case 0:
        m_pPageWidget->ResetArrows();
        if (m_pArrow != NULL) {
            m_pArrow->m_State = lyt::Object::STATE_CHANGED;
        }
        Change(&BsSkb::State_WaitCancel);
        break;
    case 1:
        Change(&BsSkb::State_Preview1);
        break;
    }
    m_pButton2->m_Control.ResetSelectAll();
    m_pButton2->m_Control.Reset();
    static_cast<lyt::TextAreaEx*>(m_pTextArea)->m_Unknown0x73A = false;
}

// 0x0057EB0C
void BsSkb::State_KeySetLoad()
{
    const char* path;
    if (m_RequestedKeySet == KEY_SET_QWERTY) {
        path = "Layout/Swkbd/swkbd_qwerty.arc";
    } else if (m_RequestedKeySet == KEY_SET_KANA) {
        path = "Layout/Swkbd/swkbd_kana.arc";
    } else if (m_RequestedKeySet == KEY_SET_GRID) {
        path = "Layout/Swkbd/swkbd_grid.arc";
    } else if (m_RequestedKeySet == KEY_SET_CELL_PHONE) {
        path = "Layout/Swkbd/swkbd_cell.arc";
    } else {
        path = NULL;
    }
    bool isRead;
    if (stage::GetCurrent() == STAGE_KEY_SET_IN_STAGE_HEAP) {
        isRead = m_KeyReader.ReadArc(path, HeapInit::s_pStageHeap);
    } else {
        isRead = m_KeyReader.ReadArc(path);
    }
    if (!isRead) {
        return;
    }
    if (m_Unknown0x11C5 == true) {
        if ((BsMenuMgr::s_Flags & 0x40) != 0) {
            Change(&BsSkb::State_ButtonOut);
            return;
        }
        StopSelf();
        return;
    }
    const char* fontName = font::Mgr::s_pInstance->GetFontName(static_cast<font::FontID>(0))->getCString();
    m_KeyReader.GetResourceAccessorVRAM()->RegistFont(fontName, font::Mgr::s_pInstance->GetFont(static_cast<font::FontID>(0)));
    m_KeyReader.LoadTextures();
    ChangeKeySet(m_RequestedKeySet);
    SetInputEnabled(false);
    m_RequestedKeySet = KEY_SET_NONE;
    m_IsKeySetLoading = false;
    if (sys::g_Unknown95F0EB == KIND_BOARD || sys::g_Unknown95F0EB == KIND_LETTER) {
        Change(&BsSkb::State_Write);
    } else {
        Change(&BsSkb::State_Wait);
    }
}

// 0x0057ECBC
void BsSkb::State_MelodyPlay()
{
    const s32 noteIndex = SoundMgr::SGetMelodyNoteIndex();
    if (m_IsMelodyStarted == true && !SoundMgr::SIsMelodyBusy()) {
        audio::bgm::GetTrackMixer()->RemoveMix(32);
        m_IsMelodyPlaying = false;
        m_pMelodyButton->SetEnabled(true, true, false);
        m_pButton2->m_Control.SetButtonDisabled(0, false);
        m_pButton2->m_Control.SetButtonDisabled(1, false);
        SetInputEnabled(false);
        m_pPageWidget->SetArrowsActive(false);
        Change(&BsSkb::State_Preview);
    }
    if (m_IsMelodyStarted == false && noteIndex != -1) {
        m_IsMelodyStarted = true;
    }
}

// 0x0057ED94
void BsSkb::State_PreviewEnd()
{
    m_pTextArea->m_State = lyt::Object::STATE_CHANGED;
    if (sys::g_Unknown95F0EB == KIND_LETTER) {
        static_cast<lyt::TextAreaMail*>(m_pTextArea)->CheckText();
    }
    if (m_pTextOutAnim->m_State == ut::FrameController::STATE_STOPPING) {
        Change(&BsSkb::State_PreviewCheck);
    }
}

// 0x0057EDE8
void BsSkb::State_ReturnWait()
{
    bool isDone = false;
    if (m_pButton1 != NULL) {
        isDone = m_pButton1->m_Control.IsSelectOkDone();
    }
    if (m_pButton2 != NULL) {
        isDone = m_pButton2->m_Control.IsSelectOkDone();
    }
    if (m_pButton3 != NULL) {
        isDone = m_pButton3->m_Control.IsSelectOkDone();
    }
    if (!isDone) {
        return;
    }
    if (m_pButton1 != NULL) {
        m_pButton1->m_Control.ResetSelectAll();
        m_pButton1->m_Control.Reset();
    }
    if (m_pButton2 != NULL) {
        m_pButton2->m_Control.ResetSelectAll();
        m_pButton2->m_Control.Reset();
    }
    if (m_pButton3 != NULL) {
        m_pButton3->m_Control.ResetSelectAll();
        m_pButton3->m_Control.Reset();
    }
    if ((sys::g_KeyboardParam.flags & PARAM_FLAG_CHECK_TEXT) != 0) {
        m_CheckText.Set(reinterpret_cast<const char16*>(sys::g_pTextBuffer), 33);
        m_TextCheck.Begin(&m_CheckText);
        Change(&BsSkb::State_NgCheck);
    } else if ((sys::g_KeyboardParam.flags & PARAM_FLAG_CHECK_NUMBERS) != 0) {
        m_CheckText.Set(reinterpret_cast<const char16*>(sys::g_pTextBuffer), 33);
        m_TextCheck.Begin(&m_CheckText);
        Change(&BsSkb::State_NgCheck2);
    } else {
        sys::SetFinished(true, m_Result);
        Change(&BsSkb::State_Wait);
    }
}

// 0x0057EF78
void BsSkb::Enter_ButtonWait()
{
    sys::g_pTextManager->CommitAll(true);
}

// 0x0057EF8C
void BsSkb::Enter_MelodyPlay()
{
    SetInputEnabled(true);
    m_pButton2->m_Control.SetButtonDisabled(0, true);
    m_pButton2->m_Control.SetButtonDisabled(1, true);
    UnpackMelody(reinterpret_cast<const u8*>(BsMenuMgr::s_pInstance->m_Unknown0x200) + 0x278, &m_Melody);
    SoundMgr::SUpdateTownMelody();
    SoundMgr::SPlayMelody(SE_MELODY_LETTER, m_Melody.notes);
    audio::bgm::GetTrackMixer()->AddMix(32);
    m_IsMelodyStarted = false;
}

// 0x0057F024
void BsSkb::Enter_PreviewEnd()
{
    BsMenuMgr::s_Flags &= ~0x100;
    if (sys::g_Unknown95F0EB == KIND_LETTER) {
        static_cast<lyt::TextAreaMail*>(m_pTextArea)->StartTextCheck();
    }
    m_pTextOutAnim->Restart();
}

// 0x0057F06C
void BsSkb::Enter_ReturnWait()
{
    // empty (in the original too)
}

// 0x0057F070
void BsSkb::State_NgCheck2()
{
    bool isNg;
    if (sys::g_Unknown95F0EB == KIND_COMMENT) {
        isNg = m_TextCheck.HasNumber();
        m_ErrorLabel = LABEL_NG_NUMBER_IN_COMMENT;
    } else {
        s32 maxNumbers;
        s32 label;
        if (m_IsRandomMatch == true) {
            label = 0;
            maxNumbers = 3;
        } else {
            label = LABEL_NG_NUMBERS;
            maxNumbers = 5;
        }
        m_ErrorLabel = label;
        isNg = m_TextCheck.HasMoreNumbers(maxNumbers);
    }
    if (isNg) {
        Change(&BsSkb::State_NgError);
    } else {
        sys::SetFinished(true, m_Result);
        Change(&BsSkb::State_Wait);
    }
}

// 0x0057F120
void BsSkb::State_NgCheck()
{
    if (!m_TextCheck.Update()) {
        return;
    }
    if (!m_TextCheck.IsOk()) {
        m_ErrorLabel = LABEL_NG_TEXT;
        Change(&BsSkb::State_NgError);
    } else if ((sys::g_KeyboardParam.flags & PARAM_FLAG_CHECK_NUMBERS) != 0) {
        Change(&BsSkb::State_NgCheck2);
    } else {
        sys::SetFinished(true, m_Result);
        Change(&BsSkb::State_Wait);
    }
}

// 0x0057F1C4
void BsSkb::State_WarningWait()
{
    if (!m_pOwner->IsFinished()) {
        return;
    }
    script::Mgr::s_pInstance->m_pTalk->ReleaseRecept();
    sys::g_pTextManager->m_IsFull = false;
    SetInputEnabled(false);
    Change(&BsSkb::State_Write);
}

// 0x0057F228
void BsSkb::State_WaitCancel()
{
    if (BsCommonDialog::GetInstance() != NULL) {
        Change(&BsSkb::State_WaitCancel2);
    }
}

// 0x0057F254
void BsSkb::Enter_NgCheck2()
{
    if (netgame::GetNetgameType() == netgame::TYPE_RANDOM_MATCH) {
        m_IsRandomMatch = true;
    }
}

// 0x0057F27C
void BsSkb::Enter_NgCheck()
{
    // empty (in the original too)
}

// 0x0057F280
void BsSkb::Enter_WarningWait()
{
    SetInputEnabled(true);
    SetFile(BsSvMgr::s_ErrorFile);
    SetNumberText(41);
    script::TalkSystem* talk = script::Mgr::s_pInstance->m_pTalk;
    talk->SetRecept(this);
    talk->SetOpenSound(2);
    m_pOwner->Next(true);
}

// 0x0057F2EC
void BsSkb::Enter_WaitCancel()
{
    g_CommonDialogArg.kind = ComDialogA::KIND_TWO_BUTTONS;
    g_CommonDialogArg.pFile = MESSAGE_FILE;
    g_CommonDialogArg.isCentered = true;
    g_CommonDialogArg.isLast = true;
    g_CommonDialogArg.isIconShown = false;
    g_CommonDialogArg.labels[1] = LABEL_CANCEL_CONFIRM + 1;
    g_CommonDialogArg.okSe = SE_SYS_DECIDE_QUIT;
    g_CommonDialogArg.labels[0] = LABEL_CANCEL_CONFIRM;
    g_CommonDialogArg.labels[3] = LABEL_CANCEL_CONFIRM + 2;
    if (sys::g_Unknown95F0EB == KIND_BOARD) {
        g_CommonDialogArg.labels[0] = LABEL_BOARD_CANCEL_CONFIRM;
        g_CommonDialogArg.labels[3] = LABEL_BOARD_CANCEL_CONFIRM + 2;
    }
    BsMenuMgr::Func6D406C(true);
}

// 0x0057F378
void BsSkb::State_PreviewCheck()
{
    if (sys::g_Unknown95F0EB == KIND_BOARD) {
        lyt::g_pUnknown9506D4[0x14C4] = true;
        Change(&BsSkb::State_Idle);
    } else if (sys::g_Unknown95F0EB == KIND_LETTER) {
        if (static_cast<lyt::TextAreaMail*>(m_pTextArea)->CheckText()) {
            lyt::g_pUnknown950BD0[0x534] = true;
            Change(&BsSkb::State_Idle);
        }
    }
}

// 0x0057F408
void BsSkb::Enter_PreviewCheck()
{
    // empty (in the original too)
}

// 0x0057F40C
void BsSkb::State_Preview1()
{
    if (m_pButton2->m_Control.IsBusy()) {
        return;
    }
    m_pButton2->m_Control.ResetSelectAll();
    m_pButton2->m_Control.Reset();
    m_pButton2->m_Control.SetText(LABEL_PREVIEW, LABEL_SEND, MESSAGE_FILE);
    m_pButton2->m_Control.SetButtonOkSound(SE_SYS_DECIDE_L, 1);
    Change(&BsSkb::State_Preview2);
}

// 0x0057F4A0
void BsSkb::State_Preview2()
{
    const nn::math::VEC3 position = m_pBg->FindPane("N_All")->m_Translate;
    m_pKeySet->GetLayout()->GetRootPane()->SetTranslate(position);
    m_pKeySetSelect->GetRootPane()->SetTranslate(position);
    if (m_pArrow != NULL) {
        m_pArrow->GetRootPane()->SetTranslate(position);
        m_pArrow->m_State = lyt::Object::STATE_CHANGED;
    }
    if (m_KeySetKind == KEY_SET_GRID) {
        m_pGridArrow->GetRootPane()->SetTranslate(position);
        m_pGridArrow->m_State = lyt::Object::STATE_CHANGED;
    }
    m_pKeySet->GetLayout()->m_State = lyt::Object::STATE_CHANGED;
    m_pBg->m_State = lyt::Object::STATE_CHANGED;
    m_pKeySetSelect->m_State = lyt::Object::STATE_CHANGED;
    if (m_pBgOutAnim->m_State == ut::FrameController::STATE_STOPPING) {
        BsMenuMgr::s_Flags &= ~0x40;
        m_pPageWidget->ResetArrows();
        Change(&BsSkb::State_Preview3);
    }
}

// 0x0057F5C8
void BsSkb::State_Preview3()
{
    bool isAnimDone = true;
    if (IsLetterWithFrame() && m_pMelodyInAnim->m_State != ut::FrameController::STATE_STOPPING) {
        isAnimDone = false;
    }
    if (!isAnimDone || m_pButton2->m_Control.IsBusy()) {
        return;
    }
    if (IsLetterWithFrame()) {
        m_pMelodyButton->SetEnabled(false, true, false);
        Change(&BsSkb::State_MelodyPlay);
        return;
    }
    Change(&BsSkb::State_Preview);
}

// 0x0057F6A4
void BsSkb::State_Rewrite0()
{
    if (m_pButton2->m_Control.IsSelectOkDone()) {
        Change(&BsSkb::State_Rewrite1);
    }
}

// 0x0057F6D8
void BsSkb::State_Rewrite1()
{
    bool isAnimDone = true;
    if (IsLetterWithFrame() && m_pMelodyOutAnim->m_State != ut::FrameController::STATE_STOPPING) {
        isAnimDone = false;
    }
    if (!isAnimDone || m_pButton2->m_Control.IsBusy()) {
        return;
    }
    switch (m_pButton2->m_Control.GetSelectedIndex()) {
    case 0:
        m_pButton2->m_Control.ResetSelectAll();
        m_pButton2->m_Control.Reset();
        m_pButton2->m_Control.SetText(LABEL_CANCEL, LABEL_REWRITE, MESSAGE_FILE);
        m_pButton2->m_Control.SetSeOk(m_CancelSe, m_OkSe);
        Change(&BsSkb::State_Rewrite2);
        break;
    case 1:
        Change(&BsSkb::State_PreviewEnd);
        break;
    }
}

// 0x0057F7E8
void BsSkb::State_Rewrite2()
{
    const nn::math::VEC3 position = m_pBg->FindPane("N_All")->m_Translate;
    m_pKeySet->GetLayout()->GetRootPane()->SetTranslate(position);
    m_pKeySetSelect->GetRootPane()->SetTranslate(position);
    if (m_pArrow != NULL) {
        m_pArrow->GetRootPane()->SetTranslate(position);
        m_pArrow->m_State = lyt::Object::STATE_CHANGED;
    }
    if (m_KeySetKind == KEY_SET_GRID) {
        m_pGridArrow->GetRootPane()->SetTranslate(position);
        m_pGridArrow->m_State = lyt::Object::STATE_CHANGED;
    }
    m_pKeySet->GetLayout()->m_State = lyt::Object::STATE_CHANGED;
    m_pBg->m_State = lyt::Object::STATE_CHANGED;
    m_pKeySetSelect->m_State = lyt::Object::STATE_CHANGED;
    if (m_pBgInAnim->m_State == ut::FrameController::STATE_STOPPING) {
        Change(&BsSkb::State_Rewrite3);
    }
}

// 0x0057F8F0
void BsSkb::State_Rewrite3()
{
    if (m_pButton2->m_Control.IsBusy()) {
        return;
    }
    if (IsLetterWithFrame()) {
        m_pPageWidget->SetArrowsActive(true);
    }
    BsMenuMgr::s_Flags |= 0x40;
    Change(&BsSkb::State_Write);
}

// 0x0057F974
void BsSkb::State_NgErrorWait()
{
    if (BsCommonDialog::GetInstance() == NULL) {
        SetInputEnabled(false);
        Change(&BsSkb::State_Wait);
    }
}

// 0x0057F9AC
void BsSkb::Enter_Preview1()
{
    m_pButton2->m_Control.StartOut();
    if (IsLetterWithFrame()) {
        Func2DA978(GetLetterMenu());
    }
}

// 0x0057FA00
void BsSkb::Enter_Preview2()
{
    m_pBgOutAnim->ApplyLimitFrame(true);
    m_pBgOutAnim->Restart();
    if (m_HasTextAnim == true) {
        lyt::TextAreaEx* textArea = static_cast<lyt::TextAreaEx*>(m_pTextArea);
        textArea->ScrollToTop();
        textArea->SetIndicatorVisible(false);
        textArea->vf_0x58(true, false);
    }
    m_pBg->m_State = lyt::Object::STATE_CHANGED;
}

// 0x0057FA94
void BsSkb::Enter_Preview3()
{
    m_pButton2->m_Control.StartIn();
    if (IsLetterWithFrame()) {
        m_pMelodyInAnim->Restart();
    }
}

// 0x0057FAF0
void BsSkb::Enter_Rewrite0()
{
    if (IsLetterWithFrame()) {
        m_pMelodyButton->SetEnabled(false, true, false);
    }
}

// 0x0057FB54
void BsSkb::Enter_Rewrite1()
{
    m_pButton2->m_Control.StartOut();
    if (IsLetterWithFrame()) {
        m_pMelodyOutAnim->Restart();
    }
}

// 0x0057FBB0
void BsSkb::Enter_Rewrite2()
{
    m_pBgInAnim->Restart();
    m_pBg->m_State = lyt::Object::STATE_CHANGED;
    if (m_KeySetKind == KEY_SET_GRID) {
        for (s32 i = 0; i < 2; i++) {
            m_pGridArrowAnims[i]->ApplyFrame(m_pGridArrowAnims[i]->m_EndFrame);
        }
    }
    static_cast<lyt::TextAreaEx*>(m_pTextArea)->ScrollBack();
}

// 0x0057FC20
void BsSkb::Enter_Rewrite3()
{
    m_pButton2->m_Control.StartIn();
    if (IsLetterWithFrame()) {
        Func2DA908(GetLetterMenu());
    }
}

// 0x0057FC74
void BsSkb::Enter_NgErrorWait()
{
    // empty (in the original too)
}

// 0x0057FC80
void BsSkb::StartTextIn()
{
    m_pTextInAnim->Restart();
}

// 0x0057FC90
void BsSkb::StartTextOut()
{
    m_pTextOutAnim->Restart();
}

// 0x0057FCA0
void BsSkb::State_WaitCancel2()
{
    if (BsCommonDialog::GetInstance() != NULL) {
        return;
    }
    if (BsMenuMgr::s_Result == 1) {
        if (sys::g_Unknown95F0EB == KIND_BOARD) {
            lyt::g_pUnknown9506D4[0x14C4] = false;
        } else if (sys::g_Unknown95F0EB == KIND_LETTER) {
            lyt::g_pUnknown950BD0[0x534] = false;
        }
        Change(&BsSkb::State_Idle);
    } else if (BsMenuMgr::s_Result == 0) {
        lyt::TextAreaEx* textArea = static_cast<lyt::TextAreaEx*>(m_pTextArea);
        textArea->SetIndicatorVisible(true);
        textArea->vf_0x58(false, false);
        Change(&BsSkb::State_Write);
    }
}

// 0x0057FD84
void BsSkb::Enter_WaitCancel2()
{
    // empty (in the original too)
}

// 0x0057FD88
bool BsSkb::IsTextInDone()
{
    return m_pTextInAnim->m_State == ut::FrameController::STATE_STOPPING;
}

// 0x0057FDA0
bool BsSkb::IsTextOutDone()
{
    return m_pTextOutAnim->m_State == ut::FrameController::STATE_STOPPING;
}

// 0x0057FDB8 slot 0x30
oml::framework::Result BsSkb::Draw()
{
    lyt::Object::DrawAll(true);
    if (m_pButton1 != NULL) {
        m_pButton1->Draw();
    }
    if (m_pButton2 != NULL) {
        m_pButton2->Draw();
    }
    if (m_pButton3 != NULL) {
        m_pButton3->Draw();
    }
    return oml::framework::RESULT_SUCCESS;
}

// 0x0057FDF4 slot 0x0C
oml::framework::Result BsSkb::Initialize()
{
    const char* fontName0 = font::Mgr::s_pInstance->GetFontName(static_cast<font::FontID>(0))->getCString();
    const char* fontName1 = font::Mgr::s_pInstance->GetFontName(static_cast<font::FontID>(1))->getCString();
    const nw::font::Font* font0 = font::Mgr::s_pInstance->GetFont(static_cast<font::FontID>(0));
    const nw::font::Font* font1 = font::Mgr::s_pInstance->GetFont(static_cast<font::FontID>(1));
    sys::KeyboardSetting& setting = sys::g_KeyboardSetting;
    switch (m_InitStep) {
    case 0: {
        if (s_pInstance != NULL) {
            return oml::framework::RESULT_SUCCESS;
        }
        m_pHeap = ssys::ma::lyt::GetAllocator()->m_pHeap;
        if (m_IsSettingInitialized != true) {
            const u8 kind = sys::g_Unknown95F0EB;
            if (kind == KIND_CHAT || kind == KIND_FRIEND) {
                sys::RestoreSetting();
            } else if (kind == KIND_17 || kind == KIND_18) {
                sys::g_KeyboardParam._unknown0x000 = 3;
                setting.mode = 0;
            } else {
                sys::ResetSetting(&setting);
            }
            m_IsSettingInitialized = true;
        }
        sead::ScopedCurrentHeapSetter setter(m_pHeap);
        sys::GetLineManager()->m_StartX = 0.0f;
        m_InitStep++;
        break;
    }
    case 1:
        if (!m_CommonReader.ReadArc("Layout/Swkbd/swkbd_common.arc")) {
            break;
        }
        {
            bool isRead;
            switch (sys::g_Unknown95F0EB) {
            case KIND_CHAT:
            case KIND_COMMENT:
            case KIND_BIRTHDAY:
            case KIND_FRIEND:
                isRead = m_Reader.ReadArc("Layout/Key/key16.arc");
                break;
            case KIND_BOARD:
                isRead = m_Reader.ReadArc("Layout/Board/board.arc");
                break;
            case KIND_LETTER:
                isRead = m_Reader.ReadArc("Layout/Letter/ltr.arc");
                break;
            default:
                isRead = m_Reader.ReadArc("Layout/common/cmn_title_win.arc");
                break;
            }
            if (!isRead) {
                break;
            }
        }
        if (!sys::LoadMessage()) {
            break;
        }
        {
            ComButtonLytBase* buttons;
            switch (sys::g_KeyboardParam._unknown0x004) {
            case 0:
                if (m_pButton1 == NULL) {
                    m_pButton1 = new (m_pHeap, 4) ComButton1Lyt;
                }
                buttons = m_pButton1;
                break;
            case 1:
                if (m_pButton2 == NULL) {
                    m_pButton2 = new (m_pHeap, 4) ComButton2Lyt;
                }
                buttons = m_pButton2;
                break;
            case 2:
                if (m_pButton3 == NULL) {
                    m_pButton3 = new (m_pHeap, 4) ComButton3Lyt;
                }
                buttons = m_pButton3;
                break;
            default:
                buttons = NULL;
                break;
            }
            if (buttons != NULL && !buttons->ReadArchive()) {
                break;
            }
        }
        m_CommonReader.GetResourceAccessorVRAM()->RegistFont(fontName0, font0);
        m_Reader.GetResourceAccessorVRAM()->RegistFont(fontName0, font0);
        m_Reader.GetResourceAccessorVRAM()->RegistFont(fontName1, font1);
        m_InitStep++;
        break;
    case 2: {
        SetupPages();
        m_pBg = new (m_pHeap, 4) lyt::Object(0, 500, NULL);
        m_pBg->Build(&m_CommonReader, "BG.bclyt", 0x4000);
        m_pBgInAnim = m_pBg->CreateAnmObject("BG_in.bclan", "G_inout");
        m_pBgOutAnim = m_pBg->CreateAnmObject("BG_out.bclan", "G_inout");
        m_pBgInAnim->ApplyLimitFrame(true);
        switch (sys::g_Unknown95F0EB) {
        case KIND_CHAT:
        case KIND_FRIEND:
            m_OkSe = SE_SYS_SEND_MESSAGE;
            break;
        case KIND_COMMENT:
        case KIND_BIRTHDAY:
        case 4:
        case 5:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
            m_OkSe = SE_SYS_DECIDE_L;
            break;
        }
        switch (sys::g_KeyboardParam._unknown0x004) {
        case 0: {
            m_pButton1->Setup(2);
            Control1Button& control = m_pButton1->m_Control;
            control.SetDecide(true);
            s32 label = LABEL_OK;
            if (BsMenuMgr::Func6D39E8()) {
                label = g_Unknown98D508[0];
            }
            control.SetText(label, MESSAGE_FILE);
            control.SetSeOk(m_OkSe);
            control.SetButtonDisabled(0, true);
            if (g_Unknown95F320 != 0) {
                control.m_Flags2 = ButtonActionControl::ENABLE_TOUCH | ButtonActionControl::ENABLE_NODE_SHORTCUT;
                control.SetInput(BsMenuMgr::INPUT_A);
            } else {
                control.SetInput(0);
                control.m_Flags2 = ButtonActionControl::ENABLE_TOUCH;
            }
            break;
        }
        case 1: {
            m_pButton2->Setup();
            Control2Button& control = m_pButton2->m_Control;
            control.SetDecide(false, true);
            s32 left = LABEL_CANCEL;
            s32 right = LABEL_OK;
            if (BsMenuMgr::Func6D39E8()) {
                left = g_Unknown98D508[0];
                right = g_Unknown98D508[1];
            }
            control.SetText(left, right, MESSAGE_FILE);
            control.SetSeOk(m_CancelSe, m_OkSe);
            control.SetButtonDisabled(1, true);
            if (g_Unknown95F320 != 0) {
                control.m_Flags2 = ButtonActionControl::ENABLE_TOUCH | ButtonActionControl::ENABLE_NODE_SHORTCUT;
                control.SetInput(BsMenuMgr::INPUT_B, BsMenuMgr::INPUT_A);
            } else {
                control.SetInput(0, 0);
                control.m_Flags2 = ButtonActionControl::ENABLE_TOUCH;
            }
            break;
        }
        case 2: {
            m_pButton3->Setup();
            Control3Button& control = m_pButton3->m_Control;
            control.SetDecide(false, false, true);
            s32 left = LABEL_CANCEL;
            s32 middle = LABEL_CANCEL;
            s32 right = LABEL_OK;
            if (BsMenuMgr::Func6D39E8()) {
                left = g_Unknown98D508[0];
                middle = g_Unknown98D508[1];
                right = g_Unknown98D508[2];
            }
            control.SetText(left, middle, right, MESSAGE_FILE);
            control.SetSeOk(m_CancelSe, m_CancelSe, m_OkSe);
            control.SetButtonDisabled(2, true);
            control.SetInput(0, 0, 0);
            if (g_Unknown95F320 != 0) {
                control.m_Flags2 = ButtonActionControl::ENABLE_TOUCH | ButtonActionControl::ENABLE_NODE_SHORTCUT;
            } else {
                control.m_Flags2 = ButtonActionControl::ENABLE_TOUCH;
            }
            break;
        }
        }
        if (m_IsResultUsed) {
            BsMenuMgr::s_Result = 2;
        }
        CreateTextArea();
        // the selection of the key sets
        const char* const selectName = "KeytopModeSelect";
        char name[64];
        nw::ut::FormatString(name, sizeof(name), "%s.bclyt", selectName);
        m_pKeySetSelect = new (m_pHeap, 4) lyt::Object(0, 500, NULL);
        m_pKeySetSelect->Build(&m_CommonReader, name, 0x4000);
        m_pKeySetSelect->m_Layout.SetLayer(152);
        m_pKeySetSelect->SetMessage("char_type_00", "T_ktpMode_00", NULL, true, 0);
        m_pKeySetSelect->SetMessage("char_type_01", "T_ktpMode_01", NULL, true, 0);
        m_pKeySetSelect->SetMessage("char_type_02", "T_ktpMode_02", NULL, true, 0);
        m_pKeySetSelect->SetMessage("char_type_03", "T_ktpMode_03", NULL, true, 0);
        // the page arrows of the grid
        m_pGridArrow = new (m_pHeap, 4) lyt::Object(0, 500, NULL);
        m_pGridArrow->Build(&m_CommonReader, "LncArw_00.bclyt", 0x4000);
        m_pGridArrow->m_Layout.SetLayer(152);
        for (s32 i = 0; i < 2; i++) {
            m_pGridArrowAnims[i] = m_pGridArrow->CreateAnmObject("LncArw_00_Appear.bclan", s_GridArrowGroups[i]);
            m_pGridArrowAnims[i]->ApplyFrame(m_pGridArrowAnims[i]->m_StartFrame);
        }
        if (sys::g_Unknown95F0EB == KIND_17 || sys::g_Unknown95F0EB == KIND_18) {
            SetFlagVisible(m_pKeySetSelect->GetRootPane(), false);
        }
        m_pBg->m_State = lyt::Object::STATE_CHANGED;
        m_pTextArea->m_State = lyt::Object::STATE_CHANGED;
        m_pKeySetSelect->m_State = lyt::Object::STATE_CHANGED;
        m_pGridArrow->m_State = lyt::Object::STATE_CHANGED;
        for (s32 i = 0; i < 4; i++) {
            gui::RadioButton::Description desc;
            desc.pLayout = m_pKeySetSelect;
            const char* group;
            if (i == 0) {
                group = "G_ktpMode_00";
            } else if (i == 1) {
                group = "G_ktpMode_01";
            } else if (i == 2) {
                group = "G_ktpMode_02";
            } else if (i == 3) {
                group = "G_ktpMode_03";
            } else {
                group = NULL;
            }
            desc.groupName = group;
            desc.group = 'FCS0';
            nw::ut::FormatString(name, sizeof(name), "%s_n0s1.bclan", selectName);
            desc.anims[gui::ButtonBase::ANIM_PRESS] = m_pKeySetSelect->CreateAnmObject(name, desc.groupName);
            desc.anims[gui::ButtonBase::ANIM_SLIDE_OUT] = NULL;
            desc.anims[gui::ButtonBase::ANIM_DECIDE] = NULL;
            desc.anims[gui::ButtonBase::ANIM_RELEASE] = NULL;
            nw::ut::FormatString(name, sizeof(name), "%s_i0.bclan", selectName);
            desc.anims[gui::ButtonBase::ANIM_DISABLE] = m_pKeySetSelect->CreateAnmObject(name, desc.groupName);
            desc.sounds[gui::ButtonBase::SOUND_PRESS] = SE_SYS_SWK_KEY;
            desc.sounds[gui::ButtonBase::SOUND_DECIDE] = SE_SYS_SWK_CHANGE_KEY_SET;
            desc.isCenterSound = true;
            m_pKeySetButtons[i] = new (m_pHeap, 4) gui::RadioButton(desc);
            if (sys::g_KeyboardParam._unknown0x000 == 3) {
                m_pKeySetButtons[i]->SetEnabled(false, true, false);
            }
        }
        for (s32 i = 0; i < 2; i++) {
            gui::RepeatButton::Description desc;
            desc.pLayout = m_pGridArrow;
            desc.groupName = s_GridArrowGroups[i];
            desc.anims[gui::ButtonBase::ANIM_PRESS] =
                m_pGridArrow->CreateAnmObject("LncArw_00_Select.bclan", s_GridArrowGroups[i]);
            desc.anims[gui::ButtonBase::ANIM_SLIDE_OUT] = NULL;
            desc.anims[gui::ButtonBase::ANIM_DECIDE] =
                m_pGridArrow->CreateAnmObject("LncArw_00_Decide.bclan", s_GridArrowGroups[i]);
            desc.repeatAnim = m_pGridArrow->CreateAnmObject("LncArw_00_Repeat.bclan", s_GridArrowGroups[i]);
            desc.repeatDelay = GRID_ARROW_REPEAT_DELAY;
            desc.repeatInterval = GRID_ARROW_REPEAT_INTERVAL;
            desc.sounds[gui::ButtonBase::SOUND_DECIDE] = -1;
            desc.isPlayRepeatAnim = false;
            desc.decideSound = SE_SYS_SWK_CHANGE_PAGE;
            desc.isCenterSound = true;
            desc.sounds[gui::ButtonBase::SOUND_PRESS] = SE_SYS_SWK_KEY;
            desc.repeatSound = SE_SYS_SWK_CHANGE_PAGE + 1;
            desc.soundValue = 0;
            m_pGridArrowButtons[i] = new (m_pHeap, 4) gui::RepeatButton(desc);
        }
        m_InitStep++;
        break;
    }
    case 3: {
        const s32 kind = GetKeySetOfMode(setting.mode);
        const char* path;
        if (kind == KEY_SET_QWERTY) {
            path = "Layout/Swkbd/swkbd_qwerty.arc";
        } else if (kind == KEY_SET_KANA) {
            path = "Layout/Swkbd/swkbd_kana.arc";
        } else if (kind == KEY_SET_GRID) {
            path = "Layout/Swkbd/swkbd_grid.arc";
        } else if (kind == KEY_SET_CELL_PHONE) {
            path = "Layout/Swkbd/swkbd_cell.arc";
        } else {
            path = NULL;
        }
        bool isRead;
        if (stage::GetCurrent() == STAGE_KEY_SET_IN_STAGE_HEAP) {
            isRead = m_KeyReader.ReadArc(path, HeapInit::s_pStageHeap);
        } else {
            isRead = m_KeyReader.ReadArc(path);
        }
        if (!isRead) {
            break;
        }
        m_KeyReader.GetResourceAccessorVRAM()->RegistFont(fontName0, font0);
        m_KeyReader.LoadTextures();
        ChangeKeySet(GetKeySetOfMode(setting.mode));
        m_InitStep++;
        break;
    }
    case 4:
        SelectKeySetButton(setting.mode);
        gui::SetCallback(&BsSkb::OnWidgetEvent, this);
        Start("BsSkb", s_StateInfos, 28);
        s_pInstance = this;
        BsMenuMgr::s_Flags &= ~0x10;
        SetInputEnabled(true);
        m_InitStep++;
        return oml::framework::RESULT_SUCCESS;
    }
    return oml::framework::RESULT_RETRY;
}

// 0x00580E0C slot 0x24
oml::framework::Result BsSkb::Calc()
{
    if (m_RequestedKeySet >= 0 && !m_IsKeySetLoading) {
        Change(&BsSkb::State_KeySetLoad);
        m_IsKeySetLoading = true;
    }
    Call();
    gui::Update(0);
    lyt::Object::CalcAll();
    sys::ClearLastNewline();
    if (m_pTextWidget != NULL) {
        m_pTextWidget->Update();
    }
    if (m_pPageWidget != NULL) {
        m_pPageWidget->Update();
    }
    if (m_pKeySet != NULL) {
        m_pKeySet->Update();
    }
    if (m_pButton1 != NULL) {
        m_pButton1->Update();
    }
    if (m_pButton2 != NULL) {
        m_pButton2->Update();
    }
    if (m_pButton3 != NULL) {
        m_pButton3->Update();
    }
    return oml::framework::RESULT_SUCCESS;
}

// 0x00580F78
void BsSkb::SetInputEnabled(bool isEnabled)
{
    m_IsInputEnabled = isEnabled;
    lyt::g_Unknown95F1A4 = isEnabled ? 1 : 0;
    if (m_pButton1 != NULL) {
        m_pButton1->m_Control.SetFlags(ButtonActionControl::DISABLED, isEnabled);
    }
    if (m_pButton2 != NULL) {
        m_pButton2->m_Control.SetFlags(ButtonActionControl::DISABLED, isEnabled);
    }
    if (m_pButton3 != NULL) {
        m_pButton3->m_Control.SetFlags(ButtonActionControl::DISABLED, isEnabled);
    }
}

// 0x0058100C slot 0x18
oml::framework::Result BsSkb::Finalize()
{
    sead::ScopedCurrentHeapSetter setter(m_pHeap);
    if (sys::g_Unknown95F0EB == KIND_LETTER) {
        m_pTextArea->DetachChild(m_pMelody);
    }
    m_CommonReader.Clear();
    m_Reader.Clear();
    m_KeyReader.Clear();
    if (m_pButton1 != NULL) {
        m_pButton1->Finalize();
        DeleteFromHeap(m_pHeap, m_pButton1);
    }
    if (m_pButton2 != NULL) {
        m_pButton2->Finalize();
        DeleteFromHeap(m_pHeap, m_pButton2);
    }
    if (m_pButton3 != NULL) {
        m_pButton3->Finalize();
        DeleteFromHeap(m_pHeap, m_pButton3);
    }
    if (m_pTextWidget != NULL) {
        DeleteFromHeap(m_pHeap, m_pTextWidget);
    }
    if (m_pPageWidget != NULL) {
        DeleteFromHeap(m_pHeap, m_pPageWidget);
    }
    if (m_pTextArea != NULL) {
        DeleteFromHeap(m_pHeap, m_pTextArea);
    }
    if (m_pBg != NULL) {
        DeleteFromHeap(m_pHeap, m_pBg);
    }
    if (m_pArrow != NULL) {
        DeleteFromHeap(m_pHeap, m_pArrow);
    }
    if (m_pKeySet != NULL) {
        DeleteFromHeap(m_pHeap, m_pKeySet);
    }
    for (s32 i = 0; i < 2; i++) {
        if (m_pGridArrowButtons[i] != NULL) {
            DeleteFromHeap(m_pHeap, m_pGridArrowButtons[i]);
        }
    }
    for (s32 i = 0; i < 4; i++) {
        if (m_pKeySetButtons[i] != NULL) {
            DeleteFromHeap(m_pHeap, m_pKeySetButtons[i]);
        }
    }
    if (m_pGridArrow != NULL) {
        DeleteFromHeap(m_pHeap, m_pGridArrow);
    }
    if (m_pKeySetSelect != NULL) {
        DeleteFromHeap(m_pHeap, m_pKeySetSelect);
    }
    if (m_pMelody != NULL) {
        DeleteFromHeap(m_pHeap, m_pMelody);
    }
    if (m_pMelodyButton != NULL) {
        DeleteFromHeap(m_pHeap, m_pMelodyButton);
    }
    sys::Destroy();
    sys::UnloadMessage();
    gui::DestroyAll();
    lyt::Object::DestroyAll();
    if (sys::g_Unknown95F0EB == KIND_CHAT || sys::g_Unknown95F0EB == KIND_FRIEND) {
        sys::g_SavedKeyboardSetting = sys::g_KeyboardSetting;
    }
    s_pInstance = NULL;
    BsMenuMgr::s_Flags &= ~0x38;
    BsMenuMgr::Func6D32A0();
    return oml::framework::RESULT_SUCCESS;
}

// 0x005814D0
void BsSkb::OnWidgetEvent(gui::Widget* sender, s32 event, s32 value, void* user)
{
    BsSkb* skb = static_cast<BsSkb*>(user);
    skb->OnEvent(sender, event, value);
    if (skb->m_pTextWidget != NULL) {
        skb->m_pTextWidget->OnEvent(sender, event);
    }
    if (skb->m_pPageWidget != NULL) {
        skb->m_pPageWidget->OnEvent(sender, event);
    }
}

// 0x00581548
void BsSkb::OnEvent(gui::Widget* sender, s32 event, s32 value)
{
    if (event == gui::EVENT_DECIDE) {
        sys::KeyboardSetting& setting = sys::g_KeyboardSetting;
        s32 mode = setting.mode;
        // the page arrows of the grid
        for (s32 i = 0; i < 2; i++) {
            if (sender == NULL || sender != m_pGridArrowButtons[i]) {
                continue;
            }
            gui::GridKeySet* grid = static_cast<gui::GridKeySet*>(m_pKeySet);
            s32* pages = &m_GridPageNum;
            s32 page;
            if (mode == 2) {
                page = setting._unknown0x18;
            } else if (mode == 3) {
                page = setting._unknown0x14;
                pages = &m_CellPhonePageNum;
            } else {
                SelectKeySetButton(2);
                page = setting._unknown0x18;
            }
            if (i != 0) {
                page++;
                if (*pages <= page) {
                    page = 0;
                }
            } else {
                page--;
                if (page < 0) {
                    page = *pages - 1;
                }
            }
            if (setting.mode == 2) {
                setting._unknown0x18 = page;
            } else if (setting.mode == 3) {
                setting._unknown0x14 = page;
            }
            const s32 oldLabel = grid->m_Label;
            if (grid->IsShiftedLabel(oldLabel)) {
                m_IsShifted = true;
            } else if (grid->GetShiftedLabel(oldLabel) != -1) {
                m_IsShifted = false;
            }
            s32 label = pages[1 + page];
            if (m_IsShifted) {
                const s32 shifted = grid->GetShiftedLabel(label);
                if (shifted != -1) {
                    label = shifted;
                }
            }
            grid->SetLabel(label);
            grid->SetPage(page, *pages);
            m_pGridArrowButtons[i]->PlayRepeatAnimation();
            m_pGridArrowButtons[i]->m_Desc.soundValue = page;
            goto melody;
        }
        // the selection of the key sets
        for (s32 i = 0; i < 4; i++) {
            if (sender == NULL || sender != m_pKeySetButtons[i]) {
                continue;
            }
            const s32 oldMode = mode;
            if (GetRegionIndex() != 0) {
                switch (i) {
                case 0:
                    mode = 0;
                    break;
                case 1:
                    mode = 3;
                    break;
                case 2:
                    mode = 2;
                    setting.defaultMode = oldMode;
                    break;
                case 3:
                    mode = 4;
                    break;
                case 4:
                    mode = 5;
                    break;
                default:
                    if (mode == 2) {
                        setting.defaultMode = oldMode;
                    }
                    break;
                }
            } else if (i == 0) {
                mode = 0;
            } else if (i == 1) {
                mode = 1;
            } else if (i == 2) {
                mode = 2;
                setting.defaultMode = oldMode;
            } else if (i == 3) {
                mode = 4;
            } else if (mode == 2) {
                setting.defaultMode = oldMode;
            }
            SelectKeySetButton(mode);
            m_RequestedKeySet = GetKeySetOfMode(setting.mode);
            goto melody;
        }
        if (sender != NULL) {
        melody:
            if (sender == m_pMelodyButton) {
                m_IsMelodyPlaying = true;
                m_pMelodyButton->SetEnabled(false, true, false);
            }
        }
    }
    if (m_pKeySet != NULL) {
        m_pKeySet->OnEvent(sender, event);
        if (m_pKeySet->m_IsHandled == true) {
            m_pKeySet->GetLayout()->m_State = lyt::Object::STATE_CHANGED;
            m_pTextArea->m_State = lyt::Object::STATE_CHANGED;
        }
    }
}

// 0x00568CA4
void BsSkb::Enter_KeySetLoad()
{
    SetInputEnabled(true);
    m_KeyReader.Clear();
    m_KeyReader.GetResourceAccessorVRAM()->ReleaseTextures();
}

// 0x00581870
BsSkb::BsSkb()
    : m_pHeap(NULL), m_pBg(NULL), m_pBgInAnim(NULL), m_pBgOutAnim(NULL), m_pTextArea(NULL), m_pTextWidget(NULL),
      m_pTextInAnim(NULL), m_pTextOutAnim(NULL), m_pMelodyInAnim(NULL), m_pMelodyOutAnim(NULL), m_pTitleMsgPosAnim(NULL),
      m_pTitleIconAnim(NULL), m_pButton1(NULL), m_pButton2(NULL), m_pButton3(NULL), m_pArrow(NULL), m_pPageWidget(NULL),
      m_KeySetKind(KEY_SET_NONE), m_RequestedKeySet(KEY_SET_NONE), m_pKeySetSelect(NULL), m_pGridArrow(NULL),
      m_pMelody(NULL), m_Unknown0x1130(0), m_pMelodyButton(NULL), m_pKeySet(NULL), m_IsLabel16(false), m_IsLabel27(false),
      m_IsShifted(false), m_IsKeySetLoading(false), m_IsResultUsed(true), m_IsMelodyPlaying(false),
      m_IsMelodyStarted(false), m_GridPageNum(0), m_CellPhonePageNum(0), m_pPages(NULL), m_Unknown0x11A8(0),
      m_Unknown0x11AC(-1), m_HasTextAnim(false), m_IsTextAcceptable(false), m_OkSe(SE_SYS_DECIDE),
      m_CancelSe(SE_SYS_CANCEL), m_Unknown0x11BC(-1), m_PageIndex(-1), m_IsSettingInitialized(false),
      m_Unknown0x11C5(false), m_Result(-1), m_ErrorLabel(-1), m_pQwerty(NULL), m_pKana(NULL), m_pGrid(NULL),
      m_pCellPhone(NULL), m_IsInputEnabled(true), m_IsWriting(0), m_IsRandomMatch(false), m_InitStep(0),
      m_Unknown0x1220(0.0f)
{
    m_Unknown0x1218[0] = 0;
    m_Unknown0x1218[1] = 0;
    for (s32 i = 0; i < 2; i++) {
        m_pGridArrowAnims[i] = NULL;
        m_pGridArrowButtons[i] = NULL;
    }
    for (s32 i = 0; i < 4; i++) {
        m_pKeySetButtons[i] = NULL;
    }
    sys::g_LineNum = 6;
    sys::Create();
    BsMenuMgr::s_Flags &= ~0x40;
}

// 0x00581AB8 slot 0x00
// 0x00581AA0 (deleting dtor)
// 0x00581AB0 (thunk 164)
// 0x00774D04 (deleting thunk 164)
// 0x0077802C (thunk 40)
// 0x00778018 (deleting thunk 40)
BsSkb::~BsSkb()
{
    // (only the members are destroyed)
}
