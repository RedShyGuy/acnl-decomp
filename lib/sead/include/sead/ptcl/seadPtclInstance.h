#pragma once

#include "decomp.h"
#include "sead/seadColor4f.h"
#include "sead/seadMatrix.h"
#include "sead/seadVector2.h"
#include "sead/seadVector3.h"

namespace sead {
namespace ptcl {
struct PtclStripe;
struct SimpleEmitterData;

// A particle (member names are ours).
class PtclInstance
{
public:
    PtclInstance(); // 0x00555B48 | mk7dlp:bytes [tier B]

    // @fields PtclInstance
    Vector3<f32> mPos;                       // 0x00, in the space of the emitter
    Vector3<f32> mWorldPos;                  // 0x0C
    Vector3<f32> mVel;                       // 0x18
    u32 mRot[3];                             // 0x24, angle indices
    u32 mRotVel[3];                          // 0x30
    Matrix34<f32> mEmitterRT;                // 0x3C
    Matrix34<f32> mEmitterSRT;               // 0x6C
    f32 mAlpha;                              // 0x9C
    f32 mAlphaAdd1;                          // 0xA0
    f32 mAlphaAdd2;                          // 0xA4
    s32 mAlphaSection1;                      // 0xA8, frames
    s32 mAlphaSection2;                      // 0xAC
    Vector2<f32> mScale;                     // 0xB0
    Vector2<f32> mScaleAdd1;                 // 0xB8
    Vector2<f32> mScaleAdd2;                 // 0xC0
    s32 mScaleSection1;                      // 0xC8
    s32 mScaleSection2;                      // 0xCC
    Vector2<f32> mTexOffset;                 // 0xD0
    Vector2<f32> mTexScale;                  // 0xD8
    Color4f mColor;                          // 0xE0
    s32 mCnt;                                // 0xF0
    s32 mLife;                               // 0xF4
    u32 mRandom;                             // 0xF8
    s32 mChildEmitCnt;                       // 0xFC, frames since the last emission of children
    f32 mFluctuationAlpha;                   // 0x100
    f32 mFluctuationScale;                   // 0x104
    Vector3<f32> mPosDiff;                   // 0x108
    SimpleEmitterData* mRes;                 // 0x114, NULL: the instance is free
    PtclInstance* mPrev;                     // 0x118
    PtclInstance* mNext;                     // 0x11C
    PtclStripe* mStripe;                     // 0x120
    // @end
};
// @asserts PtclInstance
ASSERT_SIZE(PtclInstance, 0x124);
// @end

} // namespace ptcl
} // namespace sead
