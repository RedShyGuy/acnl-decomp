#pragma once

// The exceptions of the C++ library ARMCC linked in (Rogue Wave; Thumb code in the original). The program has no
// exception handling: the library hands its errors to __rw::s_pErrorHandler, which prints them and aborts. Class
// names from the RTTI, the virtual functions after the C++ standard, the rest is ours. Not for use together with
// GCC's C++ headers.

#include <stdarg.h>
#include "types.h"

namespace std {
class exception
{
public:
    exception();
    virtual ~exception();
    virtual const char* what() const;
};

class __rw_exception : public exception
{
public:
    __rw_exception();
    virtual ~__rw_exception();
    virtual const char* what() const;

    // the message of the error id with the arguments (appended to the one there is)
    __rw_exception& Format(int id, va_list arguments);

private:
    char* m_pWhat;  // 0x4 (allocated with new[])
};
} // namespace std

namespace __rw {
// the messages of the error ids (printf formats: the first argument is the place of the error)
extern const char* const s_ErrorMessages[23];

// the message of id from the catalog of the program (RWSTDERR) formatted into pBuffer (see FormatString), or NULL
char* GetErrorMessage(char* pBuffer, size_t size, int id, va_list arguments);
// formats into pBuffer (size bytes; NULL: a buffer from new[] that grows until the text fits; then NULL if the
// given one is too small)
char* FormatString(char* pBuffer, size_t size, const char* pFormat, va_list arguments);
} // namespace __rw
