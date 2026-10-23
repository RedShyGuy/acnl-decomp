#pragma once

#include "decomp.h"
#include "nw/anim/res/anim_ResAnimGroupMember.h"

namespace nw {
namespace anim {
namespace res {
// A member of the material colors (the class name and SetValue are from the reference symbols)
class ResMaterialColorMember : public ResAnimGroupMember
{
public:
    // (inline)
    explicit ResMaterialColorMember(const void* pData = nullptr) : ResAnimGroupMember(pData) {}

    // the value of the field of the member written to the target object
    void SetValue(void* pTarget, const void* pValue) const; // 0x00744A1C | nintendogs:callseq [tier A]
};
} // namespace res
} // namespace anim
} // namespace nw
