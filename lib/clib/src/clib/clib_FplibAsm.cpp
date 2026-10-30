// The software floating point helpers of the ARM C library (fplib): hand-written assembly in the original, so the
// bodies are copied from ACNL USA 1.5 (orig/0004000000086300/code.elf) as naked functions (see clib_MemoryAsm.cpp).
// Floats and doubles are passed in integer registers. The special cases branch to the shared NaN/infinity
// handlers; __fpl_fnaninf reads a data word behind its call and returns behind it.

#include "types.h"

// (the result for a NaN argument of a double function: the default NaN 0x7FF80000_00000000 or the
// quiet argument, by the flags in ip)
// 0x008394C0 | mk7dlp:callgraph [tier C]
extern "C" __attribute__((naked, pcs("aapcs"))) void __fpl_dcheck_NaN1()
{
    asm volatile(
        ".syntax unified\n"
        "ldr r1, [pc, #4]\n"
        "mov r0, #0\n"
        "b __fpl_return_NaN\n"
        ".word 0x7ff80000\n");
}

// (float to unsigned int, rounding towards zero; the argument in r0)
// 0x008394D0 (name is ours)
extern "C" __attribute__((naked, pcs("aapcs"))) u32 _ffixu(f32 value)
{
    asm volatile(
        ".syntax unified\n"
        "vmov s0, r0\n"
        "vcvt.u32.f32 s2, s0\n"
        "vmov r0, s2\n"
        "bx lr\n");
}

// (unsigned long long to float: the code of _ll_sto_f without the sign; the result in r0)
// 0x008394E0 (name is ours)
extern "C" __attribute__((naked, pcs("aapcs"))) f32 _ll_uto_f(u64 value)
{
    asm volatile(
        ".syntax unified\n"
        "mov r2, #1065353216\n"
        "b .L8394fc\n");
}

// (long long to float; the result in r0)
// 0x008394E8 | nintendogs:bytes [tier A]
extern "C" __attribute__((naked, pcs("aapcs"))) f32 _ll_sto_f(s64 value)
{
    asm volatile(
        ".syntax unified\n"
        "ands r2, r1, #-2147483648\n"
        "bpl .L8394f8\n"
        "rsbs r0, r0, #0\n"
        "rsc r1, r1, #0\n"
        ".L8394f8:\n"
        "orr r2, r2, #1065353216\n"
        ".L8394fc:\n"
        "movs r3, r1\n"
        "add r2, r2, #251658240\n"
        "movne r3, #32\n"
        "movseq r1, r0\n"
        "bxeq lr\n"
        "clz ip, r1\n"
        "sub r3, r3, ip\n"
        "lsl r1, r1, ip\n"
        "orr r1, r1, r0, lsr r3\n"
        "add r2, r2, r3, lsl #23\n"
        "rsb r3, r3, #32\n"
        "lsls ip, r0, r3\n"
        "orrs ip, ip, r1, lsl #25\n"
        "adc r0, r2, r1, lsr #8\n"
        "movsne ip, #67108864\n"
        "biccs r0, r0, #1\n"
        "bx lr\n");
}

// (NaN and infinite arguments of a float function: the word behind the call holds three bits per
// case (NaN, +inf, -inf ...) that select a result or one of the returns behind that word)
// 0x00839540 | nintendogs:bytes [tier A]
extern "C" __attribute__((naked, pcs("aapcs"))) void __fpl_fnaninf()
{
    asm volatile(
        ".syntax unified\n"
        "ldr r4, [lr], #4\n"
        "tst r4, r4\n"
        "bmi .L839558\n"
        "lsl r2, r1, #1\n"
        "cmp r2, #-16777216\n"
        "bhi .L839580\n"
        ".L839558:\n"
        "lsl r3, r0, #1\n"
        "cmp r3, #-16777216\n"
        "bhi .L839580\n"
        "bne .L839588\n"
        "lsr ip, r0, #31\n"
        "cmp r2, #-16777216\n"
        "add ip, ip, ip, lsl #1\n"
        "add ip, ip, #2\n"
        "adceq ip, ip, r1, lsr #31\n"
        "b .L83958c\n"
        ".L839580:\n"
        "mov ip, #8\n"
        "b .L83958c\n"
        ".L839588:\n"
        "lsr ip, r1, #31\n"
        ".L83958c:\n"
        "add r3, ip, ip, lsl #1\n"
        "lsr r4, r4, r3\n"
        "and r4, r4, #7\n"
        "subs ip, r4, #4\n"
        "bcs .L8395ac\n"
        "add ip, lr, r4, lsl #2\n"
        "pop {r4, lr}\n"
        "bx ip\n"
        ".L8395ac:\n"
        "add pc, pc, ip, lsl #2\n"
        "nop\n"
        "b .L8395cc\n"
        "b .L8395d0\n"
        "b .L8395c0\n"
        ".L8395c0:\n"
        "mov r0, #-2147483648\n"
        "sub r0, r0, #4194304\n"
        "pop {r4, pc}\n"
        ".L8395cc:\n"
        "mov r0, r1\n"
        ".L8395d0:\n"
        "lsls r2, r0, #1\n"
        "rsbsne r2, r2, #16777216\n"
        "movhi r0, #0\n"
        "pop {r4, pc}\n");
}

