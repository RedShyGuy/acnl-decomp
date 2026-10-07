#include "nn/pia/transport/transport_AttendanceTable.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/transport/transport_Api.h"

namespace nn {
namespace pia {
namespace transport {
// 0x0097E44C
nn::pia::transport::AttendanceTable* nn::pia::transport::AttendanceTable::s_pInstance;

// 0x0044FEC0 | fefates:bytes [tier B]
nn::Result nn::pia::transport::AttendanceTable::Initialize()
{
    if (m_IsInitialized) {
        return common::RESULT_ALREADY_INITIALIZED;
    }
    m_AttendanceBitmap = 0;
    m_IsInitialized = true;
    m_IsStarted = false;
    return nn::Result();
}

// 0x0044FEF0 | fefates:bytes [tier B]
nn::Result nn::pia::transport::AttendanceTable::CreateInstance()
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
    s_pInstance = new AttendanceTable;
    return nn::Result();
}

// 0x0044FF6C | fefates:callseq [tier C]
void nn::pia::transport::AttendanceTable::DestroyInstance()
{
    if (s_pInstance != nullptr) {
        delete s_pInstance;
        s_pInstance = nullptr;
    }
}

// 0x0044FF94 | fefates:bytes [tier B]
nn::Result nn::pia::transport::AttendanceTable::Update(bool isAttending, nn::pia::StationIndex stationIndex)
{
    if (stationIndex > STATION_INDEX_MAX) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (!m_IsInitialized) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (!m_IsStarted) {
        return common::RESULT_INVALID_STATE;
    }
    if (isAttending) {
        m_AttendanceBitmap |= 1u << stationIndex;
    } else {
        m_AttendanceBitmap &= ~(1u << stationIndex);
    }
    return nn::Result();
}

// 0x0044FFF0 | fefates:callseq [tier C]
void nn::pia::transport::AttendanceTable::Cleanup()
{
    m_IsStarted = false;
}

// 0x0044FFFC | fefates:bytes [tier B]
nn::Result nn::pia::transport::AttendanceTable::Startup()
{
    if (!m_IsInitialized) {
        return common::RESULT_NOT_INITIALIZED;
    }
    if (m_IsStarted) {
        return common::RESULT_INVALID_STATE;
    }
    m_AttendanceBitmap = 0;
    m_IsStarted = true;
    return nn::Result();
}

// 0x0045003C | fefates:callseq [tier C]
void nn::pia::transport::AttendanceTable::Finalize()
{
    m_IsInitialized = false;
}

// 0x007352E8 slot 0x00
void nn::pia::transport::AttendanceTable::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace transport
} // namespace pia
} // namespace nn
