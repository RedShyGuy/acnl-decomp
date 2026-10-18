#include "nn/nex/nex_Credentials.h"

namespace nn {
namespace nex {
// 0x00357650 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
// 0x003575E0 (deleting dtor)
nn::nex::Credentials::~Credentials()
{
    Clear();
}

// 0x00137B04 | mk7dlp:bytes [tier B]
void nn::nex::Credentials::MarkInvalid()
{
}

// 0x0072A100 | fefates:bytes [tier B]
nn::nex::EndPoint* nn::nex::Credentials::GetConnection(unsigned short streamType) const
{
    if (streamType == 0) {
        return nullptr;
    }
    return streamType == STREAM_TYPE_SECURE ? m_pSecureConnection : m_pConnection;
}

} // namespace nex
} // namespace nn
