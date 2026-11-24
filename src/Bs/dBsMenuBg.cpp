#include "Bs/dBsMenuBg.h"

#include "Bs/dBsMenuMgr.h"
#include "Other/dPaneVisible.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvPlayer.h"
#include "netgame/netgame_Api.h"
#include "nw/lyt/lyt_Pane.h"
#include "ssys/ma/lyt/dLayoutMgr.h"
#include "stage/stage_Api.h"

// 0x00950718
BsMenuBg* BsMenuBg::s_pInstance;

// the states with their names (debug); the machine starts waiting
// 0x00849148
const BsMenuBg::StateInfo BsMenuBg::s_StateInfos[3] = {
    {"待機", &BsMenuBg::Enter_Wait, &BsMenuBg::State_Wait},
    {"背景替え", &BsMenuBg::Enter_ChangeBg, &BsMenuBg::State_ChangeBg},
    {"上画面着替え", &BsMenuBg::Enter_ChangeTop, &BsMenuBg::State_ChangeTop},
};

namespace {
const u32 CMD_CACHE_SIZE = 0x3000;
} // namespace

// 0x00695EC8
BsMenuBg::BsMenuBg()
    : m_IsShown(false), m_TopKind(0), m_IsTopShown(false), m_MenuKind(-1), m_TopFrames(0), m_KindFrame(0.0f),
      m_StageFrame(0.0f)
{
}

// 0x00695FF4 slot 0x00
// 0x00695F88 (deleting dtor)
// 0x00775C5C (thunk)
// 0x00775BF0 (deleting thunk)
BsMenuBg::~BsMenuBg()
{
    // (only the members are destroyed)
}

// 0x006959E4 slot 0x0C
oml::framework::Result BsMenuBg::Initialize()
{
    if (BsMenuMgr::Get() == NULL) {
        return oml::framework::RESULT_RETRY;
    }
    if (!m_Reader.ReadArc("Layout/Menu/mvmnt_undscrn.arc")) {
        return oml::framework::RESULT_RETRY;
    }
    m_Layout.SetArcResAccReader(&m_Reader);
    m_TopLayout.SetArcResAccReader(&m_Reader);
    m_Layout.Build("mvmnt_undscrn.bclyt", NULL, CMD_CACHE_SIZE);
    m_Layout.SetLayer(0);
    m_TopLayout.Build("mvmnt_undscrn.bclyt", NULL, CMD_CACHE_SIZE);
    m_TopLayout.SetLayer(0);
    m_LoopAnim.Initialize("mvmnt_undscrn_loop.bclan", &m_Reader);
    m_KindAnim.Initialize("mvmnt_undscrn_kind.bclan", &m_Reader);
    m_ChangeAnim.Initialize("mvmnt_undscrn_change.bclan", &m_Reader);
    m_TopLoopAnim.Initialize("mvmnt_undscrn_loop.bclan", &m_Reader);
    m_TopKindAnim.Initialize("mvmnt_undscrn_kind.bclan", &m_Reader);
    m_TopChangeAnim.Initialize("mvmnt_undscrn_change.bclan", &m_Reader);
    Setup();
    Start("BsMenuBg", s_StateInfos, 3);
    m_IsTopShown = false;
    m_TopKind = 0;
    s_pInstance = this;
    return oml::framework::RESULT_SUCCESS;
}

// 0x00695CB0 slot 0x18
oml::framework::Result BsMenuBg::Finalize()
{
    m_Layout.UnbindAllAnimation();
    m_TopLayout.UnbindAllAnimation();
    m_Reader.Clear();
    s_pInstance = NULL;
    return oml::framework::RESULT_SUCCESS;
}

// 0x00695BF4 slot 0x24
oml::framework::Result BsMenuBg::Calc()
{
    if (!m_IsShown) {
        return oml::framework::RESULT_SUCCESS;
    }
    UpdateStageFrame();
    Call();
    m_LoopAnim.IncreaseFrame();
    m_Layout.Animate();
    return oml::framework::RESULT_SUCCESS;
}

