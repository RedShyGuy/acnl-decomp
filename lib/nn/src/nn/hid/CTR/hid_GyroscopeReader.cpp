#include "nn/hid/CTR/hid_GyroscopeReader.h"
#include <math.h>
#include <string.h>
#include "nn/hid/CTR/detail/hid_Ipc.h"
#include "nn/hid/CTR/hid_Gyroscope.h"
#include "nn/hidlow/CTR/hidlow_GyroscopeLowLifoRing.h"
#include "nn/math/math_ARMv6.h"
#include "nn/math/math_MTX33.h"
#include "nn/os/os_Tick.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace hid {
namespace CTR {
namespace {
// the samples of the first Read: one per 100 ms since the start (value from the binary)
const s64 FIRST_READ_INTERVAL_MSEC = 100;
// the correction of the turn per update
const f32 DIRECTION_CORRECTION_FACTOR = 0.0001f;
// NormalizeDirection repeats until the rows are orthonormal enough
const f32 NORMALIZE_THRESHOLD = 2.999f;

inline int Round(f64 value)
{
    return static_cast<int>(value < 0.0 ? value - 0.5 : value + 0.5);
}

inline int Round(f32 value)
{
    return static_cast<int>(value < 0.0f ? value - 0.5f : value + 0.5f);
}

inline void NormalizeRow(f32* row)
{
    f32 inverse = 1.0f / sqrtf(row[0] * row[0] + row[1] * row[1] + row[2] * row[2]);
    row[0] *= inverse;
    row[1] *= inverse;
    row[2] *= inverse;
}

inline void Cross(f32* out, const f32* a, const f32* b)
{
    out[0] = a[1] * b[2] - a[2] * b[1];
    out[1] = a[2] * b[0] - a[0] * b[2];
    out[2] = a[0] * b[1] - a[1] * b[0];
}

inline bool IsIdentity(const nn::math::MTX34& mtx)
{
    return mtx.m[0][0] == 1.0f && mtx.m[0][1] == 0.0f && mtx.m[0][2] == 0.0f && mtx.m[0][3] == 0.0f &&
           mtx.m[1][0] == 0.0f && mtx.m[1][1] == 1.0f && mtx.m[1][2] == 0.0f && mtx.m[1][3] == 0.0f &&
           mtx.m[2][0] == 0.0f && mtx.m[2][1] == 0.0f && mtx.m[2][2] == 1.0f && mtx.m[2][3] == 0.0f;
}
} // namespace

// the readers and the tick of the start of the gyroscope (set by the constructor, which is not
// in this program; names are ours)
// 0x0097FA68
int s_GyroscopeReaderCount;
// 0x0097FA70
nn::os::Tick s_GyroscopeStartTick;
// the defaults of ReviseDirectionWithAcceleration
// 0x00AF6204
nn::math::VEC3 s_ZeroVector = nn::math::VEC3::Zero();
// 0x00AF6210
nn::math::MTX33 s_IdentityMatrix = nn::math::MTX33::Identity();

inline int nn::hid::CTR::GyroscopeReader::ConvertRawValue(s16 raw, int axis) const
{
    f64 value = (static_cast<f64>(raw) - static_cast<f64>(m_RawZero[axis])) * m_RawScale[axis];
    return Round(value * static_cast<f64>(m_Sensitivity[axis]));
}

inline f32 nn::hid::CTR::GyroscopeReader::CalculateRotation(f32 speed) const
{
    f32 turn = speed * m_DirectionMagnification;
    f32 correction = m_DirectionCorrection * turn;
    return m_DirectionScale * ((1.0f + correction * correction * DIRECTION_CORRECTION_FACTOR) * turn);
}

// 0x00352808 | nintendogs:bytes-fuzzy [tier B]
bool nn::hid::CTR::GyroscopeReader::ReadLatest(GyroscopeStatus* pStatus)
{
    if (m_HistoryCount == 0) {
        Read(m_History, &m_HistoryCount, HISTORY_NUM);
    } else {
        GyroscopeStatus buffer[HISTORY_NUM];
        int readCount = 0;
        Read(buffer, &readCount, HISTORY_NUM);
        int keepCount = HISTORY_NUM - readCount;
        if (keepCount > m_HistoryCount) {
            keepCount = m_HistoryCount;
        }
        m_HistoryCount = keepCount;
        memcpy(&m_History[readCount], &m_History[0], keepCount * sizeof(GyroscopeStatus));
        memcpy(&m_History[0], buffer, readCount * sizeof(GyroscopeStatus));
        m_HistoryCount += readCount;
    }
    if (m_LastTick == -1) {
        return false;
    }
    *pStatus = m_Status;
    return true;
}

// 0x00352934 | nintendogs:bytes [tier B]
void nn::hid::CTR::GyroscopeReader::CalculateDirection()
{
    f32 (*direction)[3] = m_Status.direction;
    f32 last[3][3];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            last[i][j] = direction[i][j];
        }
    }
    f32 turn = CalculateRotation(m_Status.speed[0]);
    for (int j = 0; j < 3; j++) {
        direction[1][j] += turn * last[2][j];
    }
    for (int j = 0; j < 3; j++) {
        direction[2][j] -= turn * last[1][j];
    }
    turn = CalculateRotation(m_Status.speed[1]);
    for (int j = 0; j < 3; j++) {
        direction[2][j] += turn * last[0][j];
    }
    for (int j = 0; j < 3; j++) {
        direction[0][j] -= turn * last[2][j];
    }
    turn = CalculateRotation(m_Status.speed[2]);
    for (int j = 0; j < 3; j++) {
        direction[0][j] += turn * last[1][j];
    }
    for (int j = 0; j < 3; j++) {
        direction[1][j] -= turn * last[0][j];
    }
    NormalizeDirection(direction, NORMALIZE_THRESHOLD);
}

