#pragma once

#include "decomp.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_InetAddress.h"
#include "nn/nex/nex_InputEmulationDevice.h"
#include "nn/nex/nex_NATProperties.h"
#include "nn/nex/nex_OutputEmulationDevice.h"
#include "nn/nex/nex_PseudoGlobalVariable.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_TransportStreamManager.h"
#include "nn/nex/nex_UPnPProperties.h"
#include "nn/nex/nex_qList.h"
#include "nn/nex/nex_qResult.h"
#include "nn/nex/nex_qVector.h"

namespace nn {
namespace nex {
class Buffer;
class PacketOut;
class Relay;
class Router;
class StationURL;
class StringStream;
class VirtualFilterDevice;

// The transport of the streams: the sockets of the ports, the streams of the ports, the network
// emulation and the NAT properties of the console. The names of the members and of the functions
// without a reference name are ours.
// RTTI N2nn3nex13RootTransportE @ 0x008CE278
// vtable 0x008FC928 (vptr 0x008FC930), offset_to_top 0, 17 entries
class RootTransport : public ::nn::nex::RootObject
{
public:
    static const u32 LOCK_LEVEL = 0x20000000;
    // the size of the buffers of the packets
    static const u32 DEFAULT_MAX_PACKET_SIZE = 0x554;
    // the number of the URLs of the NAT check servers (2 servers, 2 ports each)
    static const u32 NAT_CHECK_URL_COUNT = 4;
    // the ports of the NAT check servers
    static const u16 NAT_CHECK_PORT_1 = 10025;
    static const u16 NAT_CHECK_PORT_2 = 10125;
    // the port of the relay server
    static const u16 RELAY_PORT = 12525;

    RootTransport(); // 0x0036B6A4
    virtual ~RootTransport(); // 0x0036BCD0 slot 0x00
    // 0x0036BCA0 slot 0x04 (deleting dtor)
    // starts the transport
    virtual nn::nex::qResult Start(); // 0x0036AED4 slot 0x08
    // listens on the port (0: a free one; pPort: the port; isNew: the URLs of the network get the port)
    virtual bool StartListen(unsigned short port, unsigned short* pPort, bool isNew, unsigned int context, bool unknown) = 0; // slot 0x0C
    virtual bool StartListenUnknown(unsigned short port, unsigned short* pPort, bool isNew, unsigned int context, bool unknown) = 0; // slot 0x10
    virtual bool StopListen(unsigned short port) = 0; // slot 0x14
    // stops listening on all ports
    virtual bool StopListenAll() = 0; // slot 0x18
    // the number of the ports
    virtual u32 GetPortCount() = 0; // slot 0x1C
    // sends the packet from the source port (stream type and port) to the destination port
    virtual bool SendPacket(unsigned short port, unsigned char streamType, unsigned char sourcePort, unsigned char destinationPort, nn::nex::PacketOut* pPacket,
                            bool unknown) = 0; // slot 0x20
    // the data from the address goes to the streams of the port; false if none took it
    virtual bool Receive(unsigned short port, nn::nex::Buffer* pData, const nn::nex::InetAddress* pAddress) = 0; // slot 0x24
    // the router and the relay of the transport (none here)
    virtual Router* GetRouter(); // 0x0036B69C slot 0x28
    virtual Relay* GetRelay(); // 0x0036B694 slot 0x2C
    virtual void* vf_0x30() = 0; // slot 0x30
    virtual void* vf_0x34() = 0; // slot 0x34
    // sends a raw buffer to the station of the URL
    virtual nn::nex::qResult SendRaw(nn::nex::StationURL* pURL, nn::nex::Buffer* pBuffer); // 0x0036B678 slot 0x38
    // the packets that use the pools of the packet buffers are dropped
    virtual void DropPoolPackets(); // 0x0036B59C slot 0x3C
    virtual void RequestStop() = 0; // slot 0x40

