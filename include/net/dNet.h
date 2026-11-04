#pragma once

#include "decomp.h"
#include "net/dSender.h"
#include "net/nex/dFramework.h"

namespace net {
class SystemCallback;

// The network of the game: the Sender for the data to the stations and the Framework (created on the
// first Initialize); every function passes on to the Framework and answers for a missing one (names
// after libgarden: Net, s_Instance, m_pFramework, Connect, CreateOrJoinRandomSession, IsError; the
// others are ours)
class Net
{
public:
    typedef nex::Framework::TagFunction TagFunction;

    Net() : m_pFramework(NULL) {}
    ~Net(); // 0x0051B510

    static Net* Get() { return &s_Instance; }
    nex::Framework* GetFramework() { return m_pFramework; }

    void ClearError(); // 0x0050D640
    bool Initialize(SystemCallback* callback); // 0x0050D654
    bool RateObject(u64 dataId, s8 rating0, s8 rating1); // 0x0050D724
    bool SearchSessions(ScanBufBase** results, u32 resultNum, u32* foundNum, s8 withData, u8 mode); // 0x0050D74C
    bool DisconnectIndependent(); // 0x0050D780
    bool CreateSession(const char16* text, u32 length, u16 param, u8 mode); // 0x0050D794
    bool SearchData(SearchDataResultList* results, const u16* dataTypes, u32 dataTypeNum, TagFunction tagFunction,
                    u8 order); // 0x0050D7B0
    bool GetData(u64 dataId, SearchDataResultList* results, u16 dataType, TagFunction tagFunction); // 0x0050D7D8
    bool ConnectIndependent(u32 param); // 0x0050D800
    bool UploadData(const void* data, u32 size, void* work, u64* dataId, const void* info); // 0x0050D814
    bool CallMatchmakeMethod25(const u32* param, const wchar_t* text); // 0x0050D83C
    bool DisconnectDataStore(); // 0x0050D850
    bool ConnectDataStore(u32 param); // 0x0050D868
    bool UploadData2(const void* data, u32 size, void* work, const void* info); // 0x0050D87C
    bool DownloadData(u64 dataId, void* buffer, u32 size, void* work, u32* length); // 0x0050D898
    bool JoinSessionByOwner(const u32* principalId); // 0x0050D8C0
    bool CreateOrJoinRandomSession(u32 param); // 0x0050D8D4 | libgarden
    bool CreateRandomSession(u32 param); // 0x0050D8E8
    bool ResetPermission(u64 dataId, u16 dataType, TagFunction tagFunction); // 0x0050D8FC
    bool SetSessionData(const char16* data, u32 length); // 0x0050D924
    void GetFriendCodeDirect(const nn::pia::StationId* stationId, u64* friendCode); // 0x0050D938
    bool SendNotification102(const wchar_t* text); // 0x0050D980
    bool SendTextMessage(const u32* principalId, const wchar_t* text); // 0x0050D994
    // whether the friends service gives a principal id
    static bool HasMyPrincipalId(); // 0x0050D9A8
    bool CheckCommand(); // 0x0050D9D8
    void SetPlayerName(const char16* name, u32 length); // 0x0050D9F0
    bool GetFriendCode(const nn::pia::StationId* stationId, u64* friendCode); // 0x0050DA04
    bool ResetSyncClock(); // 0x0050DA50
    bool SendNotification103(u32 param1, u32 param2, const wchar_t* text); // 0x0050DA64
    bool OpenSession(s8 setParam, u16 param); // 0x0050DA78
    bool LeaveSession(); // 0x0050DA90
    bool CloseSession(); // 0x0050DAA8
    bool JoinSession(u32 sessionId); // 0x0050DAC0
    bool Disconnect(); // 0x0050DAD4
    bool Connect(bool isLocal); // 0x0050DAEC | libgarden
    void Finalize(); // 0x0050DB00
    bool ModifySessionAttribute(const nn::pia::StationId* stationId); // 0x0050DB4C