// 0x00352B00 | nintendogs:bytes [tier B]
void nn::hid::CTR::GyroscopeReader::CalculateGyroscopeAxisStatus(f32* pSpeed, int* pStableCount, f32* pZeroPoint, int value, f32 unit, int* pSamples)
{
    pSamples[m_SampleIndex] = value;
    *pStableCount = 1;
    int range = Round(m_DriftRange / unit);
    if (range == 0) {
        range = 1;
    }
    int low = value - range;
    int high = value + range;
    *pSpeed = static_cast<f32>(value);

    // the recent samples within the range of this one
    int sum = value;
    u32 end = (m_SampleIndex - m_DriftSampleNum) & (SAMPLE_NUM - 1);
    u32 index = (m_SampleIndex - 1) & (SAMPLE_NUM - 1);
    do {
        int sample = pSamples[index];
        if (sample < low || high < sample) {
            break;
        }
        index = (index - 1) & (SAMPLE_NUM - 1);
        sum += sample;
        (*pStableCount)++;
    } while (index != end);

    // the longer the speed is stable, the more it follows the average
    f32 stable = static_cast<f32>(*pStableCount - 1) / static_cast<f32>(m_DriftSampleNum - 1);
    stable = stable * stable;
    stable = stable * stable;
    f32 average = static_cast<f32>(sum) / static_cast<f32>(*pStableCount);
    stable = stable * stable;
    stable = stable * stable;
    stable = stable * stable;
    *pSpeed += (average - *pSpeed) * stable;
    if (m_IsZeroDriftEnabled) {
        *pZeroPoint += (*pSpeed - *pZeroPoint) * (m_DriftRevisePower * stable);
    }
    f32 speed = (*pSpeed - *pZeroPoint) * unit;
    *pSpeed = speed;
    if (m_IsZeroPlayEnabled) {
        if (speed < -m_ZeroPlay || speed > m_ZeroPlay) {
            m_ZeroPlayState = 0.0f;
            return;
        }
        if (0.0f > speed) {
            speed = -speed;
        }
        f32 state = 1.0f - speed / m_ZeroPlay;
        if (m_ZeroPlayState < state) {
            state = m_ZeroPlayState;
        }
        m_ZeroPlayState = state;
        *pSpeed = 0.0f;
    }
}

