#pragma once

#include "decomp.h"
#include "nn/os/os_Event.h"
#include "pead/peadIDisposer.h"

namespace pead {
// RTTI N4pead5EventE @ 0x008D1274
// vtable 0x00904CC8 (vptr 0x00904CD0), offset_to_top 0, 2 entries
//
// A kernel event that its heap destroys (no copy in sead of the game; the method and member names are ours).
class Event : public ::pead::IDisposer
{
public:
    // creates the kernel event (manualReset: sticky, else one-shot)
    explicit Event(bool manualReset); // 0x0053B98C
    virtual ~Event(); // 0x0053BA28 slot 0x00 | slot vf_0x00 of pead::IDisposer
    // 0x0053B9E4 slot 0x04 | slot vf_0x04 of pead::IDisposer (deleting dtor)

    void Signal(); // 0x0053B96C
    void Clear(); // 0x0053B8B4
    void Wait(); // 0x0053B944
    // waits at most the given number of ticks; false on the timeout
    bool Wait(s64 ticks); // 0x0053B8D4

    // the description of the result of a wait that timed out (3dbrew: 1022, timeout)
    static const bit32 DESCRIPTION_TIMEOUT = 0x3FE;

    nn::os::Event mEvent; // 0x10
};
ASSERT_SIZE(Event, 0x14);
} // namespace pead
