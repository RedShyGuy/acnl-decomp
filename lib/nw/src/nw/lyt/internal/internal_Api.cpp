#include "nw/lyt/internal/internal_Api.h"

#include <cstring>

#include "nn/gx/gx_Api.h"
#include "nw/lyt/internal/lyt_GlConstants.h"
#include "nw/lyt/lyt_AnimTransform.h"
#include "nw/lyt/lyt_AnimTransformBasic.h"
#include "nw/lyt/lyt_DrawInfo.h"
#include "nw/lyt/lyt_GraphicsResource.h"
#include "nw/lyt/lyt_Layout.h"
#include "nw/lyt/lyt_Material.h"
#include "nw/lyt/lyt_Pane.h"

namespace nw {
namespace lyt {
namespace internal {
// 0x004BEE20 | nintendogs:bytes [tier A]
bool EqualsResName(const char* name1, const char* name2)
{
    return std::strncmp(name1, name2, res::RESOURCE_NAME_LENGTH) == 0;
}

// 0x004BEF48 | nintendogs:bytes [tier A]
bool EqualsMaterialName(const char* name1, const char* name2)
{
    return std::strncmp(name1, name2, res::MATERIAL_NAME_LENGTH) == 0;
}

// 0x004BEFD4 | nintendogs:bytes [tier A]
void UnbindAnimationLink(AnimationList* pAnimList, AnimTransform* pAnimTrans)
{
    AnimationList::iterator it = pAnimList->begin();
    while (it != pAnimList->end()) {
        AnimationLink* pLink = AnimationList::GetPointerFromNode(it.GetNode());
        ++it;
        if (pAnimTrans == nullptr || pLink->m_pAnimTransform == pAnimTrans) {
            pAnimList->erase(ut::internal::LinkListImpl::iterator(&pLink->m_Link));
            pLink->Reset();
        }
    }
}

// 0x004BEEC8 (name is ours)
AnimationLink* FindAnimationLink(AnimationList* pAnimList, AnimTransform* pAnimTrans)
{
    for (AnimationList::iterator it = pAnimList->begin(); it != pAnimList->end(); ++it) {
        AnimationLink* pLink = AnimationList::GetPointerFromNode(it.GetNode());
        if (pLink->m_pAnimTransform == pAnimTrans) {
            return pLink;
        }
    }
    return nullptr;
}

// 0x004BEF08 (name is ours)
AnimationLink* FindAnimationLink(AnimationList* pAnimList, const AnimResource& animRes)
{
    for (AnimationList::iterator it = pAnimList->begin(); it != pAnimList->end(); ++it) {
        AnimationLink* pLink = AnimationList::GetPointerFromNode(it.GetNode());
        if (animRes.GetResourceBlock() == pLink->m_pAnimTransform->GetAnimResource()) {
            return pLink;
        }
    }
    return nullptr;
}

// 0x004BF394 (name is ours)
void DrawQuad(const DrawInfo& drawInfo, const nn::math::VEC2& basePt, const Size& size, u8 texCoordNum, const TexCoordQuad* texCoords, const ut::Color8* vtxColors)
{
    GraphicsResource& graphicsResource = *drawInfo.m_pGraphicsResource;
    if (graphicsResource.m_IsVertexAttribDirty) {
        graphicsResource.m_IsVertexAttribDirty = false;
        graphicsResource.SetupVertexAttrib();
    }

    if (vtxColors != nullptr) {
        const GLint loc = graphicsResource.GetUniformLocation(GraphicsResource::UNIFORM_VERTEXCOLOR);
        for (int i = 0; i < 4; ++i) {
            glUniform4f(loc + i, vtxColors[i].r, vtxColors[i].g, vtxColors[i].b, vtxColors[i].a);
        }
    }

    if (texCoordNum != 0 && texCoords != nullptr) {
        for (int i = 0; i < GraphicsResource::VERTEX_TEXCOORD_MAX; ++i) {
            const int src = graphicsResource.m_VertexTexCoordSrc[i];
            if (src >= 0 && texCoords != nullptr && src < texCoordNum) {
                const GLint loc = graphicsResource.GetUniformLocation(GraphicsResource::UNIFORM_VERTEXTEXCOORD0 + i);
                const nn::math::VEC2* coords = texCoords[src];
                for (int j = 0; j < 4; ++j) {
                    glUniform4f(loc + j, coords[j].x, coords[j].y, 0.0f, 1.0f);
                }
            }
        }
    }

    glUniform4f(graphicsResource.GetUniformLocation(GraphicsResource::UNIFORM_TRANSFORM), size.width, size.height, basePt.x, basePt.y);

    if (!graphicsResource.m_IsModelViewMtxLoaded) {
        graphicsResource.m_IsModelViewMtxLoaded = true;
        glUniform4fv(graphicsResource.GetUniformLocation(GraphicsResource::UNIFORM_MODELVIEW), 3, graphicsResource.m_ModelViewMtx.m[0]);
    }

    glDrawElements(gl::TRIANGLES, 6, gl::UNSIGNED_SHORT, 0);
}

// 0x004BEE3C (name is ours)
void DrawQuad(const DrawInfo& drawInfo, const nn::math::VEC2& basePt, const Size& size)
{
    GraphicsResource& graphicsResource = *drawInfo.m_pGraphicsResource;
    glUniform4f(graphicsResource.GetUniformLocation(GraphicsResource::UNIFORM_TRANSFORM), size.width, size.height, basePt.x, basePt.y);
    glDrawElements(gl::TRIANGLES, 6, gl::UNSIGNED_SHORT, 0);
}

// 0x004BEDCC (name is ours)
nw::lyt::internal::AnimPaneTree::AnimPaneTree(Pane* pTargetPane, const AnimResource& animRes)
    : m_AnimRes(), m_AnimPaneIdx(0), m_LinkNum(0), m_AnimMatIdxs(), m_AnimMatCnt(0)
{
    Set(pTargetPane, animRes);
}

// (inline)
u16 nw::lyt::internal::AnimPaneTree::FindAnimContent(const res::AnimationBlock* pAnimBlock, const char* animContName, u8 animContType)
{
    const u32* animContOffsets = reinterpret_cast<const u32*>(reinterpret_cast<uptr>(pAnimBlock) + pAnimBlock->animContOffsetsOffset);
    for (u16 i = 0; i < pAnimBlock->animContNum; ++i) {
        const res::AnimationContent* pAnimCont = reinterpret_cast<const res::AnimationContent*>(reinterpret_cast<uptr>(pAnimBlock) + animContOffsets[i]);
        if (pAnimCont->type == animContType && EqualsMaterialName(pAnimCont->name, animContName)) {
            return i;
        }
    }
    return 0xFFFF;
}

// 0x004BEC0C (name is ours)
void nw::lyt::internal::AnimPaneTree::Set(Pane* pTargetPane, const AnimResource& animRes)
{
    const res::AnimationBlock* pAnimBlock = animRes.GetResourceBlock();
    u16 linkNum = 0;

    const u16 animPaneIdx = FindAnimContent(pAnimBlock, pTargetPane->m_Name, res::ANIMCONTENTTYPE_PANE);
    if (animPaneIdx != 0xFFFF) {
        ++linkNum;
    }

    const u8 animMatCnt = pTargetPane->GetMaterialNum();
    u16 animMatIdxs[MATERIAL_NUM_MAX];
    for (u8 i = 0; i < animMatCnt; ++i) {
        animMatIdxs[i] = FindAnimContent(pAnimBlock, pTargetPane->GetMaterial(i)->GetName(), res::ANIMCONTENTTYPE_MATERIAL);
        if (animMatIdxs[i] != 0xFFFF) {
            ++linkNum;
        }
    }

    if (linkNum > 0) {
        m_AnimRes = animRes;
        m_AnimPaneIdx = animPaneIdx;
        m_AnimMatCnt = animMatCnt;
        for (u8 i = 0; i < animMatCnt; ++i) {
            m_AnimMatIdxs[i] = animMatIdxs[i];
        }
        m_LinkNum = linkNum;
    }
}

// 0x0073E95C (name is ours)
AnimTransform* nw::lyt::internal::AnimPaneTree::Bind(Layout* pLayout, Pane* pTargetPane, ResourceAccessor* pResAccessor) const
{
    AnimTransformBasic* pAnimTrans = static_cast<AnimTransformBasic*>(pLayout->CreateAnimTransform());
    pAnimTrans->SetResource(m_AnimRes.GetResourceBlock(), pResAccessor, m_LinkNum);

    AnimationLink* pCrAnimLink = nullptr;
    if (m_AnimPaneIdx != 0xFFFF) {
        pCrAnimLink = pAnimTrans->FindUnbindLink(pCrAnimLink);
        if (pCrAnimLink != nullptr) {
            pCrAnimLink->Set(pAnimTrans, m_AnimPaneIdx, true);
            pTargetPane->AddAnimationLink(pCrAnimLink);
            ++pCrAnimLink;
        }
    }

    u8 animMatMax = pTargetPane->GetMaterialNum();
    if (animMatMax >= m_AnimMatCnt) {
        animMatMax = m_AnimMatCnt;
    }
    for (u8 i = 0; i < animMatMax; ++i) {
        if (m_AnimMatIdxs[i] != 0xFFFF) {
            Material* pMaterial = pTargetPane->GetMaterial(i);
            pCrAnimLink = pAnimTrans->FindUnbindLink(pCrAnimLink);
            if (pCrAnimLink != nullptr) {
                pCrAnimLink->Set(pAnimTrans, m_AnimMatIdxs[i], true);
                pMaterial->AddAnimationLink(pCrAnimLink);
                ++pCrAnimLink;
            }
        }
    }
    return pAnimTrans;
}

} // namespace internal
} // namespace lyt
} // namespace nw
