#include "nw/anim/res/anim_ResAnimGroupMember.h"

#include "nw/anim/res/anim_ResFragmentLightMember.h"
#include "nw/anim/res/anim_ResHemiSphereLightMember.h"
#include "nw/anim/res/anim_ResMaterialColorMember.h"
#include "nw/anim/res/anim_ResProjectionUpdaterMember.h"
#include "nw/anim/res/anim_ResTextureCoordinatorMember.h"
#include "nw/anim/res/anim_ResVertexLightMember.h"
#include "nw/gfx/gfx_CalculatedTransform.h"
#include "nw/gfx/gfx_Model.h"
#include "nw/gfx/res/gfx_ResCamera.h"
#include "nw/gfx/res/gfx_ResFog.h"
#include "nw/gfx/res/gfx_ResLight.h"
#include "nw/gfx/res/gfx_ResMaterial.h"
#include "nw/gfx/res/gfx_ResMesh.h"
#include "nw/gfx/res/gfx_ResModel.h"
#include "nw/gfx/res/gfx_ResTextureMapper.h"
#include "nw/gfx/res/gfx_ResTextureSampler.h"

namespace nw {
namespace anim {
namespace res {
// 0x007442FC | nintendogs:callseq [tier A]
void nw::anim::res::ResAnimGroupMember::SetValueForType(void* pTarget, const void* pValue) const
{
    switch (GetMemberType()) {
    case ResAnimGroupMemberData::MEMBER_TYPE_TRANSFORM: {
        gfx::CalculatedTransform* pTransform = static_cast<gfx::CalculatedTransform*>(pTarget);
        pTransform->SetTransform(*static_cast<const nn::math::Transform3*>(pValue));
        pTransform->UpdateScaleFlags();
        pTransform->UpdateRotateFlags();
        pTransform->UpdateTranslateFlags();
        pTransform->UpdateCompositeFlags();
        break;
    }
    case ResAnimGroupMemberData::MEMBER_TYPE_MATERIAL_COLOR:
        ResMaterialColorMember(m_pData).SetValue(pTarget, pValue);
        break;
    case ResAnimGroupMemberData::MEMBER_TYPE_TEXTURE_SAMPLER: {
        // the border color of the sampler of the mapper, also in the commands of the mapper
        gfx::res::ResPixelBasedTextureMapperData* pMapper = static_cast<gfx::res::ResPixelBasedTextureMapperData*>(pTarget);
        gfx::res::ResTextureSampler sampler(pMapper->sampler.ToPtr());
        if (GetField() != 0) {
            break;
        }
        sampler.ptr()->borderColor = *static_cast<const ut::FloatColor*>(pValue);
        const u32 color = sampler.ptr()->borderColor.ToPicaU32();
        sampler.GetOwnerCommand().pCommands[gfx::res::ResPixelBasedTextureMapperData::COMMAND_BORDER_COLOR] = color;
        break;
    }
    case ResAnimGroupMemberData::MEMBER_TYPE_TEXTURE_MAPPER: {
        gfx::res::ResTextureMapper mapper(pTarget);
        const gfx::res::ResReferenceTextureData* pReference =
            static_cast<const ut::Offset*>(pValue)->To<gfx::res::ResReferenceTextureData>();
        if (GetField() != 0) {
            break;
        }
        mapper.SetTexture(gfx::res::ResTexture(pReference->target.ToPtr()));
        break;
    }
    case ResAnimGroupMemberData::MEMBER_TYPE_BLEND_OPERATION: {
        gfx::res::ResBlendOperationData* pOperation = static_cast<gfx::res::ResBlendOperationData*>(pTarget);
        if (GetField() != 0) {
            break;
        }
        pOperation->blendColor = *static_cast<const ut::FloatColor*>(pValue);
        pOperation->blendColorCommand = pOperation->blendColor.ToPicaU32();
        break;
    }
    case ResAnimGroupMemberData::MEMBER_TYPE_TEXTURE_COORDINATOR:
        ResTextureCoordinatorMember(m_pData).SetValue(pTarget, pValue);
        break;
    case ResAnimGroupMemberData::MEMBER_TYPE_MODEL: {
        gfx::Model* pModel = static_cast<gfx::Model*>(pTarget);
        if (GetField() == 0) {
            pModel->m_IsBranchVisible = *static_cast<const bool*>(pValue);
        } else if (GetField() == 1) {
            pModel->m_IsVisible = *static_cast<const bool*>(pValue);
        }
        break;
    }
    case ResAnimGroupMemberData::MEMBER_TYPE_MESH:
        static_cast<gfx::res::ResMeshData*>(pTarget)->isVisible = *static_cast<const bool*>(pValue);
        break;
    case ResAnimGroupMemberData::MEMBER_TYPE_MESH_NODE_VISIBILITY:
        static_cast<gfx::res::ResMeshNodeVisibilityData*>(pTarget)->isVisible = *static_cast<const bool*>(pValue);
        break;
    case ResAnimGroupMemberData::MEMBER_TYPE_SCENE_NODE: {
        if (GetField() != 0) {
            break;
        }
        gfx::TransformNode* pNode = static_cast<gfx::TransformNode*>(pTarget);
        pNode->m_Transform.SetTransform(*static_cast<const nn::math::Transform3*>(pValue));
        pNode->m_Transform.EnableFlags(gfx::CalculatedTransform::FLAG_IS_DIRTY);
        break;
    }
    case ResAnimGroupMemberData::MEMBER_TYPE_VIEW_UPDATER: {
        gfx::res::ResCameraViewUpdaterData* pUpdater = static_cast<gfx::res::ResCameraViewUpdaterData*>(pTarget);
        switch (pUpdater->typeInfo) {
        case gfx::res::ResAimTargetViewUpdaterData::TYPE: {
            gfx::res::ResAimTargetViewUpdaterData* pAim = static_cast<gfx::res::ResAimTargetViewUpdaterData*>(pUpdater);
            if (GetField() == 0) {
                pAim->targetPosition = *static_cast<const nn::math::VEC3*>(pValue);
            } else if (GetField() == 1) {
                pAim->twist = *static_cast<const f32*>(pValue);
            }
            break;
        }
        case gfx::res::ResRotateViewUpdaterData::TYPE:
            if (GetField() == 3) {
                static_cast<gfx::res::ResRotateViewUpdaterData*>(pUpdater)->viewRotate = *static_cast<const nn::math::VEC3*>(pValue);
            }
            break;
        case gfx::res::ResLookAtTargetViewUpdaterData::TYPE: {
            gfx::res::ResLookAtTargetViewUpdaterData* pLookAt = static_cast<gfx::res::ResLookAtTargetViewUpdaterData*>(pUpdater);
            if (GetField() == 0) {
                pLookAt->targetPosition = *static_cast<const nn::math::VEC3*>(pValue);
            } else if (GetField() == 2) {
                pLookAt->upwardVector = *static_cast<const nn::math::VEC3*>(pValue);
            }
            break;
        }
        default:
            break;
        }
        break;
    }
    case ResAnimGroupMemberData::MEMBER_TYPE_PROJECTION_UPDATER:
        ResProjectionUpdaterMember(m_pData).SetValue(pTarget, pValue);
        break;
    case ResAnimGroupMemberData::MEMBER_TYPE_LIGHT: {
        const bool isEnabled = *static_cast<const bool*>(pValue);
        if (GetField() == 0) {
            static_cast<gfx::res::ResLightData*>(pTarget)->isLightEnabled = isEnabled;
        }
        break;
    }
    case ResAnimGroupMemberData::MEMBER_TYPE_FRAGMENT_LIGHT:
        ResFragmentLightMember(m_pData).SetValue(pTarget, pValue);
        break;
    case ResAnimGroupMemberData::MEMBER_TYPE_VERTEX_LIGHT:
        ResVertexLightMember(m_pData).SetValue(pTarget, pValue);
        break;
    case ResAnimGroupMemberData::MEMBER_TYPE_HEMISPHERE_LIGHT:
        ResHemiSphereLightMember(m_pData).SetValue(pTarget, pValue);
        break;
    case ResAnimGroupMemberData::MEMBER_TYPE_AMBIENT_LIGHT: {
        if (GetField() != 0) {
            break;
        }
        gfx::res::ResAmbientLightData* pLight = static_cast<gfx::res::ResAmbientLightData*>(pTarget);
        pLight->color = *static_cast<const ut::FloatColor*>(pValue);
        pLight->colorU32 = pLight->color.ToPicaU32();
        break;
    }
    case ResAnimGroupMemberData::MEMBER_TYPE_FOG:
        if (GetField() == 0) {
            static_cast<gfx::res::ResFogData*>(pTarget)->color = *static_cast<const ut::FloatColor*>(pValue);
        }
        break;
    default:
        break;
    }
}

} // namespace res
} // namespace anim
} // namespace nw
