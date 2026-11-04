#include "net/nex/dImpl.h"

#include "net/dAcConnection.h"
#include "nn/ac/CTR/CTR_Api.h"
#include "nn/nex/nex_DataStoreLogicServerClient.h"
#include "nn/nex/nex_DataStoreMetaInfo.h"
#include "nn/nex/nex_DataStoreSearchParam.h"
#include "nn/nex/nex_DataStoreSearchResult.h"
#include "nn/nex/nex_ErrorCodeConverter.h"
#include "nn/nex/nex_MemoryManager.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_ResultRange.h"
#include "nn/nex/nex_SystemError.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_DataStoreGetMetaParam.h"
#include "nn/nex/nex_DataStorePersistenceTarget.h"
#include "nn/nex/nex_NotificationEvent.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_StringStream.h"
#include "nn/os/os_Thread.h"
#include "nn/friends/CTR/detail/detail_Api.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_GlobalVariables.h"
#include "nn/nex/nex_NgsFacade.h"
#include "nn/nex/nex_ObjectThreadRoot.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/local/local_Api.h"
#include "nn/pia/local/local_LocalFacade.h"
#include "nn/pia/local/local_LocalNetwork.h"
#include "nn/pia/local/local_UdsNetworkFactory.h"
#include "nn/pia/session/session_Api.h"
#include "nn/pia/transport/transport_Api.h"
#include "nn/pia/transport/transport_ReliableProtocol.h"
#include "nn/socket/socket_Api.h"
#include "nn/nex/nex_UtilityClient.h"
#include "nn/nex/nex_qMap.h"
#include "nn/nex/nex_BackEndServices.h"
#include "nn/os/os_Event.h"
#include "nn/nex/nex_TextMessage.h"
#include "Other/dRegion.h"
#include "nn/nex/nex_MatchmakeExtensionClient.h"
#include "nn/nex/nex_MessagingClient.h"
#include "nn/nex/nex_qList.h"
#include "nn/pia/inet/inet_Api.h"
#include "nn/pia/inet/inet_NexFacade.h"
#include "nn/pia/inet/inet_NexNetworkFactory.h"
#include "nn/pia/util/util_Api.h"
#include "nn/nex/nex_IndependentServer.h"
#include "nn/pia/inet/inet_NexCreateSessionSetting.h"
#include "nn/pia/inet/inet_NexJoinSessionSetting.h"
#include "nn/pia/inet/inet_NexSessionInfo.h"
#include "nn/pia/session/session_ISessionInfoList.h"
#include "nn/pia/local/local_LocalSessionInfo.h"
#include "nn/pia/local/local_LocalSessionSearchCriteria.h"
#include "nn/pia/local/local_UdsJoinSessionSetting.h"
#include "nn/nex/nex_DataStoreDeleteParam.h"
#include "nn/pia/local/local_LocalNetworkTypes.h"
#include "nn/pia/local/local_UdsCreateSessionSetting.h"
#include "script/dWordCPtrSv.h"
#include "nn/http/http_Api.h"
#include "nn/nex/nex_CustomDataHolder.h"
#include "nn/nex/nex_DataStoreRateObjectParam.h"
#include "nn/nex/nex_DataStoreRatingInfo.h"
#include "nn/nex/nex_DataStoreRatingTarget.h"
#include "nn/pia/common/common_CachedPrint.h"
#include "nn/nex/nex_DateTime.h"
#include "nn/nex/nex_DataStorePrepareUpdateParam.h"
#include "nn/nex/nex_DataStorePreparePostParam.h"
#include "nn/nex/nex_DataStoreRatingInitParam.h"
#include "nn/nex/nex_DataStoreChangeMetaParam.h"
#include "nn/nex/nex_DataStorePermission.h"
#include "nn/nex/nex_DataStorePrepareGetParam.h"
#include "nn/pia/inet/inet_NexSessionSearchCriteriaUnused.h"
#include "nn/nex/nex_MessageRecipient.h"
#include "nn/nex/nex_UserMessage.h"
#include "nn/pia/common/common_StationAddress.h"
#include "nn/nex/nex_DataStoreClientTemplate.h"
#include "script/dWordPtrSv.h"
#include "nn/pia/inet/inet_NexSessionSearchCriteriaOwner.h"
#include "nn/pia/local/local_UdsNetworkSetting.h"
#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/transport/transport_StationManager.h"
#include "nn/pia/transport/transport_ThreadStreamManager.h"
#include <cstdio>
#include <new>
#include "sead/seadNew.h"
#include "nn/pia/common/common_Scheduler.h"
#include "sead/seadExpHeap.h"
#include "sead/seadSafeStringBase.h"

namespace net {
// 0x00953CAC (name is ours; set by the game)
sead::Heap* g_pNetHeap;

namespace nex {
// The globals of the Impl.
// 0x00954680
u8 net::nex::Impl::s_SessionAttribute3;
// 0x00954681
bool net::nex::Impl::s_HasServiceToken;
// 0x00954684
Impl* net::nex::Impl::s_pInstance;
// 0x00954688
u32 net::nex::Impl::s_ServerSetting0;
// 0x0095468C
u32 net::nex::Impl::s_ServerSetting1;
// 0x00954690
u32 net::nex::Impl::s_ServerSetting2;
// 0x00954694
sead::ExpHeap* net::nex::Impl::s_pNexHeap;
// 0x00954698
nn::pia::StationId net::nex::Impl::s_InvalidStationId = nn::pia::GetStationIdOfIndex253();
// 0x00792C70 (static initializer)

// 0x00A24000
u8 net::nex::Impl::s_ReceiveBuffer[0xE000];
// 0x00A32000
char net::nex::Impl::s_ServiceToken[0x1000];
// 0x00A33000
u64 net::nex::Impl::s_SocketMemory[0x50000 / sizeof(u64)] __attribute__((aligned(0x1000)));

namespace {
const wchar_t* const ACCESS_KEY = L"d6f08b40";
// the access key of the independent server
const char* const INDEPENDENT_ACCESS_KEY = "2c10f49b";
} // namespace

// 0x0050D2E8 (name is ours)
u32 net::nex::Impl::GetServerSetting1()
{
    return s_ServerSetting1 > 5 ? 5 : s_ServerSetting1;
}

// 0x0050D300 (name is ours)
u32 net::nex::Impl::GetServerSetting2()
{
    // only 1 and 2 are valid values
    if (s_ServerSetting2 == 2 || s_ServerSetting2 == 1) {
        return s_ServerSetting2;
    }
    return 0;
}

// 0x0050E638 (name is ours)
bool net::nex::Impl::GetMetaChecked(u64 dataId, u16 dataType, TagFunction tagFunction,
                                    nn::nex::DataStoreMetaInfo* metaInfo)
{
    if (dataId == 0) {
        return false;
    }
    if (!GetMeta(dataId, metaInfo, tagFunction != NULL ? 1 : 0, false)) {
        return false;
    }
    if (dataType != 0xFFFF && metaInfo->m_DataType != dataType) {
        return false;
    }
    if (tagFunction != NULL) {
        TagList tags;
        TagWriter writer;
        writer.m_pTags = &tags;
        writer.m_pSourceTags = NULL;
        tagFunction(&writer);
        TagList objectTags(metaInfo->m_Tags);
        for (nn::nex::String* tag = tags.begin(); tag != tags.end(); tag++) {
            nn::nex::String* it = objectTags.begin();
            for (; it != objectTags.end(); it++) {
                if (nn::nex::String::IsEqual(it->m_pString, tag->m_pString)) {
                    break;
                }
            }
            if (it == objectTags.end()) {
                return false;
            }
        }
    }
    return true;
}

// 0x0050E858 (name is ours)
bool net::nex::Impl::Initialize()
{
    if (s_pNexHeap != NULL) {
        return true;
    }
    s_pNexHeap = sead::ExpHeap::create(0xD6000, sead::SafeStringBase<char>("NexHeap"), m_pHeap,
                                       sead::Heap::HEAP_DIRECTION_FORWARD, true);
    memset(m_PlayerName, 0, sizeof(m_PlayerName));
    return true;
}

// 0x0050E8D0 (name is ours)
bool net::nex::Impl::SearchData(SearchDataResultList* results, const u16* dataTypes, TagFunction tagFunction,
                                SearchOrder order)
{
    if (results == NULL) {
        return false;
    }
    if (m_pDataStore == NULL || !m_pDataStore->IsConnected()) {
        u32 errorCode = 0;
        if (!AcConnection::IsConnected()) {
            nn::ac::CTR::GetLastErrorCode(&errorCode);
        }
        if (m_ErrorCallback != NULL) {
            m_ErrorCallback(2, errorCode);
        }
        return false;
    }
    nn::nex::DataStoreSearchParam param;
    param.m_SearchTarget = 1;
    param.m_ResultRange = nn::nex::ResultRange(0, SEARCH_RESULT_SIZE);
    if (dataTypes[0] != 0xFFFF) {
        nn::nex::qVector<u16> types;
        for (u32 i = 0; i < SEARCH_DATA_TYPE_MAX && i < SEARCH_DATA_TYPE_NUM; i++) {
            if (dataTypes[i] == 0xFFFF) {
                break;
            }
            types.push_back(dataTypes[i]);
        }
        param.SetDataTypes(types);
    }
    if (tagFunction != NULL) {
        TagList tags;
        TagWriter writer;
        writer.m_pTags = &tags;
        writer.m_pSourceTags = NULL;
        tagFunction(&writer);
        param.m_Tags = tags;
    }
    switch (order) {
    case SEARCH_ORDER_1:
        param.m_ResultOrderColumn = 64;
        param.m_ResultOrder = 1;
        break;
    case SEARCH_ORDER_2:
        param.m_ResultOrderColumn = 6;
        param.m_ResultOrder = 1;
        break;
    default:
        break;
    }
    nn::nex::DataStoreSearchResult result;
    if (!SearchObject(param, &result)) {
        return false;
    }
    for (nn::nex::DataStoreMetaInfo* metaInfo = result.m_Result.begin(); metaInfo != result.m_Result.end();
         metaInfo++) {
        if (!AddResult(*metaInfo, results)) {
            break;
        }
    }
    return true;
}

// 0x0050ED0C (name is ours)
bool net::nex::Impl::GetData(u64 dataId, SearchDataResultList* results, u16 dataType, TagFunction tagFunction)
{
    if (results == NULL) {
        return false;
    }
    if (m_pDataStore == NULL || !m_pDataStore->IsConnected()) {
        u32 errorCode = 0;
        if (!AcConnection::IsConnected()) {
            nn::ac::CTR::GetLastErrorCode(&errorCode);
        }
        if (m_ErrorCallback != NULL) {
            m_ErrorCallback(2, errorCode);
        }
        return false;
    }
    nn::nex::DataStoreMetaInfo metaInfo;
    if (!GetMetaChecked(dataId, dataType, tagFunction, &metaInfo)) {
        return false;
    }
    AddResult(metaInfo, results);
    return true;
}

// 0x0050F57C (name is ours)
bool net::nex::Impl::SearchObject(const nn::nex::DataStoreSearchParam& param, nn::nex::DataStoreSearchResult* result)
{
    nn::nex::ProtocolCallContext context;
    if (!m_pDataStore->SearchObject(&context, param, result)) {
        ReportSystemError(1);
        return false;
    }
    context.Wait(0xFFFFFFFF);
    if (context.GetOutcome()) {
        return true;
    }
    u32 errorCode = nn::nex::ErrorCodeConverter::ConvertToNetworkErrorCode(context.GetOutcome());
    if (m_ErrorCallback != NULL) {
        m_ErrorCallback(1, errorCode);
    }
    return false;
}

// 0x00510BF8 (name is ours)
bool net::nex::Impl::GetMeta(u64 dataId, nn::nex::DataStoreMetaInfo* metaInfo, u8 resultOption, bool reportsError)
{
    if (metaInfo == NULL) {
        return false;
    }
    nn::nex::DataStoreGetMetaParam param;
    param.m_PersistenceTarget = nn::nex::DataStorePersistenceTarget();
    param.m_AccessPassword = 0;
    param.m_ResultOption = 0;
    param.m_DataId = dataId;
    if (resultOption != 0) {
        param.m_ResultOption = resultOption;
    }
    nn::nex::ProtocolCallContext context;
    if (!m_pDataStore->GetMeta(&context, param, metaInfo)) {
        if (reportsError) {
            ReportSystemError(1);
        }
        return false;
    }
    context.Wait(0xFFFFFFFF);
    if (context.GetOutcome()) {
        return true;
    }
    if (reportsError) {
        u32 errorCode = nn::nex::ErrorCodeConverter::ConvertToNetworkErrorCode(context.GetOutcome());
        if (m_ErrorCallback != NULL) {
            m_ErrorCallback(1, errorCode);
        }
    }
    return false;
}

// 0x00514144 (name is ours)
bool net::nex::Impl::AddResult(const nn::nex::DataStoreMetaInfo& metaInfo, SearchDataResultList* results)
{
    if (results->m_UsedNum >= results->m_Num) {
        return false;
    }
    SearchDataResult* result = results->Add(metaInfo.m_OwnerId, metaInfo.m_DataId, metaInfo.m_DataType);
    if (result == NULL) {
        return true;
    }
    nn::nex::String name(metaInfo.m_Name);
    result->SetName(reinterpret_cast<const char16*>(name.m_pString), name.GetLength());
    if (result->GetMetaBinaryBuffer() != NULL) {
        nn::nex::qVector<u8> binary(metaInfo.m_MetaBinary.m_pBegin, metaInfo.m_MetaBinary.m_pEnd);
        u32 size = result->GetMetaBinarySize();
        if (result->GetMetaBinarySize() == binary.size()) {
            u8* buffer = static_cast<u8*>(result->GetMetaBinaryBuffer());
            for (u32 i = 0; i < size; i++) {
                buffer[i] = binary[i];
            }
        }
    }
    return true;
}

// 0x004DE2BC (name is ours)
void Dispatch()
{
    nn::pia::common::Scheduler::s_pInstance->Dispatch(5);
    if (nn::nex::Scheduler::GetInstanceInline() != NULL) {
        nn::nex::Scheduler::GetInstanceInline()->DispatchIfSingleThreaded(3, 0);
    }
    nn::os::Thread::SleepImpl(nn::fnd::TimeSpan::FromMilliSeconds(8));
}

// 0x005152C0 (name is ours)
// 0x005152BC (deleting dtor)
net::nex::Impl::NotificationEventHandler::~NotificationEventHandler()
{
    // empty (in the original too)
}

// 0x0051526C (name is ours)
void net::nex::Impl::NotificationEventHandler::ProcessNotificationEvent(const nn::nex::NotificationEvent& event)
{
    nn::nex::StringStream stream;
    stream << event;
    if (s_pInstance != NULL) {
        s_pInstance->OnNotificationEvent(event, true);
    }
}
} // namespace nex
} // namespace net

