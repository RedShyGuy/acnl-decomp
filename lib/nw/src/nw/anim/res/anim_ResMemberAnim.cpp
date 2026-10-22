#include "nw/anim/res/anim_ResMemberAnim.h"

#include "nw/anim/res/anim_ResRgbaColorAnim.h"
#include "nw/anim/res/anim_ResTransformAnim.h"
#include "nw/anim/res/anim_ResVec3Anim.h"
#include "nw/gfx/gfx_CalculatedTransform.h"

namespace nw {
namespace anim {
namespace res {
// 0x007438D4 | nintendogs:callgraph [tier A]
int nw::anim::res::ResMemberAnim::GetPrimitiveSize() const
{
    switch (GetPrimitiveType()) {
    case PRIMITIVE_FLOAT:
    case PRIMITIVE_INT:
        return sizeof(f32);
    case PRIMITIVE_BOOL:
        return sizeof(bool);
    case PRIMITIVE_VECTOR2:
        return 2 * sizeof(f32);
    case PRIMITIVE_VECTOR3:
        return 3 * sizeof(f32);
    case PRIMITIVE_TRANSFORM:
        return sizeof(gfx::CalculatedTransform);
    case PRIMITIVE_RGBA_COLOR:
        return 4 * sizeof(f32);
    case PRIMITIVE_TEXTURE:
        return sizeof(ut::Offset);
    default:
        return 0;
    }
}

// 0x00743940 | nintendogs:callgraph [tier A]
void nw::anim::res::ResMemberAnim::ApplyCacheForType(void* pTarget, const void* pCache) const
{
    switch (GetPrimitiveType()) {
    case PRIMITIVE_FLOAT:
    case PRIMITIVE_INT:
        *static_cast<u32*>(pTarget) = *static_cast<const u32*>(pCache);
        break;
    case PRIMITIVE_BOOL:
        *static_cast<bool*>(pTarget) = *static_cast<const bool*>(pCache);
        break;
    case PRIMITIVE_VECTOR2: {
        const u32 flags = m_pData->flags;
        if (!(flags & ResVec2AnimData::FLAG_NOT_EXIST_X)) {
            static_cast<u32*>(pTarget)[0] = static_cast<const u32*>(pCache)[0];
        }
        if (!(flags & ResVec2AnimData::FLAG_NOT_EXIST_Y)) {
            static_cast<u32*>(pTarget)[1] = static_cast<const u32*>(pCache)[1];
        }
        break;
    }
    case PRIMITIVE_VECTOR3: {
        const u32 flags = m_pData->flags;
        for (int i = 0; i < 3; ++i) {
            if (!(flags & (ResVec3AnimData::FLAG_NOT_EXIST_X << i))) {
                static_cast<u32*>(pTarget)[i] = static_cast<const u32*>(pCache)[i];
            }
        }
        break;
    }
    case PRIMITIVE_TRANSFORM:
        *static_cast<gfx::CalculatedTransform*>(pTarget) = *static_cast<const gfx::CalculatedTransform*>(pCache);
        break;
    case PRIMITIVE_RGBA_COLOR: {
        const u32 flags = m_pData->flags;
        for (int i = 0; i < 4; ++i) {
            if (!(flags & (ResRgbaColorAnimData::FLAG_NOT_EXIST_R << i))) {
                static_cast<u32*>(pTarget)[i] = static_cast<const u32*>(pCache)[i];
            }
        }
        break;
    }
    case PRIMITIVE_TEXTURE:
        *static_cast<ut::Offset*>(pTarget) = *static_cast<const ut::Offset*>(pCache);
        break;
    }
}

// 0x00743A4C | nintendogs:bytes-fuzzy [tier A]
u32 nw::anim::res::ResMemberAnim::EvaluateResultForType(void* pTarget, u32 result, f32 frame, const void* pOriginalValue) const
{
    switch (GetPrimitiveType()) {
    case PRIMITIVE_FLOAT: {
        const ResMemberAnimData* pData = m_pData;
        *static_cast<f32*>(pTarget) =
            (pData->flags & ResMemberAnimData::FLAG_CONSTANT) ? pData->constantValue : CalcFloatCurve(pData->curve.To<ResFloatCurveData>(), frame);
        result |= 1;
        break;
    }
    case PRIMITIVE_INT: {
        const ResMemberAnimData* pData = m_pData;
        *static_cast<s32*>(pTarget) =
            (pData->flags & ResMemberAnimData::FLAG_CONSTANT) ? pData->constantIntValue : CalcIntCurve(pData->curve.To<ResIntCurveData>(), frame);
        result |= 1;
        break;
    }
    case PRIMITIVE_BOOL: {
        const ResMemberAnimData* pData = m_pData;
        const bool value = (pData->flags & ResMemberAnimData::FLAG_CONSTANT) ? (pData->flags & ResMemberAnimData::FLAG_CONSTANT_VALUE) >> 1
                                                                              : CalcBoolCurve(pData->curve.To<ResBoolCurveData>(), frame);
        result |= 1;
        *static_cast<bool*>(pTarget) = value;
        break;
    }
    case PRIMITIVE_VECTOR2: {
        f32* pValue = static_cast<f32*>(pTarget);
        const ResVec2AnimData* pData = reinterpret_cast<const ResVec2AnimData*>(m_pData);
        const u32 flags = pData->flags;
        if (!(flags & ResVec2AnimData::FLAG_NOT_EXIST_X)) {
            pValue[0] = (pData->flags & ResVec2AnimData::FLAG_CONSTANT_X) ? pData->components[0].constantValue
                                                                          : CalcFloatCurve(pData->components[0].curve.To<ResFloatCurveData>(), frame);
            result |= 1;
        }
        if (!(flags & ResVec2AnimData::FLAG_NOT_EXIST_Y)) {
            pData = reinterpret_cast<const ResVec2AnimData*>(m_pData);
            pValue[1] = (pData->flags & ResVec2AnimData::FLAG_CONSTANT_Y) ? pData->components[1].constantValue
                                                                          : CalcFloatCurve(pData->components[1].curve.To<ResFloatCurveData>(), frame);
            result |= 2;
        }
        break;
    }
    case PRIMITIVE_VECTOR3:
        result = static_cast<const ResVec3Anim*>(this)->EvaluateResult(pTarget, result, frame);
        break;
    case PRIMITIVE_TRANSFORM:
        result = static_cast<const ResTransformAnim*>(this)->EvaluateResult(pTarget, result, frame, pOriginalValue);
        break;
    case PRIMITIVE_RGBA_COLOR:
        result = static_cast<const ResRgbaColorAnim*>(this)->EvaluateResult(pTarget, result, frame);
        break;
    case PRIMITIVE_TEXTURE: {
        const ResTextureAnimData* pData = reinterpret_cast<const ResTextureAnimData*>(m_pData);
        const f32 indexValue = (pData->flags & ResMemberAnimData::FLAG_CONSTANT) ? pData->index.constantValue
                                                                                 : CalcFloatCurve(pData->index.curve.To<ResFloatCurveData>(), frame);
        const s32 index = static_cast<s32>(indexValue);
        // (an index out of range leaves the texture as it is)
        if (index >= 0 && index < pData->textureCount) {
            *static_cast<ut::Offset*>(pTarget) = pData->textures.To<ut::Offset>()[index];
            result |= 1;
        }
        break;
    }
    }
    return result;
}

} // namespace res
} // namespace anim
} // namespace nw
