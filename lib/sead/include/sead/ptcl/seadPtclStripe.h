#pragma once

#include "decomp.h"
#include "sead/seadColor4f.h"
#include "sead/seadMatrix.h"
#include "sead/seadVector3.h"

namespace sead {
namespace ptcl {
class PtclInstance;
struct SimpleEmitterData;

// A point of the path of a stripe (member names are ours).
struct StripeHistory
{
    DECOMP_EMIT_COPY StripeHistory() {} // 0x0055898C (empty; ARMCC builds the array with it)

    Vector3<f32> pos;   // 0x00
    f32 scale;          // 0x0C
    Matrix34<f32> mtx;  // 0x10
    Vector3<f32> dir;   // 0x40
};
ASSERT_SIZE(StripeHistory, 0x4C);

// A ribbon that follows a particle (the struct name is from the reference symbols, the member names
// are ours).
struct PtclStripe
{
    PtclStripe(); // 0x0054F158 (name is ours)

    // @fields PtclStripe
    PtclInstance* mPtcl;                     // 0x00
    s32 mHistoryStart;                       // 0x04, the oldest entry of the ring
    s32 mHistoryPos;                         // 0x08, the next entry of the ring
    StripeHistory mHistory[128];             // 0x0C
    s32 mNumHistory;                         // 0x260C
    u32 mGroupID;                            // 0x2610
    SimpleEmitterData* mRes;                 // 0x2614, NULL: the stripe is free
    s32 mCnt;                                // 0x2618
    Color4f mColor;                          // 0x261C
    f32 mPtclTexOffsetU;                     // 0x262C, of the particle
    f32 mPtclTexOffsetV;                     // 0x2630
    f32 mPtclTexScaleU;                      // 0x2634
    f32 mPtclTexScaleV;                      // 0x2638
    f32 mTexScrollU;                         // 0x263C, StripeData::texScrollU added per frame
    f32 mTexScrollV;                         // 0x2640
    Matrix34<f32> mEmitterMtx;               // 0x2644
    Vector3<f32> mDir;                       // 0x2674, the smoothed direction of the movement
    PtclStripe* mPrev;                       // 0x2680
    PtclStripe* mNext;                       // 0x2684
    // @end
};
// @asserts PtclStripe
ASSERT_SIZE(PtclStripe, 0x2688);
// @end

} // namespace ptcl
} // namespace sead
