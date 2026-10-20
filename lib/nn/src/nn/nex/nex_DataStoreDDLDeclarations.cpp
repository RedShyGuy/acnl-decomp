#include "nn/nex/nex_DataStoreDDLDeclarations.h"
#include "nn/nex/nex_PseudoGlobalVariable.h"

namespace nn {
namespace nex {
// the declarations per context (name is ours)
// 0x00ADD9FC (name is ours)
PseudoGlobalVariable<DataStoreDDLDeclarations> g_DataStoreDDLDeclarations;

// 0x003B22F0
nn::nex::DataStoreDDLDeclarations::DataStoreDDLDeclarations() : DDLDeclarations(true)
{
    // nothing to do
}

// 0x003B231C slot 0x00
// 0x003B230C (deleting dtor)
nn::nex::DataStoreDDLDeclarations::~DataStoreDDLDeclarations()
{
    // nothing to do
}

// 0x003B22EC slot 0x08
void nn::nex::DataStoreDDLDeclarations::Declare()
{
    // nothing to do
}

} // namespace nex
} // namespace nn

// ARMCC's static initializer of the file: the constructors of the globals above
// 0x0079F69C (static initializer)
