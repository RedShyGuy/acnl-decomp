#pragma once

#include "decomp.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_MatchmakeParam.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_qVector.h"

namespace nn {
namespace nex {
// The DDL part of MatchmakeSessionSearchCriteria (MatchmakeExtension), with three revisions (1: the
// MatchmakeParam, 2: two flags, 3: a value). The binary has no DDL declaration of it; the layout is
// from MatchmakeSessionSearchCriteria::MatchmakeSessionSearchCriteria and its setters, the names
// are ours.
// RTTI N2nn3nex35_DDL_MatchmakeSessionSearchCriteriaE @ 0x008CF384
// vtable 0x008FF724 (vptr 0x008FF72C), offset_to_top 0, 2 entries
class _DDL_MatchmakeSessionSearchCriteria : public ::nn::nex::RootObject
{
public:
    // the highest revision of the structure
    static const u8 REVISION = 3;

    // (inline)
    _DDL_MatchmakeSessionSearchCriteria() : m_StructureVersion(REVISION), m_Attributes(0) {}
    virtual ~_DDL_MatchmakeSessionSearchCriteria(); // 0x003CBBCC slot 0x00 | fefates:callseq
    // 0x003CBB58 slot 0x04 (deleting dtor)
    nn::nex::_DDL_MatchmakeSessionSearchCriteria& operator=(const nn::nex::_DDL_MatchmakeSessionSearchCriteria& other); // 0x003CBC3C

    static void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_MatchmakeSessionSearchCriteria& data); // 0x003CB984 | fefates:bytes [tier B]
    // (inline; no instance in the binary)
    static DECOMP_ALWAYS_INLINE void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_MatchmakeSessionSearchCriteria* pData)
    {
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
        _Type_qvector<_Type_string>::Extract(pMessage, &pData->m_Attributes);
        if (pMessage->m_IsError) {
            return;
        }
        _Type_string::Extract(pMessage, &pData->m_GameMode);
        if (pMessage->m_IsError) {
            return;
        }
        _Type_string::Extract(pMessage, &pData->m_MinParticipants);
        if (pMessage->m_IsError) {
            return;
        }
        _Type_string::Extract(pMessage, &pData->m_MaxParticipants);
        if (pMessage->m_IsError) {
            return;
        }
        _Type_string::Extract(pMessage, &pData->m_MatchmakeSystemType);
        if (pMessage->m_IsError) {
            return;
        }
        *pMessage >> pData->m_IsVacantOnly;
        if (pMessage->m_IsError) {
            return;
        }
        *pMessage >> pData->m_Unknown0x35;
        if (pMessage->m_IsError) {
            return;
        }
        *pMessage >> pData->m_Unknown0x36;
        if (pMessage->m_IsError) {
            return;
        }
        pMessage->ExtractPrimitive(&pData->m_Unknown0x38);
        if (pMessage->m_IsError) {
            return;
        }
        pMessage->ExtractPrimitive(&pData->m_Unknown0x3C);
        if (pMessage->m_IsError) {
            return;
        }
        if (pData->m_StructureVersion >= 1) {
            DDLClassMetaType<MatchmakeParam>::Extract(pMessage, &pData->m_MatchmakeParam);
            if (pMessage->m_IsError) {
                return;
            }
            if (pData->m_StructureVersion >= 2) {
                *pMessage >> pData->m_Unknown0x64;
                if (pMessage->m_IsError) {
                    return;
                }
                *pMessage >> pData->m_Unknown0x65;
                if (pMessage->m_IsError) {
                    return;
                }
                if (pData->m_StructureVersion >= 3) {
                    pMessage->ExtractPrimitive(&pData->m_Unknown0x68);
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

    u8 m_StructureVersion;              // 0x04
    qVector<String> m_Attributes;       // 0x08, a string per attribute ("" or "%u" or "%u,%u")
    String m_GameMode;                  // 0x14
    String m_MinParticipants;           // 0x1C
    String m_MaxParticipants;           // 0x24
    String m_MatchmakeSystemType;       // 0x2C
    bool m_IsVacantOnly;                // 0x34
    bool m_Unknown0x35;                 // 0x35
    bool m_Unknown0x36;                 // 0x36
    u32 m_Unknown0x38;                  // 0x38
    u16 m_Unknown0x3C;                  // 0x3C, 1 with m_IsVacantOnly
    // revision 1
    MatchmakeParam m_MatchmakeParam;    // 0x40
    // revision 2
    bool m_Unknown0x64;                 // 0x64
    bool m_Unknown0x65;                 // 0x65
    // revision 3
    u32 m_Unknown0x68;                  // 0x68
};
ASSERT_OFFSET(_DDL_MatchmakeSessionSearchCriteria, m_GameMode, 0x14);
ASSERT_OFFSET(_DDL_MatchmakeSessionSearchCriteria, m_MatchmakeParam, 0x40);
ASSERT_SIZE(_DDL_MatchmakeSessionSearchCriteria, 0x6C);
} // namespace nex
} // namespace nn
