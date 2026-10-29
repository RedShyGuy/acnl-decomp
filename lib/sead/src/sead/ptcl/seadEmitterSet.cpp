#include "sead/ptcl/seadEmitterSet.h"
#include "sead/ptcl/seadEmitterCalc.h"
#include "sead/ptcl/seadEmitterData.h"
#include "sead/ptcl/seadEmitterInstance.h"
#include "sead/ptcl/seadPtclInstance.h"
#include "sead/ptcl/seadPtclSystem.h"

#include <math.h>

namespace sead {
namespace ptcl {
// 0x0054E44C | mk7dlp:bytes [tier B]
void sead::ptcl::EmitterSet::emitParticle(const Vector3<f32>& offset)
{
    for (s32 i = 0; i < mNumEmitter; i++) {
        EmitterInstance* emitter = mInstance[i];
        if (emitter->mCreateID != mCreateID) {
            continue;
        }
        PtclInstance* oldHead = emitter->mPtclHead;
        EmitterCalc::s_EmitFunctions[emitter->mRes->volumeType](emitter);
        for (PtclInstance* ptcl = emitter->mPtclHead; ptcl != oldHead; ptcl = ptcl->mNext) {
            ptcl->mPos.x += offset.x;
            ptcl->mPos.y += offset.y;
            ptcl->mPos.z += offset.z;
        }
    }
}

// 0x0054E4FC | mk7dlp:bytes [tier B]
void sead::ptcl::EmitterSet::kill()
{
    for (s32 i = 0; i < mNumEmitter; i++) {
        if (mInstance[i]->mCreateID == mCreateID) {
            mSystem->killEmitter(mInstance[i]);
        }
    }
}

// 0x0054E544 | mk7dlp:bytes [tier B]
void sead::ptcl::EmitterSet::setMtx(const Matrix34<f32>& mtx)
{
    mSRT = mtx;
    f32 scaleX = sqrtf(mtx.m[0][0] * mtx.m[0][0] + mtx.m[1][0] * mtx.m[1][0] + mtx.m[2][0] * mtx.m[2][0]);
    f32 scaleY = sqrtf(mtx.m[0][1] * mtx.m[0][1] + mtx.m[1][1] * mtx.m[1][1] + mtx.m[2][1] * mtx.m[2][1]);
    f32 scaleZ = sqrtf(mtx.m[0][2] * mtx.m[0][2] + mtx.m[1][2] * mtx.m[1][2] + mtx.m[2][2] * mtx.m[2][2]);
    mScale.x = scaleX;
    mScale.y = scaleY;
    mScale.z = scaleZ;
    if (scaleX > 0.0f) {
        f32 inv = 1.0f / scaleX;
        mRT.m[0][0] = mtx.m[0][0] * inv;
        mRT.m[1][0] = mtx.m[1][0] * inv;
        mRT.m[2][0] = mtx.m[2][0] * inv;
    } else {
        mRT.m[2][0] = 0.0f;
        mRT.m[1][0] = 0.0f;
        mRT.m[0][0] = 0.0f;
    }
    if (scaleY > 0.0f) {
        f32 inv = 1.0f / scaleY;
        mRT.m[0][1] = mtx.m[0][1] * inv;
        mRT.m[1][1] = mtx.m[1][1] * inv;
        mRT.m[2][1] = mtx.m[2][1] * inv;
    } else {
        mRT.m[2][1] = 0.0f;
        mRT.m[1][1] = 0.0f;
        mRT.m[0][1] = 0.0f;
    }
    if (scaleZ > 0.0f) {
        f32 inv = 1.0f / scaleZ;
        mRT.m[0][2] = mtx.m[0][2] * inv;
        mRT.m[1][2] = mtx.m[1][2] * inv;
        mRT.m[2][2] = mtx.m[2][2] * inv;
    } else {
        mRT.m[2][2] = 0.0f;
        mRT.m[1][2] = 0.0f;
        mRT.m[0][2] = 0.0f;
    }
    mPtclScaleXY.x = mPtclScale.x * scaleX;
    mPtclScaleXY.y = mPtclScale.y * scaleY;
}

// 0x0054E6A8 (name is ours)
s32 sead::ptcl::EmitterSet::preCalc(s32 num, u32* cmd)
{
    s32 maxSize = 0;
    for (s32 frame = 0; frame < num; frame++) {
        for (s32 i = 0; i < mNumEmitter; i++) {
            EmitterInstance* emitter = mInstance[i];
            if (emitter->mCreateID != mCreateID) {
                continue;
            }
            s32 numPtcl;
            u32* end = emitter->mCalc->calcCTR(cmd, emitter, &numPtcl);
            s32 size = reinterpret_cast<uptr>(end) - reinterpret_cast<uptr>(cmd);
            if (maxSize < size) {
                maxSize = size;
            }
        }
    }
    return maxSize;
}

// 0x0054E734 | mk7dlp:bytes [tier B]
sead::ptcl::EmitterSet::EmitterSet() : mNumCreated(0), mNumEmitter(0), mCreateID(0), mColor()
{
}
} // namespace ptcl
} // namespace sead
