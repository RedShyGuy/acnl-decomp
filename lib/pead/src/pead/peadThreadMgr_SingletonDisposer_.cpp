#include "pead/peadThreadMgr_SingletonDisposer_.h"

// 0x0053DDEC slot 0x00 | slot vf_0x00 of pead::IDisposer
// 0x0053DD94 (deleting dtor)
pead::ThreadMgr::SingletonDisposer_::~SingletonDisposer_()
{
    if (this != ThreadMgr::s_pSingletonDisposer) {
        return;
    }
    ThreadMgr::s_pSingletonDisposer = nullptr;
    ThreadMgr::s_pInstance->~ThreadMgr();
    ThreadMgr::s_pInstance = nullptr;
}
