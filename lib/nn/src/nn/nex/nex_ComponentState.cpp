#include "nn/nex/nex_ComponentState.h"

#include <cwchar>

namespace nn {
namespace nex {
// 0x00370F64 slot 0x10
void nn::nex::ComponentState::vf_0x10()
{
    // nothing to do
}

// 0x00370FC0 slot 0x00
// 0x00370F68 (deleting dtor)
nn::nex::ComponentState::~ComponentState()
{
    // nothing to do: the bases are destroyed by the compiler
}

// 0x0072AF00 slot 0x08
const wchar_t* nn::nex::ComponentState::GetType() const
{
    return L"ComponentState";
}

// 0x0072AF28 slot 0x0C
bool nn::nex::ComponentState::IsAKindOf(const wchar_t* pType) const
{
    if (wcscmp(L"ComponentState", pType) == 0) {
        return true;
    }
    return wcscmp(L"SystemComponent", pType) == 0;
}

} // namespace nex
} // namespace nn
