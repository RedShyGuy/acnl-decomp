#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_Message.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"
#include "nn/nex/nex__DDL_UpdateMatchmakeSessionParam.h"

namespace nn {
namespace nex {
// 0x003CB0B8 slot 0x00
// 0x003CB074 (deleting dtor)
nn::nex::_DDL_UpdateMatchmakeSessionParam::~_DDL_UpdateMatchmakeSessionParam()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003CAE2C (name is ours)
void nn::nex::_DDL_UpdateMatchmakeSessionParam::Add(nn::nex::Message* pMessage, const nn::nex::_DDL_UpdateMatchmakeSessionParam& data)
{
    bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
    u32 start = 0;
    if (!isWithoutHeader) {
        start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion, REVISION);
    }
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_GatheringId), sizeof(u32));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_ModificationFlags), sizeof(u32));
    _Type_qvector<_Type_uint32>::AddInline(pMessage, data.m_Attributes);
    *pMessage << data.m_IsOpenParticipation;
    _Type_qvector<_Type_byte>::AddInline(pMessage, data.m_ApplicationBuffer);
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_ProgressScore), sizeof(u8));
    _DDL_MatchmakeParam::Add(pMessage, data.m_MatchmakeParam);
    u64 startedTime = data.m_StartedTime;
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&startedTime), sizeof(startedTime));
    _Type_string::Add(pMessage, data.m_UserPassword);
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_GameMode), sizeof(u32));
    _Type_string::Add(pMessage, data.m_Description);
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_MinParticipants), sizeof(u16));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_MaxParticipants), sizeof(u16));
    if (!isWithoutHeader) {
        if (data.m_StructureVersion >= 1) {
            pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_MatchmakeSystemType), sizeof(u32));
            pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_ParticipationPolicy), sizeof(u32));
            pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_PolicyArgument), sizeof(u32));
        }
        DDLStructure::EndAdd(pMessage, start);
    }
}

} // namespace nex
} // namespace nn
