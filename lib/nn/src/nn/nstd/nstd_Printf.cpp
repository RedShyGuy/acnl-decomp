#include "nn/nstd/nstd_String.h"

namespace nn {
namespace nstd {
namespace {
// the flags of a conversion (names are ours)
const u32 FLAG_BLANK = 0x1;         // ' '
const u32 FLAG_PLUS = 0x2;          // '+' (only behind a ' ')
const u32 FLAG_PREFIX = 0x4;        // 0x before hexadecimal, 0 before octal (%p)
const u32 FLAG_LEFT = 0x8;          // '-'
const u32 FLAG_ZERO = 0x10;         // '0'
const u32 FLAG_LONG = 0x20;         // l
const u32 FLAG_SHORT = 0x40;        // h
const u32 FLAG_LONG_LONG = 0x80;    // ll
const u32 FLAG_CHAR = 0x100;        // hh
const u32 FLAG_UNSIGNED = 0x1000;

// the output: the characters behind the size of the buffer are counted but not written
// (names are ours)
struct Output
{
    char* m_Current;
    size_t m_Rest;

    void PutChar(char c)
    {
        if (m_Rest != 0) {
            *m_Current = c;
            m_Rest--;
        }
        m_Current++;
    }

    void Fill(char c, int count)
    {
        size_t length = m_Rest > static_cast<size_t>(count) ? count : m_Rest;
        for (size_t i = 0; i < length; i++) {
            m_Current[i] = c;
        }
        m_Rest -= length;
        m_Current += count;
    }

