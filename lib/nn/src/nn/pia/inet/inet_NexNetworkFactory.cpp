#include "nn/pia/inet/inet_NexNetworkFactory.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/inet/inet_CreateMeshJob.h"
#include "nn/pia/inet/inet_InetLeaveWithHostMigrationJob.h"
#include "nn/pia/inet/inet_JoinMeshJob.h"
#include "nn/pia/inet/inet_MissingStationHandler.h"
#include "nn/pia/inet/inet_NexConnectStationJob.h"
#include "nn/pia/inet/inet_NexDisconnectStationJob.h"
#include "nn/pia/inet/inet_NexFacade.h"
#include "nn/pia/inet/inet_NexJointSessionJob.h"
#include "nn/pia/inet/inet_NexMatchAutoMatchmakeJob.h"
#include "nn/pia/inet/inet_NexMatchBrowseMatchmakeJob.h"
#include "nn/pia/inet/inet_NexMatchClearSystemPasswordJob.h"
#include "nn/pia/inet/inet_NexMatchCreateSessionJob.h"
#include "nn/pia/inet/inet_NexMatchDestroySessionJob.h"
#include "nn/pia/inet/inet_NexMatchGenerateSystemPasswordJob.h"
#include "nn/pia/inet/inet_NexMatchJoinSessionJob.h"
#include "nn/pia/inet/inet_NexMatchLeaveSessionJob.h"
#include "nn/pia/inet/inet_NexMatchMeshLayerController.h"
#include "nn/pia/inet/inet_NexMatchModifyAttributeJob.h"
#include "nn/pia/inet/inet_NexMatchUpdateApplicationDataJob.h"
#include "nn/pia/inet/inet_NexMatchUpdateSessionSettingJob.h"
#include "nn/pia/inet/inet_NexMatchmakeSession.h"
#include "nn/pia/inet/inet_NexMonitoringDataSender.h"
#include "nn/pia/inet/inet_NexProcessHostMigrationJob.h"
#include "nn/pia/inet/inet_NexSessionInfo.h"
#include "nn/pia/inet/inet_SocketInputStream.h"
#include "nn/pia/inet/inet_SocketOutputStream.h"
#include "nn/pia/session/session_KickoutManageJob.h"
#include "nn/pia/session/session_LeaveMeshJob.h"
#include "nn/pia/session/session_SessionInfoList.h"
#include "nn/pia/session/session_SignatureSettingStorage.h"
#include "nn/pia/transport/transport_StationPacketHandler.h"
#include <string.h>

