#pragma once

#include "decomp.h"
#include "math/dAngle16.h"
#include "nn/math/math_Vector3.h"

// The curve of the world of the stages that roll away from the camera (functions of the game; names are ours, only
// the declarations g3d uses): the position on the curve and the angle the ground tilts there.
math::Angle16 GetWorldCurvePosition(nn::math::VEC3* curvedPosition, const nn::math::VEC3& position); // 0x0052E4A4
// the same relative to the current camera
math::Angle16 GetWorldCurvePositionFromCamera(nn::math::VEC3* curvedPosition, const nn::math::VEC3& position); // 0x0052E898
