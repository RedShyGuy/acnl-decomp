#include "font/dBase.h"
#include "font/dMgr.h"

namespace font {
// 0x0052F120 (name after the RTTI)
font::Base::Base()
{
    // (only the base is constructed)
}

// 0x0052F148 slot 0x00 (name after the RTTI)
// 0x0052F138 (deleting dtor)
font::Base::~Base()
{
    // (only the base is destroyed)
}

// 0x0052F0C4 slot 0x0C (name after oml::framework::Process)
oml::framework::Result font::Base::Initialize()
{
    if (Mgr::s_pInstance->IsReady()) {
        return oml::framework::RESULT_SUCCESS;
    }
    return oml::framework::RESULT_RETRY;
}

// 0x0052F104 slot 0x18 (name after oml::framework::Process)
oml::framework::Result font::Base::Finalize()
{
    Mgr::s_pInstance->Finalize();
    return oml::framework::RESULT_SUCCESS;
}

// 0x0052F0E8 slot 0x24 (name after oml::framework::Process)
oml::framework::Result font::Base::Calc()
{
    Mgr::s_pInstance->Calc();
    return oml::framework::RESULT_SUCCESS;
}

// 0x0052F0A8 slot 0x30 (name after oml::framework::Process)
oml::framework::Result font::Base::Draw()
{
    Mgr::s_pInstance->Draw();
    return oml::framework::RESULT_SUCCESS;
}
} // namespace font
