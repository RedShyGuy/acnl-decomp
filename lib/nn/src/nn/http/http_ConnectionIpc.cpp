#include "nn/http/http_ConnectionIpc.h"
#include "nn/os/os_ThreadLocalRegion.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace http {
namespace {
// command headers (3dbrew "HTTP Services")
const bit32 COMMAND_INITIALIZE = 0x00010044;
const bit32 COMMAND_CREATE_CONTEXT = 0x00020082;
const bit32 COMMAND_CLOSE_CONTEXT = 0x00030040;
const bit32 COMMAND_CANCEL_CONNECTION = 0x00040040;
const bit32 COMMAND_GET_DOWNLOAD_SIZE_STATE = 0x00060040;
const bit32 COMMAND_INITIALIZE_CONNECTION_SESSION = 0x00080042;
const bit32 COMMAND_BEGIN_REQUEST_ASYNC = 0x000A0040;
const bit32 COMMAND_RECEIVE_DATA = 0x000B0082;
const bit32 COMMAND_SET_PROXY_DEFAULT = 0x000E0040;
const bit32 COMMAND_ADD_REQUEST_HEADER = 0x001100C4;
const bit32 COMMAND_ADD_POST_DATA_ASCII = 0x001200C4;
const bit32 COMMAND_ADD_POST_DATA_BINARY = 0x001300C4;
const bit32 COMMAND_SET_POST_DATA_TYPE = 0x00150080;
const bit32 COMMAND_SEND_POST_DATA_RAW = 0x001A0082;
const bit32 COMMAND_NOTIFY_FINISH_SEND_POST_DATA = 0x001D0040;
const bit32 COMMAND_GET_RESPONSE_HEADER = 0x001E00C4;
const bit32 COMMAND_GET_RESPONSE_STATUS_CODE = 0x00220040;
const bit32 COMMAND_ADD_TRUSTED_ROOT_CA = 0x00240082;
const bit32 COMMAND_ADD_DEFAULT_CERT = 0x00250080;
const bit32 COMMAND_SET_CLIENT_CERT_DEFAULT = 0x00280080;
const bit32 COMMAND_GET_SSL_ERROR = 0x002A0040;
const bit32 COMMAND_SET_POST_DATA_TYPE_SIZE = 0x003800C0;
const bit32 COMMAND_FINALIZE = 0x00390000;

// the translation descriptors (3dbrew "IPC")
const bit32 DESCRIPTION_PROCESS_ID = 0x20;
const bit32 DESCRIPTION_HANDLE_COPY = 0;

inline bit32 ReadBufferDescriptor(size_t size)
{
    return (size << 4) | 0xA;
}

inline bit32 WriteBufferDescriptor(size_t size)
{
    return (size << 4) | 0xC;
}

// a static buffer of id 3
inline bit32 StaticBufferDescriptor(size_t size)
{
    return (size << 14) | 0xC02;
}

inline void SetByte(bit32* word, u8 value)
{
    *reinterpret_cast<u8*>(word) = value;
}

inline nn::Result Send(nn::Handle session, bit32* command)
{
    nn::Result result = nn::svc::SendSyncRequest(session);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result(command[1]);
}

inline nn::Result SendHandleCommand(nn::Handle session, bit32 header, int handle)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = header;
    command[1] = handle;
    return Send(session, command);
}

// a label and a value of two buffers (headers and post data)
inline nn::Result SendLabelCommand(nn::Handle session, bit32 header, int handle, const char* label, size_t labelSize, const void* value, size_t valueSize)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = header;
    command[1] = handle;
    command[2] = labelSize;
    command[3] = valueSize;
    command[4] = StaticBufferDescriptor(labelSize);
    command[5] = reinterpret_cast<uptr>(label);
    command[6] = ReadBufferDescriptor(valueSize);
    command[7] = reinterpret_cast<uptr>(value);
    return Send(session, command);
}
} // namespace

