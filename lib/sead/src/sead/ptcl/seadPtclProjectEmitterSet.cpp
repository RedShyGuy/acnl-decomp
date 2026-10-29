#include "sead/ptcl/seadPtclProjectEmitterSet.h"
#include "sead/ptcl/seadPtclResource.h"
#include "sead/ptcl/seadPtclSystem.h"

namespace sead {
namespace ptcl {
// 0x0055C1C8 | mk7dlp:bytes [tier B]
void sead::ptcl::PtclProjectEmitterSet::bindTarget(s32 index, PtclResource* resource, s32 emitterSetId)
{
    s32 numEmitter = 0;
    for (s32 i = 0; i < NUM_EMITTER_MAX; i++) {
        if (mEmitters[i] != nullptr) {
            numEmitter++;
        }
    }
    resource->bindTarget(emitterSetId, &mBind[index], mEmitterTbl, numEmitter, mName, mUserData);
}

// 0x0055C244 | mk7dlp:bytes-fuzzy [tier B]
void sead::ptcl::PtclProjectEmitterSet::unbindResource(PtclResource* resource)
{
    for (u32 i = 0; i < 2; i++) {
        ResourceBind& bind = mBind[i];
        if (bind.setData != nullptr && mSystem != nullptr && mSystem->mResources[bind.resourceId] == resource) {
            resource->unbind(&bind, false, false);
            updateMetaName_();
        }
    }
}

// 0x0055C30C | mk7dlp:bytes [tier B]
void sead::ptcl::PtclProjectEmitterSet::setOrder(s32 order)
{
    mOrder = order;
    updateMetaName_();
}
} // namespace ptcl
} // namespace sead
