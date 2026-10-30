// The conversion dispatcher of printf: in the ARM C library one small piece of assembly per conversion
// ("cmp r1, #'d'; beq <formatter>"), which armlink puts together for the conversions the program uses; copied from
// ACNL USA 1.5 (orig/0004000000086300/code.elf) as a naked function (see clib_MemoryAsm.cpp). The branches to
// the Thumb formatters go through veneers of the linker.

#include "clib/clib_Printf.h"

// (the formatter of the conversion c; ll sets bit 7 of the integer conversions, l bit 7 of c and s;
// armlink built it from one piece per conversion linked in; 0: not a conversion)
// 0x0011C700 (name is ours)
extern "C" __attribute__((naked)) int __printf_dispatch(clib::PrintfState* pState, int c, const u8* pArguments)
{
    asm volatile(
        ".syntax unified\n"
        "cmp r1, #110\n"
        "beq __printf_count\n"
        "cmp r1, #112\n"
        "beq __printf_pointer\n"
        "cmp r1, #102\n"
        "beq __printf_float\n"
        "cmp r1, #101\n"
        "beq __printf_float\n"
        "cmp r1, #103\n"
        "beq __printf_float\n"
        "cmp r1, #97\n"
        "beq __printf_hexfloat\n"
        "ldr r3, [r0]\n"
        "lsrs r3, r3, #8\n"
        "orrcs r1, r1, #128\n"
        "cmp r1, #105\n"
        "beq __printf_int_dec\n"
        "cmp r1, #100\n"
        "beq __printf_int_dec\n"
        "cmp r1, #117\n"
        "beq __printf_int_dec\n"
        "cmp r1, #111\n"
        "beq __printf_int_oct\n"
        "cmp r1, #120\n"
        "beq __printf_int_hex\n"
        "cmp r1, #233\n"
        "beq __printf_longlong_dec\n"
        "cmp r1, #228\n"
        "beq __printf_longlong_dec\n"
        "cmp r1, #245\n"
        "beq __printf_longlong_dec\n"
        "cmp r1, #239\n"
        "beq __printf_longlong_oct\n"
        "cmp r1, #248\n"
        "beq __printf_longlong_hex\n"
        "ldr r3, [r0]\n"
        "lsrs r3, r3, #7\n"
        "orrcs r1, r1, #128\n"
        "cmp r1, #99\n"
        "beq __printf_char\n"
        "cmp r1, #115\n"
        "beq __printf_string\n"
        "cmp r1, #227\n"
        "beq __printf_wchar\n"
        "cmp r1, #243\n"
        "beq __printf_wstring\n"
        "movs r0, #0\n"
        "bx lr\n");
}
