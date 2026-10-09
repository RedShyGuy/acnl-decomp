#include "nn/http/http_Api.h"
#include "nn/http/detail/http_LibManager.h"

namespace nn {
namespace http {
namespace {
// status, invalid state, module 40 http, not initialized (name is ours)
const bit32 RESULT_NOT_INITIALIZED = 0xD8A0A3F8;
} // namespace

// 0x0046F714 (name is ours)
nn::Result Initialize(uptr buffer, size_t size)
{
    return detail::s_LibManager.Initialize(buffer, size);
}

// 0x0046FF10 | fefates:bytes [tier B]
nn::Result Finalize()
{
    detail::LibManager& manager = detail::s_LibManager;
    if (!manager.m_IsInitialized) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    manager.m_Ipc.FinalizeClient();
    manager.m_Session.Close();
    if (manager.m_TransferMemory.GetHandle().IsValid()) {
        manager.m_TransferMemory.Finalize();
    }
    manager.m_IsInitialized = false;
    return nn::Result();
}

} // namespace http
} // namespace nn
