#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_Message.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"
#include "nn/nex/nex__DDL_JoinMatchmakeSessionParam.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::_DDL_JoinMatchmakeSessionParam::_DDL_JoinMatchmakeSessionParam()
{
}

// 0x003C4A48 slot 0x00
// 0x003C4A10 (deleting dtor)
nn::nex::_DDL_JoinMatchmakeSessionParam::~_DDL_JoinMatchmakeSessionParam()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003C48B8 (name is ours)
void nn::nex::_DDL_JoinMatchmakeSessionParam::Add(nn::nex::Message* pMessage, const nn::nex::_DDL_JoinMatchmakeSessionParam& data)
{
    bool isWithoutHeader = pMessage->m_IsWithoutStructureHeader;
    u32 start = 0;
    if (!isWithoutHeader) {
        start = DDLStructure::BeginAdd(pMessage, data.m_StructureVersion, REVISION);
    }
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_GatheringId), sizeof(u32));
    _Type_qlist<_Type_uint32>::Add(pMessage, data.m_AdditionalParticipants);
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_GatheringIdForParticipationCheck), sizeof(u32));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_JoinMatchmakeSessionOption), sizeof(u32));
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Unknown0x2C), sizeof(u8));
    _Type_string::Add(pMessage, data.m_Unknown0x30);
    _Type_string::Add(pMessage, data.m_Unknown0x38);
    _Type_string::Add(pMessage, data.m_JoinMessage);
    pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_ParticipationCount), sizeof(u16));
    if (!isWithoutHeader) {
        if (data.m_StructureVersion >= 1) {
            pMessage->AppendRaw(reinterpret_cast<const u8*>(&data.m_Unknown0x4A), sizeof(u16));
        }
        DDLStructure::EndAdd(pMessage, start);
    }
}

} // namespace nex
} // namespace nn
