#include "nn/pia/local/local_LocalNetworkFactory.h"
#include "nn/pia/local/local_LocalInputStream.h"
#include "nn/pia/local/local_LocalKickoutManageJob.h"
#include "nn/pia/local/local_LocalLeaveWithHostMigrationJobNew.h"
#include "nn/pia/local/local_LocalMatchBrowseMatchmakeJob.h"
#include "nn/pia/local/local_LocalMatchCreateSessionJob.h"
#include "nn/pia/local/local_LocalMatchDestroySessionJob.h"
#include "nn/pia/local/local_LocalMatchJoinSessionJob.h"
#include "nn/pia/local/local_LocalMatchLeaveSessionJob.h"
#include "nn/pia/local/local_LocalMatchMeshLayerController.h"
#include "nn/pia/local/local_LocalMatchUpdateApplicationDataJob.h"
#include "nn/pia/local/local_LocalOutputStream.h"
#include "nn/pia/local/local_LocalProcessHostMigrationJobNew.h"
#include "nn/pia/session/session_CreateMeshJob.h"
#include "nn/pia/session/session_JoinMeshJob.h"
#include "nn/pia/session/session_LeaveMeshJob.h"
#include "nn/pia/session/session_SignatureSettingStorage.h"
#include "nn/pia/transport/transport_ConnectStationJob.h"
#include "nn/pia/transport/transport_DisconnectStationJob.h"
#include "nn/pia/transport/transport_StationPacketHandler.h"

