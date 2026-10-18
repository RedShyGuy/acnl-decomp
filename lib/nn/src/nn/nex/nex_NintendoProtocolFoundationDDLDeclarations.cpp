#include "nn/nex/nex_NintendoProtocolFoundationDDLDeclarations.h"
#include "nn/nex/nex_PseudoGlobalVariable.h"

namespace nn {
namespace nex {
// the declarations per context (name is ours)
// 0x00AE0818 (name is ours)
PseudoGlobalVariable<NintendoProtocolFoundationDDLDeclarations> g_NintendoProtocolFoundationDDLDeclarations;

// 0x003CD1F4
nn::nex::NintendoProtocolFoundationDDLDeclarations::NintendoProtocolFoundationDDLDeclarations() : DDLDeclarations(true)
{
    // nothing to do
}

// 0x003CD220 slot 0x00
// 0x003CD210 (deleting dtor)
nn::nex::NintendoProtocolFoundationDDLDeclarations::~NintendoProtocolFoundationDDLDeclarations()
{
    // nothing to do
}

// 0x003CD1F0 slot 0x08
void nn::nex::NintendoProtocolFoundationDDLDeclarations::Declare()
{
    // nothing to do
}

} // namespace nex
} // namespace nn