// 0x00352CBC (name is ours)
f32 nn::hid::CTR::GyroscopeReader::ReviseDirectionWithAcceleration(f32 (*direction)[3], const nn::math::VEC3* pAcceleration)
{
    f32 accX = pAcceleration->x;
    f32 accY = pAcceleration->y;
    f32 accZ = pAcceleration->z;
    f32 accLength = sqrtf(accX * accX + accY * accY + accZ * accZ);
    if (accLength == 0.0f) {
        return 0.0f;
    }
    // only near 1 G: the weight falls to 0 at 1 G +- m_AccReviseRange
    f32 weight;
    if (accLength < 1.0f) {
        f32 edge = 1.0f - m_AccReviseRange;
        if (edge >= accLength) {
            return 0.0f;
        }
        weight = (1.0f / m_AccReviseRange) * (accLength - edge);
    } else {
        f32 edge = m_AccReviseRange + 1.0f;
        if (edge <= accLength) {
            return 0.0f;
        }
        weight = (-1.0f / m_AccReviseRange) * (accLength - edge);
    }

    // gravity in the coordinates of the orientation, moved towards -Y
    f32 inverse = 1.0f / accLength;
    f32 power = m_AccRevisePower * (weight * weight);
    f32 normalX = accX * inverse;
    f32 normalY = accY * inverse;
    f32 normalZ = accZ * inverse;
    f32 gravityX = normalX * direction[0][0] + normalY * direction[1][0] + normalZ * direction[2][0];
    f32 gravityY = normalX * direction[0][1] + normalY * direction[1][1] + normalZ * direction[2][1];
    f32 gravityZ = normalX * direction[0][2] + normalY * direction[1][2] + normalZ * direction[2][2];
    f32 targetX = gravityX - gravityX * power;
    f32 targetY = gravityY + (-1.0f - gravityY) * power;
    f32 targetZ = gravityZ - gravityZ * power;
    f32 targetSquare = targetX * targetX + targetY * targetY + targetZ * targetZ;
    nn::math::VEC3 target;
    if (targetSquare == 0.0f) {
        target = s_ZeroVector;
    } else {
        f32 targetInverse = 1.0f / sqrtf(targetSquare);
        target.x = targetX * targetInverse;
        target.y = targetY * targetInverse;
        target.z = targetZ * targetInverse;
    }
    if (target.x == s_ZeroVector.x && target.y == s_ZeroVector.y && target.z == s_ZeroVector.z) {
        return 0.0f;
    }

    // the rotation from gravity to the target around their cross product
    f32 crossX = gravityY * target.z - gravityZ * target.y;
    f32 crossY = gravityZ * target.x - gravityX * target.z;
    f32 crossZ = gravityX * target.y - gravityY * target.x;
    f32 rotation[3][3];
    if (sqrtf(crossX * crossX + crossY * crossY + crossZ * crossZ) == 0.0f) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                rotation[i][j] = s_IdentityMatrix.m[i][j];
            }
        }
    } else {
        f32 crossInverse = 1.0f / sqrtf(crossX * crossX + crossY * crossY + crossZ * crossZ);
        f32 axis[3] = { crossX * crossInverse, crossY * crossInverse, crossZ * crossInverse };
        f32 gravity[3] = { gravityX, gravityY, gravityZ };
        f32 goal[3] = { target.x, target.y, target.z };
        // the gravity and the target crossed with the axis
        f32 p[3] = { gravityY * axis[2] - gravityZ * axis[1], gravityZ * axis[0] - gravityX * axis[2],
                     gravityX * axis[1] - gravityY * axis[0] };
        f32 q[3] = { target.y * axis[2] - target.z * axis[1], target.z * axis[0] - target.x * axis[2],
                     target.x * axis[1] - target.y * axis[0] };
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                rotation[i][j] = axis[j] * axis[i] + q[j] * p[i] + gravity[i] * goal[j];
            }
        }
    }

    f32 result[3][3];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result[i][j] = direction[i][0] * rotation[0][j] + direction[i][1] * rotation[1][j] + direction[i][2] * rotation[2][j];
        }
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            direction[i][j] = result[i][j];
        }
    }
    NormalizeDirection(direction, NORMALIZE_THRESHOLD);

    f32 differenceX = gravityX - target.x;
    f32 differenceY = gravityY - target.y;
    f32 differenceZ = gravityZ - target.z;
    return sqrtf(differenceX * differenceX + differenceY * differenceY + differenceZ * differenceZ);
}

