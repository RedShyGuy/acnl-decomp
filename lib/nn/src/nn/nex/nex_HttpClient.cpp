#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_HttpConnection.h"
#include "nn/nex/nex_HttpClient.h"

namespace nn {
namespace nex {
// ctor candidate(s) 0x003550C0 (unverified)
nn::nex::HttpClient::HttpClient()
{
}

// 0x003550D0 slot 0x00 | virtual slot, introduced by nn::nex::HttpClient
void nn::nex::HttpClient::vf_0x00()
{
}

// 0x002F8500 slot 0x04 | fefates:bytes
nn::nex::HttpClient::~HttpClient()
{
}

// 0x00354F30 | fefates:bytes-fuzzy [tier B]
void nn::nex::HttpClient::Start(nn::nex::CallContext*, const nn::nex::String&, nn::nex::HttpConnection::Method, nn::nex::HttpEventListener*, unsigned int)
{
}

} // namespace nex
} // namespace nn