// 0x0046F728 | fefates:bytes [tier B]
nn::Result nn::http::ConnectionIpc::GetResultCode(int handle, int* pStatusCode)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_RESPONSE_STATUS_CODE;
    command[1] = handle;
    nn::Result result = nn::svc::SendSyncRequest(m_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pStatusCode = command[2];
    return nn::Result(command[1]);
}

// 0x0046F764 | fefates:bytes [tier B]
nn::Result nn::http::ConnectionIpc::AddHeaderField(int handle, const char* label, size_t labelSize, const char* value, size_t valueSize)
{
    return SendLabelCommand(m_Session, COMMAND_ADD_REQUEST_HEADER, handle, label, labelSize, value, valueSize);
}

// 0x0046F7C4 | fefates:bytes [tier B]
nn::Result nn::http::ConnectionIpc::FinalizeClient()
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_FINALIZE;
    return Send(m_Session, command);
}

// 0x0046F7EC | fefates:bytes [tier B]
nn::Result nn::http::ConnectionIpc::GetHeaderField(int handle, const char* label, size_t labelSize, char* value, size_t valueSize, size_t* pValueSize)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_RESPONSE_HEADER;
    command[1] = handle;
    command[2] = labelSize;
    command[3] = valueSize;
    command[4] = StaticBufferDescriptor(labelSize);
    command[5] = reinterpret_cast<uptr>(label);
    command[6] = WriteBufferDescriptor(valueSize);
    command[7] = reinterpret_cast<uptr>(value);
    nn::Result result = nn::svc::SendSyncRequest(m_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pValueSize = command[2];
    return nn::Result(command[1]);
}

// 0x0046F858 | fefates:bytes [tier B]
nn::Result nn::http::ConnectionIpc::SendPostDataRaw(int handle, const u8* data, size_t size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SEND_POST_DATA_RAW;
    command[1] = handle;
    command[2] = size;
    command[3] = ReadBufferDescriptor(size);
    command[4] = reinterpret_cast<uptr>(data);
    return Send(m_Session, command);
}

// 0x0046F89C | mk7dlp:callseq-callee [tier A]
nn::Result nn::http::ConnectionIpc::SetProxyDefault(int handle)
{
    return SendHandleCommand(m_Session, COMMAND_SET_PROXY_DEFAULT, handle);
}

// 0x0046F8CC (name after 3dbrew)
nn::Result nn::http::ConnectionIpc::AddPostDataAscii(int handle, const char* label, size_t labelSize, const char* value, size_t valueSize)
{
    return SendLabelCommand(m_Session, COMMAND_ADD_POST_DATA_ASCII, handle, label, labelSize, value, valueSize);
}

// 0x0046F92C | fefates:bytes [tier B]
nn::Result nn::http::ConnectionIpc::CancelConnection(int handle)
{
    return SendHandleCommand(m_Session, COMMAND_CANCEL_CONNECTION, handle);
}

// 0x0046F95C | mk7dlp:callseq-callee [tier A]
nn::Result nn::http::ConnectionIpc::CreateConnection(const char* url, size_t urlSize, RequestMethod method, int* pHandle)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_CREATE_CONTEXT;
    command[1] = urlSize;
    SetByte(&command[2], method);
    command[4] = reinterpret_cast<uptr>(url);
    command[3] = ReadBufferDescriptor(urlSize);
    nn::Result result = nn::svc::SendSyncRequest(m_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pHandle = command[2];
    return nn::Result(command[1]);
}

// 0x0046F9B4 (name after 3dbrew)
nn::Result nn::http::ConnectionIpc::AddPostDataBinary(int handle, const char* label, size_t labelSize, const u8* data, size_t dataSize)
{
    return SendLabelCommand(m_Session, COMMAND_ADD_POST_DATA_BINARY, handle, label, labelSize, data, dataSize);
}

// 0x0046FA14 | fefates:bytes [tier B]
nn::Result nn::http::ConnectionIpc::DestroyConnection(int handle)
{
    return SendHandleCommand(m_Session, COMMAND_CLOSE_CONTEXT, handle);
}

// 0x0046FA44 | fefates:bytes [tier B]
nn::Result nn::http::ConnectionIpc::SetInternalRootCA(int handle, u32 certId)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_ADD_DEFAULT_CERT;
    command[1] = handle;
    command[2] = certId;
    return Send(m_Session, command);
}

