#include "net/dNet.h"

#include "Other/dFriendList.h"
#include "nn/os/CTR/CTR_Api.h"

namespace net {
// 0x00A23020
Net net::Net::s_Instance;
// 0x00954670
nn::pia::StationId net::Net::s_InvalidStationId(0xFFFFFFFD, 0xFFFFFFFF);
// (builds s_Instance and s_InvalidStationId)
// 0x00785450 (static initializer)

// 0x0050D640 (name is ours)
void net::Net::ClearError()
{
    if (m_pFramework != NULL) {
        m_pFramework->ClearError();
    }
}

// 0x0050D654 (name is ours)
bool net::Net::Initialize(SystemCallback* callback)
{
    if (m_pFramework != NULL && m_pFramework->IsBlocked()) {
        return false;
    }
    if (m_pFramework == NULL) {
        m_pFramework = new (g_pNetHeap, 4) nex::Framework();
    }
    bool isInitialized = m_pFramework->Initialize(&m_Sender, callback);
    if (!isInitialized) {
        Finalize();
    }
    // (the result went to a removed debug output)
    nn::os::CTR::IsRunningAsExtApplication();
    return isInitialized;
}

// 0x0050D724 (name is ours)
bool net::Net::RateObject(u64 dataId, s8 rating0, s8 rating1)
{
    if (m_pFramework == NULL) {
        return false;
    }
    return m_pFramework->RateObject(dataId, rating0, rating1);
}

// 0x0050D74C (name is ours)
bool net::Net::SearchSessions(ScanBufBase** results, u32 resultNum, u32* foundNum, s8 withData, u8 mode)
{
    if (foundNum == NULL || m_pFramework == NULL) {
        return false;
    }
    return m_pFramework->SearchSessions(results, resultNum, foundNum, withData, mode);
}

// 0x0050D780 (name is ours)
bool net::Net::DisconnectIndependent()
{
    if (m_pFramework == NULL) {
        return false;
    }
    return m_pFramework->DisconnectIndependent();
}

// 0x0050D794 (name is ours)
bool net::Net::CreateSession(const char16* text, u32 length, u16 param, u8 mode)
{
    if (m_pFramework == NULL) {
        return false;
    }
    return m_pFramework->CreateSession(text, length, param, mode);
}

// 0x0050D7B0 (name is ours)
bool net::Net::SearchData(SearchDataResultList* results, const u16* dataTypes, u32 dataTypeNum, TagFunction tagFunction,
                          u8 order)
{
    if (m_pFramework == NULL) {
        return false;
    }
    return m_pFramework->SearchData(results, dataTypes, dataTypeNum, tagFunction, order);
}

// 0x0050D7D8 (name is ours)
bool net::Net::GetData(u64 dataId, SearchDataResultList* results, u16 dataType, TagFunction tagFunction)
{
    if (m_pFramework == NULL) {
        return false;
    }
    return m_pFramework->GetData(dataId, results, dataType, tagFunction);
}

// 0x0050D800 (name is ours)
bool net::Net::ConnectIndependent(u32 param)
{
    if (m_pFramework == NULL) {
        return false;
    }
    return m_pFramework->ConnectIndependent(param);
}

// 0x0050D814 (name is ours)
bool net::Net::UploadData(const void* data, u32 size, void* work, u64* dataId, const void* info)
{
    if (m_pFramework == NULL) {
        return false;
    }
    return m_pFramework->UploadData(data, size, work, dataId, info);
}

// 0x0050D83C (name is ours)
bool net::Net::CallMatchmakeMethod25(const u32* param, const wchar_t* text)
{
    if (m_pFramework == NULL) {
        return false;
    }
    return m_pFramework->CallMatchmakeMethod25(param, text);
}

// 0x0050D850 (name is ours)
bool net::Net::DisconnectDataStore()
{
    if (m_pFramework == NULL) {
        return true;
    }
    return m_pFramework->DisconnectDataStore();
}

// 0x0050D868 (name is ours)
bool net::Net::ConnectDataStore(u32 param)
{
    if (m_pFramework == NULL) {
        return false;
    }
    return m_pFramework->ConnectDataStore(param);
}

// 0x0050D87C (name is ours)
bool net::Net::UploadData2(const void* data, u32 size, void* work, const void* info)
{
    if (m_pFramework == NULL) {
        return false;
    }
    return m_pFramework->UploadData2(data, size, work, info);
}

// 0x0050D898 (name is ours)
bool net::Net::DownloadData(u64 dataId, void* buffer, u32 size, void* work, u32* length)
{
    if (m_pFramework == NULL) {
        return false;
    }
    return m_pFramework->DownloadData(dataId, buffer, size, work, length);
}

// 0x0050D8C0 (name is ours)
bool net::Net::JoinSessionByOwner(const u32* principalId)
{
    if (m_pFramework == NULL) {
        return false;
    }
    return m_pFramework->JoinSessionByOwner(principalId);
}

// 0x0050D8D4 | libgarden
bool net::Net::CreateOrJoinRandomSession(u32 param)
{
    if (m_pFramework == NULL) {
        return false;
    }
    return m_pFramework->CreateOrJoinRandomSession(param);
}

// 0x0050D8E8 (name is ours)
bool net::Net::CreateRandomSession(u32 param)
{
    if (m_pFramework == NULL) {
        return false;
    }
    return m_pFramework->CreateRandomSession(param);
}

// 0x0050D8FC (name is ours)
bool net::Net::ResetPermission(u64 dataId, u16 dataType, TagFunction tagFunction)
{
    if (m_pFramework == NULL) {
        return false;
    }
    return m_pFramework->ResetPermission(dataId, dataType, tagFunction);
}

// 0x0050D924 (name is ours)
bool net::Net::SetSessionData(const char16* data, u32 length)
{
    if (m_pFramework == NULL) {
        return false;
    }
    return m_pFramework->SetSessionData(data, length);
}

// 0x0050D938 (name is ours)
void net::Net::GetFriendCodeDirect(const nn::pia::StationId* stationId, u64* friendCode)
{
    if (m_pFramework != NULL && *stationId != nn::pia::StationId(0xFFFFFFFD, 0xFFFFFFFF)) {
        m_pFramework->GetFriendCodeDirect(stationId, friendCode);
        return;
    }
    *friendCode = 0;
}

// 0x0050D980 (name is ours)
bool net::Net::SendNotification102(const wchar_t* text)
{
    if (m_pFramework == NULL) {
        return false;
    }
    return m_pFramework->SendNotification102(text);
}

// 0x0050D994 (name is ours)
bool net::Net::SendTextMessage(const u32* principalId, const wchar_t* text)
{
    if (m_pFramework == NULL) {
        return false;
    }
    return m_pFramework->SendTextMessage(principalId, text);
}

// 0x0050D9A8 (name is ours)
bool net::Net::HasMyPrincipalId()
{
    FriendList* friendList = FriendList::s_pInstance;
    if (friendList == NULL) {
        return false;
    }
    return friendList->GetMyPrincipalId() != 0;
}

// 0x0050D9D8 (name is ours)
bool net::Net::CheckCommand()
{
    if (m_pFramework == NULL) {
        return true;
    }
    return m_pFramework->CheckCommand();
}

// 0x0050D9F0 (name is ours)
void net::Net::SetPlayerName(const char16* name, u32 length)
{
    if (m_pFramework != NULL) {
        m_pFramework->SetPlayerName(name, length);
    }
}

// 0x0050DA04 (name is ours)
bool net::Net::GetFriendCode(const nn::pia::StationId* stationId, u64* friendCode)
{
    if (m_pFramework != NULL && *stationId != nn::pia::StationId(0xFFFFFFFD, 0xFFFFFFFF)) {
        return m_pFramework->GetFriendCode(stationId, friendCode);
    }
    *friendCode = 0;
    return true;
}

// 0x0050DA50 (name is ours)
bool net::Net::ResetSyncClock()
{
    if (m_pFramework == NULL) {
        return false;
    }
    return m_pFramework->ResetSyncClock();
}

// 0x0050DA64 (name is ours)
bool net::Net::SendNotification103(u32 param1, u32 param2, const wchar_t* text)
{
    if (m_pFramework == NULL) {
        return false;
    }
    return m_pFramework->SendNotification103(param1, param2, text);
}

// 0x0050DA78 (name is ours)
bool net::Net::OpenSession(s8 setParam, u16 param)
{
    if (m_pFramework == NULL) {
        return true;
    }
    return m_pFramework->OpenSession(setParam, param);
}

// 0x0050DA90 (name is ours)
bool net::Net::LeaveSession()
{
    if (m_pFramework == NULL) {
        return true;
    }
    return m_pFramework->LeaveSession();
}

// 0x0050DAA8 (name is ours)
bool net::Net::CloseSession()
{
    if (m_pFramework == NULL) {
        return true;
    }
    return m_pFramework->CloseSession();
}

// 0x0050DAC0 (name is ours)
bool net::Net::JoinSession(u32 sessionId)
{
    if (m_pFramework == NULL) {
        return false;
    }
    return m_pFramework->JoinSession(sessionId);
}

// 0x0050DAD4 (name is ours)
bool net::Net::Disconnect()
{
    if (m_pFramework == NULL) {
        return true;
    }
    return m_pFramework->Disconnect();
}

// 0x0050DAEC | libgarden
bool net::Net::Connect(bool isLocal)
{
    if (m_pFramework == NULL) {
        return false;
    }
    return m_pFramework->Connect(isLocal);
}

// 0x0050DB00 (name is ours)
void net::Net::Finalize()
{
    if (m_pFramework != NULL) {
        m_pFramework->Finalize();
        if (!m_pFramework->IsInitialized(false)) {
            delete m_pFramework;
            m_pFramework = NULL;
        }
    }
}

// 0x0050DB4C (name is ours)
bool net::Net::ModifySessionAttribute(const nn::pia::StationId* stationId)
{
    if (m_pFramework == NULL) {
        return true;
    }
    return m_pFramework->ModifySessionAttribute(stationId);
}

// 0x0051B510 (name is ours)
net::Net::~Net()
{
    Finalize();
}
} // namespace net
