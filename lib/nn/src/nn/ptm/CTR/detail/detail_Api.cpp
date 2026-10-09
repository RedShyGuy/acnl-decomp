#include "nn/ptm/CTR/detail/detail_Api.h"
#include "nn/fnd/fnd_DateTime.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace ptm {
namespace CTR {
namespace detail {

namespace {
// the clock of the shared page (3dbrew "Configuration Memory": DATETIME_0 / DATETIME_1, 0x20 bytes
// each; the field names are ours)
struct SharedDateTime
{
    s64 dateTime;           // 0x0 milliseconds at updateTick
    s64 updateTick;         // 0x8 system tick of the last update
    s32 tickFrequency;      // 0x10 ticks per second (PTM sets 0xFFB0FF0)
    u32 unknown14;          // 0x14
    s64 correction;         // 0x18 milliseconds added in full at the update, fading out over an hour
};
ASSERT_SIZE(SharedDateTime, 0x20);

// bit 0 selects the valid entry; the value changes with every update
u32* const DATETIME_SELECTOR = reinterpret_cast<u32*>(0x1FF81000);
const SharedDateTime* const DATETIMES = reinterpret_cast<const SharedDateTime*>(0x1FF81020);

const s64 MILLISECONDS_PER_HOUR = 60 * 60 * 1000;

// the range GetSwcMilliSeconds keeps the time in (names are ours)
const s32 MIN_YEAR = 2000;
const s32 MAX_YEAR = 2100;

inline void DataMemoryBarrier()
{
    __asm__ volatile("mcr p15, 0, %0, c7, c10, 5" : : "r"(0) : "memory");
}

// the bits 32..95 of the 128 bit product a * b (inline)
inline s64 MultiplyShift32(s64 a, s64 b)
{
    s64 aHigh = a >> 32;
    s64 aLow = static_cast<u32>(a);
    s64 bHigh = b >> 32;
    s64 bLow = static_cast<u32>(b);
    return aHigh * bLow + bHigh * aLow +
           ((aHigh * bHigh) << 32) + static_cast<s64>(static_cast<u64>(aLow * bLow) >> 32);
}
} // namespace

// 0x0012A4AC | fefates:bytes [tier B]
s64 GetSwcMilliSeconds()
{
    const s64 minimum =
        (nn::fnd::DateTime(MIN_YEAR, 1, 1, 0, 0, 0, 0) - nn::fnd::DateTime::EPOCH).GetMilliSeconds();
    const s64 range = (nn::fnd::DateTime(MAX_YEAR, 1, 1, 0, 0, 0, 0) - nn::fnd::DateTime(MIN_YEAR, 1, 1, 0, 0, 0, 0))
                          .GetMilliSeconds();

    // a consistent copy of the entry the selector points to
    s64 tick;
    u32 selector;
    s64 dateTime;
    s64 updateTick;
    s32 tickFrequency;
    s64 correction;
    do {
        tick = nn::svc::GetSystemTick();
        selector = *DATETIME_SELECTOR;
        const SharedDateTime& entry = DATETIMES[selector & 1];
        dateTime = entry.dateTime;
        updateTick = entry.updateTick;
        tickFrequency = entry.tickFrequency;
        correction = entry.correction;
        DataMemoryBarrier();
    } while (selector != *DATETIME_SELECTOR);

    // milliseconds per tick as a 32.32 fixed point number
    const s64 millisecondsPerTick = (static_cast<s64>(1000) << 32) / tickFrequency;
    s64 now = dateTime + MultiplyShift32(tick - updateTick, millisecondsPerTick);

    // the part of the correction that is left (all of it right after the update, none after an hour)
    const s64 left = MILLISECONDS_PER_HOUR - (now - dateTime);
    const s64 factor = ((left > 0 ? left : 0) << 32) / MILLISECONDS_PER_HOUR;
    now += MultiplyShift32(correction, factor);

    return minimum + (now - minimum) % range;
}

} // namespace detail
} // namespace CTR
} // namespace ptm
} // namespace nn
