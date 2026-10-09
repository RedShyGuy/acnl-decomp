#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"
#include "nn/http/http_Types.h"

namespace nn {
namespace http {
// The commands of http:C (3dbrew "HTTP Services") on one session. The class name and the method
// names are from the symbols (the 3dbrew names of the commands without symbols); the parameter
// names are ours.
class ConnectionIpc
{
public:
    ConnectionIpc() {}

    nn::Result InitializeGeneralSession(nn::Handle sharedMemory, size_t size); // 0x0046FB9C | tier C
    nn::Result CreateConnection(const char* url, size_t urlSize, RequestMethod method, int* pHandle); // 0x0046F95C | mk7dlp:callseq-callee [tier A]
    nn::Result DestroyConnection(int handle); // 0x0046FA14 | fefates:bytes [tier B]
    nn::Result CancelConnection(int handle); // 0x0046F92C | fefates:bytes [tier B]
    nn::Result GetConnectionProgress(int handle, u32* pReceived, u32* pTotal); // 0x0046FAA8 | fefates:bytes [tier B]
    nn::Result InitializeConnectionSession(int handle); // 0x0046FC10 | mk7dlp:callseq-callee [tier A]
    nn::Result StartConnectionAsync(int handle); // 0x0046FA78 | fefates:bytes [tier B]
    nn::Result ReadBody(int handle, u8* pBuffer, size_t size); // 0x0046FC88 | fefates:bytes [tier B]
    nn::Result SetProxyDefault(int handle); // 0x0046F89C | mk7dlp:callseq-callee [tier A]
    nn::Result AddHeaderField(int handle, const char* label, size_t labelSize, const char* value, size_t valueSize); // 0x0046F764 | fefates:bytes [tier B]
    nn::Result AddPostDataAscii(int handle, const char* label, size_t labelSize, const char* value, size_t valueSize); // 0x0046F8CC (name after 3dbrew)
    nn::Result AddPostDataBinary(int handle, const char* label, size_t labelSize, const u8* data, size_t dataSize); // 0x0046F9B4 (name after 3dbrew)
    nn::Result SetLazyPostDataSetting(int handle, PostDataType type); // 0x0046FB60 | fefates:bytes [tier B]
    nn::Result SendPostDataRaw(int handle, const u8* data, size_t size); // 0x0046F858 | fefates:bytes [tier B]
    nn::Result NofityFinishSendPostData(int handle); // 0x0046FBE0 | fefates:bytes [tier B]
    nn::Result GetHeaderField(int handle, const char* label, size_t labelSize, char* value, size_t valueSize, size_t* pValueSize); // 0x0046F7EC | fefates:bytes [tier B]
    nn::Result GetResultCode(int handle, int* pStatusCode); // 0x0046F728 | fefates:bytes [tier B]
    nn::Result SetRootCA(int handle, const u8* cert, size_t size); // 0x0046FCCC | fefates:bytes [tier B]
    nn::Result SetInternalRootCA(int handle, u32 certId); // 0x0046FA44 | fefates:bytes [tier B]
    nn::Result SetInternalClientCert(int handle, u32 certId); // 0x0046FB2C | fefates:bytes [tier B]
    nn::Result GetConnectionSslError(int handle, int* pError); // 0x0046FAF0 | fefates:bytes [tier B]
    nn::Result FinalizeClient(); // 0x0046F7C4 | fefates:bytes [tier B]
    nn::Result SetLazyPostDataSettingWithSize(int handle, PostDataType type, size_t size); // 0x0046FC48 | fefates:bytes [tier B]

    nn::Handle m_Session; // 0x0 (name is ours)
};
ASSERT_SIZE(ConnectionIpc, 0x4);
} // namespace http
} // namespace nn
