#include "nn/nex/nex_String.h"
#include "nn/nex/nex_Data.h"
#include "nn/nex/nex_AnyObjectAdapter.h"
#include "nn/nex/nex_AnyDataAdapter.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::AnyDataAdapter::AnyDataAdapter()
{
}

// 0x00370800 slot 0x00 | fefates:bytes
nn::nex::AnyDataAdapter::~AnyDataAdapter()
{
}

// 0x003707C8 slot 0x04 | virtual slot, introduced by nn::nex::AnyDataAdapter
void nn::nex::AnyDataAdapter::vf_0x04()
{
}

// 0x0072ABF0 slot 0x08 | fefates:bytes
void nn::nex::AnyDataAdapter::StreamIn(nn::nex::Message*, const nn::nex::Data&) const
{
}

// 0x0072AC9C slot 0x0C | fefates:bytes
void nn::nex::AnyDataAdapter::StreamOut(nn::nex::Message*, nn::nex::Data**) const
{
}

// 0x003705E8 | fefates:bytes [tier B]
void nn::nex::AnyDataAdapter::UnregisterAllPrototypes()
{
}

// 0x0072AB70 | fefates:bytes [tier B]
void nn::nex::AnyDataAdapter::FindPrototype(const nn::nex::String&) const
{
}

} // namespace nex
} // namespace nn
