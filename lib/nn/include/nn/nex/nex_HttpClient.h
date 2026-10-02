#pragma once

#include "decomp.h"
#include "nn/nex/nex_HttpConnection.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex10HttpClientE @ 0x008CDF48
// vtable 0x008FC140 (vptr 0x008FC148), offset_to_top 0, 2 entries
class HttpClient : public ::nn::nex::RootObject, public ::nn::nex::NonCopyable
{
public:
    HttpClient(); // ctor candidate(s) 0x003550C0 (unverified)
    virtual void vf_0x00(); // 0x003550D0 slot 0x00 | virtual slot, introduced by nn::nex::HttpClient
    virtual ~HttpClient(); // 0x002F8500 slot 0x04 | fefates:bytes
    void Start(nn::nex::CallContext*, const nn::nex::String&, nn::nex::HttpConnection::Method, nn::nex::HttpEventListener*, unsigned int); // 0x00354F30 | fefates:bytes-fuzzy [tier B]
};
} // namespace nex
} // namespace nn
