#pragma once

#include "decomp.h"
#include "Human/dHumanAnimName.h"
#include "Human/dHumanModel.h"

namespace g3d {
class GroupSkeletalAnim;
class Model;
class ResourceSet;
} // namespace g3d

// The face control of a human model (class name from the RTTI, size after libgarden; the members are ours and only
// named where the code shows them). HumanModel animates the face through it (the talk animations NML_TALK_1/2 alternate
// in Update); HumanFaceAnimControl adds the material animations of the face.
// RTTI N10HumanModel8FaceCtrlE @ 0x008CD82C
class HumanModel::FaceCtrl
{
public:
    // (inline, in the constructors of the derived classes)
    FaceCtrl()
        : m_pResourceSet(NULL), m_pGroupAnim(NULL), m_pHumanModel(NULL), m_AnimId10(HumanAnimName::ANIM_FA),
          m_AnimId12(HumanAnimName::ANIM_FB), m_Unknown14(0), m_Unknown16(false), m_Unknown17(false),
          m_Unknown18(false), m_pModel(NULL)
    {
    }
    virtual ~FaceCtrl() {}

    // the animation slot of the mouth in the GroupSkeletalAnim of the model
    static const s32 FACE_ANIM_SLOT = 3;

    // (in the file of HumanModel) the talk animations of the mouth: with m_Unknown18 in slot 3 of the animation
    // group of the model (NML_TALK_1/2 alternate), else in the second slot of HumanFaceAnimControl
    bool Update(); // 0x001AC59C

    g3d::ResourceSet* m_pResourceSet; // 0x04 the face resources (the graphics file of the material animations)
    g3d::GroupSkeletalAnim* m_pGroupAnim; // 0x08 the animation group of the model (HumanModel::BindFaceCtrlAnim)
    HumanModel* m_pHumanModel;        // 0x0C (HumanModel::BindFaceCtrl)
    u16 m_AnimId10;                   // 0x10 HumanAnimName
    u16 m_AnimId12;                   // 0x12 HumanAnimName
    u16 m_Unknown14;                  // 0x14 (0 or 1: which talk animation is next)
    bool m_Unknown16;                 // 0x16 (a new talk animation is due; cleared by Update)
    bool m_Unknown17;                 // 0x17
    bool m_Unknown18;                 // 0x18 the talk animations play in the animation group of the model
    g3d::Model* m_pModel;             // 0x1C the model of the face
};
ASSERT_SIZE(HumanModel::FaceCtrl, 0x20);
