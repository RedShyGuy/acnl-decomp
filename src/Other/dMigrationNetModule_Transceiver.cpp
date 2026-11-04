#include "net/dSystemCallback.h"
#include "Other/dMigrationNetModule_Transceiver.h"

// ctor candidate(s) 0x0029CD6C, 0x002CFD00 (unverified)
MigrationNetModule::Transceiver::Transceiver()
{
}

// 0x0050D024 slot 0x00
// 0x002E4974 (deleting dtor)
MigrationNetModule::Transceiver::~Transceiver()
{
}

// 0x002E4894 slot 0x08
bool MigrationNetModule::Transceiver::OnJoinApproval(const char16* name, u32 length)
{
}

// 0x002E48BC slot 0x0C
void MigrationNetModule::Transceiver::OnStationJoined(const nn::pia::StationId& stationId, const char16* name, u32 length, u32 frame)
{
}

// 0x002E4854 slot 0x10
void MigrationNetModule::Transceiver::OnStationLeft(const nn::pia::StationId& stationId, u32 frame)
{
}

// 0x002E4924 slot 0x14
void MigrationNetModule::Transceiver::OnReceive(const nn::pia::StationId& stationId, const void* data, u32 size)
{
}

