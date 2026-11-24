#include "Other/dComDialogA.h"

#include "Bs/dBsCommonDialog.h"
#include "Bs/dBsMenuMgr.h"
#include "Font/dMgr.h"
#include "Other/dControl1Button.h"
#include "Other/dControl2Button.h"
#include "Other/dFormatBuffer.h"
#include "Other/dPaneVisible.h"
#include "Other/dRegion.h"
#include "Sound/dSoundMgr.h"
#include "Sv/dSvApi.h"
#include "Bs/dBsSvMgr.h"
#include "Sv/dSvFgName.h"
#include "Sv/dSvPersonalId.h"
#include "Sv/dSvPlayer.h"
#include "nw/lyt/lyt_TextBox.h"
#include "script/dDesignWords.h"
#include "script/dPhrase.h"
#include "script/dRenderRubySet.h"
#include "script/dScriptExtern.h"
#include "script/dUiMgr.h"
#include "script/dWordFix.h"
#include "script/dWordNumDigit.h"
#include "ssys/ma/lyt/dLayoutMgr.h"
#include "ssys/ma/lyt/lyt_Api.h"
#include "ssys/ma/dAllocator.h"
#include "nw/lyt/lyt_Picture.h"
#include "sead/seadNew.h"

namespace {
// the command cache sizes and the layers of the layouts and of the buttons
const u32 DIALOG_CMD_CACHE_SIZE = 19456;
const u32 WAIT_CMD_CACHE_SIZE = 4096;
const u8 DIALOG_LAYER = 160;
const u8 WAIT_LAYER = 162;
const u8 BUTTON_LAYER = 161;
// the input of the buttons (ButtonActionControl::Flag2: touch and the shortcuts of the nodes)
const u16 BUTTON_FLAGS2 = ButtonActionControl::ENABLE_TOUCH | ButtonActionControl::ENABLE_NODE_SHORTCUT;
// the label files of the buttons
const char* const BUTTON_FILE = "SYS_2D_UI";
const char* const DIALOG_BUTTON_FILE = "SYS_2D_Dialog_Button";
// the number of digits of the inserted numbers
const u32 NUMBER_DIGITS = 10;
} // namespace

// 0x009AC068
DialogTextRequest g_DialogTextRequest;
// held sound whose pan the dialog resets (starts as the save waiting sound)
// 0x00952674
u32 g_Unknown952674 = SE_SYS_WAITING_SAVE;

// the states with their names (debug); the machine starts idle
// 0x00858AA8
const ComDialogA::StateInfo ComDialogA::s_StateInfos[7] = {
    {"idle", &ComDialogA::Enter_Idle, &ComDialogA::State_Idle},
    {"in", &ComDialogA::Enter_In, &ComDialogA::State_In},
    {"out", &ComDialogA::Enter_Out, &ComDialogA::State_Out},
    {"icon in", &ComDialogA::Enter_IconIn, &ComDialogA::State_IconIn},
    {"icon out", &ComDialogA::Enter_IconOut, &ComDialogA::State_IconOut},
    {"wait", &ComDialogA::Enter_Wait, &ComDialogA::State_Wait},
    {"pagechnage", &ComDialogA::Enter_PageChange, &ComDialogA::State_PageChange},
};

// 0x001A9920
void DialogTextRequest::Clear()
{
    index = -1;
    number = -1;
    arg0 = 0;
    _unknown0x10 = -1;
    arg1 = 0;
    pressed = -1;
    isCancel = true;
}

// 0x001AA4FC
ComDialogA::ComDialogA()
    : m_pRender(NULL), m_pOutAnim(NULL), m_pOneButton(NULL), m_pTwoButtons(NULL), m_Unknown0x1854(0), m_Unknown0x1858(0),
      m_IsBlackShown(false), m_IsIconShown(false), m_IsCloseRequested(false), m_IsDecided(true),
      m_OpenSe(SE_SYS_DIALOGUE_OPEN), m_Kind(KIND_NONE), m_InfoKind(-1), m_NextState(NULL)
{
}

// 0x001AA6B8 slot 0x00
// 0x001AA63C (deleting dtor)
ComDialogA::~ComDialogA()
{
    // (only the members are destroyed)
}

