#include "nw/gfx/res/gfx_ResGraphicsFile.h"
#include "nw/gfx/res/gfx_ResLight.h"

namespace nw {
namespace gfx {
namespace res {
// 0x001376F4 | nintendogs:bytes [tier A]
void nw::gfx::res::ResLight::Cleanup()
{
}

// 0x004AA2E8 | nintendogs:bytes [tier A]
u32 nw::gfx::res::ResLight::Setup(os::IAllocator* /*pAllocator*/, ResGraphicsFile graphicsFile)
{
    u32 result = RESULT_SUCCESS;
    ResFragmentLight light(DynamicCastData<ResLightData>(m_pData, TYPE_FRAGMENT_LIGHT));
    if (!light.IsValid()) {
        return result;
    }

    // the distance attenuation table
    ResLookupTable distanceSampler = light.GetDistanceSampler();
    const u32 distanceResult = SetupReferenceLut(
        ResReferenceLookupTable(DynamicCastData<ResLookupTableData>(distanceSampler.ptr(), ResLookupTable::TYPE_REFERENCE)), graphicsFile);
    if (light.GetDistanceSampler().IsValid() && !(distanceResult & RESULT_FAILURE_MASK)) {
        result |= light.GetDistanceSampler().Setup();
    }
    result |= distanceResult;

    // the spot angle table
    if (light.GetAngleSampler().IsValid()) {
        ResLookupTable angleSampler = light.GetAngleSampler().GetSampler();
        if (!angleSampler.IsValid()) {
            result |= RESULT_LUT_MISSING;
            return result;
        }
        const u32 angleResult = SetupReferenceLut(
            ResReferenceLookupTable(IsTypeOf(angleSampler.ptr(), ResLookupTable::TYPE_REFERENCE) ? angleSampler.ptr() : nullptr), graphicsFile);
        if (!(angleResult & RESULT_FAILURE_MASK)) {
            result |= angleSampler.Setup();
        }
        result |= angleResult;
    }
    return result;
}

} // namespace res
} // namespace gfx
} // namespace nw
