#pragma once

#include "decomp.h"
#include "nn/hid/CTR/hid_Types.h"
#include "nn/math/math_MTX34.h"

namespace nn {
namespace hid {
namespace CTR {
class Accelerometer;

// Reads the accelerometer: smooths the raw values, subtracts an offset and turns the axes
// (member names are ours).
class AccelerometerReader
{
public:
    AccelerometerReader(Accelerometer& accelerometer); // 0x00354220 | nintendogs:bytes-fuzzy [tier B]
    ~AccelerometerReader(); // 0x003542A8 | tier C
    // the newest state; false without one
    bool ReadLatest(AccelerometerStatus* pStatus); // 0x00353C94 | nintendogs:bytes [tier B]
    // the raw values in G (1/512 G per unit)
    void ConvertToAcceleration(AccelerationFloat* pAcceleration, int count, const AccelerometerStatus* pStatus); // 0x00353D58 | nintendogs:bytes [tier B]
    void Read(AccelerometerStatus* pBuffer, int* pReadCount, int count); // 0x00353E7C | nintendogs:bytes [tier B]
    // the offset and the axis rotation
    void Transform(AccelerometerStatus* pStatus); // 0x003540A4 | nintendogs:bytes [tier B]
    DECOMP_NOINLINE void ResetAxisRotation(); // 0x00353DD4 (name is ours)

private:
    // smooths the three axes with the last values (inline, name is ours)
    void Smooth(AccelerometerStatus* pStatus);

    Accelerometer* m_pAccelerometer;   // 0x00
    s16 m_Play;                        // 0x04
    s16 m_Sensitivity;                 // 0x06
    s16 m_Last[3];                     // 0x08
    s16 m_Offset[3];                   // 0x0E
    nn::math::MTX34 m_AxisRotation;    // 0x18
    bool m_IsOffsetEnabled;            // 0x48
    bool m_IsAxisRotationEnabled;      // 0x49
    s32 m_LastIndex;                   // 0x4C
    s64 m_LastTick;                    // 0x50
};
ASSERT_SIZE(AccelerometerReader, 0x58);
} // namespace CTR
} // namespace hid
} // namespace nn
