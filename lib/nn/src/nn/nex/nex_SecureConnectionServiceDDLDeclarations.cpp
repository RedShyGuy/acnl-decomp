#include "nn/nex/nex_SecureConnectionServiceDDLDeclarations.h"
#include "nn/nex/nex_PseudoGlobalVariable.h"

namespace nn {
namespace nex {
// the declarations per context (name is ours)
// 0x00AE07E4 (name is ours)
PseudoGlobalVariable<SecureConnectionServiceDDLDeclarations> g_SecureConnectionServiceDDLDeclarations;

// 0x003CC464
nn::nex::SecureConnectionServiceDDLDeclarations::SecureConnectionServiceDDLDeclarations() : DDLDeclarations(true)
{
    // nothing to do
}

// 0x003CC490 slot 0x00
// 0x003CC480 (deleting dtor)
nn::nex::SecureConnectionServiceDDLDeclarations::~SecureConnectionServiceDDLDeclarations()
{
    // nothing to do
}

// 0x003CC460 slot 0x08
void nn::nex::SecureConnectionServiceDDLDeclarations::Declare()
{
    // nothing to do
}

} // namespace nex
} // namespace nn
