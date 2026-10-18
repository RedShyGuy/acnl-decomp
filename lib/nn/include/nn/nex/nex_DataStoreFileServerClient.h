#pragma once

#include "decomp.h"
#include "nn/nex/nex_HttpConnection.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_qResult.h"

namespace nn {
namespace nex {
class CallContext;
class HttpEventListener;
class String;

// The client of the DataStore file server: the requests run in a JobDataStoreFileServer.
// RTTI N2nn3nex25DataStoreFileServerClientE @ 0x008CEE68
// vtable 0x008FE9B4 (vptr 0x008FE9BC), offset_to_top 0, 2 entries
class DataStoreFileServerClient : public ::nn::nex::RootObject, public ::nn::nex::NonCopyable
{
public:
    // (inline)
    DataStoreFileServerClient() {}
    virtual ~DataStoreFileServerClient(); // 0x003B6188 slot 0x00
    // 0x003B6184 (deleting dtor)
    nn::nex::qResult ApiImpl(nn::nex::CallContext* pContext, const nn::nex::String& url, nn::nex::HttpEventListener* pListener, nn::nex::HttpConnection::Method method,
                             unsigned int chunkSize); // 0x003B5FA8 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
