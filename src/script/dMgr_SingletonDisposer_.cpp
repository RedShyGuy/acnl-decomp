#include "script/dMgr_SingletonDisposer_.h"

// 0x005EA410 slot 0x00 (name after sead)
// 0x005EA3CC (deleting dtor)
script::Mgr::SingletonDisposer_::~SingletonDisposer_()
{
    if (this != Mgr::s_pSingletonDisposer) {
        return;
    }
    Mgr::s_pSingletonDisposer = NULL;
    Mgr::s_pInstance = NULL;
}
