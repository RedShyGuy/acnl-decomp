#pragma once

#include "decomp.h"
#include "nw/anim/res/anim_ResAnimGroupMember.h"
#include "nw/ut/ut_Offset.h"
#include "nw/ut/ut_ResDicPatricia.h"

namespace nw {
namespace anim {
namespace res {
// An animation group: the members animations can bind to, by path, and the blend operation of
// each kind of value (names are ours).
struct ResAnimGroupData
{
    enum Flag
    {
        // the animation objects write the values to the members themselves (no blend operations,
        // no SetValueForType)
        FLAG_DIRECT_RESULT = 1 << 0,
    };

    u32 m_Unknown0x0;      // 0x0
    u32 flags;             // 0x4 Flag
    u32 m_Unknown0x8[2];   // 0x8
    s32 memberCount;       // 0x10
    ut::Offset memberDic;  // 0x14 ResDicPatricia of the ResAnimGroupMemberData
    s32 blendOpCount;      // 0x18
    ut::Offset blendOps;   // 0x1C the kind of each blend operation (u32 array, see AnimGroup)
    u32 evaluationTiming;  // 0x20 in which pass of the scene update the group is animated (0 or 1)
};
ASSERT_OFFSET(ResAnimGroupData, blendOps, 0x1C);

// the binary resource of an animation group (the wrapper of its data)
class ResAnimGroup
{
public:
    // (inline)
    explicit ResAnimGroup(const void* pData = nullptr) : m_pData(static_cast<const ResAnimGroupData*>(pData)) {}
    const ResAnimGroupData* ptr() const { return m_pData; }
    s32 GetMemberCount() const { return m_pData->memberCount; }
    u32 GetFlags() const { return m_pData->flags; }
    u8 GetEvaluationTiming() const { return static_cast<u8>(m_pData->evaluationTiming); }
    s32 GetBlendOpCount() const { return m_pData->blendOpCount; }
    u32 GetBlendOpType(int index) const { return m_pData->blendOps.To<u32>()[index]; }
    ut::ResDicPatricia GetMemberDic() const
    {
        ut::ResDicPatricia dic = {m_pData->memberDic.To<ut::ResDicPatricia::Data>()};
        return dic;
    }
    // the member at index (the order of the dictionary)
    ResAnimGroupMember GetMember(int index) const
    {
        return ResAnimGroupMember(GetMemberDic().GetDataByIndex(index));
    }

    const ResAnimGroupData* m_pData; // 0x0
};
ASSERT_SIZE(ResAnimGroup, 0x4);
} // namespace res
} // namespace anim
} // namespace nw
