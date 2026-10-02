#pragma once

#include "decomp.h"

namespace oml {
namespace framework {
// RTTI N3oml9framework7ProcessE @ 0x008D1028
// vtable 0x0090476C (vptr 0x00904774), offset_to_top 0, 15 entries
class Process
{
public:
    virtual ~Process(); // 0x0052218C slot 0x00 | libgarden
    // 0x00522184 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void CanInitialize() const; // 0x00522128 slot 0x08 | slot vf_0x08 of oml::framework::Process
    virtual void Initialize(); // 0x005220BC slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void HandleInitializationResult(oml::framework::Result); // 0x00521614 slot 0x10 | slot vf_0x10 of oml::framework::Process
    virtual void CanFinalize() const; // 0x00522130 slot 0x14 | slot vf_0x14 of oml::framework::Process
    virtual void Finalize(); // 0x005220DC slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void HandleFinalizationResult(oml::framework::Result); // 0x0052161C slot 0x1C | slot vf_0x1C of oml::framework::Process
    virtual void CanCalc() const; // 0x00521620 slot 0x20 | slot vf_0x20 of oml::framework::Process
    virtual void Calc(); // 0x005220C8 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void HandleCalcResult(oml::framework::Result); // 0x00521718 slot 0x28 | slot vf_0x28 of oml::framework::Process
    virtual void CanDraw() const; // 0x005220D0 slot 0x2C | slot vf_0x2C of oml::framework::Process
    virtual void Draw(); // 0x005220B0 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void ProcessDrawResult(oml::framework::Result); // 0x005220E8 slot 0x34 | libgarden
    virtual void OnNotify(); // 0x00521710 slot 0x38 | libgarden
    void operator delete(void*); // 0x00313B24 | libgarden [tier A]
    void StopSelf(); // 0x00520B5C | libgarden [tier A]
    void Stop(); // 0x00521754 | libgarden [tier A]
    Process(); // 0x00522138 | libgarden [tier A]
    void Destroy(oml::framework::Process*); // 0x005251F0 | libgarden [tier A]
    void* operator new(unsigned int); // 0x0052A488 | libgarden [tier A]
};
} // namespace framework
} // namespace oml
