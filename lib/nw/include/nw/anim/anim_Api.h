#pragma once

#include "decomp.h"

namespace nw {
namespace anim {
// how a frame controller maps its frame into [startFrame, endFrame] (the signature after the
// reference symbols, the type and parameter names are ours)
typedef f32 (*PlayPolicyFunc)(f32 startFrame, f32 endFrame, f32 frame, void* pUserData);

// the animation repeated
f32 PlayPolicy_Loop(f32 startFrame, f32 endFrame, f32 frame, void* pUserData);
// the animation played once: held at the first and the last frame
f32 PlayPolicy_Onetime(f32 startFrame, f32 endFrame, f32 frame, void* pUserData);
} // namespace anim
} // namespace nw
