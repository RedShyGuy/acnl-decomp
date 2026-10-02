#pragma once

#include "decomp.h"
#include "nn/nex/nex_HttpConnection.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex25DataStoreFileServerClientE @ 0x008CEE68
// vtable 0x008FE9B4 (vptr 0x008FE9BC), offset_to_top 0, 2 entries
class DataStoreFileServerClient : public ::nn::nex::RootObject, public ::nn::nex::NonCopyable
{
public:
    DataStoreFileServerClient(); // ctor address unknown
    virtual void vf_0x00(); // 0x003B6188 slot 0x00 | virtual slot, introduced by nn::nex::DataStoreFileServerClient
    virtual void vf_0x04(); // 0x003B6184 slot 0x04 | virtual slot, introduced by nn::nex::DataStoreFileServerClient
    void ApiImpl(nn::nex::CallContext*, const nn::nex::String&, nn::nex::HttpEventListener*, nn::nex::HttpConnection::Method, unsigned int); // 0x003B5FA8 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
