#include "script/dTalkSystem.h"
#include "script/dWordRes.h"
#include "Bs/dBsDemoCancelMgr.h"
#include "Bs/dBsScene.h"
#include "Other/dController.h"
#include "Other/dDemoActor.h"
#include "Other/dProcMgr.h"
#include "demo/demo_Api.h"
#include "netgame/dNetGameMgr.h"
#include "script/dChoiceStandardWin.h"
#include "script/dITalkRecept.h"
#include "script/dInsertEngine.h"
#include "script/dMgr.h"
#include "script/dPeriodCountEngine.h"
#include "script/dRenderMain.h"
#include "script/dRenderRuby.h"
#include "script/dRenderRubySet.h"
#include "script/dTalkInsert.h"
#include "script/dTalkMeasure.h"
#include "script/dTalkPhrase.h"
#include "script/dTalkSeq.h"
#include "script/dTalkWin.h"
#include "sead/seadNew.h"
#include "ssys/ma/ma_Math.h"
#include "time/time_Api.h"

namespace script {
// (inline) the end of the page is forgotten
inline void TalkSystem::ClearPage()
{
    m_pSeq->m_IsFastRequested = 0;
    m_AutoTime = -1;
    m_IsAutoOnly = 0;
    m_IsJumped = 0;
    m_pWin->m_Window.SetPageIconVisible(false);
}

// 0x0060296C (name is ours)
void TalkSystem::ReleaseMessage()
{
    if (m_Message.IsInitialized()) {
        m_Message.Finalize();
    }
    m_Files.Release();
}

// 0x006029F0 (name is ours)
void TalkSystem::HideWaitIcon(bool isKept)
{
    m_WaitIconTime = 0;
    if (!isKept) {
        m_WaitIconMode = 0;
        m_pWin->m_Window.SetWaitIconVisible(false);
    }
}

// 0x00602A14 (name is ours)
void TalkSystem::SetupText()
{
    s32 index = m_pMessage->m_Index;
    const char16* text = static_cast<const char16*>(m_Message.GetText(index));
    TalkAttribute* attribute = m_pAttribute;
    attribute->Clear();
    attribute->pData = static_cast<const u8*>(m_Message.GetAttribute(index));
    m_pSeq->SetText(&text, &MsgEngine::s_pNoStop);
    m_pJump->m_File.copy(m_pMessage->m_File, -1);
    TalkLabel* message = m_pMessage;
    TalkLabel* jump = m_pJump;
    u8 mode = message->m_NameMode;
    static_cast<IWord&>(jump->m_Name).Copy(message->m_Name);
    jump->m_NameMode = mode;
    m_pJump->m_Number = m_pAttribute->pData[2];
    m_pJump->m_Flags = TalkLabel::FLAG_NUMBER;
    u8 value = m_pAttribute->pData[3];
    m_pRecept->OnBegin(value < 18 ? value : 0);
    InsertEngine insert(m_pInsert, false);
    if (insert.m_pInsert != NULL && text != NULL) {
        insert.Run(text);
    }
    m_pState->SetSpeaker(m_pAttribute);
}

// 0x00602B64 (name is ours)
void TalkSystem::UpdateOpen()
{
    if (!m_pWin->m_Window.Func770A18()) {
        m_NextState = STATE_WRITE;
    }
}

// 0x00602B88 (name is ours)
void TalkSystem::EnterOpen()
{
    TalkWin* win = m_pWin;
    if (win->m_IsSpecial) {
        win->m_Window.OpenSpecial();
    } else {
        win->m_Window.Open();
        SeID sound;
        if (win->m_OpenSound < 5) {
            sound = s_OpenSounds[win->m_OpenSound];
        } else {
            sound = win->m_WindowKind == 6 ? SE_SYS_WIN_TEXT_WARNING_OPEN : SE_SYS_WIN_TEXT_OPEN;
        }
        SoundMgr::PlaySound(sound, 0.0f);
    }
    win->m_OpenSound = 5;
    win->m_State = 1;
    m_QuitMode = QUIT_NONE;
    m_IsQuitForced = 0;
    m_IsQuitNext = 0;
}

// 0x00602C14 (name is ours)
void TalkSystem::SetRecept(ITalkRecept* recept)
{
    m_pRecept = recept;
    recept->m_pOwner = this;
    if (BsDemoCancelMgr::s_pInstance != NULL && demo::CheckOrder(demo::REVOKED_PLAYER_CONTROL, false)) {
        DemoActor* actor = demo::GetActor();
        if (actor != NULL &&
            (actor->m_Index == PROC_AC_NPC_SP_SHOP_ISLAND || actor->m_Index == PROC_AC_NPC_SP_KAPPEIS_KID_ONE)) {
            return;
        }
        if (BsDemoCancelMgr::s_pInstance->m_State == 0) {
            BsDemoCancelMgr::s_pInstance->m_State = 1;
        }
    }
}

// 0x00602CA0 (name is ours)
void TalkSystem::ReleaseRecept()
{
    ITalkRecept* recept = m_pRecept;
    if (recept != NULL) {
        recept->m_pOwner = NULL;
        static_cast<IWord&>(recept->m_Name).Clear();
        recept->m_NameMode = 3;
        recept->m_File.clear();
        recept->m_Label.clear();
        recept->m_Number = -1;
        m_pRecept = NULL;
    }
}

// 0x00602D04 (name is ours)
void TalkSystem::UpdateClose()
{
    if (m_pWin->m_Window.Func770A18()) {
        return;
    }
    if (m_QuitMode == QUIT_NONE || m_IsEnd) {
        Reset();
        m_NextState = STATE_IDLE;
    } else {
        m_pSeq->Reset();
        m_NextState = STATE_CLOSED;
    }
}

// 0x00602D60 (name is ours)
void TalkSystem::LeaveOpen()
{
    // empty (in the original too)
}

// 0x00602D64 (name is ours)
void TalkSystem::EnterClose()
{
    TalkWin* win = m_pWin;
    win->m_Shake.Stop();
    if (win->m_IsSpecial) {
        win->m_Window.CloseSpecial();
        win->m_IsSpecial = 0;
    } else {
        win->m_Window.Close();
        SeID sound = win->m_CloseSound < 3 ? s_CloseSounds[win->m_CloseSound] : SE_SYS_WIN_TEXT_CLOSE;
        SoundMgr::PlaySound(sound, 0.0f);
    }
    win->m_CloseSound = 3;
    win->m_State = 3;
}

// 0x00602E20 (name is ours)
void TalkSystem::SetWaitIcon(u8 mode, s32 time)
{
    if (time > 0) {
        m_WaitIconTime = time;
        m_WaitIconMode = mode;
    }
}

// 0x00602E30 (name is ours)
void TalkSystem::UpdateIdle()
{
    // empty (in the original too)
}

// 0x00602E34 (name is ours)
void TalkSystem::UpdateLoad()
{
    bool isStarted = false;
    if (m_IsLoading) {
        TalkFile* file = m_Files.m_pCurrent;
        bool isDone;
        if (file != &m_Files.m_Main) {
            isDone = true;
        } else {
            isDone = file->m_Loader.Load(file->m_Path, file->m_pHeap, 0);
            if (!isDone) {
                return;
            }
        }
        if (!m_Message.IsInitialized()) {
            m_Message.Initialize(m_Files.m_pCurrent->GetMessageData());
        }
        if (isDone) {
            isStarted = true;
        }
    } else if (m_pMessage->m_IsUnknown0x79) {
        isStarted = true;
    }
    if (isStarted) {
        *m_pDate = *time::GetCurrentDate();
        m_pRecept->OnMessageStart(m_pMessage);
    }
    u8 next = m_pWin->m_State == 2 ? STATE_WRITE : STATE_OPEN;
    if (!m_pMessage->Resolve(&m_Message)) {
        if (m_pWin->m_State == 2) {
            m_NextState = STATE_CLOSE;
        } else {
            Reset();
            m_NextState = STATE_IDLE;
        }
        return;
    }
    SetupText();
    TalkLabel* message = m_pMessage;
    TalkWin* win = m_pWin;
    IWord& name = message->m_Name;
    u8 mode = message->m_NameMode;
    nw::lyt::TextBox* nameTextBox = win->m_Window.m_pNameTextBox;
    const char16* text = name.GetWord();
    u32 size = name.GetSize();
    bool isEmpty = true;
    if (text != NULL && size != 0 && text[0] != 0) {
        isEmpty = false;
    }
    u8 isNameShown;
    if (nameTextBox != NULL && !isEmpty) {
        isNameShown = 1;
        nameTextBox->SetString(reinterpret_cast<const wchar_t*>(name.GetWord()), 0);
    } else {
        isNameShown = 0;
    }
    win->m_Window.SetNameColor(s_NameColors[mode < 3 ? mode : 0]);
    win->m_Window.m_IsNameShown = isNameShown;
    win = m_pWin;
    u8 speaker = m_pAttribute->GetSpeaker();
    u8 window = speaker < TalkAttribute::SPEAKER_NUM ? TalkSeq::s_SpeakerWindows[speaker] : 0;
    win->m_Window.SetWindowColor(window < 11 ? s_WindowColors[window] : 0);
    win->m_Window.m_Unknown0xAF1 = window == 6 || window == 8 || window == 9;
    win->m_WindowKind = window;
    m_NextState = next;
}

// 0x006030D0 (name is ours)
void TalkSystem::LeaveClose()
{
    // empty (in the original too)
}

// 0x006030D4 (name is ours)
void TalkSystem::SetTimer(s32 time, bool isAuto)
{
    m_NetTimeMax = time;
    m_NetTime = -1;
    m_IsNetAuto = isAuto;
}

// 0x006030E8 (name is ours)
void TalkSystem::EnterIdle()
{
    // empty (in the original too)
}

// 0x006030EC (name is ours)
void TalkSystem::EnterLoad()
{
    if (m_Files.m_pCurrent->GetMessageData() == NULL) {
        ITalkRecept* recept = m_pRecept;
        if (!recept->m_Label.isEmpty()) {
            m_pJump->m_Label.copy(recept->m_Label, -1);
            m_pJump->m_Flags = TalkLabel::FLAG_LABEL;
        } else if (recept->m_Number >= 0) {
            m_pJump->m_Number = recept->m_Number;
            m_pJump->m_Flags = TalkLabel::FLAG_NUMBER;
        }
        TalkLabel* jump = m_pJump;
        jump->m_File.copy(sead::FixedSafeString<32>(m_pRecept->m_File), -1);
        recept = m_pRecept;
        jump = m_pJump;
        u8 mode = recept->m_NameMode;
        static_cast<IWord&>(jump->m_Name).Copy(recept->m_Name);
        jump->m_NameMode = mode;
    }
    m_pMessage->Set(*m_pJump);
    m_pJump->Clear();
    m_pRecept->OnMessage(m_pMessage);
    bool isSame = false;
    if (m_Files.m_pCurrent->GetMessageData() != NULL &&
        m_Files.m_pCurrent->m_Name.compare(m_pMessage->m_File, sead::SafeStringBase<char>::MAX_LENGTH) == 0) {
        isSame = true;
    }
    m_IsLoading = !isSame;
    if (isSame) {
        return;
    }
    ReleaseMessage();
    m_Files.Select(m_pMessage->m_File, Mgr::s_pFileHeap);
    TalkFile* file = m_Files.m_pCurrent;
    if (file == &m_Files.m_Main && !file->m_Loader.Load(file->m_Path, file->m_pHeap, 0)) {
        return;
    }
    if (m_Message.IsInitialized()) {
        return;
    }
    m_Message.Initialize(m_Files.m_pCurrent->GetMessageData());
}

// 0x00603408 (name is ours)
void TalkSystem::Next(bool isCloseAtEnd)
{
    m_IsCloseAtEnd = isCloseAtEnd;
    m_NextState = STATE_LOAD;
    if (m_State == STATE_KEEP) {
        m_pSeq->Reset();
    }
}

// 0x0060342C (name is ours)
void TalkSystem::Finish()
{
    if (m_State == STATE_KEEP) {
        m_IsEnd = 1;
        m_NextState = STATE_CLOSE;
    } else {
        Reset();
        m_NextState = STATE_IDLE;
    }
}

// 0x00603464 (name is ours)
void TalkSystem::UpdateKeep()
{
    if (m_QuitMode != QUIT_NONE) {
        Quit();
        return;
    }
    if (m_IsChoice) {
        m_NextState = STATE_CHOICE;
        return;
    }
    if (m_IsJumped == 0) {
        return;
    }
    if (IsJumpPending()) {
        m_pSeq->m_State = TalkSeq::STATE_NONE;
        m_NextState = STATE_LOAD;
        ResetText(false);
        m_pSeq->Reset();
        m_IsInputLocked = 0;
        m_ChoiceKind = 5;
        m_IsChoice = 0;
        m_Flags &= ~FLAG_CHOICE;
    }
}

// 0x00603554 (name is ours)
void TalkSystem::LeaveIdle()
{
    // empty (in the original too)
}

// 0x00603558 (name is ours)
void TalkSystem::LeaveLoad()
{
    // empty (in the original too)
}

// 0x0060355C (name is ours)
void TalkSystem::EnterKeep()
{
    // empty (in the original too)
}

// 0x00603560 (name is ours)
void TalkSystem::ResetText(bool isResetSeq)
{
    ClearPage();
    if (isResetSeq) {
        TalkSeq* seq = m_pSeq;
        seq->m_LineNum = 0;
        seq->m_Timer.count = 0;
        seq->m_Timer.time = 0.0f;
        seq->m_IsCenterPending = 1;
        seq->m_IsStylePending = 1;
        seq->m_HasText = 0;
        seq->m_pMain->Clear();
        seq->m_pRuby->Clear();
        seq->m_Song.isDone = 0;
        seq->m_Song.isStopped = 0;
    }
    *m_pDate = *time::GetCurrentDate();
    m_pState->isOpenSound = 1;
}

// 0x00603604 (name is ours)
void TalkSystem::UpdateChoice()
{
    if (m_NetTimeMax < 0 &&
        netgame::NetGameMgr::s_pInstance->m_LockMgr.IsWaitingForCmd(netgame::LockMgr::CMD_TOUR_END)) {
        m_NetTimeMax = 180;
        m_NetTime = -1;
        m_IsNetAuto = 1;
    }
    CheckNet(STATE_CHOICE);
    if (m_IsNetAuto) {
        ChoiceStandardWin* choice = m_pChoice;
        if (choice->m_RequestCursor < 0) {
            if (m_NetTime == -1 && m_NetTimeMax >= 0) {
                m_NetTime = m_NetTimeMax;
            }
            if (m_NetTime > 0) {
                m_NetTime -= 2;
                if (m_NetTime <= 0) {
                    m_NetTime = 0;
                }
                if (m_AutoTime == 0 || m_NetTime == 0) {
                    s32 cancel = choice->m_CancelIndex;
                    if (choice->GetItem(cancel) != NULL) {
                        ButtonActionControl& control = choice->m_Window.m_Control;
                        if (!control.IsSelectOkDone() && !control.IsSelectOkOngoing() &&
                            !choice->m_Window.IsSelectDone()) {
                            choice->m_RequestCursor = cancel;
                        }
                    } else {
                        choice->m_RequestCursor = 0;
                    }
                }
            }
        }
    }
    if (!m_pChoice->IsIdle()) {
        return;
    }
    if (m_ChoiceTime == 0) {
        bool isReacting = false;
        const ChoiceItem* item = m_pChoice->GetChosenItem();
        s32 cursor = m_pChoice->GetCursor();
        Jump(item->m_Value, NULL, NULL, 3);
        if (static_cast<u32>(item->m_Sound) < 47) {
            isReacting = Func5C8F30(item->m_Sound, 0);
        }
        m_pRecept->OnChoice(item, cursor);
        if (!isReacting) {
            m_ChoiceTime = 14;
        }
    }
    m_ChoiceTime++;
    if (m_ChoiceTime >= 15) {
        if (m_QuitMode == QUIT_NONE) {
            EndMessage();
        } else {
            Quit();
        }
    }
}

// 0x0060383C (name is ours)
void TalkSystem::UpdateWrite()
{
    if (!m_IsQuitForced && !m_IsInputLocked) {
        if (Controller::IsHold(2) || Controller::IsHold(0x2000) || Controller::IsHold(0x4000) ||
            (Controller::IsTrig(0x8000) && (BsScene::s_ChangeRequest.pScene == NULL ||
                                            BsScene::s_ChangeRequest.pScene->m_Index != PROC_SC_STAGE_MINI_GAME_0))) {
            m_pSeq->m_IsFastRequested = 1;
        }
        if (!m_IsTextStopped) {
            m_pSeq->SetText2(&MsgEngine::s_pNoStop);
        }
        u32 mood = 0;
        ITalkRecept* recept = m_pState->pOwner->m_pRecept;
        if (recept != NULL) {
            u32 value = recept->GetMood();
            if (value < 6) {
                mood = value;
            }
        }
        SoundMgr::SSetSpeechMood(mood);
    }
    u32 pageTime = m_PageTime;
    u8 seqState = m_pSeq->m_State;
    bool isEnd = seqState == TalkSeq::STATE_END;
    bool isFull = seqState == TalkSeq::STATE_FULL;
    bool isStop = seqState == TalkSeq::STATE_STOP;
    bool isNone = seqState == TalkSeq::STATE_NONE;
    bool isLate = pageTime > 2;
    if (isEnd || m_IsQuitForced || m_IsInputLocked) {
        m_PageTime = pageTime + 1;
    }
    if (isFull || isLate || isStop || isNone) {
        if (isLate) {
            u8 value = m_pAttribute->pData[4];
            m_pRecept->OnPageEnd(value < 18 ? value : 0);
        }
        m_NextState = STATE_PAGE;
    }
}

// 0x006039D4 (name is ours)
void TalkSystem::UpdatePage()
{
    if (m_IsQuitForced) {
        Quit();
        return;
    }
    if (m_NetTimeMax < 0 &&
        netgame::NetGameMgr::s_pInstance->m_LockMgr.IsWaitingForCmd(netgame::LockMgr::CMD_TOUR_END)) {
        m_NetTimeMax = 180;
        m_NetTime = -1;
        m_IsNetAuto = 1;
    }
    CheckNet(STATE_PAGE);
    if (m_AutoTime > 0) {
        m_AutoTime -= 2;
        if (m_AutoTime <= 0) {
            m_AutoTime = 0;
        }
    }
    if (m_NetTime == -1 && m_NetTimeMax >= 0) {
        m_NetTime = m_NetTimeMax;
    }
    if (m_NetTime > 0) {
        m_NetTime -= 2;
        if (m_NetTime <= 0) {
            m_NetTime = 0;
        }
    }
    if (m_WaitIconTime > 0) {
        m_WaitIconTime--;
        if (m_WaitIconTime == 0) {
            m_WaitIconMode = 0;
            m_pWin->m_Window.SetWaitIconVisible(false);
        }
    }
    m_pSeq->SetText2(&MsgEngine::s_pNoStop);
    u8 seqState = m_pSeq->m_State;
    bool isFull = seqState == TalkSeq::STATE_FULL;
    bool isEnd = seqState == TalkSeq::STATE_END;
    bool isStop = seqState == TalkSeq::STATE_STOP;
    if (!isFull && !isEnd && !isStop && !m_IsInputLocked) {
        return;
    }
    bool isTouch = Controller::IsTrig(0x8000) && (BsScene::s_ChangeRequest.pScene == NULL ||
                                                  BsScene::s_ChangeRequest.pScene->m_Index != PROC_SC_STAGE_MINI_GAME_0);
    bool isA = Controller::IsTrig(1);
    bool isB = Controller::IsTrig(2);
    bool isInput = isTouch | isA | isB;
    bool isAuto = m_AutoTime >= 0;
    s8 isAutoOnly = m_IsAutoOnly;
    bool isTimeUp = m_AutoTime == 0 || m_NetTime == 0;
    if (m_WaitIconTime != 0 || (!m_IsJumped && (isInput & ~isAutoOnly) == 0 && !isTimeUp)) {
        if (m_WaitIconTime == 0 && (isAuto & isAutoOnly) == 0 && m_WaitIconMode == 0) {
            m_pWin->m_Window.SetPageIconVisible(true);
        }
        return;
    }
    if ((isInput & ~m_IsJumped) != 0) {
        SoundMgr::PlaySound(SE_SYS_WIN_TEXT_PAGE_INC, 0.0f);
    }
    if (isEnd || m_IsInputLocked) {
        if (m_IsChoice) {
            m_NextState = STATE_CHOICE;
        } else if (m_QuitMode == QUIT_NONE) {
            EndMessage();
        } else {
            Quit();
        }
        return;
    }
    if (isFull) {
        m_pSeq->m_State = TalkSeq::STATE_WRITE;
        m_NextState = STATE_WRITE;
        ResetText(true);
        return;
    }
    m_pSeq->m_State = TalkSeq::STATE_WRITE;
    m_NextState = STATE_WRITE;
    ClearPage();
}

// 0x00603CD0 (name is ours)
void TalkSystem::LeaveKeep()
{
    // empty (in the original too)
}

// 0x00603CD4 (name is ours)
void TalkSystem::EnterChoice()
{
    m_pWin->m_Window.SetPageIconVisible(false);
    m_pChoice->Begin(m_pPhrase, m_pRecept, m_pSeq);
    m_ChoiceTime = 0;
    if (m_IsNetAuto && m_NetTimeMax >= 0) {
        m_NetTime = m_NetTimeMax;
    }
}

// 0x00603D2C (name is ours)
void TalkSystem::EnterWrite()
{
    m_PageTime = 0;
    TalkState* state = m_pState;
    if (state->isOpenSound) {
        if ((state->flags & TalkState::FLAG_SILENT) == 0) {
            SoundMgr::SQueueSpeechChar(167, 100);
        }
        state->isOpenSound = 0;
    }
}

// 0x00603D68 (name is ours)
void TalkSystem::EnterPage()
{
    if (m_WaitIconMode != 0) {
        m_pWin->m_Window.SetWaitIconVisible(true);
        SeID sound;
        if (m_WaitIconMode == 2) {
            sound = SE_SYS_WAITING_CONNECT;
        } else if (m_WaitIconMode == 3) {
            sound = SE_SYS_WAITING_SILENT;
        } else {
            sound = SE_SYS_WAITING_SAVE;
        }
        m_pWin->m_Window.m_WaitIcon.m_Sound = sound;
    }
    if (m_NetTimeMax >= 0) {
        m_NetTime = m_NetTimeMax;
    }
}

// 0x00603DCC (name is ours)
void TalkSystem::ClearJump()
{
    m_pJump->Clear();
}

// 0x00603E2C (name is ours)
void TalkSystem::UpdateClosed()
{
    if (m_IsQuitNext && !m_pJump->IsEnd() && m_NextState == STATE_NUM && m_State == STATE_CLOSED) {
        m_NextState = STATE_LOAD;
    }
}

// 0x00603E70 (name is ours)
void TalkSystem::LeaveChoice()
{
    // empty (in the original too)
}

// 0x00603E74 (name is ours)
void TalkSystem::LeavePage()
{
    // empty (in the original too)
}

// 0x00603E78 (name is ours)
void TalkSystem::LeaveWrite()
{
    // empty (in the original too)
}

// 0x00603E84 (name is ours)
void TalkSystem::EnterClosed()
{
    // empty (in the original too)
}

// 0x00603E88 (name is ours)
void TalkSystem::EndMessage()
{
    m_pRecept->OnEnd();
    if (IsJumpPending()) {
        m_pSeq->m_State = TalkSeq::STATE_NONE;
        m_NextState = STATE_LOAD;
        ResetText(false);
        m_pSeq->Reset();
    } else {
        m_pSeq->m_State = TalkSeq::STATE_NONE;
        if (m_IsCloseAtEnd) {
            m_NextState = STATE_CLOSE;
            ResetText(false);
            m_IsInputLocked = 0;
            m_IsChoice = 0;
            m_ChoiceKind = 5;
            m_Flags &= ~FLAG_CHOICE;
            m_pState->isOpenSound = 0;
            HideWaitIcon(false);
            return;
        }
        m_NextState = STATE_KEEP;
        ResetText(false);
    }
    m_IsInputLocked = 0;
    m_ChoiceKind = 5;
    m_IsChoice = 0;
    m_Flags &= ~FLAG_CHOICE;
}

// 0x00603FCC (name is ours)
void TalkSystem::LeaveClosed()
{
    // empty (in the original too)
}

// 0x00603FD0 (name is ours)
bool TalkSystem::Continue()
{
    if (m_State == STATE_CLOSED) {
        m_NextState = STATE_LOAD;
        return true;
    }
    return false;
}

// 0x00603FEC (name is ours)
void TalkSystem::Jump(s32 label, const char* file, const IWord* name, u8 mode)
{
    m_pJump->m_Number = label;
    m_pJump->m_Flags = TalkLabel::FLAG_NUMBER;
    if (file != NULL) {
        m_pJump->m_File.copy(sead::SafeStringBase<char>(file), -1);
    }
    if (name != NULL) {
        TalkLabel* jump = m_pJump;
        static_cast<IWord&>(jump->m_Name).Copy(*name);
        jump->m_NameMode = mode;
    }
}

// 0x00604068 (name is ours)
void TalkSystem::CheckNet(u8 state)
{
    if (BsDemoCancelMgr::s_pInstance == NULL || !BsDemoCancelMgr::IsActiveStage() ||
        !BsDemoCancelMgr::s_pInstance->m_IsActive) {
        return;
    }
    BsDemoCancelMgr* mgr = BsDemoCancelMgr::s_pInstance;
    if (state == STATE_PAGE) {
        if (mgr->m_State == 1 || mgr->m_State == 2) {
            SetTimer(1, false);
            return;
        }
        s32 index = m_pMessage->m_Index;
        bool isError = m_pMessage->m_File.isEqual(sead::SafeStringBase<char>("SYS_Error")) && index == 22;
        if (isError || m_Flags != 0) {
            SetTimer(1, false);
            return;
        }
        RequestQuit(0, true, false);
    } else if (state == STATE_CHOICE) {
        if (mgr->m_State != 2 || mgr->IsTimeOver()) {
            SetTimer(1, true);
        }
    }
}

// 0x0060440C (name is ours)
void TalkSystem::RequestQuit(u8 mode, bool isForced, bool isNext)
{
    if (CanQuit(isForced)) {
        m_QuitMode = mode;
        m_IsQuitForced = isForced;
        m_IsQuitNext = isNext;
    }
}

// 0x0060443C (name is ours)
void TalkSystem::SetOpenSound(u8 sound)
{
    m_pWin->m_OpenSound = sound;
}

// 0x00604448 (name is ours)
void TalkSystem::Quit()
{
    m_pRecept->OnEnd();
    m_pSeq->m_State = TalkSeq::STATE_NONE;
    m_NextState = STATE_CLOSE;
    ResetText(false);
    m_IsInputLocked = 0;
    m_IsChoice = 0;
    m_ChoiceKind = 5;
    m_Flags &= ~FLAG_CHOICE;
    m_pState->isOpenSound = 0;
}

// 0x006044A8 (name is ours)
void TalkSystem::SetCloseSound(u8 sound)
{
    m_pWin->m_CloseSound = sound;
}

// 0x006044BC (name is ours)
void TalkSystem::SetChoiceNum(s32 num, s32 cancel)
{
    m_pChoice->SetNum(num, cancel);
}

// 0x006044E8 (name is ours)
void TalkSystem::StopChoice()
{
    m_pChoice->Stop();
}

// 0x006044F8 (name is ours)
void TalkSystem::SetChoiceItem(s32 index, const ChoiceItemData* data)
{
    m_pChoice->SetItem(index, data);
}

// 0x00604528 (name is ours)
void TalkSystem::StartChoice(s32 num, s32 cancel)
{
    m_IsChoice = 1;
    m_Flags |= FLAG_CHOICE;
    m_pChoice->Start(num, cancel);
}

// 0x00604580 (name is ours)
void TalkSystem::StartChoice()
{
    m_IsChoice = 1;
    m_Flags |= FLAG_CHOICE;
    m_pChoice->Start();
}

// 0x006045B4 (name is ours)
void TalkSystem::Release()
{
    ReleaseMessage();
    for (s32 i = 0; i < m_Files.m_Num; i++) {
        m_Files.m_Files[i].Clear();
    }
    m_Files.m_Num = 0;
    ChoiceStandardWin* choice = m_pChoice;
    choice->m_IsSetup = 0;
    for (s32 i = 0; i < ChoiceStandardWin::RENDER_NUM; i++) {
        choice->m_pRenders[i]->Clear();
    }
    choice->m_Window.Release();
    m_pWin->Release();
}

// 0x006047C4 (name is ours)
void TalkSystem::SetWaitIcon(u8 mode)
{
    m_WaitIconMode = mode;
    m_WaitIconTime = -1;
}

// 0x006047D4 (name is ours)
void TalkSystem::Reset()
{
    m_NextState = STATE_NUM;
    m_QuitMode = QUIT_NONE;
    m_State = STATE_IDLE;
    m_IsQuitForced = 0;
    m_IsQuitNext = 0;
    m_Flags = 0;
    m_WaitIconMode = 0;
    m_WaitIconTime = 0;
    m_pWin->m_Window.SetWaitIconVisible(false);
    m_IsJumped = 0;
    m_IsTextStopped = 0;
    m_IsEnd = 0;
    m_IsInputLocked = 0;
    m_IsCloseAtEnd = 1;
    m_IsChoice = 0;
    m_ChoiceTime = 0;
    m_ChoiceKind = 5;
    ReleaseMessage();
    m_pAttribute->Clear();
    m_pSeq->Reset();
    Insert* insert = m_pPhrase->m_pInsert;
    if (insert != NULL) {
        insert->Clear();
    }
    m_pState->flags = 0;
    m_pState->isOpenSound = 0;
    m_pState->isSentenceEnd = 0;
    m_pMessage->Clear();
    m_pJump->Clear();
    m_IsAutoOnly = 0;
    m_AutoTime = -1;
    m_NetTime = -2;
    m_NetTimeMax = -1;
    m_IsNetAuto = 0;
    m_PageTime = 0;
    m_IsLoading = 0;
}

// 0x00604934 (name is ours)
void TalkSystem::SetJumped()
{
    m_IsJumped = 1;
}

// 0x00604950 (name is ours)
void TalkSystem::Update()
{
    (this->*s_UpdateFuncs[m_State])();
    m_StateTime++;
    if (m_NextState != STATE_NUM) {
        (this->*s_LeaveFuncs[m_State])();
        u8 next = m_NextState;
        m_State = next;
        m_NextState = STATE_NUM;
        m_StateTime = 0;
        (this->*s_EnterFuncs[next])();
    }
}

// 0x00604A2C (name is ours)
TalkSystem::TalkSystem(nn::fnd::DateTimeParameters* pDate)
    : m_pDate(pDate), m_State(STATE_IDLE), m_NextState(STATE_NUM), m_QuitMode(QUIT_NONE), m_IsQuitForced(0),
      m_IsQuitNext(0), m_Flags(0), m_StateTime(0), m_WaitIconMode(0), m_WaitIconTime(0), m_IsJumped(0), m_IsEnd(0),
      m_IsTextStopped(0), m_IsInputLocked(0), m_IsCloseAtEnd(1), m_IsChoice(0), m_ChoiceTime(0), m_ChoiceKind(5),
      m_pAttribute(new (std::nothrow) TalkAttribute()), m_pMain(new (std::nothrow) RenderMain(256, 32, 2)),
      m_pRuby(new (std::nothrow) RenderRuby(256, 2, m_pMain)), m_pWin(new (std::nothrow) TalkWin(m_pMain, m_pRuby)),
      m_pMeasure(new (std::nothrow) TalkMeasure(this, m_pWin)),
      m_pSeq(new (std::nothrow) TalkSeq(this, m_pMain, m_pRuby, m_pWin, m_pMeasure)),
      m_pChoice(new (std::nothrow) ChoiceStandardWin()), m_pInsert(new (std::nothrow) TalkInsert(this)),
      m_pPhrase(new (std::nothrow) TalkPhrase(m_pInsert)), m_pState(new (std::nothrow) TalkState(this)),
      m_pMessage(new (std::nothrow) TalkLabel()), m_pJump(new (std::nothrow) TalkLabel()), m_pRecept(NULL),
      m_IsAutoOnly(0), m_AutoTime(-1), m_NetTime(-2), m_NetTimeMax(-1), m_IsNetAuto(0), m_PageTime(0), m_IsLoading(0),
      m_pSong(new (std::nothrow) TalkSong(this))
{
}

// 0x00604EA0 (name is ours)
TalkSystem::~TalkSystem()
{
    delete m_pSong;
    delete m_pJump;
    delete m_pMessage;
    delete m_pState;
    delete m_pPhrase;
    delete m_pInsert;
    delete m_pChoice;
    delete m_pSeq;
    delete m_pMeasure;
    delete m_pWin;
    delete m_pRuby;
    delete m_pMain;
    delete m_pAttribute;
}

// 0x00605020 (name is ours)
void TalkState::OnPeriod(u32 c, const char16* const* cur, const char16* const* end)
{
    bool isEnd = false;
    if (c == '.') {
        if (isSentenceEnd) {
            isEnd = true;
        } else {
            PeriodCountEngine engine;
            if (engine.Count(cur, end) >= 1) {
                isEnd = true;
            }
        }
    }
    isSentenceEnd = isEnd;
}

// 0x006050C0 (name is ours)
u16 CharToSpeechSound(u32 c)
{
    return CharToSpeechSound(c, 0);
}

// 0x006050C8 (name is ours)
u16 CharToSpeechSound(u32 c, u32)
{
    if (c - '0' <= 9) {
        c += 39;
    } else if (c - 0x3041 <= 85) {
        c -= 0x3040;
    } else if (c - 0x30A1 <= 85) {
        c -= 0x30A0;
    } else if (c - 'a' <= 25) {
    } else if (c - 'A' <= 25) {
        c += 32;
    } else if (c - 0xFF41 <= 25) {
        c -= 0xFEE0;
    } else if (c - 0xFF21 <= 25) {
        c -= 0xFEC0;
    } else if (c != 0xF7 && c - 0xE0 <= 30) {
        c -= 101;
    } else if (c != 0xD7 && c - 0xC0 <= 30) {
        c -= 69;
    } else if (c - 0x3B1 <= 24) {
        c -= 0x305;
    } else if (c - 0x391 <= 24) {
        c -= 0x2E5;
    } else if (ssys::ma::IsInRange(c, 0xAC00, 0xD7A3)) {
    } else if (ssys::ma::IsInRange(c, 0x1100, 0x11F9)) {
    } else if (ssys::ma::IsInRange(c, 0x3131, 0x318E)) {
    } else {
        u32 sound = 218;
        for (u32 i = 0; i < sizeof(s_SpeechSounds) / sizeof(s_SpeechSounds[0]); i++) {
            if (s_SpeechSounds[i].c == c) {
                sound = s_SpeechSounds[i].sound;
                break;
            }
        }
        return sound;
    }
    if (c >= 0x10000) {
        c = 218;
    }
    return c;
}

// 0x0060526C (name is ours)
void TalkState::SetSpeaker(const TalkAttribute* attribute)
{
    sead::CheckedArray<u8, 4> voices = {};
    ITalkRecept* recept = pOwner->m_pRecept;
    if (recept != NULL) {
        recept->GetVoices(&voices);
    }
    SoundMgr::SSetSpeechVoices(voices);
    SoundMgr::SSetSpeechCharFrames(2);
    u8 speaker = attribute->GetSpeaker();
    if (speaker < TalkAttribute::SPEAKER_NUM) {
        u8 window = TalkSeq::s_SpeakerWindows[speaker];
        if (window != 0 && window != 3) {
            flags |= FLAG_FIXED_WINDOW;
        } else {
            flags &= ~FLAG_FIXED_WINDOW;
        }
    } else {
        flags &= ~FLAG_FIXED_WINDOW;
    }
    speaker = attribute->GetSpeaker();
    if (speaker < TalkAttribute::SPEAKER_NUM && TalkSeq::s_SpeakerInstant[speaker] == 1) {
        flags |= FLAG_INSTANT;
    } else {
        flags &= ~FLAG_INSTANT;
    }
}

// 0x0075DDF8 (name is ours)
bool TalkSystem::GetTextByNumber(IWord* word, s32 number)
{
    if (!m_Message.IsInitialized()) {
        return false;
    }
    sead::FixedSafeString<8> label;
    FormatLabelNumber(&label, number);
    const void* text = m_Message.GetTextByLabel(label.getCString());
    if (text == NULL) {
        return false;
    }
    word->Copy(WordRes(static_cast<const char16*>(text)));
    return true;
}
} // namespace script
