#include "script/dUiMgr_SingletonDisposer_.h"

// 0x005EB1D4 slot 0x00 (name after sead)
// 0x005EB134 (deleting dtor)
script::UiMgr::SingletonDisposer_::~SingletonDisposer_()
{
    if (this != UiMgr::s_pSingletonDisposer) {
        return;
    }
    UiMgr::s_pSingletonDisposer = NULL;
    UiMgr::s_pInstance->~UiMgr();
    UiMgr::s_pInstance = NULL;
}
