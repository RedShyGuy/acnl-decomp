#include "sead/ptcl/seadPtclModelMgr.h"
#include "sead/seadNew.h"

#include <string.h>

namespace sead {
namespace ptcl {
namespace {
// the file name of path into dst (the part after the last backslash; name is ours)
// 0x0055A774 (name is ours)
DECOMP_NOINLINE void extractFileName_(BufferedSafeStringBase<char>* dst, const SafeStringBase<char>& path)
{
    s32 index = path.rfindIndex(SafeStringBase<char>("\\"));
    if (index < 0) {
        dst->copy(path, -1);
        return;
    }
    s32 length = path.getLength();
    BufferedSafeStringBase<char> name(const_cast<char*>(path.getCString()) + index + 1, length - index);
    dst->copy(name, -1);
}
} // namespace

// 0x0055CFD4 (name is ours)
sead::ptcl::PtclModelData::PtclModelData(Heap* heap, PtclEditorSetting* setting)
    : mAllocator(nullptr), mDeviceAllocator(nullptr), mFactory()
{
    mHeap = heap;
    mSetting = setting;
    field_0x64 = 0;
    mSkeleton = nullptr;
    for (s32 i = 0; i < 6; i++) {
        field_0x68[i] = 0;
    }
    field_0x84 = 0;
    field_0x88 = nullptr;
    mModel = nullptr;
    field_0x9C = true;
    field_0x254 = 0;
    field_0x258 = 0;
    field_0x250 = 0;
}

// 0x0055D0D4 (name is ours)
sead::ptcl::PtclModelData::~PtclModelData()
{
    // (only the members are destroyed)
}

// 0x00555B7C (name is ours)
void sead::ptcl::PtclModelMgr::reset_()
{
    mBoneIndex = -1;
    field_0x29BC = 0;
    mIsVisible = true;
    mTranslate = Vector3<f32>::zero;
    mRotate = Vector3<u32>::zero;
    mScale = Vector3<f32>::ones;
    memset(mAnims, 0, sizeof(mAnims));
    for (s32 i = 0; i < 6; i++) {
        mAnims[i].index = -1;
    }
    // "DispName = モデル未設定 , Icon = MODEL" (no model set)
    mMetaName.copy(SafeStringBase<char>("DispName = \xE3\x83\xA2\xE3\x83\x87\xE3\x83\xAB\xE6\x9C\xAA\xE8\xA8\xAD\xE5\xAE\x9A , "
                              "Icon = MODEL"),
                   -1);
    field_0x29E4 = 0;
    mFrameRate = 1.0f;
}

// 0x00555CDC | mk7dlp
void sead::ptcl::PtclModelMgr::start()
{
    mFrame = 0.0f;
}

// 0x00555CF0 (name is ours)
sead::ptcl::PtclModelMgr::PtclModelMgr(Heap* heap, PtclEditorSetting* setting, s32 index, PtclPreview* preview,
                                       PtclProject* project)
    : hostio::Node()
{
    mHeap = heap;
    mData = new (heap, 4) PtclModelData(heap, setting);
    mIndex = index;
    mPreview = preview;
    mProject = project;
    mFrame = 0.0f;
    mSetting = setting;
    mIsVisible = true;
    reset_();
    mModelPath.copy(SafeStringBase<char>(""), -1);
    for (s32 i = 0; i < 4; i++) {
        mPaths4[i].copy(SafeStringBase<char>(""), -1);
    }
    for (s32 i = 0; i < 2; i++) {
        mPathsB[i].copy(SafeStringBase<char>(""), -1);
    }
    for (s32 i = 0; i < 2; i++) {
        mPathsC[i].copy(SafeStringBase<char>(""), -1);
    }
    for (s32 i = 0; i < 2; i++) {
        mPathsD[i].copy(SafeStringBase<char>(""), -1);
    }
    for (s32 i = 0; i < 2; i++) {
        mPathsE[i].copy(SafeStringBase<char>(""), -1);
    }
    for (s32 i = 0; i < 2; i++) {
        mPathsF[i].copy(SafeStringBase<char>(""), -1);
    }
    mPathG.copy(SafeStringBase<char>(""), -1);
    updateFileNames_();
    mInfo.copy(SafeStringBase<char>(""), -1);
    mMetaInfo.copy(SafeStringBase<char>(""), -1);
}

// 0x005561BC slot 0x04
// 0x005561AC (deleting dtor)
sead::ptcl::PtclModelMgr::~PtclModelMgr()
{
    if (mData != nullptr) {
        delete mData;
    }
}

// 0x0055A628 (name is ours)
void sead::ptcl::PtclModelMgr::updateFileNames_()
{
    extractFileName_(&mModelName, mModelPath);
    for (s32 i = 0; i < 4; i++) {
        extractFileName_(&mNames4[i], mPaths4[i]);
    }
    for (s32 i = 0; i < 2; i++) {
        extractFileName_(&mNamesB[i], mPathsB[i]);
    }
    for (s32 i = 0; i < 2; i++) {
        extractFileName_(&mNamesC[i], mPathsC[i]);
    }
    for (s32 i = 0; i < 2; i++) {
        extractFileName_(&mNamesD[i], mPathsD[i]);
    }
    for (s32 i = 0; i < 2; i++) {
        extractFileName_(&mNamesE[i], mPathsE[i]);
    }
    for (s32 i = 0; i < 2; i++) {
        extractFileName_(&mNamesF[i], mPathsF[i]);
    }
    extractFileName_(&mNameG, mPathG);
}
} // namespace ptcl
} // namespace sead
