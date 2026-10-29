#include "sead/ptcl/seadPtclGameProgrammer.h"
#include "sead/ptcl/seadEmitterSet.h"
#include "sead/seadMathCalcCommon.h"

namespace sead {
namespace ptcl {
// 0x0055A3A8 | mk7dlp
void sead::ptcl::PtclGameProgrammer::updateHandle(Handle* handle, const Matrix34<f32>& mtx, const Matrix34<f32>& view)
{
    EmitterSet* set = handle->mEmitterSet;
    set->mColor = mColor;
    set->mPtclScale.x = mPtclScale.x;
    set->mPtclScale.y = mPtclScale.y;
    set->mPtclScaleXY.x = set->mPtclScale.x * set->mScale.x;
    set->mPtclScaleXY.y = set->mPtclScale.y * set->mScale.y;

    Matrix34<f32> srt;
    if (mIsBillboard != 0) {
        // the rotation of the view undone, without its translation
        Matrix34<f32> inv = view;
        f32 tmp = inv.m[0][1];
        inv.m[0][1] = inv.m[1][0];
        inv.m[1][0] = tmp;
        tmp = inv.m[0][2];
        inv.m[0][2] = inv.m[2][0];
        inv.m[2][0] = tmp;
        tmp = inv.m[1][2];
        inv.m[1][2] = inv.m[2][1];
        inv.m[2][1] = tmp;
        inv.m[2][3] = 0.0f;
        inv.m[1][3] = 0.0f;
        inv.m[0][3] = 0.0f;
        Matrix34CalcCtr<f32>::makeSRTIdx(srt, toVEC3(mScale), Vector3<u32>::zero, toVEC3(mTranslate));
        Matrix34CalcCtr<f32>::multiply(srt, srt, inv);
        Matrix34CalcCtr<f32>::multiply(srt, mtx, srt);
    } else {
        Matrix34CalcCtr<f32>::makeSRTIdx(srt, toVEC3(mScale), mRotate, toVEC3(mTranslate));
        Matrix34CalcCtr<f32>::multiply(srt, mtx, srt);
    }

    if (mIsNoScale == 1) {
        set->mRT = srt;
        set->mSRT = set->mRT;
        set->mScale = Vector3<f32>::ones;
        set->mPtclScaleXY.x = set->mPtclScale.x * set->mScale.x;
        set->mPtclScaleXY.y = set->mPtclScale.y * set->mScale.y;
    } else {
        set->setMtx(srt);
    }
    set->mFigureVelocityScale = mFigureVelocityScale;
    set->mDirectionalVelocityScale = mDirectionalVelocityScale;
    set->mRandomVelocityScale = mRandomVelocityScale;
    set->mAddVelocity = mAddVelocity;
    set->mStartFrame = mStartFrame;
    set->mEmitterVolumeScale = mEmitterVolumeScale;
    set->mPtclEmitScale.x = mPtclEmitScale.x;
    set->mPtclEmitScale.y = mPtclEmitScale.y;
    if (mIsSetDirectional) {
        set->mDirectional = mDirectional;
    }
    set->mIsSetDirectional = mIsSetDirectional;
    for (u32 i = 0; i < 8; i++) {
        set->mRecord[i].emitRate = static_cast<s32>(mRecords[i].emitRate * 256.0f);
        set->mRecord[i].emitInterval = static_cast<s32>(mRecords[i].emitInterval * 256.0f);
        set->mRecord[i].life = static_cast<s32>(mRecords[i].life * 256.0f);
    }
}
} // namespace ptcl
} // namespace sead