// 0x001A9F44
bool ComDialogA::Load()
{
    if (!m_DialogReader.ReadArc("Layout/common/cmn_dialog_A.arc")) {
        return false;
    }
    // (the reader at +0x34 of the object of g_pUnknown94F4C8 has save_wait)
    m_WaitReader.ShareArc(reinterpret_cast<const ssys::ma::lyt::ArcResAccReader*>(g_pUnknown94F4C8 + 52));
    switch (g_CommonDialogArg.kind) {
    case KIND_ONE_BUTTON:
        if (m_pOneButton == NULL) {
            m_pOneButton = new (ssys::ma::lyt::GetAllocator()->m_pHeap, 4) Control1Button;
        }
        break;
    case KIND_TWO_BUTTONS:
        if (m_pTwoButtons == NULL) {
            m_pTwoButtons = new (ssys::ma::lyt::GetAllocator()->m_pHeap, 4) Control2Button;
        }
        break;
    }
    return true;
}

// 0x001A994C
bool ComDialogA::Initialize()
{
    m_DialogLayout.SetArcResAccReader(&m_DialogReader);
    m_WaitLayout.SetArcResAccReader(&m_WaitReader);
    const font::Mgr* fonts = font::Mgr::s_pInstance;
    m_DialogReader.GetResourceAccessorVRAM()->RegistFont(fonts->GetFontName(font::GARDEN_MSG_16)->getCString(),
                                                         fonts->GetFont(font::GARDEN_MSG_16));
    m_DialogReader.GetResourceAccessorVRAM()->RegistFont(fonts->GetFontName(font::GARDEN_RUBY)->getCString(),
                                                         fonts->GetFont(font::GARDEN_RUBY));
    m_DialogLayout.Build("cmn_dialog_A.bclyt", NULL, DIALOG_CMD_CACHE_SIZE);
    g_FormatBuffer.format("%s.bclyt", "save_wait");
    m_WaitLayout.Build(g_FormatBuffer.getCString(), NULL, WAIT_CMD_CACHE_SIZE);
    m_DialogLayout.m_Layer = DIALOG_LAYER;
    m_WaitLayout.m_Layer = WAIT_LAYER;
    m_InAnim.Initialize("cmn_dialog_A_in_00.bclan", &m_DialogReader);
    m_OutOkAnim.Initialize("cmn_dialog_A_out_ok_00.bclan", &m_DialogReader);
    m_OutNgAnim.Initialize("cmn_dialog_A_out_ng_00.bclan", &m_DialogReader);
    g_FormatBuffer.format("%s_%s.bclan", "save_wait", "in");
    m_WaitInAnim.Initialize(g_FormatBuffer.getCString(), &m_WaitReader);
    g_FormatBuffer.format("%s_%s.bclan", "save_wait", "out");
    m_WaitOutAnim.Initialize(g_FormatBuffer.getCString(), &m_WaitReader);
    g_FormatBuffer.format("%s_%s.bclan", "save_wait", "wait_save");
    m_WaitLoopAnim.Initialize(g_FormatBuffer.getCString(), &m_WaitReader);
    m_WaitLayout.BindAnimation(&m_WaitLoopAnim);
    SetupLayout();
    Start("ComDialogA", s_StateInfos, 7);
    return true;
}

