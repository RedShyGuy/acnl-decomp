#include "nn/nex/nex_ProtocolFoundationDDLDeclarations.h"
#include "nn/nex/nex_PseudoGlobalVariable.h"

namespace nn {
namespace nex {
// the declarations per context (name is ours)
// 0x00AE077C (name is ours)
PseudoGlobalVariable<ProtocolFoundationDDLDeclarations> g_ProtocolFoundationDDLDeclarations;

// 0x003CB164
nn::nex::ProtocolFoundationDDLDeclarations::ProtocolFoundationDDLDeclarations() : DDLDeclarations(true)
{
    // nothing to do
}

// 0x003CB190 slot 0x00
// 0x003CB180 (deleting dtor)
nn::nex::ProtocolFoundationDDLDeclarations::~ProtocolFoundationDDLDeclarations()
{
    // nothing to do
}

// 0x003CB160 slot 0x08
void nn::nex::ProtocolFoundationDDLDeclarations::Declare()
{
    // nothing to do
}

} // namespace nex
} // namespace nn
