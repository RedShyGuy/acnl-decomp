#include "nn/ssl/ssl_Api.h"
#include "nn/dbg/dbg_Api.h"
#include "nn/srv/srv_Api.h"
#include "nn/ssl/detail/ssl_LibManager.h"

namespace nn {
namespace ssl {

namespace {
const char SERVICE_NAME[] = "ssl:C";
} // namespace

// 0x00467204 | fefates:bytes [tier B]
nn::Result Initialize()
{
    detail::LibManager& manager = detail::s_LibManager;
    nn::os::CriticalSection::ScopedLock lock(manager.mLock);
    if (manager.mInitializeCount >= 1) {
        manager.mInitializeCount++;
        return nn::Result();
    }
    if (nn::srv::Initialize().IsFailure()) {
        nndbgPanic();
    }
    nn::Result result = nn::srv::GetServiceHandle(&manager.mSession, SERVICE_NAME);
    if (result.IsFailure()) {
        return result;
    }
    manager.mConnection.SetSession(manager.mSession.GetHandle());
    nn::Result initResult = manager.mConnection.InitializeGeneralSession();
    if (initResult.IsFailure()) {
        manager.mSession.Close();
        return initResult;
    }
    manager.mInitializeCount = 1;
    return result;
}

// 0x004673E8 | fefates:bytes [tier B]
nn::Result Finalize()
{
    detail::LibManager& manager = detail::s_LibManager;
    nn::os::CriticalSection::ScopedLock lock(manager.mLock);
    if (manager.mInitializeCount <= 0) {
        return nn::Result(detail::RESULT_NOT_INITIALIZED);
    }
    manager.mInitializeCount--;
    if (manager.mInitializeCount < 1) {
        manager.mSession.Close();
    }
    return nn::Result();
}

} // namespace ssl
} // namespace nn
