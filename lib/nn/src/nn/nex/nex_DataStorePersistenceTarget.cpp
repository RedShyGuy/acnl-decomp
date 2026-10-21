#include "nn/nex/nex_DataStorePersistenceTarget.h"

namespace nn {
namespace nex {
// 0x003B980C slot 0x00
// 0x003B9808 (deleting dtor)
nn::nex::DataStorePersistenceTarget::~DataStorePersistenceTarget()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
