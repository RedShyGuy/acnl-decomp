#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_HttpConnection.h"
#include "nn/nex/nex_DataStoreFileServerClient.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::DataStoreFileServerClient::DataStoreFileServerClient()
{
}

// 0x003B6188 slot 0x00 | virtual slot, introduced by nn::nex::DataStoreFileServerClient
void nn::nex::DataStoreFileServerClient::vf_0x00()
{
}

// 0x003B6184 slot 0x04 | virtual slot, introduced by nn::nex::DataStoreFileServerClient
void nn::nex::DataStoreFileServerClient::vf_0x04()
{
}

// 0x003B5FA8 | fefates:bytes [tier B]
void nn::nex::DataStoreFileServerClient::ApiImpl(nn::nex::CallContext*, const nn::nex::String&, nn::nex::HttpEventListener*, nn::nex::HttpConnection::Method, unsigned int)
{
}

} // namespace nex
} // namespace nn
