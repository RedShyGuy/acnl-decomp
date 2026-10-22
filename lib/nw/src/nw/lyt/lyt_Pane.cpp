#include "nw/lyt/lyt_Pane.h"

#include "nn/math/math_ARMv6.h"
#include "nn/math/math_Api.h"
#include "nw/lyt/internal/internal_Api.h"
#include "nw/lyt/lyt_AnimTransform.h"
#include "nw/lyt/lyt_DrawInfo.h"
#include "nw/lyt/lyt_GraphicsResource.h"
#include "nw/lyt/lyt_Layout.h"
#include "nw/lyt/lyt_Material.h"
#include "nw/lyt/lyt_ResourceFormat.h"

namespace nw {
namespace lyt {
namespace local {
namespace {
// 0x0014EE54 | mk7dlp:bytes
// the parent matrix times the scale and translation (the rotation is none)
void MultMatrixSRTxST(nn::math::MTX34* pMtx, const nn::math::MTX34* pMtxParent, const nn::math::VEC2* pScale, const nn::math::VEC3* pTransform)
{
    f32 m00 = pMtxParent->m[0][0];
    f32 m01 = pMtxParent->m[0][1];
    const f32 m02 = pMtxParent->m[0][2];
    f32 m03 = pMtxParent->m[0][3];
    f32 m10 = pMtxParent->m[1][0];
    f32 m11 = pMtxParent->m[1][1];
    const f32 m12 = pMtxParent->m[1][2];
    f32 m13 = pMtxParent->m[1][3];
    f32 m20 = pMtxParent->m[2][0];
    f32 m21 = pMtxParent->m[2][1];
    const f32 m22 = pMtxParent->m[2][2];
    f32 m23 = pMtxParent->m[2][3];

    m03 += m00 * pTransform->x;
    m13 += m10 * pTransform->x;
    m23 += m20 * pTransform->x;
    m03 += m01 * pTransform->y;
    m13 += m11 * pTransform->y;
    m23 += m21 * pTransform->y;
    m03 += m02 * pTransform->z;
    m13 += m12 * pTransform->z;
    m23 += m22 * pTransform->z;

    m00 *= pScale->x;
    m01 *= pScale->y;
    m10 *= pScale->x;
    m11 *= pScale->y;
    m20 *= pScale->x;
    m21 *= pScale->y;

    pMtx->m[0][0] = m00;
    pMtx->m[0][1] = m01;
    pMtx->m[0][2] = m02;
    pMtx->m[0][3] = m03;
    pMtx->m[1][0] = m10;
    pMtx->m[1][1] = m11;
    pMtx->m[1][2] = m12;
    pMtx->m[1][3] = m13;
    pMtx->m[2][0] = m20;
    pMtx->m[2][1] = m21;
    pMtx->m[2][2] = m22;
    pMtx->m[2][3] = m23;
}
} // namespace
} // namespace local

namespace {
// degrees to the angle index of SinCosFIdx (256 per turn)
inline f32 DegToFIdx(f32 deg)
{
    return deg * (256.0f / 360.0f);
}
} // namespace

// 0x00976164 (name is ours)
const PaneTypeInfo nw::lyt::Pane::s_TypeInfo(nullptr);

// sets s_TypeInfo
// 0x00786370 (static initializer)

namespace internal {
// 0x00976161 (name is ours)
bool g_IsIgnorePartsAlphaZero = true;
} // namespace internal

// 0x004B7E78 | nintendogs:callseq-callee [tier A]
nw::lyt::Pane::Pane(const res::Pane* pBlock)
{
    Init();

    m_BasePosition = pBlock->basePosition;
    internal::CopyString(m_Name, sizeof(m_Name), pBlock->name);
    internal::CopyString(m_UserData, sizeof(m_UserData), pBlock->userData);
    m_Translate = pBlock->translate;
    m_Rotate = pBlock->rotate;
    m_Scale = pBlock->scale;
    m_Size = pBlock->size;
    m_Alpha = pBlock->alpha;
    m_GlbAlpha = m_Alpha;
    m_Flag = pBlock->flag;
    m_BasePosition = (m_BasePosition & ~BASEPOSITION_IGNORE_PARTS_ALPHA_ZERO) | (internal::g_IsIgnorePartsAlphaZero << 4);
}

// 0x004B7C78 | nintendogs:bytes-fuzzy [tier A]
void nw::lyt::Pane::Init()
{
    m_pParent = nullptr;
    m_Flag = 0;
    m_pExtUserDataList = nullptr;
    nn::math::ARMv6::MTX34CopyAsm(&m_Mtx, &nn::math::MTX34::Identity());
    nn::math::ARMv6::MTX34CopyAsm(&m_GlbMtx, &nn::math::MTX34::Identity());
}

// 0x004B7FC4 slot 0x00 | nintendogs:bytes
// 0x004B7FB0 (deleting dtor)
nw::lyt::Pane::~Pane()
{
    for (PaneList::iterator it = m_ChildList.begin(); it != m_ChildList.end();) {
        PaneList::iterator currIt = it;
        ++it;
        m_ChildList.erase(currIt);
        Pane* pChild = FromLink(currIt.GetNode());
        if (!pChild->IsUserAllocated()) {
            Layout::DeleteObj(pChild);
        }
    }

    UnbindAnimationSelf(nullptr);
}

// 0x0073CE08 slot 0x08 (name is ours)
const PaneTypeInfo* nw::lyt::Pane::GetRuntimeTypeInfo() const
{
    return &s_TypeInfo;
}

// 0x0073CD54 slot 0x0C
ut::Color8 nw::lyt::Pane::GetVtxColor(u32 /*idx*/) const
{
    return ut::Color8();
}

// 0x004B7470 slot 0x10
void nw::lyt::Pane::SetVtxColor(u32 /*idx*/, ut::Color8 /*value*/)
{
    // empty (in the original too: a plain pane has no vertex colors)
}

// 0x0073CD68 slot 0x14 | mk7dlp:bytes
u8 nw::lyt::Pane::GetColorElement(u32 idx) const
{
    if (idx == ANIMTARGET_PANE_ALPHA) {
        return m_Alpha;
    }
    return GetVtxColorElement(idx);
}

// 0x004B7904 slot 0x18 | nintendogs:bytes
void nw::lyt::Pane::SetColorElement(u32 idx, u8 value)
{
    if (idx == ANIMTARGET_PANE_ALPHA) {
        m_Alpha = value;
    } else {
        SetVtxColorElement(idx, value);
    }
}

// 0x0073CE14 slot 0x1C
u8 nw::lyt::Pane::GetVtxColorElement(u32 /*idx*/) const
{
    return 0xFF;
}

// 0x004B7BE8 slot 0x20
void nw::lyt::Pane::SetVtxColorElement(u32 /*idx*/, u8 /*value*/)
{
    // empty (in the original too: a plain pane has no vertex colors)
}

// 0x0073CD60 slot 0x24
u8 nw::lyt::Pane::GetMaterialNum() const
{
    return 0;
}

// 0x0073CC58 slot 0x28
Material* nw::lyt::Pane::GetMaterial(u32 /*idx*/)
{
    return nullptr;
}

// 0x004B7890 slot 0x2C | nintendogs:bytes
Pane* nw::lyt::Pane::FindPaneByName(const char* findName, bool bRecursive)
{
    if (internal::EqualsResName(m_Name, findName)) {
        return this;
    }
    if (bRecursive) {
        for (PaneList::iterator it = m_ChildList.begin(); it != m_ChildList.end(); ++it) {
            Pane* pPane = FromLink(it.GetNode())->FindPaneByName(findName, bRecursive);
            if (pPane != nullptr) {
                return pPane;
            }
        }
    }
    return nullptr;
}

// 0x004B7994 slot 0x30 | nintendogs:bytes
Material* nw::lyt::Pane::FindMaterialByName(const char* findName, bool bRecursive)
{
    const u32 materialNum = GetMaterialNum();
    for (u32 i = 0; i < materialNum; ++i) {
        Material* pMaterial = GetMaterial(i);
        if (pMaterial != nullptr && internal::EqualsMaterialName(pMaterial->GetName(), findName)) {
            return pMaterial;
        }
    }
    if (bRecursive) {
        for (PaneList::iterator it = m_ChildList.begin(); it != m_ChildList.end(); ++it) {
            Material* pMaterial = FromLink(it.GetNode())->FindMaterialByName(findName, bRecursive);
            if (pMaterial != nullptr) {
                return pMaterial;
            }
        }
    }
    return nullptr;
}

// 0x004B7D84 slot 0x34 | nintendogs:bytes
void nw::lyt::Pane::Animate(u32 option)
{
    AnimateSelf(option);
    if (IsVisible() || !(option & ANIMATE_OPTION_SKIP_INVISIBLE)) {
        for (PaneList::iterator it = m_ChildList.begin(); it != m_ChildList.end(); ++it) {
            FromLink(it.GetNode())->Animate(option);
        }
    }
}

// 0x004B7334 slot 0x38 | nintendogs:bytes
void nw::lyt::Pane::AnimateSelf(u32 option)
{
    for (AnimationList::iterator it = m_AnimList.begin(); it != m_AnimList.end(); ++it) {
        AnimationLink* pLink = AnimationList::GetPointerFromNode(it.GetNode());
        if (!pLink->m_IsDisable) {
            pLink->m_pAnimTransform->Animate(pLink->m_Index, this);
        }
    }

    if (IsVisible() || !(option & ANIMATE_OPTION_SKIP_INVISIBLE)) {
        const u32 materialNum = GetMaterialNum();
        for (u32 i = 0; i < materialNum; ++i) {
            Material* pMaterial = GetMaterial(i);
            if (pMaterial != nullptr) {
                pMaterial->Animate();
            }
        }
    }
}

// 0x004B7870 slot 0x3C | nintendogs:bytes
void nw::lyt::Pane::BindAnimation(AnimTransform* pAnimTrans, bool bRecursive, bool bDisable)
{
    pAnimTrans->Bind(this, bRecursive, bDisable);
}

// 0x004B7920 slot 0x40 | nintendogs:bytes
void nw::lyt::Pane::UnbindAnimation(AnimTransform* pAnimTrans, bool bRecursive)
{
    UnbindAnimationSelf(pAnimTrans);
    if (bRecursive) {
        for (PaneList::iterator it = m_ChildList.begin(); it != m_ChildList.end(); ++it) {
            FromLink(it.GetNode())->UnbindAnimation(pAnimTrans, bRecursive);
        }
    }
}

// 0x004B7BEC slot 0x44 (name is ours)
void nw::lyt::Pane::UnbindAllAnimation(bool bRecursive)
{
    UnbindAnimation(nullptr, bRecursive);
}

// 0x004BEF64 slot 0x48 | nintendogs:bytes
void nw::lyt::Pane::UnbindAnimationSelf(AnimTransform* pAnimTrans)
{
    const u32 materialNum = GetMaterialNum();
    for (u32 i = 0; i < materialNum; ++i) {
        Material* pMaterial = GetMaterial(i);
        if (pMaterial != nullptr) {
            pMaterial->UnbindAnimation(pAnimTrans);
        }
    }
    internal::UnbindAnimationLink(&m_AnimList, pAnimTrans);
}

// 0x004BEEC0 slot 0x4C (name is ours)
AnimationLink* nw::lyt::Pane::FindAnimationLinkSelf(AnimTransform* pAnimTrans)
{
    return internal::FindAnimationLink(&m_AnimList, pAnimTrans);
}

// 0x004BEF00 slot 0x50 (name is ours)
AnimationLink* nw::lyt::Pane::FindAnimationLinkSelf(const AnimResource& animRes)
{
    return internal::FindAnimationLink(&m_AnimList, animRes);
}

// 0x004B7A58 slot 0x54 (name is ours)
void nw::lyt::Pane::SetAnimationEnable(AnimTransform* pAnimTrans, bool bEnable, bool bRecursive)
{
    AnimationLink* pAnimLink = FindAnimationLinkSelf(pAnimTrans);
    if (pAnimLink != nullptr) {
        pAnimLink->m_IsDisable = !bEnable;
    }

    const u32 materialNum = GetMaterialNum();
    for (u32 i = 0; i < materialNum; ++i) {
        GetMaterial(i)->SetAnimationEnable(pAnimTrans, bEnable);
    }

    if (bRecursive) {
        for (PaneList::iterator it = m_ChildList.begin(); it != m_ChildList.end(); ++it) {
            FromLink(it.GetNode())->SetAnimationEnable(pAnimTrans, bEnable, bRecursive);
        }
    }
}

// 0x004B7B20 slot 0x58 (name is ours)
void nw::lyt::Pane::SetAnimationEnable(const AnimResource& animRes, bool bEnable, bool bRecursive)
{
    AnimationLink* pAnimLink = FindAnimationLinkSelf(animRes);
    if (pAnimLink != nullptr) {
        pAnimLink->m_IsDisable = !bEnable;
    }

    const u32 materialNum = GetMaterialNum();
    for (u32 i = 0; i < materialNum; ++i) {
        GetMaterial(i)->SetAnimationEnable(animRes, bEnable);
    }

    if (bRecursive) {
        for (PaneList::iterator it = m_ChildList.begin(); it != m_ChildList.end(); ++it) {
            FromLink(it.GetNode())->SetAnimationEnable(animRes, bEnable, bRecursive);
        }
    }
}

// 0x004B7474 slot 0x5C | libgarden
void nw::lyt::Pane::CalculateMtx(const DrawInfo& drawInfo)
{
    if (!IsVisible() && !drawInfo.m_Flag.isInvisiblePaneCalculateMtx) {
        return;
    }

    if (!(m_Flag & FLAG_USER_GLOBAL_MTX)) {
        const nn::math::MTX34* pParentMtx = (m_pParent != nullptr) ? &m_pParent->m_GlbMtx : &drawInfo.m_ViewMtx;

        if (!(m_Flag & FLAG_USER_MTX)) {
            nn::math::VEC2 scale = m_Scale;
            u32 mtxCondition = MTX_CONDITION_SCALE;
            if (drawInfo.m_Flag.isLocationAdjust && (m_Flag & FLAG_LOCATION_ADJUST)) {
                mtxCondition = MTX_CONDITION_LOCATION_ADJUST;
                scale.x = drawInfo.m_LocationAdjustScale.x * scale.x;
                scale.y = drawInfo.m_LocationAdjustScale.y * scale.y;
            }

            if (m_Rotate.x != 0.0f || m_Rotate.y != 0.0f) {
                if (((m_Flag & FLAG_MTX_CONDITION_MASK) >> 4) != mtxCondition) {
                    m_Flag = (m_Flag & ~FLAG_MTX_CONDITION_MASK) | (mtxCondition << 4);

                    f32 sinx, cosx, siny, cosy, sinz, cosz;
                    nn::math::SinCosFIdx(&sinx, &cosx, DegToFIdx(m_Rotate.x));
                    nn::math::SinCosFIdx(&siny, &cosy, DegToFIdx(m_Rotate.y));
                    nn::math::SinCosFIdx(&sinz, &cosz, DegToFIdx(m_Rotate.z));

                    const f32 cosxcosz = cosx * cosz;
                    const f32 sinxsiny = sinx * siny;
                    const f32 cosxsinz = cosx * sinz;
                    m_Mtx.m[0][0] = scale.x * (cosy * cosz);
                    m_Mtx.m[0][1] = scale.y * (sinxsiny * cosz - cosxsinz);
                    m_Mtx.m[0][2] = cosxcosz * siny + sinx * sinz;
                    m_Mtx.m[0][3] = m_Translate.x;
                    m_Mtx.m[1][0] = scale.x * (cosy * sinz);
                    m_Mtx.m[1][1] = scale.y * (cosxcosz + sinxsiny * sinz);
                    m_Mtx.m[1][2] = cosxsinz * siny - sinx * cosz;
                    m_Mtx.m[1][3] = m_Translate.y;
                    m_Mtx.m[2][0] = -(scale.x * siny);
                    m_Mtx.m[2][1] = scale.y * (sinx * cosy);
                    m_Mtx.m[2][2] = cosx * cosy;
                    m_Mtx.m[2][3] = m_Translate.z;
                }
            } else if (m_Rotate.z != 0.0f) {
                if (((m_Flag & FLAG_MTX_CONDITION_MASK) >> 4) != mtxCondition) {
                    m_Flag = (m_Flag & ~FLAG_MTX_CONDITION_MASK) | (mtxCondition << 4);

                    f32 sinz, cosz;
                    nn::math::SinCosFIdx(&sinz, &cosz, DegToFIdx(m_Rotate.z));
                    m_Mtx.m[0][0] = scale.x * cosz;
                    m_Mtx.m[0][1] = -(scale.y * sinz);
                    m_Mtx.m[0][2] = 0.0f;
                    m_Mtx.m[0][3] = m_Translate.x;
                    m_Mtx.m[1][0] = scale.x * sinz;
                    m_Mtx.m[1][1] = scale.y * cosz;
                    m_Mtx.m[1][2] = 0.0f;
                    m_Mtx.m[1][3] = m_Translate.y;
                    m_Mtx.m[2][0] = 0.0f;
                    m_Mtx.m[2][1] = 0.0f;
                    m_Mtx.m[2][2] = 1.0f;
                    m_Mtx.m[2][3] = m_Translate.z;
                }
            } else {
                local::MultMatrixSRTxST(&m_GlbMtx, pParentMtx, &scale, &m_Translate);
                goto calcAlpha;
            }
        }
        nn::math::ARMv6::MTX34MultAsm(&m_GlbMtx, pParentMtx, &m_Mtx);
    }

calcAlpha:
    if (drawInfo.m_Flag.isInfluencedAlpha && m_pParent != nullptr) {
        m_GlbAlpha = static_cast<u8>(m_Alpha * drawInfo.m_GlobalAlpha);
    } else {
        m_GlbAlpha = m_Alpha;
    }

    if ((m_Flag & FLAG_INFLUENCED_ALPHA) && m_Alpha != 0xFF) {
        DrawInfo& mtDrawInfo = const_cast<DrawInfo&>(drawInfo);
        const f32 crGlobalAlpha = drawInfo.m_GlobalAlpha;
        const u32 bInfluencedAlpha = drawInfo.m_Flag.isInfluencedAlpha;
        mtDrawInfo.m_GlobalAlpha = m_Alpha * crGlobalAlpha * (1.0f / 255.0f);
        mtDrawInfo.m_Flag.isInfluencedAlpha = 1;

        for (PaneList::iterator it = m_ChildList.begin(); it != m_ChildList.end(); ++it) {
            FromLink(it.GetNode())->CalculateMtx(drawInfo);
        }

        mtDrawInfo.m_GlobalAlpha = crGlobalAlpha;
        mtDrawInfo.m_Flag.isInfluencedAlpha = bInfluencedAlpha;
    } else {
        for (PaneList::iterator it = m_ChildList.begin(); it != m_ChildList.end(); ++it) {
            FromLink(it.GetNode())->CalculateMtx(drawInfo);
        }
    }
}

// 0x004B7C00 slot 0x60 (name is ours)
void nw::lyt::Pane::Draw(DrawInfo& drawInfo)
{
    if (!IsVisible()) {
        return;
    }
    if (!(m_BasePosition & BASEPOSITION_IGNORE_PARTS_ALPHA_ZERO) || m_GlbAlpha != 0) {
        DrawSelf(drawInfo);
    }
    for (PaneList::iterator it = m_ChildList.begin(); it != m_ChildList.end(); ++it) {
        FromLink(it.GetNode())->Draw(drawInfo);
    }
}

// 0x004B7E74 slot 0x64 (name is ours)
void nw::lyt::Pane::DrawSelf(DrawInfo& /*drawInfo*/)
{
    // empty (in the original too: a plain pane draws nothing)
}

// 0x0073CE1C slot 0x68
u32* nw::lyt::Pane::MakeUniformDataSelf(u32* pCommand, DrawInfo& /*drawInfo*/, Drawer& /*drawer*/)
{
    return pCommand;
}

// 0x004B7DF0 slot 0x6C (name is ours)
void nw::lyt::Pane::LoadMtx(DrawInfo& drawInfo)
{
    GraphicsResource& graphicsResource = *drawInfo.m_pGraphicsResource;
    nn::math::MTX34 glbMtx;
    nn::math::ARMv6::MTX34CopyAsm(&glbMtx, &m_GlbMtx);
    graphicsResource.m_ModelViewMtx = glbMtx;
    graphicsResource.m_IsModelViewMtxLoaded = false;
}

// 0x004B73FC | libgarden [tier A]
void nw::lyt::Pane::AppendChild(Pane* pChild)
{
    m_ChildList.PushBack(pChild);
    pChild->m_pParent = this;
}

// 0x004B7828 (name is ours)
void nw::lyt::Pane::PrependChild(Pane* pChild)
{
    m_ChildList.insert(m_ChildList.begin(), PaneList::GetNodeFromPointer(pChild));
    pChild->m_pParent = this;
}

// 0x004B7420 | mk7dlp:bytes [tier B]
void nw::lyt::Pane::InsertChild(Pane* pNext, Pane* pChild)
{
    m_ChildList.insert(PaneList::iterator(PaneList::GetNodeFromPointer(pNext)), PaneList::GetNodeFromPointer(pChild));
    pChild->m_pParent = this;
}

// 0x004B7450 | libgarden [tier A]
void nw::lyt::Pane::RemoveChild(Pane* pChild)
{
    m_ChildList.Erase(pChild);
    pChild->m_pParent = nullptr;
}

// 0x004B7984 | nintendogs:callgraph [tier A]
void nw::lyt::Pane::AddAnimationLink(AnimationLink* pAnimationLink)
{
    m_AnimList.PushBack(pAnimationLink);
}

// 0x004B7850 (name is ours)
void nw::lyt::Pane::SetTranslate(const nn::math::VEC3& value)
{
    m_Translate = value;
    m_Flag &= ~FLAG_MTX_CONDITION_MASK;
}

// 0x004B7E34 (name is ours)
void nw::lyt::Pane::SetName(const char* name)
{
    internal::CopyString(m_Name, sizeof(m_Name), name);
}

// 0x0073CD84 (name is ours)
u32* nw::lyt::Pane::MakeUniformData(u32* pCommand, DrawInfo& drawInfo, Drawer& drawer)
{
    if (!(m_BasePosition & BASEPOSITION_IGNORE_PARTS_ALPHA_ZERO) || m_GlbAlpha != 0) {
        pCommand = MakeUniformDataSelf(pCommand, drawInfo, drawer);
    }
    for (PaneList::iterator it = m_ChildList.begin(); it != m_ChildList.end(); ++it) {
        Pane* pChild = FromLink(it.GetNode());
        if (pChild->IsVisible()) {
            pCommand = pChild->MakeUniformData(pCommand, drawInfo, drawer);
        }
    }
    return pCommand;
}

// 0x0073CC60 | mk7dlp:bytes [tier B]
Material* nw::lyt::Pane::GetMaterial()
{
    if (GetMaterialNum() == 0) {
        return nullptr;
    }
    return GetMaterial(0);
}

// 0x0073CC9C | nintendogs:callgraph [tier A]
ut::Rect nw::lyt::Pane::GetPaneRect() const
{
    ut::Rect ret(0.0f, 0.0f, 0.0f, 0.0f);
    const nn::math::VEC2 basePt = GetVtxPos();
    ret.left = basePt.x;
    ret.top = basePt.y;
    ret.right = ret.left + m_Size.width;
    ret.bottom = ret.top - m_Size.height;
    return ret;
}

// 0x0073CE24 | nintendogs:callgraph [tier A]
nn::math::VEC2 nw::lyt::Pane::GetVtxPos() const
{
    nn::math::VEC2 basePt(0.0f, 0.0f);

    switch (GetBasePositionH()) {
    case 1:
        basePt.x = m_Size.width * -0.5f;
        break;
    case 2:
        basePt.x = -m_Size.width;
        break;
    }

    switch (GetBasePositionV()) {
    case 1:
        basePt.y = m_Size.height * 0.5f;
        break;
    case 2:
        basePt.y = m_Size.height;
        break;
    }

    return basePt;
}

} // namespace lyt
} // namespace nw
