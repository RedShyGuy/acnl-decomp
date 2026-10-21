#include "nn/cx/cx_Zlib.h"

// The zlib API of the cx library: each function passes the call on to the nncxZlib_ function of the
// zlib sources (lib/nn/src/nn/cx/zlib). Except for deflateInit2_ the call is a tail call that
// armlink replaced by a nop, so the entry falls into the function behind it.

// 0x007A456C
extern "C" int deflateInit2_(z_stream* strm, int level, int method, int windowBits, int memLevel, int strategy,
                             const char* version, int stream_size)
{
    return nncxZlib_deflateInit2_(strm, level, method, windowBits, memLevel, strategy, version, stream_size);
}

// 0x007AFED0
extern "C" int deflate(z_stream* strm, int flush)
{
    return nncxZlib_deflate(strm, flush);
}

// 0x007B102C
extern "C" unsigned long deflateBound(z_stream* strm, unsigned long sourceLen)
{
    return nncxZlib_deflateBound(strm, sourceLen);
}

// 0x007B112C
extern "C" int deflateEnd(z_stream* strm)
{
    return nncxZlib_deflateEnd(strm);
}