// 0x001AA0A4
void ComDialogA::SetupLayout()
{
    m_pRender = new (ssys::ma::lyt::GetAllocator()->m_pHeap, 4) script::RenderRubySet(512, 1);
    SetFlagVisible(m_DialogLayout.FindPane("N_anime_00"), false);
    nw::lyt::TextBox* text;
    nw::lyt::TextBox* ruby = NULL;
    if (GetRegionIndex() != 0) {
        text = m_DialogLayout.FindTextBox("T_msg_00_W");
    } else {
        text = m_DialogLayout.FindTextBox("T_msg_00");
        ruby = m_DialogLayout.FindTextBox("RT_msg_00");
    }
    m_pRender->SetTextBoxes(text, ruby, NULL, NULL);
    m_Kind = g_CommonDialogArg.kind;
    switch (g_CommonDialogArg.kind) {
    case KIND_ONE_BUTTON:
        m_pOneButton->Initialize();
        m_pOneButton->m_pLayout = &m_DialogLayout;
        m_pOneButton->SetDecide(true);
        m_pOneButton->SetText(g_CommonDialogArg.labels[3], BUTTON_FILE);
        m_pOneButton->SetPane(m_DialogLayout.FindPane("N_pos_C"));
        m_pOneButton->m_Flags2 = BUTTON_FLAGS2;
        m_pOneButton->SetInput(g_CommonDialogArg.input == 0 ? 1 : 2);
        m_pOneButton->m_pLayout->m_Layer = BUTTON_LAYER;
        m_pOneButton->SetFlags(ButtonActionControl::DISABLED);
        break;
    case KIND_TWO_BUTTONS: {
        m_pTwoButtons->Initialize();
        m_pTwoButtons->m_pLayout = &m_DialogLayout;
        m_pTwoButtons->SetDecide(false, true);
        m_pTwoButtons->SetText(g_CommonDialogArg.labels[1], g_CommonDialogArg.labels[3], BUTTON_FILE);
        nw::lyt::Pane* right = m_DialogLayout.FindPane("N_pos_R");
        m_pTwoButtons->SetPanes(m_DialogLayout.FindPane("N_pos_L"), right);
        m_pTwoButtons->m_Flags2 = BUTTON_FLAGS2;
        m_pTwoButtons->SetInput(2, 1);
        m_pTwoButtons->m_pLayout->m_Layer = BUTTON_LAYER;
        m_pTwoButtons->SetFlags(ButtonActionControl::DISABLED);
        break;
    }
    }
    Compose();
    m_IsBlackShown = false;
    m_IsIconShown = g_CommonDialogArg.isIconShown;
    switch (g_CommonDialogArg.icon) {
    case 0:
        m_OpenSe = SE_SYS_DIALOGUE_OPEN;
        break;
    case 1:
        m_OpenSe = SE_SYS_DIALOGUE_ERROR_OPEN;
        break;
    case 2:
        m_OpenSe = SE_SYS_FRIEND_SHOUT_ON_OK;
        break;
    case 3:
        m_OpenSe = SE_SYS_FRIEND_SHOUT_OFF_OK;
        break;
    case 4:
        m_OpenSe = SE_SYS_PARTY_ADD_BEST_OK;
        break;
    case 5:
        m_OpenSe = SE_SYS_EXH_FAVORITE_ON_OK;
        break;
    case 6:
        m_OpenSe = SE_SYS_EXH_FAVORITE_OFF_OK;
        break;
    case 7:
        m_OpenSe = SE_SYS_QR_SCAN_FINISH;
        break;
    }
    m_DialogLayout.FindPane("N_wait_pos_00")->AppendChild(m_WaitLayout.GetRootPane());
    SetFlagVisible(m_WaitLayout.FindPicture("P_icon_00"), false);
}

// 0x001A9C90
void ComDialogA::Open()
{
    const s32 right = g_CommonDialogArg.labels[3];
    switch (g_CommonDialogArg.kind) {
    case KIND_ONE_BUTTON:
        m_pOneButton->SetText(right, BUTTON_FILE);
        m_pOneButton->ResetSelectAll();
        m_pOneButton->Reset();
        break;
    case KIND_TWO_BUTTONS:
        m_pTwoButtons->SetText(g_CommonDialogArg.labels[1], right, BUTTON_FILE);
        m_pTwoButtons->ResetSelectAll();
        m_pTwoButtons->Reset();
        break;
    }
    SetupText();
    m_IsIconShown = g_CommonDialogArg.isIconShown;
}

// 0x001A9D58
bool ComDialogA::Finalize()
{
    BsMenuMgr::Func6D32D0();
    g_DialogTextRequest.index = -1;
    g_DialogTextRequest.number = -1;
    g_DialogTextRequest.arg0 = 0;
    g_DialogTextRequest.arg1 = 0;
    g_DialogTextRequest.isCancel = true;
    m_DialogLayout.FindPane("N_wait_pos_00")->RemoveChild(m_WaitLayout.GetRootPane());
    m_DialogReader.Clear();
    m_DialogLayout.UnbindAllAnimation();
    m_WaitLayout.UnbindAllAnimation();
    if (m_pRender != NULL) {
        delete m_pRender;
        m_pRender = NULL;
    }
    if (m_pOneButton != NULL) {
        m_pOneButton->Finalize();
        delete m_pOneButton;
        m_pOneButton = NULL;
    } else if (m_pTwoButtons != NULL) {
        m_pTwoButtons->Finalize();
        delete m_pTwoButtons;
        m_pTwoButtons = NULL;
    }
    return true;
}

// 0x001A9E70
void ComDialogA::Calc()
{
    Call();
    if (m_pOneButton != NULL) {
        m_pOneButton->Update();
    }
    if (m_pTwoButtons != NULL) {
        m_pTwoButtons->Update();
    }
    m_DialogLayout.Animate();
}

// 0x0056A934
void ComDialogA::Draw()
{
    ssys::ma::lyt::LayoutMgr::Get()->Register(&m_DialogLayout, true);
}

