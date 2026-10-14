#include "nn/nex/nex_AnyObjectHolder.h"
#include "nn/nex/nex_Data.h"
#include "nn/nex/nex_Gathering.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
// the copies of the inline functions that armlink placed at the end of the code
// 0x007D6E48 slot 0x00
// 0x007D6E1C (deleting dtor)
template AnyObjectHolder<Data, String>::~AnyObjectHolder();
// 0x007D6EF4 slot 0x00
// 0x007D6EC8 (deleting dtor)
template AnyObjectHolder<Gathering, String>::~AnyObjectHolder();
// 0x007D6E70 (name is ours)
template void AnyObjectHolder<Gathering, String>::StreamOut(Message* pMessage);
} // namespace nex
} // namespace nn