// 0x00695988 slot 0x30
oml::framework::Result BsMenuBg::Draw()
{
    if (m_IsShown) {
        ssys::ma::lyt::LayoutMgr::Get()->Register(&m_Layout, true);
        if (m_IsTopShown) {
            m_TopLoopAnim.IncreaseFrame();
            m_TopLayout.Animate();
            ssys::ma::lyt::LayoutMgr::Get()->Register(&m_TopLayout, false);
        }
    }
    return oml::framework::RESULT_SUCCESS;
}

// 0x00695CF4
void BsMenuBg::Setup()
{
    m_IsShown = false;
    m_TopKind = 0;
    m_StageFrame = 0.0f;
    m_Layout.BindAnimation(&m_LoopAnim, m_Layout.FindGroup("G_base_00"), false);
    m_Layout.BindAnimation(&m_LoopAnim, m_Layout.FindGroup("G_base_01"), false);
    m_TopLayout.BindAnimation(&m_TopLoopAnim);
    m_TopLayout.FindPane("P_base_00")->m_Alpha = 0;
    SetFlagVisible(m_TopLayout.FindPane("P_base_01"), false);
    m_pBase0 = m_Layout.FindPane("P_base_00");
    m_pBase1 = m_Layout.FindPane("P_base_01");
    SetFlagVisible(m_pBase1, false);
    m_MenuKind = -1;
    m_Layout.m_DrawInfo.m_LocationAdjustScale = nn::math::VEC2(1.25f, 1.0f);
    m_Layout.m_DrawInfo.m_Flag.isLocationAdjust = true;
    m_Layout.Animate();
    m_TopLayout.FindPane("N_mvmnt_Uscrn")->SetTranslate(nn::math::VEC3(0.0f, 0.0f, 20.0f));
    m_TopLayout.m_IsStereo = true;
    m_TopLayout.m_StereoDistance = 10.0f;
    m_TopLayout.Animate();
}

// 0x006952C0
void BsMenuBg::UpdateStageFrame()
{
    f32 frame;
    if (netgame::GetNetgameType() == netgame::TYPE_DREAM) {
        frame = 10.0f;
    } else if (stage::IsKind(stage::KIND_ISLAND, stage::GetCurrent()) ||
               stage::IsKind(stage::KIND_ISLAND_HUT, stage::GetCurrent()) ||
               stage::IsKind(stage::KIND_KOTOBUKI, stage::GetCurrent()) ||
               stage::IsKind(stage::KIND_TOUR, stage::GetCurrent()) ||
               stage::GetCurrent() == stage::NAME_SEA_DEPARTURE || stage::GetCurrent() == stage::NAME_SEA_RETURN) {
        frame = 9.0f;
    } else if (stage::GetCurrent() == stage::NAME_PHOTO_BOX && g_UnkAAFF18.pPlayer != NULL &&
               g_UnkAAFF18.pPlayer->emotes.HasAny()) {
        frame = 5.0f;
    } else {
        frame = 0.0f;
    }
    if (frame >= 0.0f && m_StageFrame != frame) {
        m_StageFrame = frame;
        Change(&BsMenuBg::State_ChangeBg);
    }
}

// 0x006953F0 | libgarden [tier A]
void BsMenuBg::ChangeBg(long kind)
{
    m_Layout.UnbindAnimation(&m_KindAnim, m_Layout.FindGroup("G_base_01"), false);
    m_Layout.UnbindAnimation(&m_ChangeAnim, m_Layout.FindGroup("G_base_01"), false);
    m_MenuKind = kind;
    Change(&BsMenuBg::State_ChangeBg);
}

// 0x0069562C
void BsMenuBg::Reset()
{
    m_IsTopShown = false;
    m_TopKind = 0;
    m_MenuKind = -1;
    ChangeBg(-1);
}

// 0x00695E90
void BsMenuBg::StartChange(s8 kind, u8 frames)
{
    if (m_TopKind == kind) {
        return;
    }
    m_TopKind = kind;
    m_TopFrames = frames;
    Change(&BsMenuBg::State_ChangeTop);
}

// 0x006952B8
void BsMenuBg::State_Wait()
{
    // empty (in the original too)
}

// 0x006952BC
void BsMenuBg::Enter_Wait()
{
    // empty (in the original too)
}

