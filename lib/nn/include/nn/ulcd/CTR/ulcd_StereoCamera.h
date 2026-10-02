#pragma once

#include "decomp.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_MTX44.h"
#include "nn/math/math_Vector3.h"

namespace nn {
namespace ulcd {
namespace CTR {

// Makes the projection and view matrices for the left and the right eye from one base camera
// (stereoscopic 3D on the upper screen). The member names are ours; the meaning follows from how
// GetParallax and CalculateMatrices use them.
class StereoCamera
{
public:
    StereoCamera(); // 0x0047EFA0 | fefates:bytes [tier B]

    // reads the stereo camera settings of the system (once per process) and clears the camera
    void Initialize(); // 0x0047E788 | nintendogs:callseq [tier A]

    // the base camera is given as a view matrix; its position and axes are stored
    void SetBaseCamera(const nn::math::MTX34* view); // 0x0047E85C | nintendogs:bytes [tier A]

    // the largest parallax (in the unit of the settings, mm on the LCD); must not be negative
    void SetParallaxLimit(f32 limit); // 0x0047E8AC (name is ours)

    // basePlaneDistance: distance from the camera to the plane that appears on the LCD surface
    // strength: strength of the 3D effect, 0 to 1
    // readSlider: read the 3D slider again (otherwise the last value is used)
    void CalculateMatrices(nn::math::MTX44* projL, nn::math::MTX34* viewL, nn::math::MTX44* projR,
                           nn::math::MTX34* viewR, f32 basePlaneDistance, f32 strength,
                           nn::math::PivotDirection pivot, bool readSlider); // 0x0047E8DC | fefates:bytes [tier B]

    // like CalculateMatrices, but with the distances of a real viewer: the camera moves back so
    // that the base frustum at basePlaneDistance is seen like the LCD at the viewing distance
    void CalculateMatricesForViewer(nn::math::MTX44* projL, nn::math::MTX34* viewL, nn::math::MTX44* projR,
                               nn::math::MTX34* viewR, f32 basePlaneDistance, f32 strength,
                               nn::math::PivotDirection pivot, bool readSlider); // 0x0047EC28 (name is ours)

    // parallax of a point at this distance from the camera, relative to the level width
    f32 GetParallax(f32 distance) const; // 0x007374E4 | nintendogs:bytes [tier A]

    f32 GetParallaxLimitOnScreen() const; // 0x0073751C (name is ours)
    f32 GetParallaxScale() const; // 0x00737560 (name is ours)

    struct Frustum
    {
        f32 left;       // 0x00
        f32 right;      // 0x04
        f32 bottom;     // 0x08
        f32 top;        // 0x0C
        f32 near;       // 0x10
        f32 far;        // 0x14
    };

    struct CameraPose
    {
        nn::math::VEC3 position;    // 0x00
        nn::math::VEC3 right;       // 0x0C
        nn::math::VEC3 up;          // 0x18
        nn::math::VEC3 target;      // 0x24 direction to the target, not a point
    };

private:
    Frustum m_BaseFrustum;      // 0x00
    CameraPose m_BaseCamera;        // 0x18
    f32 m_ParallaxLimit;            // 0x48
    f32 m_BasePlaneWidth;               // 0x4C width of the view at m_BasePlaneDistance
    f32 m_BasePlaneDistance;          // 0x50
    f32 m_NearDistance;       // 0x54
    f32 m_FarDistance;        // 0x58
    f32 m_EyeOffset;           // 0x5C how far each eye's camera is moved to the side
    f32 m_SliderValue;                 // 0x60 position of the 3D slider, 0 to 1
};
ASSERT_SIZE(StereoCamera::Frustum, 0x18);
ASSERT_SIZE(StereoCamera::CameraPose, 0x30);
ASSERT_SIZE(StereoCamera, 0x64);

} // namespace CTR
} // namespace ulcd
} // namespace nn
