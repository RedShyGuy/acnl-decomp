#include "nn/pia/local/local_UdsMatchmakeSession.h"
#include <string.h>
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/local/local_LocalCreateSessionSetting.h"
#include "nn/pia/local/local_LocalNetwork.h"
#include "nn/pia/local/local_UdsSessionInfo.h"
#include "nn/pia/session/session_Session.h"
#include "nn/pia/session/session_SessionInfoList.h"

namespace nn {
namespace pia {
namespace local {
namespace {
inline nn::Result ConvertResult(nn::Result result)
{
    if (result == common::RESULT_LOCAL_NETWORK_LOST) {
        return common::RESULT_NOT_IN_SESSION;
    }
    return result;
}
} // namespace

// 0x0041A79C
bool nn::pia::local::UdsMatchmakeSession::IsBrowseCompleted()
{
    if (!m_pNetwork->IsScanNetworkAsyncCompleted()) {
        return false;
    }
    m_Request = REQUEST_NONE;
    nn::Result result = m_pNetwork->GetScanNetworkAsyncResult();
    if (result.IsFailure()) {
        m_pCallContext->SignalFailure(ConvertResult(result));
        m_pCallContext = nullptr;
        return true;
    }

    // the matching networks after the offset, as many as the criteria allow
    u32 resultOffset = m_SearchCriteria.m_ResultOffset;
    u32 resultNumMax = m_SearchCriteria.m_ResultNumMax;
    u32 matchNum = 0;
    u32 resultNum = 0;
    m_pSessionInfoList->Clear();
    for (u32 i = 0; i < LocalNetwork::s_pInstance->m_DescriptionNum; i++) {
        if (LocalNetwork::s_pInstance->GetNetworkDescription(&m_ScanDescriptions[i], i).IsFailure()) {
            break;
        }
        if (!m_SearchCriteria.IsMatch(&m_ScanDescriptions[i])) {
            continue;
        }
        if (matchNum < resultOffset) {
            matchNum++;
            continue;
        }
        matchNum++;
        if (resultNum >= resultNumMax) {
            continue;
        }
        UdsSessionInfo* pInfo = m_pSessionInfoList->Add();
        if (pInfo == nullptr) {
            break;
        }
        pInfo->SetNetworkDescription(&m_ScanDescriptions[i]);
        pInfo->UpdateLinkLevel(i);
        pInfo->UpdateStationInfos(i);
        pInfo->Trace(32);
        resultNum++;
    }
    m_pCallContext->SignalSuccess(nn::Result());
    m_pCallContext = nullptr;
    return true;
}

// 0x0041A98C
bool nn::pia::local::UdsMatchmakeSession::IsCreateCompleted(u32* pSessionId)
{
    if (!m_pNetwork->IsCreateNetworkAsyncCompleted()) {
        return false;
    }
    m_Request = REQUEST_NONE;
    nn::Result result = m_pNetwork->GetCreateNetworkAsyncResult();
    if (result.IsFailure()) {
        m_pCallContext->SignalFailure(ConvertResult(result));
        m_pCallContext = nullptr;
        return true;
    }
    *pSessionId = m_pNetwork->GetSessionId();
    m_Unknown0x4 = true;
    m_pCallContext->SignalSuccess(nn::Result());
    m_pCallContext = nullptr;
    return true;
}

// 0x0041AA20
nn::pia::session::ISessionInfoList* nn::pia::local::UdsMatchmakeSession::GetSessionInfoList()
{
    return m_pSessionInfoList;
}

// 0x0041AA28
void nn::pia::local::UdsMatchmakeSession::SetCreateSessionSetting(const nn::pia::local::LocalCreateSessionSetting* pSetting)
{
    LocalCreateNetworkSetting* pDst = m_pCreateNetworkSetting;
    // (the slot is not const)
    const LocalCreateNetworkSetting* pSrc = const_cast<LocalCreateSessionSetting*>(pSetting)->GetCreateNetworkSetting();
    pDst->m_SubId = pSrc->m_SubId;
    pDst->m_NodeCountMax = pSrc->m_NodeCountMax;
    pDst->m_LocalCommunicationId = pSrc->m_LocalCommunicationId;
    pDst->m_Channel = pSrc->m_Channel;
    memcpy(pDst->m_Passphrase, pSrc->m_Passphrase, pSrc->m_PassphraseSize);
    pDst->m_PassphraseSize = pSrc->m_PassphraseSize;
    memcpy(pDst->m_ApplicationData, pSrc->m_ApplicationData, pSrc->m_ApplicationDataSize);
    pDst->m_ApplicationDataSize = pSrc->m_ApplicationDataSize;
}

// 0x0041AAAC
nn::Result nn::pia::local::UdsMatchmakeSession::SetJoinSetting(const nn::pia::local::LocalSessionInfo* pInfo, const void* pPassphrase, u32 passphraseSize)
{
    if (passphraseSize > LocalConnectNetworkSetting::PASSPHRASE_SIZE_MAX) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    memcpy(m_ConnectNetworkSetting.m_Passphrase, pPassphrase, passphraseSize);
    m_ConnectNetworkSetting.m_PassphraseSize = passphraseSize;
    m_NetworkDescription = *static_cast<const UdsNetworkDescription*>(pInfo->GetNetworkDescription());
    m_ConnectNetworkSetting.m_pDescription = &m_NetworkDescription;
    return nn::Result();
}

// 0x0041AB10
nn::pia::local::UdsMatchmakeSession::UdsMatchmakeSession() : m_ConnectNetworkSetting(), m_NetworkDescription()
{
    m_pCreateNetworkSetting = common::NewObject<LocalCreateNetworkSetting>();
    m_pConnectNetworkSetting = common::NewObject<LocalConnectNetworkSetting>();
    m_pSessionInfoList = static_cast<session::SessionInfoList<UdsSessionInfo>*>(session::Session::s_pInstance->m_pSessionInfoList);
}

// 0x0041AC70
// 0x0041AC14 (deleting dtor)
nn::pia::local::UdsMatchmakeSession::~UdsMatchmakeSession()
{
    if (m_pCreateNetworkSetting != nullptr) {
        common::DeleteObject(m_pCreateNetworkSetting);
        m_pCreateNetworkSetting = nullptr;
    }
    if (m_pConnectNetworkSetting != nullptr) {
        common::DeleteObject(m_pConnectNetworkSetting);
        m_pConnectNetworkSetting = nullptr;
    }
    m_pSessionInfoList = nullptr;
}

// 0x007311D4
nn::pia::local::LocalConnectNetworkSetting* nn::pia::local::UdsMatchmakeSession::GetConnectNetworkSetting()
{
    return &m_ConnectNetworkSetting;
}

} // namespace local
} // namespace pia
} // namespace nn
