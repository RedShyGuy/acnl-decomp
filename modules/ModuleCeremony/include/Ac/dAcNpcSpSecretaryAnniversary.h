#pragma once

#include "decomp.h"
#include "Ac/dAcNpcSp.h"

// vtable +0x5700 in ModuleCeremony.cro, offset_to_top 0, 89 entries
class AcNpcSpSecretaryAnniversary : public ::AcNpcSp
{
public:
    class TalkRecept;
    AcNpcSpSecretaryAnniversary(); // ctor address unknown
    virtual ~AcNpcSpSecretaryAnniversary(); // ModuleCeremony.cro +0x00381C slot 0x00
    virtual void Initialize(); // ModuleCeremony.cro +0x003EA0 slot 0x0C
    virtual void Finalize(); // ModuleCeremony.cro +0x003F3C slot 0x18
    virtual void Unk0(); // ModuleCeremony.cro +0x003BBC slot 0x3C
};
