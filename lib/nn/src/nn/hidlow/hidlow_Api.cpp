#include "nn/hidlow/hidlow_Api.h"
#include <math.h>

namespace nn {
namespace hidlow {
namespace {
// the buttons (3dbrew "HID Shared Memory": bit 2 SELECT, bit 3 START; names are ours)
const bit32 BUTTON_SELECT = 1 << 2;
const bit32 BUTTON_START = 1 << 3;

// ClampStickMinimum: the edge of the dead zone, the size of the square and its largest radius
// squared (values from the binary)
const f32 MINIMUM_EDGE = 36.0f;
const f32 MINIMUM_RADIUS_SQUARE_MAX = 1600.0f;
const int MINIMUM_RADIUS = 40;

// the integer square root (Newton's method)
inline int SqrtInt(int value)
{
    if (value <= 0) {
        return 0;
    }
    int estimate = 1;
    int rest = value;
    while (rest > estimate) {
        estimate <<= 1;
        rest >>= 1;
    }
    int root;
    do {
        root = estimate;
        estimate = (value / root + root) >> 1;
    } while (estimate < root);
    return root;
}

// the value beyond the dead zone of one axis
inline int ClampAxis(int value, int min)
{
    if (value < 0) {
        if (-value > min) {
            return value + min;
        }
        return 0;
    }
    if (min < value) {
        return value - min;
    }
    return 0;
}

// START when both or only one of START and SELECT is in the state (the other not held)
inline bit32 GatherState(bit32 state, bit32 hold)
{
    if ((BUTTON_SELECT | BUTTON_START) & ~state) {
        if (((state & BUTTON_SELECT) && !(hold & BUTTON_START)) ||
            ((state & BUTTON_START) && !(hold & BUTTON_SELECT))) {
            return state | BUTTON_START;
        }
        return state & ~BUTTON_START;
    }
    return state | BUTTON_START;
}
} // namespace

// 0x00483F8C | nintendogs:bytes [tier A]
s16 ClampStickCross(s16* pX, s16* pY, int x, int y, int min, int max)
{
    int dx = ClampAxis(x, min);
    int dy = ClampAxis(y, min);
    int range = max - min;
    int lengthSquare = dx * dx + dy * dy;
    if (range * range <= lengthSquare) {
        int length = SqrtInt(lengthSquare << 14);
        int scale = range << 7;
        dx = static_cast<s16>((dx * scale) / length);
        dy = static_cast<s16>((dy * scale) / length);
    }
    *pX = dx;
    *pY = dy;
    return static_cast<s16>(range);
}

// 0x00484090 | nintendogs:bytes [tier A]
s16 ClampStickCircle(s16* pX, s16* pY, int x, int y, int min, int max)
{
    int lengthSquare = x * x + y * y;
    if (lengthSquare <= min * min) {
        *pY = 0;
        *pX = 0;
        return 0;
    }
    int length = SqrtInt(lengthSquare << 14);
    int range;
    int scale;
    if (max * max <= lengthSquare) {
        range = max - min;
        scale = range << 7;
    } else {
        scale = length - (min << 7);
        range = (length >> 7) - min;
    }
    *pX = (x * scale) / length;
    *pY = (y * scale) / length;
    return static_cast<s16>(range);
}

// 0x00484174 | nintendogs:bytes [tier A]
void ClampStickMinimum(s16* pX, s16* pY, int x, int y, int min, int max)
{
    (void)min;
    f32 lengthSquare = static_cast<f32>(x * x + y * y);
    if (lengthSquare != 0.0f) {
        // the point where the direction leaves the square of the dead zone
        f32 edgeX;
        f32 edgeY;
        if (y * y < x * x) {
            if (x < 0) {
                edgeY = (-MINIMUM_EDGE * y) / x;
                edgeX = -MINIMUM_EDGE;
            } else {
                edgeX = MINIMUM_EDGE;
                edgeY = (MINIMUM_EDGE * y) / x;
            }
        } else {
            if (y < 0) {
                edgeY = -MINIMUM_EDGE;
                edgeX = (-MINIMUM_EDGE * x) / y;
            } else {
                edgeY = MINIMUM_EDGE;
                edgeX = (MINIMUM_EDGE * x) / y;
            }
        }
        f32 edgeSquare = edgeX * edgeX + edgeY * edgeY;
        if (edgeSquare >= MINIMUM_RADIUS_SQUARE_MAX) {
            edgeSquare = MINIMUM_RADIUS_SQUARE_MAX;
        }
        if (lengthSquare >= edgeSquare) {
            f32 edge = sqrtf(edgeSquare);
            f32 span = static_cast<f32>(max) - edge;
            f32 length = sqrtf(lengthSquare);
            f32 over = length - edge;
            f32 scale = static_cast<f32>(max - MINIMUM_RADIUS);
            if (lengthSquare >= static_cast<f32>(max * max)) {
                *pX = static_cast<s32>(x * scale / length);
                *pY = static_cast<s32>(y * scale / length);
            } else {
                *pX = static_cast<s32>(x * scale * over / (span * length));
                *pY = static_cast<s32>(y * scale * over / (span * length));
            }
            return;
        }
    }
    *pX = 0;
    *pY = 0;
}

// 0x00484340 | nintendogs:callgraph [tier A]
void GatherStartAndSelect(nn::hid::CTR::PadStatus* pStatus)
{
    GatherStartAndSelect(pStatus->hold, pStatus->trigger, pStatus->release);
}

// 0x0048434C | nintendogs:bytes [tier A]
void GatherStartAndSelect(bit32& hold, bit32& trigger, bit32& release)
{
    trigger = GatherState(trigger, hold);
    release = GatherState(release, hold);
    if (hold & BUTTON_SELECT) {
        hold = (hold & ~BUTTON_SELECT) | BUTTON_START;
    }
    trigger &= ~BUTTON_SELECT;
    release &= ~BUTTON_SELECT;
}

} // namespace hidlow
} // namespace nn
