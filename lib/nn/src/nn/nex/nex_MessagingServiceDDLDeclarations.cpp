#include "nn/nex/nex_MessagingServiceDDLDeclarations.h"
#include "nn/nex/nex_PseudoGlobalVariable.h"

namespace nn {
namespace nex {
// the declarations per context (name is ours)
// 0x00ADDBC4 (name is ours)
PseudoGlobalVariable<MessagingServiceDDLDeclarations> g_MessagingServiceDDLDeclarations;

// 0x003C6B0C
nn::nex::MessagingServiceDDLDeclarations::MessagingServiceDDLDeclarations() : DDLDeclarations(true)
{
    // nothing to do
}

// 0x003C6B38 slot 0x00
// 0x003C6B28 (deleting dtor)
nn::nex::MessagingServiceDDLDeclarations::~MessagingServiceDDLDeclarations()
{
    // nothing to do
}

// 0x003C6B08 slot 0x08
void nn::nex::MessagingServiceDDLDeclarations::Declare()
{
    // nothing to do
}

} // namespace nex
} // namespace nn
