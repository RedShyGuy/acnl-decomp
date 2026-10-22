#pragma once

#include "decomp.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Quaternion.h"
#include "nn/math/math_Vector3.h"
#include "nw/ut/ut_Offset.h"

// The animation curves of the binary resources. The struct names that the reference symbols use are
// ResFloatCurveData, ResIntCurveData, ResBoolCurveData, ResBakedCurveData and ResFullBakedCurveData;
// the other names and all member names are ours.
namespace nw {
namespace anim {
namespace res {
// how a curve continues before its first and behind its last frame
enum RepeatMethod : u8
{
    REPEAT_METHOD_CLAMP = 0,  // the value at the first / last frame
    REPEAT_METHOD_REPEAT = 1, // the curve again
    REPEAT_METHOD_MIRROR = 2, // the curve back and forth
};

// the part all curves start with
struct ResAnimCurveData
{
    // the frame inside [startFrame, endFrame] for a frame outside (by the repeat methods)
    f32 NormalizeFrame(f32 frame) const;

    f32 startFrame;      // 0x0
    f32 endFrame;        // 0x4
    u8 preRepeatMethod;  // 0x8 RepeatMethod
    u8 postRepeatMethod; // 0x9 RepeatMethod
    u8 padding[2];       // 0xA
};
ASSERT_SIZE(ResAnimCurveData, 0xC);

// a piece of a float curve with its keys
struct ResFloatSegmentData
{
    enum Flag
    {
        FLAG_CONSTANT = 1 << 0,     // one value, no keys
        INTERPOLATION_SHIFT = 2,    // bits 2-4: InterpolationType
        KEY_FORMAT_SHIFT = 5,       // bits 5-7: the format of the keys
    };

    f32 startFrame; // 0x0
    f32 endFrame;   // 0x4
    u32 flags;      // 0x8
    union           // 0xC
    {
        f32 constantValue;
        u16 keyCount; // the start of the keys (see internal::KeysHeader)
    };
};

enum InterpolationType
{
    INTERPOLATION_STEP = 0,
    INTERPOLATION_LINEAR = 1,
    INTERPOLATION_HERMITE = 2,
};

struct ResFloatCurveData : public ResAnimCurveData
{
    enum Flag
    {
        FLAG_COMPOSITE = 1 << 0,      // the sum of two curves
        FLAG_CONSTANT = 1 << 1,       // one value
        FLAG_SINGLE_SEGMENT = 1 << 2, // one segment
    };

    u32 flags; // 0xC
    union      // 0x10
    {
        f32 constantValue;
        u32 segmentCount;
        ut::Offset firstCurve; // FLAG_COMPOSITE
    };
    union // 0x14
    {
        ut::Offset segments[1]; // (variable length)
        ut::Offset secondCurve; // FLAG_COMPOSITE
    };
};
ASSERT_OFFSET(ResFloatCurveData, flags, 0xC);
ASSERT_OFFSET(ResFloatCurveData, segments, 0x14);

struct ResIntCurveData : public ResAnimCurveData
{
    enum Flag
    {
        FLAG_CONSTANT = 1 << 0,
        KEY_FORMAT_SHIFT = 3, // bits 3-5
    };

    u32 flags; // 0xC
    union      // 0x10
    {
        s32 constantValue;
        u16 keyCount; // the start of the keys
    };
};

struct ResBoolCurveData : public ResAnimCurveData
{
    enum Flag
    {
        FLAG_CONSTANT = 1 << 0,
        FLAG_CONSTANT_VALUE = 1 << 1, // the value of a constant curve
        FLAG_BAKED = 1 << 2,          // one bit per frame
        KEY_FORMAT_SHIFT = 3,         // bits 3-5 (the keys of ResIntCurveData)
    };

    u32 flags; // 0xC
    union      // 0x10
    {
        u8 bits[1]; // FLAG_BAKED
        u16 keyCount;
    };
};

// a key of a baked curve: the value of a frame and the flags of the value
template <typename T>
struct ResBakedKey
{
    T value;
    u32 flags;
};

// a curve with a value per frame
template <typename T>
struct ResBakedCurveData : public ResAnimCurveData
{
    enum Flag
    {
        FLAG_CONSTANT = 1 << 0, // the value of the first frame for all
    };

    u32 flags;               // 0xC
    ResBakedKey<T> keys[1];  // 0x10
};

// a curve with a matrix per frame
struct ResFullBakedCurveData : public ResAnimCurveData
{
    nn::math::MTX34 keys[1]; // 0xC
};

s32 CalcIntCurve(const ResIntCurveData* pCurve, f32 frame);
bool CalcBoolCurve(const ResBoolCurveData* pCurve, f32 frame);
f32 CalcFloatCurve(const ResFloatCurveData* pCurve, f32 frame);
// (name is ours)
void CalcRotateCurve(nn::math::MTX34* pMtx, u32* pFlags, const ResBakedCurveData<nn::math::QUAT>* pCurve, f32 frame);
void CalcVector3Curve(nn::math::VEC3* pValue, u32* pFlags, const ResBakedCurveData<nn::math::VEC3_>* pCurve, f32 frame);
void CalcTransformCurve(nn::math::MTX34* pMtx, const ResFullBakedCurveData* pCurve, f32 frame);
void CalcTranslateCurve(nn::math::MTX34* pMtx, u32* pFlags, const ResBakedCurveData<nn::math::VEC3_>* pCurve, f32 frame);
} // namespace res
} // namespace anim
} // namespace nw
