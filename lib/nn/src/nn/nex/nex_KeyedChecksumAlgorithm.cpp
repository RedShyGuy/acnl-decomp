#include "nn/nex/nex_KeyedChecksumAlgorithm.h"

namespace nn {
namespace nex {
// 0x0039DA70
void nn::nex::KeyedChecksumAlgorithm::KeyHasChanged()
{
    // nothing to do: the algorithms with a key schedule override it
}

// 0x0039DA74 | fefates:bytes [tier B]
bool nn::nex::KeyedChecksumAlgorithm::SetKey(const nn::nex::Key& key)
{
    return SetKeyInline(key);
}

// 0x0039DAF4
// 0x0039DAC0 (deleting dtor)
nn::nex::KeyedChecksumAlgorithm::~KeyedChecksumAlgorithm()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
