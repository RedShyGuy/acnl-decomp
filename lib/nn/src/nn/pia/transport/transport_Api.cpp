#include "nn/pia/transport/transport_Api.h"
#include "nn/pia/common/common_HeapManager.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/transport/transport_Transport.h"
#include "pead/peadExpHeap.h"
#include "pead/peadSafeStringBase.h"

namespace nn {
namespace pia {
namespace transport {
namespace {
// 0x00975A88
bool s_IsInitialized;
// 0x00975A89
bool s_IsInSetupMode;
} // namespace

// 0x0044D32C | fefates:bytes [tier B]
nn::Result BeginSetup()
{
    if (!s_IsInitialized) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (s_IsInSetupMode) {
        return common::RESULT_INVALID_STATE;
    }
    common::HeapManager::SetCurrentHeap(MODULE_TYPE_TRANSPORT);
    s_IsInSetupMode = true;
    return nn::Result();
}

// 0x0044D378 | fefates:bytes [tier B]
nn::Result Initialize()
{
    if (s_IsInitialized) {
        return common::RESULT_ALREADY_INITIALIZED;
    }
    common::HeapManager::Setup(MODULE_TYPE_TRANSPORT, 0, pead::SafeStringBase<char>("pia transport heap"));
    s_IsInitialized = true;
    return nn::Result();
}

// 0x0044DE74 | fefates:callgraph [tier C]
bool IsInSetupMode()
{
    return s_IsInSetupMode;
}

// 0x0044DE84 | fefates:callgraph [tier C]
bool IsInitialized()
{
    return s_IsInitialized;
}

// 0x0044EE84 | fefates:callseq [tier C]
StationId Conv2StationId(nn::pia::StationIndex stationIndex)
{
    if (common::IsValidPointer(Transport::s_pInstance)) {
        StationId stationId;
        if (Transport::s_pInstance->ConvertToStationId(&stationId, stationIndex).IsSuccess()) {
            return stationId;
        }
    }
    return GetStationIdOfIndex253();
}

// 0x00454150 | fefates:bytes [tier B]
StationIndex Conv2StationIndex(nn::pia::StationId stationId)
{
    if (common::IsValidPointer(Transport::s_pInstance)) {
        StationIndex stationIndex;
        if (Transport::s_pInstance->ConvertToStationIndex(&stationIndex, stationId).IsSuccess()) {
            return stationIndex;
        }
    }
    return STATION_INDEX_UNIDENTIFIED;
}

// 0x0045F8FC | fefates:bytes [tier B]
nn::Result EndSetup()
{
    if (!s_IsInitialized) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (!s_IsInSetupMode) {
        return common::RESULT_INVALID_STATE;
    }
    common::HeapManager::GetHeap(MODULE_TYPE_TRANSPORT)->adjust();
    common::HeapManager::ClearCurrentHeap();
    s_IsInSetupMode = false;
    return nn::Result();
}

// 0x0045F958 | fefates:bytes [tier B]
void Finalize()
{
    if (!s_IsInitialized) {
        return;
    }
    if (s_IsInSetupMode) {
        common::HeapManager::GetHeap(MODULE_TYPE_TRANSPORT)->adjust();
        common::HeapManager::ClearCurrentHeap();
        s_IsInSetupMode = false;
    }
    if (common::IsValidPointer(Transport::s_pInstance)) {
        Transport::DestroyInstance();
    }
    common::HeapManager::Cleanup(MODULE_TYPE_TRANSPORT);
    s_IsInitialized = false;
}

} // namespace transport
} // namespace pia
} // namespace nn
