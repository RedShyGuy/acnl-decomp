#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
// The errors of nex: SignalError records the error of the current thread (with the source file and
// line of the check). The names of the error codes are ours.
class SystemError
{
public:
    // an index is out of range
    static const u32 ERROR_OUT_OF_RANGE = 0xE0000003;
    // an object does not exist
    static const u32 ERROR_NULL = 0xE0000008;
    // a wait timed out
    static const u32 ERROR_TIMEOUT = 0xE000000C;
    // an object is registered already
    static const u32 ERROR_ALREADY_EXISTS = 0xE000000F;
    // a function is called in the wrong state
    static const u32 ERROR_INVALID_STATE = 0xE000000E;
    // a read behind the end of a buffer
    static const u32 ERROR_BUFFER_OVERRUN = 0xE0000019;
    // the module of the errors of the facade and its error that has a text
    static const u32 MODULE_FACADE = 0x10;
    static const u32 ERROR_FACADE_NOT_INITIALIZED = 0xE0100000;
    // a blocking call on the thread of the scheduler (it would wait for itself)
    static const u32 ERROR_BLOCKING_CALL = 0xE000001A;

    // records the error and its detail as the last ones of the current thread
    static void SignalError(wchar_t* file, unsigned int line, unsigned int error, unsigned int detail); // 0x0035C030 | fefates:callgraph [tier C]
    // the text of the error (at most size - 1 characters; nothing for the other errors of the
    // module of the facade)
    static void GetErrorDescription(unsigned int error, wchar_t* pBuffer, unsigned int size); // 0x0035C128 (name is ours)
    // forgets the errors of a thread (that ended)
    static void ClearThread(u32 threadId); // 0x0035C1C4
    // the last error of the current thread
    static u32 GetLastError(); // 0x0035C308 (name is ours)
    // the last error of the current thread is cleared
    static void ClearLastError(); // 0x0035C328 (name is ours)
};
} // namespace nex
} // namespace nn
