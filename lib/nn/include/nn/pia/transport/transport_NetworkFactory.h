#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace common {
class IPacketInput;
class IPacketOutput;
class MonitoringDataSender;
} // namespace common
namespace session {
class CreateMeshJob;
class JoinMeshJob;
class KickoutManageJob;
class LeaveMeshJob;
class LeaveWithHostMigrationJob;
class ProcessHostMigrationJob;
class SignatureSettingStorage;
} // namespace session
namespace transport {
class ConnectStationJob;
class DisconnectStationJob;
class MissingStationHandler;
class PacketHandler;

// RTTI N2nn3pia9transport14NetworkFactoryE @ 0x008D017C
//
// Creates the objects that depend on the network (local: uds, inet: nex). Abstract, 43 slots;
// the slot names are from the symbols of the creators in LocalNetworkFactory, the other slots
// are not known yet (the ones marked "ours" are named after what LocalNetworkFactory creates).
class NetworkFactory : public ::nn::pia::common::RootObject
{
public:
    virtual ~NetworkFactory() {}
    // slot 0x04: deleting dtor
    virtual ConnectStationJob* CreateConnectStationJob() = 0; // slot 0x08
    virtual DisconnectStationJob* CreateDisconnectStationJob() = 0; // slot 0x0C
    virtual nn::pia::common::IPacketInput* CreateInputStream() = 0; // slot 0x10
    virtual session::CreateMeshJob* CreateCreateMeshJob() = 0; // slot 0x14
    virtual session::JoinMeshJob* CreateJoinMeshJob() = 0; // slot 0x18 (name is ours)
    virtual session::LeaveMeshJob* CreateLeaveMeshJob() = 0; // slot 0x1C (name is ours)
    virtual nn::pia::common::IPacketOutput* CreateOutputStream() = 0; // slot 0x20
    virtual nn::pia::session::ProcessHostMigrationJob* CreateProcessHostMigrationJob() = 0; // slot 0x24 (name is ours)
    virtual nn::pia::session::LeaveWithHostMigrationJob* CreateLeaveWithHostMigrationJob() = 0; // slot 0x28 (name is ours)
    virtual nn::pia::session::KickoutManageJob* CreateKickoutManageJob() = 0; // slot 0x2C (name is ours)
    virtual nn::Result CreateProtocols() = 0; // slot 0x30
    virtual PacketHandler* CreatePacketHandler() = 0; // slot 0x34 (name is ours)
    // whether the packets need a signature (SignatureManager::SetNecessity; name is ours)
    virtual bool IsSignatureNecessary() = 0; // slot 0x38
    virtual nn::pia::session::SignatureSettingStorage* CreateSignatureSettingStorage() = 0; // slot 0x3C (name is ours)
    virtual bool vf_0x40() = 0; // slot 0x40
    virtual bool IsRelayRouteSupported() = 0; // slot 0x44 (name is ours)
    virtual bool IsJointSessionSupported() = 0; // slot 0x48 (name is ours)
    virtual nn::pia::common::MonitoringDataSender* CreateMonitoringDataSender() = 0; // slot 0x4C (name is ours)
    // the header size for StationPacketHandler::Initialize (name is ours)
    virtual u32 GetPacketHeaderSize() = 0; // slot 0x50
    virtual MissingStationHandler* CreateMissingStationHandler() = 0; // slot 0x54
    virtual nn::pia::session::CreateSessionJob* CreateCreateSessionJob() = 0; // slot 0x58 (name is ours)
    virtual nn::pia::session::AutoMatchmakeJob* CreateAutoMatchmakeJob() = 0; // slot 0x5C (name is ours)
    virtual nn::pia::session::BrowseMatchmakeJob* CreateBrowseMatchmakeJob() = 0; // slot 0x60 (name is ours)
    virtual nn::pia::session::JoinSessionJob* CreateJoinSessionJob() = 0; // slot 0x64 (name is ours)
    virtual nn::pia::session::LeaveSessionJob* CreateLeaveSessionJob() = 0; // slot 0x68 (name is ours)
    virtual nn::pia::session::DestroySessionJob* CreateDestroySessionJob() = 0; // slot 0x6C (name is ours)
    virtual nn::pia::session::GenerateMatchmakeSystemPasswordJob* CreateGenerateMatchmakeSystemPasswordJob() = 0; // slot 0x70 (name is ours)
    virtual nn::pia::session::ClearMatchmakeSystemPasswordJob* CreateClearMatchmakeSystemPasswordJob() = 0; // slot 0x74 (name is ours)
    virtual u32 GetStringBufferLength() = 0; // slot 0x78 (name is ours)
    virtual u16* CreateStringBuffer(u32 length) = 0; // slot 0x7C (name is ours)
    virtual nn::pia::session::JointSessionJob* CreateJointSessionJob() = 0; // slot 0x80 (name is ours)
    virtual nn::pia::session::ModifyAttributeJob* CreateModifyAttributeJob() = 0; // slot 0x84 (name is ours)
    virtual nn::pia::session::UpdateSessionSettingJob* CreateUpdateSessionSettingJob() = 0; // slot 0x88 (name is ours)
    virtual nn::pia::session::UpdateApplicationDataJob* CreateUpdateApplicationDataJob() = 0; // slot 0x8C (name is ours)
    virtual nn::pia::session::ISessionInfoList* CreateSessionInfoList(u32 capacity) = 0; // slot 0x90 (name is ours)
    virtual nn::pia::session::CommonMatchmakeSession* CreateMatchmakeSession() = 0; // slot 0x94 (name is ours)
    virtual nn::pia::session::MeshLayerController* CreateMeshLayerController() = 0; // slot 0x98 (name is ours)
    virtual bool vf_0x9C() = 0; // slot 0x9C
    virtual bool vf_0xA0() = 0; // slot 0xA0
    virtual u8 GetHostMigrationMode() = 0; // slot 0xA4 (name is ours)
    virtual u32 GetSessionInfoNumMax() = 0; // slot 0xA8 (name is ours)
};
} // namespace transport
} // namespace pia
} // namespace nn