namespace net {
namespace nex {
// 0x004DE280 (name is ours)
void net::nex::Impl::Free(void* p)
{
    s_pNexHeap->free(p);
}

// 0x004DE29C (name is ours)
void* net::nex::Impl::Allocate(u32 size)
{
    return s_pNexHeap->tryAlloc(size, 4);
}

// 0x0050EE00 (name is ours)
s32 net::nex::Impl::ConnectLocal()
{
    m_pSessionInfos = new (m_pHeap, 4) ISessionInfo*[16];
    if (m_pSessionInfos != NULL && InitializeCommon() && InitializeLocalNetwork()) {
        nn::pia::local::UdsNetworkFactory factory;
        nn::pia::transport::Transport::Setting transportSetting;
        transportSetting.m_pNetworkFactory = &factory;
        if (InitializeTransport(transportSetting)) {
            nn::pia::session::Session::Setting sessionSetting;
            sessionSetting.m_pNetworkFactory = &factory;
            sessionSetting.m_RelayMode = 0;
            sessionSetting.m_SessionInfoNumMax = 16;
            sessionSetting.m_IsBandwidthCheckEnabled = false;
            if (nn::pia::session::Initialize().IsSuccess()) {
                nn::pia::session::BeginSetup();
                if (nn::pia::session::Session::CreateInstance(sessionSetting).IsSuccess()) {
                    nn::pia::session::EndSetup();
                    nn::pia::session::Session::s_pInstance->SetEventCallback(OnSessionEvent);
                    nn::pia::session::Session::StartupSetting startupSetting;
                    startupSetting.m_IsHostMigrationEnabled = true;
                    startupSetting.m_pIdentificationData = NULL;
                    startupSetting.m_CryptoMode = static_cast<nn::pia::common::Crypto::Mode>(0);
                    startupSetting.m_TimeoutMSec = 10000;
                    startupSetting.m_KeepAliveIntervalMSec = 1000;
                    startupSetting.m_BandwidthCheckBandwidth = -1;
                    startupSetting.m_BandwidthCheckPacketSize = 0;
                    startupSetting.m_BandwidthCheckDurationMSec = 1000;
                    startupSetting.m_IsBandwidthCheckOneWay = false;
                    startupSetting.m_pPlayerName = NULL;
                    nn::Result result = StartupSession(startupSetting);
                    if (result.IsSuccess()) {
                        return 0;
                    }
                    if (result == RESULT_LOCAL_STARTUP_FAILED_0 || result == RESULT_LOCAL_STARTUP_FAILED_1) {
                        FinalizeLocal();
                        return static_cast<s32>(0x80000000);
                    }
                    FinalizeLocal();
                    return static_cast<s32>(0x80000006);
                }
                nn::pia::session::EndSetup();
            }
            if (nn::pia::session::Session::s_pInstance != NULL) {
                nn::pia::session::Session::s_pInstance->ClearEventCallback();
                nn::pia::session::Session::DestroyInstance();
            }
            nn::pia::session::Finalize();
        }
    }
    FinalizeLocal();
    return static_cast<s32>(0x80000006);
}

// 0x0050F378 (name is ours)
void net::nex::Impl::Finalize()
{
    FinalizeCommon();
}

// 0x0050F37C (name is ours)
void net::nex::Impl::FinalizeCommon()
{
    if (nn::pia::common::IsInitialized()) {
        m_CriticalSection.lock();
        if (nn::pia::common::Scheduler::s_pInstance != NULL) {
            nn::pia::common::Scheduler::DestroyInstance();
        }
        nn::pia::common::Finalize();
        m_CriticalSection.unlock();
    }
    if (m_pCommonMemory != NULL) {
        m_pHeap->free(m_pCommonMemory);
        m_pCommonMemory = NULL;
    }
}

DECOMP_ALWAYS_INLINE net::nex::Impl::ReceivePool::ReceivePool() : m_pFree(NULL), m_pBuffers(NULL)
{
    void* work = m_Buffers;
    if (work != NULL) {
        m_pFree = work;
        // the free list runs through the first word of each buffer
        for (s32 i = 0; i < 99; i++) {
            void* buffer = &m_Buffers[i];
            *static_cast<void**>(buffer) = &m_Buffers[i + 1];
        }
        void* last = &m_Buffers[99];
        *static_cast<void**>(last) = NULL;
        m_pBuffers = m_Buffers;
    }
    m_Used.setBuffer(100, m_UsedPointers);
}

// 0x0050F3E4 (name is ours)
bool net::nex::Impl::InitializeNexGlobals()
{
    if (m_pReceivePool == NULL) {
        m_pReceivePool = new (m_pHeap, 4) ReceivePool;
    }
    m_HasNotification = false;
    nn::nex::MemoryManager::s_AllocateFunction = Allocate;
    nn::nex::MemoryManager::s_FreeFunction = Free;
    nn::nex::GlobalVariables::AcquireInstance();
    nn::nex::Core::s_ThreadMode = 3;
    nn::nex::ObjectThreadRoot::SetUsesAutoStack(false);
    return true;
}

// 0x0050F4D4 (name is ours)
bool net::nex::Impl::InitializeCommon()
{
    if (nn::pia::common::IsInitialized()) {
        return true;
    }
    m_pCommonMemory = m_pHeap->tryAlloc(COMMON_MEMORY_SIZE, 4);
    if (m_pCommonMemory == NULL) {
        return false;
    }
    m_CriticalSection.lock();
    if (nn::pia::common::Initialize(m_pCommonMemory, COMMON_MEMORY_SIZE).IsSuccess()) {
        nn::pia::common::BeginSetup();
        if (nn::pia::common::Scheduler::CreateInstance(13).IsSuccess()) {
            nn::pia::common::EndSetup();
            m_CriticalSection.unlock();
            return true;
        }
        nn::pia::common::EndSetup();
    }
    FinalizeCommon();
    m_CriticalSection.unlock();
    return false;
}

// 0x0050F68C (name is ours)
void net::nex::Impl::FinalizeLocal()
{
    if (nn::pia::session::Session::s_pInstance != NULL) {
        nn::pia::session::Session::s_pInstance->Cleanup();
    }
    m_IsSessionStarted = false;
    m_SendSkipCount = 0;
    if (nn::pia::session::Session::s_pInstance != NULL) {
        nn::pia::session::Session::s_pInstance->ClearEventCallback();
        nn::pia::session::Session::DestroyInstance();
    }
    nn::pia::session::Finalize();
    nn::pia::transport::Transport* transport = nn::pia::transport::Transport::s_pInstance;
    if (transport != NULL) {
        nn::pia::transport::ReliableProtocol* protocol = static_cast<nn::pia::transport::ReliableProtocol*>(
            transport->m_ProtocolManager.SearchProtocol(m_ReliableProtocolId, nn::pia::transport::PROTOCOL_TYPE_RELIABLE));
        if (protocol != NULL) {
            protocol->Finalize();
            transport->m_ProtocolManager.DestroyProtocol(m_ReliableProtocolId.m_Id);
        }
        m_ReliableProtocolId.m_Id = 0;
        nn::pia::transport::Transport::DestroyInstance();
    }
    if (nn::pia::transport::IsInitialized()) {
        nn::pia::transport::Finalize();
    }
    nn::pia::local::LocalFacade::DestroyInstance();
    if (nn::pia::common::IsValidPointer(nn::pia::local::LocalNetwork::s_pInstance)) {
        nn::pia::local::LocalNetwork::s_pInstance->Finalize();
    }
    nn::pia::local::LocalNetwork::DestroyInstance();
    if (nn::pia::local::IsInitialized()) {
        nn::pia::local::Finalize();
    }
    FinalizeCommon();
    if (m_pSessionInfos != NULL) {
        delete[] m_pSessionInfos;
        m_pSessionInfos = NULL;
    }
}

// (inline)
void net::nex::Impl::FinalizeSocketIfUnused()
{
    if (m_pNgsFacade == NULL && m_pIndependentServer == NULL) {
        nn::socket::Finalize();
        AcConnection::Disconnect();
        m_IsSocketInitialized = false;
        m_IsAcConnected = false;
    }
}

// (inline)
void net::nex::Impl::FinalizeNexGlobals()
{
    nn::nex::MemoryManager::s_AllocateFunction = NULL;
    nn::nex::MemoryManager::s_FreeFunction = NULL;
    nn::nex::GlobalVariables::ReleaseInstance();
    m_HasNotification = false;
    if (m_pReceivePool != NULL) {
        delete m_pReceivePool;
        m_pReceivePool = NULL;
    }
    m_SessionId = 0;
}

// 0x0050F7A0 (name is ours)
bool net::nex::Impl::InitializeNex(bool unused)
{
    if (m_pNgsFacade != NULL) {
        return true;
    }
    if (!m_IsSocketInitialized) {
        if (!InitializeNexGlobals()) {
            return false;
        }
        if (!InitializeSocket()) {
            FinalizeNexGlobals();
            return false;
        }
    }
    if (Login(GAME_SERVER_ID, ACCESS_KEY)) {
        return true;
    }
    FinalizeSocketIfUnused();
    FinalizeNexGlobals();
    return false;
}

// 0x005109E0 (name is ours)
bool net::nex::Impl::InitializeSocket()
{
    u32 errorCode;
    if (!AcConnection::Connect(&errorCode)) {
        if (m_ErrorCallback != NULL) {
            m_ErrorCallback(3, errorCode);
        }
        return false;
    }
    if (nn::socket::GetRequiredMemorySize(SOCKET_SHARED_MEMORY_SIZE, SOCKET_SESSION_NUM) > sizeof(s_SocketMemory)) {
        if (m_ErrorCallback != NULL) {
            m_ErrorCallback(3, 0);
        }
        FinalizeSocketIfUnused();
        return false;
    }
    if (nn::socket::Initialize(reinterpret_cast<uptr>(s_SocketMemory), sizeof(s_SocketMemory), SOCKET_SHARED_MEMORY_SIZE,
                               SOCKET_SESSION_NUM)
            .IsFailure()) {
        if (m_ErrorCallback != NULL) {
            m_ErrorCallback(3, 0);
        }
        FinalizeSocketIfUnused();
        return false;
    }
    m_IsSocketInitialized = true;
    m_IsAcConnected = true;
    return true;
}

// 0x00510F00 (name is ours)
void net::nex::Impl::FinalizeNex()
{
    Logout();
    if (m_pNgsFacade == NULL) {
        FinalizeSocketIfUnused();
        if (!m_IsSocketInitialized) {
            FinalizeNexGlobals();
        }
    }
}

// 0x00515738 (name is ours)
void net::nex::Impl::FinalizeIndependent()
{
    Logout();
    if (m_pIndependentServer != NULL) {
        delete m_pIndependentServer;
        m_pIndependentServer = NULL;
    }
    FinalizeSocketIfUnused();
    if (!m_IsSocketInitialized) {
        FinalizeNexGlobals();
    }
}

// 0x005163A4 (name is ours)
void net::nex::Impl::Logout()
{
    if (m_pNgsFacade == NULL) {
        return;
    }
    if (m_pMatchmakeExtension != NULL || m_pMessaging != NULL || m_pDataStore != NULL) {
        return;
    }
    if (m_pNgsFacade->IsConnected().Equals(false) && !nn::nex::Core::GetInstance()->m_IsInterrupted) {
        nn::nex::Core::GetInstance()->SetInterrupted(true);
    }
    m_pNgsFacade->UnregisterNotificationEventHandler(&m_NotificationEventHandler);
    {
        nn::nex::ProtocolCallContext context;
        m_pNgsFacade->Logout(&context);
        context.Wait(0xFFFFFFFF);
    }
    nn::friends::CTR::detail::Logout();
    nn::nex::CallContext context;
    m_pNgsFacade->Terminate(&context);
    context.Wait(0xFFFFFFFF);
    if (m_pNgsFacade != NULL) {
        delete m_pNgsFacade;
    }
    m_pNgsFacade = NULL;
    m_IsLoggedIn = false;
    m_GameServerId = 0;
}
} // namespace nex
} // namespace net

namespace net {
namespace nex {
// 0x00513BC8 (name is ours)
bool net::nex::Impl::GetIntegerSettings()
{
    nn::nex::Credentials* credentials = m_pNgsFacade->m_pCredentials;
    if (credentials == NULL) {
        return false;
    }
    bool isSuccess = false;
    nn::nex::UtilityClient* client = new nn::nex::UtilityClient;
    if (client != NULL) {
        if (client->Bind(credentials)) {
            nn::nex::qMap<u16, u32> settings;
            {
                nn::nex::ProtocolCallContext context;
                if (client->GetIntegerSettings(&context, GetRegionIndex(), &settings)) {
                    context.Wait(0xFFFFFFFF);
                    if (context.GetOutcome()) {
                        s_ServerSetting0 = settings[0];
                        u32 setting1 = settings[1];
                        if (setting1 <= 5) {
                            s_ServerSetting1 = setting1;
                        }
                        s_ServerSetting2 = settings[2];
                        isSuccess = true;
                    }
                }
            }
        }
        delete client;
    }
    return isSuccess;
}

// 0x00515F40 (name is ours)
bool net::nex::Impl::Login(u32 gameServerId, const wchar_t* accessKey)
{
    m_pNgsFacade = new nn::nex::NgsFacade;
    if (m_pNgsFacade == NULL) {
        ReportSystemError(3);
        return false;
    }
    nn::nex::Core::GetInstance()->SetInterrupted(false);
    nn::Result result = nn::pia::common::RESULT_NOT_SET;
    nn::os::Event event;
    event.Initialize(nn::os::RESET_TYPE_STICKY);
    result = nn::friends::CTR::detail::Login(&event);
    if (result.IsFailure()) {
        if (m_ErrorCallback != NULL) {
            m_ErrorCallback(3, nn::friends::CTR::detail::ResultToErrorCode(result));
        }
    } else {
        event.Wait();
        result = nn::friends::CTR::detail::GetLastResponseResult();
        if (result.IsFailure()) {
            if (m_ErrorCallback != NULL) {
                m_ErrorCallback(3, nn::friends::CTR::detail::ResultToErrorCode(result));
            }
        } else if (!nn::friends::CTR::detail::HasLoggedIn()) {
            if (AcConnection::IsConnected()) {
                u32 errorCode = nn::friends::CTR::detail::ResultToErrorCode(result);
                if (m_ErrorCallback != NULL) {
                    m_ErrorCallback(2, errorCode);
                }
            } else {
                u32 errorCode;
                nn::ac::CTR::GetLastErrorCode(&errorCode);
                if (m_ErrorCallback != NULL) {
                    m_ErrorCallback(2, errorCode);
                }
            }
        } else {
            nn::nex::ProtocolCallContext context;
            if (!m_pNgsFacade->Login(&context, gameServerId, accessKey, LOGIN_TIMEOUT, 0)) {
                ReportSystemError(3);
            } else {
                while (true) {
                    if (nn::nex::Scheduler::GetInstanceInline() != NULL) {
                        nn::nex::Scheduler::GetInstanceInline()->DispatchIfSingleThreaded(3, 0);
                    }
                    if (context.m_State != nn::nex::CallContext::STATE_CALL_IN_PROGRESS) {
                        break;
                    }
                    nn::os::Thread::SleepImpl(nn::fnd::TimeSpan::FromMilliSeconds(8));
                }
                nn::nex::qResult outcome = context.GetOutcome();
                if (outcome) {
                    GetIntegerSettings();
                    if (m_pNgsFacade->IsConnected()) {
                        m_pNgsFacade->RegisterNotificationEventHandler(&m_NotificationEventHandler);
                        m_GameServerId = gameServerId;
                        m_IsLoggedIn = true;
                        return true;
                    }
                    if (AcConnection::IsConnected()) {
                        if (m_ErrorCallback != NULL) {
                            m_ErrorCallback(2, 0);
                        }
                    } else {
                        u32 errorCode;
                        nn::ac::CTR::GetLastErrorCode(&errorCode);
                        if (m_ErrorCallback != NULL) {
                            m_ErrorCallback(2, errorCode);
                        }
                    }
                } else {
                    u32 errorCode = m_pNgsFacade->GetLastLoginErrorCode();
                    if (m_ErrorCallback != NULL) {
                        m_ErrorCallback(3, errorCode);
                    }
                }
                nn::nex::BackEndServices::GetResultString(nn::nex::String(), outcome);
            }
        }
    }
    Logout();
    return false;
}
} // namespace nex
} // namespace net

