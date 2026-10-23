#include "nw/anim/res/anim_ResMaterialColorMember.h"

#include "nw/gfx/res/gfx_ResMaterial.h"

namespace nw {
namespace anim {
namespace res {
// 0x00744A1C | nintendogs:callseq [tier A]
void nw::anim::res::ResMaterialColorMember::SetValue(void* pTarget, const void* pValue) const
{
    gfx::res::ResMaterialColorData* pColor = static_cast<gfx::res::ResMaterialColorData*>(pTarget);
    const ut::FloatColor& value = *static_cast<const ut::FloatColor*>(pValue);
    switch (GetField()) {
    case 0:
        pColor->emission = value;
        pColor->emissionU8 = pColor->emission.ToPicaU32();
        break;
    case 1:
        // (the alpha of the ambient color stays)
        pColor->ambient.r = value.r;
        pColor->ambient.g = value.g;
        pColor->ambient.b = value.b;
        pColor->ambientU8 = pColor->ambient.ToPicaU32();
        break;
    case 2:
        pColor->diffuse = value;
        pColor->diffuseU8 = pColor->diffuse.ToPicaU32();
        break;
    case 3:
        pColor->specular0 = value;
        pColor->specular0U8 = pColor->specular0.ToPicaU32();
        break;
    case 4:
        pColor->specular1 = value;
        pColor->specular1U8 = pColor->specular1.ToPicaU32();
        break;
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10: {
        const int index = GetField() - 5;
        pColor->constant[index] = value;
        pColor->constantU8[index] = pColor->constant[index].ToPicaU32();
        break;
    }
    default:
        break;
    }
}

} // namespace res
} // namespace anim
} // namespace nw
