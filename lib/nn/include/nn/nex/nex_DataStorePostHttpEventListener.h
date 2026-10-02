#pragma once

#include "decomp.h"
#include "nn/nex/nex_HttpEventListener.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex30DataStorePostHttpEventListenerE @ 0x008CF190
// vtable 0x008FF384 (vptr 0x008FF38C), offset_to_top 0, 6 entries
class DataStorePostHttpEventListener : public ::nn::nex::HttpEventListener, public ::nn::nex::NonCopyable, public ::nn::nex::RootObject
{
public:
    DataStorePostHttpEventListener(); // ctor candidate(s) 0x003C23CC (unverified)
    virtual void vf_0x00(); // 0x003C2450 slot 0x00 | virtual slot, introduced by nn::nex::DataStorePostHttpEventListener
    virtual ~DataStorePostHttpEventListener(); // 0x003C2420 slot 0x04 | mk7dlp:bytes
    virtual void PrepareRequest(nn::nex::HttpConnection*); // 0x003C1D00 slot 0x08 | fefates:bytes-fuzzy
    virtual void PostChunkedBuffer(nn::nex::HttpConnection*, unsigned char*, unsigned int, bool*); // 0x003C20F8 slot 0x0C | fefates:bytes
    virtual void ProcessResponseHeader(nn::nex::HttpConnection*, int); // 0x00386D6C slot 0x10 | slot vf_0x10 of nn::nex::DataStorePostHttpEventListener
    virtual void ProcessResponse(const unsigned char*, unsigned int); // 0x00386D50 slot 0x14 | slot vf_0x14 of nn::nex::DataStorePostHttpEventListener
    DataStorePostHttpEventListener(unsigned, const nn::nex::qVector<nn::nex::DataStoreKeyValue>&, const nn::nex::qVector<nn::nex::DataStoreKeyValue>&, const nn::nex::qVector<unsigned char>&, unsigned, unsigned, const unsigned char*, unsigned, nn::nex::DataStorePostObjectEventListener*); // 0x003C23CC | mk7dlp:bytes [tier A]
};
} // namespace nex
} // namespace nn
