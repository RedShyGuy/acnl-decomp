#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_HttpEventListener.h"
#include "nn/nex/nex_DataStorePostHttpEventListener.h"

namespace nn {
namespace nex {
// ctor candidate(s) 0x003C23CC (unverified)
nn::nex::DataStorePostHttpEventListener::DataStorePostHttpEventListener()
{
}

// 0x003C2450 slot 0x00 | virtual slot, introduced by nn::nex::DataStorePostHttpEventListener
void nn::nex::DataStorePostHttpEventListener::vf_0x00()
{
}

// 0x003C2420 slot 0x04 | mk7dlp:bytes
nn::nex::DataStorePostHttpEventListener::~DataStorePostHttpEventListener()
{
}

// 0x003C1D00 slot 0x08 | fefates:bytes-fuzzy
void nn::nex::DataStorePostHttpEventListener::PrepareRequest(nn::nex::HttpConnection*)
{
}

// 0x003C20F8 slot 0x0C | fefates:bytes
void nn::nex::DataStorePostHttpEventListener::PostChunkedBuffer(nn::nex::HttpConnection*, unsigned char*, unsigned int, bool*)
{
}

// 0x00386D6C slot 0x10 | slot vf_0x10 of nn::nex::DataStorePostHttpEventListener
void nn::nex::DataStorePostHttpEventListener::ProcessResponseHeader(nn::nex::HttpConnection*, int)
{
}

// 0x00386D50 slot 0x14 | slot vf_0x14 of nn::nex::DataStorePostHttpEventListener
void nn::nex::DataStorePostHttpEventListener::ProcessResponse(const unsigned char*, unsigned int)
{
}

// 0x003C23CC | mk7dlp:bytes [tier A]
nn::nex::DataStorePostHttpEventListener::DataStorePostHttpEventListener(unsigned, const nn::nex::qVector<nn::nex::DataStoreKeyValue>&, const nn::nex::qVector<nn::nex::DataStoreKeyValue>&, const nn::nex::qVector<unsigned char>&, unsigned, unsigned, const unsigned char*, unsigned, nn::nex::DataStorePostObjectEventListener*)
{
}

} // namespace nex
} // namespace nn
