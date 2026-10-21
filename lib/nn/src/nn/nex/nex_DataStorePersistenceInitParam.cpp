#include "nn/nex/nex_DataStorePersistenceInitParam.h"

namespace nn {
namespace nex {
// 0x003BF7A4 slot 0x00
// 0x003BF7A0 (deleting dtor)
nn::nex::DataStorePersistenceInitParam::~DataStorePersistenceInitParam()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
