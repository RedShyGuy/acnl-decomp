#pragma once

// Types of nn::camera. The type names are from the binary; the values, the members and the type
// names marked so after 3dbrew "Camera Services".

#include "decomp.h"

namespace nn {
namespace camera {
namespace CTR {
enum Port : u8
{
    PORT_NONE = 0,
    PORT_CAM1 = 1 << 0,
    PORT_CAM2 = 1 << 1,
    PORT_BOTH = PORT_CAM1 | PORT_CAM2,
};

enum CameraSelect : u8
{
    SELECT_NONE = 0,
    SELECT_OUT1 = 1 << 0,
    SELECT_IN1 = 1 << 1,
    SELECT_OUT2 = 1 << 2,
    SELECT_IN1_OUT1 = SELECT_OUT1 | SELECT_IN1,
    SELECT_OUT1_OUT2 = SELECT_OUT1 | SELECT_OUT2,
    SELECT_IN1_OUT2 = SELECT_IN1 | SELECT_OUT2,
    SELECT_ALL = SELECT_OUT1 | SELECT_IN1 | SELECT_OUT2,
};

enum Context : u8
{
    CONTEXT_NONE = 0,
    CONTEXT_A = 1 << 0,
    CONTEXT_B = 1 << 1,
    CONTEXT_BOTH = CONTEXT_A | CONTEXT_B,
};

enum Flip : u8
{
    FLIP_NONE = 0,
    FLIP_HORIZONTAL = 1,
    FLIP_VERTICAL = 2,
    FLIP_REVERSE = 3,
};

// (type name after 3dbrew)
enum Size : u8
{
    SIZE_VGA = 0,
    SIZE_QVGA = 1,
    SIZE_QQVGA = 2,
    SIZE_CIF = 3,
    SIZE_QCIF = 4,
    SIZE_DS_LCD = 5,
    SIZE_DS_LCDx4 = 6,
    SIZE_CTR_TOP_LCD = 7,
    SIZE_CTR_BOTTOM_LCD = SIZE_QVGA,
};

enum FrameRate : u8
{
    FRAME_RATE_15 = 0,
    FRAME_RATE_15_TO_5 = 1,
    FRAME_RATE_15_TO_2 = 2,
    FRAME_RATE_10 = 3,
    FRAME_RATE_8_5 = 4,
    FRAME_RATE_5 = 5,
    FRAME_RATE_20 = 6,
    FRAME_RATE_20_TO_5 = 7,
    FRAME_RATE_30 = 8,
    FRAME_RATE_30_TO_5 = 9,
    FRAME_RATE_15_TO_10 = 10,
    FRAME_RATE_20_TO_10 = 11,
    FRAME_RATE_30_TO_10 = 12,
};

enum PhotoMode : u8
{
    PHOTO_MODE_NORMAL = 0,
    PHOTO_MODE_PORTRAIT = 1,
    PHOTO_MODE_LANDSCAPE = 2,
    PHOTO_MODE_NIGHTVIEW = 3,
    PHOTO_MODE_LETTER = 4,
};

enum ShutterSoundType : u8
{
    SHUTTER_SOUND_TYPE_NORMAL = 0,
    SHUTTER_SOUND_TYPE_MOVIE = 1,
    SHUTTER_SOUND_TYPE_MOVIE_END = 2,
};

struct StereoCameraCalibrationData
{
    bool isValidRotationXY;  // 0x00
    bool reservedFlag[3];    // 0x01
    f32 scale;               // 0x04
    f32 rotationZ;           // 0x08
    f32 translationX;        // 0x0C
    f32 translationY;        // 0x10
    f32 rotationX;           // 0x14
    f32 rotationY;           // 0x18
    f32 angleOfViewRight;    // 0x1C
    f32 angleOfViewLeft;     // 0x20
    f32 distanceToChart;     // 0x24
    f32 distanceCameras;     // 0x28
    s16 imageWidth;          // 0x2C
    s16 imageHeight;         // 0x2E
    u8 reserved[16];         // 0x30
};
ASSERT_SIZE(StereoCameraCalibrationData, 0x40);
} // namespace CTR
} // namespace camera
} // namespace nn
