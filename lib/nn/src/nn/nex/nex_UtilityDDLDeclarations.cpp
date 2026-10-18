#include "nn/nex/nex_UtilityDDLDeclarations.h"
#include "nn/nex/nex_PseudoGlobalVariable.h"

namespace nn {
namespace nex {
// the declarations per context (name is ours)
// 0x00ADDB48 (name is ours)
PseudoGlobalVariable<UtilityDDLDeclarations> g_UtilityDDLDeclarations;

// 0x0039ED2C
nn::nex::UtilityDDLDeclarations::UtilityDDLDeclarations() : DDLDeclarations(true)
{
    // nothing to do
}

// 0x00377648 slot 0x00
// 0x0039ED48 (deleting dtor)
nn::nex::UtilityDDLDeclarations::~UtilityDDLDeclarations()
{
    // nothing to do
}

// 0x0039ED28 slot 0x08
void nn::nex::UtilityDDLDeclarations::Declare()
{
    // nothing to do
}

} // namespace nex
} // namespace nn
