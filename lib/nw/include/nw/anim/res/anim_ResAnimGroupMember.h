#pragma once

#include "decomp.h"

namespace nw {
namespace anim {
namespace res {
// A member of an animation group: one value of an object an animation can change (names are ours).
// The object is found when the group is set up.
struct ResAnimGroupMemberData
{
    // the kind of the animated value (only the material ones are known by name; name is ours)
    enum MemberType
    {
        MEMBER_TYPE_TRANSFORM = 0,           // a CalculatedTransform
        MEMBER_TYPE_MATERIAL_COLOR = 1,
        MEMBER_TYPE_TEXTURE_SAMPLER = 2,     // the border color
        MEMBER_TYPE_TEXTURE_MAPPER = 3,      // the texture
        MEMBER_TYPE_BLEND_OPERATION = 4,     // the blend color
        MEMBER_TYPE_TEXTURE_COORDINATOR = 5,
        MEMBER_TYPE_SCENE_NODE = 9,          // a value of a scene node (field 0: its transform)
        MEMBER_TYPE_VIEW_UPDATER = 10,       // a value of the view updater of a camera
        MEMBER_TYPE_PROJECTION_UPDATER = 11, // a value of the projection updater of a camera
        MEMBER_TYPE_LIGHT = 12,              // a value of a light (field 0: whether it is enabled)
        MEMBER_TYPE_FRAGMENT_LIGHT = 13,
        MEMBER_TYPE_VERTEX_LIGHT = 14,
        MEMBER_TYPE_HEMISPHERE_LIGHT = 15,
        MEMBER_TYPE_AMBIENT_LIGHT = 16,      // (field 0: the color)
        MEMBER_TYPE_FOG = 17,                // (field 0: the color)
    };

    u32 m_Unknown0x0[2]; // 0x0
    u32 valueOffset;     // 0x8 the position of the value in the object
    u32 blendOpIndex;    // 0xC the index of its blend operation in the group
    u32 memberType;      // 0x10 MemberType
    u32 field;           // 0x14 which value of the object (by type)
    void* pObject;       // 0x18 the animated object (resolved at setup)
};
ASSERT_OFFSET(ResAnimGroupMemberData, pObject, 0x18);

// the binary resource of a group member (the wrapper of its data)
class ResAnimGroupMember
{
public:
    // (inline)
    explicit ResAnimGroupMember(const void* pData = nullptr) : m_pData(static_cast<const ResAnimGroupMemberData*>(pData)) {}
    const ResAnimGroupMemberData* ptr() const { return m_pData; }
    u32 GetMemberType() const { return m_pData->memberType; }
    u32 GetField() const { return m_pData->field; }
    void* GetObject() const { return m_pData->pObject; }

    void SetValueForType(void*, const void*) const; // 0x007442FC | nintendogs:callseq [tier A]

    const ResAnimGroupMemberData* m_pData; // 0x0
};
ASSERT_SIZE(ResAnimGroupMember, 0x4);
} // namespace res
} // namespace anim
} // namespace nw
