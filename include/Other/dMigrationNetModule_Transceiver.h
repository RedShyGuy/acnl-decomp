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
    // (a nop in the original that falls into the destructor of net::SystemCallback)
    virtual ~Transceiver(); // 0x0050D024 slot 0x00
    // 0x002E4974 slot 0x04 (deleting dtor)
    virtual bool OnJoinApproval(const char16* name, u32 length); // 0x002E4894 slot 0x08
    virtual void OnStationJoined(const nn::pia::StationId& stationId, const char16* name, u32 length, u32 frame); // 0x002E48BC slot 0x0C
    virtual void OnStationLeft(const nn::pia::StationId& stationId, u32 frame); // 0x002E4854 slot 0x10
    virtual void OnReceive(const nn::pia::StationId& stationId, const void* data, u32 size); // 0x002E4924 slot 0x14
};
