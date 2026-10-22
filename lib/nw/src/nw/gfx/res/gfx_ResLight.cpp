#include <cmath>
#include <cstring>

#include "nw/gfx/res/gfx_ResGraphicsFile.h"
#include "nw/gfx/res/gfx_ResLight.h"

namespace nw {
namespace gfx {
namespace res {
namespace {
// (inline) a float as the PICA float20: 1 sign, 7 exponent and 12 mantissa bits (a too large one
// becomes the largest positive value; name is ours)
inline u32 Float32ToFloat20(f32 value)
{
    u32 bits;
    std::memcpy(&bits, &value, sizeof(bits));
    const u32 sign = (bits & 0x80000000) ? 0x80000 : 0;
    const u32 mantissa = ((bits << 9) >> 9) >> 11;
    s32 exponent = 0;
    if ((bits & 0x7FFFFFFF) != 0) {
        exponent = static_cast<s32>((bits << 1) >> 24) - 64;
        if (exponent < 0) {
            return sign;
        }
        if (exponent > 0x7F) {
            return 0x7F000;
        }
    }
    return mantissa | sign | ((exponent & 0x7F) << 12);
}

// the smallest distance between the start and the end of the attenuation
const f32 DISTANCE_ATTENUATION_MIN = 0.01f;
} // namespace

// 0x001376F4 | nintendogs:bytes [tier A]
void nw::gfx::res::ResLight::Cleanup()
{
    ResFragmentLight light(DynamicCastData<ResLightData>(m_pData, TYPE_FRAGMENT_LIGHT));
    if (!light.IsValid()) {
        return;
    }
    if (light.GetDistanceSampler().IsValid()) {
        light.GetDistanceSampler().Cleanup();
    }
    if (light.GetAngleSampler().IsValid()) {
        ResLookupTable angleSampler = light.GetAngleSampler().GetSampler();
        if (angleSampler.IsValid()) {
            angleSampler.Cleanup();
        }
    }
}

// 0x004A76EC (name is ours)
void nw::gfx::res::ResFragmentLight::SetDistanceAttenuation(f32 start, f32 end)
{
    f32 distance = end - start;
    if (std::fabs(distance) < DISTANCE_ATTENUATION_MIN) {
        distance = DISTANCE_ATTENUATION_MIN;
    }
    ptr()->distanceAttenuationScale = Float32ToFloat20(1.0f / distance);
    ptr()->distanceAttenuationBias = Float32ToFloat20(-start / distance);
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
