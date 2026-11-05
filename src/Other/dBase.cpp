#include "Other/dBase.h"

// The Can... functions call a function that only returns true, the Handle... functions fall into the ones of Process
// (in the original the calls are replaced by their only instruction or a nop).

// 0x00316FA8 slot 0x00 (name after libgarden)
// 0x0052A478 (deleting dtor)
Base::~Base()
{
    // (only the base is destroyed)
}

// 0x0052A438 slot 0x08 (name after libgarden)
bool Base::CanInitialize() const
{
    return Process::CanInitialize();
}

// 0x00521610 slot 0x10 (name after libgarden)
void Base::HandleInitializationResult(oml::framework::Result result)
{
    Process::HandleInitializationResult(result);
}

// 0x0052A44C slot 0x14 (name after libgarden)
bool Base::CanFinalize() const
{
    return Process::CanFinalize();
}

// 0x00521618 slot 0x1C (name after libgarden)
void Base::HandleFinalizationResult(oml::framework::Result result)
{
    Process::HandleFinalizationResult(result);
}

// 0x0052A3A0 slot 0x20 (name after libgarden)
bool Base::CanCalc() const
{
    return Process::CanCalc();
}

// 0x00521714 slot 0x28 (name after libgarden)
void Base::HandleCalcResult(oml::framework::Result result)
{
    Process::HandleCalcResult(result);
}

// 0x0052A424 slot 0x2C (name after libgarden)
bool Base::CanDraw() const
{
    return Process::CanDraw();
}

// 0x00748D08 slot 0x3C (name after libgarden)
bool Base::Unk0()
{
    return true;
}
