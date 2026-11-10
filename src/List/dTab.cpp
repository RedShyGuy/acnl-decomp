#include "List/dTab.h"

// the states with their names (Japanese, debug): do nothing, entering, exiting, waiting
// 0x0090C830
template <>
const Tab<2>::Info Tab<2>::s_StateInfos[4] = {
    {"何もしない", &Tab<2>::Enter_None, &Tab<2>::State_None},
    {"入場処理", &Tab<2>::Enter_In, &Tab<2>::State_In},
    {"退場処理", &Tab<2>::Enter_Out, &Tab<2>::State_Out},
    {"待機処理", &Tab<2>::Enter_Wait, &Tab<2>::State_Wait},
};
// 0x0090C880
template <>
const Tab<4>::Info Tab<4>::s_StateInfos[4] = {
    {"何もしない", &Tab<4>::Enter_None, &Tab<4>::State_None},
    {"入場処理", &Tab<4>::Enter_In, &Tab<4>::State_In},
    {"退場処理", &Tab<4>::Enter_Out, &Tab<4>::State_Out},
    {"待機処理", &Tab<4>::Enter_Wait, &Tab<4>::State_Wait},
};
// 0x0090C8D0
template <>
const Tab<7>::Info Tab<7>::s_StateInfos[4] = {
    {"何もしない", &Tab<7>::Enter_None, &Tab<7>::State_None},
    {"入場処理", &Tab<7>::Enter_In, &Tab<7>::State_In},
    {"退場処理", &Tab<7>::Enter_Out, &Tab<7>::State_Out},
    {"待機処理", &Tab<7>::Enter_Wait, &Tab<7>::State_Wait},
};

template <int N>
Tab<N>::~Tab()
{
    // (only the members are destroyed)
}

template <int N>
void Tab<N>::State_None()
{
    // empty (in the original too)
}

template <int N>
void Tab<N>::Enter_None()
{
    // empty (in the original too)
}

template <int N>
void Tab<N>::State_In()
{
    if (m_InAnims[0].IsDone()) {
        for (s32 i = 0; i < N; i++) {
            m_TabLayouts[i].UnbindAnimation(&m_InAnims[i]);
        }
        m_IsAnimating = false;
        m_Control.SetFlags(ButtonActionControl::DISABLED, false);
        this->Change(&Tab::State_Wait);
        return;
    }
    for (s32 i = 0; i < N; i++) {
        m_InAnims[i].IncreaseFrame();
        m_TabLayouts[i].ssys::ma::lyt::Layout::Animate();
    }
}

template <int N>
void Tab<N>::Enter_In()
{
    for (s32 i = 0; i < N; i++) {
        m_TabLayouts[i].BindAnimation(&m_InAnims[i]);
        m_InAnims[i].ssys::ma::lyt::FrameCtrl::SetFrame(0.0f);
        m_TabLayouts[i].ssys::ma::lyt::Layout::Animate();
    }
    m_IsAnimating = true;
}

template <int N>
void Tab<N>::State_Out()
{
    if (m_OutAnims[0].IsDone()) {
        for (s32 i = 0; i < N; i++) {
            m_TabLayouts[i].UnbindAnimation(&m_OutAnims[i]);
        }
        m_IsAnimating = false;
        this->Change(&Tab::State_None);
        return;
    }
    for (s32 i = 0; i < N; i++) {
        m_OutAnims[i].IncreaseFrame();
        m_TabLayouts[i].ssys::ma::lyt::Layout::Animate();
    }
}

template <int N>
void Tab<N>::Enter_Out()
{
    for (s32 i = 0; i < N; i++) {
        m_TabLayouts[i].BindAnimation(&m_OutAnims[i]);
        m_OutAnims[i].ssys::ma::lyt::FrameCtrl::SetFrame(0.0f);
        m_TabLayouts[i].ssys::ma::lyt::Layout::Animate();
    }
    m_IsAnimating = true;
    m_Control.SetFlags(ButtonActionControl::DISABLED, true);
}

template <int N>
void Tab<N>::State_Wait()
{
    // a tab was chosen: it becomes the current one
    if (m_Control.IsSelectOkDone()) {
        ButtonActionCstmNode& node = m_Nodes[m_Current];
        node.m_IsPressed = false;
        node.Animate(node.m_TouchOk, node.m_IsPressed);
        node.Animate(node.m_SelectOk, node.m_IsPressed);
        m_Nodes[m_Current].Select(false);
        m_Nodes[m_Current].m_IsActive = true;
        m_Current = m_Control.GetPressedIndex();
        m_Nodes[m_Current].m_IsActive = false;
        m_Control.Reset();
    }
    for (s32 i = 0; i < N; i++) {
        m_TabLayouts[i].ssys::ma::lyt::Layout::Animate();
    }
}

template <int N>
void Tab<N>::Enter_Wait()
{
    // empty (in the original too)
}

// 0x004DF3A0 slot 0x00
// 0x004DF30C (deleting dtor)
template Tab<2>::~Tab();
// 0x0082CD68 slot 0x08
template const char* Tab<2>::GetName(Func func) const;
// 0x007EB130
template void Tab<2>::State_None();
// 0x007EB21C
template void Tab<2>::Enter_None();
// 0x007EAEE0
template void Tab<2>::State_In();
// 0x007EAF94
template void Tab<2>::Enter_In();
// 0x007EB008
template void Tab<2>::State_Out();
// 0x007EB0B0
template void Tab<2>::Enter_Out();
// 0x007EB134
template void Tab<2>::State_Wait();
// 0x007EB220
template void Tab<2>::Enter_Wait();
// 0x0081BAD0 (name is ours)
template void state::Mode<Tab<2> >::Change(Func func);

// 0x004DF4B8 slot 0x00
// 0x004DF430 (deleting dtor)
template Tab<4>::~Tab();
// 0x0082CDE0 slot 0x08
template const char* Tab<4>::GetName(Func func) const;
// 0x007EB224
template <>
DECOMP_NOINLINE void Tab<4>::Initialize(ssys::ma::lyt::ArcResAccReader* arcReader)
{
    InitializeImpl(arcReader);
}
// 0x007EB7E4
template void Tab<4>::State_None();
// 0x007EB8C4
template void Tab<4>::Enter_None();
// 0x007EB5BC
template void Tab<4>::State_In();
// 0x007EB664
template void Tab<4>::Enter_In();
// 0x007EB6D0
template void Tab<4>::State_Out();
// 0x007EB76C
template void Tab<4>::Enter_Out();
// 0x007EB7E8
template void Tab<4>::State_Wait();
// 0x007EB8C8
template void Tab<4>::Enter_Wait();
// 0x0081BBD8 (name is ours)
template void state::Mode<Tab<4> >::Change(Func func);

// 0x004DF5D8 slot 0x00
// 0x004DF53C (deleting dtor)
template Tab<7>::~Tab();
// 0x0082CE58 slot 0x08
template const char* Tab<7>::GetName(Func func) const;
// 0x007EBB4C
template void Tab<7>::State_None();
// 0x007EBC38
template void Tab<7>::Enter_None();
// 0x007EB8CC
template void Tab<7>::State_In();
// 0x007EB990
template void Tab<7>::Enter_In();
// 0x007EBA0C
template void Tab<7>::State_Out();
// 0x007EBABC
template void Tab<7>::Enter_Out();
// 0x007EBB50
template void Tab<7>::State_Wait();
// 0x007EBC3C
template void Tab<7>::Enter_Wait();
// 0x0081BCE0 (name is ours)
template void state::Mode<Tab<7> >::Change(Func func);
