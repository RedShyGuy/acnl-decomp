#include "nn/pia/session/session_MeshLayerController.h"
#include "nn/nstd/nstd_String.h"
#include "nn/pia/common/common_CryptoSetting.h"
#include "nn/pia/session/session_CommonMatchmakeSession.h"
#include "nn/pia/session/session_Mesh.h"
#include "nn/pia/session/session_MeshEventListenerForSession.h"
#include "nn/pia/session/session_Session.h"
#include "nn/pia/transport/transport_Transport.h"
#include <string.h>

namespace nn {
namespace pia {
namespace session {
// 0x00439CDC (name is ours)
void nn::pia::session::MeshLayerController::CleanupMesh()
{
    if (Mesh::s_pInstance == nullptr) {
        return;
    }
    Mesh::s_pInstance->Cleanup();
    Mesh::s_pInstance->ClearEventListener();
    Mesh::s_pInstance->ClearJoinApprovalCallback();
    Mesh::s_pInstance->ClearHostCandidateCallback();
    Mesh::s_pInstance->ClearIdentificationCallback();
}

// 0x00439D20 (name is ours)
nn::Result nn::pia::session::MeshLayerController::StartupMesh(nn::pia::session::CommonMatchmakeSession* pSession, bool)
{
    // (the base ignores the flag)
    return StartupMeshCore(pSession, false);
}

// 0x00439D28 (name is ours)
nn::Result nn::pia::session::MeshLayerController::StartupMeshCore(nn::pia::session::CommonMatchmakeSession* pSession, bool isBandwidthCheckOneWay)
{
    Mesh::s_pInstance->m_pEventListener = Session::s_pInstance->m_pMeshEventListener;
    Mesh::s_pInstance->m_JoinApprovalCallback = Session::CheckJoinApproval;
    Mesh::s_pInstance->SetHostCandidateCallback(Session::GetHostCandidatePriority);
    Mesh::s_pInstance->m_IdentificationCallback = Session::GetCurrentSessionId;

    const u8* pIdentificationData = nullptr;
    if (m_IsIdentificationDataSet) {
        pIdentificationData = m_IdentificationData;
    }
    common::CryptoSetting cryptoSetting;
    cryptoSetting.m_Mode = m_CryptoMode;
    // the key of the signature is the key of the encryption too
    const common::SignatureSetting* pSignatureSetting = pSession->GetSignatureSetting();
    if (cryptoSetting.m_Mode != common::Crypto::MODE_NONE) {
        if (pSignatureSetting->m_KeySize < common::CryptoSetting::KEY_SIZE) {
            memset(cryptoSetting.m_Key, 0, sizeof(cryptoSetting.m_Key));
            memcpy(cryptoSetting.m_Key, pSignatureSetting->m_pKey, pSignatureSetting->m_KeySize);
        } else {
            memcpy(cryptoSetting.m_Key, pSignatureSetting->m_pKey, sizeof(cryptoSetting.m_Key));
        }
    }

    Mesh::StartupSetting setting;
    setting.m_TimeoutMSec = m_TimeoutMSec;
    setting.m_KeepAliveIntervalMSec = m_KeepAliveIntervalMSec;
    setting.m_IsHostMigrationEnabled = m_IsHostMigrationEnabled;
    setting.m_pIdentificationData = pIdentificationData;
    setting.m_pCryptoSetting = &cryptoSetting;
    setting.m_SignatureSetting.Set(pSignatureSetting->m_Mode, pSignatureSetting->m_pKey, pSignatureSetting->m_KeySize);
    setting.m_BandwidthCheckBandwidth = m_BandwidthCheckBandwidth;
    setting.m_BandwidthCheckPacketSize = m_BandwidthCheckPacketSize;
    setting.m_BandwidthCheckDurationMSec = m_BandwidthCheckDurationMSec;
    if (isBandwidthCheckOneWay) {
        setting.m_IsBandwidthCheckOneWay = isBandwidthCheckOneWay;
    } else {
        setting.m_IsBandwidthCheckOneWay = m_IsBandwidthCheckOneWay;
    }
    setting.m_pPlayerName = &m_PlayerName;
    nn::Result result = Mesh::s_pInstance->Startup(setting);
    if (result.IsFailure()) {
        vf_0x14();
    }
    return result;
}

// 0x00439F24 (name is ours)
void nn::pia::session::MeshLayerController::vf_0x38()
{
    // empty (in the original too)
}

// 0x00439F28 (name is ours)
void nn::pia::session::MeshLayerController::SetOtherSessionId(u32 sessionId)
{
    Session* pSession = Session::s_pInstance;
    if (pSession != nullptr) {
        pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0] = sessionId;
    }
}

// 0x00439F58 (name is ours)
void nn::pia::session::MeshLayerController::SetCurrentSessionId(u32 sessionId)
{
    Session* pSession = Session::s_pInstance;
    if (pSession != nullptr) {
        pSession->m_SessionIds[pSession->m_CurrentIndex] = sessionId;
    }
}

// 0x00439F7C (name is ours)
nn::pia::session::JointSessionJob* nn::pia::session::MeshLayerController::GetJointSessionJob()
{
    Session* pSession = Session::s_pInstance;
    if (pSession == nullptr) {
        return nullptr;
    }
    return pSession->m_pJointSessionJob;
}

// 0x00439F94
bool nn::pia::session::MeshLayerController::HasOtherSession(u32, u32)
{
    return false;
}

// 0x00439F9C (name is ours)
void nn::pia::session::MeshLayerController::ClearUnknown0x69()
{
    transport::Transport::s_pInstance->m_pStationIdCallback = nullptr;
    m_Unknown0x69 = false;
}

// 0x00439FB8 (name is ours)
nn::Result nn::pia::session::MeshLayerController::Startup(bool isHostMigrationEnabled, const u8* pIdentificationData, nn::pia::common::Crypto::Mode cryptoMode,
                                                         u32 timeoutMSec, u32 keepAliveIntervalMSec, s32 bandwidthCheckBandwidth,
                                                         u32 bandwidthCheckPacketSize, bool isBandwidthCheckOneWay, s32 bandwidthCheckDurationMSec,
                                                         const nn::pia::transport::Station::PlayerName* pPlayerName, bool value)
{
    m_CryptoMode = cryptoMode;
    m_TimeoutMSec = timeoutMSec;
    m_KeepAliveIntervalMSec = keepAliveIntervalMSec;
    m_IsHostMigrationEnabled = isHostMigrationEnabled;
    m_BandwidthCheckBandwidth = bandwidthCheckBandwidth;
    m_BandwidthCheckPacketSize = bandwidthCheckPacketSize;
    m_BandwidthCheckDurationMSec = bandwidthCheckDurationMSec;
    m_IsBandwidthCheckOneWay = isBandwidthCheckOneWay;
    if (pIdentificationData == nullptr) {
        m_IsIdentificationDataSet = false;
    } else {
        m_IsIdentificationDataSet = true;
        // the UTF-16 characters (their number at 34), then the bytes 35 and 34
        nnnstdMemCpy(m_IdentificationData, pIdentificationData, pIdentificationData[34] * 2);
        m_IdentificationData[35] = pIdentificationData[35];
        m_IdentificationData[34] = pIdentificationData[34];
    }
    if (pPlayerName != nullptr) {
        nnnstdMemCpy(&m_PlayerName, pPlayerName, sizeof(m_PlayerName));
    } else {
        memset(&m_PlayerName, 0, sizeof(m_PlayerName));
    }
    transport::Transport::s_pInstance->m_pStationIdCallback = Session::s_pInstance->GetStationIdCallback();
    m_Unknown0x69 = value;
    return nn::Result();
}

// 0x00733920 (name is ours)
bool nn::pia::session::MeshLayerController::vf_0x34() const
{
    return m_Unknown0x69;
}

} // namespace session
} // namespace pia
} // namespace nn
