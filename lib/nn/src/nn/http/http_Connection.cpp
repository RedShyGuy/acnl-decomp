#include "nn/util/ADLFireWall/util_NonCopyable.h"
#include "nn/http/http_Connection.h"

namespace nn {
namespace http {
// 0x0046F6B8 slot 0x00 | fefates:bytes-fuzzy
nn::http::Connection::~Connection()
{
}

// 0x0046F658 slot 0x04 | virtual slot, introduced by nn::http::Connection
void nn::http::Connection::vf_0x04()
{
}

// 0x0046F1A8 | mk7dlp:callseq [tier A]
void nn::http::Connection::Initialize(const char*, nn::http::RequestMethod, bool)
{
}

// 0x0046F4A0 | fefates:bytes [tier B]
void nn::http::Connection::SetLazyPostDataSetting(nn::http::PostDataType, unsigned int)
{
}

// 0x0046F554 | fefates:bytes [tier B]
void nn::http::Connection::Cancel()
{
}

// 0x0046F590 | fefates:bytes [tier B]
void nn::http::Connection::Finalize()
{
}

// 0x0046F5EC | mk7dlp:callgraph [tier A]
void nn::http::Connection::SetRootCa(const unsigned char*, unsigned)
{
}

// 0x0046F63C | mk7dlp:bytes [tier A]
nn::http::Connection::Connection()
{
}

// 0x00737168 | fefates:bytes [tier B]
void nn::http::Connection::GetProgress(unsigned int*, unsigned int*) const
{
}

// 0x007371C4 | fefates:bytes [tier B]
void nn::http::Connection::GetSslError(int*) const
{
}

// 0x00737218 | fefates:bytes [tier B]
void nn::http::Connection::GetStatusCode(int*) const
{
}

// 0x00737250 | fefates:bytes [tier B]
void nn::http::Connection::GetHeaderField(const char*, char*, unsigned int, unsigned int*) const
{
}

} // namespace http
} // namespace nn
