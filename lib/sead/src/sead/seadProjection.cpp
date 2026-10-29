#include "sead/seadProjection.h"

namespace sead {
// 0x0053F248 slot 0x08
// 0x0053F244 (deleting dtor)
sead::Projection::~Projection()
{
    // empty (in the original too)
}

// 0x0054E1C4 (name is ours)
void sead::Projection::swapRows01_(Matrix44<f32>* mtx)
{
    f32 row0[4] = {mtx->m[0][0], mtx->m[0][1], mtx->m[0][2], mtx->m[0][3]};
    f32 row1[4] = {mtx->m[1][0], mtx->m[1][1], mtx->m[1][2], mtx->m[1][3]};
    for (s32 i = 0; i < 4; i++) {
        mtx->m[1][i] = row0[i];
    }
    for (s32 i = 0; i < 4; i++) {
        mtx->m[0][i] = row1[i];
    }
}

namespace {
inline void negateRow(Matrix44<f32>* mtx, s32 row)
{
    mtx->m[row][0] = -mtx->m[row][0];
    mtx->m[row][1] = -mtx->m[row][1];
    mtx->m[row][2] = -mtx->m[row][2];
    mtx->m[row][3] = -mtx->m[row][3];
}
} // namespace

// 0x0074A244 slot 0x18 (name is ours)
void sead::Projection::doUpdateDeviceMatrix(Matrix44<f32>* dst, const Matrix44<f32>& src,
                                            Graphics::DevicePosture posture) const
{
    Matrix44CalcCtr<f32>::copy(*dst, src);
    switch (posture) {
    case Graphics::DEVICE_POSTURE_SAME:
        break;
    case Graphics::DEVICE_POSTURE_ROTATE_90:
        negateRow(dst, 0);
        swapRows01_(dst);
        break;
    case Graphics::DEVICE_POSTURE_ROTATE_270:
        negateRow(dst, 1);
        swapRows01_(dst);
        break;
    case Graphics::DEVICE_POSTURE_ROTATE_180:
        negateRow(dst, 0);
        negateRow(dst, 1);
        break;
    case Graphics::DEVICE_POSTURE_FLIP_X:
        negateRow(dst, 0);
        break;
    case Graphics::DEVICE_POSTURE_FLIP_Y:
        negateRow(dst, 1);
        break;
    default:
        break;
    }
    dst->m[2][0] *= mDeviceZScale;
    dst->m[2][1] *= mDeviceZScale;
    dst->m[2][2] = (dst->m[2][2] + dst->m[3][2] * mDeviceZOffset) * mDeviceZScale;
    dst->m[2][3] = dst->m[2][3] * mDeviceZScale + dst->m[3][3] * mDeviceZOffset;
}

// 0x0074A428 (name is ours)
const sead::Matrix44<f32>& sead::Projection::getDeviceProjectionMatrix() const
{
    if (mDirty) {
        doUpdateMatrix(&mMatrix);
        mDirty = false;
        mDeviceDirty = true;
    }
    if (mDeviceDirty) {
        doUpdateDeviceMatrix(&mDeviceMatrix, mMatrix, mDevicePosture);
        mDeviceDirty = false;
    }
    return mDeviceMatrix;
}
} // namespace sead
