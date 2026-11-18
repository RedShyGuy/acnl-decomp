#include "Other/dToolBank.h"

// The variables and the state table of ToolBank (in a file of their own: GCC would fold them into the functions).

// 0x0095D570
f32 ToolBank::s_FloatGravity = -0.45f;
// 0x0095D574
s32 ToolBank::s_FloatFallFrames = 38;
// 0x0095D578
f32 ToolBank::s_FloatDriftSpeed = 0.3f;

// 0x0095D5A0
const ToolBank::FloatStateInfo ToolBank::s_FloatStates[FLOAT_STATE_NUM] = {
    {&ToolBank::EnterFloatHold, &ToolBank::UpdateFloatHold, &ToolBank::ExitFloatHold, false, true},
    {&ToolBank::EnterFloatCast, &ToolBank::UpdateFloatCast, &ToolBank::ExitFloatCast, false, true},
    {&ToolBank::EnterFloatSwing, &ToolBank::UpdateFloatSwing, &ToolBank::ExitFloatSwing, false, true},
    {&ToolBank::EnterFloatFall, &ToolBank::UpdateFloatFall, &ToolBank::ExitFloatFall, false, true},
    {&ToolBank::EnterFloatFloat, &ToolBank::UpdateFloatFloat, &ToolBank::ExitFloatFloat, true, false},
    {&ToolBank::EnterFloatBite, &ToolBank::UpdateFloatBite, &ToolBank::ExitFloatBite, true, false},
    {&ToolBank::EnterFloatHooked, &ToolBank::UpdateFloatHooked, &ToolBank::ExitFloatHooked, false, false},
    {&ToolBank::EnterFloatReel, &ToolBank::UpdateFloatReel, &ToolBank::ExitFloatReel, false, true},
    {&ToolBank::EnterFloatPullOut, &ToolBank::UpdateFloatPullOut, &ToolBank::ExitFloatPullOut, false, false},
    {&ToolBank::EnterFloatSink, &ToolBank::UpdateFloatSink, &ToolBank::ExitFloatSink, true, true},
};

// 0x008878D0
const f32 ToolBank::s_HitHeightMargin = 24.0f;
// 0x008878D4
const f32 ToolBank::s_HitHeadEnd = 1.2f;
// 0x008878D8
const f32 ToolBank::s_HitHeadHalfWidth = 0.22f;
// 0x008878DC
const f32 ToolBank::s_HitHeadLength = 0.41f;
// 0x008878E0
const f32 ToolBank::s_HitHandleX = 0.2f;
// 0x008878E4
const f32 ToolBank::s_HitPoint0Z = 0.5f;
// 0x008878E8
const f32 ToolBank::s_HitPoint0X = 0.4f;
// 0x008878EC
const f32 ToolBank::s_HitGripZ = 0.135f;
// 0x008878F0
const f32 ToolBank::s_HitGripX = 0.2f;
// 0x00887948
const f32 ToolBank::s_FloatShadowStep = 0.05f;
// 0x00891308
const f32 ToolBank::s_VisibilityFrames[8] = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 2.0f, 2.0f};
