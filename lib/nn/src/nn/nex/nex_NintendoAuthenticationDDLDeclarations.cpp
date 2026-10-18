#include "nn/nex/nex_NintendoAuthenticationDDLDeclarations.h"
#include "nn/nex/nex_PseudoGlobalVariable.h"

namespace nn {
namespace nex {
// the declarations per context (name is ours)
// 0x00AE6A14 (name is ours)
PseudoGlobalVariable<NintendoAuthenticationDDLDeclarations> g_NintendoAuthenticationDDLDeclarations;

// 0x003CC36C
nn::nex::NintendoAuthenticationDDLDeclarations::NintendoAuthenticationDDLDeclarations() : DDLDeclarations(true)
{
    // nothing to do
}

// 0x003CC398 slot 0x00
// 0x003CC388 (deleting dtor)
nn::nex::NintendoAuthenticationDDLDeclarations::~NintendoAuthenticationDDLDeclarations()
{
    // nothing to do
}

// 0x003CC368 slot 0x08
void nn::nex::NintendoAuthenticationDDLDeclarations::Declare()
{
    // nothing to do
}

} // namespace nex
} // namespace nn
