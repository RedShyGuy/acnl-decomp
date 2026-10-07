#include "nn/pia/transport/transport_StationIdTable.h"
#include "nn/pia/common/common_HeapManager.h"
#include "nn/pia/common/common_Result.h"
#include "pead/peadHeapMgr.h"

namespace nn {
namespace pia {
namespace transport {
// 0x0044F23C (name is ours)
nn::Result nn::pia::transport::StationIdTable::Initialize(u32 entryNum)
{
    if (entryNum == 0) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (common::IsValidPointer(m_pBuffer)) {
        return common::RESULT_ALREADY_INITIALIZED;
    }
    // a value initialized u64 array, so the nodes are 8 byte aligned
    u32 wordNum = (entryNum * sizeof(common::ObjList<Entry>::Node) + 7) / 8;
    u64* pBuffer = static_cast<u64*>(pead::AllocMemory(wordNum * 8, common::HeapManager::GetHeap()));
    if (pBuffer != nullptr) {
        for (u32 i = 0; i < wordNum; i++) {
            ::new (&pBuffer[i]) u64();
        }
    }
    m_pBuffer = pBuffer;
    if (!common::IsValidPointer(pBuffer)) {
        return common::RESULT_OUT_OF_MEMORY;
    }
    m_List.Initialize(reinterpret_cast<common::ObjList<Entry>::Node*>(pBuffer), entryNum);
    m_EntryNumMax = entryNum;
    return nn::Result();
}

// 0x0044F354 (name is ours)
nn::Result nn::pia::transport::StationIdTable::Remove(nn::pia::StationIndex stationIndex)
{
    return RemoveCore(FindCore(stationIndex));
}

// 0x0044F370 (name is ours)
nn::Result nn::pia::transport::StationIdTable::RemoveCore(Entry* pEntry)
{
    if (pEntry == nullptr) {
        return common::RESULT_NOT_FOUND;
    }
    pEntry->m_StationId = GetStationIdOfIndex253();
    pEntry->m_StationIndex = STATION_INDEX_UNIDENTIFIED;
    pEntry->m_Key = 0;
    m_List.Erase(pEntry);
    return nn::Result();
}

// 0x0044F3DC (name is ours)
void nn::pia::transport::StationIdTable::SetEntryNumMax(u32 num)
{
    if (m_List.GetCapacity() >= num) {
        m_EntryNumMax = num;
    }
}

// 0x0044F3EC (name is ours)
nn::Result nn::pia::transport::StationIdTable::Add(nn::pia::StationId stationId, nn::pia::StationIndex stationIndex, u32 key)
{
    if (m_List.GetFreeCount() == 0) {
        return common::RESULT_NO_FREE_ENTRY;
    }
    Entry* pEntry = m_List.PushBackNew();
    pEntry->m_StationId = stationId;
    pEntry->m_StationIndex = stationIndex;
    pEntry->m_Key = key;
    return nn::Result();
}

// 0x0044F484 (name is ours)
void nn::pia::transport::StationIdTable::Clear()
{
    m_List.Clear();
    m_EntryNumMax = m_List.GetCapacity();
}

// 0x0044F4F4 (name is ours)
nn::Result nn::pia::transport::StationIdTable::Remove(nn::pia::StationId stationId)
{
    return RemoveCore(FindCore(stationId));
}

// 0x0044F510 | fefates:bytes [tier B]
void nn::pia::transport::StationIdTable::Finalize()
{
    if (common::IsValidPointer(m_pBuffer)) {
        Clear();
        if (m_pBuffer != nullptr) {
            pead::FreeMemory(m_pBuffer);
        }
        m_pBuffer = nullptr;
    }
}

// 0x0044F5A8 | fefates:bytes [tier B]
nn::pia::transport::StationIdTable::StationIdTable() : m_pBuffer(nullptr)
{
}

// 0x0044F5F0 | fefates:bytes [tier B]
nn::pia::transport::StationIdTable::~StationIdTable()
{
    Finalize();
}

// 0x00734EE0 | fefates:bytes [tier B]
s32 nn::pia::transport::StationIdTable::GetEntryNum() const
{
    s32 num = 0;
    for (common::ObjList<Entry>::Node* node = m_List.Begin(); node != m_List.End(); node = m_List.Advance(node)) {
        num++;
    }
    return num;
}

// 0x00734F10 | fefates:bytes [tier B]
nn::Result nn::pia::transport::StationIdTable::Find(nn::pia::transport::StationIdTable::Entry* pEntry, nn::pia::StationIndex stationIndex) const
{
    if (!common::IsValidPointer(pEntry)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    const Entry* pFound = FindCore(stationIndex);
    if (pFound == nullptr) {
        return common::RESULT_NOT_FOUND;
    }
    *pEntry = *pFound;
    return nn::Result();
}

// 0x00734F74 | fefates:bytes [tier B]
nn::Result nn::pia::transport::StationIdTable::Find(nn::pia::transport::StationIdTable::Entry* pEntry, nn::pia::StationId stationId) const
{
    if (!common::IsValidPointer(pEntry)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    const Entry* pFound = FindCore(stationId);
    if (pFound == nullptr) {
        return common::RESULT_NOT_FOUND;
    }
    *pEntry = *pFound;
    return nn::Result();
}

// 0x00734FE0 | fefates:bytes [tier B]
nn::Result nn::pia::transport::StationIdTable::Find(nn::pia::transport::StationIdTable::Entry* pEntry, unsigned int key) const
{
    if (!common::IsValidPointer(pEntry)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    const Entry* pFound = FindCore(key);
    if (pFound == nullptr) {
        return common::RESULT_NOT_FOUND;
    }
    *pEntry = *pFound;
    return nn::Result();
}

// 0x00735074 | fefates:bytes-fuzzy [tier B]
nn::pia::transport::StationIdTable::Entry* nn::pia::transport::StationIdTable::FindCore(nn::pia::StationIndex stationIndex) const
{
    for (common::ObjList<Entry>::Node* node = m_List.Begin(); node != m_List.End(); node = m_List.Advance(node)) {
        if (node->m_Value.m_StationIndex == stationIndex) {
            return &node->m_Value;
        }
    }
    return nullptr;
}

// 0x007350B0 | fefates:bytes [tier B]
nn::pia::transport::StationIdTable::Entry* nn::pia::transport::StationIdTable::FindCore(nn::pia::StationId stationId) const
{
    for (common::ObjList<Entry>::Node* node = m_List.Begin(); node != m_List.End(); node = m_List.Advance(node)) {
        if (node->m_Value.m_StationId == stationId) {
            return &node->m_Value;
        }
    }
    return nullptr;
}

// 0x00735104 | fefates:bytes [tier B]
nn::pia::transport::StationIdTable::Entry* nn::pia::transport::StationIdTable::FindCore(unsigned int key) const
{
    for (common::ObjList<Entry>::Node* node = m_List.Begin(); node != m_List.End(); node = m_List.Advance(node)) {
        if (node->m_Value.m_Key == key) {
            return &node->m_Value;
        }
    }
    return nullptr;
}

} // namespace transport
} // namespace pia
} // namespace nn
