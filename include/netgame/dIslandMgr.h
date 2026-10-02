#pragma once

#include "decomp.h"

namespace netgame {
class IslandMgr
{
public:
    struct LeaveRandomMatchMode { u32 _unknown; }; // TODO: real type unknown (placeholder)
    void StartFindRandomMatch(); // 0x0061E3B0 | libgarden [tier A]
    void EndSeaDepartureDemo(bool); // 0x0061E3E8 | libgarden [tier A]
    void StartTourDeskArrivalAfterTour(); // 0x0061E568 | libgarden [tier A]
    void DepartForIsland(); // 0x0061E8BC | libgarden [tier A]
    void RetireFromIsland(); // 0x0061E954 | libgarden [tier A]
    void UpdatePlayerTourSplit(unsigned char, unsigned char); // 0x0061EA30 | libgarden [tier A]
    void LeaveRandomMatch(netgame::IslandMgr::LeaveRandomMatchMode); // 0x0061EBF4 | libgarden [tier A]
    void ConfirmLeaveSelf(); // 0x0061F018 | libgarden [tier A]
    void StartTourRandomMatch(); // 0x0061F760 | libgarden [tier A]
    void CancelTrip(); // 0x0061F8E8 | libgarden [tier A]
    void RequestRandomMatchSeeOff(); // 0x0061FD78 | libgarden [tier A]
    void ChooseTour(TourName); // 0x0061FDC0 | libgarden [tier A]
    void GetRandomMatchingResult(); // 0x0075F09C | libgarden [tier A]
    void IsRetiringRandomMatch(); // 0x0075F140 | libgarden [tier A]
    void CanRetireRandomMatch(); // 0x0075F1C4 | libgarden [tier A]
};
} // namespace netgame