    // a random port from 49152 to 65534
    static u16 GetNextPortNumber(); // 0x0036B24C | mk7dlp:bytes [tier A]
    static RootTransport* GetInstance(); // 0x0036AEF0 | fefates:callgraph [tier C]
    // the well-known port of the transports (9203 by default)
    static u16 GetWellKnownPortNumber(); // 0x0036B278
    static void SetWellKnownPortNumber(unsigned short port); // 0x0036B2C4
    // the time of the check of a relay (milliseconds)
    static s32 GetCheckAndRelayTimeout(); // 0x0036B268
    void SetNATProperties(const nn::nex::NATProperties& properties); // 0x0036B148 | fefates:callgraph [tier C]
    nn::nex::NATProperties GetNATProperties() const; // 0x0072A99C | fefates:callgraph [tier C]
    // the data that no stream took goes to the data handlers; false if one took it
    bool ReceiveUnhandled(nn::nex::Buffer* pData, const nn::nex::InetAddress* pAddress); // 0x0036B04C
    // the address of the relay server is resolved (if the relay is enabled); false if it fails
    bool ResolveRelayAddress(); // 0x0036B1A0
    // the URLs of the NAT check servers (the addresses resolved, each with two ports)
    bool SetNATCheckServer1(const wchar_t* pServerName, unsigned short port1, unsigned short port2); // 0x0036B314
    bool SetNATCheckServer2(const wchar_t* pServerName, unsigned short port1, unsigned short port2); // 0x0036B458
    bool InitializeNATCheckURLs(); // 0x0036B5A0

    static PseudoGlobalVariable<u16> s_WellKnownPortNumber;
    static s32 s_CheckAndRelayTimeout;
    static const wchar_t* s_pNATCheckServerName1;
    static const wchar_t* s_pNATCheckServerName2;

    CriticalSection m_Unknown0x04;          // 0x04
    CriticalSection m_Unknown0x2C;          // 0x2C
    u32 m_MaxPacketSize;                    // 0x54 (the size of the buffers of the packets)
    u8 m_Unknown0x58;                       // 0x58
    OutputEmulationDevice m_OutputDevice;   // 0x5C
    InputEmulationDevice m_InputDevice;     // 0x78
    TransportStreamManager m_StreamManager; // 0x94
    // an object that may take a received packet (TransportStreamManager; slot 0x08 is called with the
    // URL of the sender and the port)
    void* m_pUnknown0xAC;                   // 0xAC
    u16 m_PortNumber;                       // 0xB0
    // the handlers of the data that no stream took (slot 0x08 of them is called with the data and the
    // URL of the sender)
    qList<void*> m_DataHandlers;            // 0xB4
    NATProperties m_NATProperties;          // 0xCC
    UPnPProperties m_UPnPProperties;        // 0xF0
    bool m_AreNATCheckURLsReady;            // 0xFC
    bool m_IsRelayAddressResolved;          // 0xFD
    bool m_Unknown0xFE;                     // 0xFE (the transport is closed: the end points fail)
    VirtualFilterDevice* m_pFilterDevice;   // 0x100
    qVector<StringStream*> m_NATCheckURLs;  // 0x104
    InetAddress m_RelayAddress;             // 0x110
    String m_RelayServerName;               // 0x130
};
ASSERT_OFFSET(RootTransport, m_MaxPacketSize, 0x54);
ASSERT_OFFSET(RootTransport, m_StreamManager, 0x94);
ASSERT_OFFSET(RootTransport, m_PortNumber, 0xB0);
ASSERT_OFFSET(RootTransport, m_NATProperties, 0xCC);
ASSERT_OFFSET(RootTransport, m_UPnPProperties, 0xF0);
ASSERT_OFFSET(RootTransport, m_RelayAddress, 0x110);
ASSERT_SIZE(RootTransport, 0x138);
} // namespace nex
} // namespace nn
