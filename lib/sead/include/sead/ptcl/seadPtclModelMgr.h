#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_Skeleton.h"
#include "sead/hostio/seadNode.h"
#include "sead/seadAllocatorNw4c.h"
#include "sead/seadDirectResourceFactory.h"
#include "sead/seadFixedSafeString.h"
#include "sead/seadGraphicsFileResNw4c.h"
#include "sead/seadMatrix.h"
#include "sead/seadVector3.h"

namespace sead {
class Heap;

namespace ptcl {
class PtclPreview;
class PtclProject;
struct PtclEditorSetting;

// a model of the preview (only its matrix is known; name is ours)
struct PtclPreviewModel
{
    u8 _unk_0x0[0x4C];
    Matrix34<f32> mMtx; // 0x4C
};

// The model file of a model of the preview and what is built from it (name and member names are ours).
struct PtclModelData
{
    PtclModelData(Heap* heap, PtclEditorSetting* setting); // 0x0055CFD4
    DECOMP_NOINLINE ~PtclModelData();

    // the matrix of bone index of the model (the model itself without a skeleton; inline, the copy is called)
    DECOMP_NOINLINE Matrix34<f32> getMtx(s32 index) const // 0x0074ECE4
    {
        const nn::math::MTX34* mtx;
        if (mSkeleton != nullptr) {
            mtx = &(*mSkeleton->GetWorldMatrices())[index];
        } else if (mModel == nullptr) {
            mtx = &Matrix34<f32>::ident;
        } else {
            mtx = &mModel->mMtx;
        }
        return static_cast<const Matrix34<f32>&>(*mtx);
    }

    Heap* mHeap;                                         // 0x000
    AllocatorNw4c mAllocator;                            // 0x004
    AllocatorNw4c mDeviceAllocator;                      // 0x00C
    DirectResourceFactory<GraphicsFileResNw4c> mFactory; // 0x014
    PtclEditorSetting* mSetting;                         // 0x060
    u32 field_0x64;
    u32 field_0x68[6];
    nw::gfx::Skeleton* mSkeleton;                        // 0x080
    u32 field_0x84;
    void* field_0x88;                                    // 0x088
    PtclPreviewModel* mModel;                            // 0x08C
    u8 _unk_0x090[0xC];
    bool field_0x9C;                                     // 0x09C
    u8 _unk_0x09D[0x183];
    Matrix34<f32> mMtx;                                  // 0x220, of the model
    u32 field_0x250;
    u32 field_0x254;
    u32 field_0x258;
    u8 _unk_0x25C[0x30];
};
ASSERT_OFFSET(PtclModelData, mFactory, 0x14);
ASSERT_OFFSET(PtclModelData, mSetting, 0x60);
ASSERT_OFFSET(PtclModelData, mSkeleton, 0x80);
ASSERT_OFFSET(PtclModelData, mMtx, 0x220);
ASSERT_SIZE(PtclModelData, 0x28C);

// RTTI N4sead4ptcl12PtclModelMgrE @ 0x008D2054
// vtable 0x00906AF8 (vptr 0x00906B00), offset_to_top 0, 3 entries
// A model of the preview of the editor: the paths of its files and their file names (the class, start and
// isValidModel are from the reference symbols, the rest is ours).
class PtclModelMgr : public ::sead::hostio::Node
{
public:
    // an animation of the model
    struct Anim
    {
        u32 field_0x0;
        s32 index; // -1: none
    };

    PtclModelMgr(Heap* heap, PtclEditorSetting* setting, s32 index, PtclPreview* preview,
                 PtclProject* project); // 0x00555CF0
    virtual ~PtclModelMgr(); // 0x005561BC slot 0x04
    // 0x005561AC slot 0x08 (deleting dtor)

    // the animation starts again
    void start(); // 0x00555CDC | mk7dlp
    DECOMP_EMIT_COPY bool isValidModel() const { return mData->field_0x88 != nullptr || mData->mModel != nullptr; } // 0x0074ECC8 | mk7dlp
    // the settings of no model
    void reset_(); // 0x00555B7C
    // the file names of the paths
    void updateFileNames_(); // 0x0055A628

    Heap* mHeap;                                // 0x0000
    PtclProject* mProject;                      // 0x0008
    PtclModelData* mData;                       // 0x000C
    u8 _unk_0x0010[0x30];
    FixedSafeString<256> mName;                 // 0x0040
    FixedSafeString<128> mMetaName;             // 0x014C
    FixedSafeString<256> mModelPath;            // 0x01D8
    FixedSafeString<256> mPaths4[4];      // 0x02E4
    FixedSafeString<256> mPathsB[2]; // 0x0714
    FixedSafeString<256> mPathsC[2]; // 0x092C
    FixedSafeString<256> mPathsD[2]; // 0x0B44
    FixedSafeString<256> mPathsE[2];   // 0x0D5C
    FixedSafeString<256> mPathsF[2];    // 0x0F74
    FixedSafeString<256> mPathG;              // 0x118C
    FixedSafeString<256> mModelName;            // 0x1298, the file names of the paths
    FixedSafeString<256> mNames4[4];      // 0x13A4
    FixedSafeString<256> mNamesB[2]; // 0x17D4
    FixedSafeString<256> mNamesC[2]; // 0x19EC
    FixedSafeString<256> mNamesD[2]; // 0x1C04
    FixedSafeString<256> mNamesE[2];   // 0x1E1C
    FixedSafeString<256> mNamesF[2];    // 0x2034
    FixedSafeString<256> mNameG;              // 0x224C
    FixedSafeString<1024> mInfo;                // 0x2358
    FixedSafeString<512> mMetaInfo;             // 0x2764
    Anim mAnims[6];                             // 0x2970
    s32 mIndex;                                 // 0x29A0
    PtclPreview* mPreview;                      // 0x29A4
    f32 mFrame;                                 // 0x29A8
    PtclEditorSetting* mSetting;                // 0x29AC
    bool mIsVisible;                            // 0x29B0
    f32 mFrameRate;                             // 0x29B4
    s32 mBoneIndex;                             // 0x29B8, -1: none
    s32 field_0x29BC;
    Vector3<f32> mTranslate;                    // 0x29C0
    Vector3<u32> mRotate;                       // 0x29CC
    Vector3<f32> mScale;                        // 0x29D8
    s32 field_0x29E4;
};
ASSERT_OFFSET(PtclModelMgr, mName, 0x40);
ASSERT_OFFSET(PtclModelMgr, mModelPath, 0x1D8);
ASSERT_OFFSET(PtclModelMgr, mPathG, 0x118C);
ASSERT_OFFSET(PtclModelMgr, mNameG, 0x224C);
ASSERT_OFFSET(PtclModelMgr, mAnims, 0x2970);
ASSERT_OFFSET(PtclModelMgr, mSetting, 0x29AC);
ASSERT_SIZE(PtclModelMgr, 0x29E8);
} // namespace ptcl
} // namespace sead
