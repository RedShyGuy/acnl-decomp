#include "nn/pia/transport/transport_StationManager.h"
#include "nn/pia/common/common_HeapManager.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_StationAddress.h"
#include "nn/pia/transport/transport_Api.h"
#include "pead/peadHeapMgr.h"

namespace nn {
namespace pia {
namespace transport {
namespace {
// the indices GetStation looks for (name is ours)
bool IsStationIndexToSearch(StationIndex stationIndex)
{
    return stationIndex <= STATION_INDEX_MAX || stationIndex == 254;
}
} // namespace

// 0x00975A8C
nn::pia::transport::StationManager* nn::pia::transport::StationManager::s_pInstance;

// 0x0044F690 | fefates:callgraph [tier C]
nn::pia::transport::Station* nn::pia::transport::StationManager::GetStation(nn::pia::StationIndex stationIndex)
{
    if (IsStationIndexToSearch(stationIndex)) {
        for (Station** it = m_ActiveStations.Begin(); it != m_ActiveStations.End(); it++) {
            if ((*it)->m_StationIndex == stationIndex) {
                return *it;
            }
        }
    }
    return nullptr;
}

// 0x0044F6E4 | fefates:bytes [tier B]
nn::pia::transport::Station* nn::pia::transport::StationManager::GetStation(nn::pia::StationId stationId)
{
    return GetStation(Conv2StationIndex(stationId));
}

// 0x0044F754 | fefates:bytes [tier B]
nn::pia::transport::Station* nn::pia::transport::StationManager::GetStation(const nn::pia::common::StationAddress& address)
{
    for (Station** it = m_ActiveStations.Begin(); it != m_ActiveStations.End(); it++) {
        if ((*it)->m_StationAddress == address) {
            return *it;
        }
    }
    return nullptr;
}

// 0x0044F7BC | fefates:bytes [tier B]
nn::Result nn::pia::transport::StationManager::Initialize(nn::pia::transport::NetworkFactory* pFactory, unsigned int stationNum)
{
    if (!common::IsValidPointer(pFactory) || stationNum == 0 || stationNum > STATION_NUM_MAX) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    u32 size = stationNum * sizeof(common::ObjList<Station>::Node);
    u8* pBuffer = static_cast<u8*>(pead::AllocMemory(size, common::HeapManager::GetHeap()));
    if (pBuffer != nullptr) {
        for (u32 i = 0; i < size; i++) {
            ::new (&pBuffer[i]) u8();
        }
    }
    m_pBuffer = pBuffer;
    if (!common::IsValidPointer(pBuffer)) {
        return common::RESULT_OUT_OF_MEMORY;
    }
    m_StationList.Initialize(reinterpret_cast<common::ObjList<Station>::Node*>(pBuffer), stationNum);
    for (u32 i = 0; i < stationNum; i++) {
        m_StationList.PushBackNew()->Initialize(pFactory);
    }
    for (common::ObjList<Station>::Node* node = m_StationList.Begin(); node != m_StationList.End(); node = m_StationList.Advance(node)) {
        m_FreeStations.PushBack(&node->m_Value);
    }
    return nn::Result();
}

// 0x0044F980 | fefates:bytes [tier B]
nn::pia::transport::Station* nn::pia::transport::StationManager::CreateStation()
{
    if (m_StationList.m_pBuffer == nullptr || m_FreeStations.GetNum() == 0 || m_ActiveStations.IsFull()) {
        return nullptr;
    }
    Station** it = &m_FreeStations.Back();
    Station* pStation = *it;
    m_ActiveStations.PushBack(pStation);
    m_FreeStations.Erase(it);
    return pStation;
}

// 0x0044FA24 | fefates:callgraph [tier C]
nn::Result nn::pia::transport::StationManager::CreateInstance()
{
    if (!IsInitialized()) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (!IsInSetupMode()) {
        return common::RESULT_INVALID_STATE;
    }
    if (s_pInstance != nullptr) {
        return common::RESULT_ALREADY_EXISTS;
    }
    s_pInstance = new StationManager();
    return nn::Result();
}

// 0x0044FA88 | fefates:bytes [tier B]
void nn::pia::transport::StationManager::DestroyStation(nn::pia::transport::Station* pStation)
{
    if (!common::IsValidPointer(pStation)) {
        return;
    }
    if (!m_StationList.Contains(pStation)) {
        return;
    }
    if (!m_ActiveStations.IsInclude(pStation)) {
        return;
    }
    Station** it = m_ActiveStations.Find(pStation);
    if (it == m_ActiveStations.End()) {
        return;
    }
    m_ActiveStations.Erase(it);
    m_FreeStations.PushBack(pStation);
    if (m_pLocalStation == pStation) {
        m_pLocalStation = nullptr;
    }
}

// 0x0044FBD4 | fefates:bytes [tier B]
void nn::pia::transport::StationManager::DestroyInstance()
{
    if (s_pInstance != nullptr) {
        delete s_pInstance;
        s_pInstance = nullptr;
    }
}

// 0x0044FC28 | fefates:bytes [tier B]
nn::pia::transport::Station* nn::pia::transport::StationManager::CreateLocalStation()
{
    if (m_pLocalStation != nullptr) {
        return nullptr;
    }
    m_pLocalStation = CreateStation();
    return m_pLocalStation;
}

// 0x0044FCE4 | fefates:bytes [tier B]
void nn::pia::transport::StationManager::Finalize()
{
    if (m_StationList.m_pBuffer != nullptr) {
        for (common::ObjList<Station>::Node* node = m_StationList.Begin(); node != m_StationList.End(); node = m_StationList.Advance(node)) {
            node->m_Value.Finalize();
        }
        while (Station* pStation = m_StationList.Back()) {
            pStation->~Station();
            m_StationList.Erase(pStation);
        }
    }
    if (m_pBuffer != nullptr) {
        pead::GetMemoryBlockInfo(m_pBuffer);
        pead::FreeMemory(m_pBuffer);
        m_pBuffer = nullptr;
    }
}

// 0x0044FDAC | fefates:bytes [tier B]
nn::pia::transport::StationManager::StationManager() : m_pBuffer(nullptr), m_pLocalStation(nullptr), m_Unknown0xA8(STATION_INDEX_UNIDENTIFIED)
{
}

// 0x00735140 | fefates:callgraph [tier C]
nn::pia::transport::Station* nn::pia::transport::StationManager::GetStation(nn::pia::StationIndex stationIndex) const
{
    if (IsStationIndexToSearch(stationIndex)) {
        for (Station* const* it = m_ActiveStations.Begin(); it != m_ActiveStations.End(); it++) {
            if ((*it)->m_StationIndex == stationIndex) {
                return *it;
            }
        }
    }
    return nullptr;
}

// 0x00735194 | libgarden [tier A]
nn::Result nn::pia::transport::StationManager::GetStationAddress(nn::pia::common::StationAddress* pAddress, nn::pia::StationId stationId) const
{
    return GetStationAddress(pAddress, Conv2StationIndex(stationId));
}

// 0x007351C0 | fefates:bytes [tier B]
nn::Result nn::pia::transport::StationManager::GetStationAddress(nn::pia::common::StationAddress* pAddress, nn::pia::StationIndex stationIndex) const
{
    if (!common::IsValidPointer(pAddress) || !IsStationIndexToSearch(stationIndex)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    const Station* pStation = GetStation(stationIndex);
    if (pStation == nullptr) {
        return common::RESULT_NOT_FOUND;
    }
    *pAddress = pStation->m_StationAddress;
    return nn::Result();
}

// 0x00735270 | fefates:bytes [tier B]
u32 nn::pia::transport::StationManager::GetParticipatingStationBitmap(bool includeLocal) const
{
    u32 bitmap = 0;
    for (Station* const* it = m_ActiveStations.Begin(); it != m_ActiveStations.End(); it++) {
        const Station* pStation = *it;
        if (pStation->m_StationIndex <= STATION_INDEX_MAX && pStation->m_State == Station::STATION_STATE_CONNECTED) {
            bitmap |= 1 << pStation->m_StationIndex;
        }
    }
    if (!includeLocal && m_pLocalStation != nullptr && m_pLocalStation->m_StationIndex <= STATION_INDEX_MAX) {
        bitmap &= ~(1 << m_pLocalStation->m_StationIndex);
    }
    return bitmap;
}

// 0x007352E4
void nn::pia::transport::StationManager::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace transport
} // namespace pia
} // namespace nn

// the container of the stations (out of line in the original)

// 0x007E5074
// 0x007E5070 (deleting dtor)
template nn::pia::common::SimpleContainer<nn::pia::transport::Station*, 12u>::~SimpleContainer();
// 0x00827FE4
template void nn::pia::common::SimpleContainer<nn::pia::transport::Station*, 12u>::Trace(u64) const;
