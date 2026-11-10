#pragma once

#include "decomp.h"
#include "lyt/dAnmObject.h"
#include "lyt/dObjectEx.h"
#include "nw/lyt/lyt_AnimResource.h"
#include "nw/lyt/lyt_Group.h"
#include "nw/lyt/lyt_TextBox.h"
#include "nw/ut/ut_Api.h"
#include "ssys/ma/lyt/dArcResAccReader.h"

// Inline helpers of the key sets (the original has them inlined in each Initialize; names are ours).
namespace gui {
namespace detail {
// the most pane animations of an animation of a key set layout
const s32 PANE_ANIM_MAX = 128;

// (inline) the pane animations of an animation of the layout ("<layout>_<anim>.bclan") are made for all panes
inline u32 CreatePaneAnimations(lyt::ObjectEx* layout, const char* layoutName, const char* animName, lyt::AnmObject** anims)
{
    char name[64];
    nw::ut::FormatString(name, sizeof(name), "%s_%s.bclan", layoutName, animName);
    nw::lyt::AnimResource animRes;
    animRes.Set(layout->m_pArcReader->GetResource(0, name));
    layout->m_Layout.GetLayout().BindAnimationAuto(animRes, layout->m_pArcReader->GetResourceAccessor());
    return layout->CreatePaneAnmObjects(layout->GetRootPane(), animRes, anims, PANE_ANIM_MAX);
}

// (inline) the pane animation of the set whose pane it is (NULL: none)
inline lyt::AnmObject* FindPaneAnimation(lyt::AnmObject* const* anims, u32 animNum, const nw::lyt::Pane* pane)
{
    for (u32 i = 0; i < animNum; i++) {
        lyt::AnmObject* anim = anims[i];
        if (anim != NULL && anim->m_pPane == pane) {
            return anim;
        }
    }
    return NULL;
}

// (inline) the sub animations of the panes of the group (at most five): for each pane and each of the N sets, the
// pane animation of the set into dsts[set][pane]
template <s32 N>
inline void FindSubAnimations(lyt::ObjectEx* layout, const char* groupName, lyt::AnmObject** const (&dsts)[N], const s32* sets,
                              lyt::AnmObject* const (*paneAnims)[PANE_ANIM_MAX], const u32* paneAnimNums)
{
    nw::lyt::Group::PaneLinkList& paneList = layout->m_Layout.GetLayout().GetGroupContainer()->FindGroupByName(groupName)->GetPaneList();
    s32 paneIdx = 0;
    for (nw::lyt::Group::PaneLinkList::iterator it = paneList.begin(); it != paneList.end() && paneIdx < 5; ++it, paneIdx++) {
        const nw::lyt::Pane* pane = nw::lyt::Group::PaneLinkList::GetPointerFromNode(it.GetNode())->m_pTarget;
        for (s32 i = 0; i < N; i++) {
            const s32 set = sets[i];
            dsts[i][paneIdx] = FindPaneAnimation(paneAnims[set], paneAnimNums[set], pane);
        }
    }
}

// (inline) the name of the text box that is the last pane of the group (NULL: none)
inline const char* FindLabelTextBox(lyt::ObjectEx* layout, const char* groupName)
{
    nw::lyt::Group::PaneLinkList& paneList = layout->m_Layout.GetLayout().GetGroupContainer()->FindGroupByName(groupName)->GetPaneList();
    nw::lyt::TextBox* textBox =
        nw::lyt::DynamicCast<nw::lyt::TextBox>(nw::lyt::Group::PaneLinkList::GetPointerFromNode(paneList.end().GetNode()->m_pPrev)->m_pTarget);
    return textBox != NULL ? textBox->m_Name : NULL;
}
} // namespace detail
} // namespace gui
