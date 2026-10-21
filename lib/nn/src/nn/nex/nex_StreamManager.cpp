#include "nn/nex/nex_StreamManager.h"
#include "nn/nex/nex_BackEndServices.h"
#include "nn/nex/nex_Network.h"
#include "nn/nex/nex_PRUDPStream.h"
#include "nn/nex/nex_ProtocolRequestBrokerInterface.h"
#include "nn/nex/nex_RootTransport.h"
#include "nn/nex/nex_StreamSettings.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
// 0x009712E4 (name is ours)
u32 StreamManager::s_KeySuffix;
// 0x009712E8 (name is ours)
f32 StreamManager::s_InputEmulationValue;
// 0x00AE06D0 (name is ours)
SystemSetting StreamManager::s_UseDistinctPort(L"UseDistinctPortForStreamManager", 0);

namespace {
// the timeouts of the stream (ms)
const u32 MAX_SILENCE_TIME = 30000;
const u32 KEEP_ALIVE_TIMEOUT = 5000;
// the job type of the connection manager
const u32 CONNECTION_JOB_TYPE = 2;
} // namespace

// 0x0036E478 | fefates:bytes-fuzzy [tier B]
bool nn::nex::StreamManager::Initialize(unsigned short port, unsigned char streamPort, unsigned int flags)
{
    if (m_IsInitialized) {
        return false;
    }
    m_pConnectionManager->m_IsAccepting = (flags & FLAG_ACCEPT_CONNECTIONS) != 0;
    bool isFailure = false;
    if (m_pStream->Initialize().Equals(isFailure)) {
        return false;
    }
    if (s_UseDistinctPort.m_Value != 0) {
        if (!m_pTransport->StartListen(port, &m_TransportPort, false, 2, false) && ((flags & FLAG_FIXED_PORTS) || !m_pTransport->StartListen(0, &m_TransportPort, false, 2, false))) {
            return false;
        }
        m_pStream->m_DefaultPort = m_TransportPort;
    } else {
        if (!m_pTransport->StartListen(port, &m_TransportPort, true, 0, false) && ((flags & FLAG_FIXED_PORTS) || !m_pTransport->StartListen(0, &m_TransportPort, true, 0, false))) {
            return false;
        }
    }
    if (!m_pStream->StartListen(streamPort, nullptr) && ((flags & FLAG_FIXED_PORTS) || !m_pStream->StartListen(0, nullptr))) {
        return false;
    }
    m_IsInitialized = true;
    return true;
}

// 0x0036E670 slot 0x14
void nn::nex::StreamManager::SetCredentials(nn::nex::Credentials* pCredentials)
{
    // nothing to do
}

// 0x0036E68C | fefates:bytes [tier B]
void nn::nex::StreamManager::AssociateSecureStream(nn::nex::ConnectionOrientedStream* pStream, nn::nex::ConnectionManager::ConnectionFilter pFilter)
{
    m_pSecureStream = pStream;
    m_pConnectionManager->AttachStream(pStream, false);
    UserContext context;
    context.m_pPointer = pStream;
    m_pConnectionManager->RegisterConnectionFilter(pFilter, context);
}

// 0x0036E6C4
nn::nex::StreamManager::StreamManager(nn::nex::Stream::Type type, nn::nex::RootTransport* pTransport) : m_pBroker(nullptr), m_pTransport(pTransport), m_Type(type)
{
    Network::AcquireInstance();
    if (s_KeySuffix != 0) {
        String key;
        key.Format(L"%s%d", BackEndServices::s_SandboxAccessKey, s_KeySuffix);
        Stream::s_Settings[m_Type].Get().SetKey(key);
    } else {
        Stream::s_Settings[m_Type].Get().SetKey(String(BackEndServices::s_SandboxAccessKey));
    }
    m_pStream = new PRUDPStream(static_cast<Stream::Type>(m_Type), m_pTransport);
    Stream::s_Settings[m_Type].Get().SetMaxSilenceTime(MAX_SILENCE_TIME);
    Stream::s_Settings[m_Type].Get().SetKeepAliveTimeout(KEEP_ALIVE_TIMEOUT);
    if (s_InputEmulationValue > 0.0f) {
        m_pTransport->m_InputDevice.Enable();
        m_pTransport->m_InputDevice.m_LossRate = s_InputEmulationValue;
    }
    m_pConnectionManager = new ConnectionManager(this, static_cast<Job::JobType>(CONNECTION_JOB_TYPE), true, false);
    m_pConnectionManager->AttachStream(m_pStream, true);
    m_IsInitialized = false;
    m_pSecureStream = nullptr;
    m_TransportPort = 0;
}

// 0x0036E988 slot 0x00 | fefates:bytes-fuzzy
// 0x0036E978 (deleting dtor)
nn::nex::StreamManager::~StreamManager()
{
    if (m_pSecureStream != nullptr) {
        delete m_pSecureStream;
        m_pSecureStream = nullptr;
    }
    if (m_IsInitialized) {
        m_pStream->StopListen();
        m_pTransport->StopListen(m_TransportPort);
        m_IsInitialized = false;
    }
    if (m_pStream != nullptr) {
        m_pStream->Terminate();
        delete m_pStream;
        m_pStream = nullptr;
    }
    delete m_pConnectionManager;
    delete m_pBroker;
    Network::ReleaseInstance();
}

// 0x0038677C
nn::nex::SystemComponent::_State nn::nex::StreamManager::Terminate()
{
    return m_pConnectionManager->m_State.Terminate();
}

// 0x0072C1DC | fefates:bytes [tier B]
bool nn::nex::StreamManager::IsTerminated() const
{
    return m_pConnectionManager->IsTerminated();
}

} // namespace nex
} // namespace nn
