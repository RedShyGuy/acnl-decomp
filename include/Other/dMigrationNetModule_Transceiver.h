#pragma once

#include "decomp.h"
#include "Other/dMigrationNetModule.h"
#include "net/dSystemCallback.h"

// RTTI N18MigrationNetModule11TransceiverE @ 0x008CDB44
// vtable 0x008FBAE8 (vptr 0x008FBAF0), offset_to_top 0, 8 entries
class MigrationNetModule::Transceiver : public ::net::SystemCallback
{
public:
    Transceiver(); // ctor candidate(s) 0x0029CD6C, 0x002CFD00 (unverified)
    virtual void vf_0x00(); // 0x0050D024 slot 0x00 | virtual slot, introduced by net::SystemCallback
    virtual void vf_0x04(); // 0x002E4974 slot 0x04 | virtual slot, introduced by net::SystemCallback
    virtual void vf_0x08(); // 0x002E4894 slot 0x08 | virtual slot, introduced by net::SystemCallback
    virtual void vf_0x0C(); // 0x002E48BC slot 0x0C | virtual slot, introduced by net::SystemCallback
    virtual void vf_0x10(); // 0x002E4854 slot 0x10 | virtual slot, introduced by net::SystemCallback
    virtual void vf_0x14(); // 0x002E4924 slot 0x14 | virtual slot, introduced by net::SystemCallback
};
