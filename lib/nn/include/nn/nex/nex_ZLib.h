#pragma once

// The copy of zlib 1.2.5 that nex links (the version string "1.2.5" is in the binary; pia uses the
// zlib of the cx library). The copy has no symbols of its own: its sources
// (lib/nn/src/nn/nex/zlib, zlib 1.2.5) give every function the prefix nexZlib_ (ours); the
// addresses are in the comments there. The namespace and the names of the allocation callbacks
// are ours.

#include "decomp.h"
#include "nn/cx/cx_Zlib.h"

#define Z_BUF_ERROR (-5)

extern "C" {
int nexZlib_deflateInit2_(z_stream* strm, int level, int method, int windowBits, int memLevel, int strategy,
                          const char* version, int stream_size);
int nexZlib_deflate(z_stream* strm, int flush);
int nexZlib_deflateEnd(z_stream* strm);
int nexZlib_deflateReset(z_stream* strm);
int nexZlib_inflateInit_(z_stream* strm, const char* version, int stream_size);
int nexZlib_inflate(z_stream* strm, int flush);
int nexZlib_inflateEnd(z_stream* strm);
int nexZlib_inflateReset(z_stream* strm);
} // extern "C"

namespace nn {
namespace nex {
namespace zlib {
// the allocation of zlib through the nex memory manager
void* Allocate(void* pOpaque, unsigned int items, unsigned int size); // 0x001614AC (name is ours)
void Free(void* pOpaque, void* pAddress); // 0x00161504 (name is ours)
} // namespace zlib
} // namespace nex
} // namespace nn
