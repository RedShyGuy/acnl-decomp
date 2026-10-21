#include "nn/nex/nex_AuthenticationFoundationDDLDeclarations.h"
#include "nn/nex/nex_PseudoGlobalVariable.h"

namespace nn {
namespace nex {
// the declarations per context (name is ours)
// 0x00AE07B0 (name is ours)
PseudoGlobalVariable<AuthenticationFoundationDDLDeclarations> g_AuthenticationFoundationDDLDeclarations;

// 0x003CC4CC
nn::nex::AuthenticationFoundationDDLDeclarations::AuthenticationFoundationDDLDeclarations() : DDLDeclarations(true)
{
    // nothing to do
}

// 0x003CC4F8 slot 0x00
// 0x003CC4E8 (deleting dtor)
nn::nex::AuthenticationFoundationDDLDeclarations::~AuthenticationFoundationDDLDeclarations()
{
    // nothing to do
}

// 0x003CC4C8 slot 0x08
void nn::nex::AuthenticationFoundationDDLDeclarations::Declare()
{
    // nothing to do
}

} // namespace nex
} // namespace nn
