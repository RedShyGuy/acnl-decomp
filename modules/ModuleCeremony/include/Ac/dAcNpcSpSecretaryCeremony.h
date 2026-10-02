#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x5594 in ModuleCeremony.cro, offset_to_top 0, 89 entries
class AcNpcSpSecretaryCeremony : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpSecretaryCeremony(); // ctor address unknown
    virtual ~AcNpcSpSecretaryCeremony(); // ModuleCeremony.cro +0x0015FC slot 0x00
    virtual void Calc(); // ModuleCeremony.cro +0x001434 slot 0x24
};
