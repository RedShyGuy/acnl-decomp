#include "nn/pia/transport/transport_StationConnectionInfoTable.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/transport/transport_Api.h"
#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/transport/transport_StationLocation.h"

namespace nn {
namespace pia {
namespace transport {
// 0x0097E480
nn::pia::transport::StationConnectionInfoTable* nn::pia::transport::StationConnectionInfoTable::s_pInstance;

// 0x0045E444 | fefates:bytes [tier B]
nn::Result nn::pia::transport::StationConnectionInfoTable::AddToTable(const nn::pia::transport::Station* pStation, const nn::pia::transport::StationConnectionInfo& info)
{
    if (pStation == nullptr) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    m_CriticalSection.Lock();
    u32 i;
    for (i = 0; i < m_StationNum; i++) {
        if (m_pStations[i] == pStation) {
            break;
        }
    }
    if (i != m_StationNum) {
        m_pInfos[i] = info;
        m_CriticalSection.Unlock();
        return nn::Result();
    }
    for (i = 0; i < m_StationNum; i++) {
        if (m_pStations[i] == nullptr) {
            break;
        }
    }
    if (i == m_StationNum) {
        m_CriticalSection.Unlock();
        return common::RESULT_INVALID_STATE;
    }
    m_pStations[i] = pStation;
    m_pInfos[i] = info;
    m_CriticalSection.Unlock();
    return nn::Result();
}

// 0x0045E544 | fefates:bytes [tier B]
void nn::pia::transport::StationConnectionInfoTable::ClearTable()
{
    m_CriticalSection.Lock();
    for (u32 i = 0; i < STATION_NUM_MAX; i++) {
        m_pStations[i] = nullptr;
    }
    m_CriticalSection.Unlock();
}

// 0x0045E598 | fefates:bytes [tier B]
nn::Result nn::pia::transport::StationConnectionInfoTable::Initialize(unsigned int stationNum)
{
    if (stationNum > STATION_NUM_MAX) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    m_StationNum = stationNum;
    m_pInfos = common::NewArray<StationConnectionInfo>(stationNum);
    for (u32 i = 0; i < STATION_NUM_MAX; i++) {
        m_pStations[i] = nullptr;
    }
    return nn::Result();
}

// 0x0045E644 | fefates:bytes [tier B]
nn::Result nn::pia::transport::StationConnectionInfoTable::CreateInstance()
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
    s_pInstance = new StationConnectionInfoTable;
    return nn::Result();
}

// 0x0045E6E8 | fefates:bytes [tier B]
void nn::pia::transport::StationConnectionInfoTable::EraseFromTable(const nn::pia::transport::Station* pStation)
{
    if (pStation == nullptr) {
        return;
    }
    m_CriticalSection.Lock();
    for (u32 i = 0; i < m_StationNum; i++) {
        if (m_pStations[i] == pStation) {
            m_pStations[i] = nullptr;
            break;
        }
    }
    m_CriticalSection.Unlock();
}

// 0x0045E758 | fefates:callgraph [tier C]
void nn::pia::transport::StationConnectionInfoTable::DestroyInstance()
{
    if (s_pInstance != nullptr) {
        delete s_pInstance;
        s_pInstance = nullptr;
    }
}

// 0x0045E848
// 0x0045E788 (deleting dtor)
nn::pia::transport::StationConnectionInfoTable::~StationConnectionInfoTable()
{
    if (m_pInfos != nullptr) {
        common::DeleteArray(m_pInfos);
        m_pInfos = nullptr;
    }
}

// 0x007367EC | fefates:bytes [tier B]
nn::pia::transport::Station* nn::pia::transport::StationConnectionInfoTable::GetStation(const nn::pia::transport::StationConnectionInfo& info) const
{
    m_CriticalSection.Lock();
    // (not used)
    common::StationAddress address(info.m_PublicLocation.m_StationAddress);
    for (u32 i = 0; i < m_StationNum; i++) {
        if (info == m_pInfos[i]) {
            Station* pStation = const_cast<Station*>(m_pStations[i]);
            m_CriticalSection.Unlock();
            return pStation;
        }
    }
    m_CriticalSection.Unlock();
    return nullptr;
}

// 0x007368B4 | fefates:bytes [tier B]
nn::Result nn::pia::transport::StationConnectionInfoTable::GetStationKey(nn::pia::StationIndex stationIndex, unsigned int* pKey) const
{
    if (stationIndex > STATION_INDEX_MAX || !common::IsValidPointer(pKey)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    m_CriticalSection.Lock();
    for (u32 i = 0; i < m_StationNum; i++) {
        if (m_pStations[i] != nullptr && m_pStations[i]->m_StationIndex == stationIndex) {
            *pKey = m_pInfos[i].m_PublicLocation.m_StationKey;
            m_CriticalSection.Unlock();
            return nn::Result();
        }
    }
    m_CriticalSection.Unlock();
    return common::RESULT_NO_DATA;
}

// 0x00736974 | fefates:bytes [tier B]
nn::Result nn::pia::transport::StationConnectionInfoTable::GetStationKey(nn::pia::transport::Station* pStation, unsigned int* pKey) const
{
    if (!common::IsValidPointer(pStation) || !common::IsValidPointer(pKey)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    m_CriticalSection.Lock();
    for (u32 i = 0; i < m_StationNum; i++) {
        if (m_pStations[i] == pStation) {
            *pKey = m_pInfos[i].m_PublicLocation.m_StationKey;
            m_CriticalSection.Unlock();
            return nn::Result();
        }
    }
    m_CriticalSection.Unlock();
    return common::RESULT_NO_DATA;
}

// 0x00736A24 | fefates:bytes [tier B]
nn::Result nn::pia::transport::StationConnectionInfoTable::GetLocalStationKey(unsigned int* pKey) const
{
    if (!common::IsValidPointer(pKey)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    *pKey = m_LocalInfo.m_PublicLocation.m_StationKey;
    return nn::Result();
}

// 0x00736A4C | fefates:bytes [tier B]
nn::pia::transport::Station* nn::pia::transport::StationConnectionInfoTable::GetStationPartialMatch(const nn::pia::transport::StationLocation& location, nn::pia::transport::StationConnectionInfoTable::PartialMatchMode mode) const
{
    m_CriticalSection.Lock();
    switch (mode) {
    case PARTIAL_MATCH_MODE_STATION_KEY:
        for (u32 i = 0; i < m_StationNum; i++) {
            const StationLocation& entry = m_pInfos[i].m_PublicLocation;
            if (location.m_StationKey == entry.m_StationKey &&
                location.m_StationAddress.m_ExtensionId == entry.m_StationAddress.m_ExtensionId) {
                Station* pStation = const_cast<Station*>(m_pStations[i]);
                m_CriticalSection.Unlock();
                return pStation;
            }
        }
        break;
    case PARTIAL_MATCH_MODE_ADDRESS:
        for (u32 i = 0; i < m_StationNum; i++) {
            const StationLocation& entry = m_pInfos[i].m_PublicLocation;
            if (location.m_StationAddress.m_InetAddress.m_Address == entry.m_StationAddress.m_InetAddress.m_Address) {
                Station* pStation = const_cast<Station*>(m_pStations[i]);
                m_CriticalSection.Unlock();
                return pStation;
            }
        }
        break;
    }
    m_CriticalSection.Unlock();
    return nullptr;
}

// 0x00736B34 | fefates:bytes [tier B]
u32 nn::pia::transport::StationConnectionInfoTable::GetPrincipalIdByStation(const nn::pia::transport::Station* pStation) const
{
    if (!common::IsValidPointer(pStation)) {
        return 0;
    }
    m_CriticalSection.Lock();
    for (u32 i = 0; i < m_StationNum; i++) {
        if (m_pStations[i] == pStation) {
            u32 principalId = m_pInfos[i].m_PublicLocation.m_PrincipalId;
            m_CriticalSection.Unlock();
            return principalId;
        }
    }
    m_CriticalSection.Unlock();
    return 0;
}

// 0x00736BC4 | fefates:bytes [tier B]
nn::Result nn::pia::transport::StationConnectionInfoTable::GetStationConnectionInfo(const nn::pia::transport::Station* pStation, nn::pia::transport::StationConnectionInfo* pInfo) const
{
    if (pStation == nullptr || pInfo == nullptr) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    m_CriticalSection.Lock();
    for (u32 i = 0; i < m_StationNum; i++) {
        if (m_pStations[i] == pStation) {
            *pInfo = m_pInfos[i];
            m_CriticalSection.Unlock();
            return nn::Result();
        }
    }
    pStation->Trace(0x40080000);
    m_CriticalSection.Unlock();
    return common::RESULT_NO_DATA;
}

// 0x00736C80 | fefates:bytes [tier B]
nn::pia::StationIndex nn::pia::transport::StationConnectionInfoTable::GetStationIndexByPrincipalID(unsigned int principalId) const
{
    StationIndex stationIndex = STATION_INDEX_UNIDENTIFIED;
    m_CriticalSection.Lock();
    for (u32 i = 0; i < m_StationNum; i++) {
        if (m_pInfos[i].m_PublicLocation.m_PrincipalId == principalId && m_pStations[i] != nullptr &&
            m_pStations[i]->m_StationIndex <= STATION_INDEX_MAX) {
            stationIndex = m_pStations[i]->m_StationIndex;
            break;
        }
    }
    m_CriticalSection.Unlock();
    return stationIndex;
}

// 0x00736D08
void nn::pia::transport::StationConnectionInfoTable::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace transport
} // namespace pia
} // namespace nn
