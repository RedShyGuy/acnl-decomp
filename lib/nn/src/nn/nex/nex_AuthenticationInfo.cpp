#include "nn/nex/nex_AuthenticationInfo.h"

namespace nn {
namespace nex {
// 0x00387420 slot 0x00
// 0x003873FC (deleting dtor)
nn::nex::AuthenticationInfo::~AuthenticationInfo()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
