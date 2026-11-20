#pragma once

#include "decomp.h"
#include "Other/dDemoActor.h"

// vtable +0x98040 in ModuleOutdoor.cro, offset_to_top 0, 54 entries
class AcStrc : public ::DemoActor
{
public:
    class Recept;
    AcStrc(); // ctor address unknown
    virtual ~AcStrc(); // ModuleOutdoor.cro +0x04B92C slot 0x00
    // (introduced by AcStrc; true once the structure has started: BsStrcMgr::UpdateInitialize waits for it)
    virtual bool vf_0x78(); // ModuleOutdoor.cro +0x07E770 slot 0x78
    virtual void vf_0x7C(); // ModuleOutdoor.cro +0x05FF78 slot 0x7C
    virtual void vf_0x80(); // ModuleOutdoor.cro +0x05FF6C slot 0x80
    virtual void vf_0x84(); // ModuleOutdoor.cro +0x05FF80 slot 0x84
    virtual void vf_0x88(); // ModuleOutdoor.cro +0x07E688 slot 0x88
    virtual void vf_0x8C(); // ModuleOutdoor.cro +0x05FEE8 slot 0x8C
    virtual void vf_0x90(); // ModuleOutdoor.cro +0x07E680 slot 0x90
    virtual void vf_0x94(); // ModuleOutdoor.cro +0x05FF84 slot 0x94
    virtual void vf_0x98(); // ModuleOutdoor.cro +0x05FAF8 slot 0x98
    // fills out (the structures with a model write it first; meaning of the rest unknown)
    virtual void vf_0x9C(void* out); // ModuleOutdoor.cro +0x05FAF0 slot 0x9C
};
