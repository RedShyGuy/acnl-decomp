// The assembly functions of nn::math (ARMv6 *Asm): hand-written assembly in the original, so the
// bodies are copied from ACNL USA 1.5 (orig/0004000000086300/code.elf) as naked functions (like
// the system calls in svc_Api.cpp). Literal loads and branches use local labels.

#include "nn/math/math_ARMv6.h"
#include "nn/math/math_MTX34.h"

namespace nn {
namespace math {

// 0x00135C3C | nintendogs:bytes [tier A]
void nn::math::ARMv6::MTX34CopyAsm(nn::math::MTX34*, const nn::math::MTX34*)
{
    asm volatile(
        "cmp r1, r0\n"
        "bxeq lr\n"
        "vldmia r1!, {s0-s5}\n"
        "mov r2, r0\n"
        "vldmia r1, {s6-s11}\n"
        "vstmia r2!, {s0-s5}\n"
        "vstmia r2, {s6-s11}\n"
        "bx lr\n"
    );
}

// 0x0014892C | nintendogs:callgraph [tier A]
void nn::math::ARMv6::VEC3TransformAsm(nn::math::VEC3*, const nn::math::MTX33*, const nn::math::VEC3*)
{
    asm volatile(
        "vldmia r1, {s0-s8}\n"
        "vldmia r2, {s9-s11}\n"
        "vmul.f32 s12, s0, s9\n"
        "vmul.f32 s13, s3, s9\n"
        "vmul.f32 s14, s6, s9\n"
        "vmla.f32 s12, s1, s10\n"
        "vmla.f32 s13, s4, s10\n"
        "vmla.f32 s14, s7, s10\n"
        "vmla.f32 s12, s2, s11\n"
        "vmla.f32 s13, s5, s11\n"
        "vmla.f32 s14, s8, s11\n"
        "vstmia r0, {s12-s14}\n"
        "bx lr\n"
    );
}

// 0x00148960 | nintendogs:callgraph [tier A]
void nn::math::ARMv6::MTX34MultAsm(nn::math::MTX34*, const nn::math::MTX34*, const nn::math::MTX34*)
{
    asm volatile(
        "vpush {d8-d10}\n"
        "vldr s3, [r1, #12]\n"
        "vldr s7, [r1, #28]\n"
        "vldr s11, [r1, #44] @ 0x2c\n"
        "vldmia r2!, {s12-s15}\n"
        "vldr s20, [r1]\n"
        "vldr s21, [r1, #16]\n"
        "vmul.f32 s0, s12, s20\n"
        "vmul.f32 s1, s13, s20\n"
        "vmul.f32 s2, s14, s20\n"
        "vmla.f32 s3, s15, s20\n"
        "vldr s20, [r1, #32]\n"
        "vmul.f32 s4, s12, s21\n"
        "vmul.f32 s5, s13, s21\n"
        "vmul.f32 s6, s14, s21\n"
        "vmla.f32 s7, s15, s21\n"
        "vldmia r2!, {s16-s19}\n"
        "vldr s21, [r1, #4]\n"
        "vmul.f32 s8, s12, s20\n"
        "vmul.f32 s9, s13, s20\n"
        "vmul.f32 s10, s14, s20\n"
        "vmla.f32 s11, s15, s20\n"
        "vldmia r2, {s12-s15}\n"
        "vldr s20, [r1, #20]\n"
        "vmla.f32 s0, s16, s21\n"
        "vmla.f32 s1, s17, s21\n"
        "vmla.f32 s2, s18, s21\n"
        "vmla.f32 s3, s19, s21\n"
        "vldr s21, [r1, #36] @ 0x24\n"
        "vmla.f32 s4, s16, s20\n"
        "vmla.f32 s5, s17, s20\n"
        "vmla.f32 s6, s18, s20\n"
        "vmla.f32 s7, s19, s20\n"
        "vldr s20, [r1, #8]\n"
        "vmla.f32 s8, s16, s21\n"
        "vmla.f32 s9, s17, s21\n"
        "vmla.f32 s10, s18, s21\n"
        "vmla.f32 s11, s19, s21\n"
        "vldr s21, [r1, #24]\n"
        "vmla.f32 s0, s12, s20\n"
        "vmla.f32 s1, s13, s20\n"
        "vmla.f32 s2, s14, s20\n"
        "vmla.f32 s3, s15, s20\n"
        "vldr s20, [r1, #40] @ 0x28\n"
        "vmla.f32 s4, s12, s21\n"
        "vmla.f32 s5, s13, s21\n"
        "vmla.f32 s6, s14, s21\n"
        "vmla.f32 s7, s15, s21\n"
        "vmla.f32 s8, s12, s20\n"
        "vmla.f32 s9, s13, s20\n"
        "vmla.f32 s10, s14, s20\n"
        "vmla.f32 s11, s15, s20\n"
        "vpop {d8-d10}\n"
        "mov r1, r0\n"
        "vstmia r1!, {s0-s3}\n"
        "vstmia r1, {s4-s11}\n"
        "bx lr\n"
    );
}

// 0x00148A44 | nintendogs:bytes [tier A]
void nn::math::ARMv6::MTX34MultScaleAsm(nn::math::MTX34*, const nn::math::MTX34*, const nn::math::VEC3*)
{
    asm volatile(
        "vldmia r1, {s0-s11}\n"
        "vldmia r2, {s12-s14}\n"
        "vmul.f32 s0, s0, s12\n"
        "vmul.f32 s1, s1, s13\n"
        "vmul.f32 s2, s2, s14\n"
        "vmul.f32 s4, s4, s12\n"
        "vmul.f32 s5, s5, s13\n"
        "vmul.f32 s6, s6, s14\n"
        "vmul.f32 s8, s8, s12\n"
        "vmul.f32 s9, s9, s13\n"
        "vmul.f32 s10, s10, s14\n"
        "vstmia r0, {s0-s11}\n"
        "bx lr\n"
    );
}

// 0x00148A78 | nintendogs:bytes [tier A]
u32 nn::math::ARMv6::MTX34InverseAsm(nn::math::MTX34*, const nn::math::MTX34*)
{
    asm volatile(
        "vldmia r1, {s0-s2}\n"
        "add r1, r1, #16\n"
        "vldmia r1, {s3-s5}\n"
        "add r1, r1, #16\n"
        "vldmia r1, {s6-s8}\n"
        "vmul.f32 s10, s0, s4\n"
        "vmul.f32 s11, s1, s5\n"
        "vmul.f32 s12, s2, s3\n"
        "vmul.f32 s13, s6, s4\n"
        "vmul.f32 s14, s3, s1\n"
        "vmul.f32 s15, s0, s7\n"
        "vmul.f32 s10, s10, s8\n"
        "vmul.f32 s11, s11, s6\n"
        "vmul.f32 s12, s12, s7\n"
        "vmls.f32 s10, s13, s2\n"
        "vmls.f32 s11, s14, s8\n"
        "vmls.f32 s12, s15, s5\n"
        "vadd.f32 s10, s10, s11\n"
        "vldr s15, .L148b90\n"
        "vadd.f32 s10, s10, s12\n"
        "vmov r2, s10\n"
        "cmp r2, #-2147483648 @ 0x80000000\n"
        "cmpne r2, #0\n"
        "moveq r0, #0\n"
        "bxeq lr\n"
        "vpush {d8-d12}\n"
        "vdiv.f32 s15, s15, s10\n"
        "vmul.f32 s16, s4, s8\n"
        "vmul.f32 s17, s1, s8\n"
        "vmul.f32 s18, s1, s5\n"
        "vmul.f32 s19, s3, s8\n"
        "vmul.f32 s20, s0, s8\n"
        "vmul.f32 s21, s0, s5\n"
        "vmul.f32 s22, s3, s7\n"
        "vmul.f32 s23, s0, s7\n"
        "vmul.f32 s24, s0, s4\n"
        "vmls.f32 s16, s7, s5\n"
        "vmls.f32 s17, s7, s2\n"
        "vmls.f32 s18, s4, s2\n"
        "vmls.f32 s19, s6, s5\n"
        "vmls.f32 s20, s6, s2\n"
        "vmls.f32 s21, s3, s2\n"
        "vmls.f32 s22, s6, s4\n"
        "vmls.f32 s23, s6, s1\n"
        "vmls.f32 s24, s3, s1\n"
        "vmul.f32 s0, s16, s15\n"
        "vnmul.f32 s1, s17, s15\n"
        "vmul.f32 s2, s18, s15\n"
        "vnmul.f32 s4, s19, s15\n"
        "vmul.f32 s5, s20, s15\n"
        "vnmul.f32 s6, s21, s15\n"
        "vmul.f32 s8, s22, s15\n"
        "vldr s12, [r1, #-20] @ 0xffffffec\n"
        "vnmul.f32 s9, s23, s15\n"
        "vmul.f32 s10, s24, s15\n"
        "vnmul.f32 s3, s0, s12\n"
        "vldr s13, [r1, #-4]\n"
        "vnmul.f32 s7, s4, s12\n"
        "vnmul.f32 s11, s8, s12\n"
        "vmls.f32 s3, s1, s13\n"
        "vldr s14, [r1, #12]\n"
        "vmls.f32 s7, s5, s13\n"
        "vmls.f32 s11, s9, s13\n"
        "vmls.f32 s3, s2, s14\n"
        "vmls.f32 s7, s6, s14\n"
        "vmls.f32 s11, s10, s14\n"
        "vpop {d8-d12}\n"
        "vstmia r0, {s0-s11}\n"
        "mov r0, #1\n"
        "bx lr\n"
        ".L148b90:\n"
        ".word 0x3f800000\n"
    );
}

// 0x00148B94 | nintendogs:bytes [tier A]
u32 nn::math::ARMv6::MTX34InvTransposeAsm(nn::math::MTX34*, const nn::math::MTX34*)
{
    asm volatile(
        "vldmia r1, {s0-s2}\n"
        "add r1, r1, #16\n"
        "vldmia r1, {s3-s5}\n"
        "add r1, r1, #16\n"
        "vldmia r1, {s6-s8}\n"
        "vmul.f32 s10, s0, s4\n"
        "vmul.f32 s11, s1, s5\n"
        "vmul.f32 s12, s2, s3\n"
        "vmul.f32 s13, s6, s4\n"
        "vmul.f32 s14, s3, s1\n"
        "vmul.f32 s15, s0, s7\n"
        "vmul.f32 s10, s10, s8\n"
        "vmul.f32 s11, s11, s6\n"
        "vmul.f32 s12, s12, s7\n"
        "vmls.f32 s10, s13, s2\n"
        "vmls.f32 s11, s14, s8\n"
        "vmls.f32 s12, s15, s5\n"
        "vadd.f32 s10, s10, s11\n"
        "vldr s15, .L148c88\n"
        "vadd.f32 s10, s10, s12\n"
        "vmov r2, s10\n"
        "cmp r2, #-2147483648 @ 0x80000000\n"
        "cmpne r2, #0\n"
        "moveq r0, #0\n"
        "bxeq lr\n"
        "vpush {d8-d12}\n"
        "vdiv.f32 s15, s15, s10\n"
        "vmul.f32 s16, s4, s8\n"
        "vmul.f32 s17, s3, s8\n"
        "vmul.f32 s18, s3, s7\n"
        "vmul.f32 s19, s1, s8\n"
        "vmul.f32 s20, s0, s8\n"
        "vmul.f32 s21, s0, s7\n"
        "vmul.f32 s22, s1, s5\n"
        "vmul.f32 s23, s0, s5\n"
        "vmul.f32 s24, s0, s4\n"
        "vmls.f32 s16, s7, s5\n"
        "vmls.f32 s17, s6, s5\n"
        "vmls.f32 s18, s6, s4\n"
        "vmls.f32 s19, s7, s2\n"
        "vmls.f32 s20, s6, s2\n"
        "vmls.f32 s21, s6, s1\n"
        "vmls.f32 s22, s4, s2\n"
        "vmls.f32 s23, s3, s2\n"
        "vmls.f32 s24, s3, s1\n"
        "vmul.f32 s0, s16, s15\n"
        "vnmul.f32 s1, s17, s15\n"
        "vmul.f32 s2, s18, s15\n"
        "vnmul.f32 s4, s19, s15\n"
        "vmul.f32 s5, s20, s15\n"
        "vnmul.f32 s6, s21, s15\n"
        "vmul.f32 s8, s22, s15\n"
        "vnmul.f32 s9, s23, s15\n"
        "vmul.f32 s10, s24, s15\n"
        "vldr s3, .L148c8c\n"
        "vldr s7, .L148c8c\n"
        "vldr s11, .L148c8c\n"
        "vpop {d8-d12}\n"
        "vstmia r0, {s0-s11}\n"
        "mov r0, #1\n"
        "bx lr\n"
        ".L148c88:\n"
        ".word 0x3f800000\n"
        ".L148c8c:\n"
        ".word 0x00000000\n"
    );
}

// 0x00148C90 | nintendogs:bytes [tier A]
void nn::math::ARMv6::MTX34MultTranslateAsm(nn::math::MTX34*, const nn::math::MTX34*, const nn::math::VEC3*)
{
    asm volatile(
        "vldmia r1, {s0-s11}\n"
        "vldmia r2, {s12-s14}\n"
        "vmla.f32 s3, s0, s12\n"
        "vmla.f32 s7, s4, s12\n"
        "vmla.f32 s11, s8, s12\n"
        "vmla.f32 s3, s1, s13\n"
        "vmla.f32 s7, s5, s13\n"
        "vmla.f32 s11, s9, s13\n"
        "vmla.f32 s3, s2, s14\n"
        "vmla.f32 s7, s6, s14\n"
        "vmla.f32 s11, s10, s14\n"
        "vstmia r0, {s0-s11}\n"
        "bx lr\n"
    );
}

// 0x00148CC4 | nintendogs:callgraph [tier A]
void nn::math::ARMv6::VEC3TransformAsm(nn::math::VEC3*, const nn::math::MTX34*, const nn::math::VEC3*)
{
    asm volatile(
        "vldmia r1, {s0-s11}\n"
        "vldmia r2, {s12-s14}\n"
        "vmla.f32 s3, s0, s12\n"
        "vmla.f32 s7, s4, s12\n"
        "vmla.f32 s11, s8, s12\n"
        "vmla.f32 s3, s1, s13\n"
        "vmla.f32 s7, s5, s13\n"
        "vmla.f32 s11, s9, s13\n"
        "vmla.f32 s3, s2, s14\n"
        "vmla.f32 s7, s6, s14\n"
        "vmla.f32 s11, s10, s14\n"
        "vstr s3, [r0]\n"
        "vstr s7, [r0, #4]\n"
        "vstr s11, [r0, #8]\n"
        "bx lr\n"
    );
}

// 0x00148D00 | nintendogs:bytes [tier A]
void nn::math::ARMv6::MTX34TransposeAsm(nn::math::MTX34*, const nn::math::MTX34*)
{
    asm volatile(
        "vldr s0, [r1]\n"
        "vldr s1, [r1, #16]\n"
        "vldr s2, [r1, #32]\n"
        "vldr s3, .L148d38\n"
        "vldr s4, [r1, #4]\n"
        "vldr s5, [r1, #20]\n"
        "vldr s6, [r1, #36] @ 0x24\n"
        "vldr s7, .L148d38\n"
        "vldr s8, [r1, #8]\n"
        "vldr s9, [r1, #24]\n"
        "vldr s10, [r1, #40] @ 0x28\n"
        "vldr s11, .L148d38\n"
        "vstmia r0, {s0-s11}\n"
        "bx lr\n"
        ".L148d38:\n"
        ".word 0x00000000\n"
    );
}

// 0x00148D44 | nintendogs:bytes [tier A]
void nn::math::ARMv6::MTX44CopyAsm(nn::math::MTX44*, const nn::math::MTX44*)
{
    asm volatile(
        "cmp r1, r0\n"
        "bxeq lr\n"
        "vldmia r1, {s0-s15}\n"
        "vstmia r0, {s0-s15}\n"
        "bx lr\n"
    );
}

// 0x00148D58 | nintendogs:bytes [tier A]
void nn::math::ARMv6::MTX34ToMTX33Asm(nn::math::MTX33*, const nn::math::MTX34*)
{
    asm volatile(
        "vldmia r1, {s0-s11}\n"
        "mov r2, r0\n"
        "vstmia r2!, {s0-s2}\n"
        "vstmia r2!, {s4-s6}\n"
        "vstmia r2, {s8-s10}\n"
        "bx lr\n"
    );
}

// 0x007E53F4 | nintendogs:bytes [tier A]
template <>
void nn::math::ARMv6::MTX33MultAsm<nn::math::MTX34>(nn::math::MTX34*, const nn::math::MTX34*, const nn::math::MTX34*)
{
    asm volatile(
        "mov r3, #0\n"
        "add r1, r1, r3\n"
        "add r2, r2, r3\n"
        "mov r3, #16\n"
        "cmp r3, #12\n"
        "bne .L7e54b4\n"
        "vpush {d8}\n"
        "vldmia r2!, {s10-s15}\n"
        "vldr s16, [r1]\n"
        "vldr s17, [r1, #12]\n"
        "vmul.f32 s0, s10, s16\n"
        "vmul.f32 s1, s11, s16\n"
        "vmul.f32 s2, s12, s16\n"
        "vldr s16, [r1, #24]\n"
        "vmul.f32 s3, s10, s17\n"
        "vmul.f32 s4, s11, s17\n"
        "vmul.f32 s5, s12, s17\n"
        "vldr s17, [r1, #4]\n"
        "vmul.f32 s6, s10, s16\n"
        "vmul.f32 s7, s11, s16\n"
        "vmul.f32 s8, s12, s16\n"
        "vldr s16, [r1, #16]\n"
        "vldmia r2, {s10-s12}\n"
        "vmla.f32 s0, s13, s17\n"
        "vmla.f32 s1, s14, s17\n"
        "vmla.f32 s2, s15, s17\n"
        "vldr s17, [r1, #28]\n"
        "vmla.f32 s3, s13, s16\n"
        "vmla.f32 s4, s14, s16\n"
        "vmla.f32 s5, s15, s16\n"
        "vldr s16, [r1, #8]\n"
        "vmla.f32 s6, s13, s17\n"
        "vmla.f32 s7, s14, s17\n"
        "vmla.f32 s8, s15, s17\n"
        "vldr s17, [r1, #20]\n"
        "vmla.f32 s0, s10, s16\n"
        "vmla.f32 s1, s11, s16\n"
        "vmla.f32 s2, s12, s16\n"
        "vldr s16, [r1, #32]\n"
        "vmla.f32 s3, s10, s17\n"
        "vmla.f32 s4, s11, s17\n"
        "vmla.f32 s5, s12, s17\n"
        "vmla.f32 s6, s10, s16\n"
        "vmla.f32 s7, s11, s16\n"
        "vmla.f32 s8, s12, s16\n"
        "vpop {d8}\n"
        "vstmia r0, {s0-s8}\n"
        "bx lr\n"
        ".L7e54b4:\n"
        "vpush {d8-d13}\n"
        "vldmia r2, {s9-s11}\n"
        "vldmia r1, {s18-s20}\n"
        "add r1, r1, r3\n"
        "add r2, r2, r3\n"
        "vldmia r2, {s12-s14}\n"
        "vldmia r1, {s21-s23}\n"
        "add r1, r1, r3\n"
        "add r2, r2, r3\n"
        "vldmia r2, {s15-s17}\n"
        "vldmia r1, {s24-s26}\n"
        "vmul.f32 s0, s9, s18\n"
        "vmul.f32 s1, s10, s18\n"
        "vmul.f32 s2, s11, s18\n"
        "vmul.f32 s3, s9, s21\n"
        "vmul.f32 s4, s10, s21\n"
        "vmul.f32 s5, s11, s21\n"
        "vmul.f32 s6, s9, s24\n"
        "vmul.f32 s7, s10, s24\n"
        "vmul.f32 s8, s11, s24\n"
        "vmla.f32 s0, s12, s19\n"
        "vmla.f32 s1, s13, s19\n"
        "vmla.f32 s2, s14, s19\n"
        "vmla.f32 s3, s12, s22\n"
        "vmla.f32 s4, s13, s22\n"
        "vmla.f32 s5, s14, s22\n"
        "vmla.f32 s6, s12, s25\n"
        "vmla.f32 s7, s13, s25\n"
        "vmla.f32 s8, s14, s25\n"
        "vmla.f32 s0, s15, s20\n"
        "vmla.f32 s1, s16, s20\n"
        "vmla.f32 s2, s17, s20\n"
        "vmla.f32 s3, s15, s23\n"
        "vmla.f32 s4, s16, s23\n"
        "vmla.f32 s5, s17, s23\n"
        "vmla.f32 s6, s15, s26\n"
        "vmla.f32 s7, s16, s26\n"
        "vmla.f32 s8, s17, s26\n"
        "vpop {d8-d13}\n"
        "add r1, r0, r3\n"
        "add r2, r1, r3\n"
        "vstmia r0, {s0-s2}\n"
        "vstmia r1, {s3-s5}\n"
        "vstmia r2, {s6-s8}\n"
        "bx lr\n"
    );
}

} // namespace math
} // namespace nn
