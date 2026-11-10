#pragma once

#include "decomp.h"
#include "nw/lyt/lyt_Pane.h"

// (inline, name is ours) the visible flag of a pane is set without comparing it first. The game has this besides
// nw::lyt::Pane::SetVisible, which only writes the flag when it changes: ARMCC keeps such a compare, and the two forms
// appear side by side (TextArea::Calc: the text boxes with the compare, the base lines without).
inline void SetFlagVisible(nw::lyt::Pane* pane, bool visible)
{
    pane->m_Flag = (pane->m_Flag & ~nw::lyt::Pane::FLAG_VISIBLE) | visible;
}
