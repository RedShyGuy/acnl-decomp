#include "script/dMailMgr_SingletonDisposer_.h"

// 0x005F5758 slot 0x00 (name after sead)
// 0x005F56F0 (deleting dtor)
script::MailMgr::SingletonDisposer_::~SingletonDisposer_()
{
    if (this != MailMgr::s_pSingletonDisposer) {
        return;
    }
    MailMgr::s_pSingletonDisposer = NULL;
    MailMgr::s_pInstance->~MailMgr();
    MailMgr::s_pInstance = NULL;
}
