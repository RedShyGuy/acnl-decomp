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
DECOMP_NOINLINE qList<ConnectionData>::qList() : m_pBufferList(nullptr), m_pFreeList(nullptr), m_pNextAvail(nullptr), m_pLast(nullptr), m_pHead(nullptr), m_Size(0)
{
    InitializeInline();
}

// 0x0082F330 (name is ours; std::list::~list, the destructor of the qList calls it)
template <>
DECOMP_NOINLINE qList<ConnectionData>::~qList()
{
    DestroyInline();
}

} // namespace nex
} // namespace nn
