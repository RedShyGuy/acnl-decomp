// The memory functions of the ARM C library: hand-written assembly in the original (copies and fills by
// words and blocks after aligning the destination), so the bodies are copied from ACNL USA 1.5
// (orig/0004000000086300/code.elf) as naked functions, like nnnstdMemCpy. armlink split them into pieces that
// branch into each other; where it removed the tail branch to the following piece ("nop" or a plain fall
// through), the copy branches to it.

#include <cstddef>

// 0x002F83C4 | nintendogs:callseq-callee [tier A]
extern "C" __attribute__((naked)) void __rt_memclr_w(void* destination, size_t size)
{
    asm volatile(
        ".syntax unified\n"
        "mov r2, #0\n"
        "b _memset_w\n");
}

// (fills a word-aligned area by words; the fill value is a word with the byte four times)
// 0x002F83C8 (name is ours)
extern "C" __attribute__((naked)) void _memset_w(void* destination, size_t size, int value)
{
    asm volatile(
        ".syntax unified\n"
        "stmfd sp!, {lr}\n"
        "mov r3, r2\n"
        "mov ip, r2\n"
        "mov lr, r2\n"
        "subs r1, r1, #32\n"
        ".L2f83dc:\n"
        "stmiacs r0!, {r2, r3, ip, lr}\n"
        "stmiacs r0!, {r2, r3, ip, lr}\n"
        "subscs r1, r1, #32\n"
        "bcs .L2f83dc\n"
        "lsls r1, r1, #28\n"
        "stmiacs r0!, {r2, r3, ip, lr}\n"
        "stmiami r0!, {r2, r3}\n"
        "ldmfd sp!, {lr}\n"
        "lsls r1, r1, #2\n"
        "strcs r2, [r0], #4\n"
        "bxeq lr\n"
        "strhmi r2, [r0], #2\n"
        "tst r1, #1073741824\n"
        "strbne r2, [r0], #1\n"
        "bx lr\n");
}

// (aligns the destination; an aligned source goes on in __rt_memcpy_w)
// 0x002F85E4 | libgarden [tier A]
extern "C" __attribute__((naked)) void* memcpy(void* destination, const void* source, size_t size)
{
    asm volatile(
        ".syntax unified\n"
        "cmp r2, #3\n"
        "bls _memcpy_lastbytes\n"
        "ands ip, r0, #3\n"
        "beq .L2f8618\n"
        "ldrb r3, [r1], #1\n"
        "cmp ip, #2\n"
        "add r2, r2, ip\n"
        "ldrbls ip, [r1], #1\n"
        "strb r3, [r0], #1\n"
        "ldrbcc r3, [r1], #1\n"
        "strbls ip, [r0], #1\n"
        "sub r2, r2, #4\n"
        "strbcc r3, [r0], #1\n"
        ".L2f8618:\n"
        "ands r3, r1, #3\n"
        "beq __rt_memcpy_w\n"
        "subs r2, r2, #8\n"
        ".L2f8624:\n"
        "bcc .L2f863c\n"
        "ldr r3, [r1], #4\n"
        "subs r2, r2, #8\n"
        "ldr ip, [r1], #4\n"
        "stmia r0!, {r3, ip}\n"
        "b .L2f8624\n"
        ".L2f863c:\n"
        "adds r2, r2, #4\n"
        "ldrpl r3, [r1], #4\n"
        "strpl r3, [r0], #4\n"
        "b _memcpy_lastbytes\n");
}

// 0x002F864C | nintendogs:bytes [tier B]
extern "C" __attribute__((naked)) void _memcpy_lastbytes(void* destination, const void* source, size_t size)
{
    asm volatile(
        ".syntax unified\n"
        "lsls r2, r2, #31\n"
        "ldrbcs r3, [r1], #1\n"
        "ldrbcs ip, [r1], #1\n"
        "ldrbmi r2, [r1], #1\n"
        "strbcs r3, [r0], #1\n"
        "strbcs ip, [r0], #1\n"
        "strbmi r2, [r0], #1\n"
        "bx lr\n");
}

