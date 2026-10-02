#pragma once

#include "decomp.h"

// Instantiations found in the binary:
//   RoadSearchCand<11, 11, IRootLocateRelative>::Cand  typeinfo 0x008CD9E8  vtable 0x008FB43C
//   RoadSearchCand<15, 15, IRootLocateRelative>::Cand  typeinfo 0x008CD9F4  vtable 0x008FB44C
//   RoadSearchCand<16, 16, IRootLocateRelative>::Cand  typeinfo 0x008CDA0C  vtable 0x008FB46C
//   RoadSearchCand<16, 16, IRootLocateZero>::Cand  typeinfo 0x008CDA00  vtable 0x008FB45C
//   RoadSearchCand<9, 9, IRootLocateRelative>::Cand  typeinfo 0x008CDA18  vtable 0x008FB47C
template <auto T0, auto T1, typename T2>
class RoadSearchCand
{
public:
    // TODO: members unknown
};
