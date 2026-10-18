#include "nn/nex/nex_Network.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_HMACChecksum.h"
#include "nn/nex/nex_Key.h"
#include "nn/nex/nex_MD5ChecksumWithKey.h"
#include "nn/nex/nex_NATTraversalEngine.h"
#include "nn/nex/nex_SocketDriver.h"
#include "nn/nex/nex_StationURL.h"
#include "nn/nex/nex_Stream.h"
#include "nn/nex/nex_StreamSettings.h"
#include "nn/nex/nex_TransportAdapter.h"
#include "nn/nex/nex_UDPTransport.h"
#include "nn/nex/nex_ZLibCompression.h"

#include <cstring>

namespace nn {
namespace nex {
// the object of the platform that makes the transport adapter (PRUDPEndPoint)
extern void* g_pUnknown0x96CA28;

namespace {
// the key of the P2P checksum unless one is configured
const u8 DEFAULT_P2P_KEY[8] = {1, 2, 3, 4, 5, 6, 7, 8};
const u32 DEFAULT_CHECKSUM_KEY_LENGTH = 16;
// the stream types whose settings get the algorithms
const Stream::Type STREAM_TYPE_DO = static_cast<Stream::Type>(1);
const Stream::Type STREAM_TYPE_RV = static_cast<Stream::Type>(2);
const Stream::Type STREAM_TYPE_RV_SECURE = static_cast<Stream::Type>(3);
const Stream::Type STREAM_TYPE_P2P_8 = static_cast<Stream::Type>(8);
const Stream::Type STREAM_TYPE_P2P_9 = static_cast<Stream::Type>(9);
const Stream::Type STREAM_TYPE_SECURE = static_cast<Stream::Type>(10);

// slot 0x08 / 0x0C of the platform factory: makes / deletes the transport adapter
typedef TransportAdapter* (*CreateAdapterFunction)(void* pFactory);
typedef void (*DeleteAdapterFunction)(void* pFactory, TransportAdapter* pAdapter);
inline TransportAdapter* CreateTransportAdapter(void* pFactory)
{
    return reinterpret_cast<CreateAdapterFunction>((*reinterpret_cast<void***>(pFactory))[2])(pFactory);
}
inline void DeleteTransportAdapter(void* pFactory, TransportAdapter* pAdapter)
{
    reinterpret_cast<DeleteAdapterFunction>((*reinterpret_cast<void***>(pFactory))[3])(pFactory, pAdapter);
}

// (inline) the key of the checksum if its length is allowed
inline void SetChecksumKey(KeyedChecksumAlgorithm* pAlgorithm, const Key& key)
{
    u32 length = key.GetLength();
    if (length >= pAlgorithm->m_MinKeyLength && length <= pAlgorithm->m_MaxKeyLength) {
        pAlgorithm->m_Key = key;
        pAlgorithm->KeyHasChanged();
    }
}
} // namespace

// the default key of the checksum of the secure streams, in .data, not in the source (name is ours)
// 0x0096CDF4
extern u8 g_DefaultChecksumKey[];

// 0x0096C925 (name is ours)
u8 g_CompressionMode;
// 0x0096C926 (name is ours)
bool g_UsesEncryption = true;
// 0x0096C927 (name is ours)
bool g_UsesSecureChecksum = true;
// 0x0096C928 (name is ours)
bool g_UsesP2PChecksum = true;
// 0x0096C9A4 (name is ours)
s32 g_CompressionWindowBits = -1;
// 0x0096C9A8 (name is ours)
s32 g_CompressionMemoryLevel = -1;
// 0x0096C9A0 (name is ours)
u32 g_P2PChecksumKeyLength;
// 0x00AE0478 (name is ours)
u8 g_P2PChecksumKey[32];

// 0x003D263C (name is ours)
void nn::nex::Network::AddStationURL(const nn::nex::StationURL& url)
{
    ScopedCS lock(m_StationUrls.m_Lock);
    for (qList<StationURL>::Node* pNode = m_StationUrls.m_List.GetFirst(); !m_StationUrls.m_List.IsEnd(pNode); pNode = pNode->m_pNext) {
        if (pNode->m_Value.GetAddress() == url.GetAddress() && pNode->m_Value.GetPortNumberInline() == url.GetPortNumberInline()) {
            return;
        }
    }
    m_StationUrls.push_back(url);
}

// 0x003D2828 | fefates:callgraph [tier C]
nn::nex::Network* nn::nex::Network::GetInstance()
{
    return GetInstanceInline();
}

// 0x003D2980 | fefates:bytes [tier B]
void nn::nex::Network::ReleaseInstance()
{
    GetInstance()->ReleaseRef();
}

// 0x003D29D0 (name is ours)
bool nn::nex::Network::UpdateStationURL(const nn::nex::StationURL& url)
{
    ScopedCS lock(m_StationUrls.m_Lock);
    for (qList<StationURL>::Node* pNode = m_StationUrls.m_List.GetFirst(); !m_StationUrls.m_List.IsEnd(pNode); pNode = pNode->m_pNext) {
        if (pNode->m_Value.GetAddress() == url.GetAddress()) {
            m_StationUrls.m_List.erase(pNode);
            ScopedCS listLock(m_StationUrls.m_Lock);
            m_StationUrls.m_List.push_back(url);
            return true;
        }
    }
    return false;
}

// 0x003D2BB0 | fefates:callgraph [tier C]
bool nn::nex::Network::ReplaceLocalURL(const nn::nex::StationURL& target, const nn::nex::StationURL& url)
{
    ScopedCS lock(m_StationUrls.m_Lock);
    for (qList<StationURL>::Node* pNode = m_StationUrls.m_List.GetFirst(); !m_StationUrls.m_List.IsEnd(pNode); pNode = pNode->m_pNext) {
        if (pNode->m_Value.GetAddress() == target.GetAddress() && pNode->m_Value.GetPortNumberInline() == target.GetPortNumberInline()) {
            pNode->m_Value.Copy(url);
            return true;
        }
    }
    return false;
}

// 0x003D2D04 | fefates:callgraph [tier C]
void nn::nex::Network::InitP2PChecksumAlgorithmKey()
{
    MD5ChecksumWithKey* pChecksum = m_pP2PChecksum;
    if (g_P2PChecksumKeyLength != 0) {
        Key key(g_P2PChecksumKey, g_P2PChecksumKeyLength);
        SetChecksumKey(pChecksum, key);
    } else {
        Key key(g_DefaultChecksumKey, DEFAULT_CHECKSUM_KEY_LENGTH);
        SetChecksumKey(pChecksum, key);
    }
}

// 0x003D2DC4 (name is ours)
void nn::nex::Network::AcquireInstance()
{
    AcquireInstanceInline();
}

// 0x003D2E14 | fefates:callgraph [tier C]
nn::nex::Network::Network()
    : m_Lock(LOCK_LEVEL), m_Singleton(KIND), m_StationUrls(1), m_IdLock(LOCK_LEVEL), m_FaultLock(LOCK_LEVEL)
{
    {
        ScopedCS lock(InstanceTable::s_Lock);
        m_Singleton.m_pOwner = this;
    }
    Core::AcquireInstance();
    m_pTransportAdapter = nullptr;
    if (g_pUnknown0x96CA28 != nullptr) {
        m_pTransportAdapter = CreateTransportAdapter(g_pUnknown0x96CA28);
        TransportAdapter::s_pInstance = m_pTransportAdapter;
    } else {
        TransportAdapter::s_pInstance = &TransportAdapter::s_DefaultInstance;
    }
    if (TransportAdapter::s_pInstance->m_pSocketDriver != nullptr) {
        SocketDriver::s_pInstance = TransportAdapter::s_pInstance->m_pSocketDriver;
    }
    TransportAdapter::s_pInstance->Load();
    m_pRootTransport = new UDPTransport();
    m_pRootTransport->Start();
    m_pNATTraversalEngine = new NATTraversalEngine();
    m_pCompression = nullptr;
    if (g_CompressionMode != 0 && g_CompressionMode == 1) {
        m_pCompression = new ZLibCompression(g_CompressionWindowBits, g_CompressionMemoryLevel);
    }
    Stream::GetSettings(STREAM_TYPE_RV_SECURE)->m_pCompressionAlgorithm = m_pCompression;
    Stream::GetSettings(STREAM_TYPE_RV)->m_pCompressionAlgorithm = m_pCompression;
    if (g_UsesEncryption) {
        Stream::GetSettings(STREAM_TYPE_RV_SECURE)->m_pEncryptionAlgorithm = &m_Encryption;
        Stream::GetSettings(STREAM_TYPE_RV)->m_pEncryptionAlgorithm = &m_Encryption;
    }
    m_pChecksum = new HMACChecksum();
    u8 key[sizeof(DEFAULT_P2P_KEY)];
    memcpy(key, DEFAULT_P2P_KEY, sizeof(key));
    g_P2PChecksumKeyLength = sizeof(key);
    memcpy(g_P2PChecksumKey, key, sizeof(key));
    m_pP2PChecksum = new MD5ChecksumWithKey();
    {
        HMACChecksum* pChecksum = m_pChecksum;
        Key defaultKey(g_DefaultChecksumKey, DEFAULT_CHECKSUM_KEY_LENGTH);
        SetChecksumKey(pChecksum, defaultKey);
    }
    InitP2PChecksumAlgorithmKey();
    if (g_UsesSecureChecksum) {
        Stream::GetSettings(STREAM_TYPE_SECURE)->m_pChecksumAlgorithm = m_pChecksum;
    }
    if (g_UsesP2PChecksum) {
        if (Stream::GetSettings(STREAM_TYPE_P2P_9)->m_pChecksumAlgorithm == nullptr) {
            Stream::GetSettings(STREAM_TYPE_P2P_9)->m_pChecksumAlgorithm = m_pP2PChecksum;
        }
        if (Stream::GetSettings(STREAM_TYPE_DO)->m_pChecksumAlgorithm == nullptr) {
            Stream::GetSettings(STREAM_TYPE_DO)->m_pChecksumAlgorithm = m_pP2PChecksum;
        }
        if (Stream::GetSettings(STREAM_TYPE_P2P_8)->m_pChecksumAlgorithm == nullptr) {
            Stream::GetSettings(STREAM_TYPE_P2P_8)->m_pChecksumAlgorithm = m_pP2PChecksum;
        }
    }
    StreamSettings* pSettings = Stream::GetSettings(STREAM_TYPE_SECURE);
    if (pSettings->m_UsesKey) {
        pSettings->m_Unknown0xA0 = 1;
    }
}

// 0x003D3434 slot 0x00
// 0x003D3404 (deleting dtor)
nn::nex::Network::~Network()
{
    SocketDriver::s_pInstance = SocketDriver::s_pDefaultInstance;
    if (TransportAdapter::s_pInstance != nullptr) {
        TransportAdapter::s_pInstance->Unload();
    }
    if (m_pNATTraversalEngine->m_CallContext.m_State == CallContext::STATE_CALL_IN_PROGRESS) {
        m_pNATTraversalEngine->m_CallContext.Cancel(CallContext::STATE_CANCELLED);
    }
    if (m_pNATTraversalEngine != nullptr) {
        delete m_pNATTraversalEngine;
    }
    m_pNATTraversalEngine = nullptr;
    if (m_pRootTransport != nullptr) {
        delete m_pRootTransport;
    }
    m_pRootTransport = nullptr;
    Core::ReleaseInstance();
    if (m_pTransportAdapter != nullptr) {
        DeleteTransportAdapter(g_pUnknown0x96CA28, m_pTransportAdapter);
        m_pTransportAdapter = nullptr;
    }
    if (m_pCompression != nullptr) {
        delete m_pCompression;
        m_pCompression = nullptr;
    }
    if (Stream::GetSettings(STREAM_TYPE_P2P_9)->m_pChecksumAlgorithm == m_pP2PChecksum) {
        Stream::GetSettings(STREAM_TYPE_P2P_9)->m_pChecksumAlgorithm = nullptr;
    }
    if (Stream::GetSettings(STREAM_TYPE_DO)->m_pChecksumAlgorithm == m_pP2PChecksum) {
        Stream::GetSettings(STREAM_TYPE_DO)->m_pChecksumAlgorithm = nullptr;
    }
    if (Stream::GetSettings(STREAM_TYPE_P2P_8)->m_pChecksumAlgorithm == m_pP2PChecksum) {
        Stream::GetSettings(STREAM_TYPE_P2P_8)->m_pChecksumAlgorithm = nullptr;
    }
    if (m_pChecksum != nullptr) {
        delete m_pChecksum;
    }
    if (m_pP2PChecksum != nullptr) {
        delete m_pP2PChecksum;
    }
}

// 0x0072E438 (name is ours)
void nn::nex::Network::GetStationURLs(nn::nex::qList<nn::nex::StationURL>& urls)
{
    ScopedCS lock(m_StationUrls.m_Lock);
    urls = m_StationUrls.m_List;
}

// 0x007D6DE8
// 0x007D6D90 (deleting dtor)
template qProtectedList<StationURL>::~qProtectedList();

} // namespace nex
} // namespace nn
