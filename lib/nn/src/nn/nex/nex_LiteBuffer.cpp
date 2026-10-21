#include "nn/nex/nex_LiteBuffer.h"

namespace nn {
namespace nex {
// (the instances: only their destructors are out of line)
// 0x007D3C78
// 0x007D3C60 (deleting dtor)
template LiteBuffer<6>::~LiteBuffer();
// 0x007D3C94
// 0x007D3C7C (deleting dtor)
template LiteBuffer<8>::~LiteBuffer();
// 0x007D3C08
// 0x007D3BF0 (deleting dtor)
template LiteBuffer<16>::~LiteBuffer();
// 0x007D3C24
// 0x007D3C0C (deleting dtor)
template LiteBuffer<20>::~LiteBuffer();
// 0x007D3C40
// 0x007D3C28 (deleting dtor)
template LiteBuffer<32>::~LiteBuffer();
// 0x007D3C5C
// 0x007D3C44 (deleting dtor)
template LiteBuffer<52>::~LiteBuffer();

} // namespace nex
} // namespace nn
