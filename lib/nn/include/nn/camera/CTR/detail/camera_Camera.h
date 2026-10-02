#pragma once

#include "decomp.h"

namespace nn {
namespace camera {
namespace CTR {
namespace detail {
class Camera
{
public:
    void GetSleepCamera(nn::camera::CTR::CameraSelect*); // 0x00131DA8 | nintendogs:bytes [tier A]
    void GetActivatedCamera(nn::camera::CTR::CameraSelect*); // 0x00131E20 | nintendogs:bytes [tier A]
    void DriverFinalize(); // 0x001373D0 | nintendogs:bytes [tier B]
    void DriverInitialize(); // 0x004822D0 | nintendogs:bytes [tier B]
    void GetStereoCameraCalibrationData(nn::camera::CTR::StereoCameraCalibrationData*); // 0x00482634 | nintendogs:bytes [tier B]
    void GetSuitableY2rStandardCoefficient(nn::y2r::CTR::StandardCoefficient*); // 0x0048269C | nintendogs:bytes [tier B]
};
} // namespace detail
} // namespace CTR
} // namespace camera
} // namespace nn
