#include "nw/anim/anim_AnimBlendOpBool.h"
#include "nw/anim/anim_AnimBlendOpFloat.h"
#include "nw/anim/anim_AnimBlendOpInt.h"
#include "nw/anim/anim_AnimBlendOpRgbaColor.h"
#include "nw/anim/anim_AnimBlendOpTexture.h"
#include "nw/anim/anim_AnimBlendOpVector2.h"
#include "nw/anim/anim_AnimBlendOpVector3.h"
#include "nw/gfx/gfx_AnimBlendOpTransform.h"
#include "nw/gfx/gfx_AnimGroup.h"
#include "nw/gfx/gfx_TransformAnimBlendOpAccScale.h"
#include "nw/gfx/gfx_TransformAnimBlendOpAccScaleQuat.h"
#include "nw/gfx/gfx_TransformAnimBlendOpQuat.h"
#include "nw/gfx/gfx_TransformAnimBlendOpStandard.h"

namespace nw {
namespace gfx {
// 0x004B2900 | nintendogs:callgraph [tier A]
u32 nw::gfx::AnimGroup::Initialize(bool useOriginalValue)
{
    u32 result = RESULT_SUCCESS;

    // the blend operation of each one of the resource (shared objects per kind)
    const s32 blendOpCount = m_ResAnimGroup.GetBlendOpCount();
    void* pBuffer = GetAllocator()->Alloc(blendOpCount * sizeof(const anim::AnimBlendOp*), 4);
    if (pBuffer == nullptr) {
        result |= RESULT_FAILURE_MASK;
    }
    if (result & RESULT_FAILURE_MASK) {
        return result;
    }
    {
        ut::MoveArray<const anim::AnimBlendOp*> blendOps(pBuffer, blendOpCount, GetAllocator());
        m_BlendOps = blendOps;
    }
    for (int i = 0; i < blendOpCount; ++i) {
        const u32 type = m_ResAnimGroup.GetBlendOpType(i);
        // 0x0097F3FC (guard 0x0097F3F8)
        static const anim::AnimBlendOpBool s_BlendOpBool;
        // 0x0097F404 (guard 0x0097F3F4)
        static const anim::AnimBlendOpInt s_BlendOpInt;
        // 0x0097F40C (guard 0x0097F3F0)
        static const anim::AnimBlendOpFloat s_BlendOpFloat;
        // 0x0097F414 (guard 0x0097F3EC)
        static const anim::AnimBlendOpVector2 s_BlendOpVector2;
        // 0x0097F41C (guard 0x0097F3E8)
        static const anim::AnimBlendOpVector3 s_BlendOpVector3;
        // 0x0097F424 (guard 0x0097F3E4)
        static const anim::AnimBlendOpRgbaColor s_BlendOpRgbaColor;
        // 0x0097F42C (guard 0x0097F3E0)
        static const anim::AnimBlendOpTexture s_BlendOpTexture;
        // 0x0097F434 (guard 0x0097F3DC)
        static const TransformAnimBlendOpStandard s_BlendOpTransformStandard;
        // 0x0097F43C (guard 0x0097F3D8)
        static const TransformAnimBlendOpAccScale s_BlendOpTransformAccScale;
        // 0x0097F444 (guard 0x0097F3D4)
        static const TransformAnimBlendOpQuat s_BlendOpTransformQuat;
        // 0x0097F44C (guard 0x0097F3D0)
        static const TransformAnimBlendOpAccScaleQuat s_BlendOpTransformAccScaleQuat;
        // 0x00AEFF14 (guard 0x0097F3CC)
        static const AnimBlendOpTransform s_BlendOpTransform;

        const anim::AnimBlendOp* pBlendOp;
        switch (type) {
        case BLEND_OP_BOOL:
            pBlendOp = &s_BlendOpBool;
            break;
        case BLEND_OP_INT:
            pBlendOp = &s_BlendOpInt;
            break;
        case BLEND_OP_FLOAT:
            pBlendOp = &s_BlendOpFloat;
            break;
        case BLEND_OP_VECTOR2:
            pBlendOp = &s_BlendOpVector2;
            break;
        case BLEND_OP_VECTOR3:
            pBlendOp = &s_BlendOpVector3;
            break;
        case BLEND_OP_RGBA_COLOR:
            pBlendOp = &s_BlendOpRgbaColor;
            break;
        case BLEND_OP_TEXTURE:
            pBlendOp = &s_BlendOpTexture;
            break;
        case BLEND_OP_TRANSFORM_STANDARD:
            pBlendOp = &s_BlendOpTransformStandard;
            break;
        case BLEND_OP_TRANSFORM_ACC_SCALE:
            pBlendOp = &s_BlendOpTransformAccScale;
            break;
        case BLEND_OP_TRANSFORM_QUAT:
            pBlendOp = &s_BlendOpTransformQuat;
            break;
        case BLEND_OP_TRANSFORM_ACC_SCALE_QUAT:
            pBlendOp = &s_BlendOpTransformAccScaleQuat;
            break;
        case BLEND_OP_TRANSFORM:
            pBlendOp = &s_BlendOpTransform;
            break;
        default:
            pBlendOp = nullptr;
            break;
        }
        m_BlendOps.PushBackFast(pBlendOp);
    }

    // the arrays per member
    const s32 memberCount = m_ResAnimGroup.GetMemberCount();
    pBuffer = GetAllocator()->Alloc(memberCount * sizeof(void*), 4);
    if (pBuffer == nullptr) {
        result |= RESULT_FAILURE_MASK;
    }
    if (result & RESULT_FAILURE_MASK) {
        return result;
    }
    {
        ut::MoveArray<void*> array(pBuffer, memberCount, GetAllocator());
        m_TargetObjects = array;
    }
    m_TargetObjects.Resize(memberCount);

    pBuffer = GetAllocator()->Alloc(memberCount * sizeof(void*), 4);
    if (pBuffer == nullptr) {
        result |= RESULT_FAILURE_MASK;
    }
    if (result & RESULT_FAILURE_MASK) {
        return result;
    }
    {
        ut::MoveArray<void*> array(pBuffer, memberCount, GetAllocator());
        m_Targets = array;
    }
    m_Targets.Resize(memberCount);

    pBuffer = GetAllocator()->Alloc(memberCount * sizeof(void*), 4);
    if (pBuffer == nullptr) {
        result |= RESULT_FAILURE_MASK;
    }
    if (result & RESULT_FAILURE_MASK) {
        return result;
    }
    {
        ut::MoveArray<void*> array(pBuffer, memberCount, GetAllocator());
        m_TargetValues = array;
    }
    m_TargetValues.Resize(memberCount);

    if (useOriginalValue) {
        pBuffer = GetAllocator()->Alloc(memberCount * sizeof(void*), 4);
        if (pBuffer == nullptr) {
            result |= RESULT_FAILURE_MASK;
        }
        if (result & RESULT_FAILURE_MASK) {
            return result;
        }
        {
            ut::MoveArray<void*> array(pBuffer, memberCount, GetAllocator());
            m_OriginalValues = array;
        }
        m_OriginalValues.Resize(memberCount);
    }
    return result;
}

// 0x004B36E0 | nintendogs:bytes [tier A]
void nw::gfx::AnimGroup::GetMemorySizeForInitialize(os::MemorySizeCalculator* pSize, anim::res::ResAnimGroup resAnimGroup, bool useOriginalValue)
{
    const s32 memberCount = resAnimGroup.GetMemberCount();
    pSize->Add(resAnimGroup.GetBlendOpCount() * sizeof(const anim::AnimBlendOp*));
    pSize->Add(memberCount * sizeof(void*));
    pSize->Add(memberCount * sizeof(void*));
    pSize->Add(memberCount * sizeof(void*));
    if (useOriginalValue) {
        pSize->Add(memberCount * sizeof(void*));
    }
}

// 0x004B3784 | nintendogs:callgraph [tier A]
nw::gfx::AnimGroup::AnimGroup(anim::res::ResAnimGroup resAnimGroup, SceneNode* pSceneNode, os::IAllocator* pAllocator)
    : GfxObject(pAllocator), m_ResAnimGroup(resAnimGroup), m_pSceneNode(pSceneNode), m_pIsTargetEnabled(nullptr), m_Unknown0x64(0)
{
}

// 0x004B38EC slot 0x00 | nintendogs:bytes
// 0x004B3808 (deleting dtor)
nw::gfx::AnimGroup::~AnimGroup()
{
    // empty (in the original too: only the arrays)
}

} // namespace gfx
} // namespace nw
