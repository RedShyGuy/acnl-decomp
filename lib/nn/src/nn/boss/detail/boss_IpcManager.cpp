// boss_IpcManager.cpp (file name from the static initializer 0x007998DC: s_IpcManager)
#include "nn/boss/detail/boss_IpcManager.h"
#include "nn/boss/detail/detail_Api.h"
#include "nn/dbg/dbg_Api.h"
#include "nn/srv/srv_Api.h"

namespace nn {
namespace boss {
namespace detail {
namespace {
// the codes of ChangeBossRetCodeToResult (names are ours)
const nn::boss::ResultCode CODE_NOT_INITIALIZED = static_cast<nn::boss::ResultCode>(43);
const nn::boss::ResultCode CODE_ALREADY_INITIALIZED = static_cast<nn::boss::ResultCode>(46);

const char USER_SERVICE_NAME[] = "boss:U";
} // namespace

// 0x00AF6190
nn::boss::detail::IpcManager s_IpcManager;

// 0x0046CF24 | nintendogs:bytes [tier A]
nn::Result nn::boss::detail::IpcManager::FinalizeUserIpc()
{
    if (!m_IsUserInitialized) {
        return ChangeBossRetCodeToResult(CODE_NOT_INITIALIZED);
    }
    m_UserSession.Close();
    m_IsUserInitialized = false;
    return nn::Result();
}

// 0x0046CF6C | nintendogs:bytes [tier A]
nn::Result nn::boss::detail::IpcManager::InitializeUserIpc()
{
    if (m_IsUserInitialized) {
        return ChangeBossRetCodeToResult(CODE_ALREADY_INITIALIZED);
    }
    if (nn::srv::Initialize().IsFailure()) {
        nndbgPanic();
    }
    nn::Result result = nn::srv::GetServiceHandle(&m_UserSession, USER_SERVICE_NAME);
    if (result.IsFailure()) {
        return result;
    }
    m_User.m_Session = m_UserSession.GetHandle();
    nn::Result sessionResult = m_User.InitializeSession(m_ProgramId);
    if (sessionResult.IsSuccess()) {
        m_IsUserInitialized = true;
        return result;
    }
    m_UserSession.Close();
    return sessionResult;
}

// 0x0046D074
// 0x0046D010 (deleting dtor)
nn::boss::detail::IpcManager::~IpcManager()
{
}

} // namespace detail
} // namespace boss
} // namespace nn
