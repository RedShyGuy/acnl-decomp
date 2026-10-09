#pragma once

#include "decomp.h"
#include "nn/hid/CTR/hid_AccelerometerReader.h"
#include "nn/hid/CTR/hid_Types.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Vector3.h"

namespace nn {
namespace hid {
namespace CTR {
class Gyroscope;

// Reads the gyroscope: converts the raw values to angular speeds (with a zero point that
// follows the drift and a dead zone), integrates the angles and the orientation and corrects the
// orientation with the accelerometer. The constructor is not in this program (the game never
// makes one); the member names and the names marked "(name is ours)" are ours.
class GyroscopeReader
{
public:
    static const int HISTORY_NUM = 32;
    static const int SAMPLE_NUM = 256;

    ~GyroscopeReader(); // 0x003537EC | mk7dlp:bytes [tier B]
    // the newest state; false before the first one
    bool ReadLatest(GyroscopeStatus* pStatus); // 0x00352808 | nintendogs:bytes-fuzzy [tier B]
    // reads the new raw values and computes a state for each (newest first)
    void Read(GyroscopeStatus* pBuffer, int* pReadCount, int count); // 0x00353128 (name is ours)
    // turns the orientation by the angular speeds
    void CalculateDirection(); // 0x00352934 | nintendogs:bytes [tier B]
    // the speed of one axis from its raw value and the recent samples
    void CalculateGyroscopeAxisStatus(f32* pSpeed, int* pStableCount, f32* pZeroPoint, int value, f32 unit, int* pSamples); // 0x00352B00 | nintendogs:bytes [tier B]
    // turns the orientation towards gravity; returns how far it was off
    f32 ReviseDirectionWithAcceleration(f32 (*direction)[3], const nn::math::VEC3* pAcceleration); // 0x00352CBC (name is ours)
    // makes the rows of the orientation orthonormal again
    static void NormalizeDirection(f32 (*direction)[3], f32 threshold); // 0x003542BC (name is ours)

private:
    // the speed of a raw value (inline, name is ours)
    int ConvertRawValue(s16 raw, int axis) const;
    // the turn of one axis (inline, name is ours)
    f32 CalculateRotation(f32 speed) const;

    int m_HistoryCount;                              // 0x0000
    GyroscopeStatus m_History[HISTORY_NUM];          // 0x0004, newest first (ReadLatest)
    bool m_IsFirstRead;                              // 0x0784
    AccelerometerReader m_AccelerometerReader;       // 0x0788
    GyroscopeStatus m_Status;                        // 0x07E0
    AccelerometerReader* m_pAccelerometerReader;     // 0x081C
    f32 m_LastSpeed[3];                              // 0x0820
    f32 m_SpeedChange[3];                            // 0x082C
    f32 m_Unit[3];                                   // 0x0838, per axis
    f32 m_DirectionMagnification;                    // 0x0844
    f32 m_AngleMagnification;                        // 0x0848
    f32 m_DirectionCorrection;                       // 0x084C
    f32 m_DirectionScale;                            // 0x0850
    bool m_IsZeroPlayEnabled;                        // 0x0854
    bool m_IsZeroDriftEnabled;                       // 0x0855
    bool m_IsAccRevisionEnabled;                     // 0x0856
    bool m_IsAxisRotationEnabled;                    // 0x0857
    f32 m_ZeroPlay;                                  // 0x0858
    f32 m_DriftRange;                                // 0x085C
    int m_DriftSampleNum;                            // 0x0860
    f32 m_DriftRevisePower;                          // 0x0864
    f32 m_AccRevisePower;                            // 0x0868
    f32 m_AccReviseRange;                            // 0x086C
    f32 m_ZeroPlayState;                             // 0x0870
    f32 m_DriftState;                                // 0x0874
    f32 m_AccRevisionState;                          // 0x0878
    f32 m_RawZero[3];                                // 0x087C
    f64 m_RawScale[3];                               // 0x0888
    f32 m_ZeroPoint[3];                              // 0x08A0
    int m_SampleIndex;                               // 0x08AC
    int m_Samples[3][SAMPLE_NUM];                    // 0x08B0
    f32 m_Sensitivity[3];                            // 0x14B0
    u32 m_Reserved14BC;                              // 0x14BC
    Gyroscope* m_pGyroscope;                         // 0x14C0
    int m_LastIndex;                                 // 0x14C4
    s64 m_LastTick;                                  // 0x14C8
    nn::math::MTX34 m_AxisRotation;                  // 0x14D0

    static void CheckLayout()
    {
        ASSERT_OFFSET(GyroscopeReader, m_AccelerometerReader, 0x788);
        ASSERT_OFFSET(GyroscopeReader, m_Status, 0x7E0);
        ASSERT_OFFSET(GyroscopeReader, m_RawScale, 0x888);
        ASSERT_OFFSET(GyroscopeReader, m_Samples, 0x8B0);
        ASSERT_OFFSET(GyroscopeReader, m_pGyroscope, 0x14C0);
        ASSERT_OFFSET(GyroscopeReader, m_AxisRotation, 0x14D0);
        ASSERT_SIZE(GyroscopeReader, 0x1500);
    }
};
} // namespace CTR
} // namespace hid
} // namespace nn
