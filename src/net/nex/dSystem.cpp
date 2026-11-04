#include "net/nex/dSystem.h"

#include "net/dSystemCallback.h"
#include "nn/crypto/crypto_Api.h"

namespace net {
namespace nex {
namespace {
// the keys of the encryption of the uploaded data
const char* const UPLOAD_KEY = "gvskivgchsuj7ifc";
const char* const UPLOAD_KEY_2 = "smzglqryoieeblrd";
} // namespace

// 0x00954678
u32 net::nex::System::s_FrameCount;
// 0x0095467C
System* net::nex::System::s_pInstance;

// 0x0087CDBC
const System::StateFunction net::nex::System::s_StateFunctions[STATE_NUM] = {
    &System::StateNone,       &System::StateIdle,        &System::StateConnectedLocal, &System::StateLocalSession,
    &System::StateLocalError, &System::StateIndependent, &System::StateConnectedNex,   &System::StateNexSession,
};

// 0x00516C24 (name is ours)
bool net::nex::System::Start()
{
    if (m_State != NONE) {
        return true;
    }
    if (!m_Impl.Initialize()) {
        return false;
    }
    m_Impl.m_JoinApprovalCallback = OnJoinApproval;
    m_Impl.m_StationCallback = OnStationEvent;
    m_Impl.m_ReceiveCallback = OnReceive;
    m_Impl.m_ErrorCallback = OnError;
    m_Impl.m_MessageCallback = OnMessage;
    m_Impl.m_NotificationCallback = OnNotification;
    ChangeState(IDLE);
    s_FrameCount = 0;
    return true;
}

// 0x00516CC8 (name is ours)
bool net::nex::System::SetCommand(Command* command)
{
    if (m_pCmd != NULL) {
        return false;
    }
    m_pCmd = command;
    if (m_Error.m_Level == 0) {
        return true;
    }
    CmdFailed();
    return false;
}

// 0x00516D04 (name is ours)
bool net::nex::System::Send(const nn::pia::StationId* stationId, const void* data, u32 size)
{
    return s_pInstance->m_Impl.Send(*stationId, data, size);
}

// 0x00516D30 (name is ours)
void net::nex::System::GetMatchInfo(MatchInfo* info, Error* error)
{
    if (m_State == NONE) {
        info->Clear();
        return;
    }
    for (s32 i = 0; i < 4; i++) {
        info->participants[i].stationId = m_Participants[i].stationId;
        info->participants[i].rtt = m_Participants[i].rtt;
    }
    info->localStation = m_LocalStationId;
    info->hostStation = m_Impl.GetHostStationId();
    info->participantCount = m_ParticipantCount;
    m_Section.Enter();
    if (m_Error.m_Level != 0 && m_pCmd == NULL) {
        error->Merge(m_Error);
        m_Error.Clear();
    }
    m_Section.Exit();
    info->sessionId = m_Impl.m_SessionId;
    info->time = m_Impl.GetSessionTime();
    info->notif3Received = false;
    if (m_Impl.m_HasNotification) {
        info->notif3Received = true;
    }
    info->instanceId = s_FrameCount;
}

// 0x00516E70 (name is ours)
void net::nex::System::OnError(u8 level, u32 errorCode)
{
    System* self = s_pInstance;
    self->m_Section.Enter();
    if (level >= 4 || self->m_State != LOCAL_ERROR) {
        self->m_Error.Raise(level, errorCode);
    }
    self->m_Section.Exit();
}

// 0x00516ECC (name is ours)
void net::nex::System::OnStationEvent(nn::pia::StationId stationId, bool isJoined, const char16* name, u32 length)
{
    System* self = s_pInstance;
    SystemCallback* callback = self->m_pCallback;
    if (callback == NULL) {
        return;
    }
    u32 frame = s_FrameCount;
    if (isJoined) {
        callback->OnStationJoined(stationId, name, length, frame);
    } else if (stationId != self->m_LocalStationId) {
        callback->OnStationLeft(stationId, frame);
    }
}

// 0x00516F68 (name is ours)
bool net::nex::System::OnJoinApproval(const char16* name, u32 length)
{
    SystemCallback* callback = s_pInstance->m_pCallback;
    if (callback != NULL) {
        return callback->OnJoinApproval(name, length);
    }
    return false;
}

// 0x00516F9C (name is ours)
void net::nex::System::OnMessage(const u32* principalId, const wchar_t* text, u32 length, u32 isNotification)
{
    SystemCallback* callback = s_pInstance->m_pCallback;
    if (callback != NULL) {
        callback->OnMessage(principalId, text, length, isNotification);
    }
}

// 0x00516FDC (name is ours)
void net::nex::System::OnReceive(nn::pia::StationId stationId, const void* data, u32 size)
{
    SystemCallback* callback = s_pInstance->m_pCallback;
    if (callback != NULL) {
        callback->OnReceive(stationId, data, size);
    }
}

// 0x0051701C (name is ours)
void net::nex::System::StateIdle()
{
    if (m_pCmd == NULL) {
        return;
    }
    switch (m_pCmd->id) {
    case Command::CONNECT_LOCAL: {
        s32 result = m_Impl.ConnectLocal();
        if (result >= 0) {
            ChangeState(CONNECTED_LOCAL);
            CmdSuccess();
        } else {
            m_Section.Enter();
            m_Error.Set(Command::CONNECT_LOCAL, result);
            m_Section.Exit();
            CmdFailed();
        }
        return;
    }
    case Command::CONNECT_NEX:
        if (m_Impl.InitializeNexCommon()) {
            if (!m_Impl.InitializeNex(false)) {
                m_Impl.Finalize();
            } else if (!m_Impl.InitializeInet()) {
                m_Impl.FinalizeNex();
                m_Impl.Finalize();
            } else {
                ChangeState(CONNECTED_NEX);
                CmdSuccess();
                return;
            }
        }
        break;
    case Command::CONNECT_DATASTORE:
        if (m_Impl.InitializeNex(m_pCmd->param.value != 0)) {
            if (m_Impl.InitializeDataStore()) {
                ChangeState(CONNECTED_NEX);
                CmdSuccess();
                return;
            }
            m_Impl.FinalizeNex();
        }
        break;
    case Command::CONNECT_INDEPENDENT:
        if (m_Impl.InitializeIndependent(m_pCmd->param.value != 0)) {
            ChangeState(INDEPENDENT);
            CmdSuccess();
            return;
        }
        break;
    default:
        break;
    }
    FailCommand(m_pCmd->id, 6);
}

// 0x00517258 (name is ours)
void net::nex::System::StateNone()
{
    if (m_pCmd == NULL) {
        return;
    }
    FailCommand(m_pCmd->id, 6);
}

// 0x005172AC (name is ours)
void net::nex::System::StateConnectedLocal()
{
    if (m_pCmd == NULL) {
        return;
    }
    Command::Param& param = m_pCmd->param;
    switch (m_pCmd->id) {
    case Command::CREATE_SESSION: {
        s32 result = m_Impl.CreateLocalSession(param.createSession.mode, param.createSession.text,
                                               param.createSession.length * 2 + 2);
        if (result >= 0) {
            m_LocalStationId = m_Impl.GetLocalStationId();
            ChangeState(LOCAL_SESSION);
            CmdSuccess();
        } else {
            m_Section.Enter();
            m_Error.Set(Command::CREATE_SESSION, 6);
            m_Section.Exit();
            ChangeState(LOCAL_ERROR);
        }
        break;
    }
    case Command::SEARCH_SESSIONS:
        m_Impl.ScanLocal(param.searchSessions.mode, param.searchSessions.results, param.searchSessions.resultNum,
                         param.searchSessions.foundNum, param.searchSessions.withData);
        CmdSuccess();
        break;
    case Command::JOIN_SESSION: {
        s32 result = m_Impl.JoinLocalSession(param.value);
        if (result >= 0) {
            m_LocalStationId = m_Impl.GetLocalStationId();
            ChangeState(LOCAL_SESSION);
            CmdSuccess();
        } else {
            m_Section.Enter();
            m_Error.Set(Command::JOIN_SESSION, result);
            m_Section.Exit();
            ChangeState(LOCAL_ERROR);
        }
        break;
    }
    case Command::LEAVE_SESSION:
    case Command::CMD_13:
    case Command::LEAVE_SESSION_14:
        CmdSuccess();
        break;
    case Command::DISCONNECT:
        m_Impl.FinalizeLocal();
        CmdSuccess();
        ChangeState(IDLE);
        break;
    default:
        FailCommand(m_pCmd->id, 6);
        break;
    }
}

// 0x0051754C (name is ours)
void net::nex::System::StateConnectedNex()
{
    if (m_pCmd == NULL) {
        return;
    }
    Command::Param& param = m_pCmd->param;
    switch (m_pCmd->id) {
    case Command::CREATE_SESSION:
        if (m_Impl.IsConnectedToMatchmakeService() &&
            m_Impl.CreateNexSession(param.createSession.text, param.createSession.length, param.createSession.param)) {
            if (m_Impl.OpenNexSession(true)) {
                m_NotifiesFriends = true;
                m_LocalStationId = m_Impl.GetLocalStationId();
                ChangeState(NEX_SESSION);
                CmdSuccess();
                return;
            }
            m_Impl.LeaveFriendSession();
        }
        FailCommand(Command::CREATE_SESSION, 6);
        return;
    case Command::SEARCH_SESSIONS:
        m_Impl.GetSessionNotifications(param.searchSessions.results, param.searchSessions.resultNum,
                                       param.searchSessions.foundNum, param.searchSessions.withData);
        CmdSuccess();
        return;
    case Command::JOIN_SESSION:
        if (m_Impl.IsConnectedToMatchmakeService()) {
            s32 result = m_Impl.JoinSessionById(param.value);
            if (result >= 0) {
                m_LocalStationId = m_Impl.GetLocalStationId();
                ChangeState(NEX_SESSION);
                CmdSuccess();
                return;
            }
            FailCommand(Command::JOIN_SESSION, result);
            return;
        }
        FailCommand(Command::JOIN_SESSION, 6);
        return;
    case Command::JOIN_RANDOM_SESSION:
        if (m_Impl.IsConnectedToMatchmakeService() && m_Impl.JoinRandomSession(param.value)) {
            if (!m_Impl.IsNexSessionHost()) {
                m_LocalStationId = m_Impl.GetLocalStationId();
                ChangeState(NEX_SESSION);
                CmdSuccess();
                return;
            }
            if (m_Impl.OpenNexSession(false)) {
                m_NotifiesFriends = false;
                m_LocalStationId = m_Impl.GetLocalStationId();
                ChangeState(NEX_SESSION);
                CmdSuccess();
                return;
            }
            m_Section.Enter();
            m_Error.Set(Command::JOIN_RANDOM_SESSION, 6);
            m_Section.Exit();
            m_Impl.LeaveFriendSession();
            CmdFailed();
            return;
        }
        FailCommand(Command::JOIN_RANDOM_SESSION, 6);
        return;
    case Command::CREATE_RANDOM_SESSION:
        if (m_Impl.IsConnectedToMatchmakeService() && m_Impl.CreateSession(param.value)) {
            if (m_Impl.OpenNexSession(false)) {
                m_LocalStationId = m_Impl.GetLocalStationId();
                ChangeState(NEX_SESSION);
                CmdSuccess();
                return;
            }
            m_Impl.LeaveFriendSession();
        }
        FailCommand(Command::CREATE_RANDOM_SESSION, 6);
        return;
    case Command::JOIN_SESSION_BY_OWNER:
        if (m_Impl.IsConnectedToMatchmakeService()) {
            s32 result = m_Impl.JoinSessionByOwner(param.value);
            if (result >= 0) {
                m_LocalStationId = m_Impl.GetLocalStationId();
                ChangeState(NEX_SESSION);
                CmdSuccess();
                return;
            }
            FailCommand(Command::JOIN_SESSION_BY_OWNER, result);
            return;
        }
        FailCommand(Command::JOIN_SESSION_BY_OWNER, 6);
        return;
    case Command::DISCONNECT:
        m_Impl.FinalizeInet();
        m_Impl.FinalizeNex();
        m_Impl.Finalize();
        if (!m_Impl.IsLoggedIn()) {
            ChangeState(IDLE);
        }
        CmdSuccess();
        return;
    case Command::CONNECT_DATASTORE:
        if (m_Impl.InitializeNex(false)) {
            if (m_Impl.InitializeDataStore()) {
                CmdSuccess();
                return;
            }
            m_Impl.FinalizeNex();
        }
        FailCommand(Command::CONNECT_DATASTORE, 6);
        return;
    case Command::DISCONNECT_DATASTORE:
        m_Impl.FinalizeDataStore();
        m_Impl.FinalizeNex();
        if (!m_Impl.IsLoggedIn()) {
            ChangeState(IDLE);
        }
        CmdSuccess();
        return;
    case Command::UPLOAD_DATA:
        if (m_Impl.HasDataStore() && param.upload.data != NULL && param.upload.size != 0) {
            nn::crypto::EncryptAndGenerateAes128Ccm(param.upload.work, param.upload.data, param.upload.size, UPLOAD_KEY);
            if (m_Impl.UploadData(param.upload.work, param.upload.size + 32, param.upload.dataId,
                                  static_cast<const Impl::UploadInfo*>(param.upload.info), NULL, 0)) {
                CmdSuccess();
                return;
            }
        }
        FailCommand(Command::UPLOAD_DATA, 6);
        return;
    case Command::UPLOAD_META_BINARY:
        if (m_Impl.HasDataStore() &&
            m_Impl.UploadMetaBinary(param.upload.dataId, static_cast<const Impl::UploadInfo*>(param.upload.info), NULL, 0)) {
            CmdSuccess();
            return;
        }
        FailCommand(Command::UPLOAD_META_BINARY, 6);
        return;
    case Command::SEARCH_DATA:
        if (m_Impl.HasDataStore()) {
            bool isSuccess;
            if (param.searchData.dataId == 0) {
                isSuccess = m_Impl.SearchData(param.searchData.results, param.searchData.dataTypes, param.searchData.tagFunction,
                                              static_cast<Impl::SearchOrder>(param.searchData.order));
            } else {
                isSuccess = m_Impl.GetData(param.searchData.dataId, param.searchData.results, param.searchData.dataTypes[0],
                                           param.searchData.tagFunction);
            }
            if (isSuccess) {
                CmdSuccess();
                return;
            }
        }
        FailCommand(Command::SEARCH_DATA, 6);
        return;
    case Command::DOWNLOAD_DATA:
        if (m_Impl.HasDataStore()) {
            u32 size;
            if (m_Impl.DownloadData(param.download.dataId, param.download.work, param.download.size + 32, &size) && size != 0 &&
                nn::crypto::DecryptAndVerifyAes128Ccm(param.download.buffer, param.download.work, size, UPLOAD_KEY)) {
                *param.download.length = size - 32;
                CmdSuccess();
                return;
            }
        }
        FailCommand(Command::DOWNLOAD_DATA, 6);
        return;
    case Command::RESET_PERMISSION:
        if (m_Impl.HasDataStore()) {
            s32 result = m_Impl.ResetPermission(param.permission.dataId, param.permission.dataType, param.permission.tagFunction);
            if (result >= 0) {
                CmdSuccess();
                return;
            }
            FailCommand(Command::RESET_PERMISSION, result);
            return;
        }
        FailCommand(Command::RESET_PERMISSION, 6);
        return;
    case Command::RATE_OBJECT:
        if (m_Impl.HasDataStore() && m_Impl.RateObject(param.rate.dataId, param.rate.rating0, param.rate.rating1)) {
            CmdSuccess();
            return;
        }
        FailCommand(Command::RATE_OBJECT, 6);
        return;
    case Command::UPLOAD_DATA_2:
        if (m_Impl.HasDataStore() && param.upload.data != NULL && param.upload.size != 0) {
            nn::crypto::EncryptAndGenerateAes128Ccm(param.upload.work, param.upload.data, param.upload.size, UPLOAD_KEY_2);
            if (m_Impl.UploadData(param.upload.work, param.upload.size + 32, NULL,
                                  static_cast<const Impl::UploadInfo*>(param.upload.info), NULL, 0)) {
                CmdSuccess();
                return;
            }
        }
        FailCommand(Command::UPLOAD_DATA_2, 6);
        return;
    case Command::CONNECT_INDEPENDENT:
    case Command::DISCONNECT_INDEPENDENT:
        CmdSuccess();
        return;
    case Command::REQUEST_SERVICE_TOKEN:
        if (m_Impl.RequestServiceToken()) {
            CmdSuccess();
            return;
        }
        FailCommand(Command::REQUEST_SERVICE_TOKEN, 6);
        return;
    case Command::DELETE_OWN_DATA:
        if (m_Impl.HasDataStore() && m_Impl.DeleteOwnData()) {
            CmdSuccess();
            return;
        }
        FailCommand(Command::DELETE_OWN_DATA, 6);
        return;
    default:
        FailCommand(m_pCmd->id, 6);
        return;
    }
}

// 0x005180A4 (name is ours)
void net::nex::System::OnNotification(const u32* principalId, u32 param1, u32 param2, const wchar_t* text, u32 length,
                                      bool isFromHandler)
{
    SystemCallback* callback = s_pInstance->m_pCallback;
    if (callback != NULL) {
        callback->OnNotification(principalId, param1, param2, text, length, isFromHandler);
    }
}

// 0x00518104 (name is ours)
void net::nex::System::StateLocalSession()
{
    if (m_pCmd == NULL) {
        return;
    }
    Command::Param& param = m_pCmd->param;
    switch (m_pCmd->id) {
    case Command::CLOSE_SESSION:
        m_Impl.CloseParticipation();
        CmdSuccess();
        break;
    case Command::OPEN_SESSION:
        m_Impl.OpenParticipation();
        CmdSuccess();
        break;
    case Command::LEAVE_SESSION:
    case Command::LEAVE_SESSION_14:
        if (m_Impl.IsLocalHost()) {
            m_Impl.LeaveNexSession();
        } else {
            m_Impl.LeaveLocalSession();
        }
        m_LocalStationId = Impl::s_InvalidStationId;
        ChangeState(LOCAL_ERROR);
        break;
    case Command::SET_SESSION_DATA:
        if (m_Impl.IsLocalHost()) {
            m_Impl.SetLocalApplicationData(param.sessionData.data, param.sessionData.length * 2 + 2);
        }
        CmdSuccess();
        break;
    case Command::GET_FRIEND_CODE:
        m_Impl.GetFriendCode(param.friendCode.stationId, param.friendCode.friendCode);
        CmdSuccess();
        break;
    default:
        FailCommand(m_pCmd->id, 6);
        break;
    }
}

// 0x0051827C (name is ours)
void net::nex::System::StateLocalError()
{
    if (m_Impl.IsInLocalNetwork()) {
        return;
    }
    ChangeState(CONNECTED_LOCAL);
    m_pCmd->status = m_Error.m_Level != 0 ? Command::FAILED : Command::DONE;
    m_pCmd = NULL;
}

// 0x005182D4 (name is ours)
void net::nex::System::StateNexSession()
{
    if (m_pCmd == NULL) {
        return;
    }
    Command::Param& param = m_pCmd->param;
    switch (m_pCmd->id) {
    case Command::CLOSE_SESSION:
        if (m_Impl.IsNexSessionHost()) {
            m_Impl.CloseNexSession();
        }
        CmdSuccess();
        return;
    case Command::OPEN_SESSION:
        if (m_Impl.IsNexSessionHost()) {
            if (m_NotifiesFriends) {
                m_Impl.OpenNexSession(true, param.openSession.setParam, param.openSession.param);
            } else {
                m_Impl.OpenNexSession(false);
            }
        }
        CmdSuccess();
        return;
    case Command::MODIFY_SESSION_ATTRIBUTE:
        if (m_Impl.ModifySessionAttribute()) {
            CmdSuccess();
            return;
        }
        FailCommand(Command::MODIFY_SESSION_ATTRIBUTE, 6);
        return;
    case Command::LEAVE_SESSION:
    case Command::LEAVE_SESSION_14:
        m_Impl.LeaveFriendSession();
        m_NotifiesFriends = false;
        m_LocalStationId = Impl::s_InvalidStationId;
        ChangeState(CONNECTED_NEX);
        CmdSuccess();
        return;
    case Command::SET_SESSION_DATA:
        if (m_Impl.IsNexSessionHost()) {
            m_Impl.UpdateSessionText(param.sessionData.data, param.sessionData.length);
        }
        CmdSuccess();
        return;
    case Command::MATCHMAKE_METHOD_25:
        if (m_Impl.CallMatchmakeMethod25(&param.text.param1, param.text.text)) {
            CmdSuccess();
            return;
        }
        FailCommand(Command::MATCHMAKE_METHOD_25, 6);
        return;
    case Command::MATCHMAKE_METHOD_26:
        if (m_Impl.CallMatchmakeMethod26(&param.value)) {
            CmdSuccess();
            return;
        }
        FailCommand(Command::MATCHMAKE_METHOD_26, 6);
        return;
    default:
        FailCommand(m_pCmd->id, 6);
        return;
    }
}

// 0x005185B4 (name is ours)
void net::nex::System::StateIndependent()
{
    if (m_pCmd == NULL) {
        return;
    }
    switch (m_pCmd->id) {
    case Command::DISCONNECT_INDEPENDENT:
        m_Impl.FinalizeIndependent();
        if (!m_Impl.m_IsSocketInitialized) {
            ChangeState(IDLE);
        }
        CmdSuccess();
        break;
    case Command::REQUEST_SERVICE_TOKEN:
        if (m_Impl.RequestServiceToken()) {
            CmdSuccess();
        } else {
            FailCommand(Command::REQUEST_SERVICE_TOKEN, 6);
        }
        break;
    default:
        FailCommand(m_pCmd->id, 6);
        break;
    }
}

// 0x005186B0 (name is ours)
void net::nex::System::Update()
{
    s_FrameCount++;
    m_Impl.Process();
    if (m_pCmd != NULL) {
        Command::Param& param = m_pCmd->param;
        switch (m_pCmd->id) {
        case Command::SEND_NOTIFICATION_102:
            if ((m_State == CONNECTED_NEX || m_State == NEX_SESSION) && m_Impl.SendNotification102(param.text.text)) {
                CmdSuccess();
            } else {
                // (the original reports the command SEND_TEXT_MESSAGE here)
                FailCommand(Command::SEND_TEXT_MESSAGE, 6);
            }
            break;
        case Command::SEND_TEXT_MESSAGE:
            if ((m_State == CONNECTED_NEX || m_State == NEX_SESSION) && m_Impl.SendTextMessage(&param.text.param1, param.text.text)) {
                CmdSuccess();
            } else {
                FailCommand(Command::SEND_TEXT_MESSAGE, 6);
            }
            break;
        case Command::SEND_NOTIFICATION_103:
            if ((m_State == CONNECTED_NEX || m_State == NEX_SESSION) &&
                m_Impl.SendNotification103(param.notification103.param1, param.notification103.param2,
                                           param.notification103.text)) {
                CmdSuccess();
            } else {
                FailCommand(Command::SEND_NOTIFICATION_103, 6);
            }
            break;
        case Command::RESET_SYNC_CLOCK:
            Impl::ResetSyncClockRequestInterval();
            CmdSuccess();
            break;
        case Command::STUB_0:
            m_Impl.Stub0(param.value);
            CmdSuccess();
            break;
        case Command::STUB_1:
            m_Impl.Stub1(param.value);
            CmdSuccess();
            break;
        case Command::STUB_2:
            m_Impl.Stub2(param.value);
            CmdSuccess();
            break;
        case Command::STUB_3:
            m_Impl.Stub3(param.value);
            CmdSuccess();
            break;
        case Command::STUB_4:
            m_Impl.Stub4(param.value);
            CmdSuccess();
            break;
        case Command::STUB_5:
            m_Impl.Stub5(param.value);
            CmdSuccess();
            break;
        default:
            break;
        }
    }
    (this->*m_StateFn)();
    m_ParticipantCount = m_Impl.GetInfo(m_Participants, 4);
    if (m_ParticipantCount > 1 && m_pSender != NULL) {
        m_pSender->Process(Send);
    }
    m_Impl.Process();
}

// 0x0051897C (name is ours)
net::nex::System::System(Sender* sender, SystemCallback* callback)
    : m_Section(nn::os::CriticalSection::InitializeTag()), m_pCmd(NULL), m_pSender(sender), m_pCallback(callback),
      m_LocalStationId(Impl::s_InvalidStationId)
{
    m_ParticipantCount = 0;
    m_NotifiesFriends = false;
    s_pInstance = this;
    for (s32 i = 0; i < 4; i++) {
        m_Participants[i].stationId = nn::pia::StationId(0xFFFFFFFD, 0xFFFFFFFF);
        m_Participants[i].rtt = 0;
    }
    ChangeState(NONE);
    s_FrameCount = 0;
}
} // namespace nex
} // namespace net
