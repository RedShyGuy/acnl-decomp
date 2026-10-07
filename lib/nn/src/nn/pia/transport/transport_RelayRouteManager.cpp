#include "nn/pia/transport/transport_RelayRouteManager.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_SessionBeginMonitoringContent.h"
#include "nn/pia/transport/transport_Transport.h"
#include "nn/nstd/nstd_String.h"
#include <string.h>

namespace nn {
namespace pia {
namespace transport {
namespace {
// the number of stations of the session
u32 GetStationNum()
{
    return Transport::s_pInstance->m_StationNum;
}
} // namespace

// 0x00454E64 | fefates:bytes [tier B]
nn::Result nn::pia::transport::RelayRouteManager::Initialize(unsigned int stationNum, bool isUsingBlockedTable)
{
    if (stationNum == 0 || stationNum > STATION_INDEX_MAX + 1) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    u32 tableSize = stationNum * stationNum;
    m_pRttTable = common::NewArray<u16>(tableSize);
    if (!common::IsValidPointer(m_pRttTable)) {
        return common::RESULT_OUT_OF_MEMORY;
    }
    memset(m_pRttTable, 0, tableSize * sizeof(u16));
    m_pRelayNodeRttTable = common::NewArray<u16>(stationNum);
    if (!common::IsValidPointer(m_pRelayNodeRttTable)) {
        return common::RESULT_OUT_OF_MEMORY;
    }
    memset(m_pRelayNodeRttTable, 0, stationNum * sizeof(u16));
    m_pRelayRouteTable = common::NewArray<u8>(tableSize);
    if (!common::IsValidPointer(m_pRelayRouteTable)) {
        return common::RESULT_OUT_OF_MEMORY;
    }
    for (u32 i = 0; i < stationNum; i++) {
        for (u32 j = 0; j < stationNum; j++) {
            m_pRelayRouteTable[i * stationNum + j] = i;
        }
    }
    m_pRelayCount = common::NewArray<u8>(stationNum);
    if (!common::IsValidPointer(m_pRelayCount)) {
        return common::RESULT_OUT_OF_MEMORY;
    }
    memset(m_pRelayCount, 0, stationNum);
    m_pConnectionBitmaps = common::NewArray<u32>(stationNum);
    if (!common::IsValidPointer(m_pConnectionBitmaps)) {
        return common::RESULT_OUT_OF_MEMORY;
    }
    memset(m_pConnectionBitmaps, 0, stationNum * sizeof(u32));
    m_IsUsingBlockedTable = isUsingBlockedTable;
    if (isUsingBlockedTable) {
        m_pBlockedTable = common::NewArray<u8>(tableSize);
        if (!common::IsValidPointer(m_pBlockedTable)) {
            return common::RESULT_OUT_OF_MEMORY;
        }
        memset(m_pBlockedTable, 0, tableSize);
    }
    m_pRefuseReasons = common::NewArray<u8>(stationNum);
    if (!common::IsValidPointer(m_pRefuseReasons)) {
        return common::RESULT_OUT_OF_MEMORY;
    }
    memset(m_pRefuseReasons, 0, stationNum);
    m_pOriginalRelayRouteTable = common::NewArray<u8>(tableSize);
    if (!common::IsValidPointer(m_pOriginalRelayRouteTable)) {
        return common::RESULT_OUT_OF_MEMORY;
    }
    nnnstdMemCpy(m_pOriginalRelayRouteTable, m_pRelayRouteTable, tableSize);
    return nn::Result();
}

// 0x00455208 | fefates:bytes [tier B]
bool nn::pia::transport::RelayRouteManager::SwitchRelay(unsigned int relay, unsigned int stationBitmap, unsigned short rttLimit, unsigned short relayCountLimit)
{
    u32 n = GetStationNum();
    for (u32 i = 1; i < n; i++) {
        if (i == relay) {
            continue;
        }
        for (u32 j = 0; j < i; j++) {
            if (j == relay || m_pRelayRouteTable[i * n + j] != relay) {
                continue;
            }
            // another station for the route between i and j
            for (u32 k = 0; k < n; k++) {
                if (!(stationBitmap & (1 << k)) || k == i || k == j || k == relay) {
                    continue;
                }
                u16 rtt0 = m_pRttTable[i * n + k];
                if (rtt0 == 0) {
                    continue;
                }
                u16 rtt1 = m_pRttTable[k * n + j];
                if (rtt1 == 0 || rtt1 + rtt0 > m_RttLimit) {
                    continue;
                }
                u32 relayCount = m_pRelayCount[k] + 2;
                if (m_RelayCountMax < relayCount) {
                    continue;
                }
                if (rttLimit < m_pRttTable[i * n + k] + m_pRttTable[k * n + j] || relayCountLimit < relayCount) {
                    continue;
                }
                m_pRelayRouteTable[i * n + j] = k;
                m_pRelayRouteTable[j * n + i] = k;
                m_pRelayCount[k] += 2;
                m_pRelayCount[relay] -= 2;
                return true;
            }
        }
    }
    return false;
}

// 0x0045537C (name is ours)
nn::Result nn::pia::transport::RelayRouteManager::SetRelayRoute(nn::pia::StationIndex from, nn::pia::StationIndex to, nn::pia::StationIndex relay)
{
    u32 n = GetStationNum();
    if (from >= n || to >= n) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    m_pRelayRouteTable[from * n + to] = relay;
    return nn::Result();
}

// 0x004553B0 (name is ours)
void nn::pia::transport::RelayRouteManager::SetRelayCountMax(u16 relayCountMax)
{
    // even: a route counts 2
    relayCountMax &= ~1;
    m_RelayCountMax = relayCountMax;
    common::g_SessionBeginMonitoringContent.m_RelayCountMax = relayCountMax;
}

// 0x004553C8 | fefates:bytes [tier B]
void nn::pia::transport::RelayRouteManager::CalcStationType(unsigned int stationBitmap, unsigned int* pStayBitmap, unsigned int* pResetBitmap)
{
    *pStayBitmap = 0;
    *pResetBitmap = 0;
    u32 n = GetStationNum();
    for (u32 i = 0; i < n; i++) {
        u32 bit = 1 << i;
        bool isRouted = (m_RoutedStationBitmap & bit) != 0;
        bool isRequested = (bit & stationBitmap) != 0;
        bool isUnconnected = true;
        for (u32 j = 0; j < n; j++) {
            if (m_pRttTable[i * n + j] != 0) {
                isUnconnected = false;
                break;
            }
        }
        if (isRequested && isUnconnected) {
            if (m_IsUsingBlockedTable) {
                for (u32 j = 0; j < n; j++) {
                    if (m_pBlockedTable[i * n + j] == 1) {
                        *pResetBitmap |= bit;
                        break;
                    }
                }
            }
        } else if (isRequested) {
            if (isRouted) {
                *pResetBitmap |= bit;
            }
        } else if (isUnconnected) {
            if (isRouted) {
                if (m_IsUsingBlockedTable && m_FirstStationIndex == i) {
                    *pStayBitmap |= bit;
                } else {
                    *pResetBitmap |= bit;
                }
            }
        } else if (isRouted) {
            *pStayBitmap |= bit;
        }
    }
}

// 0x00455510 | fefates:bytes [tier B]
void nn::pia::transport::RelayRouteManager::ProcStayStation(unsigned int stationBitmap, unsigned int* pRefusedBitmap)
{
    u32 n = GetStationNum();
    // the routes over stations that are not connected any more are broken
    for (u32 i = 0; i < n; i++) {
        if (!(stationBitmap & (1 << i))) {
            continue;
        }
        for (u32 j = 0; j < n; j++) {
            if (i == j || !(stationBitmap & (1 << j)) || m_pRttTable[i * n + j] != 0) {
                continue;
            }
            u8 relay = m_pRelayRouteTable[i * n + j];
            if (relay == i) {
                continue;
            }
            if (relay == j) {
                m_pRelayRouteTable[i * n + j] = m_BrokenRoute;
            } else if (relay < n && (m_pRttTable[i * n + relay] == 0 || m_pRttTable[relay * n + j] == 0)) {
                m_pRelayRouteTable[i * n + j] = m_BrokenRoute;
            }
        }
    }
    n = GetStationNum();
    memset(m_pRelayCount, 0, n);
    for (u32 i = 0; i < n; i++) {
        for (u32 j = 0; j < n; j++) {
            u8 relay = m_pRelayRouteTable[i * n + j];
            if (relay != i && relay != j && relay < n) {
                m_pRelayCount[relay]++;
            }
        }
    }
    while (!DecideBrokenRouteRelayStation(stationBitmap, pRefusedBitmap)) {
    }
}

// 0x0045567C | fefates:bytes [tier B]
void nn::pia::transport::RelayRouteManager::UpdateRelayTable(unsigned int stationBitmap)
{
    u32 n = GetStationNum();
    // the new stations: direct routes to all
    for (u32 i = 0; i < n; i++) {
        u32 bit = 1 << i;
        if ((m_RoutedStationBitmap & bit) || !(bit & stationBitmap)) {
            continue;
        }
        for (u32 j = 0; j < n; j++) {
            if ((m_RoutedStationBitmap | stationBitmap) & (1 << j)) {
                m_pRelayRouteTable[i * n + j] = j;
                m_pRelayRouteTable[j * n + i] = i;
            }
        }
        m_RoutedStationBitmap |= bit;
    }
    // the other pairs: no route
    for (u32 i = 0; i < n; i++) {
        for (u32 j = i; j < n; j++) {
            if ((m_RoutedStationBitmap & (1 << i)) && (m_RoutedStationBitmap & (1 << j))) {
                continue;
            }
            if (m_pRelayRouteTable[i * n + j] == i && m_pRelayRouteTable[j * n + i] == j) {
                continue;
            }
            m_pRelayRouteTable[i * n + j] = i;
            m_pRelayRouteTable[j * n + i] = j;
        }
    }
}

// 0x004557A0 | fefates:bytes [tier B]
bool nn::pia::transport::RelayRouteManager::ProcNewStationOne(unsigned int stationIndex, unsigned int stationBitmap)
{
    u32 n = GetStationNum();
    u32 i;
    for (i = 0; i < n; i++) {
        if (!(stationBitmap & (1 << i))) {
            continue;
        }
        if (m_pRttTable[stationIndex * n + i] != 0) {
            m_pRelayRouteTable[stationIndex * n + i] = i;
            m_pRelayRouteTable[i * n + stationIndex] = stationIndex;
            continue;
        }

        // a relay station between them
        bool isConnected = false;
        u8 reason = 1;
        for (u32 k = 0; k < n; k++) {
            if (!(stationBitmap & (1 << k)) || k == stationIndex || k == i) {
                continue;
            }
            u32 stationNum = GetStationNum();
            u16 rtt0 = m_pRttTable[stationIndex * stationNum + k];
            if (rtt0 == 0) {
                continue;
            }
            u16 rtt1 = m_pRttTable[k * stationNum + i];
            if (rtt1 == 0) {
                continue;
            }
            if (rtt1 + rtt0 > m_RttLimit) {
                reason = 2;
                continue;
            }
            if (m_pRelayCount[k] + 2 > m_RelayCountMax) {
                reason = 3;
                continue;
            }
            m_pRelayRouteTable[stationIndex * n + i] = k;
            m_pRelayRouteTable[i * n + stationIndex] = k;
            m_pRelayCount[k] += 2;
            isConnected = true;
            break;
        }
        if (isConnected) {
            continue;
        }

        // a relay station that is busy, after moving one of its routes
        u32 stationNum = GetStationNum();
        u32 candidateBitmap = 0;
        for (u32 k = 0; k < stationNum; k++) {
            u32 bit = 1 << k;
            if (!(bit & stationBitmap)) {
                continue;
            }
            u16 rtt0 = m_pRttTable[stationIndex * stationNum + k];
            if (rtt0 == 0) {
                continue;
            }
            u16 rtt1 = m_pRttTable[k * stationNum + i];
            if (rtt1 == 0 || rtt0 + rtt1 > m_RttLimit || m_pRelayCount[k] > m_RelayCountMax) {
                continue;
            }
            candidateBitmap |= bit;
        }
        for (u32 k = 0; k < stationNum; k++) {
            if (!(candidateBitmap & (1 << k))) {
                continue;
            }
            if (SwitchRelay(k, stationBitmap, m_RttLimit, m_RelayCountMax)) {
                m_pRelayRouteTable[stationIndex * stationNum + i] = k;
                m_pRelayRouteTable[i * stationNum + stationIndex] = k;
                m_pRelayCount[k] += 2;
                isConnected = true;
                break;
            }
        }
        if (isConnected) {
            continue;
        }

        // no route: the routes of the station so far are taken back
        m_pRefuseReasons[stationIndex] = reason;
        for (u32 j = 0; j < i; j++) {
            u8 relay = m_pRelayRouteTable[stationIndex * n + j];
            if (relay != stationIndex && relay != j) {
                m_pRelayCount[relay] -= 2;
            }
            m_pRelayRouteTable[stationIndex * n + j] = stationIndex;
            m_pRelayRouteTable[j * n + stationIndex] = j;
        }
        return false;
    }
    return true;
}

// 0x00455B24 | fefates:bytes [tier B]
void nn::pia::transport::RelayRouteManager::ProcRefusedStation(unsigned int refusedBitmap, unsigned int stationBitmap)
{
    u32 n = GetStationNum();
    for (u32 i = 0; i < n; i++) {
        if (!(refusedBitmap & (1 << i))) {
            continue;
        }
        for (u32 j = 0; j < n; j++) {
            if (!((refusedBitmap | stationBitmap) & (1 << j)) || i == j) {
                continue;
            }
            if (m_pRttTable[i * n + j] != 0) {
                m_pRelayRouteTable[i * n + j] = j;
                m_pRelayRouteTable[j * n + i] = i;
                continue;
            }
            // a relay station within the limits, else any relay station
            u32 relay;
            bool isFound = false;
            for (relay = 0; relay < n; relay++) {
                if (!(stationBitmap & (1 << relay))) {
                    continue;
                }
                u32 stationNum = GetStationNum();
                u16 rtt0 = m_pRttTable[i * stationNum + relay];
                if (rtt0 == 0) {
                    continue;
                }
                u16 rtt1 = m_pRttTable[relay * stationNum + j];
                if (rtt1 == 0 || rtt1 + rtt0 > m_RttLimit || m_pRelayCount[relay] + 2 > m_RelayCountMax) {
                    continue;
                }
                isFound = true;
                break;
            }
            if (!isFound) {
                for (relay = 0; relay < n; relay++) {
                    if ((stationBitmap & (1 << relay)) && m_pRttTable[i * n + relay] != 0 && m_pRttTable[relay * n + j] != 0) {
                        isFound = true;
                        break;
                    }
                }
            }
            if (isFound) {
                m_pRelayRouteTable[i * n + j] = relay;
                m_pRelayRouteTable[j * n + i] = relay;
                m_pRelayCount[relay] += 2;
            }
        }
    }
}

// 0x00455CCC | fefates:bytes [tier B]
void nn::pia::transport::RelayRouteManager::RelayRouteOptimization(unsigned int stationBitmap)
{
    while (RelayRouteOptimizationRTTOne(stationBitmap)) {
    }
    for (;;) {
        // the slowest relayed route and the busiest relay station
        u32 n = GetStationNum();
        u32 rttMax = 0;
        for (u32 i = 0; i < n; i++) {
            if (!(stationBitmap & (1 << i))) {
                continue;
            }
            for (u32 j = i + 1; j < n; j++) {
                if (!(stationBitmap & (1 << j))) {
                    continue;
                }
                u8 relay = m_pRelayRouteTable[i * n + j];
                if (relay == i || relay == j) {
                    continue;
                }
                u32 rtt = m_pRttTable[i * n + relay] + m_pRttTable[relay * n + j];
                if (rttMax < rtt) {
                    rttMax = rtt;
                }
            }
        }
        u32 relayCountMax = 0;
        u32 busiestStation;
        for (u32 k = 0; k < n; k++) {
            if (m_pRelayCount[k] > relayCountMax) {
                busiestStation = k;
                relayCountMax = m_pRelayCount[k];
            }
        }
        if (relayCountMax == 0) {
            return;
        }
        if (!SwitchRelay(busiestStation, stationBitmap, rttMax, relayCountMax - 1)) {
            return;
        }
    }
}

// 0x00455DF4 | fefates:bytes [tier B]
nn::Result nn::pia::transport::RelayRouteManager::SearchRefugeRelayRoute(nn::pia::StationIndex brokenStationIndex)
{
    u32 n = GetStationNum();
    memset(m_pRelayCount, 0, n);
    for (u32 i = 0; i < n; i++) {
        for (u32 j = 0; j < n; j++) {
            u8 relay = m_pRelayRouteTable[i * n + j];
            if (relay != i && relay != j && relay < n) {
                m_pRelayCount[relay]++;
            }
        }
    }
    if (m_pRelayCount[brokenStationIndex] == 0) {
        return nn::Result();
    }

    // another relay station (the least busy one) for each route over the broken station
    bool isAllFound = true;
    for (u32 i = 0; i < n; i++) {
        if (i == brokenStationIndex) {
            continue;
        }
        for (u32 j = i + 1; j < n; j++) {
            if (j == brokenStationIndex || m_pRelayRouteTable[i * n + j] != brokenStationIndex) {
                continue;
            }
            u32 relay = n;
            u32 relayCountMin = n * n;
            for (u32 k = 0; k < n; k++) {
                if (k == i || k == j || k == brokenStationIndex) {
                    continue;
                }
                if (m_pRelayRouteTable[i * n + k] == k && m_pRelayRouteTable[k * n + j] == j && m_pRelayCount[k] < relayCountMin) {
                    relayCountMin = m_pRelayCount[k];
                    relay = k;
                }
            }
            if (relay == n) {
                isAllFound = false;
                continue;
            }
            m_pRelayRouteTable[i * n + j] = relay;
            m_pRelayRouteTable[j * n + i] = relay;
            m_pRelayCount[brokenStationIndex] -= 2;
            m_pRelayCount[relay] += 2;
        }
    }
    if (!isAllFound) {
        return common::RESULT_NOT_FOUND;
    }
    return nn::Result();
}

// 0x00455FB8 | fefates:bytes [tier B]
void nn::pia::transport::RelayRouteManager::InitRelayRouteManagerData()
{
    u32 n = GetStationNum();
    for (u32 i = 0; i < n; i++) {
        for (u32 j = 0; j < n; j++) {
            m_pRelayRouteTable[i * n + j] = i;
        }
    }
    m_RoutedStationBitmap = 0;
    m_DirectionsVersionHigh = 0;
    m_DirectionsVersionLow = 0;
    m_RefusedStationBitmap = 0;
}

// 0x00456060 | fefates:bytes [tier B]
nn::Result nn::pia::transport::RelayRouteManager::UpdateDirectConnectionData(const unsigned char* rttTable, unsigned int size, unsigned int stationBitmap, unsigned int* pRefusedBitmap, nn::pia::StationIndex firstStationIndex, unsigned char* pReasons)
{
    m_FirstStationIndex = firstStationIndex;
    if (m_RoutedStationBitmap == 0) {
        u32 bit = 1 << firstStationIndex;
        if (!(bit & stationBitmap)) {
            m_RoutedStationBitmap = bit;
        }
    }
    u32 n = GetStationNum();
    u32 tableSize = n * n;
    if (tableSize != size || !common::IsValidPointer(rttTable) || !common::IsValidPointer(pRefusedBitmap) || !common::IsValidPointer(pReasons)) {
        return common::RESULT_INVALID_ARGUMENT;
    }

    // the round trip times (both directions the same) and the connections
    for (u32 i = 0; i < n; i++) {
        for (u32 j = 0; j < n; j++) {
            m_pRttTable[i * n + j] = rttTable[i * n + j] << 2;
        }
    }
    for (u32 i = 0; i < n; i++) {
        u32 connectionBitmap = 0;
        for (u32 j = 0; j < n; j++) {
            if (m_pRttTable[i * n + j] != 0) {
                connectionBitmap |= 1 << j;
            }
        }
        m_pConnectionBitmaps[i] = connectionBitmap;
    }
    if (m_IsUsingBlockedTable) {
        for (u32 i = 0; i < n; i++) {
            for (u32 j = 0; j < n; j++) {
                if (m_pBlockedTable[i * n + j] == 1) {
                    m_pRttTable[i * n + j] = 0;
                }
            }
        }
    }
    for (u32 i = 1; i < n; i++) {
        for (u32 j = 0; j < i; j++) {
            u16 rtt0 = m_pRttTable[i * n + j];
            u16 rtt1 = m_pRttTable[j * n + i];
            u16 rtt = rtt0 != 0 && rtt1 != 0 ? (rtt1 + rtt0) >> 1 : 0;
            m_pRttTable[i * n + j] = rtt;
            m_pRttTable[j * n + i] = rtt;
        }
    }

    // the stations that stay and the ones whose routes are reset
    *pRefusedBitmap = 0;
    u32 stayBitmap = 0;
    u32 resetBitmap = 0;
    CalcStationType(stationBitmap, &stayBitmap, &resetBitmap);
    for (u32 i = 0; i < n; i++) {
        u32 bit = 1 << i;
        if ((bit & stayBitmap) && (m_RefusedStationBitmap & bit)) {
            stayBitmap &= ~bit;
            *pRefusedBitmap |= bit;
            resetBitmap |= bit;
        }
    }
    u32 stationNum = GetStationNum();
    for (u32 i = 0; i < stationNum; i++) {
        if (!(resetBitmap & (1 << i))) {
            continue;
        }
        for (u32 j = 0; j < stationNum; j++) {
            m_pRelayRouteTable[i * stationNum + j] = i;
            m_pRelayRouteTable[j * stationNum + i] = j;
        }
    }
    for (u32 i = 0; i < stationNum; i++) {
        for (u32 j = 0; j < stationNum; j++) {
            u8 relay = m_pRelayRouteTable[i * stationNum + j];
            if (i != relay && j != relay && (resetBitmap & (1 << relay))) {
                m_pRelayRouteTable[i * stationNum + j] = m_BrokenRoute;
            }
        }
    }
    ProcStayStation(stayBitmap, pRefusedBitmap);
    // (not used)
    bool isInvalidRoute = false;
    for (u32 i = 0; i < n; i++) {
        for (u32 j = 0; j < n; j++) {
            if (m_pRelayRouteTable[i * n + j] >= n) {
                isInvalidRoute = true;
            }
        }
    }
    (void)isInvalidRoute;

    // the new stations, the first one first
    u32 newBitmap = *pRefusedBitmap | stationBitmap;
    u32 routedBitmap = stayBitmap & ~*pRefusedBitmap;
    *pRefusedBitmap = 0;
    stationNum = GetStationNum();
    if (newBitmap & (1 << m_FirstStationIndex)) {
        ProcNewStationOne(m_FirstStationIndex, routedBitmap);
        u32 bit = 1 << m_FirstStationIndex;
        newBitmap -= bit;
        routedBitmap |= bit;
    }
    bool isProgress;
    do {
        isProgress = false;
        for (u32 i = 0; i < stationNum; i++) {
            u32 bit = 1 << i;
            if (!(bit & newBitmap)) {
                continue;
            }
            if (ProcNewStationOne(i, routedBitmap)) {
                isProgress = true;
                newBitmap -= bit;
                routedBitmap |= bit;
            }
        }
    } while (isProgress);
    if (stationNum != 0) {
        u32 refusedBitmap = *pRefusedBitmap;
        for (u32 i = 0; i < stationNum; i++) {
            if ((1 << i) & newBitmap) {
                refusedBitmap |= 1 << i;
            }
        }
        *pRefusedBitmap = refusedBitmap;
    }
    for (u32 i = 0; i < n; i++) {
        pReasons[i] = (*pRefusedBitmap & (1 << i)) ? m_pRefuseReasons[i] : 0;
    }
    RelayRouteOptimization(routedBitmap);
    ProcRefusedStation(*pRefusedBitmap, routedBitmap);

    // the stations with relayed routes
    m_RoutedStationBitmap = 0;
    stationNum = GetStationNum();
    for (u32 i = 0; i < stationNum; i++) {
        for (u32 j = 0; j < stationNum; j++) {
            if (m_pRelayRouteTable[i * stationNum + j] != i) {
                m_RoutedStationBitmap |= 1 << i;
                break;
            }
        }
    }
    m_RefusedStationBitmap = *pRefusedBitmap;
    nnnstdMemCpy(m_pOriginalRelayRouteTable, m_pRelayRouteTable, tableSize);
    return nn::Result();
}

// 0x004565F8 | fefates:bytes [tier B]
bool nn::pia::transport::RelayRouteManager::RelayRouteOptimizationRTTOne(unsigned int stationBitmap)
{
    // the slowest relayed route
    u32 n = GetStationNum();
    u32 rttMax = 0;
    u32 from;
    u32 to;
    u32 relay;
    for (u32 i = 0; i < n; i++) {
        if (!(stationBitmap & (1 << i))) {
            continue;
        }
        for (u32 j = i + 1; j < n; j++) {
            if (!(stationBitmap & (1 << j))) {
                continue;
            }
            u8 r = m_pRelayRouteTable[i * n + j];
            if (r == i || r == j) {
                continue;
            }
            u32 rtt = m_pRttTable[i * n + r] + m_pRttTable[r * n + j];
            if (rtt > rttMax) {
                rttMax = rtt;
                relay = r;
                from = i;
                to = j;
            }
        }
    }
    if (rttMax == 0) {
        return false;
    }

    // the faster relay stations, the fastest first
    u32 candidateNum = 0;
    u32 candidates[STATION_INDEX_MAX + 1][2] = {};
    for (u32 k = 0; k < n; k++) {
        if (!(stationBitmap & (1 << k))) {
            continue;
        }
        u16 rtt0 = m_pRttTable[from * n + k];
        if (rtt0 == 0) {
            continue;
        }
        u16 rtt1 = m_pRttTable[k * n + to];
        if (rtt1 == 0) {
            continue;
        }
        u32 rtt = rtt1 + rtt0;
        if (rttMax <= rtt || rtt > m_RttLimit) {
            continue;
        }
        candidates[candidateNum][0] = k;
        candidates[candidateNum][1] = rtt;
        candidateNum++;
    }
    if (candidateNum == 0) {
        return false;
    }
    for (u32 pass = candidateNum - 1; pass != 0; pass--) {
        for (u32 j = 0; j < pass; j++) {
            if (candidates[j][1] > candidates[j + 1][1]) {
                for (u32 k = 0; k < 2; k++) {
                    u8 temp = candidates[j + 1][k];
                    candidates[j + 1][k] = candidates[j][k];
                    candidates[j][k] = temp;
                }
            }
        }
    }
    for (u32 m = 0; m < candidateNum; m++) {
        u32 k = candidates[m][0];
        if (m_pRelayCount[k] + 2 > m_RelayCountMax) {
            // full: only after moving one of its routes
            if (m_pRelayCount[k] > m_RelayCountMax) {
                continue;
            }
            m_pRelayCount[relay] -= 2;
            bool isSwitched = SwitchRelay(k, stationBitmap, rttMax - 1, m_RelayCountMax);
            m_pRelayCount[relay] += 2;
            if (!isSwitched) {
                continue;
            }
        }
        m_pRelayRouteTable[from * n + to] = k;
        m_pRelayRouteTable[to * n + from] = k;
        m_pRelayCount[relay] -= 2;
        m_pRelayCount[k] += 2;
        return true;
    }
    return false;
}

// 0x004568D0 | fefates:bytes [tier B]
bool nn::pia::transport::RelayRouteManager::DecideBrokenRouteRelayStation(unsigned int stationBitmap, unsigned int* pRefusedBitmap)
{
    // the stations by their number of broken routes
    u32 n = GetStationNum();
    u8 brokenNums[STATION_INDEX_MAX + 1][2];
    for (u32 i = 0; i < n; i++) {
        u8 brokenNum = 0;
        for (u32 j = 0; j < n; j++) {
            if (m_pRelayRouteTable[i * n + j] == m_BrokenRoute) {
                brokenNum++;
            }
            if (m_pRelayRouteTable[j * n + i] == m_BrokenRoute) {
                brokenNum++;
            }
        }
        brokenNums[i][0] = i;
        brokenNums[i][1] = brokenNum;
    }
    for (u32 pass = n - 1; pass != 0; pass--) {
        for (u32 j = 0; j < pass; j++) {
            if (brokenNums[j][1] > brokenNums[j + 1][1]) {
                for (u32 k = 0; k < 2; k++) {
                    u8 temp = brokenNums[j][k];
                    brokenNums[j][k] = brokenNums[j + 1][k];
                    brokenNums[j + 1][k] = temp;
                }
            }
        }
    }
    u8 relayNums[STATION_INDEX_MAX + 1][2];
    for (u32 i = 0; i < n; i++) {
        relayNums[i][0] = brokenNums[i][0];
        relayNums[i][1] = brokenNums[i][1];
    }

    for (u32 a = 1; a < n; a++) {
        u8 station0 = brokenNums[a][0];
        for (u32 b = 0; b < a; b++) {
            u8 station1 = brokenNums[b][0];
            if (!(stationBitmap & (1 << station0)) || !(stationBitmap & (1 << station1)) ||
                m_pRelayRouteTable[station0 * n + station1] != m_BrokenRoute) {
                continue;
            }

            // a relay station (the ones with few broken routes first)
            u8 reason = 1;
            bool isFound = false;
            for (u32 c = 0; c < n; c++) {
                u8 relay = relayNums[c][0];
                if (relay == station0 || relay == station1 || !(stationBitmap & (1 << relay)) || (*pRefusedBitmap & (1 << relay))) {
                    continue;
                }
                u32 stationNum = GetStationNum();
                u16 rtt0 = m_pRttTable[station0 * stationNum + relay];
                if (rtt0 == 0) {
                    continue;
                }
                u16 rtt1 = m_pRttTable[relay * stationNum + station1];
                if (rtt1 == 0) {
                    continue;
                }
                if (rtt0 + rtt1 > m_RttLimit) {
                    reason = 2;
                    continue;
                }
                if (m_pRelayCount[relay] + 2 > m_RelayCountMax) {
                    reason = 3;
                    continue;
                }
                isFound = true;
                m_pRelayRouteTable[station0 * n + station1] = relay;
                m_pRelayRouteTable[station1 * n + station0] = relay;
                for (u32 k = 0; k < n; k++) {
                    if (relayNums[k][0] == station0 || relayNums[k][0] == station1) {
                        relayNums[k][1] -= 2;
                    }
                }
                for (u32 pass = n - 1; pass != 0; pass--) {
                    for (u32 j = 0; j < pass; j++) {
                        if (relayNums[j][1] > relayNums[j + 1][1]) {
                            for (u32 k = 0; k < 2; k++) {
                                u8 temp = relayNums[j][k];
                                relayNums[j][k] = relayNums[j + 1][k];
                                relayNums[j + 1][k] = temp;
                            }
                        }
                    }
                }
                m_pRelayCount[relay] += 2;
                break;
            }
            if (isFound) {
                continue;
            }

            // a busy relay station, after moving one of its routes
            u32 bitmap = stationBitmap & ~*pRefusedBitmap;
            u32 stationNum = GetStationNum();
            u32 candidateBitmap = 0;
            for (u32 k = 0; k < stationNum; k++) {
                u32 bit = 1 << k;
                if (!(bit & bitmap)) {
                    continue;
                }
                u16 rtt0 = m_pRttTable[station0 * stationNum + k];
                if (rtt0 == 0) {
                    continue;
                }
                u16 rtt1 = m_pRttTable[k * stationNum + station1];
                if (rtt1 == 0 || rtt0 + rtt1 > m_RttLimit || m_pRelayCount[k] > m_RelayCountMax) {
                    continue;
                }
                candidateBitmap |= bit;
            }
            for (u32 k = 0; k < stationNum; k++) {
                if (!(candidateBitmap & (1 << k))) {
                    continue;
                }
                if (SwitchRelay(k, bitmap, m_RttLimit, m_RelayCountMax)) {
                    m_pRelayRouteTable[station0 * stationNum + station1] = k;
                    m_pRelayRouteTable[station1 * stationNum + station0] = k;
                    m_pRelayCount[k] += 2;
                    isFound = true;
                    break;
                }
            }
            if (isFound) {
                continue;
            }

            // no relay station: one of the two is refused (not the first station, else the one
            // with more broken routes)
            u8 refused = 0;
            if (m_FirstStationIndex == station0) {
                refused = station1;
            } else if (m_FirstStationIndex == station1) {
                refused = station0;
            } else {
                for (u32 c = 0; c < n; c++) {
                    if (relayNums[c][0] == station0) {
                        refused = station1;
                        break;
                    }
                    if (relayNums[c][0] == station1) {
                        refused = station0;
                        break;
                    }
                }
            }
            *pRefusedBitmap |= 1 << refused;
            m_pRefuseReasons[refused] = reason;
            for (u32 i = 0; i < n; i++) {
                for (u32 t = 0; t < 2; t++) {
                    u32 x = t == 0 ? refused : i;
                    u32 y = t == 0 ? i : refused;
                    u8 relay = m_pRelayRouteTable[x * n + y];
                    if (relay != x && relay != y && relay < n) {
                        m_pRelayCount[relay]--;
                    }
                    m_pRelayRouteTable[x * n + y] = x;
                }
            }
            for (u32 i = 0; i < n; i++) {
                for (u32 j = 0; j < n; j++) {
                    u8 relay = m_pRelayRouteTable[i * n + j];
                    if (relay != i && relay != j && relay == refused) {
                        m_pRelayRouteTable[i * n + j] = m_BrokenRoute;
                    }
                }
            }
            return false;
        }
    }
    return true;
}

// 0x00456F3C | fefates:bytes [tier B]
nn::Result nn::pia::transport::RelayRouteManager::UpdateRttBetweenRelayNodeData(const unsigned char* rttTable, unsigned int size)
{
    u32 n = GetStationNum();
    if (size != n || !common::IsValidPointer(rttTable)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    for (u32 i = 0; i < n; i++) {
        m_pRelayNodeRttTable[i] = rttTable[i] << 2;
    }
    return nn::Result();
}

// 0x00456FB8 (name is ours)
nn::Result nn::pia::transport::RelayRouteManager::SetRelayRouteDirections(const unsigned char* pData, unsigned int size)
{
    u32 n = GetStationNum();
    u32 tableSize = n * n;
    u32 bitNum = m_RouteBits * tableSize;
    if (bitNum & 7) {
        bitNum += 8 - (bitNum & 7);
    }
    u32 routeSize = bitNum >> 3;
    if (routeSize + n * sizeof(u32) + sizeof(u32) > size || !common::IsValidPointer(pData)) {
        return common::RESULT_INVALID_ARGUMENT;
    }

    // the routes, m_RouteBits each, from the high bits of the bytes on
    u32 position = 0;
    u8 bitOffset = 0;
    for (u32 i = 0; i < n; i++) {
        for (u32 j = 0; j < n; j++) {
            u8 bits = m_RouteBits;
            s8 shift = 8 - bitOffset - bits;
            u8 route = 0;
            if (shift < 0) {
                // the route continues in the next byte
                route = (pData[position] & ((1 << (bits + shift)) - 1)) << -shift;
                bitOffset = 0;
                position++;
                shift += 8;
            }
            route += (pData[position] >> shift) & ((1 << (8 - shift - bitOffset)) - 1);
            m_pRelayRouteTable[i * n + j] = route;
            bitOffset = 8 - shift;
            if (shift == 0) {
                bitOffset = 0;
                position++;
            }
        }
    }
    position = routeSize;
    for (u32 i = 0; i < n; i++) {
        m_pConnectionBitmaps[i] = (pData[position] << 24) + (pData[position + 1] << 16) + (pData[position + 2] << 8) + pData[position + 3];
        position += sizeof(u32);
    }
    nnnstdMemCpy(&m_RefusedStationBitmap, pData + position, sizeof(u32));

    m_RoutedStationBitmap = 0;
    u32 stationNum = GetStationNum();
    for (u32 i = 0; i < stationNum; i++) {
        for (u32 j = 0; j < stationNum; j++) {
            if (m_pRelayRouteTable[i * stationNum + j] != i) {
                m_RoutedStationBitmap |= 1 << i;
                break;
            }
        }
    }
    nnnstdMemCpy(m_pOriginalRelayRouteTable, m_pRelayRouteTable, tableSize);
    return nn::Result();
}

// 0x004571E8 | fefates:bytes [tier B]
void nn::pia::transport::RelayRouteManager::Finalize()
{
    if (m_pRttTable != nullptr) {
        common::DeleteArray(m_pRttTable);
        m_pRttTable = nullptr;
    }
    if (m_pRelayNodeRttTable != nullptr) {
        common::DeleteArray(m_pRelayNodeRttTable);
        m_pRelayNodeRttTable = nullptr;
    }
    if (m_pRelayRouteTable != nullptr) {
        common::DeleteArray(m_pRelayRouteTable);
        m_pRelayRouteTable = nullptr;
    }
    if (m_pRelayCount != nullptr) {
        common::DeleteArray(m_pRelayCount);
        m_pRelayCount = nullptr;
    }
    if (m_pConnectionBitmaps != nullptr) {
        common::DeleteArray(m_pConnectionBitmaps);
        m_pConnectionBitmaps = nullptr;
    }
    if (m_IsUsingBlockedTable && m_pBlockedTable != nullptr) {
        common::DeleteArray(m_pBlockedTable);
        m_pBlockedTable = nullptr;
    }
    if (m_pRefuseReasons != nullptr) {
        common::DeleteArray(m_pRefuseReasons);
        m_pRefuseReasons = nullptr;
    }
    if (m_pOriginalRelayRouteTable != nullptr) {
        common::DeleteArray(m_pOriginalRelayRouteTable);
        m_pOriginalRelayRouteTable = nullptr;
    }
}

// 0x0045731C (name is ours)
void nn::pia::transport::RelayRouteManager::SetRttLimit(u16 rttLimit)
{
    m_RttLimit = rttLimit;
    common::g_SessionBeginMonitoringContent.m_RelayRttLimit = rttLimit;
}

// 0x00457330 | fefates:bytes [tier B]
nn::pia::transport::RelayRouteManager::RelayRouteManager()
    : m_pRttTable(nullptr), m_pRelayNodeRttTable(nullptr), m_pRelayRouteTable(nullptr), m_pRelayCount(nullptr),
      m_pConnectionBitmaps(nullptr), m_pBlockedTable(nullptr), m_pRefuseReasons(nullptr), m_RttLimit(500), m_RelayCountMax(144),
      m_DirectionsVersionLow(0), m_DirectionsVersionHigh(0), m_FirstStationIndex(STATION_INDEX_UNIDENTIFIED), m_RouteBits(5), m_BrokenRoute(255),
      m_Unknown0x2B(0), m_RoutedStationBitmap(0), m_RefusedStationBitmap(0), m_pOriginalRelayRouteTable(nullptr)
{
}

// 0x007356DC | fefates:bytes [tier B]
nn::Result nn::pia::transport::RelayRouteManager::GetRelayRoute(nn::pia::StationIndex from, nn::pia::StationIndex to, nn::pia::StationIndex* pRelay) const
{
    u32 n = GetStationNum();
    if (from >= n || to >= n || !common::IsValidPointer(pRelay)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    *pRelay = static_cast<StationIndex>(m_pRelayRouteTable[from * n + to]);
    return nn::Result();
}

// 0x00735738 | fefates:bytes [tier B]
nn::Result nn::pia::transport::RelayRouteManager::GetDestStationList(nn::pia::StationIndex from, nn::pia::StationIndex relay, unsigned int* pBitmap) const
{
    u32 n = GetStationNum();
    if (from >= n || relay >= n || !common::IsValidPointer(pBitmap)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    *pBitmap = 0;
    const u8* pRoutes = &m_pRelayRouteTable[from * n];
    u32 bit = 1;
    for (u32 i = n; i != 0; i--) {
        if (*pRoutes++ == relay) {
            *pBitmap |= bit;
        }
        bit <<= 1;
    }
    return nn::Result();
}

// 0x007357C4 | fefates:bytes [tier B]
nn::Result nn::pia::transport::RelayRouteManager::GetDirectStationList(nn::pia::StationIndex from, unsigned int* pBitmap) const
{
    u32 n = GetStationNum();
    if (from >= n || !common::IsValidPointer(pBitmap)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    *pBitmap = 0;
    for (u32 i = 0; i < n; i++) {
        if (m_pRelayRouteTable[from * n + i] == i) {
            *pBitmap |= 1 << i;
        }
    }
    return nn::Result();
}

// 0x00735854 | fefates:bytes [tier B]
nn::Result nn::pia::transport::RelayRouteManager::GetOriginalRelayRoute(nn::pia::StationIndex from, nn::pia::StationIndex to, nn::pia::StationIndex* pRelay) const
{
    u32 n = GetStationNum();
    if (from >= n || to >= n || !common::IsValidPointer(pRelay)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    *pRelay = static_cast<StationIndex>(m_pOriginalRelayRouteTable[from * n + to]);
    return nn::Result();
}

// 0x007358B0 | fefates:bytes [tier B]
u32 nn::pia::transport::RelayRouteManager::GetRelayRouteDirectionsSize() const
{
    u32 n = GetStationNum();
    u32 bitNum = m_RouteBits * (n * n);
    if (bitNum & 7) {
        bitNum += 8 - (bitNum & 7);
    }
    return (bitNum >> 3) + n * sizeof(u32) + sizeof(u32);
}

// 0x007358F0 | fefates:bytes [tier B]
nn::Result nn::pia::transport::RelayRouteManager::GetOriginalDirectStationList(nn::pia::StationIndex from, unsigned int* pBitmap) const
{
    u32 n = GetStationNum();
    if (from >= n || !common::IsValidPointer(pBitmap)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    *pBitmap = 0;
    for (u32 i = 0; i < n; i++) {
        if (m_pOriginalRelayRouteTable[from * n + i] == i) {
            *pBitmap |= 1 << i;
        }
    }
    return nn::Result();
}

// 0x00735980 (name is ours)
nn::Result nn::pia::transport::RelayRouteManager::GetRelayRouteDirections(unsigned char*, unsigned int, unsigned int*) const
{
}

} // namespace transport
} // namespace pia
} // namespace nn