// 0x001AA430
void ComDialogA::StartIn()
{
    if (GetState() == &ComDialogA::State_Idle) {
        Change(&ComDialogA::State_In);
    }
}

// 0x001AA484
void ComDialogA::StartOut()
{
    if (GetState() != &ComDialogA::State_Idle) {
        return;
    }
    if (m_IsIconShown) {
        Change(&ComDialogA::State_IconOut);
    } else {
        Change(&ComDialogA::State_Out);
    }
}

// 0x001A9394
void ComDialogA::Close()
{
    if (GetState() == &ComDialogA::State_Idle || GetState() == &ComDialogA::State_Wait) {
        if (m_Kind == KIND_ONE_BUTTON) {
            BsMenuMgr::s_Result = 1;
        } else if (m_Kind == KIND_TWO_BUTTONS) {
            BsMenuMgr::s_Result = 0;
        }
        if (m_IsIconShown) {
            Change(&ComDialogA::State_IconOut);
        } else {
            Change(&ComDialogA::State_Out);
        }
        return;
    }
    m_IsCloseRequested = true;
}

// 0x0070B5D4
u8 ComDialogA::GetDecidedIndex() const
{
    if (m_pOneButton != NULL) {
        return m_pOneButton->GetDecidedIndex();
    }
    if (m_pTwoButtons != NULL) {
        return m_pTwoButtons->GetDecidedIndex();
    }
    return 0;
}

// 0x0070B60C
bool ComDialogA::IsSelectOkDone() const
{
    if (m_pOneButton != NULL) {
        return m_pOneButton->IsSelectOkDone();
    }
    if (m_pTwoButtons != NULL) {
        return m_pTwoButtons->IsSelectOkDone();
    }
    return false;
}

// 0x001A7E0C
void ComDialogA::Compose()
{
    const s32 index = g_DialogTextRequest.index;
    const s32 number = g_DialogTextRequest.number;
    if (index == -1 && number == -1) {
        if (g_CommonDialogArg.labels[0] != -1) {
            SetupText();
        }
        return;
    }
    script::DialogCompo::Callback callback;
    switch (g_CommonDialogArg.wordKind) {
    case -6:
        callback = &ComDialogA::InsertWord;
        break;
    case -5:
        callback = &ComDialogA::InsertItem;
        break;
    case -4:
        callback = &ComDialogA::InsertNumber;
        break;
    case -3:
        callback = &ComDialogA::InsertPersonNumber;
        break;
    case -2:
        callback = &ComDialogA::InsertPerson;
        break;
    case -1:
        callback = NULL;
        break;
    default:
        callback = &ComDialogA::InsertDesign;
        break;
    }
    if (index != -1) {
        m_Text.ComposeByIndex(index, callback, NULL);
    } else if (number != -1) {
        m_Text.ComposeByNumber(number, callback, NULL);
    }
    SetupButtons();
    m_NextState = &ComDialogA::State_Wait;
}

// 0x001A8834
void ComDialogA::InsertWord(script::UiMgr* ui, void* arg)
{
    ui->GetPhrase()->SetWord2DC(*g_CommonDialogArg.pWord, 0);
}

// 0x001A87D8
void ComDialogA::InsertItem(script::UiMgr* ui, void* arg)
{
    ui->GetPhrase()->SetWord2C0(g_CommonDialogArg.pItem->GetName(NULL), 0);
}

// 0x001A7F18
void ComDialogA::InsertNumber(script::UiMgr* ui, void* arg)
{
    script::WordNumDigit number;
    number.Func312CD4(g_CommonDialogArg.number, NUMBER_DIGITS, NUMBER_DIGITS, 4);
    ui->GetPhrase()->SetTag4_6(number, 0);
}

// 0x001A9768
void ComDialogA::InsertPersonNumber(script::UiMgr* ui, void* arg)
{
    script::WordFix<9> name;
    Func56DE5C(&name, 4);
    MakePlayerName(&name, g_CommonDialogArg.pPerson->playerData.playerName, g_CommonDialogArg.pPerson->playerData.gender & 1);
    ui->GetPhrase()->SetWord2B4(name, 0);
    script::WordFix<9> town;
    MakeTownName(&town, &g_CommonDialogArg.pPerson->townData);
    ui->GetPhrase()->SetWord2BC(town, 0);
    script::WordNumDigit number;
    number.Func312CD4(g_CommonDialogArg.number, NUMBER_DIGITS, NUMBER_DIGITS, 4);
    ui->GetPhrase()->SetTag4_6(number, 0);
}