// 0x002FBC00 | nintendogs:bytes [tier A]
extern "C" __attribute__((naked)) void __rt_memcpy_w(void* destination, const void* source, size_t size)
{
    asm volatile(
        ".syntax unified\n"
        "push {r4, r5, r6, r7, r8, r9, sl, lr}\n"
        "subs r2, r2, #32\n"
        "bcc .L2fbc34\n"
        "ldm r1!, {r3, r4, r5, r6}\n"
        "pld [r1, #64]\n"
        "ldm r1!, {r7, r8, r9, sl}\n"
        ".L2fbc18:\n"
        "stmia r0!, {r3, r4, r5, r6}\n"
        "subs r2, r2, #32\n"
        "stmia r0!, {r7, r8, r9, sl}\n"
        "ldmcs r1!, {r3, r4, r5, r6}\n"
        "pld [r1, #64]\n"
        "ldmcs r1!, {r7, r8, r9, sl}\n"
        "bcs .L2fbc18\n"
        ".L2fbc34:\n"
        "lsls ip, r2, #28\n"
        "ldmcs r1!, {r3, r4, ip, lr}\n"
        "stmiacs r0!, {r3, r4, ip, lr}\n"
        "ldmmi r1!, {r3, r4}\n"
        "stmiami r0!, {r3, r4}\n"
        "pop {r4, r5, r6, r7, r8, r9, sl, lr}\n"
        "lsls ip, r2, #30\n"
        "ldrcs r3, [r1], #4\n"
        "strcs r3, [r0], #4\n"
        "bxeq lr\n"
        "lsls r2, r2, #31\n"
        "ldrhcs r3, [r1], #2\n"
        "ldrbmi r2, [r1], #1\n"
        "strhcs r3, [r0], #2\n"
        "strbmi r2, [r0], #1\n"
        "bx lr\n");
}

// 0x00301278 | nintendogs:callgraph [tier A]
extern "C" __attribute__((naked)) void __rt_memclr(void* destination, size_t size)
{
    asm volatile(
        ".syntax unified\n"
        "mov r2, #0\n"
        "b _memset\n");
}

// (the fill value is a word with the byte four times)
// 0x0030127C | nintendogs:bytes-fuzzy [tier B]
extern "C" __attribute__((naked)) void _memset(void* destination, size_t size, int value)
{
    asm volatile(
        ".syntax unified\n"
        "cmp r1, #4\n"
        "bcc .L3012a4\n"
        "ands ip, r0, #3\n"
        "beq _memset_w\n"
        "rsb ip, ip, #4\n"
        "cmp ip, #2\n"
        "strbne r2, [r0], #1\n"
        "sub r1, r1, ip\n"
        "strhge r2, [r0], #2\n"
        "b _memset_w\n"
        ".L3012a4:\n"
        "lsls ip, r1, #31\n"
        "strbcs r2, [r0], #1\n"
        "strbcs r2, [r0], #1\n"
        "strbmi r2, [r0], #1\n"
        "bx lr\n");
}

// (the body of memset)
// 0x0015114C (name is ours)
extern "C" __attribute__((naked)) void* memsetImpl(void* destination, int value, size_t size)
{
    asm volatile(
        ".syntax unified\n"
        "and r3, r1, #255\n"
        "mov r1, r2\n"
        "push {r0, lr}\n"
        "orr r2, r3, r3, lsl #8\n"
        "orr r2, r2, r2, lsl #16\n"
        "bl _memset\n"
        "pop {r0, pc}\n");
}

// 0x001401F0 | nintendogs:callgraph [tier A]
extern "C" __attribute__((naked)) void __aeabi_memset(void* destination, size_t size, int value)
{
    asm volatile(
        ".syntax unified\n"
        "and r3, r2, #255\n"
        "orr r2, r3, r3, lsl #8\n"
        "orr r2, r2, r2, lsl #16\n"
        "b _memset\n");
}

