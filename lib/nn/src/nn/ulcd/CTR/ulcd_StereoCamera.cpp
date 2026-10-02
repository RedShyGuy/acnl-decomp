#include "nn/ulcd/CTR/ulcd_StereoCamera.h"
#include "nn/cfg/CTR/CTR_Api.h"
#include "nn/cfg/CTR/detail/detail_Api.h"
#include "nn/dbg/dbg_Api.h"
#include "nn/math/math_ARMv6.h"

#include <math.h>

namespace nn {
namespace ulcd {
namespace CTR {

namespace {

// config block 0x50005, the stereo camera settings of the system (member names are ours; the
// distances are in mm)
struct StereoCameraSetting {
    f32 interocularDistance;    // 0x00 distance between the eyes of the viewer
    f32 viewDistance;           // 0x04 distance from the eyes to the LCD
    f32 screenWidth;            // 0x08
    f32 screenHeight;           // 0x0C
    f32 parallaxLimit;          // 0x10 default for StereoCamera::m_ParallaxLimit
    f32 unknown[3];             // 0x14
};

const u32 CONFIG_BLOCK_STEREO_CAMERA = 0x50005;

// 0x00975FD0
bool s_IsSettingLoaded;
// 0x00AE4F30
StereoCameraSetting s_Setting;

// the shared page of the kernel (read-only for applications)
const uptr SHARED_PAGE = 0x1FF81000;

// the 3D slider, 0 when the 3D picture is off (the 3D LED state at 0x84 is set)
inline f32 GetSliderValue()
{
    if (*reinterpret_cast<const u8*>(SHARED_PAGE + 0x84) == 0) {
        return *reinterpret_cast<const f32*>(SHARED_PAGE + 0x80);
    }
    return 0.0f;
}

// the axes of a camera (Direction is the name from the binary, the members are ours)
struct Direction {
    nn::math::VEC3 right;       // 0x00
    nn::math::VEC3 up;          // 0x0C
    nn::math::VEC3 target;      // 0x18
};

inline void Normalize(nn::math::VEC3* v)
{
    f32 scale = 1.0f / sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
    v->x *= scale;
    v->y *= scale;
    v->z *= scale;
}

// position and axes of the camera of a view matrix: the columns of its inverse; the camera looks
// along -z
// 0x0047EFBC | fefates:bytes [tier B]
DECOMP_NOINLINE void GetLookPose(const nn::math::MTX34* view, nn::math::VEC3* position, Direction* direction)
{
    nn::math::MTX34 camera;
    nn::math::ARMv6::MTX34InverseAsm(&camera, view);

    position->x = camera.m[0][3];
    position->y = camera.m[1][3];
    position->z = camera.m[2][3];

    direction->right.x = camera.m[0][0];
    direction->right.y = camera.m[1][0];
    direction->right.z = camera.m[2][0];
    direction->up.x = camera.m[0][1];
    direction->up.y = camera.m[1][1];
    direction->up.z = camera.m[2][1];
    direction->target.x = -camera.m[0][2];
    direction->target.y = -camera.m[1][2];
    direction->target.z = -camera.m[2][2];

    Normalize(&direction->right);
    Normalize(&direction->up);
    Normalize(&direction->target);
}

} // namespace

// 0x0047E788 | nintendogs:callseq [tier A]
void nn::ulcd::CTR::StereoCamera::Initialize()
{
    if (!s_IsSettingLoaded) {
        nn::cfg::CTR::Initialize();
        if (nn::cfg::CTR::detail::GetConfig(&s_Setting, sizeof(s_Setting), CONFIG_BLOCK_STEREO_CAMERA).IsFailure()) {
            nndbgPanic();
        }
        nn::cfg::CTR::Finalize();
        s_IsSettingLoaded = true;
    }

    m_ParallaxLimit = s_Setting.parallaxLimit;
    m_BasePlaneWidth = 0.0f;
    m_BasePlaneDistance = 0.0f;
    m_NearDistance = 0.0f;
    m_FarDistance = 0.0f;
    m_EyeOffset = 0.0f;
    m_SliderValue = 0.0f;

    m_BaseFrustum.left = 0.0f;
    m_BaseFrustum.right = 0.0f;
    m_BaseFrustum.bottom = 0.0f;
    m_BaseFrustum.top = 0.0f;
    m_BaseFrustum.near = 0.0f;
    m_BaseFrustum.far = 0.0f;

    const nn::math::VEC3 zero = { 0.0f, 0.0f, 0.0f };
    m_BaseCamera.position = zero;
    m_BaseCamera.right = zero;
    m_BaseCamera.up = zero;
    m_BaseCamera.target = zero;
}

// 0x0047E85C | nintendogs:bytes [tier A]
void nn::ulcd::CTR::StereoCamera::SetBaseCamera(const nn::math::MTX34* view)
{
    Direction direction;
    GetLookPose(view, &m_BaseCamera.position, &direction);
    m_BaseCamera.right = direction.right;
    m_BaseCamera.up = direction.up;
    m_BaseCamera.target = direction.target;
}

// 0x0047E8AC (name is ours)
void nn::ulcd::CTR::StereoCamera::SetParallaxLimit(f32 limit)
{
    if (limit < 0.0f) {
        nndbgPanic();
    }
    m_ParallaxLimit = limit;
}

// 0x0047E8DC | fefates:bytes [tier B]
void nn::ulcd::CTR::StereoCamera::CalculateMatrices(nn::math::MTX44* projL, nn::math::MTX34* viewL, nn::math::MTX44* projR,
                                                   nn::math::MTX34* viewR, f32 basePlaneDistance, f32 strength,
                                                   nn::math::PivotDirection pivot, bool readSlider)
{
    if (!(0.0f <= strength && strength <= 1.0f)) {
        nndbgPanic();
    }

    m_BasePlaneDistance = basePlaneDistance;

    // the limit parallax (mm on the LCD) in the units of the scene at basePlaneDistance
    f32 height = fabsf(m_BaseFrustum.top - m_BaseFrustum.bottom);
    f32 limit = height * basePlaneDistance / (m_BaseFrustum.near * s_Setting.screenHeight) * m_ParallaxLimit;

    // the interval that gives the limit parallax at the far clip plane
    if (m_BaseFrustum.far > basePlaneDistance) {
        m_EyeOffset = m_BaseFrustum.far / (m_BaseFrustum.far - basePlaneDistance) * limit;
    } else {
        m_EyeOffset = 0.0f;
    }
    m_EyeOffset *= strength;

    if (readSlider) {
        m_SliderValue = GetSliderValue();
    }
    m_EyeOffset = m_EyeOffset * m_SliderValue * 0.5f;

    // the frustums are moved against the cameras so that both meet at basePlaneDistance
    Frustum frustumL;
    Frustum frustumR;
    frustumL.left = m_EyeOffset * m_BaseFrustum.near / basePlaneDistance + m_BaseFrustum.left;
    frustumL.right = m_EyeOffset * m_BaseFrustum.near / basePlaneDistance + m_BaseFrustum.right;
    frustumR.right = m_BaseFrustum.right - m_EyeOffset * m_BaseFrustum.near / m_BasePlaneDistance;
    frustumR.left = m_BaseFrustum.left - m_EyeOffset * m_BaseFrustum.near / m_BasePlaneDistance;
    frustumR.bottom = frustumL.bottom = m_BaseFrustum.bottom;
    frustumR.top = frustumL.top = m_BaseFrustum.top;
    frustumR.near = frustumL.near = m_BaseFrustum.near;
    frustumR.far = frustumL.far = m_BaseFrustum.far;

    CameraPose cameraL;
    cameraL.position = m_BaseCamera.position - m_BaseCamera.right * m_EyeOffset;
    cameraL.target = cameraL.position + m_BaseCamera.target;
    cameraL.right = m_BaseCamera.right;
    cameraL.up = m_BaseCamera.up;

    CameraPose cameraR;
    cameraR.position = m_BaseCamera.position + m_BaseCamera.right * m_EyeOffset;
    cameraR.target = cameraR.position + m_BaseCamera.target;
    cameraR.right = m_BaseCamera.right;
    cameraR.up = m_BaseCamera.up;

    m_NearDistance = m_BaseFrustum.near;
    m_FarDistance = m_BaseFrustum.far;
    m_BasePlaneWidth = fabsf(m_BaseFrustum.right - m_BaseFrustum.left) * (m_BasePlaneDistance / m_BaseFrustum.near);

    nn::math::ARMv6::MTX44FrustumC_FAST(projL, frustumL.left, frustumL.right, frustumL.bottom, frustumL.top,
                                        frustumL.near, frustumL.far);
    nn::math::ARMv6::MTX44PivotC_FAST(projL, pivot);
    nn::math::ARMv6::MTX44FrustumC_FAST(projR, frustumR.left, frustumR.right, frustumR.bottom, frustumR.top,
                                        frustumR.near, frustumR.far);
    nn::math::ARMv6::MTX44PivotC_FAST(projR, pivot);
    nn::math::ARMv6::MTX34LookAtC_FAST(viewL, &cameraL.position, &cameraL.up, &cameraL.target);
    nn::math::ARMv6::MTX34LookAtC_FAST(viewR, &cameraR.position, &cameraR.up, &cameraR.target);
}

// 0x0047EC28 (name is ours)
void nn::ulcd::CTR::StereoCamera::CalculateMatricesForViewer(nn::math::MTX44* projL, nn::math::MTX34* viewL, nn::math::MTX44* projR,
                                                       nn::math::MTX34* viewR, f32 basePlaneDistance, f32 strength,
                                                       nn::math::PivotDirection pivot, bool readSlider)
{
    if (!(0.0f <= strength && strength <= 1.0f)) {
        nndbgPanic();
    }

    f32 left = m_BaseFrustum.left;
    f32 right = m_BaseFrustum.right;
    f32 bottom = m_BaseFrustum.bottom;
    f32 top = m_BaseFrustum.top;

    // the base frustum at basePlaneDistance, and the scene units per mm on the LCD
    f32 scale = basePlaneDistance / m_BaseFrustum.near;
    f32 width = fabsf(right - left) * scale;
    f32 baseHeight = fabsf(top - bottom);
    f32 height = baseHeight * scale;
    f32 unit = height / s_Setting.screenHeight;

    // the camera moves to the viewing distance; the clip planes keep their distance to basePlaneDistance
    m_BasePlaneDistance = s_Setting.viewDistance * unit;
    f32 near = m_BasePlaneDistance - (basePlaneDistance - m_BaseFrustum.near);
    if (near <= 0.0f) {
        near = m_BasePlaneDistance * 0.01f;
    }
    f32 far = m_BasePlaneDistance + (m_BaseFrustum.far - basePlaneDistance);
    if (far <= near) {
        far = near * 2.0f;
    }

    f32 ratio = near / m_BasePlaneDistance;
    width *= ratio;
    f32 frustumScale = height * ratio / baseHeight;
    left *= frustumScale;
    right *= frustumScale;
    bottom *= frustumScale;
    top *= frustumScale;

    m_EyeOffset = s_Setting.interocularDistance * unit * strength;
    if (readSlider) {
        m_SliderValue = GetSliderValue();
    }
    m_EyeOffset = m_EyeOffset * m_SliderValue * 0.5f;

    Frustum frustumL;
    Frustum frustumR;
    frustumL.left = m_EyeOffset * near / m_BasePlaneDistance + left;
    frustumL.right = m_EyeOffset * near / m_BasePlaneDistance + right;
    frustumR.right = right - m_EyeOffset * near / m_BasePlaneDistance;
    frustumR.left = left - m_EyeOffset * near / m_BasePlaneDistance;
    frustumR.bottom = frustumL.bottom = bottom;
    frustumR.top = frustumL.top = top;
    frustumR.near = frustumL.near = near;
    frustumR.far = frustumL.far = far;

    nn::math::VEC3 position = m_BaseCamera.position - m_BaseCamera.target * (m_BasePlaneDistance - basePlaneDistance);

    CameraPose cameraL;
    cameraL.position = position - m_BaseCamera.right * m_EyeOffset;
    cameraL.target = cameraL.position + m_BaseCamera.target;
    cameraL.right = m_BaseCamera.right;
    cameraL.up = m_BaseCamera.up;

    CameraPose cameraR;
    cameraR.position = position + m_BaseCamera.right * m_EyeOffset;
    cameraR.target = cameraR.position + m_BaseCamera.target;
    cameraR.right = m_BaseCamera.right;
    cameraR.up = m_BaseCamera.up;

    m_NearDistance = near;
    m_FarDistance = far;
    m_BasePlaneWidth = m_BasePlaneDistance / near * width;

    nn::math::ARMv6::MTX44FrustumC_FAST(projL, frustumL.left, frustumL.right, frustumL.bottom, frustumL.top,
                                        frustumL.near, frustumL.far);
    nn::math::ARMv6::MTX44PivotC_FAST(projL, pivot);
    nn::math::ARMv6::MTX44FrustumC_FAST(projR, frustumR.left, frustumR.right, frustumR.bottom, frustumR.top,
                                        frustumR.near, frustumR.far);
    nn::math::ARMv6::MTX44PivotC_FAST(projR, pivot);
    nn::math::ARMv6::MTX34LookAtC_FAST(viewL, &cameraL.position, &cameraL.up, &cameraL.target);
    nn::math::ARMv6::MTX34LookAtC_FAST(viewR, &cameraR.position, &cameraR.up, &cameraR.target);
}

// 0x0047EFA0 | fefates:bytes [tier B]
nn::ulcd::CTR::StereoCamera::StereoCamera()
{
    m_BasePlaneDistance = 0.0f;
    m_EyeOffset = 0.0f;
}

// 0x007374E4 | nintendogs:bytes [tier A]
f32 nn::ulcd::CTR::StereoCamera::GetParallax(f32 distance) const
{
    if (distance <= 0.0f) {
        return 0.0f;
    }
    return (distance - m_BasePlaneDistance) * m_EyeOffset / distance / m_BasePlaneWidth;
}

// 0x0073751C (name is ours)
f32 nn::ulcd::CTR::StereoCamera::GetParallaxLimitOnScreen() const
{
    return m_ParallaxLimit / s_Setting.screenWidth * 0.5f * GetSliderValue();
}

// 0x00737560 (name is ours)
f32 nn::ulcd::CTR::StereoCamera::GetParallaxScale() const
{
    return m_EyeOffset / m_BasePlaneWidth;
}

} // namespace CTR
} // namespace ulcd
} // namespace nn
