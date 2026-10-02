#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0xCA80 in ModuleStation.cro, offset_to_top 0, 89 entries
class AcNpcSpStationMaster : public ::AcNpcSp
{
public:
    class AsyncAction;
    class TalkRecept;
    AcNpcSpStationMaster(); // ctor address unknown
    virtual ~AcNpcSpStationMaster(); // ModuleStation.cro +0x006FD4 slot 0x00
    virtual void Initialize(); // ModuleStation.cro +0x00B918 slot 0x0C
    virtual void Finalize(); // ModuleStation.cro +0x00B9B4 slot 0x18
    virtual void Unk0(); // ModuleStation.cro +0x00A4C8 slot 0x3C
};