// (copies backwards when the areas overlap)
// 0x0031C4E0 | nintendogs:bytes [tier A]
extern "C" __attribute__((naked)) void __rt_memmove(void* destination, const void* source, size_t size)
{
    asm volatile(
        ".syntax unified\n"
        "subs r3, r0, r1\n"
        "cmpcs r2, r3\n"
        "bls memcpy\n"
        "cmp r2, #3\n"
        "add r0, r0, r2\n"
        "add r1, r1, r2\n"
        "bls __memmove_lastfew\n"
        "tst r0, #3\n"
        ".L31c500:\n"
        "ldrbne r3, [r1, #-1]!\n"
        "subne r2, r2, #1\n"
        "strbne r3, [r0, #-1]!\n"
        "tst r0, #3\n"
        "bne .L31c500\n"
        "ands r3, r1, #3\n"
        "beq __memmove_aligned\n"
        "subs r2, r2, #8\n"
        ".L31c520:\n"
        "bcc .L31c53c\n"
        "ldr r3, [r1, #-4]!\n"
        "subs r2, r2, #8\n"
        "ldr ip, [r1, #-4]!\n"
        "str r3, [r0, #-4]!\n"
        "str ip, [r0, #-4]!\n"
        "b .L31c520\n"
        ".L31c53c:\n"
        "adds r2, r2, #4\n"
        "ldrpl r3, [r1, #-4]!\n"
        "strpl r3, [r0, #-4]!\n"
        "b __memmove_lastfew\n");
}

// 0x0031C54C | nintendogs:bytes [tier B]
extern "C" __attribute__((naked)) void __memmove_lastfew(void* destinationEnd, const void* sourceEnd, size_t size)
{
    asm volatile(
        ".syntax unified\n"
        "lsls r2, r2, #31\n"
        "ldrbcs r3, [r1, #-1]!\n"
        "ldrbcs ip, [r1, #-1]!\n"
        "ldrbmi r2, [r1, #-1]\n"
        "strbcs r3, [r0, #-1]!\n"
        "strbcs ip, [r0, #-1]!\n"
        "strbmi r2, [r0, #-1]\n"
        "bx lr\n");
}

// 0x0013A36C | nintendogs:bytes [tier A]
extern "C" __attribute__((naked)) void __rt_memmove_w(void* destination, const void* source, size_t size)
{
    asm volatile(
        ".syntax unified\n"
        "subs r3, r0, r1\n"
        "cmpcs r2, r3\n"
        "bls __rt_memcpy_w\n"
        "add r0, r0, r2\n"
        "tst r0, #3\n"
        "add r1, r1, r2\n"
        ".L13a384:\n"
        "subne r2, r2, #1\n"
        "ldrbne r3, [r1, #-1]!\n"
        "strbne r3, [r0, #-1]!\n"
        "tst r0, #3\n"
        "bne .L13a384\n"
        "b __memmove_aligned\n");
}

// 0x0013A398 | nintendogs:callgraph [tier C]
extern "C" __attribute__((naked)) void __memmove_aligned(void* destinationEnd, const void* sourceEnd, size_t size)
{
    asm volatile(
        ".syntax unified\n"
        "subs r2, r2, #16\n"
        "bcc .L13a3b8\n"
        "push {r4, lr}\n"
        ".L13a3a4:\n"
        "ldmdb r1!, {r3, r4, ip, lr}\n"
        "subs r2, r2, #16\n"
        "stmdb r0!, {r3, r4, ip, lr}\n"
        "bcs .L13a3a4\n"
        "pop {r4, lr}\n"
        ".L13a3b8:\n"
        "lsls r3, r2, #29\n"
        "ldmdbcs r1!, {r3, ip}\n"
        "stmdbcs r0!, {r3, ip}\n"
        "ldrmi r3, [r1, #-4]!\n"
        "strmi r3, [r0, #-4]!\n"
        "tst r2, #3\n"
        "bxeq lr\n"
        "lsls r2, r2, #31\n"
        "ldrhcs r3, [r1, #-2]!\n"
        "ldrbmi r2, [r1, #-1]\n"
        "strhcs r3, [r0, #-2]!\n"
        "strbmi r2, [r0, #-1]\n"
        "bx lr\n");
}

// (falls into memsetImpl)
// 0x00151148 | libgarden [tier A]
extern "C" void* memset(void* destination, int value, size_t size)
{
    return memsetImpl(destination, value, size);
}