namespace net {
namespace nex {
// 0x00511E30 (name is ours)
bool net::nex::Impl::InitializeLocalNetwork()
{
    if (nn::pia::local::Initialize().IsFailure()) {
        return false;
    }
    bool isSuccess = false;
    nn::pia::local::BeginSetup();
    if (nn::pia::local::LocalNetwork::CreateInstance().IsSuccess() && nn::pia::local::LocalNetwork::s_pInstance != NULL) {
        nn::pia::local::UdsNetworkSetting setting;
        setting.m_ReceiveBufferSize = sizeof(s_SocketMemory);
        setting.m_pReceiveBuffer = s_SocketMemory;
        setting.m_ScanBufferSize = 0x4000;
        setting.m_SendOption = 3;
        setting.m_ReceiveOption = 1;
        setting.m_pUserName = NULL;
        if (nn::pia::local::LocalNetwork::s_pInstance->Initialize(setting).IsSuccess() &&
            nn::pia::local::LocalFacade::CreateInstance().IsSuccess() && nn::pia::local::LocalFacade::s_pInstance != NULL) {
            isSuccess = true;
            nn::pia::local::EndSetup();
            return isSuccess;
        }
    }
    nn::pia::local::EndSetup();
    nn::pia::local::LocalFacade::DestroyInstance();
    if (nn::pia::common::IsValidPointer(nn::pia::local::LocalNetwork::s_pInstance)) {
        nn::pia::local::LocalNetwork::s_pInstance->Finalize();
    }
    nn::pia::local::LocalNetwork::DestroyInstance();
    if (nn::pia::local::IsInitialized()) {
        nn::pia::local::Finalize();
    }
    return isSuccess;
}

// 0x00512B40 (name is ours)
nn::Result net::nex::Impl::StartupSession(nn::pia::session::Session::StartupSetting& setting)
{
    if (m_IsSessionStarted) {
        return nn::Result();
    }
    nn::pia::transport::Station::PlayerName playerName;
    memcpy(&playerName, m_PlayerName, sizeof(playerName));
    IdentificationData identification;
    setting.m_pPlayerName = &playerName;
    setting.m_IsHostMigrationEnabled = false;
    setting.m_TimeoutMSec = 30000;
    setting.m_KeepAliveIntervalMSec = 2000;
    memcpy(identification.name, L"Name", sizeof(char16) * 4);
    identification.length = 4;
    identification.unk_0x23 = 0;
    setting.m_pIdentificationData = reinterpret_cast<const u8*>(&identification);
    nn::Result result = nn::pia::session::Session::s_pInstance->Startup(setting);
    if (result.IsSuccess()) {
        m_IsSessionStarted = true;
    }
    return result;
}

// 0x0051451C (name is ours)
void net::nex::Impl::OnSessionEvent(nn::pia::session::Session::EventType type, nn::pia::StationId stationId)
{
    if (s_pInstance == NULL || s_pInstance->m_StationCallback == NULL) {
        return;
    }
    switch (type) {
    case nn::pia::session::Session::EVENT_TYPE_JOIN: {
        nn::pia::transport::Station* station = nn::pia::transport::StationManager::s_pInstance->GetStation(stationId);
        if (station == NULL) {
            return;
        }
        nn::pia::transport::Station::PlayerName name;
        if (station->GetPlayerName(&name).IsFailure()) {
            return;
        }
        if (nn::pia::transport::StationManager::s_pInstance->m_pLocalStation->m_StationId == stationId) {
            return;
        }
        nn::pia::transport::Station::PlayerName copy = name;
        const char16* text = reinterpret_cast<const char16*>(copy.m_Data);
        u32 length = 0;
        while (length < 16 && text[length] != 0) {
            length++;
        }
        s_pInstance->m_StationCallback(stationId, true, text, length);
        break;
    }
    case nn::pia::session::Session::EVENT_TYPE_LEAVE:
        s_pInstance->m_StationCallback(stationId, false, NULL, 0);
        break;
    default:
        break;
    }
}

// 0x005137E8 (name is ours)
bool net::nex::Impl::InitializeTransport(nn::pia::transport::Transport::Setting& setting)
{
    nn::pia::transport::Initialize();
    bool isSuccess = false;
    nn::pia::transport::BeginSetup();
    setting.m_StationNumMax = 4;
    setting.m_SendPacketNum = 8;
    setting.m_ReceivePacketNum = 32;
    setting.m_AnalysisIntervalSec = 0;
    if (nn::pia::transport::Transport::CreateInstance(setting).IsSuccess()) {
        nn::pia::transport::Transport* transport = nn::pia::transport::Transport::s_pInstance;
        m_ReliableProtocolId = transport->m_ProtocolManager.CreateProtocol<nn::pia::transport::ReliableProtocol>(
            nn::pia::transport::PROTOCOL_TYPE_RELIABLE, 0);
        if (transport->m_ProtocolManager
                .GetProtocol<nn::pia::transport::ReliableProtocol>(m_ReliableProtocolId, nn::pia::transport::PROTOCOL_TYPE_RELIABLE)
                ->Initialize(64, 64)
                .IsSuccess()) {
            transport->EnableKeepAlive();
            isSuccess = true;
        }
    }
    nn::pia::transport::EndSetup();
    nn::pia::transport::ThreadStreamManager* streamManager = nn::pia::transport::ThreadStreamManager::s_pInstance;
    if (streamManager != NULL) {
        streamManager->SetSendThreadPriority(8);
        streamManager->SetReceiveThreadPriority(8);
        streamManager->SetSendThreadWaitMSec(4);
        streamManager->SetReceiveThreadWaitMSec(4);
    }
    if (!isSuccess) {
        nn::pia::transport::Transport* transport = nn::pia::transport::Transport::s_pInstance;
        if (transport != NULL) {
            nn::pia::transport::ReliableProtocol* protocol = transport->m_ProtocolManager.GetProtocol<nn::pia::transport::ReliableProtocol>(
                m_ReliableProtocolId, nn::pia::transport::PROTOCOL_TYPE_RELIABLE);
            if (protocol != NULL) {
                protocol->Finalize();
                transport->m_ProtocolManager.DestroyProtocol(m_ReliableProtocolId.m_Id);
            }
            m_ReliableProtocolId.m_Id = 0;
            nn::pia::transport::Transport::DestroyInstance();
        }
        if (nn::pia::transport::IsInitialized()) {
            nn::pia::transport::Finalize();
        }
    }
    return isSuccess;
}
} // namespace nex
} // namespace net

namespace net {
namespace nex {
// 0x00516B48 (name is ours)
net::nex::Impl::Impl()
    : m_pHeap(NULL), m_pCommonMemory(NULL), m_pSessionInfos(NULL), m_ScanMode(0), m_pNgsFacade(NULL), m_pMatchmakeExtension(NULL), m_SessionId(0),
      m_OwnerPrincipalId(0), m_pReceivePool(NULL), m_HasNotification(false), m_pMessaging(NULL), m_pDataStore(NULL), m_pIndependentServer(NULL),
      m_GameServerId(0), m_IsSocketInitialized(false), m_IsAcConnected(false), m_IsLoggedIn(false), m_ReliableProtocolId(0, 0),
      m_JoinApprovalCallback(NULL), m_StationCallback(NULL), m_ReceiveCallback(NULL), m_ErrorCallback(NULL), m_MessageCallback(NULL),
      m_NotificationCallback(NULL), unk_0x60(0), m_LastSendSize(0), m_SendSkipCount(0), m_IsSessionStarted(false), m_IsSessionOpen(false), m_IsInSession(false)
{
    m_SessionText[234] = 0; // (the constructor clears this character too)
    s_pInstance = this;
    m_pHeap = g_pNetHeap;
    m_SessionText[0] = 0;
}

// 0x003BDDF4 (a nop that falls into the destructor of MessagingNotificationHandler)
// 0x00513400 (deleting dtor)
net::nex::Impl::MessageRecvHandler::~MessageRecvHandler()
{
    // (only the base is destroyed)
}

// 0x00513328 (name is ours)
void net::nex::Impl::MessageRecvHandler::DeliverMessage(nn::nex::AnyObjectHolder<nn::nex::Data, nn::nex::String>& oUserMessage)
{
    if (oUserMessage.m_pObject != NULL && oUserMessage.m_pObject->IsAKindOf(nn::nex::String(L"TextMessage"))) {
        nn::nex::TextMessage* message = static_cast<nn::nex::TextMessage*>(oUserMessage.m_pObject);
        if (s_pInstance != NULL && s_pInstance->m_MessageCallback != NULL) {
            u32 length = message->m_strSubject.GetLength();
            u32 sender = message->m_pidSender;
            s_pInstance->m_MessageCallback(&sender, message->m_strSubject.m_pString, length, 0);
        }
    }
}
} // namespace nex
} // namespace net

