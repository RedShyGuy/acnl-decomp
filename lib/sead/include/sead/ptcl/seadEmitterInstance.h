#pragma once

#include "decomp.h"
#include "sead/ptcl/seadEmitterSet.h"
#include "sead/ptcl/seadPtclRandom.h"
#include "sead/seadMatrix.h"

namespace sead {
namespace ptcl {
class EmitterCalc;
class EmitterSet;
class PtclInstance;
struct SimpleEmitterData;

// An emitter of an emitter set (member names are ours).
class EmitterInstance
{
public:
    EmitterInstance(); // 0x00557F40 | mk7dlp:bytes [tier B]

    // @fields EmitterInstance
    s32 mCnt;                                // 0x00
    s32 mEmitCnt;                            // 0x04, frames since the last emission
    u32 mGroupID;                            // 0x08
    EmitterSet* mEmitterSet;                 // 0x0C
    EmitterSetRecord* mRecord;               // 0x10
    u32 mCreateID;                           // 0x14
    Matrix34<f32> mRT;                       // 0x18
    Matrix34<f32> mSRT;                      // 0x48
    PtclRandom mRandom;                      // 0x78
    s32 mEmitInterval;                       // 0x80, frames to the next emission
    f32 mFadeAlpha;                          // 0x84
    s32 mGpuStopFrame;                       // 0x88, GPU: the frame the emission stopped in a fade, -1: none
    s32 mGpuLoopState;                       // 0x8C, GPU, infinite emitters: 0 running, 1/3 stopped before/after the loop frame, 2/4 done
    s32 mGpuFadeCnt;                         // 0x90, GPU: the frames until the particles of a fade are dead
    s32 mGpuCnt;                             // 0x94, GPU: the frame of the shaders (wraps for infinite emitters)
    u32 mGpuSet;                             // 0x98, GPU: the set of random vertices of sGpuParticlePrimitive
    EmitterInstance* mPrev;                  // 0x9C
    EmitterInstance* mNext;                  // 0xA0
    EmitterCalc* mCalc;                      // 0xA4, NULL: the instance is free
    SimpleEmitterData* mRes;                 // 0xA8
    PtclInstance* mPtclHead;                 // 0xAC
    PtclInstance* mChildHead;                // 0xB0
    // @end
};
// @asserts EmitterInstance
ASSERT_SIZE(EmitterInstance, 0xB4);
// @end

} // namespace ptcl
} // namespace sead
