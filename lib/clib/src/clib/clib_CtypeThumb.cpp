// The character conversions and the multibyte functions of the ARM C library (Thumb code in the original). The
// classes come from the ctype table of the locale (__rt_ctype_table); behind the 256 classes the block has a flag
// for a multibyte locale (+0x101) and the offset of its conversion function (+0x103, from there).

#include "decomp.h"
#include "clib/clib_Locale.h"
#include "clib/clib_Thumb.h"

namespace {
// the values of errno (names are ours)
const int ERROR_ILLEGAL_SEQUENCE = 4;

// the results of the multibyte conversion (names are ours)
const int MB_INVALID = -1;
const int MB_INCOMPLETE = -2;

typedef int (*MultibyteConverter)(wchar_t* character, u8 byte, mbstate_t* state);
typedef int (*WideConverter)(char* buffer, wchar_t character, mbstate_t* state);
} // namespace

extern "C" {

int* __aeabi_errno_addr();

// 0x00100DC0 | manual:ours [tier A]
DECOMP_NOINLINE int tolower(int c)
{
    if (((*__rt_ctype_table())[c] & clib::CTYPE_UPPER) != 0) {
        return c + 32;
    }
    return c;
}

// (the classes of the "C" locale: only ASCII and Latin-1 letters; the sharp s has no capital)
// 0x00100DDA | manual:abi [tier B]
wchar_t towupper(wchar_t c)
{
    if (((*__rt_ctype_table())[c] & clib::CTYPE_LOWER) != 0 && c != 0xDF) {
        return c - 32;
    }
    return c;
}

// 0x002F88E8 (name is ours, after the C standard)
wchar_t towlower(wchar_t c)
{
    if (c <= 0x7F) {
        return tolower(c);
    }
    return c;
}

// 0x00100CF8 | manual:abi [tier B]
int mbsinit(const mbstate_t* state)
{
    if (state == NULL || state->m_State == 0) {
        return 1;
    }
    return 0;
}

int mbrtowc(wchar_t* character, const char* string, size_t size, mbstate_t* state);

// (for a state of NULL the original has the call of an empty function, removed by armlink)
// 0x00100D0A | manual:abi [tier B]
int mbrlen(const char* string, size_t size, mbstate_t* state)
{
    return mbrtowc(NULL, string, size, state);
}

// one byte of a multibyte character: 1 complete, MB_INCOMPLETE, MB_INVALID
// 0x002F7A08 (name is ours)
DECOMP_NOINLINE int __mbrtowc_byte(wchar_t* character, u8 byte, mbstate_t* state)
{
    const u8* classes = *__rt_ctype_table();
    if (classes[0x101] == 1) {
        // a multibyte locale: its conversion function
        const u8* offset = classes + 0x103;
        return reinterpret_cast<MultibyteConverter>(offset + *reinterpret_cast<const s32*>(offset))(character, byte,
                                                                                                    state);
    }
    if (classes[byte] == 0) {
        return MB_INVALID;
    }
    *character = byte;
    return 1;
}

// the bytes of one wide character in buffer: their count, or -1
// 0x002F8558 (name is ours)
int __wcrtomb_char(char* buffer, wchar_t character, mbstate_t* state)
{
    const u8* classes = *__rt_ctype_table();
    if (classes[0x101] == 1) {
        // a multibyte locale: its conversion function
        const u8* offset = classes + 0x107;
        return reinterpret_cast<WideConverter>(offset + *reinterpret_cast<const s32*>(offset))(buffer, character,
                                                                                                state);
    }
    if (character > 0xFF || classes[character] == 0) {
        return -1;
    }
    *buffer = character;
    return 1;
}

// 0x002F7FC0 (name is ours)
int __isdigit(int c)
{
    if (static_cast<u32>(c - '0') < 10) {
        return 1;
    }
    return 0;
}

// (for a state of NULL the original has the call of an empty function, removed by armlink)
// 0x002F7994 | manual:abi [tier B]
int mbrtowc(wchar_t* character, const char* string, size_t size, mbstate_t* state)
{
    int count = 0;
    if (string == NULL) {
        character = NULL;
        size = 1;
        string = "";
    }
    wchar_t result;
    while (size-- != 0) {
        int converted = __mbrtowc_byte(&result, *string++, state);
        count++;
        if (converted == MB_INVALID) {
            *__aeabi_errno_addr() = ERROR_ILLEGAL_SEQUENCE;
            return converted;
        }
        if (converted != MB_INCOMPLETE) {
            if (character != NULL) {
                *character = result;
            }
            return result != 0 ? count : 0;
        }
    }
    return MB_INCOMPLETE;
}

void* memcpy(void* destination, const void* source, size_t size);

} // extern "C"

namespace {
// the conversion state of wcstombs: the call that would get it is an empty function in the original, which
// armlink replaced by two nops (the state is NULL)
inline mbstate_t* WcstombsState()
{
    return NULL;
}

// the same for wcrtomb without a state
inline mbstate_t* WcrtombState()
{
    return NULL;
}
} // namespace

extern "C" {

// at most size bytes (and no terminator); -1 for a character without a multibyte form
// 0x0010147E | manual:ours [tier A]
size_t wcstombs(char* pDestination, const wchar_t* pSource, size_t size)
{
    size_t total = 0;
    mbstate_t* pState = WcstombsState();
    for (;;) {
        mbstate_t saved;
        if (pState != NULL) {
            saved = *pState;
        }
        char bytes[8];
        int count = __wcrtomb_char(bytes, *pSource, pState);
        if (count == -1) {
            return count;
        }
        if (pDestination != NULL) {
            if (count + total > size) {
                if (pState != NULL) {
                    *pState = saved;
                }
                return total;
            }
            memcpy(pDestination + total, bytes, count);
        }
        total += count;
        if (*pSource == 0) {
            return total - 1;
        }
        pSource++;
    }
}

// 0x001014EA (name is ours, after the C standard)
s64 llabs(s64 value)
{
    if (value < 0) {
        return -value;
    }
    return value;
}

// the bytes of c (at most 8); -1 (EILSEQ) for a character without a multibyte form
// 0x001032F2 (name is ours, after the C standard)
size_t wcrtomb(char* pDestination, wchar_t c, mbstate_t* pState)
{
    if (pState == NULL) {
        pState = WcrtombState();
    }
    char bytes[8];
    int count = __wcrtomb_char(bytes, c, pState);
    if (count == -1) {
        *__aeabi_errno_addr() = ERROR_ILLEGAL_SEQUENCE;
    } else if (pDestination != NULL) {
        memcpy(pDestination, bytes, count);
    }
    return count;
}

// MB_CUR_MAX: the longest multibyte character of the locale (1 without multibyte characters)
// 0x0031B580 (name is ours)
int __mb_cur_max()
{
    const u8* pClasses = *__rt_ctype_table();
    if (pClasses[0x101] != 1) {
        return 1;
    }
    return *reinterpret_cast<const int*>(pClasses + 0x10B);
}

} // extern "C"


