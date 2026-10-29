#pragma once

#include "decomp.h"
#include "sead/seadVector2.h"

namespace sead {
namespace hostio {
// RTTI N4sead6hostio6ICurveE @ 0x008D2128
// The interface of the curves: the value at t and the point (t, value) (no vtable of its own in the
// binary; the slot names are ours).
class ICurve
{
public:
    virtual f32 interpolateToF32(f32 t) = 0;
    virtual Vector2<f32> interpolateToVec2f(f32 t) = 0;
};
} // namespace hostio
} // namespace sead
