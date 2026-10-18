#include "nn/nex/nex_NATTraversalDDLDeclarations.h"
#include "nn/nex/nex_PseudoGlobalVariable.h"

namespace nn {
namespace nex {
// the declarations per context (name is ours)
// 0x00AE6BCC (name is ours)
PseudoGlobalVariable<NATTraversalDDLDeclarations> g_NATTraversalDDLDeclarations;

// 0x003BD2BC
nn::nex::NATTraversalDDLDeclarations::NATTraversalDDLDeclarations() : DDLDeclarations(true)
{
    // nothing to do
}

// 0x003BD2E8 slot 0x00
// 0x003BD2D8 (deleting dtor)
nn::nex::NATTraversalDDLDeclarations::~NATTraversalDDLDeclarations()
{
    // nothing to do
}

// 0x003BD2B8 slot 0x08
void nn::nex::NATTraversalDDLDeclarations::Declare()
{
    // nothing to do
}

} // namespace nex
} // namespace nn
