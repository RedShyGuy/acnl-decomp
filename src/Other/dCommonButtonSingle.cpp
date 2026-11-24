#include "Other/dCommonButtonSingle.h"

#include "Bs/dBsMenuMgr.h"
#include "Other/dFormatBuffer.h"
#include "Other/dPaneVisible.h"
#include "Other/dRegion.h"
#include "Sound/dSoundLabel.h"
#include "event/dEventBase.h"
#include "nw/lyt/lyt_Bounding.h"
#include "nw/lyt/lyt_Pane.h"
#include "nw/lyt/lyt_TextBox.h"
#include "script/dUiMgr.h"
#include "ssys/ma/dAllocator.h"
#include "ssys/ma/lyt/dArcResAccReader.h"
#include "ssys/ma/lyt/lyt_Api.h"

namespace {
// the command cache size of the layout
const u32 CMD_CACHE_SIZE = 4;
// the layer of the layout
const u8 LAYOUT_LAYER = 155;
// the hand cursor points at the button a fifth of its height higher
const f32 CURSOR_OFFSET_RATE = 0.2f;
} // namespace

// 0x009AC084
ssys::ma::LoadSplit CommonButtonSingle::s_Archive;

// 0x002E0778
void CommonButtonSingle::StartOut()
{
    m_ButtonLayout.BindAnimation(&m_OutAnim);
    m_OutAnim.SetFrame(0.0f);
    m_pState = &CommonButtonSingle::State_Out;
}

// 0x002E07B8
void CommonButtonSingle::State_In()
{
    if (m_InAnim.IsDone()) {
        m_ButtonLayout.UnbindAnimation(&m_InAnim);
        m_pState = &CommonButtonSingle::State_Idle;
        return;
    }
    m_InAnim.IncreaseFrame();
    m_ButtonLayout.Animate();
}

// 0x002E0814
void CommonButtonSingle::SetDecide(bool isDecide)
{
    m_Index = isDecide;
    if (isDecide) {
        SetSeOk(SE_SYS_DECIDE);
        SetOkSoundEnabled(true);
        return;
    }
    SetSeOk(SE_SYS_CANCEL);
    m_Input = BsMenuMgr::INPUT_B;
    SetOkSoundEnabled(true);
}

// 0x002E0854
void CommonButtonSingle::SetVisible(bool visible)
{
    m_IsVisible = visible;
    if (m_pParentPane != NULL) {
        SetFlagVisible(m_pParentPane, visible);
    }
}

// 0x002E0878
void CommonButtonSingle::State_Out()
{
    if (m_OutAnim.IsDone()) {
        m_ButtonLayout.UnbindAnimation(&m_OutAnim);
        m_pState = &CommonButtonSingle::State_Idle;
        return;
    }
    m_OutAnim.IncreaseFrame();
    m_ButtonLayout.Animate();
}

// 0x002E08D4
void CommonButtonSingle::SetText(s32 index, const char* label)
{
    if (index == -1) {
        return;
    }
    if (m_IsBack) {
        return;
    }
    script::UiMgr::s_pInstance->Begin(NULL);
    script::UiMgr::s_pInstance->ReadWord(&m_Word, label, index, 0);
    script::UiMgr::s_pInstance->End();
}

// 0x002E0948
void CommonButtonSingle::SetPane(nw::lyt::Pane* pane)
{
    nw::lyt::Pane* root = m_ButtonLayout.GetRootPane();
    if (root == NULL) {
        return;
    }
    if (root->m_pParent != NULL) {
        root->m_pParent->RemoveChild(root);
    }
    m_IsActive = false;
    if (pane == NULL) {
        return;
    }
    pane->AppendChild(m_ButtonLayout.GetRootPane());
    m_pParentPane = pane;
    m_IsActive = true;
}

// 0x002E0998
bool CommonButtonSingle::LoadArchive()
{
    ssys::ma::Allocator* allocator = ssys::ma::lyt::GetDeviceAllocator();
    return s_Archive.Read(sead::SafeStringBase<char>("Layout/common/cmn_btn.arc"), allocator->m_pHeap, 128);
}

// 0x002E09F4
void CommonButtonSingle::SetDisabled(bool disabled)
{
    if (m_IsDisabled == disabled) {
        return;
    }
    m_IsDisabled = disabled;
    m_ButtonLayout.BindAnimation(&m_NgAnim);
    if (disabled) {
        m_NgAnim.SetFrame(0.0f);
    } else {
        m_NgAnim.SetLastFrame();
    }
    m_ButtonLayout.Animate();
    m_ButtonLayout.UnbindAnimation(&m_NgAnim);
    m_IsActive = !disabled;
}

