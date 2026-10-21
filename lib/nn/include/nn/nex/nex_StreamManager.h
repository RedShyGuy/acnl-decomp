#pragma once

#include "decomp.h"
#include "nn/nex/nex_ConnectionManager.h"
#include "nn/nex/nex_EndPointEventHandler.h"
#include "nn/nex/nex_Stream.h"
#include "nn/nex/nex_SystemSetting.h"

namespace nn {
namespace nex {
class ConnectionOrientedStream;
class Credentials;
class PRUDPStream;
class ProtocolRequestBrokerInterface;
class RootTransport;

// The PRUDP stream of a back end with its ConnectionManager (and the secure stream). The names of
// the members and of the functions without a reference name are ours.
// RTTI N2nn3nex13StreamManagerE @ 0x008CE29C
// vtable 0x008FC9C0 (vptr 0x008FC9C8), offset_to_top 0, 6 entries
class StreamManager : public ::nn::nex::EndPointEventHandler
{
public:
    // the flags of Initialize
    static const u32 FLAG_ACCEPT_CONNECTIONS = 1 << 0;
    // only the given ports (no free one if they are taken)
    static const u32 FLAG_FIXED_PORTS = 1 << 1;

    StreamManager(nn::nex::Stream::Type type, nn::nex::RootTransport* pTransport); // 0x0036E6C4
    virtual ~StreamManager(); // 0x0036E988 slot 0x00 | fefates:bytes-fuzzy
    // 0x0036E978 (deleting dtor)
    virtual void Receive(nn::nex::EndPoint* pEndPoint, nn::nex::Buffer* pData, unsigned char substreamId) = 0; // slot 0x08
    virtual void FaultDetection(nn::nex::EndPoint* pEndPoint, unsigned int error) = 0; // slot 0x0C
    virtual void PeerDisconnected(nn::nex::EndPoint* pEndPoint) = 0; // slot 0x10
    virtual void SetCredentials(nn::nex::Credentials* pCredentials); // 0x0036E670 slot 0x14
    // the transport listens on the port, the stream on the stream port
    bool Initialize(unsigned short port, unsigned char streamPort, unsigned int flags); // 0x0036E478 | fefates:bytes-fuzzy [tier B]
    void AssociateSecureStream(nn::nex::ConnectionOrientedStream* pStream, nn::nex::ConnectionManager::ConnectionFilter pFilter); // 0x0036E68C | fefates:bytes [tier B]
    nn::nex::SystemComponent::_State Terminate(); // 0x0038677C
    bool IsTerminated() const; // 0x0072C1DC | fefates:bytes [tier B]

    // the number after the sandbox access key in the key of the streams (0: none)
    static u32 s_KeySuffix;
    // the value of the input emulation of the transport (if > 0)
    static f32 s_InputEmulationValue;
    // "UseDistinctPortForStreamManager": the transport listens on its own port
    static SystemSetting s_UseDistinctPort;

    PRUDPStream* m_pStream;                    // 0x0C
    ConnectionOrientedStream* m_pSecureStream; // 0x10
    ProtocolRequestBrokerInterface* m_pBroker; // 0x14
    ConnectionManager* m_pConnectionManager;   // 0x18
    bool m_IsInitialized;                      // 0x1C
    RootTransport* m_pTransport;               // 0x20
    u8 m_Type;                                 // 0x24
    u16 m_TransportPort;                       // 0x26
};
ASSERT_OFFSET(StreamManager, m_pStream, 0x0C);
ASSERT_SIZE(StreamManager, 0x28);
} // namespace nex
} // namespace nn
