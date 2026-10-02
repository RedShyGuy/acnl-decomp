#include "nn/http/http_ConnectionIpc.h"

namespace nn {
namespace http {
// 0x0046F728 | fefates:bytes [tier B]
void nn::http::ConnectionIpc::GetResultCode(int, int*)
{
}

// 0x0046F764 | fefates:bytes [tier B]
void nn::http::ConnectionIpc::AddHeaderField(int, const char*, unsigned int, const char*, unsigned int)
{
}

// 0x0046F7C4 | fefates:bytes [tier B]
void nn::http::ConnectionIpc::FinalizeClient()
{
}

// 0x0046F7EC | fefates:bytes [tier B]
void nn::http::ConnectionIpc::GetHeaderField(int, const char*, unsigned int, char*, unsigned int, unsigned int*)
{
}

// 0x0046F858 | fefates:bytes [tier B]
void nn::http::ConnectionIpc::SendPostDataRaw(int, const unsigned char*, unsigned int)
{
}

// 0x0046F89C | mk7dlp:callseq-callee [tier A]
void nn::http::ConnectionIpc::SetProxyDefault(int)
{
}

// 0x0046F92C | fefates:bytes [tier B]
void nn::http::ConnectionIpc::CancelConnection(int)
{
}

// 0x0046F95C | mk7dlp:callseq-callee [tier A]
void nn::http::ConnectionIpc::CreateConnection(const char*, unsigned, nn::http::RequestMethod, int*)
{
}

// 0x0046FA14 | fefates:bytes [tier B]
void nn::http::ConnectionIpc::DestroyConnection(int)
{
}

// 0x0046FA44 | fefates:bytes [tier B]
void nn::http::ConnectionIpc::SetInternalRootCA(int, unsigned int)
{
}

// 0x0046FA78 | fefates:bytes [tier B]
void nn::http::ConnectionIpc::StartConnectionAsync(int)
{
}

// 0x0046FAA8 | fefates:bytes [tier B]
void nn::http::ConnectionIpc::GetConnectionProgress(int, unsigned int*, unsigned int*)
{
}

// 0x0046FAF0 | fefates:bytes [tier B]
void nn::http::ConnectionIpc::GetConnectionSslError(int, int*)
{
}

// 0x0046FB2C | fefates:bytes [tier B]
void nn::http::ConnectionIpc::SetInternalClientCert(int, unsigned int)
{
}

// 0x0046FB60 | fefates:bytes [tier B]
void nn::http::ConnectionIpc::SetLazyPostDataSetting(int, nn::http::PostDataType)
{
}

// 0x0046FBE0 | fefates:bytes [tier B]
void nn::http::ConnectionIpc::NofityFinishSendPostData(int)
{
}

// 0x0046FC10 | mk7dlp:callseq-callee [tier A]
void nn::http::ConnectionIpc::InitializeConnectionSession(int)
{
}

// 0x0046FC48 | fefates:bytes [tier B]
void nn::http::ConnectionIpc::SetLazyPostDataSettingWithSize(int, nn::http::PostDataType, unsigned int)
{
}

// 0x0046FC88 | fefates:bytes [tier B]
void nn::http::ConnectionIpc::ReadBody(int, unsigned char*, unsigned int)
{
}

// 0x0046FCCC | fefates:bytes [tier B]
void nn::http::ConnectionIpc::SetRootCA(int, const unsigned char*, unsigned int)
{
}

} // namespace http
} // namespace nn
