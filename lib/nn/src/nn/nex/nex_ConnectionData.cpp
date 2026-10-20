#include "nn/nex/nex_ConnectionData.h"

namespace nn {
namespace nex {
// 0x00371020 slot 0x00
// 0x00371000 (deleting dtor)
nn::nex::ConnectionData::~ConnectionData()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003CF3A0 (name is ours)
template <>
DECOMP_NOINLINE qList<ConnectionData>::qList()
{
    // nothing to do: std::list makes the head node
}

// 0x003CF420 (name is ours; it calls the destructor of std::list)
template qList<ConnectionData>::~qList();

} // namespace nex
} // namespace nn

namespace std {
// 0x0082F330 (name is ours; std::list::~list, the destructor of the qList calls it)
template <>
DECOMP_NOINLINE DECOMP_NOIPA list<nn::nex::ConnectionData, nn::nex::MemAllocator<nn::nex::ConnectionData> >::~list()
{
    DestroyInline();
}
} // namespace std
