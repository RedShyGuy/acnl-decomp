#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_Message.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"
#include "nn/nex/nex__DDL_AutoMatchmakeParam.h"

namespace nn {
namespace nex {
// 0x003B1A2C slot 0x00
// 0x003B1A1C (deleting dtor)
nn::nex::_DDL_AutoMatchmakeParam::~_DDL_AutoMatchmakeParam()
{
    // nothing to do
}

// 0x003B18C4 (name is ours)
void nn::nex::_DDL_AutoMatchmakeParam::Add(nn::nex::Message* pMessage, const nn::nex::_DDL_AutoMatchmakeParam& data)
{
    bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
    u32 start = 0;
    if (!isWithoutHeader) {
        start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion);
    }
    _DDL_MatchmakeSession::Add(pMessage, data.m_SourceMatchmakeSession);
    _Type_qlist<_Type_uint32>::Add(pMessage, data.m_AdditionalParticipants);
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_GatheringIdForParticipationCheck), sizeof(u32));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_AutoMatchmakeOption), sizeof(u32));
    _Type_string::Add(pMessage, data.m_JoinMessage);
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_ParticipationCount), sizeof(u16));
    _Type_qlist<DDLClassMetaType<MatchmakeSessionSearchCriteria> >::AddInline(pMessage, data.m_SearchCriteria);
    _Type_qlist<_Type_uint32>::Add(pMessage, data.m_TargetGatherings);
    if (!isWithoutHeader) {
        DDLStructure::EndAdd(pMessage, start);
    }
}

} // namespace nex
} // namespace nn
