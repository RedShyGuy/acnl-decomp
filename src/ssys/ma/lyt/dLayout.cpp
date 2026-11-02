#include "ssys/ma/lyt/dLayout.h"
#include "nn/gr/CTR/detail/gr_Command.h"
#include "nn/gx/gx_Api.h"
#include "nn/math/math_ARMv6.h"
#include "nn/math/math_Api.h"
#include "nw/lyt/lyt_Bounding.h"
#include "nw/lyt/lyt_Group.h"
#include "nw/lyt/lyt_Pane.h"
#include "nw/lyt/lyt_Picture.h"
#include "nw/lyt/lyt_TextBox.h"
#include "ssys/ma/lyt/dAnimation.h"
#include "ssys/ma/lyt/dArcResAccReader.h"

namespace ssys {
namespace ma {
namespace lyt {
namespace {
const u32 REG_SCISSORTEST_MODE = 0x065;
// the parameter of NN_GX_CMDLIST_BINDING (nngxGetCmdlistParameteri)
const u32 CMDLIST_BINDING = 0x207;
// the vertical field of view of the perspective projection: half of it as an angle index (256 = a full turn), 45 degrees
const f32 HALF_FOVY_IDX = 16.0f;
// the depth range of the projections
const f32 ORTHO_NEAR = -500.0f;
const f32 ORTHO_FAR = 500.0f;
const f32 PERSPECTIVE_NEAR = 0.05f;
const f32 PERSPECTIVE_FAR = 500.0f;
// the parallax of the 3D camera is relative to the width of the upper screen
const f32 PARALLAX_WIDTH = 400.0f;

// (inline) a perspective projection like the nw::gfx camera makes it (name is ours)
inline void MakePerspective(nn::math::MTX44* pOut, f32 halfFovyIdx, f32 aspect, f32 near, f32 far)
{
    f32 sinValue, cosValue;
    nn::math::SinCosFIdx(&sinValue, &cosValue, halfFovyIdx);
    const f32 cotangent = cosValue / sinValue;
    const f32 depth = 1.0f / (far - near);
    pOut->m[0][1] = 0.0f;
    pOut->m[0][2] = 0.0f;
    pOut->m[0][3] = 0.0f;
    pOut->m[1][0] = 0.0f;
    pOut->m[1][2] = 0.0f;
    pOut->m[1][3] = 0.0f;
    pOut->m[2][0] = 0.0f;
    pOut->m[2][1] = 0.0f;
    pOut->m[3][0] = 0.0f;
    pOut->m[3][1] = 0.0f;
    pOut->m[3][2] = -1.0f;
    pOut->m[3][3] = 0.0f;
    pOut->m[0][0] = cotangent / aspect;
    pOut->m[1][1] = cotangent;
    pOut->m[2][2] = far * depth;
    pOut->m[2][3] = far * near * depth;
}

// (inline) a coordinate of the scissor rectangle inside [0, size) (name is ours)
inline u16 ClampScissor(f32 value, u16 size)
{
    if (value >= size) {
        value = size - 1;
    } else if (value < 0.0f) {
        value = 0.0f;
    }
    return static_cast<u16>(value);
}
} // namespace

// 0x00569118 | libgarden [tier A]
void ssys::ma::lyt::Layout::BindAnimation(Animation* anim, const nw::lyt::Group* group, bool recursive)
{
    anim->m_IsBound = 1;
    nw::lyt::Group::PaneLinkList& paneList = const_cast<nw::lyt::Group*>(group)->GetPaneList();
    for (nw::lyt::Group::PaneLinkList::iterator it = paneList.begin(); it != paneList.end(); ++it) {
        nw::lyt::Group::PaneLinkList::GetPointerFromNode(it.GetNode())->m_pTarget->BindAnimation(anim->m_pAnimTransform,
                                                                                                 recursive, false);
    }
}

// 0x00569178 (name is ours)
void ssys::ma::lyt::Layout::UpdateParallax()
{
    if (m_AnimateDone == true) {
        // Animate makes the matrices again anyway
        m_IsProjUpdated = false;
    } else {
        m_PrevParallax = m_Parallax;
        f32 parallax;
        if (!m_IsStereo) {
            parallax = 0.0f;
        } else {
            nn::math::MTX34 view(m_DrawInfo.m_ViewMtx);
            LayoutMgr::Get()->m_StereoCamera.SetBaseCamera(&view);
            f32 distance;
            if (m_IsPerspective == true) {
                distance = m_StereoDistance + m_CameraDistance;
            } else {
                distance = m_StereoDistance;
            }
            nn::math::MTX44 projL;
            nn::math::MTX34 viewL;
            nn::math::MTX44 projR;
            nn::math::MTX34 viewR;
            LayoutMgr::Get()->m_StereoCamera.CalculateMatrices(&projL, &viewL, &projR, &viewR, distance, 1.0f,
                                                               nn::math::PIVOT_DIRECTION_1, true);
            parallax = LayoutMgr::Get()->m_StereoCamera.GetParallaxScale() * PARALLAX_WIDTH;
        }
        m_Parallax = parallax;
        if (parallax == m_PrevParallax) {
            return;
        }
        m_Layout.nw::lyt::Layout::CalculateMtx(m_DrawInfo);
        m_IsProjUpdated = false;
        m_AnimateDone = true;
    }
    m_IsViewUpdated = false;
}

// 0x00569274 | libgarden [tier A]
nw::lyt::Pane* ssys::ma::lyt::Layout::FindPane(const char* name)
{
    return m_Layout.GetRootPane()->FindPaneByName(name, true);
}

// 0x00569288 slot 0x18 (name after libgarden)
void ssys::ma::lyt::Layout::UpdateProj()
{
    nw::ut::Rect rect = m_Layout.GetLayoutRect();
    nn::math::MTX44 proj;
    if (m_IsPerspective == true) {
        MakePerspective(&proj, HALF_FOVY_IDX, (rect.right - rect.left) / (rect.top - rect.bottom), PERSPECTIVE_NEAR,
                        PERSPECTIVE_FAR);
    } else {
        nn::math::ARMv6::MTX44OrthoC_FAST(&proj, rect.left, rect.right, rect.bottom, rect.top, ORTHO_NEAR, ORTHO_FAR);
    }
    nn::math::ARMv6::MTX44PivotC_FAST(&proj, nn::math::PIVOT_DIRECTION_1);
    m_DrawInfo.SetProjectionMtx(proj);
}

// 0x0056946C | libgarden [tier A]
void ssys::ma::lyt::Layout::UnbindAnimation(Animation* anim, const nw::lyt::Group* group, bool recursive)
{
    anim->m_IsBound = 0;
    nw::lyt::Group::PaneLinkList& paneList = const_cast<nw::lyt::Group*>(group)->GetPaneList();
    for (nw::lyt::Group::PaneLinkList::iterator it = paneList.begin(); it != paneList.end(); ++it) {
        nw::lyt::Group::PaneLinkList::GetPointerFromNode(it.GetNode())->m_pTarget->UnbindAnimation(anim->m_pAnimTransform,
                                                                                                   recursive);
    }
}

// 0x005694C0 (name is ours)
void ssys::ma::lyt::Layout::SetGroupVisible(const char* groupName, bool visible)
{
    nw::lyt::Group* group = m_Layout.GetGroupContainer()->FindGroupByName(groupName);
    nw::lyt::Group::PaneLinkList& paneList = group->GetPaneList();
    for (nw::lyt::Group::PaneLinkList::iterator it = paneList.begin(); it != paneList.end(); ++it) {
        nw::lyt::Pane* pane = nw::lyt::Group::PaneLinkList::GetPointerFromNode(it.GetNode())->m_pTarget;
        pane->m_Flag = (pane->m_Flag & ~nw::lyt::Pane::FLAG_VISIBLE) | visible;
    }
}

// 0x0056950C | libgarden [tier A]
nw::lyt::Picture* ssys::ma::lyt::Layout::FindPicture(const char* name)
{
    return nw::lyt::DynamicCast<nw::lyt::Picture>(FindPane(name));
}

// 0x00569568 | libgarden [tier A]
nw::lyt::TextBox* ssys::ma::lyt::Layout::FindTextBox(const char* name)
{
    return nw::lyt::DynamicCast<nw::lyt::TextBox>(FindPane(name));
}

// 0x005695C4 (name is ours)
nw::lyt::Bounding* ssys::ma::lyt::Layout::FindBounding(const char* name)
{
    return nw::lyt::DynamicCast<nw::lyt::Bounding>(FindPane(name));
}

// 0x00569620 (name is ours)
nw::lyt::AnimTransform* ssys::ma::lyt::Layout::CreateAnimTransform(const void* animResBuf,
                                                                   nw::lyt::ResourceAccessor* resAccessor)
{
    nw::lyt::AnimTransform* animTrans = m_Layout.nw::lyt::Layout::CreateAnimTransform(animResBuf, resAccessor);
    if (animTrans != NULL) {
        m_Layout.m_AnimTransList.Erase(animTrans);
    }
    return animTrans;
}

// 0x005696F8 slot 0x0C | libgarden
void ssys::ma::lyt::Layout::Animate()
{
    m_Unknown0x122 = 0;
    m_Layout.nw::lyt::Layout::Animate(m_AnimateOption);
    if (!m_IsViewUpdated) {
        if (m_IsPerspective == true) {
            nw::ut::Rect rect = m_Layout.GetLayoutRect();
            f32 height = rect.top - rect.bottom;
            f32 sinValue, cosValue;
            nn::math::SinCosFIdx(&sinValue, &cosValue, HALF_FOVY_IDX);
            m_CameraDistance = (height * 0.5f) / (sinValue / cosValue);
        }
        f32 z;
        if (m_IsStereo == true) {
            z = m_StereoDistance;
        } else {
            z = m_CameraPos.z;
        }
        nn::math::VEC3 cameraPos(m_CameraPos.x, m_CameraPos.y, z);
        nn::math::VEC3 cameraUp(0.0f, 1.0f, 0.0f);
        nn::math::VEC3 target(0.0f, 0.0f, 0.0f);
        nn::math::MTX34 view;
        nn::math::ARMv6::MTX34LookAtC_FAST(&view, &cameraPos, &cameraUp, &target);
        m_DrawInfo.m_ViewMtx = view;
        if (!m_IsStereo) {
            nn::math::ARMv6::MTX34InverseAsm(&m_InvViewMtx, &view);
        }
        m_IsViewUpdated = true;
    }
    m_Layout.nw::lyt::Layout::CalculateMtx(m_DrawInfo);
    m_PrevParallax = m_Parallax;
    f32 parallax;
    if (!m_IsStereo) {
        parallax = 0.0f;
    } else {
        nn::math::MTX34 view(m_DrawInfo.m_ViewMtx);
        LayoutMgr::Get()->m_StereoCamera.SetBaseCamera(&view);
        f32 distance;
        if (m_IsPerspective == true) {
            distance = m_StereoDistance + m_CameraDistance;
        } else {
            distance = m_StereoDistance;
        }
        nn::math::MTX44 projL;
        nn::math::MTX34 viewL;
        nn::math::MTX44 projR;
        nn::math::MTX34 viewR;
        LayoutMgr::Get()->m_StereoCamera.CalculateMatrices(&projL, &viewL, &projR, &viewR, distance, 1.0f,
                                                           nn::math::PIVOT_DIRECTION_1, true);
        parallax = LayoutMgr::Get()->m_StereoCamera.GetParallaxScale() * PARALLAX_WIDTH;
    }
    m_Parallax = parallax;
    m_AnimateDone = true;
}

// 0x005699A4 slot 0x08 (name after libgarden)
void ssys::ma::lyt::Layout::DrawCached()
{
    Draw(m_DrawInfo);
}

// 0x005699AC (name is ours)
void ssys::ma::lyt::Layout::Draw(const nw::lyt::DrawInfo& drawInfo)
{
    const u16 width = m_Layout.m_LayoutSize.width;
    const u16 height = m_Layout.m_LayoutSize.height;
    // the PICA x axis runs along the height of the layout
    if (m_IsScissorEnable == true) {
        // (image 0x008AF280)
        u32 command[4] = {3, nn::gr::CTR::detail::CommandHeader(REG_SCISSORTEST_MODE, 0xF, 2, true), 0, 0};
        command[2] = ClampScissor(m_ScissorY, height) | (ClampScissor(m_ScissorX, width) << 16);
        command[3] = ClampScissor(m_ScissorY + m_ScissorHeight - 1.0f, height) |
                     (ClampScissor(m_ScissorX + m_ScissorWidth - 1.0f, width) << 16);
        nngxAdd3DCommand(command, sizeof(command), true);
    } else {
        // (image 0x008AF290)
        const u32 command[4] = {0, nn::gr::CTR::detail::CommandHeader(REG_SCISSORTEST_MODE, 0xF, 2, true), 0,
                                static_cast<u32>((height - 1) | ((width - 1) << 16))};
        nngxAdd3DCommand(command, sizeof(command), true);
    }
    if (m_SavedBufferSize <= 0) {
        return;
    }
    f32 parallax;
    if ((m_DrawMode == DRAW_MODE_TOP_LEFT || m_DrawMode == DRAW_MODE_TOP_RIGHT) && m_IsStereo == true) {
        if (m_DrawMode == DRAW_MODE_TOP_LEFT) {
            parallax = -m_Parallax;
        } else {
            parallax = m_Parallax;
        }
    } else {
        parallax = 0.0f;
    }
    nn::ulcd::CTR::StereoCamera& camera = LayoutMgr::Get()->m_StereoCamera;
    LayoutMgr::Get()->m_Drawer.SetParameter(parallax, camera.GetBasePlaneDistance(), 0.0f);
    LayoutMgr::Get()->m_Drawer.DrawBegin(m_DrawInfo, 0);
    nngxUseSavedCmdlist(m_Cmdlist, m_SavedBufferOffset, m_SavedBufferSize, m_SavedRequestOffset, m_SavedRequestSize,
                        m_SavedStateMask, m_IsCopyCmd);
    LayoutMgr::Get()->m_Drawer.DrawEnd(0);
}

// 0x00569C6C slot 0x10 | libgarden
bool ssys::ma::lyt::Layout::Build(const char* name, const ArcResAccReader* arcReader, u32 cmdCacheSize)
{
    if (m_Layout.GetRootPane() != NULL) {
        return true;
    }
    if (arcReader == NULL) {
        arcReader = m_pArcReader;
        if (arcReader == NULL) {
            return false;
        }
    }
    void* lytRes = arcReader->m_pResourceAccessor->GetResource('blyt', name, NULL);
    if (lytRes == NULL) {
        arcReader->m_pResourceAccessor->GetResource(0, name, NULL);
        return false;
    }
    m_SavedStateMask = 0;
    m_IsCopyCmd = true;
    s32 prevCmdlist;
    nngxGetCmdlistParameteri(CMDLIST_BINDING, &prevCmdlist);
    nngxGenCmdlists(1, &m_Cmdlist);
    nngxBindCmdlist(m_Cmdlist);
    m_CmdlistStorageSize = cmdCacheSize;
    nngxCmdlistStorage(cmdCacheSize, 1);
    nngxBindCmdlist(prevCmdlist);
    bool result = m_Layout.nw::lyt::Layout::Build(lytRes, arcReader->m_pResourceAccessor);
    if (result) {
        Animate();
    }
    return result;
}

// 0x00569D68 (name is ours)
void ssys::ma::lyt::Layout::BindAnimation(Animation* anim, nw::lyt::Pane* pane, bool recursive)
{
    anim->m_IsBound = 1;
    anim->m_pAnimTransform->Bind(pane, recursive, false);
}

// 0x00569D9C slot 0x14 (name after libgarden)
void ssys::ma::lyt::Layout::DrawSave()
{
    if (!m_IsProjUpdated) {
        UpdateProj();
        m_IsProjUpdated = true;
    }
    if (!m_AnimateDone) {
        return;
    }
    s32 prevCmdlist;
    nngxGetCmdlistParameteri(CMDLIST_BINDING, &prevCmdlist);
    nngxBindCmdlist(m_Cmdlist);
    nngxClearCmdlist();
    nngxStartCmdlistSave();
    LayoutMgr::Get()->m_Drawer.DrawBegin(1);
    LayoutMgr::Get()->m_Drawer.Draw(&m_Layout, m_DrawInfo);
    LayoutMgr::Get()->m_Drawer.DrawEnd(1);
    nngxStopCmdlistSave(&m_SavedBufferOffset, &m_SavedBufferSize, &m_SavedRequestOffset, &m_SavedRequestSize);
    nngxBindCmdlist(prevCmdlist);
    m_AnimateDone = false;
}

// 0x00569F08 (name is ours)
void ssys::ma::lyt::Layout::UnbindAnimation(Animation* anim)
{
    anim->m_IsBound = 0;
    m_Layout.nw::lyt::Layout::UnbindAnimation(anim->m_pAnimTransform);
}

// 0x00569F68 slot 0x00 | libgarden
// 0x00569F1C (deleting dtor)
ssys::ma::lyt::Layout::~Layout()
{
    if (m_Cmdlist != 0) {
        nngxDeleteCmdlists(1, &m_Cmdlist);
    }
}

} // namespace lyt
} // namespace ma
} // namespace ssys
