#include <cmath>

#include "nn/math/math_Api.h"
#include "nw/math/math_Api.h"

namespace nw {
namespace math {
namespace {
// (inline; name is ours)
inline nn::math::VEC3 Normalize(const nn::math::VEC3& v)
{
    f32 scale = 1.0f / std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return nn::math::VEC3(v.x * scale, v.y * scale, v.z * scale);
}
} // namespace

// 0x004D9948 | nintendogs:bytes-fuzzy [confirmed by mk7dlp] [tier A]
void MTX34LookAtFIdx(nn::math::MTX34* pOut, const nn::math::VEC3* pCamPos, f32 twist, const nn::math::VEC3* pTarget)
{
    nn::math::VEC3 look = *pCamPos - *pTarget;
    f32 (*m)[4] = pOut->m;
    if (look.x == 0.0f && look.z == 0.0f) {
        // straight up or down: the x axis stays, y and z are the world z and y axes
        m[0][0] = 1.0f;
        m[0][1] = 0.0f;
        m[0][2] = 0.0f;
        m[0][3] = -pCamPos->x;
        m[1][0] = 0.0f;
        m[1][1] = 0.0f;
        m[2][0] = 0.0f;
        m[2][2] = 0.0f;
        if (look.y > 0.0f) {
            m[1][2] = -1.0f;
            m[1][3] = pCamPos->z;
            m[2][1] = 1.0f;
            m[2][3] = -pCamPos->y;
        } else {
            m[1][2] = 1.0f;
            m[1][3] = -pCamPos->z;
            m[2][1] = -1.0f;
            m[2][3] = pCamPos->y;
        }
        return;
    }

    // z: from the target to the camera; x: horizontal; y = z x x
    nn::math::VEC3 axisZ = Normalize(look);
    nn::math::VEC3 axisX = Normalize(nn::math::VEC3(look.z, 0.0f, -look.x));
    nn::math::VEC3 axisY(axisZ.y * axisX.z - axisZ.z * axisX.y, axisZ.z * axisX.x - axisZ.x * axisX.z, axisZ.x * axisX.y - axisZ.y * axisX.x);

    // turned by the twist (the y component of the horizontal axis is 0)
    f32 sin;
    f32 cos;
    nn::math::SinCosFIdx(&sin, &cos, twist);
    nn::math::VEC3 right(axisX.x * cos + axisY.x * sin, axisY.y * sin, axisX.z * cos + axisY.z * sin);
    nn::math::VEC3 up(axisY.x * cos - axisX.x * sin, axisY.y * cos, axisY.z * cos - axisX.z * sin);

    m[0][0] = right.x;
    m[0][1] = right.y;
    m[0][2] = right.z;
    m[0][3] = -(pCamPos->x * right.x + pCamPos->y * right.y + pCamPos->z * right.z);
    m[1][0] = up.x;
    m[1][1] = up.y;
    m[1][2] = up.z;
    m[1][3] = -(pCamPos->x * up.x + pCamPos->y * up.y + pCamPos->z * up.z);
    m[2][0] = axisZ.x;
    m[2][1] = axisZ.y;
    m[2][2] = axisZ.z;
    m[2][3] = -(pCamPos->x * axisZ.x + pCamPos->y * axisZ.y + pCamPos->z * axisZ.z);
}

} // namespace math
} // namespace nw
