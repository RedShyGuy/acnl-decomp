// The exceptions and the error reporting of the C++ library (Thumb code in the original). See rw/rw_Exception.h.

#include <new>
#include "decomp.h"
#include "rw/rw_Exception.h"
#include "rw/rw_String.h"
#include "clib/clib_Stdio.h"
#include "clib/clib_Thumb.h"

extern "C" {
size_t strlen(const char* string);
char* strcpy(char* destination, const char* source);
char* strcat(char* destination, const char* source);
int vsnprintf(char* pDestination, size_t size, const char* pFormat, va_list arguments);
int fprintf(clib::File* pStream, const char* pFormat, ...);
void abort();
}


namespace __rw {
// 0x008C9418 (name is ours)
extern const char* const s_ErrorMessages[23] = {
    "%s: unspecified error",
    "%s: exception",
    "%s: unexpected exception",
    "%s: bad_alloc: out of memory",
    "%s: bad cast",
    "%s: logic error",
    "%s: out of memory",
    "%s: bad cast",
    "%s: length error: %u > %u",
    "%s: argument value %u out of range [0, %u)",
    "%s: runtime error",
    "%s: range error: invalid range [%d, %d)",
    "%s: overflow error",
    "%s: underflow error",
    "%s: stream object has set ios::failbit",
    "%s: stream object has set ios::badbit",
    "%s: stream object has set ios::eofbit",
    "%s: stream object has set %s",
    "%s: facet %u not found in \"%s\" locale",
    "%s: bad locale name: \"%s\"",
    "%s: failed to construct locale name",
    "%s: conversion failed",
    "%s: invalid pointer %p",
};

// prints the message (or the kind of the error) and stops the program
// 0x0010C24C (name is ours)
void DefaultErrorHandler(int id, char* pMessage)
{
    if (pMessage != NULL) {
        fprintf(&__stderr, "Exception: %s.\n", pMessage);
        delete[] pMessage;
    } else {
        const char* pKind;
        switch (id) {
        case 0:
            pKind = "exception";
            break;
        case 1:
            pKind = "unexpected exception";
            break;
        case 2:
            pKind = "bad_alloc: out of memory";
            break;
        default:
            pKind = "unknown exception";
            break;
        }
        fprintf(&__stderr, "Exception: %s.\n", pKind);
    }
    abort();
}

// 0x00982FF4 (name is ours)
void (*s_pErrorHandler)(int id, char* pMessage) = DefaultErrorHandler;

// 0x0030777C (name is ours)
char* FormatString(char* pBuffer, size_t size, const char* pFormat, va_list arguments)
{
    const size_t DEFAULT_SIZE = 64;
    size_t length = size != 0 ? size : DEFAULT_SIZE;
    if (pBuffer == NULL) {
        pBuffer = new char[length];
    }
    for (;;) {
        int written = vsnprintf(pBuffer, length, pFormat, arguments);
        if (written < 0) {
            pBuffer[0] = '\0';
            return pBuffer;
        }
        if (static_cast<int>(length) < written) {
            length = written + 1;
        } else if (static_cast<int>(length) == written) {
            length *= 2;
        } else {
            return pBuffer;
        }
        if (size != 0) {
            return NULL;
        }
        delete[] pBuffer;
        pBuffer = new char[length];
    }
}

// 0x0030757C (name is ours)
void ThrowError(int id, ...)
{
    va_list arguments;
    va_start(arguments, id);
    if (s_pErrorHandler == NULL) {
        return;
    }
    const int SIMPLE_ERRORS = 3;
    if (id <= SIMPLE_ERRORS) {
        s_pErrorHandler(id, NULL);
        return;
    }
    u32 storage[2];
    std::__rw_exception* pException = new (storage) std::__rw_exception();
    pException->Format(id, arguments);
    s_pErrorHandler(id, const_cast<char*>(pException->what()));
    pException->~__rw_exception();
    va_end(arguments);
}
} // namespace __rw

// 0x0011C024 (name after the C++ standard)
DECOMP_NOINLINE std::exception::exception()
{
    // (only the vptr is set)
}

// 0x0011C03C (name after the C++ standard)
// 0x0011C030 (deleting dtor)
std::exception::~exception()
{
    // empty (in the original too)
}

// 0x0011BAEC (name after the C++ standard)
const char* std::exception::what() const
{
    return "";
}

// 0x0011BD9C (name after the C++ standard)
std::__rw_exception::__rw_exception()
{
    m_pWhat = NULL;
}

// 0x0011BDBC (name after the C++ standard)
// 0x0011BDB0 (deleting dtor)
std::__rw_exception::~__rw_exception()
{
    delete[] m_pWhat;
}

// 0x0011B5E2 (name after the C++ standard)
const char* std::__rw_exception::what() const
{
    return m_pWhat;
}

// the message from the catalog of the program, else from the table
// 0x0011BD14 (name is ours)
std::__rw_exception& std::__rw_exception::Format(int id, va_list arguments)
{
    char* pMessage = __rw::GetErrorMessage(NULL, 0, id, arguments);
    if (pMessage == NULL) {
        if (id < 0 || id >= 23) {
            id = 0;
        }
        pMessage = __rw::FormatString(NULL, 0, __rw::s_ErrorMessages[id], arguments);
    }
    if (m_pWhat != NULL) {
        size_t length = strlen(m_pWhat);
        char* pJoined = strcat(strcpy(new char[length + strlen(pMessage) + 1], m_pWhat), pMessage);
        delete[] m_pWhat;
        delete[] pMessage;
        pMessage = pJoined;
    }
    m_pWhat = pMessage;
    return *this;
}
