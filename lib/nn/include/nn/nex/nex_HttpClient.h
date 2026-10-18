#pragma once

#include "decomp.h"
#include "nn/nex/nex_HttpConnection.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex10HttpClientE @ 0x008CDF48
// vtable 0x008FC140 (vptr 0x008FC148), offset_to_top 0, 2 entries
class CallContext;
class HttpEventListener;
class String;

// Starts HTTP requests (JobHttp).
class HttpClient : public ::nn::nex::RootObject, public ::nn::nex::NonCopyable
{
public:
    HttpClient(); // 0x003550C0
    virtual ~HttpClient(); // 0x003550D0 slot 0x00
    // 0x002F8500 slot 0x04 (deleting dtor)
    // the request runs in a JobHttp (chunkSize: of the post data and of the received parts)
    nn::nex::qResult Start(nn::nex::CallContext* pContext, const nn::nex::String& url, nn::nex::HttpConnection::Method method, nn::nex::HttpEventListener* pListener,
                           unsigned int chunkSize); // 0x00354F30 | fefates:bytes-fuzzy [tier B]
};
} // namespace nex
} // namespace nn
