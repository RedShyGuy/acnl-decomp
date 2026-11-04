#include "net/nex/dFramework.h"

#include "sead/seadSafeStringBase.h"

namespace net {
namespace nex {
// 0x003CE9D0 (name is ours)
void net::nex::Framework::ThreadFunc(sead::Thread* thread, s32 message)
{
    if (message == 1) {
        m_pSystem->SetCommand(&m_Command);
    }
    m_pSystem->Update();
    m_Section.Enter();
    m_pSystem->GetMatchInfo(&m_MatchInfoThread, &m_ErrorThread);
    if (m_Command.processState == 1 && m_Command.status != Command::NOT_DONE) {
        m_Command.processState = 2;
    }
    m_Section.Exit();
    m_pPacer->Pace();
}

// 0x00518A68 (name is ours)
bool net::nex::Framework::Initialize(Sender* sender, SystemCallback* callback)
{
    if (m_IsBlocked) {
        return false;
    }
    if (m_State != UNINITIALIZED) {
        return true;
    }
    m_pSystem = new (g_pNetHeap, 4) System(sender, callback);
    if (!m_pSystem->Start()) {
        return false;
    }
    Reset();
    m_State = READY;
    CreateThread();
    return true;
}

// 0x00518AF0 (name is ours)
bool net::nex::Framework::RateObject(u64 dataId, s8 rating0, s8 rating1)
{
    if (!(m_ConnectFlags & CONNECT_FLAG_DATASTORE) || dataId == 0) {
        m_Error.Set(Command::RATE_OBJECT);
        return false;
    }
    Command::Param param;
    param.rate.dataId = dataId;
    param.rate.rating0 = rating0;
    param.rate.rating1 = rating1;
    return ProcessCommand(Command::RATE_OBJECT, &param);
}

// 0x00518B78 (name is ours)
bool net::nex::Framework::SearchSessions(ScanBufBase** results, u32 resultNum, u32* foundNum, s8 withData, u8 mode)
{
    if (m_State != LOGGED_IN || !(m_ConnectFlags & CONNECT_FLAG_SESSION)) {
        m_Error.Set(Command::SEARCH_SESSIONS);
        return false;
    }
    Command::Param param;
    param.searchSessions.results = results;
    param.searchSessions.resultNum = resultNum;
    param.searchSessions.foundNum = foundNum;
    param.searchSessions.mode = mode;
    param.searchSessions.withData = withData;
    return ProcessCommand(Command::SEARCH_SESSIONS, &param);
}

// 0x00518BFC (name is ours)
void net::nex::Framework::CreateThread()
{
    sead::Heap* heap = g_pNetHeap;
    if (m_pPacer == NULL) {
        m_pPacer = ::new (heap, 4) nn::nex::Pacer(30.0f);
    }
    if (m_pCommandThread == NULL) {
        m_pCommandThread = new (heap, 4)
            sead::DelegateThread(sead::SafeStringBase<char>("NetCommandThread"), &m_Delegate, heap, 12,
                                 sead::MessageQueue::BLOCK_TYPE_NO_BLOCK, 0x7FFFFFFF, 0x8000, 2);
        m_pCommandThread->start();
    }
}

// 0x00518CB4 (name is ours)
bool net::nex::Framework::DisconnectIndependent()
{
    if (!(m_ConnectFlags & CONNECT_FLAG_INDEPENDENT) && m_Command.id == Command::NONE) {
        return true;
    }
    bool isDone = ProcessCommand(Command::DISCONNECT_INDEPENDENT);
    if (isDone) {
        m_ConnectFlags &= ~CONNECT_FLAG_INDEPENDENT;
        if (m_ConnectFlags == 0) {
            m_State = READY;
            Reset();
        }
    }
    return isDone;
}

// 0x00518D14 (name is ours)
bool net::nex::Framework::CreateSession(const char16* text, u32 length, u16 param, u8 mode)
{
    if (m_State == STATE_3) {
        return true;
    }
    if (m_State != LOGGED_IN || !(m_ConnectFlags & CONNECT_FLAG_SESSION)) {
        m_Error.Set(Command::CREATE_SESSION);
        return false;
    }
    if (m_Command.id == Command::NONE && length >= 92) {
        return true;
    }
    Command::Param commandParam;
    commandParam.createSession.text = text;
    commandParam.createSession.length = length;
    commandParam.createSession.param = param;
    commandParam.createSession.mode = mode;
    bool isDone = ProcessCommand(Command::CREATE_SESSION, &commandParam);
    if (isDone) {
        m_State = STATE_3;
    }
    return isDone;
}

// 0x00518DD8 (name is ours)
bool net::nex::Framework::SearchData(SearchDataResultList* results, const u16* dataTypes, u32 dataTypeNum,
                                     TagFunction tagFunction, u8 order)
{
    if (!(m_ConnectFlags & CONNECT_FLAG_DATASTORE)) {
        m_Error.Set(Command::SEARCH_DATA);
        return false;
    }
    Command::Param param;
    param.searchData.dataId = 0;
    param.searchData.results = results;
    param.searchData.tagFunction = tagFunction;
    param.searchData.order = order;
    if (dataTypeNum > 8) {
        dataTypeNum = 8;
    }
    u32 i;
    for (i = 0; i < dataTypeNum; i++) {
        param.searchData.dataTypes[i] = dataTypes[i];
    }
    for (; i < 8; i++) {
        param.searchData.dataTypes[i] = 0xFFFF;
    }
    return ProcessCommand(Command::SEARCH_DATA, &param);
}

// 0x00518ED0 (name is ours)
bool net::nex::Framework::GetData(u64 dataId, SearchDataResultList* results, u16 dataType, TagFunction tagFunction)
{
    if (!(m_ConnectFlags & CONNECT_FLAG_DATASTORE) || dataId == 0) {
        m_Error.Set(Command::SEARCH_DATA);
        return false;
    }
    Command::Param param;
    param.searchData.dataId = dataId;
    param.searchData.results = results;
    param.searchData.tagFunction = tagFunction;
    param.searchData.dataTypes[0] = dataType;
    for (u32 i = 1; i < 8; i++) {
        param.searchData.dataTypes[i] = 0xFFFF;
    }
    return ProcessCommand(Command::SEARCH_DATA, &param);
}

// 0x00518F7C (name is ours)
bool net::nex::Framework::ConnectIndependent(u32 value)
{
    if (IsLoggedIn(true)) {
        if (!m_IsInternet) {
            m_Error.Set(Command::CONNECT_INDEPENDENT);
            return false;
        }
        if (m_ConnectFlags & CONNECT_FLAG_INDEPENDENT) {
            return true;
        }
    } else if (m_State != READY) {
        m_Error.Set(Command::CONNECT_INDEPENDENT);
        return false;
    }
    Command::Param param;
    param.value = value;
    bool isDone = ProcessCommand(Command::CONNECT_INDEPENDENT, &param);
    if (!isDone) {
        return isDone;
    }
    m_ConnectFlags |= CONNECT_FLAG_INDEPENDENT;
    if (!IsInternet(false)) {
        m_State = LOGGED_IN;
        m_IsInternet = true;
    }
    return isDone;
}

// 0x0051906C (name is ours)
bool net::nex::Framework::UploadData(const void* data, u32 size, void* work, u64* dataId, const void* info)
{
    if (!(m_ConnectFlags & CONNECT_FLAG_DATASTORE)) {
        m_Error.Set(Command::UPLOAD_DATA);
        return false;
    }
    Command::Param param;
    param.upload.data = data;
    param.upload.size = size;
    param.upload.work = work;
    param.upload.dataId = dataId;
    param.upload.info = info;
    return ProcessCommand(Command::UPLOAD_DATA, &param);
}

// 0x005190DC (name is ours)
bool net::nex::Framework::CallMatchmakeMethod25(const u32* value, const wchar_t* text)
{
    if (!IsInternet(false) && m_Command.id == Command::NONE) {
        return true;
    }
    Command::Param param;
    param.text.param1 = *value;
    param.text.text = text;
    return ProcessCommand(Command::MATCHMAKE_METHOD_25, &param);
}

// 0x00519164 (name is ours)
bool net::nex::Framework::DisconnectDataStore()
{
    if (!(m_ConnectFlags & CONNECT_FLAG_DATASTORE) && m_Command.id == Command::NONE) {
        return true;
    }
    bool isDone = ProcessCommand(Command::DISCONNECT_DATASTORE);
    if (isDone) {
        m_ConnectFlags &= ~CONNECT_FLAG_DATASTORE;
        if (m_ConnectFlags == 0) {
            m_State = READY;
            Reset();
        }
    }
    return isDone;
}

// 0x005191C4 (name is ours)
bool net::nex::Framework::ConnectDataStore(u32 value)
{
    if (IsLoggedIn(false)) {
        if (!m_IsInternet) {
            m_Error.Set(Command::CONNECT_DATASTORE);
            return false;
        }
        if (m_ConnectFlags & CONNECT_FLAG_DATASTORE) {
            return true;
        }
    } else if (m_State != READY) {
        m_Error.Set(Command::CONNECT_DATASTORE);
        return false;
    }
    Command::Param param;
    param.value = value;
    bool isDone = ProcessCommand(Command::CONNECT_DATASTORE, &param);
    if (!isDone) {
        return isDone;
    }
    m_ConnectFlags |= CONNECT_FLAG_DATASTORE;
    if (!IsInternet(false)) {
        m_State = LOGGED_IN;
        m_IsInternet = true;
    }
    return isDone;
}

// 0x005192C4 (name is ours)
bool net::nex::Framework::UploadData2(const void* data, u32 size, void* work, const void* info)
{
    if (!(m_ConnectFlags & CONNECT_FLAG_DATASTORE)) {
        m_Error.Set(Command::UPLOAD_DATA_2);
        return false;
    }
    Command::Param param;
    param.upload.data = data;
    param.upload.size = size;
    param.upload.work = work;
    param.upload.info = info;
    return ProcessCommand(Command::UPLOAD_DATA_2, &param);
}

// 0x00519330 (name is ours)
bool net::nex::Framework::DownloadData(u64 dataId, void* buffer, u32 size, void* work, u32* length)
{
    if (!(m_ConnectFlags & CONNECT_FLAG_DATASTORE)) {
        m_Error.Set(Command::DOWNLOAD_DATA);
        return false;
    }
    Command::Param param;
    param.download.dataId = dataId;
    param.download.buffer = buffer;
    param.download.size = size;
    param.download.work = work;
    param.download.length = length;
    return ProcessCommand(Command::DOWNLOAD_DATA, &param);
}

// 0x005193AC (name is ours)
bool net::nex::Framework::JoinSessionByOwner(const u32* principalId)
{
    if (m_State != LOGGED_IN || !(m_ConnectFlags & CONNECT_FLAG_SESSION)) {
        m_Error.Set(Command::JOIN_SESSION_BY_OWNER);
        return false;
    }
    Command::Param param;
    param.value = *principalId;
    bool isDone = ProcessCommand(Command::JOIN_SESSION_BY_OWNER, &param);
    if (isDone) {
        m_State = STATE_5;
    }
    return isDone;
}

// 0x00519444 | libgarden
bool net::nex::Framework::CreateOrJoinRandomSession(u32 value)
{
    if (m_State != LOGGED_IN || !(m_ConnectFlags & CONNECT_FLAG_SESSION)) {
        m_Error.Set(Command::JOIN_RANDOM_SESSION);
        return false;
    }
    Command::Param param;
    param.value = value;
    bool isDone = ProcessCommand(Command::JOIN_RANDOM_SESSION, &param);
    if (isDone) {
        m_State = STATE_5;
    }
    return isDone;
}

// 0x005194D4 (name is ours)
bool net::nex::Framework::CreateRandomSession(u32 value)
{
    if (m_State != LOGGED_IN || !(m_ConnectFlags & CONNECT_FLAG_SESSION)) {
        m_Error.Set(Command::CREATE_RANDOM_SESSION);
        return false;
    }
    Command::Param param;
    param.value = value;
    bool isDone = ProcessCommand(Command::CREATE_RANDOM_SESSION, &param);
    if (isDone) {
        m_State = STATE_5;
    }
    return isDone;
}

// 0x00519564 (name is ours)
bool net::nex::Framework::ResetPermission(u64 dataId, u16 dataType, TagFunction tagFunction)
{
    if (!(m_ConnectFlags & CONNECT_FLAG_DATASTORE)) {
        m_Error.Set(Command::RESET_PERMISSION);
        return false;
    }
    if (dataId == 0) {
        // (an error, but the command counts as done)
        m_Error.Set(Command::RESET_PERMISSION);
        return true;
    }
    Command::Param param;
    param.permission.dataId = dataId;
    param.permission.dataType = dataType;
    param.permission.tagFunction = tagFunction;
    return ProcessCommand(Command::RESET_PERMISSION, &param);
}

// 0x00519608 (name is ours)
bool net::nex::Framework::SetSessionData(const char16* data, u32 length)
{
    if (m_Command.id == Command::NONE && (!IsHost() || length >= 92)) {
        return true;
    }
    Command::Param param;
    param.sessionData.data = data;
    param.sessionData.length = length;
    return ProcessCommand(Command::SET_SESSION_DATA, &param);
}

// 0x005196A0 (name is ours)
void net::nex::Framework::GetFriendCodeDirect(const nn::pia::StationId* stationId, u64* friendCode)
{
    if (m_pSystem != NULL) {
        m_pSystem->m_Impl.GetFriendCode(*stationId, friendCode);
        return;
    }
    *friendCode = 0;
}

// 0x005196D0 (name is ours)
bool net::nex::Framework::SendNotification102(const wchar_t* text)
{
    if (!(IsInternet(false) && (m_ConnectFlags & CONNECT_FLAG_NEX)) && m_Command.id == Command::NONE) {
        return true;
    }
    Command::Param param;
    param.text.text = text;
    return ProcessCommand(Command::SEND_NOTIFICATION_102, &param);
}

// 0x0051975C (name is ours)
bool net::nex::Framework::SendTextMessage(const u32* principalId, const wchar_t* text)
{
    if (!(IsInternet(false) && (m_ConnectFlags & CONNECT_FLAG_NEX)) && m_Command.id == Command::NONE) {
        return true;
    }
    Command::Param param;
    param.text.param1 = *principalId;
    param.text.text = text;
    return ProcessCommand(Command::SEND_TEXT_MESSAGE, &param);
}

// 0x005197F0 (name is ours)
bool net::nex::Framework::CheckCommand()
{
    if (m_Command.id == Command::NONE) {
        return true;
    }
    if (m_Command.processState != 0) {
        return false;
    }
    if (m_Command.status == Command::DONE) {
        switch (m_Command.id) {
        case Command::CREATE_SESSION:
            m_State = STATE_3;
            break;
        case Command::JOIN_SESSION:
            m_State = STATE_4;
            break;
        case Command::JOIN_RANDOM_SESSION:
        case Command::CREATE_RANDOM_SESSION:
        case Command::JOIN_SESSION_BY_OWNER:
            m_State = STATE_5;
            break;
        default:
            break;
        }
    }
    m_Command.id = Command::NONE;
    return true;
}

// 0x00519870 (name is ours)
void net::nex::Framework::SetPlayerName(const char16* name, u32 length)
{
    if (m_pSystem != NULL) {
        m_pSystem->m_Impl.SetPlayerName(name, length);
    }
}

// 0x00519888 (name is ours)
bool net::nex::Framework::GetFriendCode(const nn::pia::StationId* stationId, u64* friendCode)
{
    if (IsLocal() && m_State > LOGGED_IN) {
        Command::Param param;
        param.friendCode.stationId = *stationId;
        param.friendCode.friendCode = friendCode;
        return ProcessCommand(Command::GET_FRIEND_CODE, &param);
    }
    *friendCode = 0;
    return true;
}

// 0x00519928 (name is ours)
bool net::nex::Framework::SendNotification103(u32 param1, u32 param2, const wchar_t* text)
{
    if (!IsInternet(false) && m_Command.id == Command::NONE) {
        return true;
    }
    Command::Param param;
    param.notification103.param1 = param1;
    param.notification103.param2 = param2;
    param.notification103.text = text;
    return ProcessCommand(Command::SEND_NOTIFICATION_103, &param);
}

// 0x005199A0 (name is ours)
bool net::nex::Framework::OpenSession(s8 setParam, u16 value)
{
    if (!IsHost() && m_Command.id == Command::NONE) {
        return true;
    }
    Command::Param param;
    param.openSession.param = value;
    param.openSession.setParam = setParam;
    return ProcessCommand(Command::OPEN_SESSION, &param);
}

// 0x00519A2C (name is ours)
bool net::nex::Framework::LeaveSession()
{
    if (!(!m_IsBlocked && m_State > LOGGED_IN) && m_Command.id == Command::NONE) {
        return true;
    }
    bool isDone;
    if (m_State == STATE_3) {
        isDone = ProcessCommand(Command::LEAVE_SESSION_14);
    } else {
        isDone = ProcessCommand(Command::LEAVE_SESSION);
    }
    if (isDone) {
        m_State = LOGGED_IN;
    }
    return isDone;
}

// 0x00519AA0 (name is ours)
bool net::nex::Framework::CloseSession()
{
    if (!IsHost() && m_Command.id == Command::NONE) {
        return true;
    }
    return ProcessCommand(Command::CLOSE_SESSION);
}

// 0x00519AF4 (name is ours)
bool net::nex::Framework::Disconnect()
{
    if (!(m_ConnectFlags & CONNECT_FLAG_SESSION) && m_Command.id == Command::NONE) {
        return true;
    }
    bool isDone = ProcessCommand(Command::DISCONNECT);
    if (isDone) {
        if (m_IsInternet) {
            m_ConnectFlags &= ~CONNECT_FLAG_NEX;
        }
        m_ConnectFlags &= ~CONNECT_FLAG_SESSION;
        if (m_ConnectFlags == 0) {
            m_State = READY;
            Reset();
        }
    }
    return isDone;
}

// 0x00519B64 (name is ours)
bool net::nex::Framework::JoinSession(u32 sessionId)
{
    if (m_State != LOGGED_IN || !(m_ConnectFlags & CONNECT_FLAG_SESSION)) {
        m_Error.Set(Command::JOIN_SESSION);
        return false;
    }
    Command::Param param;
    param.value = sessionId;
    bool isDone = ProcessCommand(Command::JOIN_SESSION, &param);
    if (isDone) {
        m_State = STATE_4;
    }
    return isDone;
}

// 0x00519BF4 (name is ours, after libgarden's Net::Connect)
bool net::nex::Framework::Connect(bool isLocal)
{
    if (IsLoggedIn(false)) {
        if (m_IsInternet == isLocal) {
            m_Error.Set(isLocal ? Command::CONNECT_LOCAL : Command::CONNECT_NEX);
            return false;
        }
        return (m_ConnectFlags & CONNECT_FLAG_SESSION) && m_Command.id == Command::NONE;
    }
    if (m_State != READY) {
        m_Error.Set(isLocal ? Command::CONNECT_LOCAL : Command::CONNECT_NEX);
        return false;
    }
    bool isDone;
    if (isLocal) {
        isDone = ProcessCommand(Command::CONNECT_LOCAL);
    } else {
        isDone = ProcessCommand(Command::CONNECT_NEX);
    }
    if (!isDone) {
        return isDone;
    }
    m_ConnectFlags |= CONNECT_FLAG_SESSION;
    if (!isLocal) {
        m_ConnectFlags |= CONNECT_FLAG_NEX;
        m_IsInternet = true;
    }
    if (!IsLoggedIn(false)) {
        m_State = LOGGED_IN;
    }
    return isDone;
}

// 0x00519D08 (name is ours)
bool net::nex::Framework::ModifySessionAttribute(const nn::pia::StationId* stationId)
{
    if (m_Command.id == Command::NONE && !(m_State == STATE_5 && *stationId == m_MatchInfo.localStation)) {
        return true;
    }
    return ProcessCommand(Command::MODIFY_SESSION_ATTRIBUTE);
}

// 0x00519D54 (name is ours)
net::nex::Framework::Framework()
    : m_Delegate(this, &Framework::ThreadFunc), m_Section(nn::os::CriticalSection::InitializeTag()), m_pSystem(NULL),
      m_pCommandThread(NULL), m_pPacer(NULL), m_State(UNINITIALIZED), m_ConnectFlags(0), m_IsInternet(false),
      m_IsBlocked(false)
{
}

// 0x00519E64 (name is ours)
net::nex::Framework::~Framework()
{
    Finalize();
}
} // namespace nex
} // namespace net
