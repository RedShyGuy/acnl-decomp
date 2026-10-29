#include "sead/seadResourceFactory.h"

namespace sead {
// 0x00546590 slot 0x00 | slot vf_0x00 of sead::IDisposer
// 0x0054652C (deleting dtor)
sead::ResourceFactory::~ResourceFactory()
{
    ResourceMgr* mgr = ResourceMgr::s_pInstance;
    if (mgr != nullptr) {
        mgr->unregisterFactory(this);
        if (mgr->mDefaultFactory == this) {
            mgr->setDefaultFactory(nullptr);
        }
    }
}
} // namespace sead