// 0x002E0A64
bool CommonButtonSingle::Setup(const char* fontName, const nw::font::Font* font, const char* fontName2,
                               const nw::font::Font* font2, bool /* unused */)
{
    if (m_IsSetup) {
        return true;
    }
    m_ResAcc.SetData(s_Archive.m_pData, ".");
    // (the layout keeps the accessor as a reader; the button has a plain ArcResAcc)
    m_ButtonLayout.m_pArcReader = static_cast<ssys::ma::lyt::ArcResAccReader*>(&m_ResAcc);
    m_ResAcc.m_ArcResourceAccessor.RegistFont(fontName, font);
    if (fontName2 != NULL) {
        m_ResAcc.m_ArcResourceAccessor.RegistFont(fontName2, font2);
    }
    if (m_IsBack) {
        g_FormatBuffer.format("%s.bclyt", "cmn_btn_back");
        m_ButtonLayout.Build(g_FormatBuffer.getCString(), NULL, CMD_CACHE_SIZE);
    } else {
        m_ButtonLayout.Build("cmn_btn.bclyt", NULL, CMD_CACHE_SIZE);
    }
    m_ButtonLayout.m_Layer = LAYOUT_LAYER;
    m_pLayout = &m_ButtonLayout;
    m_pPane = m_ButtonLayout.FindBounding("B_btn_A");
    m_pGroup = m_ButtonLayout.FindGroup("G_btn");
    m_pAll = m_ButtonLayout.FindPane("N_all");
    m_pAll->SetTranslate(nn::math::VEC3(0.0f, 0.0f, 0.0f));
    m_Position.y += m_pAll->m_Size.height * CURSOR_OFFSET_RATE;
    m_InAnim.Initialize("cmn_btn_in.bclan", &m_ResAcc);
    m_OutAnim.Initialize("cmn_btn_out.bclan", &m_ResAcc);
    m_Touch.Initialize("cmn_btn_touch.bclan", &m_ResAcc);
    m_Select.Initialize("cmn_btn_select.bclan", &m_ResAcc);
    m_TouchOk.Initialize("cmn_btn_touch_ok.bclan", &m_ResAcc);
    m_SelectOk.Initialize("cmn_btn_select_ok.bclan", &m_ResAcc);
    m_NgAnim.Initialize("cmn_btn_ng.bclan", &m_ResAcc);
    m_ButtonLayout.BindAnimation(&m_InAnim);
    m_InAnim.SetFrame(0.0f);
    m_ButtonLayout.Animate();
    m_ButtonLayout.UnbindAnimation(&m_InAnim);
    SetSeOk(SE_SYS_DECIDE);
    SetOkSoundEnabled(true);
    // the Japanese layout has its own text boxes
    const char* text;
    const char* shadow;
    const char* group;
    if (GetRegion() == event::REGION_JAPAN) {
        text = "T_btn";
        shadow = "S_btn";
        group = "G_txt_01";
    } else {
        text = "T_btn_W";
        shadow = "S_btn_W";
        group = "G_txt_00";
    }
    m_pText = m_ButtonLayout.FindTextBox(text);
    m_pShadow = m_ButtonLayout.FindTextBox(shadow);
    m_ButtonLayout.SetGroupVisible(group, false);
    m_Garden.SetTextBoxes(m_pText, m_pShadow);
    m_IsSetup = true;
    m_ButtonLayout.BindAnimation(&m_OutAnim);
    m_OutAnim.SetLastFrame();
    m_ButtonLayout.Animate();
    m_ButtonLayout.UnbindAnimation(&m_OutAnim);
    return true;
}

// 0x002E0E38
void CommonButtonSingle::SetOut()
{
    m_ButtonLayout.BindAnimation(&m_OutAnim);
    m_OutAnim.SetLastFrame();
    m_ButtonLayout.Animate();
    m_ButtonLayout.UnbindAnimation(&m_OutAnim);
}

// 0x00569E84
void CommonButtonSingle::SetIn()
{
    if (!m_IsBack) {
        m_Garden.SetText(&m_Word);
        m_Garden.CommitText();
    }
    if (!m_IsDisabled) {
        ButtonActionNode::Select(false);
    }
    m_ButtonLayout.BindAnimation(&m_InAnim);
    m_InAnim.SetLastFrame();
    m_ButtonLayout.Animate();
    m_ButtonLayout.UnbindAnimation(&m_InAnim);
}

// 0x005E3894
void CommonButtonSingle::Finalize()
{
    nw::lyt::Pane* root = m_ButtonLayout.GetRootPane();
    if (root != NULL) {
        if (root->m_pParent != NULL) {
            root->m_pParent->RemoveChild(root);
        }
        m_IsActive = false;
    }
    m_ButtonLayout.UnbindAllAnimation();
    m_Garden.Clear();
}

// 0x002E0E6C slot 0x10
void CommonButtonSingle::FUN_002b72e0()
{
    (this->*m_pState)();
}

// 0x002E0E94
void CommonButtonSingle::AnimateLayout()
{
    if (!m_IsBack) {
        m_Garden.SetText(&m_Word);
        m_Garden.CommitText();
    }
    m_ButtonLayout.Animate();
}

// 0x002E0ED4
void CommonButtonSingle::StartIn()
{
    if (!m_IsBack) {
        m_Garden.SetText(&m_Word);
        m_Garden.CommitText();
    }
    if (!m_IsDisabled) {
        ButtonActionNode::Select(false);
    }
    m_ButtonLayout.BindAnimation(&m_InAnim);
    m_InAnim.SetFrame(0.0f);
    m_pState = &CommonButtonSingle::State_In;
}

// 0x002E0F5C
void CommonButtonSingle::State_Idle()
{
    // empty (in the original too)
}

// 0x002E0F60
CommonButtonSingle::CommonButtonSingle()
    : m_pText(NULL), m_pShadow(NULL), m_Garden(16), m_pParentPane(NULL), m_pAll(NULL), m_IsVisible(true),
      m_pState(&CommonButtonSingle::State_Idle), m_Unknown0x514(0), m_IsSetup(false), m_IsBack(false), m_IsDisabled(false)
{
    m_Index = 2;
}

// 0x002E1108 slot 0x00
// 0x002E1078 (deleting dtor)
CommonButtonSingle::~CommonButtonSingle()
{
    m_Touch.Finalize();
    m_Select.Finalize();
    m_TouchOk.Finalize();
    m_SelectOk.Finalize();
}
