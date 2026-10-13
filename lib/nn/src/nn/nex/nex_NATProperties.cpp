#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_NATProperties.h"

namespace nn {
namespace nex {
// 0x0036208C slot 0x00 | mk7dlp:bytes
// 0x00362050 (deleting dtor)
nn::nex::NATProperties::~NATProperties()
{
}

// 0x00361FF0 | fefates:bytes [tier B]
nn::nex::NATProperties::NATProperties()
{
}

// 0x00361FE8 (name is ours)
void nn::nex::NATProperties::SetPrivateAddress(const nn::nex::String& address)
{
    m_PrivateAddress = address;
}

// 0x003D147C (name is ours)
void nn::nex::NATProperties::SetPublicAddress(const nn::nex::String& address)
{
    m_PublicAddress = address;
}

} // namespace nex
} // namespace nn
