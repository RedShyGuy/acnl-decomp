#pragma once

#include "decomp.h"
#include "Bs/dBsTimeBelWindow.h"
#include "state/dMode.h"

// RTTI N15BsTimeBelWindow9MoneyProcE @ 0x008CDA6C
// vtable 0x008FB9D8 (vptr 0x008FB9E0), offset_to_top 0, 3 entries
class BsTimeBelWindow::MoneyProc : public ::state::Mode<BsTimeBelWindow::MoneyProc>
{
public:
    MoneyProc(); // ctor address unknown
    virtual void vf_0x00(); // 0x0029593C slot 0x00 | virtual slot, introduced by BsTimeBelWindow::MoneyProc
    virtual void vf_0x04(); // 0x00295904 slot 0x04 | virtual slot, introduced by BsTimeBelWindow::MoneyProc
    virtual void vf_0x08(); // 0x0082D8A8 slot 0x08 | virtual slot, introduced by BsTimeBelWindow::MoneyProc
};
