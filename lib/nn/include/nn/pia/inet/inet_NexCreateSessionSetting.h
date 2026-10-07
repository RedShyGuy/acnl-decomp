#pragma once

#include "decomp.h"
#include "nn/nex/nex_qVector.h"
#include "nn/pia/session/session_CreateSessionSetting.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet23NexCreateSessionSettingE
// vtable 0x00900458 (vptr 0x00900460)
//
// The create setting of the inet network (the setters are inline in the application). The layout
// is from the constructor; the names are ours.
class NexCreateSessionSetting : public ::nn::pia::session::CreateSessionSetting
{
public:
    NexCreateSessionSetting(); // 0x00403E28
    virtual ~NexCreateSessionSetting(); // 0x00403EF4 slot 0x00
    // 0x00403EF0 slot 0x04 (deleting dtor)

    static const u32 ATTRIBUTE_NUM = 6;

    // (inline; armlink placed them at the end of the code; names are ours)
    bool GetParamUsGI() const; // 0x0072F1D8
    // 0 if the index is outside
    u32 GetAttribute(u32 index) const; // 0x0072F1E4
    bool IsParamUsGISet() const; // 0x0072F1F8
    bool IsParamRVSet() const; // 0x0072F204
    bool IsParamVRSet() const; // 0x0072F210
    u16 GetUnknown0x474() const; // 0x0072F21C
    bool IsOpenParticipation() const; // 0x0072F228
    const wchar_t* GetParamOIA() const; // 0x0072F234
    const wchar_t* GetUserPassword() const; // 0x0072F240
    bool IsParamDRSet() const; // 0x0072F24C
    bool IsParamOIASet() const; // 0x0072F258
    bool IsParamNCCSet() const; // 0x0072F264
    // whether one of the parameters RV, VR, DR, UsGI or NCC is set
    bool IsAnyParamSet() const; // 0x0072F270
    void GetApplicationData(nex::qVector<u8>* pData) const; // 0x0072F2AC

    u32 m_Unknown0x8;                  // 0x008
    u8 m_Unknown0xC;                   // 0x00C
    wchar_t m_Description[257];        // 0x00E
    u32 m_Attributes[ATTRIBUTE_NUM];   // 0x210
    u8 m_ApplicationData[0x200];       // 0x228
    u32 m_ApplicationDataSize;         // 0x428
    u32 m_Unknown0x42C;                // 0x42C, 128 (the start of the data in m_ApplicationData)
    bool m_IsOpenParticipation;        // 0x430
    u8 m_ProgressScore;                // 0x431
    u32 m_ParamRV;                     // 0x434
    bool m_IsParamRVSet;               // 0x438
    u32 m_ParamVR;                     // 0x43C
    bool m_IsParamVRSet;               // 0x440
    u32 m_ParamDR;                     // 0x444
    bool m_IsParamDRSet;               // 0x448
    bool m_ParamUsGI;                  // 0x449
    bool m_IsParamUsGISet;             // 0x44A
    u32 m_ParamNCC;                    // 0x44C
    bool m_IsParamNCCSet;              // 0x450
    wchar_t m_ParamOIA[16];            // 0x452
    bool m_IsParamOIASet;              // 0x472
    u16 m_Unknown0x474;                // 0x474
    u32 m_ReferGatheringId;            // 0x478
    wchar_t m_UserPassword[33];        // 0x47C
};
ASSERT_OFFSET(NexCreateSessionSetting, m_Attributes, 0x210);
ASSERT_OFFSET(NexCreateSessionSetting, m_ApplicationDataSize, 0x428);
ASSERT_OFFSET(NexCreateSessionSetting, m_UserPassword, 0x47C);
ASSERT_SIZE(NexCreateSessionSetting, 0x4C0);
} // namespace inet
} // namespace pia
} // namespace nn
