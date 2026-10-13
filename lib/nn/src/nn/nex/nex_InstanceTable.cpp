#include "nn/nex/nex_InstanceTable.h"

namespace nn {
namespace nex {
// 0x00ADE85C (name is ours)
InstanceTable InstanceTable::s_Instance;
// 0x00ADE94C (name is ours)
CriticalSection InstanceTable::s_Lock(2);

// 0x00361D80
// 0x00361D04 (deleting dtor)
nn::nex::InstanceTable::~InstanceTable()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
