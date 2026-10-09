#include "nn/hid/CTR/hid_AnalogStickClamper.h"
#include <math.h>
#include "nn/hidlow/hidlow_Api.h"

namespace nn {
namespace hid {
namespace CTR {
namespace {
// the limits of the clamping (values from the binary)
const s16 CIRCLE_MIN_LOWEST = 40;
const s16 CROSS_MIN_LOWEST = 36;
const s16 MAX_HIGHEST = 145;
// the dead zone of ClampStickMinimum
const int MINIMUM_RADIUS = 40;
// the change of the range per stick unit and update
const f32 RANGE_STEP = 1.0f / 145;

// the defaults (values from the binary)
const s16 DEFAULT_MIN_CROSS = 36;
const s16 DEFAULT_THRESHOLD = 141;
const f32 DEFAULT_SCALE_MAX = 1.5f;
const f32 DEFAULT_RANGE = 141.0f;
} // namespace

// 0x003538B8 | nintendogs:bytes [tier A]
void nn::hid::CTR::AnalogStickClamper::ClampValueOfClamp()
{
    if (m_Min[CLAMP_MODE_CIRCLE] < CIRCLE_MIN_LOWEST) {
        m_Min[CLAMP_MODE_CIRCLE] = CIRCLE_MIN_LOWEST;
    }
    if (m_Min[CLAMP_MODE_CROSS] < CROSS_MIN_LOWEST) {
        m_Min[CLAMP_MODE_CROSS] = CROSS_MIN_LOWEST;
    }
    if (m_Max[CLAMP_MODE_CIRCLE] > MAX_HIGHEST) {
        m_Max[CLAMP_MODE_CIRCLE] = MAX_HIGHEST;
    }
    if (m_Max[CLAMP_MODE_CROSS] > MAX_HIGHEST) {
        m_Max[CLAMP_MODE_CROSS] = MAX_HIGHEST;
    }
    if (m_Max[CLAMP_MODE_MINIMUM] > MAX_HIGHEST) {
        m_Max[CLAMP_MODE_MINIMUM] = MAX_HIGHEST;
    }
}

// 0x00353904 | nintendogs:bytes [tier B]
void nn::hid::CTR::AnalogStickClamper::NormalizeStickWithScale(f32* pX, f32* pY, s16 x, s16 y)
{
    f32 change = 0.0f;
    f32 range;
    switch (m_Mode) {
    case CLAMP_MODE_CIRCLE:
        range = static_cast<f32>(m_Threshold - m_Min[CLAMP_MODE_CIRCLE]);
        break;
    case CLAMP_MODE_CROSS:
        range = static_cast<f32>(m_Threshold - m_Min[CLAMP_MODE_CROSS]);
        break;
    case CLAMP_MODE_MINIMUM:
        range = static_cast<f32>(m_Threshold - m_Min[CLAMP_MODE_MINIMUM]);
        break;
    }
    f32 length = sqrtf(static_cast<f32>(x * x + y * y));
    f32 normalX;
    f32 normalY;
    if (length == 0.0f) {
        normalX = 0.0f;
        normalY = 0.0f;
    } else {
        normalX = x / length;
        normalY = y / length;
        change = length - m_LastLength;
        if (range > length) {
            if (m_LastLength >= range) {
                m_RangeChange = change;
            } else if (m_RangeChange >= change) {
                m_RangeChange = change;
            }
            normalX = normalX * length / range;
            normalY = normalY * length / range;
        } else if (m_LastLength >= range) {
            if (change > m_RangeChange) {
                m_RangeChange = change;
            }
        } else if (m_LastChange < change) {
            m_RangeChange = change;
        } else {
            m_RangeChange = m_LastChange;
        }
    }

    f32 limit = (m_ScaleMax > 1.0f) ? m_ScaleMax * range : range;
    f32 rangeChange = m_RangeChange;
    if (length >= range) {
        if (rangeChange <= 1.0f) {
            rangeChange = 1.0f;
        }
    } else if (rangeChange > -1.0f) {
        rangeChange = -1.0f;
    }
    m_RangeChange = rangeChange;

    f32 step;
    switch (m_Mode) {
    case CLAMP_MODE_CIRCLE:
        step = static_cast<f32>(m_Max[CLAMP_MODE_CIRCLE] - m_Min[CLAMP_MODE_CIRCLE]) * RANGE_STEP;
        break;
    case CLAMP_MODE_CROSS:
        step = static_cast<f32>(m_Max[CLAMP_MODE_CROSS] - m_Min[CLAMP_MODE_CROSS]) * RANGE_STEP;
        break;
    case CLAMP_MODE_MINIMUM:
        step = static_cast<f32>(m_Max[CLAMP_MODE_MINIMUM] - MINIMUM_RADIUS) * RANGE_STEP;
        break;
    }
    m_Range += m_RangeChange * step;
    if (m_Range < range) {
        m_Range = range;
    } else if (m_Range > limit) {
        m_Range = limit;
    }
    f32 scale = m_Range / limit;
    normalX *= scale;
    normalY *= scale;
    f32 lengthSquare = normalX * normalX + normalY * normalY;
    if (lengthSquare > 1.0f) {
        f32 normalLength = sqrtf(lengthSquare);
        normalX /= normalLength;
        normalY /= normalLength;
    }
    m_LastLength = length;
    m_LastChange = change;
    *pX = normalX;
    *pY = normalY;
}

// 0x00353B88 (name is ours)
void nn::hid::CTR::AnalogStickClamper::SetNormalizeStickScaleSettings(f32 scale, s16 threshold)
{
    if (threshold > MAX_HIGHEST) {
        threshold = MAX_HIGHEST;
    }
    m_ScaleMax = scale;
    m_Threshold = threshold;
}

// 0x00353B9C | nintendogs:bytes [tier A]
void nn::hid::CTR::AnalogStickClamper::ClampCore(s16* pX, s16* pY, int x, int y)
{
    switch (m_Mode) {
    case CLAMP_MODE_CIRCLE:
        nn::hidlow::ClampStickCircle(pX, pY, x, y, m_Min[CLAMP_MODE_CIRCLE], m_Max[CLAMP_MODE_CIRCLE]);
        break;
    case CLAMP_MODE_CROSS:
        nn::hidlow::ClampStickCross(pX, pY, x, y, m_Min[CLAMP_MODE_CROSS], m_Max[CLAMP_MODE_CROSS]);
        break;
    case CLAMP_MODE_MINIMUM:
        nn::hidlow::ClampStickMinimum(pX, pY, x, y, m_Min[CLAMP_MODE_MINIMUM], m_Max[CLAMP_MODE_MINIMUM]);
        break;
    }
}

// 0x00353C30 | nintendogs:bytes [tier A]
nn::hid::CTR::AnalogStickClamper::AnalogStickClamper()
{
    m_Min[CLAMP_MODE_CIRCLE] = CIRCLE_MIN_LOWEST;
    m_Min[CLAMP_MODE_CROSS] = DEFAULT_MIN_CROSS;
    m_Min[CLAMP_MODE_MINIMUM] = CIRCLE_MIN_LOWEST;
    m_Max[CLAMP_MODE_CIRCLE] = MAX_HIGHEST;
    m_Max[CLAMP_MODE_CROSS] = MAX_HIGHEST;
    m_Max[CLAMP_MODE_MINIMUM] = MAX_HIGHEST;
    m_Mode = CLAMP_MODE_CIRCLE;
    m_Threshold = DEFAULT_THRESHOLD;
    m_RangeChange = 0.0f;
    m_LastLength = 0.0f;
    m_LastChange = 0.0f;
    m_ScaleMax = DEFAULT_SCALE_MAX;
    m_Range = DEFAULT_RANGE;
}

} // namespace CTR
} // namespace hid
} // namespace nn