// 0x001A9600
void ComDialogA::InsertPerson(script::UiMgr* ui, void* arg)
{
    script::WordFix<9> name;
    Func56DE5C(&name, 4);
    MakePlayerName(&name, g_CommonDialogArg.pPerson->playerData.playerName, g_CommonDialogArg.pPerson->playerData.gender & 1);
    ui->GetPhrase()->SetWord2B4(name, 0);
    script::WordFix<9> town;
    MakeTownName(&town, &g_CommonDialogArg.pPerson->townData);
    ui->GetPhrase()->SetWord2BC(town, 0);
}

// 0x001A92D4
void ComDialogA::InsertDesign(script::UiMgr* ui, void* arg)
{
    SvPlayer* player = SvGetCurrentPlayer();
    const u8 slot = Func7133A0(player->patterns, static_cast<u8>(g_CommonDialogArg.wordKind));
    const DesignWords words(static_cast<SvMyDesign*>(Func20C4C8(player->patterns, slot)));
    ui->GetPhrase()->SetWord2B4(words.creator, 0);
    ui->GetPhrase()->SetTag17_5(words.title);
}

// 0x001A8928
void ComDialogA::SetupText()
{
    script::WordFix<512> text;
    script::UiMgr::s_pInstance->Begin(NULL);
    switch (g_CommonDialogArg.wordKind) {
    case -5:
        script::UiMgr::s_pInstance->GetPhrase()->SetWord2C0(g_CommonDialogArg.pItem->GetName(NULL), 0);
        break;
    case -4: {
        script::WordNumDigit number;
        number.Func312CD4(g_CommonDialogArg.number, NUMBER_DIGITS, NUMBER_DIGITS, 4);
        script::UiMgr::s_pInstance->GetPhrase()->SetTag4_6(number, 0);
        break;
    }
    case -3: {
        script::WordNumDigit number;
        number.Func312CD4(g_CommonDialogArg.number, NUMBER_DIGITS, NUMBER_DIGITS, 4);
        script::UiMgr::s_pInstance->GetPhrase()->SetTag4_6(number, 0);
    }
        // fall through
    case -2: {
        script::WordFix<9> name;
        Func56DE5C(&name, 4);
        MakePlayerName(&name, g_CommonDialogArg.pPerson->playerData.playerName,
                       g_CommonDialogArg.pPerson->playerData.gender & 1);
        script::UiMgr::s_pInstance->GetPhrase()->SetWord2B4(name, 0);
        script::WordFix<9> town;
        MakeTownName(&town, &g_CommonDialogArg.pPerson->townData);
        script::UiMgr::s_pInstance->GetPhrase()->SetWord2BC(town, 0);
        break;
    }
    case -1:
        break;
    default: {
        SvPlayer* player = SvGetCurrentPlayer();
        const u8 slot = Func7133A0(player->patterns, static_cast<u8>(g_CommonDialogArg.wordKind));
        const DesignWords words(static_cast<SvMyDesign*>(Func20C4C8(player->patterns, slot)));
        script::UiMgr::s_pInstance->GetPhrase()->SetWord2B4(words.creator, 0);
        script::UiMgr::s_pInstance->GetPhrase()->SetTag17_5(words.title);
        break;
    }
    }
    if (g_CommonDialogArg.callback != NULL) {
        g_CommonDialogArg.callback();
        g_CommonDialogArg.callback = NULL;
    }
    script::UiMgr::s_pInstance->ReadWord(&text, g_CommonDialogArg.pFile, g_CommonDialogArg.labels[0], 0);
    script::UiMgr::s_pInstance->End();
    m_pRender->SetText(&text);
    m_pRender->CommitText();
    nw::lyt::TextBox* textBox = m_DialogLayout.FindTextBox("T_msg_00");
    nw::lyt::TextBox* rubyBox = m_DialogLayout.FindTextBox("RT_msg_00");
    const u8 position = g_CommonDialogArg.isCentered == 1 ? 1 : 0;
    if (textBox != NULL) {
        textBox->SetTextPositionH(position);
    }
    if (rubyBox != NULL) {
        rubyBox->SetTextPositionH(position);
    }
    m_DialogLayout.Animate();
}