namespace net {
namespace nex {
namespace {
// 0x0051142C (name is ours)
DECOMP_NOINLINE s32 CompareSecondWord(const void* a, const void* b)
{
    u32 left = static_cast<const u32*>(a)[2];
    u32 right = *static_cast<const u32*>(b);
    if (left > right) {
        return 1;
    }
    if (left < right) {
        return -1;
    }
    return 0;
}

// 0x005127C0 (name is ours)
DECOMP_NOINLINE s32 CompareWord(const void* a, const void* b)
{
    u32 left = *static_cast<const u32*>(a);
    u32 right = *static_cast<const u32*>(b);
    if (left > right) {
        return 1;
    }
    if (left < right) {
        return -1;
    }
    return 0;
}
} // namespace

// 0x005111EC (a nop in the original that falls into LeaveSession)
void net::nex::Impl::LeaveLocalSession()
{
    LeaveSession();
}

// 0x005111F0 (name is ours)
void net::nex::Impl::LeaveSession()
{
    nn::pia::session::Session* session = nn::pia::session::Session::s_pInstance;
    nn::Result result = session->CheckStatus();
    bool leaves;
    if (result.IsFailure()) {
        leaves = result != RESULT_LOCAL_STARTUP_FAILED_1;
    } else {
        leaves = session->GetStatus() != 0;
    }
    if (leaves && nn::pia::session::Session::s_pInstance->LeaveSessionAsync().IsSuccess()) {
        while (!nn::pia::session::Session::s_pInstance->IsLeaveSessionAsyncCompleted()) {
            Dispatch();
        }
        nn::pia::session::Session::s_pInstance->GetLeaveSessionAsyncResult();
    }
    nn::pia::session::Session::s_pInstance->ClearJoinApprovalCallback();
    if (nn::pia::session::Session::s_pInstance != NULL) {
        nn::pia::session::Session::SetSyncClockRequestInterval(2000);
    }
    m_IsInSession = false;
    if (nn::pia::session::Session::s_pInstance != NULL) {
        nn::pia::session::Session::s_pInstance->Cleanup();
    }
    m_IsSessionStarted = false;
    m_SendSkipCount = 0;
}

// 0x005112B4 | libgarden
bool net::nex::Impl::OpenNexSession(bool notifyFriends, bool setParam, u16 param)
{
    if (m_pMatchmakeExtension == NULL || !m_pMatchmakeExtension->IsConnected() || m_pNgsFacade == NULL) {
        return false;
    }
    if (!m_IsInSession) {
        ReportSystemError(3);
        return false;
    }
    if (setParam) {
        m_SessionParam = param;
    }
    nn::Result result = nn::pia::session::Session::s_pInstance->OpenParticipationAsync();
    if (result.IsSuccess()) {
        while (!nn::pia::session::Session::s_pInstance->IsOpenParticipationAsyncCompleted()) {
            Dispatch();
        }
        result = nn::pia::session::Session::s_pInstance->GetOpenParticipationAsyncResult();
        if (result.IsSuccess()) {
            m_IsSessionOpen = true;
            if (notifyFriends) {
                SendNotification(NOTIFICATION_TYPE_101, m_SessionId, m_SessionParam | 0x8010000,
                                 nn::nex::String(m_SessionText));
            }
            return true;
        }
    }
    ReportPiaResult(3, result, 0, 0);
    return false;
}

// 0x00512704 (name is ours)
void net::nex::Impl::ReportPiaResult(u8 level, const nn::Result& result, u32 unused0, u32 unused1)
{
    s32 category = nn::pia::util::GetResultCategory(result);
    u8 resultLevel;
    if (category == 0) {
        resultLevel = 0;
    } else if (category == 1 || category == 2) {
        resultLevel = 1;
    } else {
        resultLevel = 3;
    }
    if (resultLevel != level) {
        resultLevel = level;
    }
    s32 kind = nn::pia::util::GetResultKind(result);
    u32 errorCode;
    if (kind == 3 || kind == 0) {
        errorCode = 0;
    } else {
        nn::pia::local::LocalNetwork* network = nn::pia::local::LocalNetwork::s_pInstance;
        if (network != NULL && (network->IsHost() || network->IsClient())) {
            errorCode = 0;
        } else {
            errorCode = nn::pia::util::GetErrorCode(result);
        }
    }
    if (m_ErrorCallback != NULL) {
        m_ErrorCallback(resultLevel, errorCode);
    }
}

// 0x00512D1C (name is ours)
bool net::nex::Impl::InitializeInet()
{
    if (m_pMatchmakeExtension != NULL && m_pMatchmakeExtension->IsConnected()) {
        return true;
    }
    if (m_pNgsFacade == NULL) {
        return false;
    }
    nn::nex::Credentials* credentials = m_pNgsFacade->m_pCredentials;
    if (credentials == NULL) {
        return false;
    }
    m_pMatchmakeExtension = new nn::nex::MatchmakeExtensionClient;
    if (m_pMatchmakeExtension != NULL && m_pMatchmakeExtension->Bind(credentials)) {
        m_pMessaging = new nn::nex::MessagingClient(true);
        if (m_pMessaging != NULL && m_pMessaging->Bind(credentials)) {
            m_pMessaging->RegisterNotificationHandler(&m_MessageRecvHandler);
            ReceiveNotifications(NOTIFICATION_TYPE_103);
            ReceiveNotifications(NOTIFICATION_TYPE_101);
            nn::pia::inet::Setting inetSetting;
            inetSetting.m_MtuSize = 1340;
            if (nn::pia::inet::Initialize(inetSetting).IsSuccess()) {
                nn::pia::inet::BeginSetup();
                if (nn::pia::inet::NexFacade::CreateInstance().IsFailure()) {
                    nn::pia::inet::EndSetup();
                } else {
                    nn::pia::inet::EndSetup();
                    nn::pia::inet::NexFacade::LoginInfo loginInfo;
                    loginInfo.m_Unknown0x4 = m_GameServerId;
                    loginInfo.m_pBackEndServices = m_pNgsFacade;
                    if (nn::pia::inet::NexFacade::s_pInstance->Bind(&loginInfo).IsSuccess()) {
                        nn::pia::inet::NexNetworkFactory factory;
                        nn::pia::transport::Transport::Setting transportSetting;
                        transportSetting.m_pNetworkFactory = &factory;
                        if (InitializeTransport(transportSetting)) {
                            nn::pia::session::Session::Setting sessionSetting;
                            sessionSetting.m_pNetworkFactory = &factory;
                            sessionSetting.m_RelayMode = 0;
                            sessionSetting.m_SessionInfoNumMax = 1;
                            sessionSetting.m_IsBandwidthCheckEnabled = false;
                            if (nn::pia::session::Initialize().IsSuccess()) {
                                nn::pia::session::BeginSetup();
                                if (nn::pia::session::Session::CreateInstance(sessionSetting).IsSuccess()) {
                                    nn::pia::session::EndSetup();
                                    nn::pia::session::Session::s_pInstance->SetEventCallback(OnSessionEvent);
                                    return true;
                                }
                                nn::pia::session::EndSetup();
                            }
                            if (nn::pia::session::Session::s_pInstance != NULL) {
                                nn::pia::session::Session::s_pInstance->ClearEventCallback();
                                nn::pia::session::Session::DestroyInstance();
                            }
                            nn::pia::session::Finalize();
                        }
                    }
                }
            }
            FinalizeInet();
            return false;
        }
    }
    ReportSystemError(3);
    FinalizeInet();
    return false;
}

// 0x00512FC8 (name is ours)
void net::nex::Impl::LeaveNexSession()
{
    LeaveSession();
    m_IsSessionOpen = false;
}

// 0x00512FE0 (name is ours)
void net::nex::Impl::FinalizeInet()
{
    if (nn::pia::session::Session::s_pInstance != NULL) {
        nn::pia::session::Session::s_pInstance->Cleanup();
    }
    m_IsSessionStarted = false;
    m_SendSkipCount = 0;
    if (nn::pia::session::Session::s_pInstance != NULL) {
        nn::pia::session::Session::s_pInstance->ClearEventCallback();
        nn::pia::session::Session::DestroyInstance();
    }
    nn::pia::session::Finalize();
    nn::pia::transport::Transport* transport = nn::pia::transport::Transport::s_pInstance;
    if (transport != NULL) {
        nn::pia::transport::ReliableProtocol* protocol = transport->m_ProtocolManager.GetProtocol<nn::pia::transport::ReliableProtocol>(
            m_ReliableProtocolId, nn::pia::transport::PROTOCOL_TYPE_RELIABLE);
        if (protocol != NULL) {
            protocol->Finalize();
            transport->m_ProtocolManager.DestroyProtocol(m_ReliableProtocolId.m_Id);
        }
        m_ReliableProtocolId.m_Id = 0;
        nn::pia::transport::Transport::DestroyInstance();
    }
    if (nn::pia::transport::IsInitialized()) {
        nn::pia::transport::Finalize();
    }
    if (nn::pia::inet::NexFacade::s_pInstance != NULL) {
        nn::pia::inet::NexFacade::s_pInstance->Unbind();
        nn::pia::inet::NexFacade::DestroyInstance();
    }
    nn::pia::inet::Finalize();
    if (m_pMessaging != NULL) {
        m_pMessaging->UnregisterNotificationHandler();
        m_pMessaging->Unbind();
        if (m_pMessaging != NULL) {
            delete m_pMessaging;
        }
        m_pMessaging = NULL;
    }
    if (m_pMatchmakeExtension != NULL) {
        m_pMatchmakeExtension->Unbind();
        if (m_pMatchmakeExtension != NULL) {
            delete m_pMatchmakeExtension;
        }
        m_pMatchmakeExtension = NULL;
    }
}

// 0x00514634 (name is ours)
void net::nex::Impl::ReceiveNotifications(u32 type)
{
    if (m_pMatchmakeExtension == NULL || !m_pMatchmakeExtension->IsConnected() || m_pNgsFacade == NULL) {
        return;
    }
    nn::nex::qList<nn::nex::NotificationEvent> events;
    nn::nex::ProtocolCallContext context;
    if (m_pMatchmakeExtension->Method10(&context, &events, type)) {
        context.Wait(0xFFFFFFFF);
        if (context.GetOutcome()) {
            for (nn::nex::qList<nn::nex::NotificationEvent>::Node* node = events.m_pHead->m_pNext; node != events.m_pHead;
                 node = node->m_pNext) {
                OnNotificationEvent(node->m_Value, false);
            }
        }
    }
}

// 0x00514F2C (name is ours)
bool net::nex::Impl::SendNotification(u32 type, u32 param1, u32 param2, const nn::nex::String& text)
{
    if (m_pMatchmakeExtension == NULL || !m_pMatchmakeExtension->IsConnected() || m_pNgsFacade == NULL ||
        text.GetLength() > 256) {
        return false;
    }
    nn::nex::ProtocolCallContext context;
    if (!m_pMatchmakeExtension->Method9(&context, type, param1, param2, nn::nex::String(text))) {
        ReportSystemError(3);
        return false;
    }
    context.Wait(0xFFFFFFFF);
    if (context.GetOutcome()) {
        return true;
    }
    nn::nex::qResult unused(static_cast<int>(0x8001000A));
    u32 errorCode = nn::nex::ErrorCodeConverter::ConvertToNetworkErrorCode(context.GetOutcome());
    if (m_ErrorCallback != NULL) {
        m_ErrorCallback(3, errorCode);
    }
    return false;
}
} // namespace nex
} // namespace net

namespace net {
namespace nex {
// (inline) the startup setting of the sessions over nex
static void SetNexStartupSetting(nn::pia::session::Session::StartupSetting* setting)
{
    setting->m_IsHostMigrationEnabled = true;
    setting->m_pIdentificationData = NULL;
    setting->m_CryptoMode = static_cast<nn::pia::common::Crypto::Mode>(0);
    setting->m_TimeoutMSec = 10000;
    setting->m_KeepAliveIntervalMSec = 1000;
    setting->m_BandwidthCheckBandwidth = -1;
    setting->m_BandwidthCheckPacketSize = 0;
    setting->m_BandwidthCheckDurationMSec = 1000;
    setting->m_IsBandwidthCheckOneWay = false;
    setting->m_pPlayerName = NULL;
}

// 0x00510DC0 (name is ours)
nn::Result net::nex::Impl::JoinSessionImpl(const nn::pia::session::JoinSessionSetting& setting)
{
    nn::Result result = nn::pia::common::RESULT_NOT_SET;
    nn::pia::session::Session::StartupSetting startupSetting;
    SetNexStartupSetting(&startupSetting);
    result = StartupSession(startupSetting);
    if (result.IsFailure()) {
        ReportPiaResult(3, result, 0, 0);
    } else {
        result = nn::pia::session::Session::s_pInstance->JoinSessionAsync(&setting);
        if (result.IsSuccess()) {
            while (!nn::pia::session::Session::s_pInstance->IsJoinSessionAsyncCompleted()) {
                Dispatch();
            }
            result = nn::pia::session::Session::s_pInstance->GetJoinSessionAsyncResult();
            if (result.IsSuccess()) {
                m_IsInSession = true;
                return nn::Result();
            }
        }
    }
    if (nn::pia::session::Session::s_pInstance != NULL) {
        nn::pia::session::Session::s_pInstance->Cleanup();
    }
    m_IsSessionStarted = false;
    m_SendSkipCount = 0;
    return result;
}

// 0x00511A24 (name is ours)
bool net::nex::Impl::CreateSessionAsync(const nn::pia::session::CreateSessionSetting& setting)
{
    nn::Result result = nn::pia::common::RESULT_NOT_SET;
    nn::pia::session::Session::s_pInstance->m_JoinApprovalCallback = OnJoinApproval;
    result = nn::pia::session::Session::s_pInstance->CreateSessionAsync(&setting);
    if (result.IsFailure()) {
        nn::pia::session::Session::s_pInstance->ClearJoinApprovalCallback();
    } else {
        while (!nn::pia::session::Session::s_pInstance->IsCreateSessionAsyncCompleted()) {
            Dispatch();
        }
        result = nn::pia::session::Session::s_pInstance->GetCreateSessionAsyncResult();
        if (result.IsSuccess()) {
            m_IsInSession = true;
            return true;
        }
    }
    ReportPiaResult(3, result, 0, 0);
    return false;
}

// 0x00514090 | libgarden
s32 net::nex::Impl::JoinSessionByOwner(const u32& ownerPid)
{
    m_OwnerPrincipalId = ownerPid;
    nn::pia::inet::NexSessionSearchCriteriaOwner criteria;
    criteria.m_OwnerPrincipalId = ownerPid;
    ISessionInfo* info;
    if (SearchSessions(criteria, &info, 1) != 1) {
        return static_cast<s32>(0x80000006);
    }
    m_SessionId = static_cast<nn::pia::inet::NexSessionInfo*>(info)->vf_0x0C();
    nn::pia::inet::NexJoinSessionSetting setting;
    setting.m_SessionId = m_SessionId;
    return JoinSession(setting);
}

// 0x005148EC (name is ours)
s32 net::nex::Impl::JoinSession(const nn::pia::session::JoinSessionSetting& setting)
{
    nn::Result result = JoinSessionImpl(setting);
    if (result.IsSuccess()) {
        nn::pia::transport::StationManager::s_pInstance->GetStation(nn::pia::session::Session::s_pInstance->m_HostStationId)
            ->GetPrincipalId(&m_OwnerPrincipalId);
        if (nn::pia::session::Session::s_pInstance != NULL) {
            nn::pia::session::Session::SetSyncClockRequestInterval(500);
        }
        return 0;
    }
    if (result == nn::pia::common::RESULT_MATCHMAKE_FAILED_244 || result == nn::pia::common::RESULT_JOIN_REFUSED) {
        return static_cast<s32>(0x80000002);
    }
    if (result == nn::pia::common::RESULT_MATCHMAKE_SESSION_GONE || result == nn::pia::common::RESULT_MATCHMAKE_FAILED_251) {
        return static_cast<s32>(0x80000001);
    }
    if (result == nn::pia::common::RESULT_JOIN_DENIED) {
        return static_cast<s32>(0x80000003);
    }
    ReportPiaResult(3, result, 0, 0);
    return static_cast<s32>(0x80000006);
}

// 0x005149C4 (name is ours)
bool net::nex::Impl::CreateSession(u32 attribute)
{
    nn::pia::inet::NexCreateSessionSetting setting;
    setting.m_Unknown0x4 = 1;
    setting.m_Unknown0x6 = 4;
    setting.m_IsOpenParticipation = false;
    setting.SetAttribute(3, 8);
    setting.SetAttribute(4, 2);
    setting.SetAttribute(5, unk_0x60);
    setting.m_Unknown0x8 = SESSION_GAME_MODE;
    setting.m_Unknown0xC = 0;
    setting.SetAttribute(1, attribute);
    setting.SetAttribute(3, 8 | (s_SessionAttribute3 << 5));
    setting.SetAttribute(2, 2);
    return CreateSessionImpl(setting);
}

// 0x00514AC4 (name is ours)
bool net::nex::Impl::RequestServiceToken()
{
    bool isSuccess = false;
    nn::nex::ProtocolCallContext context;
    if (m_pIndependentServer->RequestServiceToken(&context, GAME_SERVER_ID, INDEPENDENT_ACCESS_KEY, s_ServiceToken, true)) {
        context.Wait(0xFFFFFFFF);
        if (context.GetOutcome()) {
            s_HasServiceToken = true;
            isSuccess = true;
        } else {
            u32 errorCode = nn::nex::ErrorCodeConverter::ConvertToNetworkErrorCode(context.GetOutcome());
            if (m_ErrorCallback != NULL) {
                m_ErrorCallback(3, errorCode);
            }
        }
    }
    return isSuccess;
}

// 0x00514BAC | libgarden
s32 net::nex::Impl::JoinSessionById(u32 sessionId)
{
    m_SessionId = sessionId;
    nn::pia::inet::NexJoinSessionSetting setting;
    setting.m_SessionId = sessionId;
    return JoinSession(setting);
}

// 0x00515154 (name is ours)
bool net::nex::Impl::CreateSessionImpl(const nn::pia::session::CreateSessionSetting& setting)
{
    nn::pia::session::Session::StartupSetting startupSetting;
    SetNexStartupSetting(&startupSetting);
    nn::Result result = StartupSession(startupSetting);
    if (result.IsFailure()) {
        ReportPiaResult(3, result, 0, 0);
    } else if (CreateSessionAsync(setting)) {
        nn::pia::session::Session* session = nn::pia::session::Session::s_pInstance;
        m_SessionId = session->m_SessionIds[session->m_CurrentIndex];
        nn::pia::transport::StationManager::s_pInstance->GetStation(nn::pia::session::Session::s_pInstance->m_HostStationId)
            ->GetPrincipalId(&m_OwnerPrincipalId);
        return true;
    }
    if (nn::pia::session::Session::s_pInstance != NULL) {
        nn::pia::session::Session::s_pInstance->Cleanup();
    }
    m_IsSessionStarted = false;
    m_SendSkipCount = 0;
    return false;
}

// 0x0051595C (name is ours)
void net::nex::Impl::ResetSyncClockRequestInterval()
{
    if (nn::pia::session::Session::s_pInstance != NULL) {
        nn::pia::session::Session::SetSyncClockRequestInterval(2000);
    }
}

// 0x0051597C (name is ours)
bool net::nex::Impl::SendNotification103(u32 param1, u32 param2, const wchar_t* text)
{
    return SendNotification(NOTIFICATION_TYPE_103, param1, param2, nn::nex::String(text));
}

// 0x005159CC (name is ours)
bool net::nex::Impl::OnJoinApproval(const nn::pia::transport::Station::IdentificationInfo* info)
{
    bool isApproved = false;
    if (s_pInstance != NULL && s_pInstance->m_JoinApprovalCallback != NULL) {
        JoinApprovalCallback callback = s_pInstance->m_JoinApprovalCallback;
        char16 name[16];
        memcpy(name, info, sizeof(name));
        u32 length = 0;
        for (const char16* p = name; *p != 0; p++) {
            length++;
        }
        if (callback(name, length)) {
            isApproved = true;
        }
    }
    return isApproved;
}
} // namespace nex
} // namespace net

