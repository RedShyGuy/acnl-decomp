#include "nn/nex/nex_Stream.h"

namespace nn {
namespace nex {
// 0x00ADE978 (name is ours)
PseudoGlobalVariable<StreamSettings> Stream::s_Settings[TYPE_COUNT];

// 0x003D113C slot 0x00
// 0x003D1124 (deleting dtor)
nn::nex::Stream::~Stream()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003D111C
bool nn::nex::Stream::IsDuplicateReorderingPacket(nn::nex::Packet*)
{
    return false;
}

// 0x003D1060 | fefates:callgraph [tier C]
nn::nex::StreamSettings* nn::nex::Stream::GetSettings(nn::nex::Stream::Type type)
{
    return &s_Settings[type].Get();
}

// 0x003D10BC | fefates:callgraph [tier C]
nn::nex::StreamSettings* nn::nex::Stream::GetThisSettings()
{
    return &s_Settings[m_Type].Get();
}

// the copies of the inline functions of the settings (in the vtable)
// 0x007D7BB8 (name is ours)
template PseudoGlobalVariable<StreamSettings>::PseudoGlobalVariable();
// 0x007D7E34 slot 0x00
// 0x007D7D34 (deleting dtor)
template PseudoGlobalVariable<StreamSettings>::~PseudoGlobalVariable();
// 0x007D7944 slot 0x08
template void PseudoGlobalVariable<StreamSettings>::AllocateValues();
// 0x007D78BC slot 0x0C
template void PseudoGlobalVariable<StreamSettings>::FreeValues();
// 0x007D7788 slot 0x10
template void PseudoGlobalVariable<StreamSettings>::ResetValue(u32 context);
// 0x007D7BA8 slot 0x14
template PseudoGlobalVariableRoot* PseudoGlobalVariable<StreamSettings>::GetNext();
// 0x007D7BB0 slot 0x18
template void PseudoGlobalVariable<StreamSettings>::SetNext(PseudoGlobalVariableRoot* pNext);

} // namespace nex
} // namespace nn