// 0x00353128 (name is ours)
void nn::hid::CTR::GyroscopeReader::Read(GyroscopeStatus* pBuffer, int* pReadCount, int count)
{
    int readMax = HISTORY_NUM;
    int readCount = 0;
    if (m_IsFirstRead) {
        nn::os::Tick elapsed(nn::svc::GetSystemTick() - s_GyroscopeStartTick);
        s64 milliSeconds = elapsed.ToTimeSpan().GetMilliSeconds();
        int sampleNum = static_cast<int>(milliSeconds / FIRST_READ_INTERVAL_MSEC) + 1;
        if (milliSeconds < 0 || sampleNum <= 0) {
            *pReadCount = 0;
            return;
        }
        if (sampleNum < readMax) {
            readMax = sampleNum;
        }
        m_IsFirstRead = false;
    }
    GyroscopeLowStatus lowBuffer[HISTORY_NUM];
    m_pGyroscope->m_pRing->ReadData(lowBuffer, readMax, &readCount, &m_LastTick, &m_LastIndex);
    if (readCount <= 0) {
        *pReadCount = 0;
        if (count > 0) {
            pBuffer[0] = m_Status;
        }
        return;
    }

    m_ZeroPlayState = 1.0f;
    m_DriftState = 1.0f;
    for (int i = readCount - 1; i >= 0; i--) {
        m_LastSpeed[0] = m_Status.speed[0];
        m_LastSpeed[1] = m_Status.speed[1];
        m_LastSpeed[2] = m_Status.speed[2];
        m_SampleIndex = (m_SampleIndex + 1) & (SAMPLE_NUM - 1);
        const GyroscopeLowStatus& low = lowBuffer[i];
        int stableCount[3];
        CalculateGyroscopeAxisStatus(&m_Status.speed[0], &stableCount[0], &m_ZeroPoint[0], ConvertRawValue(low.x, 0), m_Unit[0], m_Samples[0]);
        CalculateGyroscopeAxisStatus(&m_Status.speed[1], &stableCount[1], &m_ZeroPoint[1], ConvertRawValue(low.y, 1), m_Unit[1], m_Samples[1]);
        CalculateGyroscopeAxisStatus(&m_Status.speed[2], &stableCount[2], &m_ZeroPoint[2], ConvertRawValue(low.z, 2), m_Unit[2], m_Samples[2]);
        if (m_IsAxisRotationEnabled && !IsIdentity(m_AxisRotation)) {
            nn::math::VEC3* pSpeed = reinterpret_cast<nn::math::VEC3*>(m_Status.speed);
            nn::math::ARMv6::VEC3TransformAsm(pSpeed, &m_AxisRotation, pSpeed);
        }
        m_SpeedChange[0] = m_Status.speed[0] - m_LastSpeed[0];
        m_SpeedChange[1] = m_Status.speed[1] - m_LastSpeed[1];
        m_SpeedChange[2] = m_Status.speed[2] - m_LastSpeed[2];

        int minCount = stableCount[2];
        if (stableCount[0] < stableCount[1]) {
            if (minCount > stableCount[0]) {
                minCount = stableCount[0];
            }
        } else if (minCount > stableCount[1]) {
            minCount = stableCount[1];
        }
        f32 state = static_cast<f32>(minCount - 1) / static_cast<f32>(m_DriftSampleNum - 1);
        if (m_DriftState < state) {
            state = m_DriftState;
        }
        m_DriftState = state;

        m_Status.angle[0] += m_AngleMagnification * m_Status.speed[0];
        m_Status.angle[1] += m_AngleMagnification * m_Status.speed[1];
        m_Status.angle[2] += m_AngleMagnification * m_Status.speed[2];
        CalculateDirection();
        if (i < count) {
            pBuffer[i] = m_Status;
        }
    }
    *pReadCount = (readCount > count) ? count : readCount;

    if (m_IsAccRevisionEnabled) {
        AccelerometerStatus accelerometerStatus;
        if (m_pAccelerometerReader->ReadLatest(&accelerometerStatus)) {
            AccelerationFloat acceleration;
            m_pAccelerometerReader->ConvertToAcceleration(&acceleration, 1, &accelerometerStatus);
            nn::math::VEC3 v = { acceleration.x, acceleration.y, acceleration.z };
            m_AccRevisionState = ReviseDirectionWithAcceleration(m_Status.direction, &v);
            if (count > 0) {
                pBuffer[0] = m_Status;
            }
            return;
        }
    }
    m_AccRevisionState = 0.0f;
}