namespace net {
namespace nex {
// 0x0051173C | libgarden
size_t net::nex::Impl::SearchSessions(const SessionSearchCriteria& criteria, ISessionInfo** buffer, size_t bufferSize)
{
    nn::Result result = nn::pia::common::RESULT_NOT_SET;
    nn::pia::session::Session::StartupSetting startupSetting;
    SetNexStartupSetting(&startupSetting);
    result = StartupSession(startupSetting);
    if (result.IsSuccess()) {
        result = nn::pia::session::Session::s_pInstance->BrowseSessionAsync(&criteria);
        if (result.IsSuccess()) {
            while (!nn::pia::session::Session::s_pInstance->IsBrowseSessionAsyncCompleted()) {
                Dispatch();
            }
            result = nn::pia::session::Session::s_pInstance->GetBrowseSessionAsyncResult();
            if (result.IsSuccess()) {
                if (buffer == NULL) {
                    return 0;
                }
                size_t count = 0;
                nn::pia::session::ISessionInfoList* list = nn::pia::session::Session::s_pInstance->GetSessionInfoList();
                ISessionInfo** it = list->Begin();
                s32 num = list->End() - it;
                for (s32 i = 1; i < num + 1 && count < bufferSize; i++) {
                    buffer[count] = *it++;
                    count++;
                }
                return count;
            }
        }
    }
    ReportPiaResult(3, result, 0, 0);
    if (nn::pia::session::Session::s_pInstance != NULL) {
        nn::pia::session::Session::s_pInstance->Cleanup();
    }
    m_IsSessionStarted = false;
    m_SendSkipCount = 0;
    return 0;
}

// 0x00511928 (libgarden name)
bool net::nex::Impl::CloseNexSession()
{
    if (m_pMatchmakeExtension == NULL || !m_pMatchmakeExtension->IsConnected() || m_pNgsFacade == NULL) {
        return false;
    }
    nn::Result result = nn::pia::session::Session::s_pInstance->CloseParticipationAsync();
    if (result.IsSuccess()) {
        while (!nn::pia::session::Session::s_pInstance->IsCloseParticipationAsyncCompleted()) {
            Dispatch();
        }
        result = nn::pia::session::Session::s_pInstance->GetCloseParticipationAsyncResult();
        if (result.IsSuccess()) {
            m_IsSessionOpen = false;
            SendNotification(NOTIFICATION_TYPE_101, m_SessionId, 0x8020000, nn::nex::String(L""));
            return true;
        }
    }
    ReportPiaResult(3, result, 0, 0);
    return false;
}

// 0x00511AF0 (name is ours)
bool net::nex::Impl::OpenParticipation()
{
    nn::Result result = nn::pia::session::Session::s_pInstance->OpenParticipationAsync();
    if (result.IsSuccess()) {
        while (!nn::pia::session::Session::s_pInstance->IsOpenParticipationAsyncCompleted()) {
            Dispatch();
        }
        result = nn::pia::session::Session::s_pInstance->GetOpenParticipationAsyncResult();
        if (result.IsSuccess()) {
            m_IsSessionOpen = true;
            return true;
        }
    }
    ReportPiaResult(3, result, 0, 0);
    return false;
}

// 0x00511B8C (name is ours)
bool net::nex::Impl::InitializeNexCommon()
{
    if (InitializeCommon()) {
        return true;
    }
    return false;
}
} // namespace nex
} // namespace net

namespace net {
namespace nex {
// 0x00511BA0 (name is ours)
void net::nex::Impl::ScanLocal(u8 mode, ScanBufBase** results, u32 resultNum, u32* foundNum, bool withData)
{
    *foundNum = 0;
    m_ScanMode = mode;
    nn::pia::local::LocalSessionSearchCriteria criteria;
    if (mode != 0) {
        criteria.m_LocalCommunicationId =
            nn::pia::local::LocalNetwork::s_pInstance->CreateLocalCommunicationId(LOCAL_COMMUNICATION_ID_1, false);
    } else {
        criteria.m_LocalCommunicationId =
            nn::pia::local::LocalNetwork::s_pInstance->CreateLocalCommunicationId(LOCAL_COMMUNICATION_ID_0, false);
    }
    criteria.m_SubId = mode + 8;
    criteria.m_ResultNumMax = 16;
    criteria.m_IsOpenedOnly = true;
    criteria.m_ResultOffset = 0;
    u32 num = SearchSessions(criteria, m_pSessionInfos, 16);
    if (num == 0) {
        return;
    }
    u32 count = 0;
    if (withData) {
        for (u32 i = 0; i < num && count < resultNum; i++) {
            nn::pia::local::LocalSessionInfo* info = static_cast<nn::pia::local::LocalSessionInfo*>(m_pSessionInfos[i]);
            if (!info->IsOpened()) {
                continue;
            }
            u32 size = 0;
            info->GetApplicationDataSize(&size);
            if (size > results[count]->GetBufferSize()) {
                continue;
            }
            u8 data[200];
            if (info->GetApplicationData(data, sizeof(data)).IsFailure()) {
                continue;
            }
            memcpy(results[count]->GetBuffer(), data, size);
            ScanBufBase* result = results[count];
            result->m_OwnerPrincipalId = 0;
            result->m_SessionId = info->GetSessionId();
            result->m_Param = 0;
            result = results[count];
            result->m_ParticipantNum = info->GetCurrentParticipants();
            count++;
        }
    } else {
        for (u32 i = 0; i < num && count < resultNum; i++) {
            nn::pia::local::LocalSessionInfo* info = static_cast<nn::pia::local::LocalSessionInfo*>(m_pSessionInfos[i]);
            if (!info->IsOpened()) {
                continue;
            }
            ScanBufBase* result = results[count];
            result->m_OwnerPrincipalId = 0;
            result->m_SessionId = info->GetSessionId();
            result->m_Param = 0;
            result = results[count];
            result->m_ParticipantNum = info->GetCurrentParticipants();
            count++;
        }
    }
    *foundNum = count;
}
} // namespace nex
} // namespace net

namespace net {
namespace nex {
namespace {
// the passphrase and application data of the local sessions
const char* const LOCAL_PASSPHRASE = "kpuapgcdumjbpadqdgxyheiyormpwoks";
const char* const LOCAL_APPLICATION_DATA = "sulpobiimairomustub";

// (inline) the session info of the pia session with the session id
DECOMP_ALWAYS_INLINE nn::pia::session::ISessionInfo* FindSessionInfo(u32 sessionId)
{
    nn::pia::session::ISessionInfoList* list = nn::pia::session::Session::s_pInstance->GetSessionInfoList();
    nn::pia::session::ISessionInfo** end = list->End();
    for (nn::pia::session::ISessionInfo** it = list->Begin(); it != end; it++) {
        if (static_cast<nn::pia::local::LocalSessionInfo*>(*it)->GetSessionId() == sessionId) {
            return *it;
        }
    }
    return NULL;
}
} // namespace

// 0x00511F5C (name is ours)
void net::nex::Impl::Update()
{
    m_CriticalSection.lock();
    if (nn::pia::common::Scheduler::s_pInstance != NULL) {
        nn::pia::common::Scheduler::s_pInstance->Dispatch(5);
    }
    m_CriticalSection.unlock();
    nn::pia::transport::Transport* transport = nn::pia::transport::Transport::s_pInstance;
    if (transport == NULL) {
        return;
    }
    nn::Result result = transport->m_StreamResult;
    if (result.IsFailure()) {
        if (m_IsInSession) {
            ReportPiaResult(2, result, 0, 0);
            m_IsInSession = false;
        }
        return;
    }
    nn::pia::StationId stationId;
    u32 size;
    result = transport->m_ProtocolManager
                 .GetProtocol<nn::pia::transport::ReliableProtocol>(m_ReliableProtocolId, nn::pia::transport::PROTOCOL_TYPE_RELIABLE)
                 ->Receive(&stationId, s_ReceiveBuffer, &size, sizeof(s_ReceiveBuffer));
    if (result.IsSuccess()) {
        if (m_ReceiveCallback != NULL) {
            m_ReceiveCallback(stationId, s_ReceiveBuffer, size);
        }
    } else if (result != nn::pia::common::RESULT_NO_DATA && result != nn::pia::common::RESULT_NOT_IN_COMMUNICATION) {
        ReportPiaResult(3, result, 0, 0);
    }
}

// 0x0051208C (name is ours)
bool net::nex::Impl::CloseParticipation()
{
    nn::Result result = nn::pia::session::Session::s_pInstance->CloseParticipationAsync();
    if (result.IsSuccess()) {
        while (!nn::pia::session::Session::s_pInstance->IsCloseParticipationAsyncCompleted()) {
            Dispatch();
        }
        result = nn::pia::session::Session::s_pInstance->GetCloseParticipationAsyncResult();
        if (result.IsSuccess()) {
            m_IsSessionOpen = false;
            return true;
        }
    }
    ReportPiaResult(3, result, 0, 0);
    return false;
}

// 0x00512124 (name is ours)
bool net::nex::Impl::DeleteOwnData()
{
    if (m_pDataStore == NULL || !m_pDataStore->IsConnected()) {
        u32 errorCode = 0;
        if (!AcConnection::IsConnected()) {
            nn::ac::CTR::GetLastErrorCode(&errorCode);
        }
        if (m_ErrorCallback != NULL) {
            m_ErrorCallback(2, errorCode);
        }
        return false;
    }
    nn::nex::DataStoreSearchParam param;
    param.m_SearchTarget = 10;
    param.m_ResultRange = nn::nex::ResultRange(0, 100);
    param.m_TotalCountEnabled = true;
    nn::nex::DataStoreSearchResult result;
    while (true) {
        {
            nn::nex::ProtocolCallContext context;
            if (!m_pDataStore->SearchObject(&context, param, &result)) {
                ReportSystemError(1);
                break;
            }
            context.Wait(0xFFFFFFFF);
            if (!context.GetOutcome()) {
                u32 errorCode = nn::nex::ErrorCodeConverter::ConvertToNetworkErrorCode(context.GetOutcome());
                if (m_ErrorCallback != NULL) {
                    m_ErrorCallback(1, errorCode);
                }
                break;
            }
        }
        if (result.m_TotalCount == 0) {
            break;
        }
        for (nn::nex::DataStoreMetaInfo* it = result.m_Result.begin(); it != result.m_Result.end(); it++) {
            nn::nex::DataStoreDeleteParam deleteParam;
            deleteParam.m_DataId = static_cast<u32>(it->m_DataId);
            deleteParam.m_UpdatePassword = 0;
            nn::nex::ProtocolCallContext context;
            if (m_pDataStore->DeleteObject(&context, deleteParam)) {
                context.Wait(0xFFFFFFFF);
                context.GetOutcome();
            }
        }
    }
    return true;
}

// 0x005123F8 (name is ours)
s32 net::nex::Impl::JoinLocalSession(u32 sessionId)
{
    nn::pia::session::ISessionInfo* info = NULL;
    if (nn::pia::session::Session::s_pInstance != NULL) {
        info = FindSessionInfo(sessionId);
    }
    if (info == NULL) {
        nn::pia::local::LocalSessionSearchCriteria criteria;
        if (m_ScanMode != 0) {
            criteria.m_LocalCommunicationId =
                nn::pia::local::LocalNetwork::s_pInstance->CreateLocalCommunicationId(LOCAL_COMMUNICATION_ID_1, false);
        } else {
            criteria.m_LocalCommunicationId =
                nn::pia::local::LocalNetwork::s_pInstance->CreateLocalCommunicationId(LOCAL_COMMUNICATION_ID_0, false);
        }
        criteria.m_SubId = m_ScanMode + 8;
        criteria.m_ResultNumMax = 16;
        criteria.m_IsOpenedOnly = true;
        criteria.m_ResultOffset = 0;
        SearchSessions(criteria, NULL, 0);
        if (nn::pia::session::Session::s_pInstance != NULL) {
            info = FindSessionInfo(sessionId);
        }
        if (info == NULL) {
            return static_cast<s32>(0x80000001);
        }
    }
    nn::pia::local::UdsJoinSessionSetting setting;
    setting.m_pSessionInfo = info;
    setting.SetPassphrase(LOCAL_PASSPHRASE, 33);
    setting.SetApplicationData(LOCAL_APPLICATION_DATA, 20);
    nn::Result result = JoinSessionImpl(setting);
    if (result.IsSuccess()) {
        if (nn::pia::session::Session::s_pInstance != NULL) {
            nn::pia::session::Session::SetSyncClockRequestInterval(500);
        }
        return 0;
    }
    if (result == nn::pia::common::RESULT_MATCHMAKE_FAILED_244 || result == nn::pia::common::RESULT_JOIN_REFUSED) {
        return static_cast<s32>(0x80000002);
    }
    if (result == nn::pia::common::RESULT_MATCHMAKE_SESSION_GONE || result == nn::pia::common::RESULT_MATCHMAKE_FAILED_251) {
        return static_cast<s32>(0x80000001);
    }
    if (result == nn::pia::common::RESULT_JOIN_DENIED) {
        return static_cast<s32>(0x80000003);
    }
    if (result != nn::pia::common::RESULT_TIMEOUT) {
        ReportPiaResult(2, result, 0, 0);
    }
    return static_cast<s32>(0x80000006);
}
} // namespace nex
} // namespace net

