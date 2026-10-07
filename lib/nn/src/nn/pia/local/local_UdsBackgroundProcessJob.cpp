#include "nn/pia/local/local_UdsBackgroundProcessJob.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_SessionBeginMonitoringContent.h"
#include "nn/pia/local/local_LocalMigrationManager.h"
#include "nn/pia/local/local_LocalNetwork.h"
#include "nn/pia/local/local_UdsNetworkDescription.h"
#include "nn/pia/local/local_UdsNetworkManager.h"
#include "nn/uds/CTR/CTR_Api.h"
#include "nn/uds/CTR/uds_NetworkDescriptionReader.h"
#include "nn/uds/CTR/uds_Result.h"
#include "nn/uds/CTR/uds_ScanResultReader.h"
#include <cstring>

namespace nn {
namespace pia {
namespace local {
namespace {
inline UdsNetworkManager* GetNetworkManager()
{
    return static_cast<UdsNetworkManager*>(LocalNetwork::s_pInstance->m_pNetworkManager);
}

// a failure of uds as a result of pia
inline nn::Result ConvertResult(const nn::Result& result)
{
    if (result == nn::uds::CTR::RESULT_NOT_AUTHORIZED_STATE) {
        return common::RESULT_LOCAL_NETWORK_UNAVAILABLE;
    }
    return GetNetworkManager()->ConvertUdsResult(result);
}
} // namespace

// 0x0041EA0C
nn::pia::common::ExecuteResult nn::pia::local::UdsBackgroundProcessJob::ScanNetwork()
{
    common::CriticalSection& criticalSection = LocalNetwork::s_pInstance->m_CriticalSection;
    criticalSection.Lock();
    nn::Result result = nn::uds::CTR::StartScan(m_ScanSetting.m_pBuffer, m_ScanSetting.m_BufferSize, m_ScanSetting.m_SubId, m_ScanSetting.m_LocalCommunicationId);
    if (result.IsFailure()) {
        result = ConvertResult(result);
        m_pCallContext->SignalFailure(result);
        m_pCallContext = nullptr;
        criticalSection.Unlock();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    nn::uds::CTR::ScanResultReader reader(m_ScanSetting.m_pBuffer);
    UdsNetworkDescription* pDescriptions = static_cast<UdsNetworkDescription*>(m_ScanSetting.m_pDescriptions);
    u32 num = 0;
    for (u32 i = 0; i < reader.GetCount() && i < LocalNetwork::NETWORK_DESCRIPTION_NUM; i++) {
        nn::uds::CTR::NetworkDescriptionReader descriptionReader = reader.GetNextDescription();
        result = descriptionReader.GetNetworkDescription(&pDescriptions[i].m_Description);
        if (result.IsFailure()) {
            if (result == nn::uds::CTR::RESULT_BEACON_WITHOUT_NETWORK) {
                m_pCallContext->SignalFailure(common::RESULT_INVALID_STATE);
            } else {
                m_pCallContext->SignalFailure(GetNetworkManager()->ConvertUdsResult(result));
            }
            m_pCallContext = nullptr;
            criticalSection.Unlock();
            return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
        }
        num++;
    }
    *m_ScanSetting.m_pDescriptionNum = num;
    if (m_pCallContext->IsCancelRequested()) {
        m_pCallContext->SignalFailure(common::RESULT_INVALID_STATE_103);
    } else {
        m_pCallContext->SignalSuccess(result);
    }
    m_pCallContext = nullptr;
    criticalSection.Unlock();
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x0041EBF8 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::local::UdsBackgroundProcessJob::CreateNetwork()
{
    nn::Result result = GetNetworkManager()->CreateSystemHandle();
    if (result.IsFailure()) {
        m_pCallContext->SignalFailure(result);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    result = nn::uds::CTR::CreateNetwork(m_CreateSetting.m_SubId, m_CreateSetting.m_NodeCountMax, m_CreateSetting.m_LocalCommunicationId, m_CreateSetting.m_Passphrase,
                                         m_CreateSetting.m_PassphraseSize, m_CreateSetting.m_Channel, m_CreateSetting.m_ApplicationData, m_CreateSetting.m_ApplicationDataSize);
    if (result.IsFailure()) {
        result = ConvertResult(result);
        m_pCallContext->SignalFailure(result);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (m_pCallContext->IsCancelRequested()) {
        m_pCallContext->SignalCancel();
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    m_WaitCount = 0;
    SetStep(&UdsBackgroundProcessJob::WaitCreateNetworkEvent, "UdsBackgroundProcessJob::WaitCreateNetworkEvent");
    return common::ExecuteResult(common::ExecuteResult::STATE_WAIT, WAIT_INTERVAL_MSEC);
}

// 0x0041ED64 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::local::UdsBackgroundProcessJob::ConnectNetwork()
{
    nn::Result result = GetNetworkManager()->CreateSystemHandle();
    if (result.IsFailure()) {
        m_pCallContext->SignalFailure(result);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    result = nn::uds::CTR::ConnectNetwork(static_cast<const UdsNetworkDescription*>(m_ConnectSetting.m_pDescription)->m_Description, nn::uds::CTR::CONNECT_TYPE_CLIENT,
                                          m_ConnectSetting.m_Passphrase, m_ConnectSetting.m_PassphraseSize);
    if (result.IsFailure()) {
        if (result == nn::uds::CTR::RESULT_NOT_AUTHORIZED_STATE) {
            result = common::RESULT_LOCAL_NETWORK_UNAVAILABLE;
        } else if (result == nn::uds::CTR::RESULT_NOT_FOUND_1018) {
            result = common::RESULT_LOCAL_CONNECT_FAILED_108;
        } else if (result == nn::uds::CTR::RESULT_OUT_OF_RESOURCE_1) {
            result = common::RESULT_LOCAL_CONNECT_FAILED_109;
        } else if (result == nn::uds::CTR::RESULT_CANCELED_1019) {
            result = common::RESULT_JOIN_DENIED;
        } else {
            result = GetNetworkManager()->ConvertUdsResult(result);
        }
        m_pCallContext->SignalFailure(result);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (m_pCallContext->IsCancelRequested()) {
        m_pCallContext->SignalCancel();
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (LocalNetwork::s_pInstance->IsEnableHostMigration()) {
        result = LocalNetwork::s_pInstance->m_pMigrationManager->SetNetworkInfo(m_ConnectSetting);
        if (result.IsFailure()) {
            m_pCallContext->SignalFailure(result);
            m_pCallContext = nullptr;
            return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
        }
    }
    m_WaitCount = 0;
    SetStep(&UdsBackgroundProcessJob::WaitConnectNetworkEvent, "UdsBackgroundProcessJob::WaitConnectNetworkEvent");
    return common::ExecuteResult(common::ExecuteResult::STATE_WAIT, WAIT_INTERVAL_MSEC);
}

// 0x0041EF58 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::local::UdsBackgroundProcessJob::DestroyNetwork()
{
    nn::Result result = nn::uds::CTR::DestroyNetwork();
    GetNetworkManager()->DestroySystemHandle();
    if (m_pCallContext->IsCancelRequested()) {
        m_pCallContext->SignalCancel();
    } else if (result.IsSuccess()) {
        m_pCallContext->SignalSuccess(result);
    } else {
        result = ConvertResult(result);
        m_pCallContext->SignalFailure(result);
    }
    m_pCallContext = nullptr;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x0041F000 | fefates:bytes [tier B]
nn::pia::common::ExecuteResult nn::pia::local::UdsBackgroundProcessJob::DisconnectNetwork()
{
    if (LocalNetwork::s_pInstance->IsHost() || LocalNetwork::s_pInstance->IsClient()) {
        // (a debug log call was removed by the linker here)
        nn::Result result = nn::uds::CTR::DisconnectNetwork();
        if (result.IsFailure()) {
            result = ConvertResult(result);
            m_pCallContext->SignalFailure(result);
            m_pCallContext = nullptr;
            return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
        }
    }
    GetNetworkManager()->DestroySystemHandle();
    if (m_pCallContext->IsCancelRequested()) {
        m_pCallContext->SignalCancel();
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    m_pCallContext->SignalSuccess(nn::Result());
    m_pCallContext = nullptr;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x0041F0E8
nn::Result nn::pia::local::UdsBackgroundProcessJob::StartupScanNetwork(nn::pia::common::CallContext* pCallContext, const nn::pia::local::LocalScanNetworkSetting* pSetting)
{
    if (Startup(pCallContext, JOB_PRIORITY_SCAN_NETWORK).IsFailure()) {
        return common::RESULT_INVALID_STATE;
    }
    m_ScanSetting = *pSetting;
    SetStep(&UdsBackgroundProcessJob::ScanNetwork, "UdsBackgroundProcessJob::ScanNetwork");
    return nn::Result();
}

// 0x0041F16C
nn::Result nn::pia::local::UdsBackgroundProcessJob::StartupCreateNetwork(nn::pia::common::CallContext* pCallContext, const nn::pia::local::LocalCreateNetworkSetting* pSetting)
{
    std::memcpy(&m_CreateSetting, pSetting, sizeof(m_CreateSetting));
    if (m_CreateSetting.m_SubId > 254) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_CreateSetting.m_PassphraseSize < LocalCreateNetworkSetting::PASSPHRASE_SIZE_MIN || m_CreateSetting.m_PassphraseSize > LocalCreateNetworkSetting::PASSPHRASE_SIZE_MAX) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    // any channel or 1, 6, 11
    u8 channel = m_CreateSetting.m_Channel;
    if (channel != 0 && channel != 1 && channel != 6 && channel != 11) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (LocalNetwork::s_pInstance->GetBeaconApplicationDataSizeMax() < m_CreateSetting.m_ApplicationDataSize) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (GetNetworkManager()->MakeBeaconForCreateNetwork(&m_CreateSetting).IsFailure()) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (Startup(pCallContext, JOB_PRIORITY_CREATE_OR_CONNECT_NETWORK).IsFailure()) {
        return common::RESULT_INVALID_STATE;
    }
    SetStep(&UdsBackgroundProcessJob::CreateNetwork, "UdsBackgroundProcessJob::CreateNetwork");
    common::g_SessionBeginMonitoringContent.m_LocalCommunicationId = m_CreateSetting.m_LocalCommunicationId;
    common::g_SessionBeginMonitoringContent.m_NodeCountMax = m_CreateSetting.m_NodeCountMax;
    return nn::Result();
}

// 0x0041F298
nn::Result nn::pia::local::UdsBackgroundProcessJob::StartupConnectNetwork(nn::pia::common::CallContext* pCallContext, const nn::pia::local::LocalConnectNetworkSetting* pSetting)
{
    std::memcpy(&m_ConnectSetting, pSetting, sizeof(m_ConnectSetting));
    if (m_ConnectSetting.m_PassphraseSize < LocalCreateNetworkSetting::PASSPHRASE_SIZE_MIN || m_ConnectSetting.m_PassphraseSize > LocalCreateNetworkSetting::PASSPHRASE_SIZE_MAX) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    // the beacon of the network has to come from pia with the same application version
    LocalNetworkManager* pManager = LocalNetwork::s_pInstance->m_pNetworkManager;
    common::CriticalSection& criticalSection = pManager->m_SystemDataCriticalSection;
    criticalSection.Lock();
    const LocalBeaconSystemData* pSystemData = LocalNetwork::s_pInstance->m_pNetworkManager->GetSystemData(m_ConnectSetting.m_pDescription);
    nn::Result result = common::RESULT_INVALID_ARGUMENT;
    if (pSystemData != nullptr) {
        result = common::RESULT_LOCAL_CONNECT_FAILED_106;
        if (pSystemData->m_Version == LocalBeaconSystemData::VERSION &&
            LocalNetwork::s_pInstance->m_pNetworkManager->GetApplicationVersion() == pSystemData->m_ApplicationVersion) {
            LocalNetwork::s_pInstance->m_pNetworkManager->m_Unknown0x1238 = pSystemData->m_Unknown0x0;
            LocalNetwork::s_pInstance->m_pNetworkManager->SetSessionId(pSystemData->m_SessionId);
            criticalSection.Unlock();
            if (Startup(pCallContext, JOB_PRIORITY_CREATE_OR_CONNECT_NETWORK).IsFailure()) {
                return common::RESULT_INVALID_STATE;
            }
            SetStep(&UdsBackgroundProcessJob::ConnectNetwork, "UdsBackgroundProcessJob::ConnectNetwork");
            common::g_SessionBeginMonitoringContent.m_LocalCommunicationId = m_ConnectSetting.m_pDescription->GetLocalCommunicationId();
            common::g_SessionBeginMonitoringContent.m_NodeCountMax = m_ConnectSetting.m_pDescription->GetMaxParticipants();
            return nn::Result();
        }
    }
    criticalSection.Unlock();
    return result;
}

// 0x0041F41C | fefates:bytes
nn::Result nn::pia::local::UdsBackgroundProcessJob::StartupDestroyNetwork(nn::pia::common::CallContext* pCallContext)
{
    nn::Result result = Startup(pCallContext, JOB_PRIORITY_DESTROY_NETWORK);
    if (result.IsFailure()) {
        return result;
    }
    m_IsDestroyPrepared = false;
    SetStep(&UdsBackgroundProcessJob::DestroyNetwork, "UdsBackgroundProcessJob::DestroyNetwork");
    return nn::Result();
}

// 0x0041F488
nn::pia::common::ExecuteResult nn::pia::local::UdsBackgroundProcessJob::WaitCreateNetworkEvent()
{
    if (m_pCallContext->IsCancelRequested()) {
        m_pCallContext->SignalCancel();
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (m_WaitCount > CREATE_NETWORK_WAIT_COUNT_MAX) {
        m_pCallContext->SignalFailure(common::RESULT_TIMEOUT);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (LocalNetwork::s_pInstance->IsHost()) {
        m_pCallContext->SignalSuccess(nn::Result());
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    m_WaitCount++;
    return common::ExecuteResult(common::ExecuteResult::STATE_WAIT, WAIT_INTERVAL_MSEC);
}

// 0x0041F544
nn::pia::common::ExecuteResult nn::pia::local::UdsBackgroundProcessJob::WaitConnectNetworkEvent()
{
    if (m_pCallContext->IsCancelRequested()) {
        m_pCallContext->SignalCancel();
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (m_WaitCount > CONNECT_NETWORK_WAIT_COUNT_MAX) {
        m_pCallContext->SignalFailure(common::RESULT_TIMEOUT);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (LocalNetwork::s_pInstance->IsClient()) {
        m_pCallContext->SignalSuccess(nn::Result());
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    m_WaitCount++;
    return common::ExecuteResult(common::ExecuteResult::STATE_WAIT, WAIT_INTERVAL_MSEC);
}

// 0x0041F600 | fefates:bytes
nn::Result nn::pia::local::UdsBackgroundProcessJob::StartupDisconnectNetwork(nn::pia::common::CallContext* pCallContext)
{
    nn::Result result = Startup(pCallContext, JOB_PRIORITY_DISCONNECT_NETWORK);
    if (result.IsFailure()) {
        return result;
    }
    m_IsDisconnectPrepared = false;
    SetStep(&UdsBackgroundProcessJob::DisconnectNetwork, "UdsBackgroundProcessJob::DisconnectNetwork");
    return nn::Result();
}

// 0x0041F670 | fefates:bytes [tier B]
nn::pia::local::UdsBackgroundProcessJob::UdsBackgroundProcessJob() : m_CreateSetting(), m_ScanSetting(), m_ConnectSetting(), m_WaitCount(0)
{
}

// 0x00420268
// 0x0041F6FC (deleting dtor)
nn::pia::local::UdsBackgroundProcessJob::~UdsBackgroundProcessJob()
{
    // empty (in the original too)
}

// 0x00731688
void nn::pia::local::UdsBackgroundProcessJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
