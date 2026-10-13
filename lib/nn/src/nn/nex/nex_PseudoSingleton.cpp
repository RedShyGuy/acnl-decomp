#include "nn/nex/nex_PseudoSingleton.h"

namespace nn {
namespace nex {
// 0x0096C916 (name is ours)
bool PseudoSingleton::s_IsContextUsed;
// 0x0096C970 (name is ours)
ThreadVariable<u32>* PseudoSingleton::s_pCurrentContext;

// 0x0037AEA8 | fefates:callseq [tier B]
bool nn::nex::PseudoSingleton::SetContext(unsigned int context)
{
    if (!s_IsContextUsed) {
        return false;
    }
    s_pCurrentContext->Set(context);
    return true;
}

// 0x0037AF48
nn::nex::PseudoSingleton::PseudoSingleton(u32 kind) : InstanceControl(kind, GetCurrentContext())
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x00378640
// 0x0037AF94 (deleting dtor)
nn::nex::PseudoSingleton::~PseudoSingleton()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
