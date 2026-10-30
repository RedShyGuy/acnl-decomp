// The C++ run-time support of the ARM C library that is Thumb code in the original: the guard of function-local
// statics (single threaded: the guard word is set on the first call), the array construction and destruction of
// the run-time ABI, and the constructor calls of the static objects.

#include "decomp.h"
#include "clib/clib_Thumb.h"

void operator delete[](void* p);

namespace {
// the exception state of a thread (names are ours)
struct ExceptionGlobals
{
    u32 _unknown0x0;
    void (*m_UnexpectedHandler)();       // 0x4
    void (*m_TerminateHandler)();        // 0x8
    u8 _unknown0xC;
    u8 _paddingD[3];
    void (*m_CurrentTerminate)();        // 0x10 (of the exception being handled)
    u32 _unknown0x14;
    u32 _unknown0x18;
    u8* m_pEmergencyBuffer;              // 0x1C
};

// the size of the emergency buffer: a flag word pair, then 128 bytes
const size_t EMERGENCY_BUFFER_SIZE = 0x88;
const size_t EMERGENCY_BLOCK_SIZE = 128;
} // namespace

extern "C" {

void* malloc(size_t size);
void abort();
void** __cxa_eh_globals_slot();
// (weak references the program does not define: 0)
void __cxa_default_unexpected() __attribute__((weak));
void __cxa_emergency_buffer_used() __attribute__((weak));

// a buffer for exceptions thrown when the heap is exhausted
// 0x00100850 | nintendogs:callgraph [tier C]
u8* __ARM_exceptions_buffer_init()
{
    u8* pBuffer = static_cast<u8*>(malloc(EMERGENCY_BUFFER_SIZE));
    if (pBuffer != NULL) {
        pBuffer[0] = 0;                      // free
    }
    return pBuffer;
}

// 0x00100862 (name is ours)
void* __ARM_exceptions_buffer_allocate(u8* pBuffer, size_t size)
{
    if (size > EMERGENCY_BLOCK_SIZE || pBuffer == NULL || pBuffer[0] != 0) {
        return NULL;
    }
    pBuffer[0] = 1;
    return pBuffer + 8;
}

// 0x0010087C (name is ours)
u8* __ARM_exceptions_buffer_free(u8* pBuffer, void* p)
{
    if (pBuffer == NULL || p != pBuffer + 8) {
        return NULL;
    }
    pBuffer[0] = 0;
    return pBuffer;
}

// 0x00100894 (name is ours)
void __cxa_default_terminate()
{
    abort();
}

// the exception state of the current thread, created on the first use
// 0x00100898 (name is ours, after the C++ ABI)
ExceptionGlobals* __cxa_get_globals()
{
    void** ppSlot = __cxa_eh_globals_slot();
    ExceptionGlobals* pGlobals = static_cast<ExceptionGlobals*>(*ppSlot);
    if (pGlobals == NULL) {
        pGlobals = static_cast<ExceptionGlobals*>(malloc(sizeof(ExceptionGlobals)));
        if (pGlobals == NULL) {
            abort();
        }
        *ppSlot = pGlobals;
        pGlobals->m_UnexpectedHandler = __cxa_default_unexpected;
        pGlobals->_unknown0x0 = 0;
        pGlobals->m_TerminateHandler = __cxa_default_terminate;
        pGlobals->_unknown0xC = 0;
        pGlobals->m_CurrentTerminate = NULL;
        pGlobals->_unknown0x14 = 0;
        pGlobals->_unknown0x18 = 0;
        pGlobals->m_pEmergencyBuffer = __cxa_emergency_buffer_used != NULL ? __ARM_exceptions_buffer_init() : NULL;
    }
    return pGlobals;
}

// calls the terminate handler of the exception (or the current one) and stops
// 0x001007A4 (name is ours)
void __cxa_call_terminate()
{
    ExceptionGlobals* pGlobals = __cxa_get_globals();
    void (*handler)() = pGlobals->m_CurrentTerminate;
    if (handler != NULL) {
        pGlobals->m_CurrentTerminate = NULL;
        handler();
    } else {
        pGlobals->m_TerminateHandler();
    }
    abort();
}

// 1 on the first call (the caller constructs the static), 0 after that
// 0x002FE96C | fefates:bytes [tier B]
int __cxa_guard_acquire(int* guard)
{
    if (*guard != 0) {
        return 0;
    }
    *guard = 1;
    return 1;
}

// 0x00301B3C | manual:abi [tier B]
void* __aeabi_vec_ctor_nocookie_nodtor(void* array, void* (*construct)(void*), size_t size, size_t count)
{
    if (construct != NULL) {
        u8* element = static_cast<u8*>(array);
        while (count-- != 0) {
            construct(element);
            element += size;
        }
    }
    return array;
}

// copies the elements of source into array with copy (a copy constructor)
// 0x0011C0E8 (name is ours, after the ARM run-time ABI)
void* __aeabi_vec_cctor_nocookie_nodtor(void* array, const void* source, size_t size, size_t count,
                                        void* (*copy)(void*, const void*))
{
    if (copy != NULL) {
        u8* element = static_cast<u8*>(array);
        const u8* from = static_cast<const u8*>(source);
        while (count-- != 0) {
            copy(element, from);
            element += size;
            from += size;
        }
    }
    return array;
}

void* __aeabi_vec_dtor(void* array, void* (*destroy)(void*), size_t size, size_t count);

// destroys an array with a cookie (element size and count in the two words in front of it)
// 0x0011C112 (name is ours, after the ARM run-time ABI)
DECOMP_NOINLINE void* __aeabi_vec_dtor_cookie(void* array, void* (*destroy)(void*))
{
    if (array == NULL) {
        return NULL;
    }
    const size_t* cookie = static_cast<const size_t*>(array) - 2;
    return __aeabi_vec_dtor(array, destroy, cookie[0], cookie[1]);
}

// destroys and frees an array with a cookie
// 0x003078D4 (name is ours, after the ARM run-time ABI)
void __aeabi_vec_delete(void* array, void* (*destroy)(void*))
{
    operator delete[](__aeabi_vec_dtor_cookie(array, destroy));
}

// destroys the elements from the last one on; returns the address of the cookie in front of the array
// 0x002F76D8 (name is ours, after the ARM run-time ABI)
DECOMP_NOINLINE void* __aeabi_vec_dtor(void* array, void* (*destroy)(void*), size_t size, size_t count)
{
    u8* first = static_cast<u8*>(array);
    if (count != 0) {
        u8* element = first + size * count;
        do {
            element -= size;
            destroy(element);
        } while (first != element);
    }
    return first - 8;
}

// the constructors of the static objects: the table of offsets to the initialization functions (.init_array)
// 0x001019D8 (name is ours)
void __cpp_initialize__aeabi_()
{
    extern const s32 s_InitArrayStart[] asm("__init_array_start");
    extern const s32 s_InitArrayEnd[] asm("__init_array_end");
    for (const s32* entry = s_InitArrayStart; entry != s_InitArrayEnd; entry++) {
        reinterpret_cast<void (*)()>(reinterpret_cast<uptr>(entry) + *entry)();
    }
}

} // extern "C"