// 0x001A8E1C
void ComDialogA::SetupButtons()
{
    // the buttons of the last page go away
    if (m_pOneButton != NULL) {
        m_pOneButton->Finalize();
        delete m_pOneButton;
        m_pOneButton = NULL;
    } else if (m_pTwoButtons != NULL) {
        m_pTwoButtons->Finalize();
        delete m_pTwoButtons;
        m_pTwoButtons = NULL;
    }
    const s32 kind = m_Text.m_Info.kind;
    const s32 flag = m_Text.m_Info.flag;
    const bool isLastPage = m_Text.IsLastPage();
    m_Text.IsFirstPage();
    if (isLastPage && kind == 2) {
        m_pTwoButtons = new (ssys::ma::lyt::GetAllocator()->m_pHeap, 4) Control2Button;
        m_pTwoButtons->Initialize();
        m_pTwoButtons->m_pLayout = &m_DialogLayout;
        m_pTwoButtons->SetDecide(false, true);
        nw::lyt::Pane* right = m_DialogLayout.FindPane("N_pos_R");
        m_pTwoButtons->SetPanes(m_DialogLayout.FindPane("N_pos_L"), right);
        m_pTwoButtons->m_Flags2 = BUTTON_FLAGS2;
        if (flag < 0) {
            m_pTwoButtons->SetInput(0, 1);
        } else if (flag == 0) {
            m_pTwoButtons->SetInput(2, 1);
        } else if (flag == 1) {
            m_pTwoButtons->SetInput(1, 2);
        }
        m_pTwoButtons->m_pLayout->m_Layer = BUTTON_LAYER;
        m_pTwoButtons->SetFlags(ButtonActionControl::DISABLED);
        g_CommonDialogArg.kind = KIND_TWO_BUTTONS;
    } else {
        m_pOneButton = new (ssys::ma::lyt::GetAllocator()->m_pHeap, 4) Control1Button;
        m_pOneButton->Initialize();
        m_pOneButton->m_pLayout = &m_DialogLayout;
        m_pOneButton->SetDecide(true);
        m_pOneButton->SetPane(m_DialogLayout.FindPane("N_pos_C"));
        m_pOneButton->m_Flags2 = BUTTON_FLAGS2;
        m_pOneButton->SetInput(flag < 0 ? 1 : 3);
        m_pOneButton->m_pLayout->m_Layer = BUTTON_LAYER;
        m_pOneButton->SetFlags(ButtonActionControl::DISABLED);
        g_CommonDialogArg.kind = KIND_ONE_BUTTON;
    }
    m_Kind = g_CommonDialogArg.kind;
    if (isLastPage) {
        m_InfoKind = m_Text.m_Info.kind;
        if (m_InfoKind == 1) {
            m_pOneButton->SetText(m_Text.GetButtonLabel(0), DIALOG_BUTTON_FILE);
        } else if (m_InfoKind == 2) {
            const s32 second = m_Text.GetButtonLabel(1);
            m_pTwoButtons->SetText(m_Text.GetButtonLabel(0), second, DIALOG_BUTTON_FILE);
        } else {
            m_pOneButton->SetText(script::DialogCompo::GetCloseLabel(), DIALOG_BUTTON_FILE);
        }
    } else {
        m_pOneButton->SetText(script::DialogCompo::GetNextLabel(), DIALOG_BUTTON_FILE);
    }
    m_pRender->SetText(&m_Text.m_PageText);
    m_pRender->CommitText();
    nw::lyt::TextBox* textBox = m_DialogLayout.FindTextBox("T_msg_00");
    nw::lyt::TextBox* rubyBox = m_DialogLayout.FindTextBox("RT_msg_00");
    const u8 position = g_CommonDialogArg.isCentered == 1 ? 1 : 0;
    if (textBox != NULL) {
        textBox->SetTextPositionH(position);
    }
    if (rubyBox != NULL) {
        rubyBox->SetTextPositionH(position);
    }
    m_DialogLayout.Animate();
}

// 0x001A7F74
void ComDialogA::State_Idle()
{
    if (m_IsIconShown) {
        if (g_Unknown952674 != 0) {
            SoundMgr::SSetHeldSoundPan(g_Unknown952674, 0.0f);
        }
        m_WaitLoopAnim.IncreaseFrame();
        m_WaitLayout.Animate();
    }
    if (m_IsCloseRequested) {
        Close();
    }
}

// 0x001A80C4
void ComDialogA::Enter_Idle()
{
    // empty (in the original too)
}