    DECOMP_EMIT_COPY bool DisconnectAll() // 0x001257A4
    {
        if (m_pFramework == NULL) {
            return true;
        }
        return m_pFramework->DisconnectAll();
    }
    DECOMP_EMIT_COPY void Update() // 0x001257BC
    {
        if (m_pFramework != NULL) {
            m_pFramework->Update();
        }
    }
    DECOMP_EMIT_COPY bool IsLocal() const // 0x00126DA4
    {
        if (m_pFramework == NULL) {
            return false;
        }
        return m_pFramework->IsLocal();
    }
    DECOMP_EMIT_COPY u32 GetFrameCount() const // 0x00747B18
    {
        if (m_pFramework == NULL) {
            return 0;
        }
        return m_pFramework->GetFrameCount();
    }
    DECOMP_EMIT_COPY const nn::pia::StationId& GetLocalStationId() const // 0x00747B2C
    {
        if (m_pFramework == NULL) {
            return s_InvalidStationId;
        }
        return m_pFramework->GetLocalStationId();
    }
    DECOMP_EMIT_COPY bool IsParticipant(const nn::pia::StationId& stationId) const // 0x00747B48
    {
        if (stationId == nn::pia::StationId(0xFFFFFFFD, 0xFFFFFFFF) || m_pFramework == NULL) {
            return false;
        }
        return m_pFramework->IsParticipant(stationId);
    }
    DECOMP_EMIT_COPY u32 GetErrorCode() const // 0x00747B88
    {
        if (m_pFramework == NULL) {
            return 0;
        }
        return m_pFramework->GetErrorCode();
    }
    DECOMP_EMIT_COPY u8 GetErrorKind() const // 0x00747B9C
    {
        if (m_pFramework == NULL) {
            return 3;
        }
        return m_pFramework->GetErrorKind();
    }
    DECOMP_EMIT_COPY u8 GetErrorLevel() const // 0x00747BB4
    {
        if (m_pFramework == NULL) {
            return 0;
        }
        return m_pFramework->GetErrorLevel();
    }
    DECOMP_EMIT_COPY bool IsInitialized(bool ignoresBlock) const // 0x00747BC8
    {
        if (m_pFramework == NULL) {
            return false;
        }
        return m_pFramework->IsInitialized(ignoresBlock);
    }
    DECOMP_EMIT_COPY s64 GetSessionTime() const // 0x00747BDC
    {
        if (m_pFramework == NULL) {
            return 0;
        }
        return m_pFramework->GetSessionTime();
    }
    DECOMP_EMIT_COPY const nn::pia::StationId& GetHostStationId() const // 0x00747BF4
    {
        if (m_pFramework == NULL) {
            return s_InvalidStationId;
        }
        return m_pFramework->GetHostStationId();
    }
    // half the round trip time to the station
    DECOMP_EMIT_COPY u32 GetLatency(const nn::pia::StationId& stationId) const // 0x00747C10
    {
        if (m_pFramework == NULL) {
            return 0;
        }
        return m_pFramework->GetRtt(stationId) / 2;
    }
    DECOMP_EMIT_COPY bool HasNotification() const // 0x00747C2C
    {
        if (m_pFramework == NULL) {
            return false;
        }
        return m_pFramework->HasNotification(1);
    }
    DECOMP_EMIT_COPY bool IsError() const // 0x00747C44 | libgarden
    {
        if (m_pFramework == NULL) {
            return false;
        }
        return m_pFramework->IsError();
    }
    DECOMP_EMIT_COPY bool IsLoggedIn(bool ignoresBlock) const // 0x00747C58
    {
        if (m_pFramework == NULL) {
            return false;
        }
        return m_pFramework->IsLoggedIn(ignoresBlock);
    }
    DECOMP_EMIT_COPY bool IsHost() const // 0x00747C6C
    {
        if (m_pFramework == NULL) {
            return false;
        }
        return m_pFramework->IsHost();
    }
    DECOMP_EMIT_COPY bool IsInternet(bool ignoresBlock) const // 0x00747C80
    {
        if (m_pFramework == NULL) {
            return false;
        }
        return m_pFramework->IsInternet(ignoresBlock);
    }

    Sender m_Sender;              // 0x00
    u32 unk_0xB4;
    nex::Framework* m_pFramework; // 0xB8

    static Net s_Instance;
    static nn::pia::StationId s_InvalidStationId;
};
ASSERT_SIZE(Net, 0xBC);
} // namespace net
