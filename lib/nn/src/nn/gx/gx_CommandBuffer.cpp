#include "nn/gx/gx_Api.h"

// The register writes of the GL functions into the command buffer (__cb_*). A command is a first
// value and a header (register | mask << 16 | extra values << 20 | consecutive << 31 after 3dbrew),
// then the extra values, padded to 8 bytes; at most 128 values per header. If the commands do not
// fit, the buffer is marked full (the current pointer becomes its end).

using namespace nn::gx::detail;

namespace {
// all bytes of a register are written
const u32 MASK_ALL = 0xF0000;
// the values go to consecutive registers
const u32 CONSECUTIVE = 0x80000000;
// the most values of one header
const u32 MAX_VALUES = 128;

// the buffer is large enough for count values (with the headers and the padding)
inline bool HasRoomFor(u32* pCurrent, u32 count)
{
    if (s_pCommandBufferEnd <= pCurrent + ((count & ~1) + (count >> 7) * 2) + 2) {
        s_pCommandBufferCurrent = s_pCommandBufferEnd;
        return false;
    }
    return true;
}
} // namespace

extern "C" {
// count registers from reg on get the values
// 0x0077A780 | nintendogs:bytes [tier B]
void __cb_writeRegs(u32 reg, u32 count, const u32* pValues)
{
    u32* pCurrent = s_pCommandBufferCurrent;
    if (!HasRoomFor(pCurrent, count)) {
        return;
    }
    for (u32 i = 0; i < count; i += MAX_VALUES) {
        u32 n = count - i;
        if (n > MAX_VALUES) {
            n = MAX_VALUES;
        }
        const u32* p = &pValues[i];
        *pCurrent++ = *p++;
        *pCurrent++ = ((n - 1) << 20) | (reg + i) | CONSECUTIVE | MASK_ALL;
        for (u32 j = 0; j < n >> 1; j++) {
            *pCurrent++ = *p++;
            *pCurrent++ = *p++;
        }
    }
    s_pCommandBufferCurrent = pCurrent;
}

// count registers from reg get the value
// 0x0013A01C | nintendogs:bytes [tier B]
void __cb_fillRegs(u32 reg, u32 count, u32 value)
{
    u32* pCurrent = s_pCommandBufferCurrent;
    if (!HasRoomFor(pCurrent, count)) {
        return;
    }
    for (u32 i = 0; i < count; i += MAX_VALUES) {
        u32 n = count - i;
        if (n > MAX_VALUES) {
            n = MAX_VALUES;
        }
        *pCurrent++ = value;
        *pCurrent++ = ((n - 1) << 20) | (reg + i) | CONSECUTIVE | MASK_ALL;
        for (u32 j = 0; j < n >> 1; j++) {
            *pCurrent++ = value;
            *pCurrent++ = value;
        }
    }
    s_pCommandBufferCurrent = pCurrent;
}

// the register gets count values one after the other
// 0x0013A0E4 | nintendogs:bytes [tier B]
void __cb_multiWriteReg(u32 reg, u32 count, const u32* pValues)
{
    u32* pCurrent = s_pCommandBufferCurrent;
    if (!HasRoomFor(pCurrent, count)) {
        return;
    }
    for (u32 i = 0; i < count; i += MAX_VALUES) {
        const u32* p = &pValues[i];
        u32 n = count - i;
        if (n > MAX_VALUES) {
            n = MAX_VALUES;
        }
        *pCurrent++ = *p++;
        *pCurrent++ = ((n - 1) << 20) | reg | MASK_ALL;
        for (u32 j = 0; j < (n & ~1); j++) {
            *pCurrent++ = *p++;
        }
    }
    s_pCommandBufferCurrent = pCurrent;
}

// a write of count values without a mask (the GPU ignores it)
// 0x0077A720 | nintendogs:bytes [tier B]
void __cb_addDummyWrite(u32 reg, u32 count)
{
    u32* pCurrent = s_pCommandBufferCurrent;
    if (pCurrent + count + 2 >= s_pCommandBufferEnd) {
        s_pCommandBufferCurrent = s_pCommandBufferEnd;
        return;
    }
    if (count == 0) {
        return;
    }
    count--;
    pCurrent[0] = 0;
    pCurrent[1] = reg | (count << 20);
    s_pCommandBufferCurrent = pCurrent + 2;
    if ((count & 1) != 0) {
        count++;
    }
    s_pCommandBufferCurrent += count;
}
} // extern "C"