namespace nn {
namespace pia {
namespace inet {
// 0x003E8018 | fefates:bytes
nn::Result nn::pia::inet::NexNetworkFactory::CreateProtocols()
{
    nn::Result result = NexFacade::s_pInstance->CreateProtocols();
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result();
}

// 0x003E803C | fefates:callseq
nn::pia::common::IPacketInput* nn::pia::inet::NexNetworkFactory::CreateInputStream()
{
    return new SocketInputStream();
}

// 0x003E806C
nn::pia::session::JoinMeshJob* nn::pia::inet::NexNetworkFactory::CreateJoinMeshJob()
{
    return new JoinMeshJob();
}

// 0x003E80B8
nn::pia::session::LeaveMeshJob* nn::pia::inet::NexNetworkFactory::CreateLeaveMeshJob()
{
    return new session::LeaveMeshJob();
}

// 0x003E80D8 | fefates:callseq
nn::pia::common::IPacketOutput* nn::pia::inet::NexNetworkFactory::CreateOutputStream()
{
    return new SocketOutputStream();
}

// 0x003E8108 | fefates:bytes
nn::pia::session::CreateMeshJob* nn::pia::inet::NexNetworkFactory::CreateCreateMeshJob()
{
    return new CreateMeshJob();
}

// 0x003E8154
nn::pia::transport::PacketHandler* nn::pia::inet::NexNetworkFactory::CreatePacketHandler()
{
    return new transport::StationPacketHandler();
}

// 0x003E8174
nn::pia::session::JointSessionJob* nn::pia::inet::NexNetworkFactory::CreateJointSessionJob()
{
    return new NexJointSessionJob();
}

// 0x003E8194
nn::pia::session::ISessionInfoList* nn::pia::inet::NexNetworkFactory::CreateSessionInfoList(u32 capacity)
{
    return new session::SessionInfoList<NexSessionInfo>(capacity);
}

// 0x003E824C
nn::pia::session::KickoutManageJob* nn::pia::inet::NexNetworkFactory::CreateKickoutManageJob()
{
    return new session::KickoutManageJob();
}

// 0x003E826C
nn::pia::session::CommonMatchmakeSession* nn::pia::inet::NexNetworkFactory::CreateMatchmakeSession()
{
    return new NexMatchmakeSession();
}

// 0x003E828C | fefates:bytes
nn::pia::transport::ConnectStationJob* nn::pia::inet::NexNetworkFactory::CreateConnectStationJob()
{
    return new NexConnectStationJob();
}

// 0x003E82AC
nn::pia::session::ModifyAttributeJob* nn::pia::inet::NexNetworkFactory::CreateModifyAttributeJob()
{
    return new NexMatchModifyAttributeJob();
}

// 0x003E82CC
nn::pia::session::JoinSessionJob* nn::pia::inet::NexNetworkFactory::CreateJoinSessionJob()
{
    return new NexMatchJoinSessionJob();
}

// 0x003E82EC
nn::pia::transport::DisconnectStationJob* nn::pia::inet::NexNetworkFactory::CreateDisconnectStationJob()
{
    return new NexDisconnectStationJob();
}

// 0x003E830C
nn::pia::session::LeaveSessionJob* nn::pia::inet::NexNetworkFactory::CreateLeaveSessionJob()
{
    return new NexMatchLeaveSessionJob();
}

// 0x003E832C
nn::pia::common::MonitoringDataSender* nn::pia::inet::NexNetworkFactory::CreateMonitoringDataSender()
{
    return new NexMonitoringDataSender();
}

// 0x003E834C
nn::pia::session::CreateSessionJob* nn::pia::inet::NexNetworkFactory::CreateCreateSessionJob()
{
    return new NexMatchCreateSessionJob();
}

// 0x003E836C | fefates:bytes
nn::pia::transport::MissingStationHandler* nn::pia::inet::NexNetworkFactory::CreateMissingStationHandler()
{
    return new MissingStationHandler();
}

// 0x003E8390
nn::pia::session::DestroySessionJob* nn::pia::inet::NexNetworkFactory::CreateDestroySessionJob()
{
    return new NexMatchDestroySessionJob();
}

// 0x003E83B0
nn::pia::session::ProcessHostMigrationJob* nn::pia::inet::NexNetworkFactory::CreateProcessHostMigrationJob()
{
    return new NexProcessHostMigrationJob();
}

// 0x003E83D0
nn::pia::session::SignatureSettingStorage* nn::pia::inet::NexNetworkFactory::CreateSignatureSettingStorage()
{
    return new session::SignatureSettingStorage();
}

// 0x003E8400
nn::pia::session::UpdateSessionSettingJob* nn::pia::inet::NexNetworkFactory::CreateUpdateSessionSettingJob()
{
    return new NexMatchUpdateSessionSettingJob();
}

// 0x003E8420
nn::pia::session::MeshLayerController* nn::pia::inet::NexNetworkFactory::CreateMeshLayerController()
{
    return new NexMatchMeshLayerController();
}

// 0x003E8440
nn::pia::session::UpdateApplicationDataJob* nn::pia::inet::NexNetworkFactory::CreateUpdateApplicationDataJob()
{
    return new NexMatchUpdateApplicationDataJob();
}

// 0x003E8460
nn::pia::session::LeaveWithHostMigrationJob* nn::pia::inet::NexNetworkFactory::CreateLeaveWithHostMigrationJob()
{
    return new InetLeaveWithHostMigrationJob();
}

// 0x003E8480
nn::pia::session::AutoMatchmakeJob* nn::pia::inet::NexNetworkFactory::CreateAutoMatchmakeJob()
{
    return new NexMatchAutoMatchmakeJob();
}

// 0x003E84A0
nn::pia::session::BrowseMatchmakeJob* nn::pia::inet::NexNetworkFactory::CreateBrowseMatchmakeJob()
{
    return new NexMatchBrowseMatchmakeJob();
}

// 0x003E84C0
u16* nn::pia::inet::NexNetworkFactory::CreateStringBuffer(u32 length)
{
    u16* pBuffer = common::NewArray<u16>(length);
    memset(pBuffer, 0, length * sizeof(u16));
    return pBuffer;
}

// 0x003E8520
nn::pia::session::ClearMatchmakeSystemPasswordJob* nn::pia::inet::NexNetworkFactory::CreateClearMatchmakeSystemPasswordJob()
{
    return new NexMatchClearSystemPasswordJob();
}

// 0x003E8540
nn::pia::session::GenerateMatchmakeSystemPasswordJob* nn::pia::inet::NexNetworkFactory::CreateGenerateMatchmakeSystemPasswordJob()
{
    return new NexMatchGenerateSystemPasswordJob();
}

// 0x003E8564
// 0x003E8560 (deleting dtor)
nn::pia::inet::NexNetworkFactory::~NexNetworkFactory()
{
    // empty (in the original too)
}

// 0x0072F104
u32 nn::pia::inet::NexNetworkFactory::GetPacketHeaderSize()
{
    return 28;
}

// 0x0072F10C
bool nn::pia::inet::NexNetworkFactory::vf_0x9C()
{
    return true;
}

// 0x0072F114
bool nn::pia::inet::NexNetworkFactory::vf_0xA0()
{
    return true;
}

// 0x0072F11C
bool nn::pia::inet::NexNetworkFactory::IsSignatureNecessary()
{
    return true;
}

// 0x0072F124
u8 nn::pia::inet::NexNetworkFactory::GetHostMigrationMode()
{
    return 2;
}

// 0x0072F12C
bool nn::pia::inet::NexNetworkFactory::IsJointSessionSupported()
{
    return true;
}

// 0x0072F134
bool nn::pia::inet::NexNetworkFactory::vf_0x40()
{
    return true;
}

// 0x0072F13C
bool nn::pia::inet::NexNetworkFactory::IsRelayRouteSupported()
{
    return true;
}

// 0x0072F144
u32 nn::pia::inet::NexNetworkFactory::GetSessionInfoNumMax()
{
    return 100;
}

// 0x0072F14C
u32 nn::pia::inet::NexNetworkFactory::GetStringBufferLength()
{
    return 17;
}

} // namespace inet
} // namespace pia
} // namespace nn