// 0x003537EC | mk7dlp:bytes [tier B]
nn::hid::CTR::GyroscopeReader::~GyroscopeReader()
{
    detail::Ipc::DisableGyroscopeLow();
    s_GyroscopeReaderCount--;
}

// 0x003542BC (name is ours)
void nn::hid::CTR::GyroscopeReader::NormalizeDirection(f32 (*direction)[3], f32 threshold)
{
    f32 sum;
    do {
        NormalizeRow(direction[0]);
        NormalizeRow(direction[1]);
        NormalizeRow(direction[2]);
        // each row moves half way to the cross product of the other two
        f32 cross0[3];
        f32 cross1[3];
        f32 cross2[3];
        Cross(cross0, direction[1], direction[2]);
        Cross(cross1, direction[2], direction[0]);
        Cross(cross2, direction[0], direction[1]);
        f32 length0 = sqrtf(cross0[0] * cross0[0] + cross0[1] * cross0[1] + cross0[2] * cross0[2]);
        f32 length1 = sqrtf(cross1[0] * cross1[0] + cross1[1] * cross1[1] + cross1[2] * cross1[2]);
        f32 length2 = sqrtf(cross2[0] * cross2[0] + cross2[1] * cross2[1] + cross2[2] * cross2[2]);
        f32 inverse = 1.0f / length0;
        direction[0][0] = (direction[0][0] + inverse * cross0[0]) * 0.5f;
        direction[0][1] = (direction[0][1] + inverse * cross0[1]) * 0.5f;
        direction[0][2] = (direction[0][2] + inverse * cross0[2]) * 0.5f;
        inverse = 1.0f / length1;
        direction[1][0] = (direction[1][0] + inverse * cross1[0]) * 0.5f;
        direction[1][1] = (direction[1][1] + inverse * cross1[1]) * 0.5f;
        direction[1][2] = (direction[1][2] + inverse * cross1[2]) * 0.5f;
        sum = length0 + length1 + length2;
        inverse = 1.0f / length2;
        direction[2][0] = (direction[2][0] + inverse * cross2[0]) * 0.5f;
        direction[2][1] = (direction[2][1] + inverse * cross2[1]) * 0.5f;
        direction[2][2] = (direction[2][2] + inverse * cross2[2]) * 0.5f;
    } while (sum < threshold);
}

} // namespace CTR
} // namespace hid
} // namespace nn
