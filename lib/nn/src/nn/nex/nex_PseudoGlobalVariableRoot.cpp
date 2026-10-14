#include "nn/nex/nex_PseudoGlobalVariableRoot.h"

namespace nn {
namespace nex {
// 0x0096C998 (name is ours)
u32 PseudoGlobalVariableRoot::s_ContextCount;

// 0x003B572C | fefates:callgraph [tier C]
nn::nex::PseudoGlobalVariableRoot::PseudoGlobalVariableRoot() : m_pNext(nullptr)
{
}

// 0x003B5758 slot 0x00
// 0x003B5740 (deleting dtor)
nn::nex::PseudoGlobalVariableRoot::~PseudoGlobalVariableRoot()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