// (the IEEE remainder of float arguments in r0/r1)
// 0x008395E0 | nintendogs:bytes [tier A]
extern "C" __attribute__((naked, pcs("aapcs"))) f32 _frem(f32 x, f32 y)
{
    asm volatile(
        ".syntax unified\n"
        "mov ip, #-16777216\n"
        "bics r3, ip, r1, lsl #1\n"
        "bicsne r2, ip, r0, lsl #1\n"
        "beq .L8396dc\n"
        "mov ip, #16711680\n"
        "tst r0, #-2147483648\n"
        "and r3, ip, r1, lsr #7\n"
        "orrne r3, r3, #5\n"
        "ands r2, ip, r0, lsr #7\n"
        "tstne r3, ip\n"
        "beq .L8396e8\n"
        "mov ip, #-2147483648\n"
        "orr r0, ip, r0, lsl #8\n"
        "orr r1, ip, r1, lsl #8\n"
        "sub r3, r3, #65536\n"
        ".L83961c:\n"
        "cmp r2, r3\n"
        "ble .L83964c\n"
        "cmp r0, #-2147483648\n"
        "bic r3, r3, #2\n"
        "bcc .L839640\n"
        "subs r0, r0, r1\n"
        "orr r3, r3, #2\n"
        "rsblt r0, r0, #0\n"
        "eorlt r3, r3, #1\n"
        ".L839640:\n"
        "lsl r0, r0, #1\n"
        "sub r2, r2, #65536\n"
        "b .L83961c\n"
        ".L83964c:\n"
        "asr ip, r2, #16\n"
        "cmp ip, r3, asr #16\n"
        "blt .L839670\n"
        "cmp r0, r1\n"
        "bcc .L839670\n"
        "beq .L8396ac\n"
        ".L839664:\n"
        "sub r0, r0, r1, lsl #1\n"
        "rsb r0, r0, #0\n"
        "eor r3, r3, #1\n"
        ".L839670:\n"
        "lsls r0, r0, #1\n"
        "bcs .L8396c4\n"
        "bmi .L8396b8\n"
        "beq .L8396a0\n"
        "clz ip, r0\n"
        "add ip, ip, #1\n"
        "lsl r0, r0, ip\n"
        "sub r2, r2, ip, lsl #16\n"
        ".L839690:\n"
        "rsbs ip, r2, #0\n"
        "blt .L8396cc\n"
        "mov r0, #0\n"
        "bx lr\n"
        ".L8396a0:\n"
        "lsl r0, r3, #29\n"
        "and r0, r0, #-2147483648\n"
        "bx lr\n"
        ".L8396ac:\n"
        "tst r3, #2\n"
        "beq .L839670\n"
        "b .L839664\n"
        ".L8396b8:\n"
        "subs r2, r2, #65536\n"
        "lsl r0, r0, #1\n"
        "ble .L839690\n"
        ".L8396c4:\n"
        "cmp r2, #0\n"
        "ble .L839690\n"
        ".L8396cc:\n"
        "lsl r2, r2, #7\n"
        "orr r0, r2, r0, lsr #9\n"
        "orr r0, r0, r3, lsl #31\n"
        "bx lr\n"
        ".L8396dc:\n"
        "push {r4, lr}\n"
        "bl __fpl_fnaninf\n"
        ".word 0x3effffed\n"
        ".L8396e8:\n"
        "tst ip, r1, lsr #7\n"
        "beq .L8396fc\n"
        "bics ip, r0, #-2147483648\n"
        "movne r0, #0\n"
        "bx lr\n"
        ".L8396fc:\n"
        "ldr r0, [pc]\n"
        "bx lr\n"
        ".word 0x7fc00000\n");
}

