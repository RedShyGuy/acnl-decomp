#include "nn/hid/CTR/hid_AccelerometerReader.h"
#include "nn/hid/CTR/detail/detail_Api.h"
#include "nn/hid/CTR/detail/hid_Ipc.h"
#include "nn/hid/CTR/hid_Devices.h"
#include "nn/hidlow/CTR/hidlow_AccelerometerLifoRing.h"
#include "nn/math/math_ARMv6.h"
#include "nn/math/math_Vector3.h"

namespace nn {
namespace hid {
namespace CTR {
namespace {
// the units of the raw values (1/512 G)
const f32 ACCELERATION_UNIT = 1.0f / 512;
// the defaults (values from the binary)
const s16 DEFAULT_SENSITIVITY = 128;

inline bool IsIdentity(const nn::math::MTX34& mtx)
{
    return mtx.m[0][0] == 1.0f && mtx.m[0][1] == 0.0f && mtx.m[0][2] == 0.0f && mtx.m[0][3] == 0.0f &&
           mtx.m[1][0] == 0.0f && mtx.m[1][1] == 1.0f && mtx.m[1][2] == 0.0f && mtx.m[1][3] == 0.0f &&
           mtx.m[2][0] == 0.0f && mtx.m[2][1] == 0.0f && mtx.m[2][2] == 1.0f && mtx.m[2][3] == 0.0f;
}
} // namespace

inline void nn::hid::CTR::AccelerometerReader::Smooth(AccelerometerStatus* pStatus)
{
    pStatus->x = m_Last[0] = detail::CalculateAccelerationTightly(pStatus->x, m_Last[0], m_Play, m_Sensitivity);
    pStatus->y = m_Last[1] = detail::CalculateAccelerationTightly(pStatus->y, m_Last[1], m_Play, m_Sensitivity);
    pStatus->z = m_Last[2] = detail::CalculateAccelerationTightly(pStatus->z, m_Last[2], m_Play, m_Sensitivity);
}

// 0x00353C94 | nintendogs:bytes [tier B]
bool nn::hid::CTR::AccelerometerReader::ReadLatest(AccelerometerStatus* pStatus)
{
    int readCount;
    s64 lastTick = -1;
    int lastIndex = -1;
    m_pAccelerometer->m_pRing->ReadData(pStatus, 1, &readCount, &lastTick, &lastIndex);
    if (readCount <= 0) {
        return false;
    }
    Smooth(pStatus);
    Transform(pStatus);
    return true;
}

// 0x00353D58 | nintendogs:bytes [tier B]
void nn::hid::CTR::AccelerometerReader::ConvertToAcceleration(AccelerationFloat* pAcceleration, int count, const AccelerometerStatus* pStatus)
{
    for (int i = 0; i < count; i++) {
        pAcceleration[i].x = pStatus[i].x * ACCELERATION_UNIT;
        pAcceleration[i].y = pStatus[i].y * ACCELERATION_UNIT;
        pAcceleration[i].z = pStatus[i].z * ACCELERATION_UNIT;
    }
}

// 0x00353DD4 (name is ours)
void nn::hid::CTR::AccelerometerReader::ResetAxisRotation()
{
    m_AxisRotation = nn::math::MTX34::Identity();
}

// 0x00353E7C | nintendogs:bytes [tier B]
void nn::hid::CTR::AccelerometerReader::Read(AccelerometerStatus* pBuffer, int* pReadCount, int count)
{
    m_pAccelerometer->m_pRing->ReadData(pBuffer, count, pReadCount, &m_LastTick, &m_LastIndex);
    for (int i = *pReadCount - 1; i >= 0; i--) {
        Smooth(&pBuffer[i]);
        Transform(&pBuffer[i]);
    }
}

// 0x003540A4 | nintendogs:bytes [tier B]
void nn::hid::CTR::AccelerometerReader::Transform(AccelerometerStatus* pStatus)
{
    if (m_IsOffsetEnabled) {
        pStatus->x -= m_Offset[0];
        pStatus->y -= m_Offset[1];
        pStatus->z -= m_Offset[2];
    }
    if (m_IsAxisRotationEnabled && !IsIdentity(m_AxisRotation)) {
        nn::math::VEC3 v;
        v.x = pStatus->x;
        v.y = pStatus->y;
        v.z = pStatus->z;
        nn::math::ARMv6::VEC3TransformAsm(&v, &m_AxisRotation, &v);
        pStatus->x = static_cast<s32>(v.x);
        pStatus->y = static_cast<s32>(v.y);
        pStatus->z = static_cast<s32>(v.z);
    }
}

// 0x00354220 | nintendogs:bytes-fuzzy [tier B]
nn::hid::CTR::AccelerometerReader::AccelerometerReader(Accelerometer& accelerometer)
    : m_pAccelerometer(&accelerometer), m_Play(0), m_Sensitivity(DEFAULT_SENSITIVITY), m_IsOffsetEnabled(false),
      m_IsAxisRotationEnabled(false), m_LastIndex(-1), m_LastTick(-1)
{
    detail::Ipc::EnableAccelerometer();
    m_Last[0] = 0;
    m_Last[1] = 0;
    m_Last[2] = 0;
    m_Offset[0] = 0;
    m_Offset[1] = 0;
    m_Offset[2] = 0;
    ResetAxisRotation();
    m_IsOffsetEnabled = false;
    m_IsAxisRotationEnabled = false;
    AccelerometerStatus status;
    int readCount;
    Read(&status, &readCount, 1);
}

// 0x003542A8 | tier C
nn::hid::CTR::AccelerometerReader::~AccelerometerReader()
{
    detail::Ipc::DisableAccelerometer();
}

} // namespace CTR
} // namespace hid
} // namespace nn