// 0x0046FA78 | fefates:bytes [tier B]
nn::Result nn::http::ConnectionIpc::StartConnectionAsync(int handle)
{
    return SendHandleCommand(m_Session, COMMAND_BEGIN_REQUEST_ASYNC, handle);
}

// 0x0046FAA8 | fefates:bytes [tier B]
nn::Result nn::http::ConnectionIpc::GetConnectionProgress(int handle, u32* pReceived, u32* pTotal)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_DOWNLOAD_SIZE_STATE;
    command[1] = handle;
    nn::Result result = nn::svc::SendSyncRequest(m_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pReceived = command[2];
    *pTotal = command[3];
    return nn::Result(command[1]);
}

// 0x0046FAF0 | fefates:bytes [tier B]
nn::Result nn::http::ConnectionIpc::GetConnectionSslError(int handle, int* pError)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_SSL_ERROR;
    command[1] = handle;
    nn::Result result = nn::svc::SendSyncRequest(m_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pError = command[2];
    return nn::Result(command[1]);
}

// 0x0046FB2C | fefates:bytes [tier B]
nn::Result nn::http::ConnectionIpc::SetInternalClientCert(int handle, u32 certId)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_CLIENT_CERT_DEFAULT;
    command[1] = handle;
    command[2] = certId;
    return Send(m_Session, command);
}

// 0x0046FB60 | fefates:bytes [tier B]
nn::Result nn::http::ConnectionIpc::SetLazyPostDataSetting(int handle, PostDataType type)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_POST_DATA_TYPE;
    command[1] = handle;
    SetByte(&command[2], type);
    return Send(m_Session, command);
}

// 0x0046FB9C | tier C
nn::Result nn::http::ConnectionIpc::InitializeGeneralSession(nn::Handle sharedMemory, size_t size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_INITIALIZE;
    command[1] = size;
    command[5] = sharedMemory.GetPrintableBits();
    command[2] = DESCRIPTION_PROCESS_ID;
    command[4] = DESCRIPTION_HANDLE_COPY;
    return Send(m_Session, command);
}

// 0x0046FBE0 | fefates:bytes [tier B]
nn::Result nn::http::ConnectionIpc::NofityFinishSendPostData(int handle)
{
    return SendHandleCommand(m_Session, COMMAND_NOTIFY_FINISH_SEND_POST_DATA, handle);
}

// 0x0046FC10 | mk7dlp:callseq-callee [tier A]
nn::Result nn::http::ConnectionIpc::InitializeConnectionSession(int handle)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_INITIALIZE_CONNECTION_SESSION;
    command[1] = handle;
    command[2] = DESCRIPTION_PROCESS_ID;
    return Send(m_Session, command);
}

// 0x0046FC48 | fefates:bytes [tier B]
nn::Result nn::http::ConnectionIpc::SetLazyPostDataSettingWithSize(int handle, PostDataType type, size_t size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_POST_DATA_TYPE_SIZE;
    command[1] = handle;
    SetByte(&command[2], type);
    command[3] = size;
    return Send(m_Session, command);
}

// 0x0046FC88 | fefates:bytes [tier B]
nn::Result nn::http::ConnectionIpc::ReadBody(int handle, u8* pBuffer, size_t size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_RECEIVE_DATA;
    command[1] = handle;
    command[2] = size;
    command[3] = WriteBufferDescriptor(size);
    command[4] = reinterpret_cast<uptr>(pBuffer);
    return Send(m_Session, command);
}

// 0x0046FCCC | fefates:bytes [tier B]
nn::Result nn::http::ConnectionIpc::SetRootCA(int handle, const u8* cert, size_t size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_ADD_TRUSTED_ROOT_CA;
    command[1] = handle;
    command[2] = size;
    command[3] = ReadBufferDescriptor(size);
    command[4] = reinterpret_cast<uptr>(cert);
    return Send(m_Session, command);
}

} // namespace http
} // namespace nn