namespace net {
namespace nex {
// 0x005127DC (name is ours)
void net::nex::Impl::StoreNotification(const u32& source, u32 key, u32 param, const nn::nex::String& text)
{
    ReceivePool* pool = m_pReceivePool;
    if (pool == NULL) {
        return;
    }
    u32 searchKey = key;
    ReceiveBuffer* buffer = NULL;
    for (s32 i = 0; i < pool->m_Used.size(); i++) {
        if (CompareSecondWord(pool->m_Used.at(i), &searchKey) == 0) {
            buffer = pool->m_Used.at(i);
            break;
        }
    }
    if (buffer == NULL) {
        buffer = m_pReceivePool->Alloc();
        if (buffer == NULL) {
            return;
        }
    }
    buffer->m_Source = source;
    buffer->m_Param = param;
    buffer->m_Key = key;
    script::WordPtrSv(buffer->m_Text, 256)
        .Copy(script::WordCPtrSv(reinterpret_cast<const char16*>(text.m_pString), text.GetLength() + 1));
    m_HasNotification = true;
}

// 0x00512930 (name is ours)
s32 net::nex::Impl::CreateLocalSession(u8 mode, const void* data, u32 dataSize)
{
    nn::pia::session::Session::StartupSetting startupSetting;
    SetNexStartupSetting(&startupSetting);
    nn::Result result = StartupSession(startupSetting);
    if (result.IsFailure()) {
        if (nn::pia::session::Session::s_pInstance != NULL) {
            nn::pia::session::Session::s_pInstance->Cleanup();
        }
        m_IsSessionStarted = false;
        m_SendSkipCount = 0;
        ReportPiaResult(3, result, 0, 0);
        return static_cast<s32>(0x80000006);
    }
    nn::pia::local::LocalCreateNetworkSetting networkSetting;
    networkSetting.m_SubId = mode + 8;
    networkSetting.m_Channel = 11;
    networkSetting.m_NodeCountMax = 4;
    if (mode != 0) {
        networkSetting.m_LocalCommunicationId =
            nn::pia::local::LocalNetwork::s_pInstance->CreateLocalCommunicationId(LOCAL_COMMUNICATION_ID_1, false);
    } else {
        networkSetting.m_LocalCommunicationId =
            nn::pia::local::LocalNetwork::s_pInstance->CreateLocalCommunicationId(LOCAL_COMMUNICATION_ID_0, false);
    }
    memcpy(networkSetting.m_Passphrase, LOCAL_PASSPHRASE, 33);
    networkSetting.m_PassphraseSize = 33;
    if (nn::pia::local::LocalNetwork::s_pInstance->GetBeaconApplicationDataSizeMax() >= dataSize) {
        memcpy(networkSetting.m_ApplicationData, data, dataSize);
        networkSetting.m_ApplicationDataSize = dataSize;
    }
    nn::pia::local::UdsCreateSessionSetting setting;
    setting.SetCreateNetworkSetting(networkSetting);
    setting.m_Unknown0x6 = 4;
    setting.SetApplicationData(LOCAL_APPLICATION_DATA, 20);
    if (CreateSessionAsync(setting)) {
        m_IsSessionOpen = true;
        nn::pia::local::LocalNetwork::s_pInstance->GetChannel();
        return 0;
    }
    if (nn::pia::session::Session::s_pInstance != NULL) {
        nn::pia::session::Session::s_pInstance->Cleanup();
    }
    m_IsSessionStarted = false;
    m_SendSkipCount = 0;
    return static_cast<s32>(0x80000006);
}
} // namespace nex
} // namespace net

namespace net {
namespace nex {
// 0x00512BE8 (name is ours)
bool net::nex::Impl::InitializeDataStore()
{
    if (m_pNgsFacade == NULL) {
        return false;
    }
    if (m_pDataStore != NULL && m_pDataStore->IsConnected()) {
        return true;
    }
    nn::nex::Credentials* credentials = m_pNgsFacade->m_pCredentials;
    if (credentials == NULL) {
        return false;
    }
    if (nn::http::Initialize(0, 0).IsSuccess()) {
        m_pDataStore = new nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>;
        if (m_pDataStore != NULL && m_pDataStore->Bind(credentials)) {
            return true;
        }
    }
    ReportSystemError(2);
    if (m_pDataStore != NULL) {
        m_pDataStore->Unbind();
        if (m_pDataStore != NULL) {
            delete m_pDataStore;
        }
        m_pDataStore = NULL;
    }
    nn::http::Finalize();
    return false;
}

// 0x00513120 (name is ours)
bool net::nex::Impl::AutoMatchmake(const nn::pia::session::CreateSessionSetting& createSetting,
                                   const SessionSearchCriteria& criteria)
{
    nn::Result result = nn::pia::common::RESULT_NOT_SET;
    nn::pia::session::Session::StartupSetting startupSetting;
    SetNexStartupSetting(&startupSetting);
    result = StartupSession(startupSetting);
    if (result.IsFailure()) {
        ReportPiaResult(3, result, 0, 0);
    } else {
        nn::pia::session::Session::s_pInstance->m_JoinApprovalCallback = OnJoinApproval;
        result = nn::pia::session::Session::s_pInstance->AutoMatchmakeAsync(&createSetting, &criteria, 1);
        if (result.IsFailure()) {
            nn::pia::session::Session::s_pInstance->ClearJoinApprovalCallback();
            ReportPiaResult(3, result, 0, 0);
        } else {
            while (!nn::pia::session::Session::s_pInstance->IsAutoMatchmakeAsyncCompleted()) {
                Dispatch();
            }
            result = nn::pia::session::Session::s_pInstance->GetAutoMatchmakeAsyncResult();
            if (result.IsSuccess()) {
                m_IsInSession = true;
                return true;
            }
            if (result == nn::pia::common::RESULT_JOIN_REFUSED || result == nn::pia::common::RESULT_MATCHMAKE_SESSION_GONE ||
                result == nn::pia::common::RESULT_JOIN_DENIED) {
                // (not reported)
            } else if (result == nn::pia::common::RESULT_INVALID_ARGUMENT || result == nn::pia::common::RESULT_INVALID_STATE ||
                       result == nn::pia::common::RESULT_NOT_IN_SESSION || result == nn::pia::common::RESULT_MATCHMAKE_FAILED_247 ||
                       result == nn::pia::common::RESULT_FATAL_196 || result == nn::pia::common::RESULT_UNREGISTER_FAILED) {
                ReportPiaResult(3, result, 0, 0);
            }
        }
    }
    if (nn::pia::session::Session::s_pInstance != NULL) {
        nn::pia::session::Session::s_pInstance->Cleanup();
    }
    m_IsSessionStarted = false;
    m_SendSkipCount = 0;
    return false;
}
} // namespace nex
} // namespace net

namespace net {
namespace nex {
// 0x00513410 (name is ours)
void net::nex::Impl::GetFriendCode(nn::pia::StationId stationId, u64* friendCode)
{
    nn::pia::transport::StationManager* stationManager = nn::pia::transport::StationManager::s_pInstance;
    if (stationManager != NULL) {
        nn::pia::common::StationAddress address;
        if (stationManager->GetStationAddress(&address, stationId).IsSuccess() &&
            nn::pia::local::LocalNetwork::s_pInstance != NULL) {
            u64 code;
            nn::pia::local::LocalStationInfo info;
            if (nn::pia::local::LocalNetwork::s_pInstance->GetStationInfo(&info, address).IsSuccess() &&
                nn::friends::CTR::detail::UnscrambleLocalFriendCode(
                    &code, reinterpret_cast<const nn::friends::CTR::ScrambledFriendCode*>(info.m_ScrambledLocalFriendCode), 1)
                    .IsSuccess()) {
                *friendCode = code;
                return;
            }
        }
    }
    *friendCode = 0;
}

// 0x005134EC (name is ours)
bool net::nex::Impl::SendNotification102(const wchar_t* text)
{
    if (text == NULL) {
        return false;
    }
    return SendNotification(NOTIFICATION_TYPE_102, 0, 0, nn::nex::String(text));
}

// 0x0051353C (name is ours)
bool net::nex::Impl::SendTextMessage(const u32* principalId, const wchar_t* text)
{
    if (m_pNgsFacade == NULL) {
        return false;
    }
    if (m_pMessaging == NULL || !m_pMessaging->IsConnected() || text == NULL) {
        return false;
    }
    nn::nex::String subject(text);
    if (subject.GetLength() > 256) {
        return false;
    }
    nn::nex::TextMessage* message = new nn::nex::TextMessage;
    message->m_strSubject = subject;
    nn::nex::CustomDataHolder<nn::nex::UserMessage> holder;
    holder.m_pObject = message;
    nn::nex::ProtocolCallContext context;
    nn::nex::MessageRecipient recipient;
    recipient.m_idRecipient = *principalId;
    recipient.m_uiRecipientType = 1;
    if (!m_pMessaging->DeliverMessage(&context, recipient, holder, 1, 0)) {
        ReportSystemError(3);
        return false;
    }
    context.Wait(0xFFFFFFFF);
    if (context.GetOutcome()) {
        return true;
    }
    u32 errorCode = nn::nex::ErrorCodeConverter::ConvertToNetworkErrorCode(context.GetOutcome());
    if (m_ErrorCallback != NULL) {
        m_ErrorCallback(3, errorCode);
    }
    return false;
}
} // namespace nex
} // namespace net

namespace net {
namespace nex {
// 0x00513970 (libgarden name)
bool net::nex::Impl::JoinRandomSession(u32 attribute)
{
    nn::pia::inet::NexCreateSessionSetting setting;
    setting.m_Unknown0x4 = 1;
    setting.m_Unknown0x6 = 4;
    setting.m_IsOpenParticipation = false;
    setting.SetAttribute(3, 8);
    setting.SetAttribute(4, 2);
    setting.SetAttribute(5, unk_0x60);
    setting.m_Unknown0x8 = SESSION_GAME_MODE;
    setting.m_Unknown0xC = 0;
    setting.SetAttribute(1, attribute);
    setting.SetAttribute(3, 8 | (s_SessionAttribute3 << 5));
    setting.SetAttribute(2, 1);
    nn::pia::inet::NexSessionSearchCriteriaUnused criteria;
    criteria.m_Unknown0x4 = 2;
    criteria.SetGameMode(SESSION_GAME_MODE);
    criteria.SetMinParticipants(1);
    criteria.SetMaxParticipants(4);
    criteria.SetMatchmakeSystemType(0);
    criteria.m_Unknown0x9D4 = 0;
    criteria.SetOpenedOnly(true);
    criteria.SetVacantOnly(true);
    criteria.SetAttribute(3, 8 | (s_SessionAttribute3 << 5));
    criteria.SetAttribute(4, 2);
    criteria.SetAttribute(5, unk_0x60);
    criteria.SetAttribute(2, 1);
    criteria.SetAttribute(1, attribute);
    if (!AutoMatchmake(setting, criteria)) {
        return false;
    }
    nn::pia::session::Session* session = nn::pia::session::Session::s_pInstance;
    m_SessionId = session->m_SessionIds[session->m_CurrentIndex];
    nn::pia::transport::StationManager::s_pInstance->GetStation(nn::pia::session::Session::s_pInstance->m_HostStationId)
        ->GetPrincipalId(&m_OwnerPrincipalId);
    if (m_pMatchmakeExtension == NULL || nn::pia::session::Session::s_pInstance == NULL ||
        !nn::pia::session::Session::s_pInstance->IsHost()) {
        if (nn::pia::session::Session::s_pInstance != NULL) {
            nn::pia::session::Session::SetSyncClockRequestInterval(500);
        }
    }
    return true;
}

// 0x00513E0C (name is ours)
bool net::nex::Impl::CallMatchmakeMethod26(const u32* principalId)
{
    if (m_pMatchmakeExtension == NULL || !m_pMatchmakeExtension->IsConnected() || m_pNgsFacade == NULL || *principalId == 0) {
        return false;
    }
    nn::nex::qList<u32> principalIds;
    principalIds.push_back(*principalId);
    nn::nex::ProtocolCallContext context;
    if (!m_pMatchmakeExtension->Method26(&context, principalIds)) {
        ReportSystemError(3);
        return false;
    }
    context.Wait(0xFFFFFFFF);
    if (context.GetOutcome()) {
        return true;
    }
    u32 errorCode = nn::nex::ErrorCodeConverter::ConvertToNetworkErrorCode(context.GetOutcome());
    if (m_ErrorCallback != NULL) {
        m_ErrorCallback(3, errorCode);
    }
    return false;
}
} // namespace nex
} // namespace net

namespace net {
namespace nex {
// 0x005144F4 (name is ours)
void net::nex::Impl::SetLocalApplicationData(const void* data, u32 size)
{
    if (data != NULL && size != 0) {
        if (size > 184) {
            size = 184;
        }
        nn::pia::local::LocalNetwork::s_pInstance->SetApplicationData(data, size);
    }
}

// 0x00514BF8 (name is ours)
void net::nex::Impl::OnNotificationEvent(const nn::nex::NotificationEvent& event, bool isFromHandler)
{
    switch (event.m_uiType / 1000) {
    case 3:
        break;
    case 4:
        m_OwnerPrincipalId = event.m_uiParam2;
        break;
    case NOTIFICATION_TYPE_101: {
        u32 param = event.m_uiParam2 & 0xFFFF;
        u32 kind = event.m_uiParam2 >> 16 << 16;
        if (kind == 0x8010000) {
            nn::nex::String text(event.m_strParam);
            StoreNotification(event.m_pidSource, event.m_uiParam1, param, text);
        } else if (kind == 0x8020000) {
            u32 key = event.m_uiParam1;
            ReceivePool* pool = m_pReceivePool;
            if (pool == NULL) {
                break;
            }
            for (s32 i = 0; i < pool->m_Used.size(); i++) {
                if (CompareSecondWord(pool->m_Used.at(i), &key) == 0) {
                    pool->Erase(i);
                    m_HasNotification = true;
                    break;
                }
            }
        }
        break;
    }
    case NOTIFICATION_TYPE_102:
        if (m_MessageCallback != NULL) {
            u32 length = event.m_strParam.GetLength();
            u32 source = event.m_pidSource;
            m_MessageCallback(&source, event.m_strParam.m_pString, length, 1);
        }
        break;
    case NOTIFICATION_TYPE_103:
        if (m_NotificationCallback != NULL) {
            u32 length = event.m_strParam.GetLength();
            u32 source = event.m_pidSource;
            m_NotificationCallback(&source, event.m_uiParam1, event.m_uiParam2, event.m_strParam.m_pString, length,
                                   isFromHandler);
            m_HasNotification = true;
        }
        break;
    case 109:
    case 110:
    case 113:
        break;
    case 111: {
        u32 source = event.m_pidSource;
        ReceivePool* pool = m_pReceivePool;
        if (pool != NULL) {
            for (s32 i = 0; i < pool->m_Used.size(); i++) {
                if (CompareWord(pool->m_Used.at(i), &source) == 0) {
                    pool->Erase(i);
                    m_HasNotification = true;
                    break;
                }
            }
        }
        if (m_NotificationCallback != NULL) {
            u32 source2 = event.m_pidSource;
            m_NotificationCallback(&source2, 0, 0, NULL, 0, false);
        }
        break;
    }
    case 114: {
        nn::nex::DateTime time;
        time.SetUnixTime(event.m_uiParam1);
        time.ToFndDateTime();
        break;
    }
    default:
        break;
    }
}
} // namespace nex
} // namespace net

