#include "sead/ptcl/seadPtclEditor.h"
#include "sead/ptcl/seadEmitterSet.h"
#include "sead/ptcl/seadPtclEditorCamera.h"
#include "sead/ptcl/seadPtclLoadCheck.h"
#include "sead/ptcl/seadPtclPreview.h"
#include "sead/ptcl/seadPtclProject.h"
#include "sead/ptcl/seadPtclProjectEmitterSet.h"
#include "sead/ptcl/seadPtclResource.h"
#include "sead/ptcl/seadPtclSystem.h"
#include "sead/seadNew.h"

#include <string.h>

namespace sead {
namespace ptcl {
inline PtclEditorSettingRange::PtclEditorSettingRange() : field_0x00(0)
{
    mRange0 = 100.0f;
    mRange2 = 1.0f;
    mRange1 = 10.0f;
    mRange3 = 10.0f;
    mFrameRange = 1000;
    mMetaRange0.format("Min=%5.1f,Max=%5.1f", -mRange0, mRange0);
    mMetaRange0Max.format("Min=0,Max=%5.1f", mRange0);
    mMetaRange2.format("Min=%5.1f,Max=%5.1f", -mRange2, mRange2);
    mMetaRange2Max.format("Min=0,Max=%5.1f", mRange2);
    mMetaRange2Tenth.format("Min=0,Max=%5.1f", mRange2 * 0.1f);
    mMetaRange1.format("Min=%5.1f,Max=%5.1f,Menu=True,MenuDefault=Auto", -mRange1, mRange1);
    mMetaRange1Max.format("Min=0,Max=%5.1f,Menu=True,MenuDefault=Auto", mRange1);
    mMetaRange3.format("Min=%5.1f,Max=%5.1f,Menu=True,MenuDefault=Auto", -mRange3, mRange3);
    mMetaRange3Max.format("Min=0,Max=%5.1f,Menu=True,MenuDefault=Auto", mRange3);
    mMetaFrame.format("Min=0,Max=%d", mFrameRange);
    mMetaFrameFrom1.format("Min=1,Max=%d", mFrameRange);
    mMetaRange0Off.format("%s,IsEnable=False", mMetaRange0.getCString());
    mMetaRange0MaxOff.format("%s,IsEnable=False", mMetaRange0Max.getCString());
    mMetaRange2MaxOff.format("%s,IsEnable=False", mMetaRange2Max.getCString());
    mMetaRange1Off.format("%s,IsEnable=False", mMetaRange1.getCString());
    mMetaRange3Off.format("%s,IsEnable=False", mMetaRange3.getCString());
    mMetaRange3MaxOff.format("%s,IsEnable=False", mMetaRange3Max.getCString());
    mMetaFrameOff.format("%s,IsEnable=False", mMetaFrame.getCString());
    mMetaRange1MaxOff.format("%s,IsEnable=False", mMetaRange1Max.getCString());
    mMetaFrameFrom1Off.format("%s,IsEnable=False", mMetaFrameFrom1.getCString());
    mMetaRange2MaxMenu.format("%s,Menu=True,MenuDefault=Auto", mMetaRange2Max.getCString());
    mMetaLongFrame.format("Min=0,Max=%d", mFrameRange * 10);
    mMetaLongFrameFrom1.format("Min=1,Max=%d", mFrameRange * 10);
    mMetaLongFrameOff.format("%s,IsEnable=False", mMetaLongFrame.getCString());
    mMetaLongFrameFrom1Off.format("%s,IsEnable=False", mMetaLongFrameFrom1.getCString());
}

inline PtclEditorSettingLight::PtclEditorSettingLight()
{
    mDir.x = -0.5f;
    mDir.y = -1.0f;
    mDir.z = -0.5f;
    Vector3CalcCtr<f32>::normalize(toVEC3(mDir));
    mColor0.r = 1.0f;
    mColor0.g = 1.0f;
    mColor0.b = 1.0f;
    mColor0.a = 1.0f;
    mColor1.r = 0.2f;
    mColor1.g = 0.2f;
    mColor1.b = 0.2f;
    mColor1.a = 1.0f;
    mColor2.r = 1.0f;
    mColor2.g = 1.0f;
    mColor2.b = 1.0f;
    mColor2.a = 1.0f;
    mColor3.r = 1.0f;
    mColor3.g = 1.0f;
    mColor3.b = 1.0f;
    mColor3.a = 1.0f;
    mUp.x = 0.0f;
    mUp.y = 1.0f;
    mUp.z = 0.0f;
    mColor4.r = 0.9f;
    mColor4.g = 1.0f;
    mColor4.b = 1.0f;
    mColor4.a = 1.0f;
    mColor5.r = 0.1f;
    mColor5.g = 0.0f;
    mColor5.b = 0.0f;
    mColor5.a = 1.0f;
    mValue0 = 0.5f;
    mColor6.r = 1.0f;
    mColor6.g = 1.0f;
    mColor6.b = 1.0f;
    mColor6.a = 1.0f;
    mValue1 = 0.0f;
    mValue2 = 200.0f;
    mValue3 = 2.0f;
    mType = 0;
}

// 0x0055AA08 (name is ours)
sead::ptcl::PtclEditorSetting::PtclEditorSetting(PtclEditor* editor) : mRange(), mLight()
{
    mEditor = editor;
    mMtx = Matrix34<f32>::ident;
    mFrameRate = 1.0f;
    for (s32 i = 0; i < 16; i++) {
        mIndexNames[i].format("[%d]", i);
    }
    // "数値１", "数値２" (value 1, value 2)
    mValueNames[0].copy(SafeStringBase<char>("\xE6\x95\xB0\xE5\x80\xA4\xEF\xBC\x91"));
    mValueNames[1].copy(SafeStringBase<char>("\xE6\x95\xB0\xE5\x80\xA4\xEF\xBC\x92"));
}

// 0x0054E778 | mk7dlp:bytes [tier B]
void sead::ptcl::PtclEditor::stopPreview(bool kill)
{
    if (kill) {
        mSystem->killEmitterGroup(255);
        field_0x10F7C = 0;
        field_0x10F78 = 0;
        return;
    }
    for (s32 i = 0; i < NUM_EMITTER_SET; i++) {
        for (s32 j = 0; j < NUM_HANDLE; j++) {
            Handle& handle = mHandles[i][j];
            if (handle.isValid() && handle.mEmitterSet->mNumCreated > 0) {
                handle.mEmitterSet->mIsFadeRequest = true;
            }
        }
    }
}

// 0x0054E80C | mk7dlp:bytes [tier B]
void sead::ptcl::PtclEditor::clearResource(PtclResource* resource)
{
    for (s32 i = 0; i < PtclProject::NUM_EMITTER_SET_MAX; i++) {
        if (mProject->mEmitterSets[i] != nullptr) {
            mProject->mEmitterSets[i]->unbindResource(resource);
        }
    }
}

// 0x0054E844 (name is ours)
void sead::ptcl::PtclEditor::bindResource(PtclResource* resource)
{
    for (s32 i = 0; i < PtclProject::NUM_EMITTER_SET_MAX; i++) {
        PtclProjectEmitterSet* set = mProject->mEmitterSets[i];
        if (set == nullptr || set->mBind[1].setData != nullptr) {
            continue;
        }
        for (s32 j = 0; j < resource->mHeader->numEmitterSet; j++) {
            const EmitterSetTbl& tbl = resource->mEmitterSetTbl[j];
            // an emitter set of the same name that is not bound yet
            if (SafeStringBase<char>(tbl.name).isEqual(set->mName) && tbl.orgEmitterTbl == tbl.emitterTbl) {
                set->bindTarget(1, resource, j);
                set->setOrder(set->mOrder);
                break;
            }
        }
    }
}

// 0x0054E9B8 (name is ours)
void sead::ptcl::PtclEditor::startPreview(bool kill)
{
    stopPreview(kill);
    for (s32 i = NUM_EMITTER_SET - 1; i >= 0; i--) {
        PtclProjectEmitterSet* set = mProject->mEmitterSets[i];
        if (set == nullptr) {
            continue;
        }
        if (mSystem->mResources[mResId]->mEmitterSetTbl[set->mBind[0].emitterSetId].numEmitter <= 0 ||
            !set->mIsVisible) {
            continue;
        }

        // the visible emitters
        u32 mask = 0;
        s32 numEmitter = 0;
        for (u32 j = 0; j < 8; j++) {
            if (set->mEmitters[j] != nullptr) {
                if (set->mEmitters[j]->mIsVisible) {
                    mask |= 1 << numEmitter;
                }
                numEmitter++;
            }
        }

        // a free handle (or the first one again)
        s32 index = -1;
        for (s32 j = 0; j < NUM_HANDLE; j++) {
            Handle& handle = mHandles[i][j];
            if (!handle.isValid() || handle.mEmitterSet->mNumCreated <= 0) {
                index = j;
                break;
            }
        }
        if (index < 0) {
            mHandles[i][0].mEmitterSet->kill();
            index = 0;
        }
        mHandleIndex[i] = index;

        Handle* handle = &mHandles[i][index];
        mSystem->mViewMtx.m[0][3] = Vector3<f32>::zero.x;
        mSystem->mViewMtx.m[1][3] = Vector3<f32>::zero.y;
        mSystem->mViewMtx.m[2][3] = Vector3<f32>::zero.z;
        mSystem->createEmitterSetID(handle, mSystem->mViewMtx, set->mBind[0].emitterSetId, mResId, 255, mask);
        mPreview->applyToEmitterSet(set, handle);
    }
    mPreview->start();
}

// 0x0054EB44 slot 0x0C (name is ours)
void sead::ptcl::PtclEditor::onRemoveEmitterSet(PtclProjectEmitterSet* set)
{
    mProject->removeEmitterSet(set);
    mPreviewFrame = 0;
    stopPreview(true);
    if (!mIsPause && mIsAutoStart) {
        startPreview(true);
    }
}

// 0x0054EBB0 (name is ours)
sead::ptcl::PtclEditor::PtclEditor(Heap* heap, PtclSystem* system, s32 resId)
    : hostio::Node(), mSetting(this), mEmptyName("")
{
    mHeap = heap;
    mSystem = system;
    field_0xF0C = -1;
    field_0xF10 = -1;
    mPreviewFrame = 0;
    mPreviewInterval = 180;
    mIsPause = false;
    mIsAutoStart = false;
    field_0x10F60 = 0;
    field_0x10F64 = 0;
    field_0x10F5E = false;
    field_0x10F68 = 0;
    field_0x10F6C = false;
    memset(mHandleIndex, 0, sizeof(mHandleIndex));

    // (the project gets mResId before it is set, like in the original)
    mProject = new (mHeap, 4) PtclProject(mHeap, mSystem, this, mResId);
    mProject->initName();
    void* data = PtclResource::createEmtpryData(heap, PtclProject::NUM_EMITTER_SET_MAX);
    field_0x14 = 0;
    mResId = resId;
    BufferLocation location = {0};
    mSystem->entryResource(heap, data, mResId, location);
    mLoadCheck = new (mHeap, 4) PtclLoadCheck(mProject, mSystem, mResId, 255);
    memset(field_0x10F98, 0, sizeof(field_0x10F98));
    field_0x18 = new (mHeap, 4) u8;
    mPreview = new (mHeap, 4) PtclPreview(mHeap, &mSetting, mProject);
    PtclEditorCamera* camera = new (mHeap, 4) PtclEditorCamera(&mSetting.mRange.mRange0);
    camera->reset();
    mCamera = camera;
    mSetting.mCamera = camera;

    mBuffer0 = new (heap, 128) u8[0x80000];
    mBuffer1 = new (heap, 128) u8[0x80000];
    mBuffer2 = new (heap, 128) u8[0x28000];
    mBuffer3 = new (heap, 128) u8[0x28000];
    mIndexBuffer = new (heap, 128) u8[0xC000];
    field_0x10F78 = 0;
    field_0x10F7C = 0;
    field_0x10F8C = 0;
    PtclSystem::initIBO(mIndexBuffer, 4096);
    field_0x10F6D = false;
}

// 0x0054EF04 slot 0x04
// 0x0054EEF4 (deleting dtor)
sead::ptcl::PtclEditor::~PtclEditor()
{
    if (mProject != nullptr) {
        delete mProject;
    }
    delete field_0x18;
}
} // namespace ptcl
} // namespace sead
