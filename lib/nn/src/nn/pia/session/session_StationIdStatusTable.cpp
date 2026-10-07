#include "nn/pia/session/session_StationIdStatusTable.h"
#include "nn/pia/common/common_HeapManager.h"
#include "nn/pia/common/common_Result.h"
#include "pead/peadHeapMgr.h"

namespace nn {
namespace pia {
namespace session {
namespace {
typedef common::ObjList<StationIdStatusTable::Entry>::Node Node;
} // namespace

// 0x0043E2EC (name is ours)
nn::Result nn::pia::session::StationIdStatusTable::Add(const nn::pia::StationId& stationId, u32 sessionId, u8 status, bool isNotified)
{
    if (m_List.GetFreeCount() == 0) {
        return common::RESULT_NO_FREE_ENTRY;
    }
    Entry* pEntry = m_List.PushBackNew();
    if (pEntry != nullptr) {
        pEntry->m_StationId = stationId;
        pEntry->m_Status = status;
        pEntry->m_IsNotified = isNotified;
        pEntry->m_IsValid = true;
        pEntry->m_Unknown0x1 = 0;
        pEntry->m_SessionId = sessionId;
        pEntry->m_Unknown0x10 = 0;
        pEntry->m_Unknown0x12 = false;
        pEntry->m_Unknown0x13 = false;
    }
    return nn::Result();
}

// 0x0043E3B4 (name is ours)
nn::Result nn::pia::session::StationIdStatusTable::Initialize(u32 stationNum)
{
    if (stationNum == 0 || stationNum > STATION_INDEX_MAX + 1) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (m_pBuffer == nullptr) {
        // a value initialized u64 array, so the nodes are 8 byte aligned
        u32 wordNum = (stationNum * sizeof(Node) + 7) / 8;
        u64* pBuffer = static_cast<u64*>(pead::AllocMemory(wordNum * 8, common::HeapManager::GetHeap()));
        if (pBuffer != nullptr) {
            for (u32 i = 0; i < wordNum; i++) {
                ::new (&pBuffer[i]) u64();
            }
        }
        m_pBuffer = pBuffer;
        m_List.Initialize(reinterpret_cast<Node*>(pBuffer), stationNum);
    }
    return nn::Result();
}

// 0x0043E4B0 (name is ours)
void nn::pia::session::StationIdStatusTable::SetValid(const nn::pia::StationId& stationId, bool isValid)
{
    for (Node* node = m_List.Begin(); node != m_List.End(); node = m_List.Advance(node)) {
        if (node->m_Value.m_StationId == stationId) {
            node->m_Value.m_IsValid = isValid;
        }
    }
}

// 0x0043E500 (name is ours)
nn::pia::session::StationIdStatusTable::Entry* nn::pia::session::StationIdStatusTable::Find(const nn::pia::StationId& stationId)
{
    for (Node* node = m_List.Begin(); node != m_List.End(); node = m_List.Advance(node)) {
        if (node->m_Value.m_StationId == stationId) {
            return &node->m_Value;
        }
    }
    return nullptr;
}

// 0x0043E560 (name is ours)
nn::Result nn::pia::session::StationIdStatusTable::Remove(const nn::pia::StationId& stationId)
{
    for (Node* node = m_List.Begin(); node != m_List.End(); node = m_List.Advance(node)) {
        if (node->m_Value.m_StationId == stationId) {
            Entry* pEntry = &node->m_Value;
            pEntry->m_Unknown0x1 = 0;
            pEntry->m_IsValid = false;
            pEntry->m_Unknown0x12 = false;
            pEntry->m_Unknown0x13 = false;
            pEntry->m_IsNotified = false;
            pEntry->m_Unknown0x10 = 0;
            pEntry->m_Status = 0;
            pEntry->m_SessionId = 0;
            pEntry->m_StationId = GetStationIdOfIndex253();
            pEntry->m_StationIndex = STATION_INDEX_UNIDENTIFIED;
            m_List.Erase(pEntry);
            return nn::Result();
        }
    }
    return common::RESULT_NOT_FOUND;
}

// 0x0043E644 (name is ours)
void nn::pia::session::StationIdStatusTable::SetUnknown0x10(const nn::pia::StationId& stationId, u8 value)
{
    for (Node* node = m_List.Begin(); node != m_List.End(); node = m_List.Advance(node)) {
        if (node->m_Value.m_StationId == stationId) {
            node->m_Value.m_Unknown0x10 = value;
            return;
        }
    }
}

// 0x0043E6A0 (name is ours)
void nn::pia::session::StationIdStatusTable::SetStationIndex(const nn::pia::StationId& stationId, nn::pia::StationIndex stationIndex)
{
    for (Node* node = m_List.Begin(); node != m_List.End(); node = m_List.Advance(node)) {
        if (node->m_Value.m_StationId == stationId) {
            node->m_Value.m_StationIndex = stationIndex;
            return;
        }
    }
}

// 0x0043E6FC (name is ours)
void nn::pia::session::StationIdStatusTable::Clear()
{
    m_List.Clear();
}

// 0x0043E754 (name is ours)
void nn::pia::session::StationIdStatusTable::SetStatus(const nn::pia::StationId& stationId, bool isTwo)
{
    for (Node* node = m_List.Begin(); node != m_List.End(); node = m_List.Advance(node)) {
        if (node->m_Value.m_StationId == stationId) {
            node->m_Value.m_Status = isTwo ? 2 : 1;
            return;
        }
    }
}

// 0x0043E7C0 (name is ours)
u8 nn::pia::session::StationIdStatusTable::GetUnknown0x1(const nn::pia::StationId& stationId)
{
    for (Node* node = m_List.Begin(); node != m_List.End(); node = m_List.Advance(node)) {
        if (node->m_Value.m_StationId == stationId) {
            return node->m_Value.m_Unknown0x1;
        }
    }
    return 0;
}

// 0x0043E820 (name is ours)
void nn::pia::session::StationIdStatusTable::ClearUnknown0x1()
{
    for (Node* node = m_List.Begin(); node != m_List.End(); node = m_List.Advance(node)) {
        node->m_Value.m_Unknown0x1 = 0;
    }
}

// 0x0043E84C (name is ours)
void nn::pia::session::StationIdStatusTable::ClearValid()
{
    for (Node* node = m_List.Begin(); node != m_List.End(); node = m_List.Advance(node)) {
        node->m_Value.m_IsValid = false;
    }
}

// 0x0043E878 (name is ours)
void nn::pia::session::StationIdStatusTable::ClearStatus()
{
    for (Node* node = m_List.Begin(); node != m_List.End(); node = m_List.Advance(node)) {
        node->m_Value.m_Status = 0;
    }
}

// 0x0043E8A4 (name is ours)
void nn::pia::session::StationIdStatusTable::SetNotified(const nn::pia::StationId& stationId, bool isNotified)
{
    for (Node* node = m_List.Begin(); node != m_List.End(); node = m_List.Advance(node)) {
        if (node->m_Value.m_StationId == stationId) {
            node->m_Value.m_IsNotified = isNotified;
        }
    }
}

// 0x0043E8F4 (name is ours)
bool nn::pia::session::StationIdStatusTable::SetUnknown0x12(const nn::pia::StationId& stationId, bool value)
{
    for (Node* node = m_List.Begin(); node != m_List.End(); node = m_List.Advance(node)) {
        if (node->m_Value.m_StationId == stationId) {
            node->m_Value.m_Unknown0x12 = value;
            return true;
        }
    }
    return false;
}

// 0x0043E958 (name is ours)
void nn::pia::session::StationIdStatusTable::RemoveLostStations()
{
    // with Session (written with it)
}

// 0x0043EAD8 (name is ours)
bool nn::pia::session::StationIdStatusTable::SetUnknown0x13(const nn::pia::StationId& stationId, bool value)
{
    for (Node* node = m_List.Begin(); node != m_List.End(); node = m_List.Advance(node)) {
        if (node->m_Value.m_StationId == stationId) {
            node->m_Value.m_Unknown0x13 = value;
            return true;
        }
    }
    return false;
}

// 0x0043EB3C (name is ours)
void nn::pia::session::StationIdStatusTable::NotifyStations()
{
    // with Session (written with it)
}

// 0x0043EC94 (name is ours)
void nn::pia::session::StationIdStatusTable::ClearUnknown0x12()
{
    for (Node* node = m_List.Begin(); node != m_List.End(); node = m_List.Advance(node)) {
        node->m_Value.m_Unknown0x12 = false;
    }
}

// 0x0043ECC0 (name is ours)
void nn::pia::session::StationIdStatusTable::ClearUnknown0x13()
{
    for (Node* node = m_List.Begin(); node != m_List.End(); node = m_List.Advance(node)) {
        node->m_Value.m_Unknown0x13 = false;
    }
}

// 0x0043ECEC (name is ours)
bool nn::pia::session::StationIdStatusTable::SetUnknown0x1(const nn::pia::StationId& stationId, bool isOne)
{
    for (Node* node = m_List.Begin(); node != m_List.End(); node = m_List.Advance(node)) {
        if (node->m_Value.m_StationId == stationId) {
            node->m_Value.m_Unknown0x1 = isOne ? 1 : 2;
            return true;
        }
    }
    return false;
}

// 0x0043ED60
nn::pia::session::StationIdStatusTable::StationIdStatusTable() : m_pBuffer(nullptr)
{
}

// 0x0043EE4C
// 0x0043EDB4 (deleting dtor)
nn::pia::session::StationIdStatusTable::~StationIdStatusTable()
{
    if (m_pBuffer != nullptr) {
        m_List.Clear();
        if (m_pBuffer != nullptr) {
            pead::FreeMemory(m_pBuffer);
        }
        m_pBuffer = nullptr;
    }
}

// 0x007339EC (name is ours)
bool nn::pia::session::StationIdStatusTable::GetSessionId(const nn::pia::StationId& stationId, u32* pSessionId) const
{
    for (Node* node = m_List.Begin(); node != m_List.End(); node = m_List.Advance(node)) {
        if (node->m_Value.m_StationId == stationId) {
            *pSessionId = node->m_Value.m_SessionId;
            return true;
        }
    }
    return false;
}

// 0x00733A54 (name is ours)
bool nn::pia::session::StationIdStatusTable::GetUnknown0x10(const nn::pia::StationId& stationId, u8* pValue) const
{
    for (Node* node = m_List.Begin(); node != m_List.End(); node = m_List.Advance(node)) {
        if (node->m_Value.m_StationId == stationId) {
            *pValue = node->m_Value.m_Unknown0x10;
            return true;
        }
    }
    return false;
}

// 0x00733ABC (name is ours)
u16 nn::pia::session::StationIdStatusTable::GetStationNum(u32 sessionId) const
{
    u16 num = 0;
    for (Node* node = m_List.Begin(); node != m_List.End(); node = m_List.Advance(node)) {
        if (node->m_Value.m_SessionId == sessionId) {
            num++;
        }
    }
    return num;
}

// 0x00733AF8 (name is ours)
bool nn::pia::session::StationIdStatusTable::GetStationIndex(const nn::pia::StationId& stationId, nn::pia::StationIndex* pStationIndex) const
{
    for (Node* node = m_List.Begin(); node != m_List.End(); node = m_List.Advance(node)) {
        if (node->m_Value.m_StationId == stationId) {
            *pStationIndex = node->m_Value.m_StationIndex;
            return true;
        }
    }
    return false;
}

// 0x00733B60 (name is ours)
u8 nn::pia::session::StationIdStatusTable::GetSessionRank(u32 sessionId) const
{
    u32 sessionIds[13];
    u8 num = 0;
    for (Node* node = m_List.Begin(); node != m_List.End(); node = m_List.Advance(node)) {
        u8 i;
        for (i = 0; i < num; i++) {
            if (sessionIds[i] == node->m_Value.m_SessionId) {
                break;
            }
        }
        if (i >= num) {
            sessionIds[num++] = node->m_Value.m_SessionId;
        }
    }
    u8 rank = 1;
    for (u8 i = 0; i < num; i++) {
        if (sessionIds[i] < sessionId) {
            rank++;
        }
    }
    return rank;
}

// 0x00733C00 (name is ours)
u8 nn::pia::session::StationIdStatusTable::GetSessionIds(u32* pSessionIds) const
{
    u8 num = 0;
    for (Node* node = m_List.Begin(); node != m_List.End(); node = m_List.Advance(node)) {
        u8 i;
        for (i = 0; i < num; i++) {
            if (pSessionIds[i] == node->m_Value.m_SessionId) {
                break;
            }
        }
        if (i >= num) {
            pSessionIds[num++] = node->m_Value.m_SessionId;
        }
    }
    return num;
}

// 0x00733C70 (name is ours)
bool nn::pia::session::StationIdStatusTable::IsValid(const nn::pia::StationId& stationId) const
{
    for (Node* node = m_List.Begin(); node != m_List.End(); node = m_List.Advance(node)) {
        if (node->m_Value.m_StationId == stationId) {
            return node->m_Value.m_IsValid;
        }
    }
    return false;
}

// 0x00733CD0 (name is ours)
u8 nn::pia::session::StationIdStatusTable::GetStatus(const nn::pia::StationId& stationId) const
{
    for (Node* node = m_List.Begin(); node != m_List.End(); node = m_List.Advance(node)) {
        if (node->m_Value.m_StationId == stationId) {
            return node->m_Value.m_Status;
        }
    }
    return 0;
}

// 0x00733D30 (name is ours)
bool nn::pia::session::StationIdStatusTable::AreValid(const nn::pia::StationId* pStationIds, u32 num) const
{
    for (Node* node = m_List.Begin(); node != m_List.End(); node = m_List.Advance(node)) {
        for (u32 i = 0; i < num; i++) {
            if (node->m_Value.m_StationId == pStationIds[i]) {
                if (!node->m_Value.m_IsValid) {
                    return false;
                }
                break;
            }
        }
    }
    return true;
}

// 0x00733DBC (name is ours)
u8 nn::pia::session::StationIdStatusTable::CheckStatus(const nn::pia::StationId* pStationIds, u32 num) const
{
    bool isListed = common::IsValidPointer(pStationIds) && num != 0;
    u8 result = 0;
    for (Node* node = m_List.Begin(); node != m_List.End(); node = m_List.Advance(node)) {
        if (isListed) {
            u32 i;
            for (i = 0; i < num; i++) {
                if (node->m_Value.m_StationId == pStationIds[i]) {
                    break;
                }
            }
            if (i >= num) {
                continue;
            }
        }
        if (node->m_Value.m_Status == 1) {
            return 2;
        }
        if (node->m_Value.m_Status == 0) {
            result = 1;
        }
    }
    return result;
}

// 0x00733E80 (name is ours)
bool nn::pia::session::StationIdStatusTable::IsNotified(const nn::pia::StationId& stationId) const
{
    for (Node* node = m_List.Begin(); node != m_List.End(); node = m_List.Advance(node)) {
        if (node->m_Value.m_StationId == stationId) {
            return node->m_Value.m_IsNotified;
        }
    }
    return false;
}

// 0x00733EE0 (name is ours)
bool nn::pia::session::StationIdStatusTable::GetUnknown0x12(const nn::pia::StationId& stationId) const
{
    for (Node* node = m_List.Begin(); node != m_List.End(); node = m_List.Advance(node)) {
        if (node->m_Value.m_StationId == stationId) {
            return node->m_Value.m_Unknown0x12;
        }
    }
    return false;
}

// 0x00733F40 (name is ours)
bool nn::pia::session::StationIdStatusTable::GetUnknown0x13(const nn::pia::StationId& stationId) const
{
    for (Node* node = m_List.Begin(); node != m_List.End(); node = m_List.Advance(node)) {
        if (node->m_Value.m_StationId == stationId) {
            return node->m_Value.m_Unknown0x13;
        }
    }
    return false;
}

// 0x00733FA0 (name is ours)
bool nn::pia::session::StationIdStatusTable::IsJointSessionStation(const nn::pia::transport::StationIdTable::Entry*) const
{
    // with Session (written with it)
}

} // namespace session
} // namespace pia
} // namespace nn
