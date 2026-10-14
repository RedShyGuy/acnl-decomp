#include "nn/nex/nex_CompressionAlgorithm.h"

namespace nn {
namespace nex {
// 0x00396270 slot 0x00
// 0x00396218 (deleting dtor)
nn::nex::CompressionAlgorithm::~CompressionAlgorithm()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
