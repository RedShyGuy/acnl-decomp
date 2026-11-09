#include "script/dStrMgr_SingletonDisposer_.h"

// 0x005F3ECC slot 0x00 (name after sead)
// 0x005F3E78 (deleting dtor)
script::StrMgr::SingletonDisposer_::~SingletonDisposer_()
{
    if (this != StrMgr::s_pSingletonDisposer) {
        return;
    }
    StrMgr::s_pSingletonDisposer = NULL;
    StrMgr::s_pInstance->~StrMgr();
    StrMgr::s_pInstance = NULL;
}
