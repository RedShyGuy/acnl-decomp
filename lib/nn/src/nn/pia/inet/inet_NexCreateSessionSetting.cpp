#include "nn/pia/inet/inet_NexCreateSessionSetting.h"
#include <cstring>

namespace nn {
namespace pia {
namespace inet {
// 0x00403E28
nn::pia::inet::NexCreateSessionSetting::NexCreateSessionSetting()
    : m_Unknown0x8(0), m_Unknown0xC(0), m_ApplicationDataSize(0), m_Unknown0x42C(128), m_IsOpenParticipation(0), m_ProgressScore(0), m_ParamRV(0), m_IsParamRVSet(0), m_ParamVR(0),
      m_IsParamVRSet(0), m_ParamDR(0), m_IsParamDRSet(0), m_ParamUsGI(0), m_IsParamUsGISet(0), m_ParamNCC(0), m_IsParamNCCSet(0),
      m_IsParamOIASet(0), m_Unknown0x474(0), m_ReferGatheringId(0)
{
    for (s32 i = 0; i < 6; i++) {
        m_Attributes[i] = 0;
    }
    memset(m_Description, 0, sizeof(m_Description));
    memset(m_UserPassword, 0, sizeof(m_UserPassword));
    memset(m_ParamOIA, 0, sizeof(m_ParamOIA));
}

// 0x00403EF4
// 0x00403EF0 (deleting dtor)
nn::pia::inet::NexCreateSessionSetting::~NexCreateSessionSetting()
{
    // empty (in the original too)
}

// 0x0072F1D8 (name is ours)
bool nn::pia::inet::NexCreateSessionSetting::GetParamUsGI() const
{
    return m_ParamUsGI;
}

// 0x0072F1E4 (name is ours)
u32 nn::pia::inet::NexCreateSessionSetting::GetAttribute(u32 index) const
{
    if (index < ATTRIBUTE_NUM) {
        return m_Attributes[index];
    }
    return 0;
}

// 0x0072F1F8 (name is ours)
bool nn::pia::inet::NexCreateSessionSetting::IsParamUsGISet() const
{
    return m_IsParamUsGISet;
}

// 0x0072F204 (name is ours)
bool nn::pia::inet::NexCreateSessionSetting::IsParamRVSet() const
{
    return m_IsParamRVSet;
}

// 0x0072F210 (name is ours)
bool nn::pia::inet::NexCreateSessionSetting::IsParamVRSet() const
{
    return m_IsParamVRSet;
}

// 0x0072F21C (name is ours)
u16 nn::pia::inet::NexCreateSessionSetting::GetUnknown0x474() const
{
    return m_Unknown0x474;
}

// 0x0072F228 (name is ours)
bool nn::pia::inet::NexCreateSessionSetting::IsOpenParticipation() const
{
    return m_IsOpenParticipation;
}

// 0x0072F234 (name is ours)
const wchar_t* nn::pia::inet::NexCreateSessionSetting::GetParamOIA() const
{
    return m_ParamOIA;
}

// 0x0072F240 (name is ours)
const wchar_t* nn::pia::inet::NexCreateSessionSetting::GetUserPassword() const
{
    return m_UserPassword;
}

// 0x0072F24C (name is ours)
bool nn::pia::inet::NexCreateSessionSetting::IsParamDRSet() const
{
    return m_IsParamDRSet;
}

// 0x0072F258 (name is ours)
bool nn::pia::inet::NexCreateSessionSetting::IsParamOIASet() const
{
    return m_IsParamOIASet;
}

// 0x0072F264 (name is ours)
bool nn::pia::inet::NexCreateSessionSetting::IsParamNCCSet() const
{
    return m_IsParamNCCSet;
}

// 0x0072F270 (name is ours)
bool nn::pia::inet::NexCreateSessionSetting::IsAnyParamSet() const
{
    return m_IsParamRVSet || m_IsParamVRSet || m_IsParamDRSet || m_IsParamUsGISet || m_IsParamNCCSet;
}

// 0x0072F2AC (name is ours)
void nn::pia::inet::NexCreateSessionSetting::GetApplicationData(nex::qVector<u8>* pData) const
{
    pData->clear();
    pData->reserve(m_ApplicationDataSize);
    for (u32 i = m_Unknown0x42C; i < m_Unknown0x42C + m_ApplicationDataSize; i++) {
        pData->push_back(m_ApplicationData[i]);
    }
}

} // namespace inet
} // namespace pia
} // namespace nn
