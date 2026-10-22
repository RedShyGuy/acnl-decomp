#pragma once

#include "decomp.h"
#include "nn/math/math_Vector4.h"
#include "nw/gfx/res/gfx_ResLookupTable.h"

namespace nw {
namespace gfx {
// The state of the GPU that gfx keeps itself: which lookup table is in which PICA table (the class
// and function names are from the reference symbols, the rest is ours).
class GraphicsDevice
{
public:
    // where a lookup table is loaded: 0-22 the tables of the fragment lighting (without the PICA id 7),
    // 23 the fog table
    enum LutTarget
    {
        LUT_TARGET_FOG = 23,
        LUT_TARGET_COUNT = 24,
    };

    // the commands that load the table (nothing if it is there already)
    static void ActivateLookupTable(res::ResImageLookupTable lut, LutTarget target); // 0x004998A4 | nintendogs:bytes [tier A]
    // the table is no longer loaded anywhere
    static void InvalidateLookupTable(res::ResImageLookupTable lut); // 0x0049993C | nintendogs:bytes [tier A]
    static void InvalidateAllLookupTables(); // 0x0049996C | nintendogs:bytes [tier A]
    // the position of a fragment light (as 16-bit floats)
    static void ActivateFragmentLightPosition(int lightIdx, const nn::math::VEC4& position); // 0x00499994 | nintendogs:bytes [tier A]

    static const res::ResImageLookupTableData* s_LookupTables[LUT_TARGET_COUNT];
};
} // namespace gfx
} // namespace nw
