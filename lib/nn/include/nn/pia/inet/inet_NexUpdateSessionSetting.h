#pragma once

#include "decomp.h"
#include "nn/nex/nex_qVector.h"

namespace nn {
namespace pia {
namespace inet {
// The setting for the change of the matchmake session (name is ours): UpdateSessionSettingJob
// passes it on as a session::CreateSessionSetting, but its layout differs and it has no RTTI in
// the binary. The layout is from NexMatchmakeSession::SetSessionSetting and the getters; the
// names are ours.
class NexUpdateSessionSetting
{
public:
    static const u32 ATTRIBUTE_NUM = 6;

    // (inline; armlink placed them at the end of the code; names are ours)
    // 0 if the index is outside
    u32 GetAttribute(u32 index) const; // 0x0072F540
    bool GetParamUpGI() const; // 0x0072F554
    bool IsParamRVSet() const; // 0x0072F560
    bool IsParamVRSet() const; // 0x0072F56C
    // whether one of the parameters RV, DR, VR, NCC or UpGI is set
    bool IsAnyParamSet() const; // 0x0072F578
    bool IsOpenParticipation() const; // 0x0072F5A8
    const wchar_t* GetUserPassword() const; // 0x0072F5B4
    bool IsParamDRSet() const; // 0x0072F5C0
    bool IsParamNCCSet() const; // 0x0072F5CC
    void GetApplicationData(nex::qVector<u8>* pData) const; // 0x0072F5D8

    u32 m_Unknown0x0;                  // 0x000
    u32 m_ModificationFlags;           // 0x004
    u8 m_MatchmakeSystemType;          // 0x008
    u16 m_MinParticipants;             // 0x00A
    u16 m_MaxParticipants;             // 0x00C
    wchar_t m_Description[257];        // 0x00E
    u32 m_Attributes[ATTRIBUTE_NUM];   // 0x210
    u8 m_ApplicationData[0x200];       // 0x228
    u32 m_ApplicationDataSize;         // 0x428
    u32 m_Unknown0x42C;                // 0x42C, the start of the data in m_ApplicationData
    bool m_IsOpenParticipation;        // 0x430
    u8 m_ProgressScore;                // 0x431
    wchar_t m_UserPassword[33];        // 0x432
    u32 m_ParamRV;                     // 0x474
    bool m_IsParamRVSet;               // 0x478
    u32 m_ParamDR;                     // 0x47C
    bool m_IsParamDRSet;               // 0x480
    u32 m_ParamVR;                     // 0x484
    bool m_IsParamVRSet;               // 0x488
    u32 m_ParamNCC;                    // 0x48C
    bool m_IsParamNCCSet;              // 0x490
    bool m_ParamUpGI;                  // 0x491
};
ASSERT_OFFSET(NexUpdateSessionSetting, m_Description, 0xE);
ASSERT_OFFSET(NexUpdateSessionSetting, m_Attributes, 0x210);
ASSERT_OFFSET(NexUpdateSessionSetting, m_UserPassword, 0x432);
ASSERT_OFFSET(NexUpdateSessionSetting, m_ParamRV, 0x474);
ASSERT_OFFSET(NexUpdateSessionSetting, m_ParamUpGI, 0x491);
} // namespace inet
} // namespace pia
} // namespace nn
