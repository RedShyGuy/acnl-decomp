#pragma once

#include "decomp.h"
#include "nn/nex/nex_InetAddress.h"
#include "nn/nex/nex_RelayStream.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_RootTransport.h"

namespace nn {
namespace nex {
class Buffer;
class Network;

// The relay of a transport (SocketTransport +0x214, RootTransport::GetRelay): with it enabled the
// relay stream listens on port 15 and the packets for stations behind a relay server go through
// it. The names of the members and of the functions without a reference name are ours.
// RTTI N2nn3nex5RelayE @ 0x008CF544
// vtable 0x008FF9EC (vptr 0x008FF9F4), offset_to_top 0, 2 entries
class Relay : public ::nn::nex::RootObject
{
public:
    // the port of the relay stream
    static const u8 PORT = 15;

    // (inline in the constructor of SocketTransport)
    Relay() : m_pStream(new RelayStream()), m_IsEnabled(false), m_IsRegistered(false), m_pNetwork(nullptr) {}
    virtual ~Relay(); // 0x003CF1E4 slot 0x00
    // 0x003CF154 (deleting dtor)
    // true if the packets to the address go through the relay server
    bool ShouldRelay(const nn::nex::InetAddress* pAddress); // 0x003CEDB4 | fefates:bytes [tier B]
    // sends the data (with the relay header) to the relay server of the address
    bool Send(nn::nex::Buffer* pData, const nn::nex::InetAddress* pAddress, unsigned short port); // 0x003CEE54 (name is ours)
    void EnableRelay(bool enable); // 0x003CEFC4 | fefates:callgraph [tier C]
    // the routes and the relay server are forgotten
    void Clear(); // 0x003CF054 (name is ours)

    // (inline in the constructor of SocketTransport) the relay stream belongs to the transport
    void SetTransport(RootTransport* pTransport)
    {
        if (m_IsRegistered) {
            if (m_pStream->m_pTransport != nullptr) {
                m_IsRegistered = !m_pStream->m_pTransport->m_StreamManager.UnregisterStream(static_cast<Stream::Type>(m_pStream->m_Type), PORT);
            }
            Clear();
        }
        m_pStream->m_pTransport = pTransport;
        if (m_IsEnabled) {
            m_IsRegistered = pTransport->m_StreamManager.RegisterStream(m_pStream, PORT, nullptr);
        }
    }

    RelayStream* m_pStream;  // 0x04
    bool m_IsEnabled;        // 0x08
    bool m_IsRegistered;     // 0x09
    Network* m_pNetwork;     // 0x0C
    InetAddress m_Address;   // 0x10 (this station as the relay server knows it)
};
ASSERT_SIZE(Relay, 0x30);
} // namespace nex
} // namespace nn
