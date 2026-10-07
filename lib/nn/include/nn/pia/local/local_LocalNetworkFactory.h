#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/transport/transport_NetworkFactory.h"

namespace nn {
namespace pia {
namespace common {
class IPacketInput;
class IPacketOutput;
} // namespace common
namespace local {
// RTTI N2nn3pia5local19LocalNetworkFactoryE @ 0x008CFB80
// vtable 0x00900B58 (vptr 0x00900B60), offset_to_top 0, 44 entries
//
// The objects of the local network for transport and session (like inet::NexNetworkFactory).
// The local network has no server: no joint sessions, no automatic matchmaking, no relay, no
// signature, no passwords and no attributes. The session infos and the matchmake session are
// the ones of the network (UdsNetworkFactory).
class LocalNetworkFactory : public ::nn::pia::transport::NetworkFactory
{
public:
    LocalNetworkFactory(); // 0x00418398
    virtual ~LocalNetworkFactory(); // 0x004183B0 slot 0x00
    // 0x004183A8 slot 0x04 (deleting dtor)
    virtual nn::pia::transport::ConnectStationJob* CreateConnectStationJob(); // 0x004181CC slot 0x08 | fefates:callseq
    virtual nn::pia::transport::DisconnectStationJob* CreateDisconnectStationJob(); // 0x00418214 slot 0x0C | fefates:callseq
    virtual nn::pia::common::IPacketInput* CreateInputStream(); // 0x004180C4 slot 0x10 | fefates:callseq
    virtual nn::pia::session::CreateMeshJob* CreateCreateMeshJob(); // 0x00418164 slot 0x14 | fefates:callseq
    virtual nn::pia::session::JoinMeshJob* CreateJoinMeshJob(); // 0x004180F4 slot 0x18
    virtual nn::pia::session::LeaveMeshJob* CreateLeaveMeshJob(); // 0x00418114 slot 0x1C
    virtual nn::pia::common::IPacketOutput* CreateOutputStream(); // 0x00418134 slot 0x20 | fefates:callseq
    virtual nn::pia::session::ProcessHostMigrationJob* CreateProcessHostMigrationJob(); // 0x00418294 slot 0x24
    virtual nn::pia::session::LeaveWithHostMigrationJob* CreateLeaveWithHostMigrationJob(); // 0x00418338 slot 0x28
    virtual nn::pia::session::KickoutManageJob* CreateKickoutManageJob(); // 0x004181AC slot 0x2C
    // (armlink placed it and the next two in the code of session)
    virtual nn::Result CreateProtocols(); // 0x0044EEF4 slot 0x30
    virtual nn::pia::transport::PacketHandler* CreatePacketHandler(); // 0x00418184 slot 0x34
    virtual bool IsSignatureNecessary(); // 0x00730E90 slot 0x38
    virtual nn::pia::session::SignatureSettingStorage* CreateSignatureSettingStorage(); // 0x004182B4 slot 0x3C
    virtual bool vf_0x40(); // 0x00734E18 slot 0x40
    virtual bool IsRelayRouteSupported(); // 0x00734E20 slot 0x44
    virtual bool IsJointSessionSupported(); // 0x00734E10 slot 0x48
    virtual nn::pia::common::MonitoringDataSender* CreateMonitoringDataSender(); // 0x0044EEFC slot 0x4C
    virtual u32 GetPacketHeaderSize(); // 0x00730E78 slot 0x50
    virtual nn::pia::transport::MissingStationHandler* CreateMissingStationHandler(); // 0x0044EF04 slot 0x54
    virtual nn::pia::session::CreateSessionJob* CreateCreateSessionJob(); // 0x00418254 slot 0x58
    virtual nn::pia::session::AutoMatchmakeJob* CreateAutoMatchmakeJob(); // 0x00418358 slot 0x5C
    virtual nn::pia::session::BrowseMatchmakeJob* CreateBrowseMatchmakeJob(); // 0x00418360 slot 0x60
    virtual nn::pia::session::JoinSessionJob* CreateJoinSessionJob(); // 0x004181F4 slot 0x64
    virtual nn::pia::session::LeaveSessionJob* CreateLeaveSessionJob(); // 0x00418234 slot 0x68
    virtual nn::pia::session::DestroySessionJob* CreateDestroySessionJob(); // 0x00418274 slot 0x6C
    virtual nn::pia::session::GenerateMatchmakeSystemPasswordJob* CreateGenerateMatchmakeSystemPasswordJob(); // 0x00418390 slot 0x70
    virtual nn::pia::session::ClearMatchmakeSystemPasswordJob* CreateClearMatchmakeSystemPasswordJob(); // 0x00418388 slot 0x74
    virtual u32 GetStringBufferLength(); // 0x00730EA0 slot 0x78
    virtual u16* CreateStringBuffer(u32 length); // 0x00418380 slot 0x7C
    virtual nn::pia::session::JointSessionJob* CreateJointSessionJob(); // 0x004181A4 slot 0x80
    virtual nn::pia::session::ModifyAttributeJob* CreateModifyAttributeJob(); // 0x004181EC slot 0x84
    virtual nn::pia::session::UpdateSessionSettingJob* CreateUpdateSessionSettingJob(); // 0x004182E4 slot 0x88
    virtual nn::pia::session::UpdateApplicationDataJob* CreateUpdateApplicationDataJob(); // 0x00418318 slot 0x8C
    virtual nn::pia::session::ISessionInfoList* CreateSessionInfoList(u32 capacity) = 0; // slot 0x90
    virtual nn::pia::session::CommonMatchmakeSession* CreateMatchmakeSession() = 0; // slot 0x94
    virtual nn::pia::session::MeshLayerController* CreateMeshLayerController(); // 0x004182EC slot 0x98
    virtual bool vf_0x9C(); // 0x00730E80 slot 0x9C
    virtual bool vf_0xA0(); // 0x00730E88 slot 0xA0
    virtual u8 GetHostMigrationMode(); // 0x00730E98 slot 0xA4
    virtual u32 GetSessionInfoNumMax() = 0; // slot 0xA8
    // (empty)
    virtual void vf_0xAC(); // 0x00730EA8 slot 0xAC
};
} // namespace local
} // namespace pia
} // namespace nn
