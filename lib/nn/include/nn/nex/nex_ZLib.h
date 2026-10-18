#pragma once

// The copy of zlib 1.2.5 that nex links (the version string "1.2.5" is in the binary; pia uses the
// zlib of the cx library). The functions are those of the public zlib API; the namespace and the
// names of the allocation callbacks are ours, the copy has no symbols of its own.

#include "decomp.h"
#include "nn/cx/cx_Zlib.h"

namespace nn {
namespace nex {
namespace zlib {
#define Z_BUF_ERROR (-5)

int deflateInit2_(z_stream* strm, int level, int method, int windowBits, int memLevel, int strategy, const char* version, int stream_size); // 0x007B57E8
int deflate(z_stream* strm, int flush); // 0x007B4688
int deflateEnd(z_stream* strm); // 0x007B5708
int deflateReset(z_stream* strm); // 0x007B5A74
int inflateInit_(z_stream* strm, const char* version, int stream_size); // 0x007B759C
int inflate(z_stream* strm, int flush); // 0x007B5BAC
int inflateEnd(z_stream* strm); // 0x007B7550
int inflateReset(z_stream* strm); // 0x007B76F0
// the allocation of zlib through the nex memory manager
void* Allocate(void* pOpaque, unsigned int items, unsigned int size); // 0x001614AC (name is ours)
void Free(void* pOpaque, void* pAddress); // 0x00161504 (name is ours)
} // namespace zlib
} // namespace nex
} // namespace nn
