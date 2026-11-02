#pragma once

#include "decomp.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_MTX44.h"
#include "nn/ulcd/CTR/ulcd_StereoCamera.h"

namespace nw {
namespace gfx {
class Camera;
} // namespace gfx
} // namespace nw

namespace graphic {
// the matrices of the 3D scene for the screens: a StereoCamera that makes the matrices of the left and the right
// image of the top screen from a camera of the engine (the class and all names are ours; only the parts g3d uses)
class ScreenCamera : public ::nn::ulcd::CTR::StereoCamera
{
public:
    // the matrices of a screen
    struct Matrices
    {
        nn::math::MTX44 projection;   // 0x00
        nn::math::MTX34 view;         // 0x40
        nn::math::MTX34 inverseView;  // 0x70
    };

    // the matrices of the left image (0x400, the draw mode of the top screen) or of the right one (others)
    const nn::math::MTX44& GetProjectionMatrix(u32 drawMode) const; // 0x00713318
    const nn::math::MTX34& GetViewMatrix(u32 drawMode) const; // 0x007132D8
    const nn::math::MTX34& GetInverseViewMatrix(u32 drawMode) const; // 0x00713328
    // the matrices of both images from the camera
    void SetCamera(const nw::gfx::Camera* camera); // 0x0020BAA0

    // the instance
    static ScreenCamera s_Instance;

    Matrices m_LeftMatrices;   // 0x064
    Matrices m_RightMatrices;  // 0x104
    u8 m_Unknown0x1A4[0x24];   // 0x1A4
};
ASSERT_OFFSET(ScreenCamera, m_LeftMatrices, 0x64);
ASSERT_OFFSET(ScreenCamera, m_RightMatrices, 0x104);
ASSERT_SIZE(ScreenCamera, 0x1C8);
} // namespace graphic