namespace nn {
namespace pia {
namespace local {
// 0x004180C4 | fefates:callseq
nn::pia::common::IPacketInput* nn::pia::local::LocalNetworkFactory::CreateInputStream()
{
    return new LocalInputStream();
}

// 0x004180F4
nn::pia::session::JoinMeshJob* nn::pia::local::LocalNetworkFactory::CreateJoinMeshJob()
{
    return new session::JoinMeshJob();
}

// 0x00418114
nn::pia::session::LeaveMeshJob* nn::pia::local::LocalNetworkFactory::CreateLeaveMeshJob()
{
    return new session::LeaveMeshJob();
}

// 0x00418134 | fefates:callseq
nn::pia::common::IPacketOutput* nn::pia::local::LocalNetworkFactory::CreateOutputStream()
{
    return new LocalOutputStream();
}

// 0x00418164 | fefates:callseq
nn::pia::session::CreateMeshJob* nn::pia::local::LocalNetworkFactory::CreateCreateMeshJob()
{
    return new session::CreateMeshJob();
}

// 0x00418184
nn::pia::transport::PacketHandler* nn::pia::local::LocalNetworkFactory::CreatePacketHandler()
{
    return new transport::StationPacketHandler();
}

// 0x004181A4
nn::pia::session::JointSessionJob* nn::pia::local::LocalNetworkFactory::CreateJointSessionJob()
{
    return nullptr;
}

// 0x004181AC
nn::pia::session::KickoutManageJob* nn::pia::local::LocalNetworkFactory::CreateKickoutManageJob()
{
    return new LocalKickoutManageJob();
}

// 0x004181CC | fefates:callseq
nn::pia::transport::ConnectStationJob* nn::pia::local::LocalNetworkFactory::CreateConnectStationJob()
{
    return new transport::ConnectStationJob();
}

// 0x004181EC
nn::pia::session::ModifyAttributeJob* nn::pia::local::LocalNetworkFactory::CreateModifyAttributeJob()
{
    return nullptr;
}

// 0x004181F4
nn::pia::session::JoinSessionJob* nn::pia::local::LocalNetworkFactory::CreateJoinSessionJob()
{
    return new LocalMatchJoinSessionJob();
}

// 0x00418214 | fefates:callseq
nn::pia::transport::DisconnectStationJob* nn::pia::local::LocalNetworkFactory::CreateDisconnectStationJob()
{
    return new transport::DisconnectStationJob();
}

// 0x00418234
nn::pia::session::LeaveSessionJob* nn::pia::local::LocalNetworkFactory::CreateLeaveSessionJob()
{
    return new LocalMatchLeaveSessionJob();
}

// 0x00418254
nn::pia::session::CreateSessionJob* nn::pia::local::LocalNetworkFactory::CreateCreateSessionJob()
{
    return new LocalMatchCreateSessionJob();
}

// 0x00418274
nn::pia::session::DestroySessionJob* nn::pia::local::LocalNetworkFactory::CreateDestroySessionJob()
{
    return new LocalMatchDestroySessionJob();
}

// 0x00418294
nn::pia::session::ProcessHostMigrationJob* nn::pia::local::LocalNetworkFactory::CreateProcessHostMigrationJob()
{
    return new LocalProcessHostMigrationJobNew();
}

// 0x004182B4
nn::pia::session::SignatureSettingStorage* nn::pia::local::LocalNetworkFactory::CreateSignatureSettingStorage()
{
    return new session::SignatureSettingStorage();
}

// 0x004182E4
nn::pia::session::UpdateSessionSettingJob* nn::pia::local::LocalNetworkFactory::CreateUpdateSessionSettingJob()
{
    return nullptr;
}

// 0x004182EC
nn::pia::session::MeshLayerController* nn::pia::local::LocalNetworkFactory::CreateMeshLayerController()
{
    return new LocalMatchMeshLayerController();
}

// 0x00418318
nn::pia::session::UpdateApplicationDataJob* nn::pia::local::LocalNetworkFactory::CreateUpdateApplicationDataJob()
{
    return new LocalMatchUpdateApplicationDataJob();
}

// 0x00418338
nn::pia::session::LeaveWithHostMigrationJob* nn::pia::local::LocalNetworkFactory::CreateLeaveWithHostMigrationJob()
{
    return new LocalLeaveWithHostMigrationJobNew();
}

// 0x00418358
nn::pia::session::AutoMatchmakeJob* nn::pia::local::LocalNetworkFactory::CreateAutoMatchmakeJob()
{
    return nullptr;
}

// 0x00418360
nn::pia::session::BrowseMatchmakeJob* nn::pia::local::LocalNetworkFactory::CreateBrowseMatchmakeJob()
{
    return new LocalMatchBrowseMatchmakeJob();
}

// 0x00418380
u16* nn::pia::local::LocalNetworkFactory::CreateStringBuffer(u32)
{
    return nullptr;
}

// 0x00418388
nn::pia::session::ClearMatchmakeSystemPasswordJob* nn::pia::local::LocalNetworkFactory::CreateClearMatchmakeSystemPasswordJob()
{
    return nullptr;
}

// 0x00418390
nn::pia::session::GenerateMatchmakeSystemPasswordJob* nn::pia::local::LocalNetworkFactory::CreateGenerateMatchmakeSystemPasswordJob()
{
    return nullptr;
}

// 0x00418398
nn::pia::local::LocalNetworkFactory::LocalNetworkFactory()
{
    // only the vptr (in the original too)
}

// 0x004183B0
// 0x004183A8 (deleting dtor)
nn::pia::local::LocalNetworkFactory::~LocalNetworkFactory()
{
    // empty (in the original too)
}

// 0x0044EEF4
nn::Result nn::pia::local::LocalNetworkFactory::CreateProtocols()
{
    // no protocols of its own
    return nn::Result();
}

// 0x0044EEFC
nn::pia::common::MonitoringDataSender* nn::pia::local::LocalNetworkFactory::CreateMonitoringDataSender()
{
    return nullptr;
}

// 0x0044EF04
nn::pia::transport::MissingStationHandler* nn::pia::local::LocalNetworkFactory::CreateMissingStationHandler()
{
    return nullptr;
}

// 0x00730E78
u32 nn::pia::local::LocalNetworkFactory::GetPacketHeaderSize()
{
    return 10;
}

// 0x00730E80
bool nn::pia::local::LocalNetworkFactory::vf_0x9C()
{
    return false;
}

// 0x00730E88
bool nn::pia::local::LocalNetworkFactory::vf_0xA0()
{
    return false;
}

// 0x00730E90
bool nn::pia::local::LocalNetworkFactory::IsSignatureNecessary()
{
    return false;
}

// 0x00730E98
u8 nn::pia::local::LocalNetworkFactory::GetHostMigrationMode()
{
    return 1;
}

// 0x00730EA0
u32 nn::pia::local::LocalNetworkFactory::GetStringBufferLength()
{
    return 0;
}

// 0x00730EA8
void nn::pia::local::LocalNetworkFactory::vf_0xAC()
{
    // empty (in the original too)
}

// 0x00734E10
bool nn::pia::local::LocalNetworkFactory::IsJointSessionSupported()
{
    return false;
}

// 0x00734E18
bool nn::pia::local::LocalNetworkFactory::vf_0x40()
{
    return false;
}

// 0x00734E20
bool nn::pia::local::LocalNetworkFactory::IsRelayRouteSupported()
{
    return false;
}

} // namespace local
} // namespace pia
} // namespace nn
