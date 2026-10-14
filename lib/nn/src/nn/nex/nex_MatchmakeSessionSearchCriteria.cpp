#include "nn/nex/nex__DDL_MatchmakeSessionSearchCriteria.h"
#include "nn/nex/nex_MatchmakeSessionSearchCriteria.h"

namespace nn {
namespace nex {
// 0x003C3034 slot 0x00 | fefates:callseq
// 0x003C2FC0 (deleting dtor)
nn::nex::MatchmakeSessionSearchCriteria::~MatchmakeSessionSearchCriteria()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003C2B8C | fefates:bytes [tier B]
void nn::nex::MatchmakeSessionSearchCriteria::SetMaxParticipants(u16 value1, u16 value2)
{
    m_MaxParticipants.Format(L"%u,%u", value1, value2);
}

// 0x003C2BAC | fefates:bytes [tier B]
void nn::nex::MatchmakeSessionSearchCriteria::SetMinParticipants(u16 value1, u16 value2)
{
    m_MinParticipants.Format(L"%u,%u", value1, value2);
}

// 0x003C2A58 (name is ours)
void nn::nex::MatchmakeSessionSearchCriteria::SetGameMode(u32 gameMode)
{
    m_GameMode.Format(L"%u", gameMode);
}

// 0x003C2A6C (name is ours)
bool nn::nex::MatchmakeSessionSearchCriteria::SetAttribute(u32 index, const qVector<u32>& values)
{
    if (values.begin() == values.end() || values.size() > MAX_ATTRIBUTE_VALUES) {
        return false;
    }
    // the values separated by '|'
    const u32* p = values.begin();
    m_Attributes[index].Format(L"%u", *p);
    for (p++; p != values.end(); p++) {
        String value(L"");
        value.Format(L"|%u", *p);
        m_Attributes[index] += value;
    }
    return true;
}

// 0x003C2B34 (name is ours)
void nn::nex::MatchmakeSessionSearchCriteria::SetAttribute(u32 index, u32 value)
{
    m_Attributes[index].Format(L"%u", value);
}

// 0x003C2B4C (name is ours)
void nn::nex::MatchmakeSessionSearchCriteria::SetVacantOnly(bool isVacantOnly)
{
    m_IsVacantOnly = isVacantOnly;
    m_Unknown0x3C = 1;
}

// 0x003C2B5C (name is ours)
void nn::nex::MatchmakeSessionSearchCriteria::SetAttributeRange(u32 index, u32 min, u32 max)
{
    m_Attributes[index].Format(L"%u,%u", min, max);
}

// 0x003C2B78 (name is ours)
void nn::nex::MatchmakeSessionSearchCriteria::SetMatchmakeSystemType(u32 type)
{
    m_MatchmakeSystemType.Format(L"%u", type);
}

// 0x00395238 (name is ours)
void nn::nex::MatchmakeSessionSearchCriteria::SetMatchmakeParam(u32 value, const MatchmakeParam& param)
{
    m_Unknown0x38 = value;
    m_MatchmakeParam = param;
}

// 0x003C2BCC | fefates:bytes [tier B]
void nn::nex::MatchmakeSessionSearchCriteria::Reset()
{
    String empty(L"");
    m_Attributes.clear();
    m_Attributes.ResizeInline(DEFAULT_ATTRIBUTE_COUNT, empty);
    m_GameMode = L"";
    m_MinParticipants = L"";
    m_MaxParticipants = L"";
    m_MatchmakeSystemType = L"";
    m_IsVacantOnly = false;
    m_Unknown0x35 = true;
    m_Unknown0x36 = true;
    m_Unknown0x64 = true;
    m_Unknown0x65 = true;
    m_Unknown0x38 = 0;
    m_Unknown0x3C = 1;
    m_Unknown0x68 = 0;
}

// 0x003C2F18 | fefates:bytes [tier B]
nn::nex::MatchmakeSessionSearchCriteria::MatchmakeSessionSearchCriteria()
{
    Reset();
}

// 0x0082FDB0 (name is ours)
template <>
DECOMP_NOINLINE void qList<MatchmakeSessionSearchCriteria>::clear()
{
    ClearInline();
}

} // namespace nex
} // namespace nn
