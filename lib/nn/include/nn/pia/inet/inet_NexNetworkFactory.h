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
namespace inet {
// RTTI N2nn3pia4inet17NexNetworkFactoryE @ 0x008CF8C0
// vtable 0x00900070 (vptr 0x00900078), offset_to_top 0, 43 entries
class NexNetworkFactory : public ::nn::pia::transport::NetworkFactory
{
public:
    NexNetworkFactory() {} // (inline; the application makes it)
    virtual ~NexNetworkFactory(); // 0x003E8564 slot 0x00
    // 0x003E8560 slot 0x04 (deleting dtor)
    virtual nn::pia::transport::ConnectStationJob* CreateConnectStationJob(); // 0x003E828C slot 0x08
    virtual nn::pia::transport::DisconnectStationJob* CreateDisconnectStationJob(); // 0x003E82EC slot 0x0C
    virtual nn::pia::common::IPacketInput* CreateInputStream(); // 0x003E803C slot 0x10
    virtual nn::pia::session::CreateMeshJob* CreateCreateMeshJob(); // 0x003E8108 slot 0x14
    virtual nn::pia::session::JoinMeshJob* CreateJoinMeshJob(); // 0x003E806C slot 0x18
    virtual nn::pia::session::LeaveMeshJob* CreateLeaveMeshJob(); // 0x003E80B8 slot 0x1C
    virtual nn::pia::common::IPacketOutput* CreateOutputStream(); // 0x003E80D8 slot 0x20
    virtual nn::pia::session::ProcessHostMigrationJob* CreateProcessHostMigrationJob(); // 0x003E83B0 slot 0x24
    virtual nn::pia::session::LeaveWithHostMigrationJob* CreateLeaveWithHostMigrationJob(); // 0x003E8460 slot 0x28
    virtual nn::pia::session::KickoutManageJob* CreateKickoutManageJob(); // 0x003E824C slot 0x2C
    virtual nn::Result CreateProtocols(); // 0x003E8018 slot 0x30
    virtual nn::pia::transport::PacketHandler* CreatePacketHandler(); // 0x003E8154 slot 0x34
    virtual bool IsSignatureNecessary(); // 0x0072F11C slot 0x38
    virtual nn::pia::session::SignatureSettingStorage* CreateSignatureSettingStorage(); // 0x003E83D0 slot 0x3C
    virtual bool vf_0x40(); // 0x0072F134 slot 0x40
    virtual bool IsRelayRouteSupported(); // 0x0072F13C slot 0x44
    virtual bool IsJointSessionSupported(); // 0x0072F12C slot 0x48
    virtual nn::pia::common::MonitoringDataSender* CreateMonitoringDataSender(); // 0x003E832C slot 0x4C
    virtual u32 GetPacketHeaderSize(); // 0x0072F104 slot 0x50
    virtual nn::pia::transport::MissingStationHandler* CreateMissingStationHandler(); // 0x003E836C slot 0x54
    virtual nn::pia::session::CreateSessionJob* CreateCreateSessionJob(); // 0x003E834C slot 0x58
    virtual nn::pia::session::AutoMatchmakeJob* CreateAutoMatchmakeJob(); // 0x003E8480 slot 0x5C
    virtual nn::pia::session::BrowseMatchmakeJob* CreateBrowseMatchmakeJob(); // 0x003E84A0 slot 0x60
    virtual nn::pia::session::JoinSessionJob* CreateJoinSessionJob(); // 0x003E82CC slot 0x64
    virtual nn::pia::session::LeaveSessionJob* CreateLeaveSessionJob(); // 0x003E830C slot 0x68
    virtual nn::pia::session::DestroySessionJob* CreateDestroySessionJob(); // 0x003E8390 slot 0x6C
    virtual nn::pia::session::GenerateMatchmakeSystemPasswordJob* CreateGenerateMatchmakeSystemPasswordJob(); // 0x003E8540 slot 0x70
    virtual nn::pia::session::ClearMatchmakeSystemPasswordJob* CreateClearMatchmakeSystemPasswordJob(); // 0x003E8520 slot 0x74
    virtual u32 GetStringBufferLength(); // 0x0072F14C slot 0x78
    virtual u16* CreateStringBuffer(u32 length); // 0x003E84C0 slot 0x7C
    virtual nn::pia::session::JointSessionJob* CreateJointSessionJob(); // 0x003E8174 slot 0x80
    virtual nn::pia::session::ModifyAttributeJob* CreateModifyAttributeJob(); // 0x003E82AC slot 0x84
    virtual nn::pia::session::UpdateSessionSettingJob* CreateUpdateSessionSettingJob(); // 0x003E8400 slot 0x88
    virtual nn::pia::session::UpdateApplicationDataJob* CreateUpdateApplicationDataJob(); // 0x003E8440 slot 0x8C
    virtual nn::pia::session::ISessionInfoList* CreateSessionInfoList(u32 capacity); // 0x003E8194 slot 0x90
    virtual nn::pia::session::CommonMatchmakeSession* CreateMatchmakeSession(); // 0x003E826C slot 0x94
    virtual nn::pia::session::MeshLayerController* CreateMeshLayerController(); // 0x003E8420 slot 0x98
    virtual bool vf_0x9C(); // 0x0072F10C slot 0x9C
    virtual bool vf_0xA0(); // 0x0072F114 slot 0xA0
    virtual u8 GetHostMigrationMode(); // 0x0072F124 slot 0xA4
    virtual u32 GetSessionInfoNumMax(); // 0x0072F144 slot 0xA8
};
} // namespace inet
} // namespace pia
} // namespace nn
