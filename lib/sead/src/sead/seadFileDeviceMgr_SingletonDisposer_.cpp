#include "sead/seadFileDeviceMgr_SingletonDisposer_.h"

// 0x00542DA8 slot 0x00 | nintendogs:bytes
// 0x00542D28 (deleting dtor)
sead::FileDeviceMgr::SingletonDisposer_::~SingletonDisposer_()
{
    if (this != FileDeviceMgr::s_pSingletonDisposer) {
        return;
    }
    FileDeviceMgr::s_pSingletonDisposer = nullptr;
    FileDeviceMgr::s_pInstance->~FileDeviceMgr();
    FileDeviceMgr::s_pInstance = nullptr;
}
