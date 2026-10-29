#include "sead/ptcl/seadPtclEditorCamera.h"

namespace sead {
namespace ptcl {
// 0x00557FD8 | mk7dlp:bytes [tier B]
void sead::ptcl::PtclEditorCamera::reset()
{
    field_0x80 = 0;
    mDefaultDistance = 22.0f;
    mDistance = 22.0f;
    mHeight = 10.0f;
    field_0x64 = 0.0f;
    mRotY = 0xF1C71B80;
    mRotX = 0;
    mFovy = 90.0f;
    mTarget = Vector3<f32>::zero;
    mIsChanged = false;
    field_0x88 = 10.0f;
    field_0x8C = 1.0f;
}
} // namespace ptcl
} // namespace sead
