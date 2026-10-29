#include "sead/seadResourceFactory.h"
#include "sead/seadResourceMgr_SingletonDisposer_.h"

// 0x00540B30 slot 0x00 | slot vf_0x00 of sead::IDisposer
// 0x00540AC8 (deleting dtor)
sead::ResourceMgr::SingletonDisposer_::~SingletonDisposer_()
{
    if (this != ResourceMgr::s_pSingletonDisposer) {
        return;
    }
    ResourceMgr::s_pSingletonDisposer = nullptr;
    ResourceMgr::s_pInstance->~ResourceMgr();
    ResourceMgr::s_pInstance = nullptr;
}
