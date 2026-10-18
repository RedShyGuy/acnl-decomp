#include "nn/nex/nex_CompressionPlugin.h"

namespace nn {
namespace nex {
// 0x00386254 slot 0x1C (name is ours)
u32 nn::nex::CompressionPlugin::GetType()
{
    return TYPE_COMPRESSION;
}

} // namespace nex
} // namespace nn
