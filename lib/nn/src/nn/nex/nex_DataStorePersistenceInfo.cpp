#include "nn/nex/nex_DataStorePersistenceInfo.h"

namespace nn {
namespace nex {
// 0x003B2344 slot 0x00
// 0x003B2340 (deleting dtor)
nn::nex::DataStorePersistenceInfo::~DataStorePersistenceInfo()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
