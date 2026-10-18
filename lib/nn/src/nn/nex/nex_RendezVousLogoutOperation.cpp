#include "nn/nex/nex_RendezVousLogoutOperation.h"

namespace nn {
namespace nex {
// 0x003B85E8 slot 0x00
// 0x003B85D8 (deleting dtor)
nn::nex::RendezVousLogoutOperation::~RendezVousLogoutOperation()
{
    // nothing to do
}

// 0x0072D9A8 slot 0x0C
u32 nn::nex::RendezVousLogoutOperation::GetTypeImpl() const
{
    return TYPE;
}

// 0x0072D97C slot 0x10
const wchar_t* nn::nex::RendezVousLogoutOperation::GetName() const
{
    return L"RendezVousLogout";
}

// 0x003B85D4 slot 0x14
void nn::nex::RendezVousLogoutOperation::vf_0x14()
{
    // nothing to do
}

} // namespace nex
} // namespace nn
