#pragma once

#include "decomp.h"
#include "g3d/dSkeletalModel.h"
#include "math/dAngle16.h"
#include "nn/math/math_Vector3.h"

// RTTI 10HumanModel @ 0x008CB0E8
// vtable 0x008EC19C (vptr 0x008EC1A4), offset_to_top 0, 11 entries
class HumanModel
{
public:
    class FaceCtrl;
    HumanModel(); // ctor candidate(s) 0x001ACE0C (unverified)
    virtual void vf_0x00(); // 0x001AC56C slot 0x00 | virtual slot, introduced by HumanModel
    virtual void vf_0x04(); // 0x001AC288 slot 0x04 | virtual slot, introduced by HumanModel
    virtual void vf_0x08(); // 0x0011C12F slot 0x08 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x0C(); // 0x0011C12F slot 0x0C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x10(); // 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x14(); // 0x0011C12F slot 0x14 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x18(); // 0x001AC280 slot 0x18 | virtual slot, introduced by HumanModel
    virtual ~HumanModel(); // 0x001ACFCC slot 0x1C | slot vf_0x1C of HumanModel
    // 0x001ACF5C slot 0x20 | slot vf_0x20 of HumanModel (deleting dtor)
    virtual void vf_0x24(); // 0x0011C12F slot 0x24 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x28(); // 0x0070B840 slot 0x28 | virtual slot, introduced by HumanModel

    // the world matrix of a bone (in the world of the model; isCurved: in the curved world on some stages; false: no
    // bone; name is ours)
    bool GetBoneMatrix(nn::math::MTX34* out, s32 boneIdx, bool isCurved) const; // 0x0070B694

    // (the layout so far as the constructor 0x001ACE0C and the tools show it; names are ours)
    u8 m_Unknown04[0x78 - 0x04];  // 0x04 (a matrix and more)
    g3d::SkeletalModel m_Model;   // 0x78
    nn::math::VEC3 m_Position;    // 0x84
    math::Angle16Vec3 m_Rotation; // 0x90
    u16 m_Unknown96;              // 0x96
    f32 m_Unknown98;              // 0x98
    s32 m_BoneIndices[8];         // 0x9C (-1: none; [3] is the hand that holds the tools)
};
ASSERT_OFFSET(HumanModel, m_Model, 0x78);
ASSERT_OFFSET(HumanModel, m_BoneIndices, 0x9C);