namespace net {
namespace nex {
// 0x005150C0 (name is ours)
bool net::nex::Impl::SetPlayerName(const char16* name, u32 length)
{
    if (s_pNexHeap == NULL) {
        return false;
    }
    if (m_IsInSession || length > 16) {
        return false;
    }
    script::WordPtrSv(m_PlayerName, 17).Copy(script::WordCPtrSv(name, length + 1));
    return true;
}

// 0x005152C4 (name is ours)
bool net::nex::Impl::ModifySessionAttribute()
{
    nn::Result result = nn::pia::session::Session::s_pInstance->ModifyAttributeAsync(2, 1);
    if (result.IsSuccess()) {
        while (!nn::pia::session::Session::s_pInstance->IsModifyAttributeAsyncCompleted()) {
            Dispatch();
        }
        result = nn::pia::session::Session::s_pInstance->GetModifyAttributeAsyncResult();
        if (result.IsSuccess()) {
            return true;
        }
    }
    ReportPiaResult(3, result, 0, 0);
    return false;
}

// 0x00515360 (name is ours)
bool net::nex::Impl::CreateNexSession(const char16* text, u32 length, u16 param)
{
    m_SessionParam = param;
    nn::pia::inet::NexCreateSessionSetting setting;
    setting.m_Unknown0x4 = 1;
    setting.m_Unknown0x6 = 4;
    setting.m_IsOpenParticipation = false;
    setting.SetAttribute(3, 8);
    setting.SetAttribute(4, 2);
    setting.SetAttribute(5, unk_0x60);
    setting.m_Unknown0x8 = FRIEND_SESSION_GAME_MODE;
    setting.m_Unknown0xC = 1;
    if (!CreateSessionImpl(setting)) {
        return false;
    }
    if (text != NULL && length != 0 && length < 256) {
        script::WordPtrSv(reinterpret_cast<char16*>(m_SessionText), 256).Copy(script::WordCPtrSv(text, length + 1));
    }
    return true;
}

// 0x00515494 (name is ours)
void net::nex::Impl::GetSessionNotifications(ScanBufBase** results, u32 resultNum, u32* foundNum, bool withText)
{
    u32 count = 0;
    ReceiveBuffer** it = m_pReceivePool->m_Used.begin();
    ReceiveBuffer** end = m_pReceivePool->m_Used.end();
    if (withText) {
        for (; it != end && count < resultNum; it++) {
            if ((*it)->m_Param >> 16 != 0) {
                continue;
            }
            if (results[count]->GetBufferSize() < sizeof((*it)->m_Text)) {
                continue;
            }
            memcpy(results[count]->GetBuffer(), (*it)->m_Text, sizeof((*it)->m_Text));
            ReceiveBuffer* buffer = *it;
            results[count]->m_OwnerPrincipalId = buffer->m_Source;
            results[count]->m_SessionId = buffer->m_Key;
            results[count]->m_Param = buffer->m_Param;
            count++;
        }
    } else {
        for (; it != end && count < resultNum; it++) {
            ReceiveBuffer* buffer = *it;
            if (buffer->m_Param >> 16 != 0) {
                continue;
            }
            results[count]->m_OwnerPrincipalId = buffer->m_Source;
            results[count]->m_SessionId = buffer->m_Key;
            results[count]->m_Param = buffer->m_Param;
            count++;
        }
    }
    *foundNum = count;
    m_HasNotification = false;
}
} // namespace nex
} // namespace net

namespace net {
namespace nex {
// 0x005155A8 (name is ours)
bool net::nex::Impl::InitializeIndependent(bool logsIn)
{
    bool isLoggedIn = false;
    if (m_pNgsFacade == NULL) {
        if (!m_IsSocketInitialized) {
            if (!InitializeNexGlobals()) {
                return false;
            }
            if (!InitializeSocket()) {
                FinalizeNexGlobals();
                return false;
            }
        }
        if (logsIn) {
            if (!Login(GAME_SERVER_ID, ACCESS_KEY)) {
                FinalizeSocketIfUnused();
                FinalizeNexGlobals();
                return false;
            }
            isLoggedIn = true;
        }
    }
    m_pIndependentServer = new nn::nex::IndependentServer;
    if (m_pIndependentServer != NULL) {
        return true;
    }
    if (isLoggedIn) {
        Logout();
    }
    FinalizeSocketIfUnused();
    FinalizeNexGlobals();
    return false;
}

// 0x005157D0 (name is ours)
void net::nex::Impl::LeaveFriendSession()
{
    if (m_pMatchmakeExtension != NULL && nn::pia::session::Session::s_pInstance != NULL &&
        nn::pia::session::Session::s_pInstance->IsHost()) {
        SendNotification(NOTIFICATION_TYPE_101, m_SessionId, 0x8020000, nn::nex::String(L""));
    }
    LeaveSession();
    m_OwnerPrincipalId = 0;
    m_IsSessionOpen = false;
    m_SessionText[0] = 0;
    m_SessionParam = 0;
}

// 0x00515868 (name is ours)
bool net::nex::Impl::UpdateSessionText(const char16* text, u32 length)
{
    if (m_pMatchmakeExtension == NULL || !m_pMatchmakeExtension->IsConnected() || m_pNgsFacade == NULL) {
        return false;
    }
    if (text == NULL || length == 0 || length >= 256) {
        return false;
    }
    script::WordPtrSv(reinterpret_cast<char16*>(m_SessionText), 256).Copy(script::WordCPtrSv(text, length + 1));
    u32 flags = m_IsSessionOpen ? 0x8010000 : 0x8020000;
    return SendNotification(NOTIFICATION_TYPE_101, m_SessionId, m_SessionParam | flags,
                            nn::nex::String(reinterpret_cast<const wchar_t*>(text)));
}
} // namespace nex
} // namespace net

namespace net {
namespace nex {
// 0x00515A44 (name is ours)
bool net::nex::Impl::Send(nn::pia::StationId stationId, const void* data, u32 size)
{
    if (!m_IsInSession) {
        return false;
    }
    if (m_SendSkipCount != 0) {
        m_SendSkipCount--;
        return false;
    }
    nn::pia::transport::ReliableProtocol* protocol =
        nn::pia::transport::Transport::s_pInstance->m_ProtocolManager.GetProtocol<nn::pia::transport::ReliableProtocol>(
            m_ReliableProtocolId, nn::pia::transport::PROTOCOL_TYPE_RELIABLE);
    if (protocol == NULL || !protocol->IsInCommunication(stationId)) {
        return false;
    }
    nn::Result result = protocol->Send(stationId, data, size);
    if (result.IsSuccess()) {
        m_LastSendSize = size;
        return true;
    }
    if (result == RESULT_SEND_BUFFER_FULL) {
        return false;
    }
    // (the messages are not used)
    char message[128];
    snprintf(message, sizeof(message), "### \xe9\x80\x81\xe4\xbf\xa1\xe3\x81\xab\xe5\xa4\xb1\xe6\x95\x97\xe3\x81\x97\xe3\x81\x9f [%08x]",
             result.GetPrintableBits());
    char name[128];
    if (result == nn::pia::common::RESULT_TIMEOUT) {
        snprintf(name, sizeof(name), "nn::pia::ResultTimeOut\n");
    } else if (result == RESULT_NOT_INITIALIZED) {
        snprintf(name, sizeof(name), "nn::pia::ResultNotInitialized\n");
    } else if (result == RESULT_NOT_IMPLEMENTED) {
        snprintf(name, sizeof(name), "nn::pia::ResultNotImplemented\n");
    } else if (result == RESULT_NOT_FOUND) {
        snprintf(name, sizeof(name), "nn::pia::ResultNotFound\n");
    } else if (result == nn::pia::common::RESULT_NO_DATA) {
        snprintf(name, sizeof(name), "nn::pia::ResultNoData\n");
    } else if (result == nn::pia::common::RESULT_INVALID_STATE) {
        snprintf(name, sizeof(name), "nn::pia::ResultInvalidState\n");
    } else if (result == nn::pia::common::RESULT_INVALID_ARGUMENT) {
        snprintf(name, sizeof(name), "nn::pia::ResultInvalidArgument\n");
    } else if (result == nn::pia::common::RESULT_NOT_IN_COMMUNICATION) {
        snprintf(name, sizeof(name), "nn::pia::ResultNotInCommunication\n");
    } else if (result == RESULT_CANCELLED) {
        snprintf(name, sizeof(name), "nn::pia::ResultCancelled\n");
    } else if (result == RESULT_BROKEN_DATA) {
        snprintf(name, sizeof(name), "nn::pia::ResultBrokenData\n");
    } else if (result == RESULT_BUFFER_SHORTAGE) {
        snprintf(name, sizeof(name), "nn::pia::ResultBufferShortage\n");
    } else if (result == RESULT_ALREADY_INITIALIZED) {
        snprintf(name, sizeof(name), "nn::pia::ResultAlreadyInitialized\n");
    } else if (result == RESULT_ALLOCATION_FAILED) {
        snprintf(name, sizeof(name), "nn::pia::ResultAllocationFailed\n");
    } else {
        snprintf(name, sizeof(name), "???\n");
    }
    return false;
}
} // namespace nex
} // namespace net

namespace net {
namespace nex {
// 0x00516364 (name is ours)
void net::nex::Impl::Process()
{
    CheckConnection();
    Update();
    m_CriticalSection.lock();
    if (nn::pia::common::CachedPrint::s_pInstance != NULL) {
        nn::pia::common::CachedPrint::s_pInstance->Flush();
    }
    m_CriticalSection.unlock();
}

// 0x005167E4 (name is ours)
bool net::nex::Impl::RateObject(u64 dataId, s32 rating0, s32 rating1)
{
    if (m_pDataStore == NULL || !m_pDataStore->IsConnected()) {
        u32 errorCode = 0;
        if (!AcConnection::IsConnected()) {
            nn::ac::CTR::GetLastErrorCode(&errorCode);
        }
        if (m_ErrorCallback != NULL) {
            m_ErrorCallback(2, errorCode);
        }
        return false;
    }
    bool isSuccess = true;
    if (rating0 != 0 && !RateObjectSlot(dataId, 0, rating0)) {
        isSuccess = false;
    }
    if (rating1 != 0 && !RateObjectSlot(dataId, 1, rating1)) {
        isSuccess = false;
    }
    return isSuccess;
}

// 0x005168B8 (name is ours)
bool net::nex::Impl::RateObjectSlot(u64 dataId, s8 slot, s32 ratingValue)
{
    nn::nex::DataStoreRatingTarget target;
    target.m_DataId = dataId;
    target.m_Slot = slot;
    nn::nex::DataStoreRateObjectParam param;
    param.m_AccessPassword = 0;
    param.m_RatingValue = ratingValue;
    nn::nex::DataStoreRatingInfo rating;
    nn::nex::ProtocolCallContext context;
    if (!m_pDataStore->RateObject(&context, target, param, &rating)) {
        ReportSystemError(1);
        return false;
    }
    context.Wait(0xFFFFFFFF);
    nn::nex::qResult outcome = context.GetOutcome();
    if (outcome) {
        return true;
    }
    if (outcome == nn::nex::qResult(static_cast<int>(RESULT_DATASTORE_RATING_0))) {
        ReportSystemError(1);
    } else if (outcome == nn::nex::qResult(static_cast<int>(RESULT_DATASTORE_RATING_1))) {
        ReportSystemError(1);
    } else if (outcome == nn::nex::qResult(static_cast<int>(RESULT_DATASTORE_RATING_2))) {
        ReportSystemError(1);
    } else {
        u32 errorCode = nn::nex::ErrorCodeConverter::ConvertToNetworkErrorCode(outcome);
        if (m_ErrorCallback != NULL) {
            m_ErrorCallback(2, errorCode);
        }
    }
    return false;
}
} // namespace nex
} // namespace net

namespace net {
namespace nex {
// 0x0050FE28 (name is ours)
void net::nex::Impl::CheckConnection()
{
    if (nn::nex::Scheduler::GetInstanceInline() != NULL) {
        nn::nex::Scheduler::GetInstanceInline()->DispatchIfSingleThreaded(3, 0);
    }
    if (m_IsAcConnected && !AcConnection::IsConnected()) {
        u32 errorCode;
        nn::ac::CTR::GetLastErrorCode(&errorCode);
        if (m_ErrorCallback != NULL) {
            m_ErrorCallback(2, errorCode);
        }
        if (!nn::nex::Core::GetInstance()->m_IsInterrupted) {
            nn::nex::Core::GetInstance()->SetInterrupted(true);
        }
        m_IsAcConnected = false;
    }
    if (m_IsLoggedIn && m_pNgsFacade != NULL) {
        nn::nex::qResult result = m_pNgsFacade->IsConnected();
        if (!result) {
            if (AcConnection::IsConnected()) {
                u32 errorCode = nn::nex::ErrorCodeConverter::ConvertToNetworkErrorCode(result);
                if (m_ErrorCallback != NULL) {
                    m_ErrorCallback(2, errorCode);
                }
            } else {
                u32 errorCode;
                nn::ac::CTR::GetLastErrorCode(&errorCode);
                if (m_ErrorCallback != NULL) {
                    m_ErrorCallback(2, errorCode);
                }
            }
            if (!nn::nex::Core::GetInstance()->m_IsInterrupted) {
                nn::nex::Core::GetInstance()->SetInterrupted(true);
            }
            m_IsLoggedIn = false;
        }
    }
}
} // namespace nex
} // namespace net

namespace net {
namespace nex {
// 0x00510AD8 (name is ours)
bool net::nex::Impl::DownloadData(u64 dataId, void* buffer, u32 bufferSize, u32* size)
{
    if (size != NULL) {
        *size = 0;
    }
    if (buffer == NULL || bufferSize == 0 || dataId == 0) {
        return false;
    }
    if (m_pDataStore == NULL || !m_pDataStore->IsConnected()) {
        u32 errorCode = 0;
        if (!AcConnection::IsConnected()) {
            nn::ac::CTR::GetLastErrorCode(&errorCode);
        }
        if (m_ErrorCallback != NULL) {
            m_ErrorCallback(2, errorCode);
        }
        return false;
    }
    nn::nex::DataStoreMetaInfo metaInfo;
    if (!GetMeta(dataId, &metaInfo, 0, true) || metaInfo.m_Size > bufferSize || !GetObject(dataId, buffer, bufferSize)) {
        return false;
    }
    if (size != NULL) {
        *size = metaInfo.m_Size;
    }
    return true;
}

// 0x00510F80 (name is ours)
bool net::nex::Impl::GetObject(u64 dataId, void* buffer, u32 bufferSize)
{
    nn::nex::DataStorePrepareGetParam param;
    param.m_DataId = dataId;
    nn::nex::ProtocolCallContext context;
    if (!m_pDataStore->GetObject(&context, param, static_cast<u8*>(buffer), bufferSize, NULL)) {
        ReportSystemError(1);
        return false;
    }
    context.Wait(0xFFFFFFFF);
    if (context.GetOutcome()) {
        return true;
    }
    u32 errorCode = nn::nex::ErrorCodeConverter::ConvertToNetworkErrorCode(context.GetOutcome());
    if (m_ErrorCallback != NULL) {
        m_ErrorCallback(1, errorCode);
    }
    return false;
}
} // namespace nex
} // namespace net

