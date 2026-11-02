#include "ssys/ma/lyt/dDummyLayout.h"
#include "nw/lyt/lyt_AnimTransform.h"
#include "nw/lyt/lyt_Group.h"
#include "nw/lyt/lyt_GroupContainer.h"

namespace ssys {
namespace ma {
namespace lyt {
// 0x00568674 slot 0x28 (name after nw::lyt::Layout)
bool ssys::ma::lyt::DummyLayout::BindAnimationAuto(const nw::lyt::AnimResource& animRes, nw::lyt::ResourceAccessor* pResAcsr)
{
    u16 groupNum = animRes.GetGroupNum();
    u16 shareNum = animRes.GetAnimationShareInfoNum();
    if (groupNum == 0 || shareNum != 0) {
        return nw::lyt::Layout::BindAnimationAuto(animRes, pResAcsr);
    }
    if (m_pRootPane == NULL || animRes.GetResourceBlock() == NULL) {
        return false;
    }
    const nw::lyt::res::AnimationGroupRef* groupRefs = animRes.GetGroupArray();
    for (int i = 0; i < groupNum; i++) {
        nw::lyt::Group* group = GetGroupContainer()->FindGroupByName(groupRefs[i].name);
        u16 animNum = animRes.CalcAnimationNum(group, animRes.IsDescendingBind());
        nw::lyt::AnimTransform* animTrans = nw::lyt::Layout::CreateAnimTransform();
        animTrans->SetResource(animRes.GetResourceBlock(), pResAcsr, animNum);
        nw::lyt::BindAnimation(group, animTrans, animRes.IsDescendingBind(), true);
    }
    return true;
}

// 0x00568770 slot 0x10 (name after nw::lyt::Layout)
nw::lyt::AnimTransform* ssys::ma::lyt::DummyLayout::CreateAnimTransform(const void* animResBuf,
                                                                       nw::lyt::ResourceAccessor* pResAcsr)
{
    nw::lyt::AnimTransform* animTrans = nw::lyt::Layout::CreateAnimTransform(animResBuf, pResAcsr);
    if (animTrans != NULL) {
        m_AnimTransList.Erase(animTrans);
    }
    return animTrans;
}

// 0x004BAA64 slot 0x00
// 0x00568798 (deleting dtor)
ssys::ma::lyt::DummyLayout::~DummyLayout()
{
    // empty (in the original too)
}

} // namespace lyt
} // namespace ma
} // namespace ssys