// 0x006956B4
void BsMenuBg::State_ChangeBg()
{
    if (!m_ChangeAnim.IsDone()) {
        m_ChangeAnim.IncreaseFrame();
        return;
    }
    // the new pattern becomes the one of P_base_00
    m_Layout.UnbindAnimation(&m_KindAnim, m_Layout.FindGroup("G_base_01"), false);
    m_Layout.UnbindAnimation(&m_ChangeAnim, m_Layout.FindGroup("G_base_01"), false);
    m_Layout.BindAnimation(&m_KindAnim, m_Layout.FindGroup("G_base_00"), false);
    m_KindAnim.SetFrame(m_KindFrame);
    m_Layout.Animate();
    m_Layout.UnbindAnimation(&m_KindAnim, m_Layout.FindGroup("G_base_00"), false);
    SetFlagVisible(m_pBase1, false);
    Change(&BsMenuBg::State_Wait);
}

// 0x006957C8
void BsMenuBg::Enter_ChangeBg()
{
    SetFlagVisible(m_pBase1, true);
    m_Layout.BindAnimation(&m_ChangeAnim, m_Layout.FindGroup("G_base_01"), false);
    m_Layout.BindAnimation(&m_KindAnim, m_Layout.FindGroup("G_base_01"), false);
    switch (m_MenuKind) {
    case 0:
        m_KindFrame = 1.0f;
        if (stage::IsKind(stage::KIND_ISLAND, stage::GetCurrent()) ||
            stage::IsKind(stage::KIND_ISLAND_HUT, stage::GetCurrent()) ||
            stage::IsKind(stage::KIND_KOTOBUKI, stage::GetCurrent()) ||
            stage::IsKind(stage::KIND_TOUR, stage::GetCurrent())) {
            m_KindFrame = 7.0f;
        }
        break;
    case 13:
        m_KindFrame = 7.0f;
        break;
    case 1:
        m_KindFrame = 2.0f;
        break;
    case 2:
        m_KindFrame = 3.0f;
        break;
    case 3:
        m_KindFrame = 4.0f;
        break;
    case 4:
        m_KindFrame = 8.0f;
        break;
    case 6:
        m_KindFrame = 6.0f;
        break;
    case 7:
        m_KindFrame = 5.0f;
        break;
    case 12:
        break;
    default:
        m_KindFrame = m_StageFrame;
        break;
    }
    m_KindAnim.SetFrame(m_KindFrame);
    m_ChangeAnim.SetFrame(0.0f);
}

// 0x00695468
void BsMenuBg::State_ChangeTop()
{
    if (!m_TopChangeAnim.IsDone()) {
        m_TopChangeAnim.IncreaseFrame();
        return;
    }
    m_TopLayout.UnbindAnimation(&m_TopChangeAnim);
    m_IsTopShown = m_TopKind;
    Change(&BsMenuBg::State_Wait);
}

// 0x006954B8
void BsMenuBg::Enter_ChangeTop()
{
    if (m_TopFrames != 6) {
        m_KindFrame = m_TopFrames;
    } else {
        switch (m_MenuKind) {
        case 0:
            m_KindFrame = 1.0f;
            break;
        case 1:
        case 12:
            m_KindFrame = 2.0f;
            break;
        case 2:
            m_KindFrame = 3.0f;
            break;
        case 3:
            m_KindFrame = 4.0f;
            break;
        case 6:
            m_KindFrame = 6.0f;
            break;
        case 7:
            m_KindFrame = 5.0f;
            break;
        default:
            m_KindFrame = 0.0f;
            break;
        }
    }
    m_IsTopShown = m_IsTopShown || m_TopKind != 0;
    // the new pattern of the upper screen
    m_TopLayout.BindAnimation(&m_TopKindAnim);
    m_TopKindAnim.SetFrame(m_KindFrame);
    m_TopLayout.Animate();
    m_TopLayout.UnbindAnimation(&m_TopKindAnim);
    // the change of the upper screen comes in / goes out
    if (m_TopKind != 0) {
        m_TopChangeAnim.SetPlayParameters(m_TopChangeAnim.GetFrameCount(), 1.0f, -1.0f, 1);
    } else {
        m_TopChangeAnim.SetPlayParameters(m_TopChangeAnim.GetFrameCount(), 1.0f, m_TopChangeAnim.GetFrameCount(), 3);
    }
    m_TopLayout.BindAnimation(&m_TopChangeAnim);
}
