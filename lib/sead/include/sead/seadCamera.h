#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadRoot.h"
#include "sead/seadMatrix.h"

namespace sead {
// RTTI N4sead6CameraE @ 0x008D20A8
// vtable 0x00906B60 (vptr 0x00906B68), offset_to_top 0, 5 entries
//
// A camera: the view matrix and the slot that computes it (doUpdateMatrix is the name of the
// reference symbols, the member name is ours).
class Camera
{
public:
    Camera() : mMatrix(Matrix34<f32>::ident) {}
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074EDBC (name is ours)
    {
        return typeInfo == getTypeInfoStatic();
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074ED70 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009835AC (guard 0x009835A8)
        static const RuntimeTypeInfo::Root s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~Camera(); // 0x0055D5A0 slot 0x08
    // 0x0055D59C slot 0x0C (deleting dtor)
    // computes the view matrix into dst
    virtual void doUpdateMatrix(Matrix34<f32>* dst) const = 0; // slot 0x10

    Matrix34<f32> mMatrix; // 0x04, the view matrix
};
ASSERT_SIZE(Camera, 0x34);
} // namespace sead