// 0x001A7FDC
void ComDialogA::State_Wait()
{
    if ((m_pOneButton != NULL || m_pTwoButtons != NULL) &&
        (m_pOneButton != NULL ? m_pOneButton : static_cast<ControlButtonBase*>(m_pTwoButtons))->IsSelectOkDone()) {
        const bool isLastPage = m_Text.IsLastPage();
        m_Text.IsFirstPage();
        if (isLastPage) {
            u8 pressed = 0;
            if (m_pOneButton != NULL || m_pTwoButtons != NULL) {
                pressed = (m_pOneButton != NULL ? m_pOneButton : static_cast<ControlButtonBase*>(m_pTwoButtons))
                              ->GetDecidedIndex();
            }
            g_DialogTextRequest.pressed = pressed;
            BsMenuMgr::s_Result = pressed;
            m_IsDecided = true;
            Change(&ComDialogA::State_Idle);
        } else {
            Change(&ComDialogA::State_PageChange);
        }
        return;
    }
    if (m_IsCloseRequested) {
        Close();
    }
}

// 0x001A80C8
void ComDialogA::Enter_Wait()
{
    m_IsDecided = false;
    SeID okSe = g_CommonDialogArg.okSe;
    SeID cancelSe = static_cast<SeID>(0);
    if (okSe < 0) {
        okSe = SE_SYS_DECIDE;
    }
    SeID buttonSe0 = static_cast<SeID>(0);
    SeID buttonSe1 = static_cast<SeID>(0);
    const bool isLastPage = m_Text.IsLastPage();
    if (isLastPage) {
        buttonSe0 = m_Text.GetButtonSe(0);
        buttonSe1 = m_Text.GetButtonSe(1);
    }
    if (m_pOneButton != NULL) {
        if (isLastPage) {
            m_pOneButton->SetSeOk(buttonSe0 != 0 ? buttonSe0 : okSe);
        } else {
            m_pOneButton->SetSeOk(SE_SYS_DECIDE_S);
        }
    }
    if (m_pTwoButtons != NULL) {
        cancelSe = buttonSe0 != 0 ? buttonSe0 : SE_SYS_CANCEL;
        if (buttonSe1 != 0) {
            okSe = buttonSe1;
        }
        m_pTwoButtons->SetSeOk(cancelSe, okSe);
    }
}

// 0x001A81B0
void ComDialogA::State_In()
{
    if (!m_InAnim.IsDone()) {
        m_InAnim.IncreaseFrame();
        return;
    }
    m_DialogLayout.Animate();
    m_DialogLayout.UnbindAnimation(&m_InAnim, m_DialogLayout.FindGroup("G_dialog_A"), false);
    if (!m_IsBlackShown) {
        m_DialogLayout.UnbindAnimation(&m_InAnim, m_DialogLayout.FindGroup("G_black"), false);
    }
    if (m_pOneButton != NULL) {
        m_pOneButton->SetFlags(ButtonActionControl::DISABLED, false);
    }
    if (m_pTwoButtons != NULL) {
        m_pTwoButtons->SetFlags(ButtonActionControl::DISABLED, false);
    }
    m_IsBlackShown = true;
    if (m_IsIconShown) {
        Change(&ComDialogA::State_IconIn);
    } else if (m_NextState != NULL) {
        Change(m_NextState);
    } else {
        Change(&ComDialogA::State_Idle);
    }
}

// 0x001A8324
void ComDialogA::Enter_In()
{
    SoundMgr::PlaySound(m_OpenSe, 0.0f);
    SetFlagVisible(m_DialogLayout.FindPane("N_anime_00"), true);
    m_DialogLayout.BindAnimation(&m_InAnim, m_DialogLayout.FindGroup("G_dialog_A"), false);
    if (!m_IsBlackShown) {
        m_DialogLayout.BindAnimation(&m_InAnim, m_DialogLayout.FindGroup("G_black"), false);
    }
    m_InAnim.SetFrame(0.0f);
    m_DialogLayout.Animate();
    if (m_pOneButton != NULL) {
        m_pOneButton->SetSeOk(g_CommonDialogArg.okSe >= 0 ? g_CommonDialogArg.okSe : SE_SYS_DECIDE);
        m_pOneButton->SetIn();
    }
    if (m_pTwoButtons != NULL) {
        const SeID okSe = g_CommonDialogArg.okSe >= 0 ? g_CommonDialogArg.okSe : SE_SYS_DECIDE;
        m_pTwoButtons->SetSeOk(g_CommonDialogArg.cancelSe >= 0 ? g_CommonDialogArg.cancelSe : SE_SYS_CANCEL, okSe);
        m_pTwoButtons->SetIn();
    }
    g_DialogTextRequest._unknown0x10 = -1;
    g_DialogTextRequest.pressed = -1;
}

