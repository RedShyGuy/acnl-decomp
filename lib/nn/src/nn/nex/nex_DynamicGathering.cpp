#include "nn/nex/nex_DynamicGathering.h"

namespace nn {
namespace nex {
// 0x0037F188 slot 0x00
// 0x0037F140 (deleting dtor)
nn::nex::DynamicGathering::~DynamicGathering()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003D12D0 slot 0x0C
nn::nex::String nn::nex::DynamicGathering::GetGatheringType() const
{
    return m_GatheringType;
}

} // namespace nex
} // namespace nn
