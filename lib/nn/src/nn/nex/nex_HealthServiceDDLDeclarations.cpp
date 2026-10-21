#include "nn/nex/nex_HealthServiceDDLDeclarations.h"
#include "nn/nex/nex_PseudoGlobalVariable.h"

namespace nn {
namespace nex {
// the declarations per context (name is ours)
// 0x00AE0748 (name is ours)
PseudoGlobalVariable<HealthServiceDDLDeclarations> g_HealthServiceDDLDeclarations;

// 0x003BDA98
nn::nex::HealthServiceDDLDeclarations::HealthServiceDDLDeclarations() : DDLDeclarations(true)
{
    // nothing to do
}

// 0x003BDAC4 slot 0x00
// 0x003BDAB4 (deleting dtor)
nn::nex::HealthServiceDDLDeclarations::~HealthServiceDDLDeclarations()
{
    // nothing to do
}

// 0x003BDA94 slot 0x08
void nn::nex::HealthServiceDDLDeclarations::Declare()
{
    // nothing to do
}

} // namespace nex
} // namespace nn