    void Copy(const char* string, int count)
    {
        size_t length = m_Rest > static_cast<size_t>(count) ? count : m_Rest;
        for (size_t i = 0; i < length; i++) {
            m_Current[i] = string[i];
        }
        m_Rest -= length;
        m_Current += count;
    }
};

inline bool IsDigit(char c)
{
    return static_cast<u32>(c - '0') <= 9;
}
} // namespace

// 0x0047E76C | tier C
int TSNPrintf(char* buffer, size_t size, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    const int length = TVSNPrintf(buffer, size, format, args);
    va_end(args);
    return length;
}

// 0x007EA370 (name is ours)
int TVSNPrintf(char* buffer, size_t size, const char* format, va_list args)
{
    Output out;
    out.m_Current = buffer;
    out.m_Rest = size;
    const char* p = format;
    while (*p != 0) {
        if (*p != '%') {
            out.PutChar(*p);
            p++;
            continue;
        }

        const char* spec = p;
        u32 flags = 0;
        int width = 0;
        int precision = -1;
        u32 base = 10;
        // the letters of the hexadecimal digits from 10 on
        char hexBase = 'a' - 10;
        for (;;) {
            char c = *++p;
            if (c == ' ') {
                flags |= FLAG_BLANK;
            } else if (c == '+') {
                if (p[-1] != ' ') {
                    break;
                }
                flags |= FLAG_PLUS;
            } else if (c == '-') {
                flags |= FLAG_LEFT;
            } else if (c == '0') {
                flags |= FLAG_ZERO;
            } else {
                break;
            }
        }

        if (*p == '*') {
            p++;
            width = va_arg(args, int);
            if (width < 0) {
                width = -width;
                flags |= FLAG_LEFT;
            }
        } else {
            while (IsDigit(*p)) {
                width = width * 10 + *p++ - '0';
            }
        }
        if (*p == '.') {
            precision = 0;
            if (*++p == '*') {
                p++;
                precision = va_arg(args, int);
                if (precision < 0) {
                    precision = -1;
                }
            } else {
                while (IsDigit(*p)) {
                    precision = precision * 10 + *p++ - '0';
                }
            }
        }
        if (*p == 'h') {
            if (*++p == 'h') {
                p++;
                flags |= FLAG_CHAR;
            } else {
                flags |= FLAG_SHORT;
            }
        } else if (*p == 'l') {
            if (*++p == 'l') {
                p++;
                flags |= FLAG_LONG_LONG;
            } else {
                flags |= FLAG_LONG;
            }
        }

        switch (*p) {
        case 'd':
        case 'i':
            goto integer;
        case 'o':
            base = 8;
            flags |= FLAG_UNSIGNED;
            goto integer;
        case 'u':
            flags |= FLAG_UNSIGNED;
            goto integer;
        case 'X':
            hexBase = 'A' - 10;
            base = 16;
            flags |= FLAG_UNSIGNED;
            goto integer;
        case 'x':
            base = 16;
            flags |= FLAG_UNSIGNED;
            goto integer;
        case 'p':
            flags |= FLAG_PREFIX;
            precision = 8;
            base = 16;
            flags |= FLAG_UNSIGNED;
            goto integer;
        case 'c': {
            if (precision >= 0) {
                goto unknown;
            }
            int pad = width - 1;
            char c = static_cast<char>(va_arg(args, int));
            if (flags & FLAG_LEFT) {
                out.PutChar(c);
                if (pad > 0) {
                    out.Fill(' ', pad);
                }
            } else {
                char fill = (flags & FLAG_ZERO) ? '0' : ' ';
                if (pad > 0) {
                    out.Fill(fill, pad);
                }
                out.PutChar(c);
            }
            p++;
            continue;
        }
        case 's': {
            const char* string = va_arg(args, const char*);
            int length = 0;
            if (precision < 0) {
                while (string[length] != 0) {
                    length++;
                }
            } else {
                while (length < precision && string[length] != 0) {
                    length++;
                }
            }
            int pad = width - length;
            if (flags & FLAG_LEFT) {
                if (length > 0) {
                    out.Copy(string, length);
                }
                if (pad > 0) {
                    out.Fill(' ', pad);
                }
            } else {
                char fill = (flags & FLAG_ZERO) ? '0' : ' ';
                if (pad > 0) {
                    out.Fill(fill, pad);
                }
                if (length > 0) {
                    out.Copy(string, length);
                }
            }
            p++;
            continue;
        }
        case 'n': {
            int count = out.m_Current - buffer;
            if (flags & FLAG_CHAR) {
                // not stored
            } else if (flags & FLAG_SHORT) {
                *va_arg(args, s16*) = count;
            } else if (flags & FLAG_LONG_LONG) {
                *va_arg(args, s64*) = count;
            } else {
                *va_arg(args, int*) = count;
            }
            p++;
            continue;
        }
        case '%':
            if (spec + 1 == p) {
                out.PutChar(*p);
                p++;
                continue;
            }
            goto unknown;
        default:
            goto unknown;
        }

    unknown:
        // the conversion as it is (its last character comes as text)
        if (p - spec > 0) {
            out.Copy(spec, p - spec);
        }
        continue;

    integer: {
        char prefix[2];
        int prefixLength = 0;
        char digits[24];
        int digitCount = 0;
        u64 value;
        if (flags & FLAG_LEFT) {
            flags &= ~FLAG_ZERO;
        }
        if (precision < 0) {
            precision = 1;
        } else {
            flags &= ~FLAG_ZERO;
        }
        if (flags & FLAG_UNSIGNED) {
            if (flags & FLAG_CHAR) {
                value = static_cast<u8>(va_arg(args, int));
            } else if (flags & FLAG_SHORT) {
                value = static_cast<u16>(va_arg(args, int));
            } else if (flags & FLAG_LONG_LONG) {
                value = va_arg(args, u64);
            } else {
                value = va_arg(args, u32);
            }
            flags &= ~(FLAG_PLUS | FLAG_BLANK);
            if (flags & FLAG_PREFIX) {
                if (base == 16) {
                    if (value != 0) {
                        prefix[0] = hexBase + ('x' - ('a' - 10));
                        prefix[1] = '0';
                        prefixLength = 2;
                    }
                } else if (base == 8) {
                    prefix[0] = '0';
                    prefixLength = 1;
                }
            }
        } else {
            s64 signedValue;
            if (flags & FLAG_CHAR) {
                signedValue = static_cast<s8>(va_arg(args, int));
            } else if (flags & FLAG_SHORT) {
                signedValue = static_cast<s16>(va_arg(args, int));
            } else if (flags & FLAG_LONG_LONG) {
                signedValue = va_arg(args, s64);
            } else {
                signedValue = va_arg(args, s32);
            }
            value = signedValue;
            if (signedValue < 0) {
                prefix[0] = '-';
                value = -value;
                prefixLength = 1;
            } else if (value != 0 || precision != 0) {
                if (flags & FLAG_PLUS) {
                    prefix[0] = '+';
                    prefixLength = 1;
                } else if (flags & FLAG_BLANK) {
                    prefix[0] = ' ';
                    prefixLength = 1;
                }
            }
        }

        // the digits from the last one on
        if (base == 8) {
            while (value != 0) {
                digits[digitCount++] = static_cast<char>((value & 7) + '0');
                value >>= 3;
            }
        } else if (base == 10) {
            if ((value >> 32) != 0) {
                while (value != 0) {
                    u64 quotient = value / 10;
                    digits[digitCount++] = static_cast<char>(value - quotient * 10 + '0');
                    value = quotient;
                }
            } else {
                u32 value32 = static_cast<u32>(value);
                while (value32 != 0) {
                    digits[digitCount++] = static_cast<char>(value32 % 10 + '0');
                    value32 /= 10;
                }
            }
        } else if (base == 16) {
            while (value != 0) {
                u32 digit = value & 15;
                digits[digitCount++] = static_cast<char>(digit < 10 ? digit + '0' : digit + hexBase);
                value >>= 4;
            }
        }
        // the 0 of octal counts as a digit
        if (prefixLength > 0 && prefix[0] == '0') {
            digits[digitCount++] = '0';
            prefixLength = 0;
        }

        int zeros = precision - digitCount;
        if (flags & FLAG_ZERO) {
            if (width - digitCount - prefixLength > zeros) {
                zeros = width - digitCount - prefixLength;
            }
        }
        if (zeros > 0) {
            width -= zeros;
        }
        int pad = width - (prefixLength + digitCount);
        if (!(flags & FLAG_LEFT) && pad > 0) {
            out.Fill(' ', pad);
        }
        while (prefixLength > 0) {
            out.PutChar(prefix[--prefixLength]);
        }
        if (zeros > 0) {
            out.Fill('0', zeros);
        }
        while (digitCount > 0) {
            out.PutChar(digits[--digitCount]);
        }
        if ((flags & FLAG_LEFT) && pad > 0) {
            out.Fill(' ', pad);
        }
        p++;
    }
    }

    if (out.m_Rest != 0) {
        *out.m_Current = 0;
    } else if (size != 0) {
        buffer[size - 1] = 0;
    }
    return out.m_Current - buffer;
}

} // namespace nstd
} // namespace nn

// 0x007B2B44 | fefates:callgraph
extern "C" int nnnstdTSNPrintf(char* buffer, size_t size, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    const int length = nn::nstd::TVSNPrintf(buffer, size, format, args);
    va_end(args);
    return length;
}
