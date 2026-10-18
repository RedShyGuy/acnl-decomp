#include "nn/nex/nex__DDL_MatchmakeSessionSearchCriteria.h"

namespace nn {
namespace nex {
// 0x003CBC3C
nn::nex::_DDL_MatchmakeSessionSearchCriteria& nn::nex::_DDL_MatchmakeSessionSearchCriteria::operator=(const nn::nex::_DDL_MatchmakeSessionSearchCriteria& other)
{
    m_StructureVersion = other.m_StructureVersion;
    m_Attributes = other.m_Attributes;
    m_GameMode = other.m_GameMode;
    m_MinParticipants = other.m_MinParticipants;
    m_MaxParticipants = other.m_MaxParticipants;
    m_MatchmakeSystemType = other.m_MatchmakeSystemType;
    m_IsVacantOnly = other.m_IsVacantOnly;
    m_Unknown0x35 = other.m_Unknown0x35;
    m_Unknown0x36 = other.m_Unknown0x36;
    m_Unknown0x38 = other.m_Unknown0x38;
    m_Unknown0x3C = other.m_Unknown0x3C;
    m_MatchmakeParam = other.m_MatchmakeParam;
    m_Unknown0x64 = other.m_Unknown0x64;
    m_Unknown0x65 = other.m_Unknown0x65;
    m_Unknown0x68 = other.m_Unknown0x68;
    return *this;
}

// 0x003CBBCC slot 0x00 | fefates:callseq
// 0x003CBB58 (deleting dtor)
nn::nex::_DDL_MatchmakeSessionSearchCriteria::~_DDL_MatchmakeSessionSearchCriteria()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003CB984 | fefates:bytes [tier B]
void nn::nex::_DDL_MatchmakeSessionSearchCriteria::Add(nn::nex::Message* pMessage, const nn::nex::_DDL_MatchmakeSessionSearchCriteria& data)
{
    bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
    u32 start = 0;
    if (!isWithoutHeader) {
        start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion, REVISION);
    }
    _Type_qvector<_Type_string>::Add(pMessage, data.m_Attributes);
    _Type_string::Add(pMessage, data.m_GameMode);
    _Type_string::Add(pMessage, data.m_MinParticipants);
    _Type_string::Add(pMessage, data.m_MaxParticipants);
    _Type_string::Add(pMessage, data.m_MatchmakeSystemType);
    *pMessage << data.m_IsVacantOnly;
    *pMessage << data.m_Unknown0x35;
    *pMessage << data.m_Unknown0x36;
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Unknown0x38), sizeof(u32));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Unknown0x3C), sizeof(u16));
    if (!isWithoutHeader) {
        if (data.m_StructureVersion >= 1) {
            DDLClassMetaType<MatchmakeParam>::Add(pMessage, data.m_MatchmakeParam);
            if (data.m_StructureVersion >= 2) {
                *pMessage << data.m_Unknown0x64;
                *pMessage << data.m_Unknown0x65;
                if (data.m_StructureVersion >= 3) {
                    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Unknown0x68), sizeof(u32));
                }
            }
        }
        DDLStructure::EndAdd(pMessage, start);
    }
}

} // namespace nex
} // namespace nn
