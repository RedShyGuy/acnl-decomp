#pragma once

#include "decomp.h"
#include "sead/seadLookAtCamera.h"
#include "sead/seadVector3.h"

namespace sead {
namespace ptcl {
// The camera of the preview of the editor: a look-at camera that orbits a point (the class is from the
// reference symbols, the member names are ours).
class PtclEditorCamera
{
public:
    explicit PtclEditorCamera(const f32* ranges) : mCamera(), mRanges(ranges) {}

    // the start values of the orbit
    void reset(); // 0x00557FD8 | mk7dlp:bytes [tier B]

    LookAtCamera mCamera;                // 0x00
    const f32* mRanges;                  // 0x58, PtclEditorSettingRange::mRange0..
    f32 mDistance;                       // 0x5C
    f32 mHeight;                         // 0x60
    f32 field_0x64;                      // 0x64
    Vector3<f32> mTarget;                // 0x68
    u32 mRotX;                           // 0x74, angle indices
    u32 mRotY;                           // 0x78
    f32 mDefaultDistance;                // 0x7C
    u32 field_0x80;                      // 0x80
    f32 mFovy;                           // 0x84, degrees
    f32 field_0x88;                      // 0x88
    f32 field_0x8C;                      // 0x8C
    u8 _unk_0x90[0x20];
    bool mIsChanged;                     // 0xB0
};
ASSERT_OFFSET(PtclEditorCamera, mRanges, 0x58);
ASSERT_OFFSET(PtclEditorCamera, mIsChanged, 0xB0);
ASSERT_SIZE(PtclEditorCamera, 0xB4);
} // namespace ptcl
} // namespace sead
