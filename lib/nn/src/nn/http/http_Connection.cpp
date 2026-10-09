#include "nn/http/http_Connection.h"
#include <string.h>
#include "nn/http/detail/http_LibManager.h"
#include "nn/srv/srv_Api.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace http {
namespace {
// module 40 http (names are ours)
// usage, invalid argument, 2: a NULL argument
const bit32 RESULT_INVALID_ARGUMENT = 0xD8E0A002;
// status, invalid state, 100: the connection is not open
const bit32 RESULT_NOT_CONNECTED = 0xD8A0A064;
// status, invalid state, 101: the connection is open
const bit32 RESULT_ALREADY_CONNECTED = 0xD8A0A065;
// status, invalid state, not initialized
const bit32 RESULT_NOT_INITIALIZED = 0xD8A0A3F8;

const char SERVICE_NAME[] = "http:C";
} // namespace

using detail::s_LibManager;

// 0x0046F1A8 | mk7dlp:callseq [tier A]
nn::Result nn::http::Connection::Initialize(const char* url, RequestMethod method, bool useDefaultProxy)
{
    if (url == NULL) {
        return nn::Result(RESULT_INVALID_ARGUMENT);
    }
    if (m_Handle > 0) {
        return nn::Result(RESULT_ALREADY_CONNECTED);
    }
    if (!s_LibManager.m_IsInitialized) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    nn::Result result = s_LibManager.m_Ipc.CreateConnection(url, strlen(url) + 1, method, &m_Handle);
    if (result.IsSuccess()) {
        result = nn::srv::GetServiceHandle(&m_Session, SERVICE_NAME, strlen(SERVICE_NAME), 0);
        if (result.IsSuccess()) {
            m_Ipc.m_Session = m_Session;
            nn::Result sessionResult = m_Ipc.InitializeConnectionSession(m_Handle);
            if (sessionResult.IsFailure()) {
                nn::svc::CloseHandle(m_Session);
                result = sessionResult;
            }
        }
        if (result.IsSuccess()) {
            if (!useDefaultProxy) {
                return nn::Result();
            }
            if (m_Handle > 0) {
                result = m_Ipc.SetProxyDefault(m_Handle);
            } else {
                result = nn::Result(RESULT_NOT_CONNECTED);
            }
            if (result.IsSuccess()) {
                return nn::Result();
            }
        }
    }
    m_Handle = 0;
    return result;
}

// 0x0046F2D0 (name after the command)
nn::Result nn::http::Connection::StartConnectionAsync()
{
    if (m_Handle <= 0) {
        return nn::Result(RESULT_NOT_CONNECTED);
    }
    return m_Ipc.StartConnectionAsync(m_Handle);
}

// 0x0046F2F0 (name after the command)
nn::Result nn::http::Connection::SetInternalClientCert(u32 certId)
{
    if (m_Handle <= 0) {
        return nn::Result(RESULT_NOT_CONNECTED);
    }
    return m_Ipc.SetInternalClientCert(m_Handle, certId);
}

// 0x0046F314 (name after the command)
nn::Result nn::http::Connection::AddHeaderField(const char* label, const char* value)
{
    if (label == NULL || value == NULL) {
        return nn::Result(RESULT_INVALID_ARGUMENT);
    }
    if (m_Handle <= 0) {
        return nn::Result(RESULT_NOT_CONNECTED);
    }
    size_t valueSize = strlen(value) + 1;
    return m_Ipc.AddHeaderField(m_Handle, label, strlen(label) + 1, value, valueSize);
}

// 0x0046F384 (name after the command)
nn::Result nn::http::Connection::SendPostDataRaw(const u8* data, size_t size)
{
    if (data == NULL || size == 0) {
        return nn::Result(RESULT_INVALID_ARGUMENT);
    }
    if (m_Handle <= 0) {
        return nn::Result(RESULT_NOT_CONNECTED);
    }
    return m_Ipc.SendPostDataRaw(m_Handle, data, size);
}

// 0x0046F3C4 (name after the command)
nn::Result nn::http::Connection::AddPostDataAscii(const char* label, const char* value)
{
    if (label == NULL || value == NULL) {
        return nn::Result(RESULT_INVALID_ARGUMENT);
    }
    if (m_Handle <= 0) {
        return nn::Result(RESULT_NOT_CONNECTED);
    }
    size_t valueSize = strlen(value) + 1;
    return m_Ipc.AddPostDataAscii(m_Handle, label, strlen(label) + 1, value, valueSize);
}

// 0x0046F434 (name after the command)
nn::Result nn::http::Connection::AddPostDataBinary(const char* label, const u8* data, size_t size)
{
    if (label == NULL || data == NULL || size == 0) {
        return nn::Result(RESULT_INVALID_ARGUMENT);
    }
    if (m_Handle <= 0) {
        return nn::Result(RESULT_NOT_CONNECTED);
    }
    return m_Ipc.AddPostDataBinary(m_Handle, label, strlen(label) + 1, data, size);
}

