#include "nn/nex/nex_AccountManagementServiceDDLDeclarations.h"
#include "nn/nex/nex_PseudoGlobalVariable.h"

namespace nn {
namespace nex {
// the declarations per context (name is ours)
// 0x00AE0590 (name is ours)
PseudoGlobalVariable<AccountManagementServiceDDLDeclarations> g_AccountManagementServiceDDLDeclarations;

// 0x003CC498
nn::nex::AccountManagementServiceDDLDeclarations::AccountManagementServiceDDLDeclarations() : DDLDeclarations(true)
{
    // nothing to do
}

// 0x003CC4C4 slot 0x00
// 0x003CC4B4 (deleting dtor)
nn::nex::AccountManagementServiceDDLDeclarations::~AccountManagementServiceDDLDeclarations()
{
    // nothing to do
}

// 0x003CC494 slot 0x08
void nn::nex::AccountManagementServiceDDLDeclarations::Declare()
{
    // nothing to do
}

} // namespace nex
} // namespace nn
