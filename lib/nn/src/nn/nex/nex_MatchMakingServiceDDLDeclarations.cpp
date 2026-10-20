#include "nn/nex/nex_MatchMakingServiceDDLDeclarations.h"
#include "nn/nex/nex_PseudoGlobalVariable.h"

namespace nn {
namespace nex {
// the declarations per context (name is ours)
// 0x00AE6B40 (name is ours)
PseudoGlobalVariable<MatchMakingServiceDDLDeclarations> g_MatchMakingServiceDDLDeclarations;

// 0x003CB0FC
nn::nex::MatchMakingServiceDDLDeclarations::MatchMakingServiceDDLDeclarations() : DDLDeclarations(true)
{
    // nothing to do
}

// 0x003CB128 slot 0x00
// 0x003CB118 (deleting dtor)
nn::nex::MatchMakingServiceDDLDeclarations::~MatchMakingServiceDDLDeclarations()
{
    // nothing to do
}

// 0x003CB0F8 slot 0x08
void nn::nex::MatchMakingServiceDDLDeclarations::Declare()
{
    // nothing to do
}

} // namespace nex
} // namespace nn

// ARMCC's static initializer of the file: the constructors of the globals above
// 0x0079E968 (static initializer)