// (float to unsigned long long; the argument in r0)
// 0x00839708 | nintendogs:bytes [tier A]
extern "C" __attribute__((naked, pcs("aapcs"))) u64 _ll_ufrom_f(f32 value)
{
    asm volatile(
        ".syntax unified\n"
        "asrs r2, r0, #23\n"
        "lsl r3, r0, #8\n"
        "orrne r3, r3, #-2147483648\n"
        "bmi .L839738\n"
        "rsbs r2, r2, #190\n"
        "bcc .L839750\n"
        "lsr r1, r3, r2\n"
        "subs ip, r2, #32\n"
        "lsrcs r0, r3, ip\n"
        "rsbcc ip, r2, #32\n"
        "lslcc r0, r3, ip\n"
        "bx lr\n"
        ".L839738:\n"
        "lsl ip, r0, #1\n"
        "cmp ip, #2130706432\n"
        "bcs .L839750\n"
        "mov r0, #0\n"
        "mov r1, #0\n"
        "bx lr\n"
        ".L839750:\n"
        "push {r4, lr}\n"
        "bl __fpl_fnaninf\n"
        ".word 0x806db6db\n"
        "mov r1, #0\n"
        "mov r0, #0\n"
        "bx lr\n"
        "mvn r0, r0, asr #31\n"
        "mov r1, r0\n"
        "bx lr\n");
}

// 0x0083977C | mk7dlp:bytes [tier B]
extern "C" __attribute__((naked, pcs("aapcs"))) void __fpl_return_NaN()
{
    asm volatile(
        ".syntax unified\n"
        "and r3, ip, #15\n"
        "cmp r3, #9\n"
        "moveq r0, #8\n"
        "beq __fpl_cmpreturn\n"
        "cmp r3, #10\n"
        "beq .L8397b0\n"
        "cmp r3, #8\n"
        "bxne lr\n"
        "tsteq ip, #64\n"
        "beq .L8397bc\n"
        "mov r0, #0\n"
        "mov r1, #0\n"
        "bx lr\n"
        ".L8397b0:\n"
        "tst ip, #64\n"
        "movne r0, #-2147483648\n"
        "bx lr\n"
        ".L8397bc:\n"
        "tst ip, #16\n"
        "beq .L8397d8\n"
        "bic r3, r0, #-268435456\n"
        "and r2, r0, #-16777216\n"
        "orr r1, r2, r3, lsr #3\n"
        "lsl r0, r3, #29\n"
        "bx lr\n"
        ".L8397d8:\n"
        "and r0, r0, #-536870912\n"
        "bic r2, r1, #-16777216\n"
        "orr r2, r2, r0\n"
        "and r0, r1, #-16777216\n"
        "orr r0, r0, r2, ror #29\n"
        "bx lr\n");
}

// (value * 2^exponent; the double in r0/r1)
// 0x008397F0 | mk7dlp:bytes [tier B]
extern "C" __attribute__((naked, pcs("aapcs"))) f64 __ARM_scalbn(f64 value, int exponent)
{
    asm volatile(
        ".syntax unified\n"
        "mov ip, #255\n"
        "orr ip, ip, #1792\n"
        "ands r3, ip, r1, lsr #20\n"
        "eorsne ip, ip, r3\n"
        "cmpne ip, r2\n"
        "cmngt r2, r3\n"
        "addgt r1, r1, r2, lsl #20\n"
        "bxgt lr\n"
        "cmp r3, #0\n"
        "beq .L83983c\n"
        "cmp ip, #0\n"
        "beq .L83984c\n"
        "mov r0, #0\n"
        "cmp r2, #0\n"
        "and r1, r1, #-2147483648\n"
        "orrge r1, r1, #1879048192\n"
        "orrge r1, r1, #267386880\n"
        "movlt r1, #0\n"
        "bx lr\n"
        ".L83983c:\n"
        "orrs r3, r0, r1, lsl #1\n"
        "mov r0, #0\n"
        "movne r1, #0\n"
        "bx lr\n"
        ".L83984c:\n"
        "orrs r3, r0, r1, lsl #12\n"
        "mov ip, #155\n"
        "orrne ip, ip, #67108864\n"
        "bne __fpl_dcheck_NaN1\n"
        "bx lr\n");
}

// (the result of a comparison with a NaN: the flags or 0, by ip)
// 0x00839864 | mk7dlp:bytes [tier B]
extern "C" __attribute__((naked, pcs("aapcs"))) void __fpl_cmpreturn()
{
    asm volatile(
        ".syntax unified\n"
        "tst ip, #1048576\n"
        "beq .L839898\n"
        "tst ip, #458752\n"
        "beq .L839880\n"
        "mvn r1, ip, lsl #15\n"
        "lsls r1, r1, #1\n"
        "bx lr\n"
        ".L839880:\n"
        "mov r0, #805306368\n"
        "mrs r2, CPSR\n"
        "bic r2, r2, #-268435456\n"
        "orr r2, r2, r0\n"
        "msr CPSR_f, r2\n"
        "bx lr\n"
        ".L839898:\n"
        "mov r0, #0\n"
        "bx lr\n");
}
