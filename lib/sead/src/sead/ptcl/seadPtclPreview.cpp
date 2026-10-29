#include "sead/ptcl/seadPtclPreview.h"
#include "sead/ptcl/seadEmitterSet.h"
#include "sead/ptcl/seadPtclEditor.h"
#include "sead/ptcl/seadPtclGameProgrammer.h"
#include "sead/ptcl/seadPtclModelMgr.h"
#include "sead/ptcl/seadPtclProjectEmitterSet.h"
#include "sead/seadNew.h"

namespace sead {
namespace ptcl {
// 0x005552E0 | mk7dlp:bytes [tier B]
void sead::ptcl::PtclPreview::resetAutoMove_(bool resetFrame)
{
    if (resetFrame) {
        mAutoMoveFrame = 0;
    }
    mAutoMoveRadius = mSetting->mRange.mRange0 * 0.08f;
    mAutoMoveRotSpeed = 0xB60B60;
    mAutoMoveRot = 0;
    mMtx = Matrix34<f32>::ident;
    mAutoMoveRotZSpeed = 0x38E38E0;
    field_0x208 = 0;
    field_0x204 = 0.0f;
    mAutoMoveTranslate = Vector3<f32>::zero;
    field_0x20C = 0;
    field_0x21C = 0;
    mBgColor = Color4f::white;
}

// 0x00555390 | mk7dlp:bytes-fuzzy [tier B]
void sead::ptcl::PtclPreview::applyToEmitterSet(PtclProjectEmitterSet* set, Handle* handle)
{
    if (set == nullptr) {
        return;
    }
    PtclGameProgrammer* programmer = set->mGameProgrammer;
    if (programmer == nullptr) {
        return;
    }

    Matrix34<f32> mtx;
    s32 modelIndex = programmer->mModelIndex;
    if (modelIndex < 0) {
        mtx = mMtx;
    } else {
        if (programmer->mBoneIndex >= 0) {
            mtx = mModels[modelIndex]->mData->getMtx(programmer->mBoneIndex);
        } else {
            mtx = mModels[modelIndex]->mData->mMtx;
        }
        if (set->mGameProgrammer->mFollowType == 1) {
            // the position only
            Matrix34<f32> pos;
            pos.m[0][0] = 1.0f;
            pos.m[0][1] = 0.0f;
            pos.m[0][2] = 0.0f;
            pos.m[0][3] = mtx.m[0][3];
            pos.m[1][0] = 0.0f;
            pos.m[1][1] = 1.0f;
            pos.m[1][2] = 0.0f;
            pos.m[1][3] = mtx.m[1][3];
            pos.m[2][0] = 0.0f;
            pos.m[2][1] = 0.0f;
            pos.m[2][2] = 1.0f;
            pos.m[2][3] = mtx.m[2][3];
            mtx = pos;
        }
        if (set->mGameProgrammer->mFollowType == 2) {
            // the rotation of the model at the position
            const Matrix34<f32>& model = mModels[modelIndex]->mData->mMtx;
            Matrix34<f32> pos;
            pos.m[0][0] = model.m[0][0];
            pos.m[0][1] = model.m[0][1];
            pos.m[0][2] = model.m[0][2];
            pos.m[0][3] = mtx.m[0][3];
            pos.m[1][0] = model.m[1][0];
            pos.m[1][1] = model.m[1][1];
            pos.m[1][2] = model.m[1][2];
            pos.m[1][3] = mtx.m[1][3];
            pos.m[2][0] = model.m[2][0];
            pos.m[2][1] = model.m[2][1];
            pos.m[2][2] = model.m[2][2];
            pos.m[2][3] = mtx.m[2][3];
            mtx = pos;
        }
    }
    programmer->updateHandle(handle, mtx, mViewMtx);
}

// 0x00555518 | mk7dlp:bytes [tier B]
void sead::ptcl::PtclPreview::start()
{
    for (s32 i = 0; i < NUM_MODEL; i++) {
        if (mModels[i]->isValidModel()) {
            mModels[i]->start();
        }
    }
}

// 0x0055554C (name is ours)
sead::ptcl::PtclPreview::PtclPreview(Heap* heap, PtclEditorSetting* setting, PtclProject* project)
    : hostio::Node(), mName(), mBgColor(), mBgTexture()
{
    mSetting = setting;
    mNumValidModel = 0;
    mHeap = heap;
    mProject = project;
    for (s32 i = 0; i < NUM_MODEL; i++) {
        mModels[i] = new (mHeap, 4) PtclModelMgr(mHeap, setting, i, this, mProject);
    }
    mNumValidModel = 0;
    for (s32 i = 0; i < NUM_MODEL; i++) {
        if (mModels[i]->isValidModel()) {
            mValidModels[mNumValidModel++] = mModels[i];
        }
    }
    resetAutoMove_(true);
    mInterval = 180;
    field_0x224 = false;
}

// 0x005556EC slot 0x04
// 0x005556CC (deleting dtor)
sead::ptcl::PtclPreview::~PtclPreview()
{
    // (only the members are destroyed)
}
} // namespace ptcl
} // namespace sead
