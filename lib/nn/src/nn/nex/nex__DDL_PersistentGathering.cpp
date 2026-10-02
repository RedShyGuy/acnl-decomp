#include "nn/nex/nex_Gathering.h"
#include "nn/nex/nex__DDL_PersistentGathering.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::_DDL_PersistentGathering::_DDL_PersistentGathering()
{
}

// 0x003B5F70 slot 0x00 | fefates:bytes
nn::nex::_DDL_PersistentGathering::~_DDL_PersistentGathering()
{
}

// 0x0072D608 slot 0x08 | fefates:callseq
void nn::nex::_DDL_PersistentGathering::Clone() const
{
}

// 0x0072D5A0 slot 0x0C | fefates:bytes
void nn::nex::_DDL_PersistentGathering::GetGatheringType() const
{
}

// 0x0072D5D0 slot 0x10 | virtual slot, introduced by nn::nex::_DDL_Gathering
void nn::nex::_DDL_PersistentGathering::vf_0x10()
{
}

// 0x0072D628 slot 0x14 | virtual slot, introduced by nn::nex::_DDL_Gathering
void nn::nex::_DDL_PersistentGathering::vf_0x14()
{
}

// 0x003B5BC4 slot 0x18 | slot vf_0x18 of nn::nex::_DDL_Gathering
void nn::nex::_DDL_PersistentGathering::StreamIn(nn::nex::Message*) const
{
}

// 0x003B5DAC slot 0x1C | slot vf_0x1C of nn::nex::_DDL_Gathering
void nn::nex::_DDL_PersistentGathering::StreamOut(nn::nex::Message*)
{
}

// 0x003B5DBC | fefates:bytes [tier B]
void nn::nex::_DDL_PersistentGathering::Extract(nn::nex::Message*, nn::nex::_DDL_PersistentGathering*)
{
}

} // namespace nex
} // namespace nn
