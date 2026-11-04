#include "net/dAcConnection.h"

#include <new>
#include "nn/ac/CTR/CTR_Api.h"

namespace net {
namespace {
// results of nn::ac::CTR::Connect that the original tests (the other cases only had debug logs, which
// armlink removed)
const bit32 RESULT_AC_CONNECT_FAILED = 0xC9209F84;
// the description of a wait that timed out (3dbrew: 1022, timeout)
const bit32 DESCRIPTION_TIMEOUT = 0x3FE;
} // namespace

// 0x009545FC
AcConnection* net::AcConnection::s_pInstance;

// 0x0050D320 (name is ours)
bool net::AcConnection::Connect(u32* errorCode)
{
    AcConnection* self = s_pInstance;
    self->m_CriticalSection.Enter();
    if (self->m_RefCount == 0) {
        self->m_DisconnectEvent.ClearSignal();
        if (!ConnectAc(errorCode, &self->m_DisconnectEvent)) {
            self->m_CriticalSection.Exit();
            return false;
        }
        self->m_IsConnected = true;
    }
    self->m_RefCount++;
    self->m_CriticalSection.Exit();
    return true;
}

// 0x0050D3A4 (name is ours)
void net::AcConnection::Disconnect()
{
    AcConnection* self = s_pInstance;
    self->m_CriticalSection.Enter();
    if (--self->m_RefCount == 0) {
        nn::ac::CTR::Close();
        nn::ac::CTR::Finalize();
        self->m_IsConnected = false;
    }
    self->m_CriticalSection.Exit();
}

// 0x0050D3EC (name is ours)
void net::AcConnection::Create()
{
    if (s_pInstance == NULL) {
        AcConnection* connection = new (std::nothrow) AcConnection;
        if (connection != NULL) {
            connection->m_CriticalSection.Initialize();
            connection->m_DisconnectEvent.Initialize(nn::os::RESET_TYPE_ONESHOT);
            connection->m_RefCount = 0;
            connection->m_IsConnected = false;
        }
        s_pInstance = connection;
    }
}

// 0x0050D46C (name is ours)
bool net::AcConnection::IsConnected()
{
    AcConnection* self = s_pInstance;
    if (self->m_IsConnected) {
        nn::Result result = nn::svc::WaitSynchronization1(self->m_DisconnectEvent.GetHandle(), 0);
        if (result.IsFailure()) {
            nn::os::CTR::detail::HandleInternalError(result);
        }
        if (result.GetDescription() == DESCRIPTION_TIMEOUT) {
            return true;
        }
        self->m_IsConnected = false;
    }
    return false;
}

// 0x0050D4C8 (name is ours)
bool net::AcConnection::ConnectAc(u32* errorCode, nn::os::Event* disconnectEvent)
{
    if (nn::ac::CTR::Initialize().IsFailure()) {
        if (errorCode != NULL) {
            *errorCode = 0;
        }
        return false;
    }
    if (disconnectEvent != NULL) {
        nn::ac::CTR::RegisterDisconnectEvent(disconnectEvent);
    }
    nnacConfig config;
    nn::ac::CTR::CreateDefaultConfig(&config);
    nn::Result result = nn::ac::CTR::Connect(config);
    if (result.IsSuccess()) {
        return true;
    }
    u32 lastErrorCode;
    u32 lastDetailErrorCode;
    nn::ac::CTR::GetLastErrorCode(&lastErrorCode);
    nn::ac::CTR::GetLastDetailErrorCode(&lastDetailErrorCode);
    if (errorCode != NULL) {
        *errorCode = lastErrorCode;
    }
    if (result == RESULT_AC_CONNECT_FAILED) {
        if (errorCode != NULL) {
            *errorCode = ERROR_CODE_AC_FAILED;
        }
    }
    nn::ac::CTR::Close();
    nn::ac::CTR::Finalize();
    return false;
}
} // namespace net
