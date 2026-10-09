// (the original file is http_CommonImpl.cpp, after its static initializer)
#include "nn/http/detail/http_LibManager.h"
#include "nn/dbg/dbg_Api.h"
#include "nn/srv/srv_Api.h"

namespace nn {
namespace http {
namespace detail {
namespace {
// status, invalid state, module 40 http, already initialized (name is ours)
const bit32 RESULT_ALREADY_INITIALIZED = 0xD8A0A3F9;
// the permissions of the shared memory: none for this process, read and write for http
const u32 MY_PERMISSION = 0;
const u32 OTHER_PERMISSION = 3;
} // namespace

// 0x00AE1EC8
LibManager s_LibManager;

// 0x0046FD10 | fefates:bytes [tier B]
nn::Result nn::http::detail::LibManager::Initialize(uptr buffer, size_t size)
{
    if (m_IsInitialized) {
        return nn::Result(RESULT_ALREADY_INITIALIZED);
    }
    if (nn::srv::Initialize().IsFailure()) {
        nndbgPanic();
    }
    nn::Result result = nn::srv::GetServiceHandle(&m_Session, "http:C");
    if (result.IsFailure()) {
        return result;
    }
    m_Ipc.m_Session = m_Session.GetHandle();
    nn::Handle sharedMemory;
    if (size != 0) {
        result = m_TransferMemory.TryInitialize(reinterpret_cast<void*>(buffer), size, MY_PERMISSION, OTHER_PERMISSION);
        if (result.IsFailure()) {
            m_Session.Close();
            return result;
        }
        sharedMemory = m_TransferMemory.GetHandle();
    }
    result = m_Ipc.InitializeGeneralSession(sharedMemory, size);
    if (result.IsSuccess()) {
        m_IsInitialized = true;
        return result;
    }
    m_Session.Close();
    if (m_TransferMemory.GetHandle().IsValid()) {
        m_TransferMemory.Finalize();
    }
    return result;
}

// 0x0046FE70 slot 0x00
// 0x0046FE14 (deleting dtor)
nn::http::detail::LibManager::~LibManager()
{
}

} // namespace detail
} // namespace http
} // namespace nn
