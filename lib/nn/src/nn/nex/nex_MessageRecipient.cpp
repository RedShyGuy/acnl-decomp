#include "nn/nex/nex_MessageRecipient.h"

namespace nn {
namespace nex {
// 0x00382714 slot 0x00
// 0x00382710 (deleting dtor)
nn::nex::MessageRecipient::~MessageRecipient()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
