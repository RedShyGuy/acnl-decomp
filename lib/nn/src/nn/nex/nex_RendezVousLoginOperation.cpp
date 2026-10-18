#include "nn/nex/nex_RendezVousLoginOperation.h"

namespace nn {
namespace nex {
// 0x003B5770 slot 0x00
// 0x003B5760 (deleting dtor)
nn::nex::RendezVousLoginOperation::~RendezVousLoginOperation()
{
    // nothing to do
}

// 0x0072D598 slot 0x0C
u32 nn::nex::RendezVousLoginOperation::GetTypeImpl() const
{
    return TYPE;
}

// 0x0072D570 slot 0x10
const wchar_t* nn::nex::RendezVousLoginOperation::GetName() const
{
    return L"RendezVousLogin";
}

// 0x003B575C slot 0x14
void nn::nex::RendezVousLoginOperation::vf_0x14()
{
    // nothing to do
}

} // namespace nex
} // namespace nn
