#include "nn/nex/nex_Operation.h"

namespace nn {
namespace nex {
// 0x003D753C
nn::nex::Operation::Operation(unsigned int id) : m_Unknown0x04(false), m_Unknown0x05(false), m_Unknown0x08(0), m_Id(id)
{
    // nothing to do
}

// 0x003D7570 slot 0x00
// 0x003D7560 (deleting dtor)
nn::nex::Operation::~Operation()
{
    // nothing to do
}

// 0x0072EA80 slot 0x08
u32 nn::nex::Operation::GetType() const
{
    return GetTypeImpl();
}

} // namespace nex
} // namespace nn
