#include "nn/nex/nex_PersistentGathering.h"

namespace nn {
namespace nex {
// 0x003946AC | fefates:bytes [tier B]
nn::nex::PersistentGathering::PersistentGathering()
{
    Reset();
}

// 0x0039475C slot 0x00
// 0x0039474C (deleting dtor)
nn::nex::PersistentGathering::~PersistentGathering()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003945E0 | fefates:bytes [tier B]
void nn::nex::PersistentGathering::Reset()
{
    m_Unknown0x34 = 0;
    m_Unknown0x38 = L"";
    m_Unknown0x40.clear();
    m_Unknown0x40.ResizeInline(RESET_UNKNOWN0X40_SIZE, 0);
    m_Unknown0x4C.clear();
    m_Unknown0x68 = 0;
    m_Unknown0x6C = 0;
    m_Unknown0x58 = *DateTime::s_pNever;
    m_Unknown0x60 = *DateTime::s_pNever;
    Gathering::Reset();
    m_Flags |= FLAG_PERSISTENT;
    SetParticipationPolicy(PARTICIPATION_POLICY, 0);
}

} // namespace nex
} // namespace nn