// 0x0046F4A0 | fefates:bytes [tier B]
nn::Result nn::http::Connection::SetLazyPostDataSetting(PostDataType type, size_t size)
{
    if (m_Handle <= 0) {
        return nn::Result(RESULT_NOT_CONNECTED);
    }
    if (size != 0) {
        if (type != POST_DATA_TYPE_RAW) {
            return nn::Result(RESULT_INVALID_ARGUMENT);
        }
        return m_Ipc.SetLazyPostDataSettingWithSize(m_Handle, type, size);
    }
    return m_Ipc.SetLazyPostDataSetting(m_Handle, type);
}

// 0x0046F4F4 (name after the command)
nn::Result nn::http::Connection::NotifyFinishSendPostData()
{
    if (m_Handle <= 0) {
        return nn::Result(RESULT_NOT_CONNECTED);
    }
    return m_Ipc.NofityFinishSendPostData(m_Handle);
}

// 0x0046F514 (name after the command)
nn::Result nn::http::Connection::ReadBody(u8* pBuffer, size_t size)
{
    if (pBuffer == NULL || size == 0) {
        return nn::Result(RESULT_INVALID_ARGUMENT);
    }
    if (m_Handle <= 0) {
        return nn::Result(RESULT_NOT_CONNECTED);
    }
    return m_Ipc.ReadBody(m_Handle, pBuffer, size);
}

// 0x0046F554 | fefates:bytes [tier B]
nn::Result nn::http::Connection::Cancel()
{
    if (m_Handle <= 0) {
        return nn::Result(RESULT_NOT_CONNECTED);
    }
    if (!s_LibManager.m_IsInitialized) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    return s_LibManager.m_Ipc.CancelConnection(m_Handle);
}

// 0x0046F590 | fefates:bytes [tier B]
nn::Result nn::http::Connection::Finalize()
{
    if (m_Handle <= 0) {
        return nn::Result(RESULT_NOT_CONNECTED);
    }
    nn::svc::CloseHandle(m_Session);
    if (!s_LibManager.m_IsInitialized) {
        m_Handle = 0;
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    nn::Result result = s_LibManager.m_Ipc.DestroyConnection(m_Handle);
    m_Handle = 0;
    return result;
}

// 0x0046F5EC | mk7dlp:callgraph [tier A]
nn::Result nn::http::Connection::SetRootCa(const u8* cert, size_t size)
{
    if (m_Handle <= 0) {
        return nn::Result(RESULT_NOT_CONNECTED);
    }
    return m_Ipc.SetRootCA(m_Handle, cert, size);
}

// 0x0046F618 (name after the command)
nn::Result nn::http::Connection::SetInternalRootCa(u32 certId)
{
    if (m_Handle <= 0) {
        return nn::Result(RESULT_NOT_CONNECTED);
    }
    return m_Ipc.SetInternalRootCA(m_Handle, certId);
}

// 0x0046F63C | mk7dlp:bytes [tier A]
nn::http::Connection::Connection() : m_Handle(0)
{
}

// 0x0046F6B8 slot 0x00
// 0x0046F658 (deleting dtor)
nn::http::Connection::~Connection()
{
    Finalize();
}

// 0x00737168 | fefates:bytes [tier B]
nn::Result nn::http::Connection::GetProgress(u32* pReceived, u32* pTotal) const
{
    if (pReceived == NULL || pTotal == NULL) {
        return nn::Result(RESULT_INVALID_ARGUMENT);
    }
    if (m_Handle <= 0) {
        return nn::Result(RESULT_NOT_CONNECTED);
    }
    if (!s_LibManager.m_IsInitialized) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    return s_LibManager.m_Ipc.GetConnectionProgress(m_Handle, pReceived, pTotal);
}

// 0x007371C4 | fefates:bytes [tier B]
nn::Result nn::http::Connection::GetSslError(int* pError) const
{
    if (pError == NULL) {
        return nn::Result(RESULT_INVALID_ARGUMENT);
    }
    if (m_Handle <= 0) {
        return nn::Result(RESULT_NOT_CONNECTED);
    }
    if (!s_LibManager.m_IsInitialized) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    return s_LibManager.m_Ipc.GetConnectionSslError(m_Handle, pError);
}

// 0x00737218 | fefates:bytes [tier B]
nn::Result nn::http::Connection::GetStatusCode(int* pStatusCode) const
{
    if (pStatusCode == NULL) {
        return nn::Result(RESULT_INVALID_ARGUMENT);
    }
    if (m_Handle <= 0) {
        return nn::Result(RESULT_NOT_CONNECTED);
    }
    return m_Ipc.GetResultCode(m_Handle, pStatusCode);
}

// 0x00737250 | fefates:bytes [tier B]
nn::Result nn::http::Connection::GetHeaderField(const char* label, char* value, size_t valueSize, size_t* pValueSize) const
{
    size_t size = 0;
    char empty = 0;
    if (label == NULL) {
        return nn::Result(RESULT_INVALID_ARGUMENT);
    }
    if (m_Handle <= 0) {
        return nn::Result(RESULT_NOT_CONNECTED);
    }
    // without a buffer only the size is read
    if (value == NULL || valueSize == 0) {
        value = &empty;
        valueSize = 1;
    }
    nn::Result result = m_Ipc.GetHeaderField(m_Handle, label, strlen(label) + 1, value, valueSize, &size);
    if (result.IsFailure()) {
        return result;
    }
    if (pValueSize != NULL) {
        *pValueSize = size;
    }
    return nn::Result();
}

} // namespace http
} // namespace nn
