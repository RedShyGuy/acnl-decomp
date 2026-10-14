#include "nn/nex/nex__DDL_MatchmakeSession.h"
#include "nn/nex/nex_MatchmakeSession.h"

namespace nn {
namespace nex {
// 0x0039BC30 slot 0x00 | fefates:bytes
// 0x0039BC20 (deleting dtor)
nn::nex::_DDL_MatchmakeSession::~_DDL_MatchmakeSession()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x0072D008 slot 0x08 | fefates:callseq
nn::nex::Gathering* nn::nex::_DDL_MatchmakeSession::Clone() const
{
    // (a new one, nothing is copied)
    return new MatchmakeSession();
}

// 0x0072CFA8 slot 0x0C | mk7dlp:bytes
nn::nex::String nn::nex::_DDL_MatchmakeSession::GetGatheringType() const
{
    return String(L"MatchmakeSession");
}

// 0x0072CFD4 slot 0x10
bool nn::nex::_DDL_MatchmakeSession::IsA(const nn::nex::String& typeName) const
{
    return String::IsEqual(typeName.m_pString, L"MatchmakeSession");
}

// 0x0072D028 slot 0x14
bool nn::nex::_DDL_MatchmakeSession::IsAKindOf(const nn::nex::String& typeName) const
{
    return String::IsEqual(typeName.m_pString, L"MatchmakeSession") || Gathering::IsAKindOf(typeName);
}

// 0x0039B71C slot 0x18
void nn::nex::_DDL_MatchmakeSession::StreamIn(nn::nex::Message* pMessage) const
{
    Add(pMessage, *this);
}

// 0x0039B72C | fefates:bytes [tier B]
void nn::nex::_DDL_MatchmakeSession::Add(nn::nex::Message* pMessage, const nn::nex::_DDL_MatchmakeSession& data)
{
    _DDL_Gathering::Add(pMessage, data);
    bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
    u32 start = 0;
    if (!isWithoutHeader) {
        start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion, REVISION);
    }
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_GameMode), sizeof(u32));
    _Type_qvector<_Type_uint32>::Add(pMessage, data.m_Attributes);
    *pMessage << data.m_IsOpenParticipation;
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_MatchmakeSystemType), sizeof(u32));
    _Type_qvector<_Type_byte>::Add(pMessage, data.m_ApplicationBuffer);
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_ParticipationCount), sizeof(u32));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_ProgressScore), sizeof(u8));
    _Type_qvector<_Type_byte>::Add(pMessage, data.m_SessionKey);
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Unknown0x6C), sizeof(u32));
    if (!isWithoutHeader) {
        if (data.m_StructureVersion >= 1) {
            DDLClassMetaType<MatchmakeParam>::Add(pMessage, data.m_MatchmakeParam);
            u64 startedTime = data.m_StartedTime;
            pMessage->AppendRaw(reinterpret_cast<const u8*>(&startedTime), sizeof(u64));
            if (data.m_StructureVersion >= 2) {
                _Type_string::Add(pMessage, data.m_UserPassword);
                if (data.m_StructureVersion >= 3) {
                    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_ReferGatheringId), sizeof(u32));
                    *pMessage << data.m_IsUserPasswordEnabled;
                    *pMessage << data.m_IsSystemPasswordEnabled;
                }
            }
        }
        DDLStructure::EndAdd(pMessage, start);
    }
}

// 0x0039B9B8 slot 0x1C
void nn::nex::_DDL_MatchmakeSession::StreamOut(nn::nex::Message* pMessage)
{
    Extract(pMessage, this);
}

