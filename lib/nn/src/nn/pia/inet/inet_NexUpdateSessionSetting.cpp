#include "nn/pia/inet/inet_NexUpdateSessionSetting.h"

namespace nn {
namespace pia {
namespace inet {
// 0x0072F540 (name is ours)
u32 nn::pia::inet::NexUpdateSessionSetting::GetAttribute(u32 index) const
{
    if (index < ATTRIBUTE_NUM) {
        return m_Attributes[index];
    }
    return 0;
}

// 0x0072F554 (name is ours)
bool nn::pia::inet::NexUpdateSessionSetting::GetParamUpGI() const
{
    return m_ParamUpGI;
}

// 0x0072F560 (name is ours)
bool nn::pia::inet::NexUpdateSessionSetting::IsParamRVSet() const
{
    return m_IsParamRVSet;
}

// 0x0072F56C (name is ours)
bool nn::pia::inet::NexUpdateSessionSetting::IsParamVRSet() const
{
    return m_IsParamVRSet;
}

// 0x0072F578 (name is ours)
bool nn::pia::inet::NexUpdateSessionSetting::IsAnyParamSet() const
{
    return m_IsParamRVSet || m_IsParamDRSet || m_IsParamVRSet || m_IsParamNCCSet || m_ParamUpGI;
}

// 0x0072F5A8 (name is ours)
bool nn::pia::inet::NexUpdateSessionSetting::IsOpenParticipation() const
{
    return m_IsOpenParticipation;
}

// 0x0072F5B4 (name is ours)
const wchar_t* nn::pia::inet::NexUpdateSessionSetting::GetUserPassword() const
{
    return m_UserPassword;
}

// 0x0072F5C0 (name is ours)
bool nn::pia::inet::NexUpdateSessionSetting::IsParamDRSet() const
{
    return m_IsParamDRSet;
}

// 0x0072F5CC (name is ours)
bool nn::pia::inet::NexUpdateSessionSetting::IsParamNCCSet() const
{
    return m_IsParamNCCSet;
}

// 0x0072F5D8 (name is ours)
void nn::pia::inet::NexUpdateSessionSetting::GetApplicationData(nex::qVector<u8>* pData) const
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
