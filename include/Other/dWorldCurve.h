#pragma once

#include "decomp.h"
#include "math/dAngle16.h"
#include "nn/math/math_Vector3.h"

// The curve of the world of the stages that roll away from the camera (functions of the game; names are ours, only
// the declarations g3d uses): the position on the curve and the angle the ground tilts there.
math::Angle16 GetWorldCurvePosition(nn::math::VEC3* curvedPosition, const nn::math::VEC3& position); // 0x0052E4A4
// the same relative to the current camera
// the matrix of position on the curve of the world from the camera, turned by rotateY (name is ours)
void MakeWorldCurveMatrix(nn::math::MTX34* out, const nn::math::VEC3* position, math::Angle16 rotateY); // 0x0052E940
math::Angle16 GetWorldCurvePositionFromCamera(nn::math::VEC3* curvedPosition, const nn::math::VEC3& position); // 0x0052E898
// the position on the curve of the world on stages that have it (stage::HasWorldCurve), else the position itself
void Func52EA98(nn::math::VEC3* out, const nn::math::VEC3* position); // 0x0052EA98
