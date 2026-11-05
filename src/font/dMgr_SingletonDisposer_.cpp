#include "font/dMgr_SingletonDisposer_.h"

// 0x0052ECFC slot 0x00 (name after sead)
// 0x0052ECAC (deleting dtor)
font::Mgr::SingletonDisposer_::~SingletonDisposer_()
{
    if (this != Mgr::s_pSingletonDisposer) {
        return;
    }
    Mgr::s_pSingletonDisposer = NULL;
    Mgr::s_pInstance->~Mgr();
    Mgr::s_pInstance = NULL;
}
