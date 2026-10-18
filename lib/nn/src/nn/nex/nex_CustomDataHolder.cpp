#include "nn/nex/nex_CustomDataHolder.h"
#include "nn/nex/nex_AuthenticationInfo.h"
#include "nn/nex/nex_UserMessage.h"

namespace nn {
namespace nex {
// 0x0037F0C4
// 0x0037F098 (deleting dtor)
template CustomDataHolder<UserMessage>::~CustomDataHolder();

// 0x0037F118
// 0x0037F0EC (deleting dtor)
template CustomDataHolder<AuthenticationInfo>::~CustomDataHolder();

} // namespace nex
} // namespace nn
