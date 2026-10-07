#include "nn/pia/transport/transport_IdentificationInfoTable.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/transport/transport_Api.h"

namespace nn {
namespace pia {
namespace transport {
// 0x0097E450
nn::pia::transport::IdentificationInfoTable* nn::pia::transport::IdentificationInfoTable::s_pInstance;

// 0x0045C0D0 | fefates:bytes [tier B]
nn::Result nn::pia::transport::IdentificationInfoTable::AddToTable(const nn::pia::transport::Station* pStation, const nn::pia::transport::Station::IdentificationInfo* pInfo)
{
    if (!common::IsValidPointer(pStation) || !common::IsValidPointer(pInfo)) {
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
        m_pInfos[i] = *pInfo;
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
    m_pInfos[i] = *pInfo;
    m_CriticalSection.Unlock();
    return nn::Result();
}

// 0x0045C1EC | fefates:bytes-fuzzy [tier B]
void nn::pia::transport::IdentificationInfoTable::ClearTable()
{
    m_CriticalSection.Lock();
    for (u32 i = 0; i < STATION_NUM_MAX; i++) {
        m_pStations[i] = nullptr;
    }
    m_CriticalSection.Unlock();
}

// 0x0045C240 | fefates:bytes [tier B]
nn::Result nn::pia::transport::IdentificationInfoTable::Initialize(unsigned int stationNum)
{
    if (stationNum > STATION_NUM_MAX) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    m_StationNum = stationNum;
    m_pInfos = common::NewArray<Station::IdentificationInfo>(stationNum);
    memset(m_pInfos, 0, m_StationNum * sizeof(Station::IdentificationInfo));
    for (u32 i = 0; i < STATION_NUM_MAX; i++) {
        m_pStations[i] = nullptr;
    }
    return nn::Result();
}

// 0x0045C304 | fefates:bytes [tier B]
nn::Result nn::pia::transport::IdentificationInfoTable::CreateInstance()
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
    s_pInstance = new IdentificationInfoTable;
    return nn::Result();
}

// 0x0045C3A0 | fefates:bytes [tier B]
void nn::pia::transport::IdentificationInfoTable::EraseFromTable(const nn::pia::transport::Station* pStation)
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

// 0x0045C410 | fefates:callgraph [tier C]
void nn::pia::transport::IdentificationInfoTable::DestroyInstance()
{
    if (s_pInstance != nullptr) {
        delete s_pInstance;
        s_pInstance = nullptr;
    }
}

// 0x0045C440 | fefates:bytes [tier B]
nn::Result nn::pia::transport::IdentificationInfoTable::GetIdentificationInfo(const nn::pia::transport::Station* pStation, nn::pia::transport::Station::IdentificationInfo* pInfo)
{
    if (!common::IsValidPointer(pStation) || !common::IsValidPointer(pInfo)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    const Station::IdentificationInfo* pFound = nullptr;
    m_CriticalSection.Lock();
    for (u32 i = 0; i < m_StationNum; i++) {
        if (m_pStations[i] == pStation) {
            pFound = &m_pInfos[i];
            break;
        }
    }
    m_CriticalSection.Unlock();
    if (pFound == nullptr) {
        return common::RESULT_NO_DATA;
    }
    *pInfo = *pFound;
    return nn::Result();
}

// 0x0045C504 | fefates:bytes [tier B]
nn::Result nn::pia::transport::IdentificationInfoTable::GetPlayerName(const nn::pia::transport::Station* pStation, nn::pia::transport::Station::PlayerName* pName)
{
    if (!common::IsValidPointer(pStation) || !common::IsValidPointer(pName)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    const Station::IdentificationInfo* pFound = nullptr;
    m_CriticalSection.Lock();
    for (u32 i = 0; i < m_StationNum; i++) {
        if (m_pStations[i] == pStation) {
            pFound = &m_pInfos[i];
            break;
        }
    }
    m_CriticalSection.Unlock();
    if (pFound == nullptr) {
        return common::RESULT_NO_DATA;
    }
    *pName = pFound->m_PlayerName;
    return nn::Result();
}

// 0x0045C5CC | fefates:bytes [tier B]
nn::Result nn::pia::transport::IdentificationInfoTable::GetLocalIdentificationInfo(nn::pia::transport::Station::IdentificationInfo* pInfo)
{
    if (!common::IsValidPointer(pInfo)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    *pInfo = m_LocalInfo;
    return nn::Result();
}

// 0x0045C608 | fefates:bytes [tier B]
void nn::pia::transport::IdentificationInfoTable::SetLocalIdentificationInfo(const nn::pia::transport::Station::IdentificationInfo* pInfo)
{
    if (common::IsValidPointer(pInfo)) {
        m_LocalInfo = *pInfo;
    }
}

// 0x0045C670
// 0x0045C62C (deleting dtor)
nn::pia::transport::IdentificationInfoTable::~IdentificationInfoTable()
{
    if (m_pInfos != nullptr) {
        common::DeleteArray(m_pInfos);
    }
}

// 0x007366BC
void nn::pia::transport::IdentificationInfoTable::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace transport
} // namespace pia
} // namespace nn
