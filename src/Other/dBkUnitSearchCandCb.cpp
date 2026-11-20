#include "Other/dBkUnitSearchCandCb.h"

// 0x002D6BB8 slot 0x04
bool BkUnitSearchCandCb::IsCandidateXZ(s32 x, s32 z)
{
    if (m_pCallback == NULL) {
        return false;
    }
    return m_pCallback(m_BlockX, m_BlockZ, x, z, m_Arg);
}

// (the slot of SearchCandXZ<16, 16>: armlink folded it with FdBkSearchCand::IsCandidateXZ)
// 0x002BCEEC (folded)
template bool SearchCandXZ<16, 16>::IsCandidateXZ(s32 x, s32 z);
