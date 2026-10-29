#pragma once

#include "decomp.h"
#include "sead/seadColor4f.h"
#include "sead/seadMatrix.h"
#include "sead/seadVector2.h"
#include "sead/seadVector3.h"

namespace sead {
namespace ptcl {
struct Handle;

// What the game sets for an emitter set, as the editor shows it (the class and updateHandle are from the
// reference symbols, the member names are ours).
class PtclGameProgrammer
{
public:
    // the factors of an emitter (8.8 fixed point in EmitterSetRecord)
    struct Record
    {
        f32 emitRate;
        f32 emitInterval;
        f32 life;
        u8 _unk_0x0C[0xC];
    };

    // the settings into the emitter set of handle; the matrix of the set is mtx times the transformation of
    // the settings (billboard: the rotation of view inverted)
    void updateHandle(Handle* handle, const Matrix34<f32>& mtx, const Matrix34<f32>& view); // 0x0055A3A8

    u8 _unk_0x00[0xC];
    Color4f mColor;                   // 0x0C
    Vector2<f32> mPtclScale;          // 0x1C
    Vector2<f32> mPtclEmitScale;      // 0x24
    Vector3<f32> mTranslate;          // 0x2C
    Vector3<u32> mRotate;             // 0x38, angle indices
    Vector3<f32> mScale;              // 0x44
    Vector3<f32> mEmitterVolumeScale; // 0x50
    u16 mIsNoScale;                   // 0x5C, 1: the matrix is used as it is
    u16 mIsBillboard;                 // 0x5E
    f32 mFigureVelocityScale;         // 0x60
    f32 mDirectionalVelocityScale;    // 0x64
    f32 mRandomVelocityScale;         // 0x68
    Vector3<f32> mAddVelocity;        // 0x6C
    Vector3<f32> mDirectional;        // 0x78
    bool mIsSetDirectional;           // 0x84
    s32 mModelIndex;                  // 0x88, the model of the preview, -1: none
    s32 mBoneIndex;                   // 0x8C, -1: the model itself
    s32 mFollowType;                  // 0x90, 0: all, 1: the position, 2: the position and the rotation
    s32 mStartFrame;                  // 0x94
    Record mRecords[8];               // 0x98
};
ASSERT_OFFSET(PtclGameProgrammer, mColor, 0x0C);
ASSERT_OFFSET(PtclGameProgrammer, mIsNoScale, 0x5C);
ASSERT_OFFSET(PtclGameProgrammer, mModelIndex, 0x88);
ASSERT_OFFSET(PtclGameProgrammer, mRecords, 0x98);
} // namespace ptcl
} // namespace sead
