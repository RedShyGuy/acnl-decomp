#pragma once

// math.h of the C library of the program (the ARM C library ARMCC linked in, decompiled in lib/clib). This directory
// comes before the system headers: the decompiled code must not use the C or C++ library of GCC.

#define INFINITY (__builtin_inff())
#define HUGE_VALF (__builtin_huge_valf())
#define NAN (__builtin_nanf(""))

// Only the float functions are in the library (with the hardware floating point names); sqrt and fabs are single
// instructions. The double functions of the C standard are not in the program.

#ifdef __cplusplus
extern "C" {
#endif

float sinf(float x) __asm__("__hardfp_sinf");
float cosf(float x) __asm__("__hardfp_cosf");
float tanf(float x) __asm__("__hardfp_tanf");
float asinf(float x) __asm__("__hardfp_asinf");
float acosf(float x) __asm__("__hardfp_acosf");
float atan2f(float y, float x) __asm__("__hardfp_atan2f");
float expf(float x) __asm__("__hardfp_expf");
float logf(float x) __asm__("__hardfp_logf");
float log10f(float x) __asm__("__hardfp_log10f");
float powf(float x, float y) __asm__("__hardfp_powf");
float ceilf(float x) __asm__("__hardfp_ceilf");
float floorf(float x) __asm__("__hardfp_floorf");
float fmodf(float x, float y) __asm__("__hardfp_fmodf");
float modff(float x, float* integralPart) __asm__("__hardfp_modff");
float sqrtf(float x);
double sqrt(double x);
float fabsf(float x);
double fabs(double x);

#ifdef __cplusplus
}
#endif