// 0x0039B9C8 | fefates:bytes [tier B]
void nn::nex::_DDL_MatchmakeSession::Extract(nn::nex::Message* pMessage, nn::nex::_DDL_MatchmakeSession* pData)
{
    _DDL_Gathering::Extract(pMessage, pData);
    if (pMessage->m_IsError) {
        return;
    }
    pData->m_StructureVersion = 0;
    bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
    u32 size = 0;
    if (!isWithoutHeader && !DDLStructure::BeginExtract(pMessage, &pData->m_StructureVersion, &size)) {
        return;
    }
    if (pMessage->m_IsError) {
        return;
    }
    u32 start = pMessage->m_Position;
    pMessage->ExtractPrimitive(&pData->m_GameMode);
    if (pMessage->m_IsError) {
        return;
    }
    _Type_qvector<_Type_uint32>::Extract(pMessage, &pData->m_Attributes);
    if (pMessage->m_IsError) {
        return;
    }
    *pMessage >> pData->m_IsOpenParticipation;
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_MatchmakeSystemType);
    if (pMessage->m_IsError) {
        return;
    }
    _Type_qvector<_Type_byte>::Extract(pMessage, &pData->m_ApplicationBuffer);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_ParticipationCount);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_ProgressScore);
    if (pMessage->m_IsError) {
        return;
    }
    _Type_qvector<_Type_byte>::Extract(pMessage, &pData->m_SessionKey);
    if (pMessage->m_IsError) {
        return;
    }
    pMessage->ExtractPrimitive(&pData->m_Unknown0x6C);
    if (pMessage->m_IsError) {
        return;
    }
    if (pData->m_StructureVersion >= 1) {
        DDLClassMetaType<MatchmakeParam>::Extract(pMessage, &pData->m_MatchmakeParam);
        if (pMessage->m_IsError) {
            return;
        }
        {
            u64 value;
            pMessage->ExtractPrimitive(&value);
            pData->m_StartedTime = DateTime(value);
        }
        if (pMessage->m_IsError) {
            return;
        }
        if (pData->m_StructureVersion >= 2) {
            _Type_string::Extract(pMessage, &pData->m_UserPassword);
            if (pMessage->m_IsError) {
                return;
            }
            if (pData->m_StructureVersion >= 3) {
                pMessage->ExtractPrimitive(&pData->m_ReferGatheringId);
                if (pMessage->m_IsError) {
                    return;
                }
                *pMessage >> pData->m_IsUserPasswordEnabled;
                if (pMessage->m_IsError) {
                    return;
                }
                *pMessage >> pData->m_IsSystemPasswordEnabled;
                if (pMessage->m_IsError) {
                    return;
                }
            }
        }
    }
    if (!isWithoutHeader) {
        DDLStructure::EndExtract(pMessage, start, size);
    }
}

// 0x0039BC74 (name is ours)
nn::nex::_DDL_MatchmakeSession& nn::nex::_DDL_MatchmakeSession::operator=(const nn::nex::_DDL_MatchmakeSession& other)
{
    _DDL_Gathering::operator=(other);
    m_StructureVersion = other.m_StructureVersion;
    m_GameMode = other.m_GameMode;
    m_Attributes = other.m_Attributes;
    m_IsOpenParticipation = other.m_IsOpenParticipation;
    m_MatchmakeSystemType = other.m_MatchmakeSystemType;
    m_ApplicationBuffer = other.m_ApplicationBuffer;
    m_ParticipationCount = other.m_ParticipationCount;
    m_ProgressScore = other.m_ProgressScore;
    m_SessionKey = other.m_SessionKey;
    m_Unknown0x6C = other.m_Unknown0x6C;
    m_MatchmakeParam = other.m_MatchmakeParam;
    m_StartedTime = other.m_StartedTime;
    m_UserPassword = other.m_UserPassword;
    m_ReferGatheringId = other.m_ReferGatheringId;
    m_IsUserPasswordEnabled = other.m_IsUserPasswordEnabled;
    m_IsSystemPasswordEnabled = other.m_IsSystemPasswordEnabled;
    return *this;
}

// 0x0072B750 (name is ours)
u32 nn::nex::_DDL_MatchmakeSession::GetAttribute(u32 index) const
{
    return m_Attributes[index];
}

} // namespace nex
} // namespace nn
