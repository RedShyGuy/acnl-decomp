#include "script/dBase.h"
#include "script/dMgr.h"

namespace script {
// 0x005EA8FC (name after the RTTI)
Base::Base()
{
    // (only the bases are constructed)
}

// 0x005EA924 slot 0x00
// 0x005EA914 (deleting dtor)
Base::~Base()
{
    // (only the bases are destroyed)
}

// 0x005EA890 slot 0x0C
oml::framework::Result Base::Initialize()
{
    Mgr* mgr = Mgr::s_pInstance;
    if (mgr == NULL) {
        mgr = Mgr::createInstance(NULL);
    }
    return mgr->Initialize() ? oml::framework::RESULT_SUCCESS : oml::framework::RESULT_RETRY;
}

// 0x005EA8DC slot 0x18
oml::framework::Result Base::Finalize()
{
    Mgr::s_pInstance->Finalize();
    Mgr::deleteInstance();
    return oml::framework::RESULT_SUCCESS;
}

// 0x005EA8C0 slot 0x24
oml::framework::Result Base::Calc()
{
    Mgr::s_pInstance->Calc();
    return oml::framework::RESULT_SUCCESS;
}

// 0x005EA874 slot 0x30
oml::framework::Result Base::Draw()
{
    Mgr::s_pInstance->Draw();
    return oml::framework::RESULT_SUCCESS;
}
} // namespace script
