#pragma once

#include "decomp.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_PRUDPMessageSelector.h"
#include "nn/nex/nex_RefCountedObject.h"
#include "nn/nex/nex_qVector.h"
#include "nn/nex/nex_StationURL.h"
#include "nn/nex/nex_PseudoSingleton.h"
#include "nn/nex/nex_qList.h"
#include "nn/nex/nex_qProtectedList.h"
#include "nn/nex/nex_ConnectivityManager.h"
#include "nn/nex/nex_InstanceTable.h"
#include "nn/nex/nex_LiteBuffer.h"
#include "nn/nex/nex_PacketBufferManager.h"
#include "nn/nex/nex_RC4Encryption.h"

namespace nn {
namespace nex {
class EndPoint;
class RootTransport;
class HMACChecksum;
class MD5ChecksumWithKey;
class NATTraversalEngine;
class CompressionAlgorithm;
class TransportAdapter;
// The network of nex: the station URLs of the console, the transport, the NAT traversal engine and
// the algorithms of the streams. The names of the members and of the functions without a reference
// name are ours.
// RTTI N2nn3nex7NetworkE @ 0x008CF680
// vtable 0x008FFB68 (vptr 0x008FFB70), offset_to_top 0, 2 entries
class Network : public ::nn::nex::RefCountedObject
{
public:
    static const u32 KIND = 1;
    static const u32 LOCK_LEVEL = 0x20000000;

    Network(); // 0x003D2E14 | fefates:callgraph [tier C]
    virtual ~Network(); // 0x003D3434 slot 0x00
    // 0x003D3404 slot 0x04 (deleting dtor)
    static void ReleaseInstance(); // 0x003D2980 | fefates:bytes [tier B]
    static Network* GetInstance(); // 0x003D2828 | fefates:callgraph [tier C]
    // (where the original inlines GetInstance)
    static Network* GetInstanceInline()
    {
        InstanceControl* pInstance = InstanceTable::s_Instance.GetInstance(KIND, PseudoSingleton::GetCurrentContext());
        return pInstance != nullptr ? static_cast<Network*>(pInstance->m_pOwner) : nullptr;
    }
    // a copy of the station URLs (inline in the original, with a copy out of line)
    void GetStationURLs(nn::nex::qList<nn::nex::StationURL>& urls); // 0x0072E438 (name is ours)
    // the station URL replaces the one with the same address; false if there is none
    bool UpdateStationURL(const nn::nex::StationURL& url); // 0x003D29D0 (name is ours)
    // the local URL with the address and port of target becomes a copy of url; false if there is none
    bool ReplaceLocalURL(const nn::nex::StationURL& target, const nn::nex::StationURL& url); // 0x003D2BB0 | fefates:callgraph [tier C]
    // creates the instance or takes a reference to it (the name is ours; inline in some callers)
    static void AcquireInstance(); // 0x003D2DC4
    static void AcquireInstanceInline()
    {
        if (GetInstance() != nullptr) {
            GetInstance()->AcquireRef();
        } else {
            new Network();
        }
    }
    // the station URL is added if there is none with its address and port (the name is ours)
    void AddStationURL(const nn::nex::StationURL& url); // 0x003D263C
    // the key of the P2P checksum (the configured one, else the default one)
    void InitP2PChecksumAlgorithmKey(); // 0x003D2D04 | fefates:callgraph [tier C]

    TransportAdapter* m_pTransportAdapter;            // 0x0C (made by the platform factory)
    CriticalSection m_Lock;                           // 0x10
    PseudoSingleton m_Singleton;                      // 0x38
    // the station URLs of the console
    qProtectedList<StationURL> m_StationUrls;         // 0x4C
    RootTransport* m_pRootTransport;                  // 0x90
    PacketBufferManager m_PacketBufferManager;        // 0x94
    ConnectivityManager m_ConnectivityManager;        // 0xD4
    NATTraversalEngine* m_pNATTraversalEngine;        // 0x100
    // the message format of the PRUDP streams
    PRUDPMessageSelector m_MessageSelector;           // 0x104
    RC4Encryption m_Encryption;                       // 0x118
    CompressionAlgorithm* m_pCompression;             // 0x348
    HMACChecksum* m_pChecksum;                        // 0x34C (of the secure streams)
    MD5ChecksumWithKey* m_pP2PChecksum;               // 0x350
    LiteBuffer<32> m_Unknown0x354;                    // 0x354
    // the ids whose packets were all acknowledged (SlidingWindow::Acknowledge), under m_IdLock
    qVector<u32> m_ReleasedIds;                       // 0x37C
    CriticalSection m_IdLock;                         // 0x388
    // an end point that failed (PRUDPEndPoint::ProcessFaultEvent)
    struct FaultedEndPoint
    {
        EndPoint* m_pEndPoint; // 0x0
        u32 m_ConnectionId;    // 0x4
    };
    qVector<FaultedEndPoint> m_FaultedEndPoints;      // 0x3B0
    CriticalSection m_FaultLock;                      // 0x3BC
};
ASSERT_SIZE(Network, 0x3E4);
ASSERT_OFFSET(Network, m_StationUrls, 0x4C);
ASSERT_OFFSET(Network, m_pRootTransport, 0x90);
ASSERT_OFFSET(Network, m_PacketBufferManager, 0x94);
ASSERT_OFFSET(Network, m_ConnectivityManager, 0xD4);
ASSERT_OFFSET(Network, m_Encryption, 0x118);
ASSERT_OFFSET(Network, m_ReleasedIds, 0x37C);
ASSERT_OFFSET(Network, m_IdLock, 0x388);

// the settings of the algorithms of the network (the names are ours)
extern u8 g_CompressionMode;            // 1: zlib
extern bool g_UsesEncryption;
extern bool g_UsesSecureChecksum;
extern bool g_UsesP2PChecksum;
extern s32 g_CompressionWindowBits;
extern s32 g_CompressionMemoryLevel;
extern u32 g_P2PChecksumKeyLength;
extern u8 g_P2PChecksumKey[32];
} // namespace nex
} // namespace nn
