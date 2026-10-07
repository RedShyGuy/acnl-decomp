#include "nn/pia/inet/inet_NexSessionSearchCriteria.h"
#include <cstring>
#include "nn/nex/nex_MatchmakeParam.h"
#include "nn/nex/nex_MatchmakeSessionSearchCriteria.h"
#include "nn/nex/nex_ResultRange.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_Variant.h"
#include "nn/nex/nex_qVector.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_SessionBeginMonitoringContent.h"
#include "nn/pia/inet/inet_NexMatchmakeSession.h"

namespace nn {
namespace pia {
namespace inet {
// 0x0040B298 | libgarden [tier A]
void nn::pia::inet::NexSessionSearchCriteria::SetGameMode(unsigned long gameMode)
{
    m_GameMode = gameMode;
    m_Flags |= FLAG_GAME_MODE;
}

// 0x0040B2B4 (name is ours)
void nn::pia::inet::NexSessionSearchCriteria::SetAttribute(u32 index, u32 value)
{
    if (index >= ATTRIBUTE_NUM) {
        return;
    }
    m_AttributeValues[index][0] = value;
    m_AttributeValueNum[index] = 1;
    m_IsAttributeRange[index] = false;
    m_Flags |= FLAG_ATTRIBUTE;
}

// 0x0040B304 | libgarden [tier A]
void nn::pia::inet::NexSessionSearchCriteria::SetOpenedOnly(bool isOpenedOnly)
{
    m_IsOpenedOnly = isOpenedOnly;
    m_Flags |= FLAG_OPENED_ONLY;
}

// 0x0040B320 (name is ours)
void nn::pia::inet::NexSessionSearchCriteria::SetVacantOnly(bool isVacantOnly)
{
    m_IsVacantOnly = isVacantOnly;
    m_Flags |= FLAG_VACANT_ONLY;
}

// 0x0040B33C (name is ours)
void nn::pia::inet::NexSessionSearchCriteria::SetMatchmakeSystemType(u8 type)
{
    m_MatchmakeSystemType = type;
    m_Flags |= FLAG_MATCHMAKE_SYSTEM_TYPE;
}

// 0x0040B358 | libgarden [tier A]
void nn::pia::inet::NexSessionSearchCriteria::SetMaxParticipants(unsigned short num)
{
    m_MaxParticipants[0] = num;
    m_MaxParticipants[1] = num;
    m_Flags |= FLAG_MAX_PARTICIPANTS;
}

// 0x0040B378 | libgarden [tier A]
void nn::pia::inet::NexSessionSearchCriteria::SetMinParticipants(unsigned short num)
{
    m_MinParticipants[0] = num;
    m_MinParticipants[1] = num;
    m_Flags |= FLAG_MIN_PARTICIPANTS;
}

// 0x0040B398 (name is ours)
void nn::pia::inet::NexSessionSearchCriteria::ConvertTo(nex::MatchmakeSessionSearchCriteria* pCriteria, nex::MatchmakeParam* pParam,
                                                        nex::ResultRange* pRange, u32 index) const
{
    common::SessionBeginMonitoringContent& content = common::g_SessionBeginMonitoringContent;
    if (common::IsValidPointer(pCriteria)) {
        if (m_Flags & FLAG_GAME_MODE) {
            pCriteria->SetGameMode(m_GameMode);
        }
        pCriteria->SetMatchmakeSystemType(NexMatchmakeSession::ConvertMatchmakeSystemType(m_MatchmakeSystemType));
        if ((m_Flags & FLAG_MIN_PARTICIPANTS) && m_MinParticipants[1] != 0xFFFF && m_MinParticipants[0] != 0xFFFF) {
            pCriteria->SetMinParticipants(m_MinParticipants[1], m_MinParticipants[0]);
        }
        if ((m_Flags & FLAG_MAX_PARTICIPANTS) && m_MaxParticipants[1] != 0xFFFF && m_MaxParticipants[0] != 0xFFFF) {
            pCriteria->SetMaxParticipants(m_MaxParticipants[1], m_MaxParticipants[0]);
        }
        if (m_Flags & FLAG_OPENED_ONLY) {
            pCriteria->m_Unknown0x35 = m_IsOpenedOnly;
        }
        if (m_Flags & FLAG_VACANT_ONLY) {
            pCriteria->SetVacantOnly(m_IsVacantOnly);
        }
        if (m_Flags & FLAG_UNKNOWN_0x9D2) {
            pCriteria->m_Unknown0x36 = m_Unknown0x9D2;
        }
        if (m_Flags & FLAG_UNKNOWN_0x9D3) {
            pCriteria->m_Unknown0x64 = m_Unknown0x9D3;
        }
        if (m_Flags & FLAG_UNKNOWN_0xA10) {
            pCriteria->m_Unknown0x68 = m_Unknown0xA10;
        }
        if (m_Unknown0x9D4 == 2) {
            // the parameters of the session
            pParam->m_Params.Clear();
            if (m_Flags & FLAG_PARAM_SI) {
                pParam->SetParam(nex::String(nex::g_MatchmakeParamKeySI), nex::Variant(m_ParamSI));
            }
            if (m_Flags & FLAG_PARAM_RV) {
                pParam->SetParam(nex::String(nex::g_MatchmakeParamKeyRV), nex::Variant(m_ParamRV));
            }
            if (m_Flags & FLAG_PARAM_DR) {
                pParam->SetParam(nex::String(nex::g_MatchmakeParamKeyDR), nex::Variant(m_ParamDR));
            }
            if (m_Flags & FLAG_PARAM_VR) {
                pParam->SetParam(nex::String(nex::g_MatchmakeParamKeyVR), nex::Variant(m_ParamVR));
            }
            if (m_Flags & FLAG_PARAM_USGI) {
                pParam->SetParam(nex::String(nex::g_MatchmakeParamKeyUsGI), nex::Variant(m_ParamUsGI));
            }
            if (m_Flags & FLAG_PARAM_OIA) {
                nex::String value(m_ParamOIA);
                pParam->SetParam(nex::String(nex::g_MatchmakeParamKeyOIA), nex::Variant(value));
            }
            if (m_Flags & FLAG_PARAM_NCC) {
                pParam->SetParam(nex::String(nex::g_MatchmakeParamKeyNCC), nex::Variant(m_ParamNCC));
            }
            pCriteria->SetMatchmakeParam(NexMatchmakeSession::ConvertSelectionMethod(m_Unknown0x9D4), *pParam);
        } else {
            pCriteria->m_Unknown0x38 = NexMatchmakeSession::ConvertSelectionMethod(m_Unknown0x9D4);
        }
        // the monitoring data of the search (two blocks)
        u8 flagBits = (m_IsOpenedOnly << 2) + (m_Unknown0x9D2 << 1) + (m_Unknown0x9D3 + (m_IsVacantOnly << 3));
        switch (index) {
        case 0:
            if (m_Flags & FLAG_GAME_MODE) {
                content.m_Unknown0x1A4 = m_GameMode;
            }
            content.m_Unknown0x1A8 = m_MinParticipants[1];
            content.m_Unknown0x1A9 = m_MinParticipants[0];
            content.m_Unknown0x1AA = m_MaxParticipants[1];
            content.m_Unknown0x1AB = m_MaxParticipants[0];
            content.m_Unknown0x1E4 = m_MatchmakeSystemType;
            content.m_Unknown0x1E5 = flagBits;
            content.m_Unknown0x1E6 = m_Unknown0x9D4;
            content.m_Unknown0x23D = m_ParamUsGI;
            content.m_Unknown0x230 = m_ParamRV;
            content.m_Unknown0x238 = m_ParamDR;
            content.m_Unknown0x234 = m_ParamVR;
            content.m_Unknown0x23C = m_ParamNCC;
            break;
        case 1:
            if (m_Flags & FLAG_GAME_MODE) {
                content.m_Unknown0x1E8 = m_GameMode;
            }
            content.m_Unknown0x1EC = m_MinParticipants[1];
            content.m_Unknown0x1ED = m_MinParticipants[0];
            content.m_Unknown0x1EE = m_MaxParticipants[1];
            content.m_Unknown0x1EF = m_MaxParticipants[0];
            content.m_Unknown0x228 = m_MatchmakeSystemType;
            content.m_Unknown0x229 = flagBits;
            content.m_Unknown0x22A = m_Unknown0x9D4;
            content.m_Unknown0x24D = m_ParamUsGI;
            content.m_Unknown0x240 = m_ParamRV;
            content.m_Unknown0x248 = m_ParamDR;
            content.m_Unknown0x244 = m_ParamVR;
            content.m_Unknown0x24C = m_ParamNCC;
            break;
        }
        // the attributes: a range, one value or a list of values
        if (m_Flags & FLAG_ATTRIBUTE) {
            for (u32 i = 0; i < ATTRIBUTE_NUM; i++) {
                if (m_IsAttributeRange[i]) {
                    pCriteria->SetAttributeRange(i, m_AttributeMin[i], m_AttributeMax[i]);
                    switch (index) {
                    case 0:
                        content.m_Unknown0x1B4[i] = m_AttributeMin[i];
                        content.m_Unknown0x1CC[i] = m_AttributeMax[i];
                        break;
                    case 1:
                        content.m_Unknown0x1F8[i] = m_AttributeMin[i];
                        content.m_Unknown0x210[i] = m_AttributeMax[i];
                        break;
                    }
                    continue;
                }
                if (m_AttributeValueNum[i] == 1) {
                    pCriteria->SetAttribute(i, m_AttributeValues[i][0]);
                } else if (m_AttributeValueNum[i] > 1) {
                    nex::qVector<u32> values;
                    for (u32 j = 0; j < m_AttributeValueNum[i]; j++) {
                        values.push_back(m_AttributeValues[i][j]);
                    }
                    pCriteria->SetAttribute(i, values);
                }
                switch (index) {
                case 0:
                    content.m_Unknown0x1AC[i] = m_AttributeValueNum[i];
                    break;
                case 1:
                    content.m_Unknown0x1F0[i] = m_AttributeValueNum[i];
                    break;
                }
            }
        }
    }
    if (common::IsValidPointer(pRange)) {
        pRange->m_Offset = m_ResultOffset;
        pRange->m_Size = m_ResultNumMax >= RESULT_NUM_MAX ? RESULT_NUM_MAX : m_ResultNumMax;
    }
}

// 0x0040BC10 | libgarden [tier A]
nn::pia::inet::NexSessionSearchCriteria::NexSessionSearchCriteria()
    : m_IsOpenedOnly(true), m_IsVacantOnly(false), m_GameMode(0), m_MatchmakeSystemType(0), m_Unknown0x9D2(true), m_Unknown0x9D3(true),
      m_Unknown0x9D4(0), m_ParamSI(0), m_ParamRV(0), m_ParamDR(0), m_ParamVR(0), m_ParamNCC(0), m_ParamUsGI(false), m_Unknown0xA10(0), m_Flags(0)
{
    m_MinParticipants[0] = 0xFFFF;
    m_MinParticipants[1] = 0xFFFF;
    m_MaxParticipants[0] = 0xFFFF;
    m_MaxParticipants[1] = 0xFFFF;
    memset(m_ParamOIA, 0, sizeof(m_ParamOIA));
    for (s32 i = 0; i < static_cast<s32>(ATTRIBUTE_NUM); i++) {
        memset(m_AttributeValues[i], 0, sizeof(m_AttributeValues[i]));
        m_AttributeValueNum[i] = 0;
        m_AttributeMin[i] = 0;
        m_AttributeMax[i] = 0;
        m_IsAttributeRange[i] = false;
    }
    m_ResultNumMax = RESULT_NUM_MAX;
}

// 0x0040BCF8
// 0x0040BCF0 (deleting dtor)
nn::pia::inet::NexSessionSearchCriteria::~NexSessionSearchCriteria()
{
    // empty (in the original too)
}

// 0x0072F84C
void nn::pia::inet::NexSessionSearchCriteria::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace inet
} // namespace pia
} // namespace nn
