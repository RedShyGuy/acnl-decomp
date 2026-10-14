#include "nn/nex/nex_LiteByteStream.h"

namespace nn {
namespace nex {
// (the instances: the destructors and Append of the 52 bytes are out of line)
// 0x007D5E00
// 0x007D5DDC (deleting dtor)
template LiteByteStream<6>::~LiteByteStream();
// 0x007D5D1C
// 0x007D5CF8 (deleting dtor)
template LiteByteStream<20>::~LiteByteStream();
// 0x007D5DCC
// 0x007D5DA8 (deleting dtor)
template LiteByteStream<52>::~LiteByteStream();
// 0x007D5D2C
template bool LiteByteStream<52>::Append(const unsigned char& value);

} // namespace nex
} // namespace nn