namespace net {
namespace nex {
// 0x00511448 (name is ours)
bool net::nex::Impl::CallMatchmakeMethod25(const u32* principalId, const wchar_t* text)
{
    if (m_pMatchmakeExtension == NULL || !m_pMatchmakeExtension->IsConnected() || m_pNgsFacade == NULL || *principalId == 0) {
        return false;
    }
    nn::nex::qList<u32> principalIds;
    principalIds.push_back(*principalId);
    nn::nex::ProtocolCallContext context;
    if (!m_pMatchmakeExtension->Method25(&context, principalIds)) {
        ReportSystemError(3);
        return false;
    }
    context.Wait(0xFFFFFFFF);
    if (!context.GetOutcome()) {
        u32 errorCode = nn::nex::ErrorCodeConverter::ConvertToNetworkErrorCode(context.GetOutcome());
        if (m_ErrorCallback != NULL) {
            m_ErrorCallback(3, errorCode);
        }
        return false;
    }
    context.Reset();
    if (m_pMatchmakeExtension->Method28(&context, *principalId, nn::nex::String(text), 0)) {
        context.Wait(0xFFFFFFFF);
        context.GetOutcome();
    }
    return true;
}
} // namespace nex
} // namespace net

namespace net {
namespace nex {
// 0x005164C0 (name is ours)
s32 net::nex::Impl::ResetPermission(u64 dataId, u16 dataType, TagFunction tagFunction)
{
    if (m_pDataStore == NULL || !m_pDataStore->IsConnected()) {
        u32 errorCode = 0;
        if (!AcConnection::IsConnected()) {
            nn::ac::CTR::GetLastErrorCode(&errorCode);
        }
        if (m_ErrorCallback != NULL) {
            m_ErrorCallback(2, errorCode);
        }
        return static_cast<s32>(0x80000006);
    }
    nn::nex::DataStoreChangeMetaParam param;
    nn::nex::DataStorePermission permission(nn::nex::DataStoreConstants::PERMISSION_DEFAULT, nn::nex::qVector<u32>());
    permission.m_Permission = nn::nex::DataStoreConstants::PERMISSION_DEFAULT;
    param.m_Permission = permission;
    param.m_ModifiesFlag = 2;
    param.m_DataId = dataId;
    nn::nex::ProtocolCallContext context;
    if (!m_pDataStore->ChangeMeta(&context, param)) {
        ReportSystemError(2);
        return static_cast<s32>(0x80000006);
    }
    context.Wait(0xFFFFFFFF);
    nn::nex::qResult outcome = context.GetOutcome();
    if (outcome) {
        return 0;
    }
    if (outcome == nn::nex::qResult(static_cast<int>(RESULT_DATASTORE_RATING_0)) ||
        outcome == nn::nex::qResult(static_cast<int>(RESULT_DATASTORE_RATING_1))) {
        return static_cast<s32>(0x80000004);
    }
    if (outcome == nn::nex::qResult(static_cast<int>(RESULT_DATASTORE_RATING_2))) {
        nn::nex::DataStoreMetaInfo metaInfo;
        if ((dataType == 0xFFFF && tagFunction == NULL) || GetMetaChecked(dataId, dataType, tagFunction, &metaInfo)) {
            return static_cast<s32>(0x80000005);
        }
        return static_cast<s32>(0x80000004);
    }
    u32 errorCode = nn::nex::ErrorCodeConverter::ConvertToNetworkErrorCode(outcome);
    if (m_ErrorCallback != NULL) {
        m_ErrorCallback(2, errorCode);
    }
    return static_cast<s32>(0x80000006);
}
} // namespace nex
} // namespace net

namespace net {
namespace nex {
// 0x0050EFC8 (name is ours)
bool net::nex::Impl::UploadData(const void* data, u32 dataSize, u64* dataId, const UploadInfo* info, const void* metaBinary,
                                u32 metaBinarySize)
{
    if (m_pDataStore == NULL || !m_pDataStore->IsConnected()) {
        u32 errorCode = 0;
        if (!AcConnection::IsConnected()) {
            nn::ac::CTR::GetLastErrorCode(&errorCode);
        }
        if (m_ErrorCallback != NULL) {
            m_ErrorCallback(2, errorCode);
        }
        return false;
    }
    if (dataId != NULL && *dataId != 0) {
        nn::nex::DataStoreMetaInfo metaInfo;
        if (GetMeta(*dataId, &metaInfo, 1, false) && nn::friends::CTR::detail::GetMyPrincipalId() == metaInfo.m_OwnerId) {
            TagList oldTags(metaInfo.m_Tags);
            UpdateResult result = UpdateObject(*dataId, data, dataSize, metaBinary, metaBinarySize, info, &oldTags);
            if (result == UPDATE_RESULT_SUCCESS) {
                if (info != NULL) {
                    *info->m_pReferredCount = metaInfo.m_ReferredCnt;
                }
                return true;
            }
            if (result == UPDATE_RESULT_FAILURE) {
                return false;
            }
        }
    }
    u32 newDataId = PostObject(data, dataSize, metaBinary, metaBinarySize, info);
    if (newDataId == 0) {
        return false;
    }
    if (dataId != NULL) {
        *dataId = newDataId;
    }
    return true;
}

// 0x0050F278 (name is ours)
bool net::nex::Impl::UploadMetaBinary(u64* dataId, const UploadInfo* info, const void* metaBinary, u32 metaBinarySize)
{
    if (m_pDataStore == NULL || !m_pDataStore->IsConnected()) {
        u32 errorCode = 0;
        if (!AcConnection::IsConnected()) {
            nn::ac::CTR::GetLastErrorCode(&errorCode);
        }
        if (m_ErrorCallback != NULL) {
            m_ErrorCallback(2, errorCode);
        }
        return false;
    }
    if (dataId != NULL && *dataId != 0) {
        UpdateResult result = UpdateObject(*dataId, NULL, 0, metaBinary, metaBinarySize, info, NULL);
        if (result == UPDATE_RESULT_SUCCESS) {
            return true;
        }
        if (result == UPDATE_RESULT_FAILURE) {
            return false;
        }
    }
    u32 newDataId = PostObject(NULL, 0, metaBinary, metaBinarySize, info);
    if (newDataId == 0) {
        return false;
    }
    if (dataId != NULL) {
        *dataId = newDataId;
    }
    return true;
}

// 0x0050F8A4 (name is ours)
net::nex::Impl::UpdateResult net::nex::Impl::UpdateObject(u64 dataId, const void* data, u32 dataSize, const void* metaBinary,
                                                           u32 metaBinarySize, const UploadInfo* info, const TagList* oldTags)
{
    nn::nex::ProtocolCallContext updateContext;
    if (data != NULL && dataSize != 0) {
        nn::nex::DataStorePrepareUpdateParam updateParam;
        updateParam.m_DataId = dataId;
        updateParam.m_Size = dataSize;
        if (!m_pDataStore->UpdateObject(&updateContext, updateParam, static_cast<const u8*>(data), false)) {
            ReportSystemError(1);
            return UPDATE_RESULT_FAILURE;
        }
        updateContext.Wait(0xFFFFFFFF);
        nn::nex::qResult outcome = updateContext.GetOutcome();
        if (!outcome) {
            // (the object is gone: it is posted again)
            if (outcome == nn::nex::qResult(static_cast<int>(RESULT_DATASTORE_RATING_0)) ||
                outcome == nn::nex::qResult(static_cast<int>(RESULT_DATASTORE_RATING_1))) {
                return UPDATE_RESULT_NOT_FOUND;
            }
            nn::nex::qResult(static_cast<int>(RESULT_DATASTORE_RATING_2));
            return UPDATE_RESULT_NOT_FOUND;
        }
    }
    nn::nex::DataStoreChangeMetaParam param;
    u32 modifiesFlag = 0;
    if (info != NULL) {
        if (info->GetName() != NULL) {
            param.m_Name = nn::nex::String(info->GetName());
            modifiesFlag = 1;
        }
        if (info->m_TagFunction != NULL) {
            TagList tags;
            TagWriter writer;
            writer.m_pTags = &tags;
            writer.m_pSourceTags = oldTags;
            info->m_TagFunction(&writer);
            param.m_Tags = tags;
            modifiesFlag |= 0x20;
        }
    }
    if (metaBinary != NULL && metaBinarySize != 0) {
        param.m_MetaBinary.clear();
        param.m_MetaBinary.push_back(static_cast<const u8*>(metaBinary), metaBinarySize);
        modifiesFlag |= 0x10;
    }
    {
        nn::nex::DataStorePermission permission(nn::nex::DataStoreConstants::PERMISSION_DEFAULT, nn::nex::qVector<u32>());
        permission.m_Permission = 0;
        param.m_Permission = permission;
    }
    param.m_DataId = dataId;
    param.m_ModifiesFlag = modifiesFlag | 2;
    nn::nex::ProtocolCallContext context;
    if (!m_pDataStore->ChangeMeta(&context, param)) {
        ReportSystemError(1);
        return UPDATE_RESULT_FAILURE;
    }
    context.Wait(0xFFFFFFFF);
    if (context.GetOutcome()) {
        return UPDATE_RESULT_SUCCESS;
    }
    u32 errorCode = nn::nex::ErrorCodeConverter::ConvertToNetworkErrorCode(context.GetOutcome());
    if (m_ErrorCallback != NULL) {
        m_ErrorCallback(1, errorCode);
    }
    return UPDATE_RESULT_FAILURE;
}

// 0x0050FF7C (name is ours)
u32 net::nex::Impl::PostObject(const void* data, u32 dataSize, const void* metaBinary, u32 metaBinarySize, const UploadInfo* info)
{
    bool hasRecipient = info->m_RecipientId != 0;
    nn::nex::DataStorePreparePostParam param;
    param.m_Size = data != NULL && dataSize != 0 ? dataSize : 0;
    if (info != NULL && info->GetName() != NULL) {
        param.m_Name = nn::nex::String(info->GetName());
    } else {
        param.m_Name = nn::nex::String("unknown@JohnDoe");
    }
    if (info != NULL && info->m_DataType != 0xFFFF) {
        param.m_DataType = info->m_DataType;
    }
    {
        nn::nex::DataStorePermission permission(nn::nex::DataStoreConstants::PERMISSION_DEFAULT, nn::nex::qVector<u32>());
        if (hasRecipient) {
            permission.m_Permission = 2;
            nn::nex::qVector<u32> recipientIds;
            recipientIds.push_back(info->m_RecipientId);
            permission.m_RecipientIds = recipientIds;
        } else {
            permission.m_Permission = 0;
        }
        param.m_Permission = permission;
    }
    {
        nn::nex::DataStorePermission permission(nn::nex::DataStoreConstants::PERMISSION_DEFAULT, nn::nex::qVector<u32>());
        if (hasRecipient) {
            permission.m_Permission = 2;
            nn::nex::qVector<u32> recipientIds;
            recipientIds.push_back(info->m_RecipientId);
            permission.m_RecipientIds = recipientIds;
        } else {
            permission.m_Permission = 0;
        }
        param.m_DelPermission = permission;
    }
    param.m_Flag = hasRecipient ? 8 : 2;
    param.m_Period = hasRecipient ? 30 : 365;
    if (info != NULL && info->m_TagFunction != NULL) {
        TagList tags;
        TagWriter writer;
        writer.m_pTags = &tags;
        writer.m_pSourceTags = NULL;
        info->m_TagFunction(&writer);
        param.m_Tags = tags;
    }
    if (!hasRecipient) {
        nn::nex::qMap<s8, nn::nex::DataStoreRatingInitParam> ratings;
        for (s8 slot = 0; slot < 2; slot++) {
            nn::nex::DataStoreRatingInitParam rating;
            rating.m_Flag = 16;
            rating.m_InternalFlag = 0;
            rating.m_LockType = 0;
            rating.m_InitialValue = 0;
            rating.m_RangeMin = 0;
            rating.m_RangeMax = 0;
            rating.m_PeriodHour = 0;
            rating.m_PeriodDuration = 0;
            ratings.insert(std::pair<const s8, nn::nex::DataStoreRatingInitParam>(slot, rating));
        }
        param.SetRatingSetting(ratings);
    }
    if (metaBinary != NULL && metaBinarySize != 0) {
        if (metaBinarySize > 1024) {
            metaBinarySize = 1024;
        }
        param.m_MetaBinary.clear();
        param.m_MetaBinary.push_back(static_cast<const u8*>(metaBinary), metaBinarySize);
    }
    nn::nex::ProtocolCallContext context;
    u64 newDataId;
    if (data != NULL && dataSize != 0) {
        if (!m_pDataStore->PostObject(&context, param, &newDataId, static_cast<const u8*>(data), false)) {
            ReportSystemError(1);
            return 0;
        }
    } else if (metaBinary != NULL && metaBinarySize != 0) {
        if (!m_pDataStore->PostMetaBinaryDirect(&context, param, &newDataId)) {
            ReportSystemError(1);
            return 0;
        }
    } else {
        return 0;
    }
    context.Wait(0xFFFFFFFF);
    if (context.GetOutcome()) {
        return static_cast<u32>(newDataId);
    }
    u32 errorCode = nn::nex::ErrorCodeConverter::ConvertToNetworkErrorCode(context.GetOutcome());
    if (m_ErrorCallback != NULL) {
        m_ErrorCallback(1, errorCode);
    }
    return 0;
}
} // namespace nex
} // namespace net

namespace net {
namespace nex {
// 0x00747D88 | libgarden (name)
u8 net::nex::Impl::GetInfo(ParticipantInfo* out, size_t count) const
{
    u8 num = 0;
    if (nn::pia::session::Session::s_pInstance != NULL && m_IsInSession) {
        nn::pia::transport::StationManager* manager = nn::pia::transport::StationManager::s_pInstance;
        nn::pia::transport::Station* const* it = manager->m_ActiveStations.Begin();
        while (count != 0) {
            if (it == manager->m_ActiveStations.End()) {
                break;
            }
            nn::pia::transport::Station* station = *it;
            if (station->m_State == 5 || station->m_StationAddress == manager->m_pLocalStation->m_StationAddress) {
                nn::pia::StationId stationId = station->m_StationId;
                if (station->m_StationAddress == manager->m_pLocalStation->m_StationAddress) {
                    out->stationId = stationId;
                    out->rtt = 0;
                } else {
                    s32 rtt = station->GetRtt();
                    out->rtt = rtt == -1 ? 250 : rtt;
                    out->stationId = stationId;
                }
                out++;
                count--;
                num++;
            }
            it++;
        }
    }
    for (; static_cast<s32>(count) > 0; count--, out++) {
        out->rtt = 0;
        out->stationId = nn::pia::StationId(0xFFFFFFFD, 0xFFFFFFFF);
    }
    return num;
}
} // namespace nex
} // namespace net

namespace net {
namespace nex {
// 0x0013C598 (name is ours)
net::nex::Impl::~Impl()
{
    DestroyNexHeap();
    s_pInstance = NULL;
}

// 0x0046FEC8 (name is ours)
void net::nex::Impl::FinalizeDataStore()
{
    if (m_pDataStore != NULL) {
        m_pDataStore->Unbind();
        if (m_pDataStore != NULL) {
            delete m_pDataStore;
        }
        m_pDataStore = NULL;
    }
    nn::http::Finalize();
}
} // namespace nex
} // namespace net
