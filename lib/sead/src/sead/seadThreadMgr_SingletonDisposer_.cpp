#include "sead/seadThreadMgr_SingletonDisposer_.h"

// 0x00562AB4 slot 0x00 | slot vf_0x00 of sead::IDisposer
// 0x00562A5C (deleting dtor)
sead::ThreadMgr::SingletonDisposer_::~SingletonDisposer_()
{
    if (this != ThreadMgr::s_pSingletonDisposer) {
        return;
    }
    ThreadMgr::s_pSingletonDisposer = nullptr;
    ThreadMgr::s_pInstance->~ThreadMgr();
    ThreadMgr::s_pInstance = nullptr;
}
