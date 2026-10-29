#pragma once

#include "decomp.h"
#include "sead/seadColor4f.h"
#include "sead/seadMatrix.h"
#include "sead/seadVector2.h"
#include "sead/seadVector3.h"

namespace sead {
namespace ptcl {
class EmitterInstance;
class PtclInstance;
class PtclSystem;

// a function called for each particle of a complex emitter (name is ours)
typedef void (*PtclCallback)(EmitterInstance* emitter, PtclInstance* ptcl);
typedef void (*EmitterCallback)(EmitterInstance* emitter);

// The factors of an emitter of a set (8.8 fixed point; member names are ours).
struct EmitterSetRecord
{
    // @fields EmitterSetRecord
    s32 emitRate;                            // 0x00, 8.8 fixed point factors (256 = 1)
    s32 emitInterval;                        // 0x04
    s32 life;                                // 0x08
    EmitterCallback emitterCallback;         // 0x0C, called once per frame (complex emitters)
    PtclCallback callback;                   // 0x10, called for each particle (complex emitters)
    PtclCallback childCallback;              // 0x14, called for each child
    // @end
};
// @asserts EmitterSetRecord
ASSERT_SIZE(EmitterSetRecord, 0x18);
// @end

// A set of up to eight emitters created together (member names are ours).
class EmitterSet
{
public:
    EmitterSet(); // 0x0054E734 | mk7dlp:bytes [tier B]

    // moves the particles of the emitters by v
    void emitParticle(const Vector3<f32>& v); // 0x0054E44C | mk7dlp:bytes [tier B]
    // kills the emitters that belong to this creation of the set
    void kill(); // 0x0054E4FC | mk7dlp:bytes [tier B]
    // the matrix of the set (mRT and mScale from its axes)
    void setMtx(const Matrix34<f32>& mtx); // 0x0054E544 | mk7dlp:bytes [tier B]
    // calculates the emitters num times into cmd; the most words one frame needed (name is ours)
    s32 preCalc(s32 num, u32* cmd); // 0x0054E6A8

    // @fields EmitterSet
    PtclSystem* mSystem;                     // 0x00
    s32 mNumCreated;                         // 0x04, the emitters that still exist
    s32 mNumEmitter;                         // 0x08
    u32 mCreateID;                           // 0x0C
    EmitterInstance* mInstance[8];           // 0x10
    EmitterSetRecord mRecord[8];             // 0x30
    s32 mResourceID;                         // 0xF0
    s32 mEmitterSetID;                       // 0xF4
    s32 field_0xF8;                          // 0xF8
    Matrix34<f32> mSRT;                      // 0xFC
    Matrix34<f32> mRT;                       // 0x12C, mSRT with axes of length 1
    Vector3<f32> mScale;                     // 0x15C, the lengths of the axes of mSRT
    Vector2<f32> mPtclScale;                 // 0x168
    Vector2<f32> mPtclEmitScale;             // 0x170
    Vector2<f32> mPtclScaleXY;               // 0x178, mPtclScale times the scale of the x and y axes
    Vector3<f32> mEmitterVolumeScale;        // 0x180
    Color4f mColor;                          // 0x18C
    f32 mFigureVelocityScale;                // 0x19C
    f32 mDirectionalVelocityScale;           // 0x1A0
    f32 mRandomVelocityScale;                // 0x1A4
    Vector3<f32> mAddVelocity;               // 0x1A8
    Vector3<f32> mDirectional;               // 0x1B4
    s32 mStartFrame;                         // 0x1C0
    s32 mNumEmitPos;                         // 0x1C4
    Vector3<f32>* mEmitPos;                  // 0x1C8, each emission is repeated at these offsets
    bool mIsFadeRequest;                     // 0x1CC
    bool mIsSetDirectional;                  // 0x1CD
    bool mIsStopCalc;                        // 0x1CE
    bool mIsInfinite;                        // 0x1CF, an emitter emits forever
    // @end
};
// @asserts EmitterSet
ASSERT_SIZE(EmitterSet, 0x1D0);
// @end

// A handle to a creation of an emitter set (member names are ours).
struct Handle
{
    bool isValid() const { return mEmitterSet != nullptr && mEmitterSet->mCreateID == mCreateID; }

    EmitterSet* mEmitterSet; // 0x0
    u32 mCreateID;           // 0x4
};
ASSERT_SIZE(Handle, 0x8);
} // namespace ptcl
} // namespace sead