// 0x001A8540
void ComDialogA::State_Out()
{
    if (!m_pOutAnim->IsDone()) {
        m_pOutAnim->IncreaseFrame();
        return;
    }
    m_DialogLayout.Animate();
    m_DialogLayout.UnbindAnimation(m_pOutAnim, m_DialogLayout.FindGroup("G_dialog_A"), false);
    if (g_CommonDialogArg.isLast == 1) {
        m_DialogLayout.UnbindAnimation(m_pOutAnim, m_DialogLayout.FindGroup("G_black"), false);
    }
    SetFlagVisible(m_DialogLayout.FindPane("N_anime_00"), false);
    m_IsCloseRequested = false;
    Change(&ComDialogA::State_Idle);
}

// 0x001A86BC
void ComDialogA::Enter_Out()
{
    m_pOutAnim = BsMenuMgr::s_Result == 0 && g_DialogTextRequest.isCancel == true ? &m_OutNgAnim : &m_OutOkAnim;
    m_DialogLayout.BindAnimation(m_pOutAnim, m_DialogLayout.FindGroup("G_dialog_A"), false);
    if (g_CommonDialogArg.isLast == 1) {
        m_DialogLayout.BindAnimation(m_pOutAnim, m_DialogLayout.FindGroup("G_black"), false);
    }
    m_pOutAnim->SetFrame(0.0f);
    if (m_pOneButton != NULL) {
        m_pOneButton->SetFlags(ButtonActionControl::DISABLED);
    }
    if (m_pTwoButtons != NULL) {
        m_pTwoButtons->SetFlags(ButtonActionControl::DISABLED);
    }
}

// 0x001A84B8
void ComDialogA::State_IconIn()
{
    if (m_WaitInAnim.IsDone()) {
        m_WaitLayout.UnbindAnimation(&m_WaitInAnim);
        Change(&ComDialogA::State_Idle);
    } else {
        m_WaitInAnim.IncreaseFrame();
        m_WaitLayout.Animate();
    }
    if (g_Unknown952674 != 0) {
        SoundMgr::SSetHeldSoundPan(g_Unknown952674, 0.0f);
    }
}

// 0x001A8638
void ComDialogA::Enter_IconIn()
{
    SetFlagVisible(m_WaitLayout.FindPicture("P_icon_00"), true);
    m_WaitLayout.BindAnimation(&m_WaitInAnim);
    m_WaitInAnim.SetFrame(0.0f);
    m_WaitLoopAnim.SetFrame(0.0f);
    m_WaitLayout.Animate();
}

// 0x001A8854
void ComDialogA::State_IconOut()
{
    if (m_WaitOutAnim.IsDone()) {
        m_WaitLayout.UnbindAnimation(&m_WaitOutAnim);
        m_IsIconShown = false;
        Change(&ComDialogA::State_Out);
    } else {
        m_WaitOutAnim.IncreaseFrame();
        m_WaitLayout.Animate();
    }
    if (g_Unknown952674 != 0) {
        SoundMgr::SSetHeldSoundPan(g_Unknown952674, 0.0f);
    }
}

// 0x001A88EC
void ComDialogA::Enter_IconOut()
{
    m_WaitLayout.BindAnimation(&m_WaitOutAnim);
    m_WaitOutAnim.SetFrame(0.0f);
    m_WaitLayout.Animate();
}

// 0x001A9480
void ComDialogA::State_PageChange()
{
    if (!m_pOutAnim->IsDone()) {
        m_pOutAnim->IncreaseFrame();
        return;
    }
    m_DialogLayout.Animate();
    m_DialogLayout.UnbindAnimation(m_pOutAnim, m_DialogLayout.FindGroup("G_dialog_A"), false);
    m_Text.NextPage();
    SetupButtons();
    m_NextState = &ComDialogA::State_Wait;
    Change(&ComDialogA::State_In);
}

// 0x001A9524
void ComDialogA::Enter_PageChange()
{
    m_pOutAnim = BsMenuMgr::s_Result == 0 && g_DialogTextRequest.isCancel == true ? &m_OutNgAnim : &m_OutOkAnim;
    m_DialogLayout.BindAnimation(m_pOutAnim, m_DialogLayout.FindGroup("G_dialog_A"), false);
    m_pOutAnim->SetFrame(0.0f);
    if (m_pOneButton != NULL) {
        m_pOneButton->SetFlags(ButtonActionControl::DISABLED);
    }
    if (m_pTwoButtons != NULL) {
        m_pTwoButtons->SetFlags(ButtonActionControl::DISABLED);
    }
}
