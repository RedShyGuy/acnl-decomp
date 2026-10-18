#include "nn/nex/nex_MatchmakeExtensionDDLDeclarations.h"
#include "nn/nex/nex_PseudoGlobalVariable.h"

namespace nn {
namespace nex {
// the declarations per context (name is ours)
// 0x00ADDAF0 (name is ours)
PseudoGlobalVariable<MatchmakeExtensionDDLDeclarations> g_MatchmakeExtensionDDLDeclarations;

// 0x003CB130
nn::nex::MatchmakeExtensionDDLDeclarations::MatchmakeExtensionDDLDeclarations() : DDLDeclarations(true)
{
    // nothing to do
}

// 0x003CB15C slot 0x00
// 0x003CB14C (deleting dtor)
nn::nex::MatchmakeExtensionDDLDeclarations::~MatchmakeExtensionDDLDeclarations()
{
    // nothing to do
}

// 0x003CB12C slot 0x08
void nn::nex::MatchmakeExtensionDDLDeclarations::Declare()
{
    // nothing to do
}

} // namespace nex
} // namespace nn
