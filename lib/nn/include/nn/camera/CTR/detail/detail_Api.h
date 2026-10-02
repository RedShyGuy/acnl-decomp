#pragma once

#include "decomp.h"

namespace nn {
namespace camera {
namespace CTR {
namespace detail {
void ArriveApplication(); // 0x0012BD6C | fefates:bytes [tier B]
void LeaveApplication(); // 0x00131D38 | fefates:bytes [tier B]
void GetMaxLines(short, short); // 0x004816D0 | nintendogs:bytes [tier B]
void PlayShutterSound(nn::camera::CTR::ShutterSoundType); // 0x00481C44 | nintendogs:bytes [tier B]
void SetTrimmingParams(nn::camera::CTR::Port, short, short, short, short); // 0x00481E38 | nintendogs:bytes [tier B]
void SetPackageParameterWithContext(const nn::camera::CTR::PackageParameterContextDetail&); // 0x00481F58 | nintendogs:bytes [tier B]
void IsBusy(nn::camera::CTR::Port); // 0x00482774 | nintendogs:bytes [tier B]
void FlipImage(nn::camera::CTR::CameraSelect, nn::camera::CTR::Flip, nn::camera::CTR::Context); // 0x00482794 | nintendogs:bytes [tier B]
} // namespace detail
} // namespace CTR
} // namespace camera
} // namespace nn
