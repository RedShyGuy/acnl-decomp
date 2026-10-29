#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadRoot.h"
#include "sead/seadGraphics.h"
#include "sead/seadMathCalcCommon.h"
#include "sead/seadMatrix.h"
#include "sead/seadVector2.h"
#include "sead/seadVector3.h"

namespace sead {
// RTTI N4sead10ProjectionE @ 0x008D13CC
// vtable 0x00904F20 (vptr 0x00904F28), offset_to_top 0, 8 entries
//
// A projection: the matrix of the derived class (recomputed when dirty) and the matrix for the device
// (turned for the device posture, depth scaled and moved). All names are ours.
class Projection
{
public:
    Projection()
        : mDirty(true), mDeviceDirty(true), mDevicePosture(Graphics::s_DefaultDevicePosture),
          mDeviceZScale(Graphics::s_DefaultDeviceZScale), mDeviceZOffset(Graphics::s_DefaultDeviceZOffset)
    {
    }
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074A494 (name is ours)
    {
        return typeInfo == getTypeInfoStatic();
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074A1F8 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009833B4 (guard 0x009833B0)
        static const RuntimeTypeInfo::Root s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~Projection(); // 0x0053F248 slot 0x08
    // 0x0053F244 slot 0x0C (deleting dtor)
    // 0 perspective, 1 orthographic
    virtual s32 getProjectionType() const = 0; // slot 0x10
    virtual void doUpdateMatrix(Matrix44<f32>* dst) const = 0; // slot 0x14
    // src turned for the posture, the depth row scaled by mDeviceZScale and moved by mDeviceZOffset
    virtual void doUpdateDeviceMatrix(Matrix44<f32>* dst, const Matrix44<f32>& src, Graphics::DevicePosture posture) const; // 0x0074A244 slot 0x18
    // the point of the camera space at the screen position (-1..1) on the near plane
    virtual void doScreenPosToCameraPosTo(Vector3<f32>* dst, const Vector2<f32>& screenPos) const = 0; // slot 0x1C

    // the matrix for the device (both matrices are recomputed when dirty)
    const Matrix44<f32>& getDeviceProjectionMatrix() const; // 0x0074A428

    // rows 0 and 1 of mtx exchanged
    static void swapRows01_(Matrix44<f32>* mtx); // 0x0054E1C4

    mutable bool mDirty;                 // 0x04
    mutable bool mDeviceDirty;           // 0x05
    mutable Matrix44<f32> mMatrix;       // 0x08
    mutable Matrix44<f32> mDeviceMatrix; // 0x48
    Graphics::DevicePosture mDevicePosture; // 0x88
    f32 mDeviceZScale;                   // 0x8C
    f32 mDeviceZOffset;                  // 0x90
};
ASSERT_SIZE(Projection, 0x94);
} // namespace sead
