#pragma once

// zlib 1.2.7 of the cx library ("1.2.7.f-NINTENDO-CTR-SDK-v1" in the binary; the functions are
// nncxZlib_*). The stream and the functions pia calls, with the names and layout of the public
// zlib API. The functions at the addresses branch to nncxZlib_deflate etc. (armlink replaced the
// branch by a nop).

#include "decomp.h"

extern "C" {

typedef void* (*alloc_func)(void* opaque, unsigned int items, unsigned int size);
typedef void (*free_func)(void* opaque, void* address);

typedef struct z_stream_s
{
    const unsigned char* next_in; // 0x00
    unsigned int avail_in;        // 0x04
    unsigned long total_in;       // 0x08
    unsigned char* next_out;      // 0x0C
    unsigned int avail_out;       // 0x10
    unsigned long total_out;      // 0x14
    const char* msg;              // 0x18
    void* state;                  // 0x1C
    alloc_func zalloc;            // 0x20
    free_func zfree;              // 0x24
    void* opaque;                 // 0x28
    int data_type;                // 0x2C
    unsigned long adler;          // 0x30
    unsigned long reserved;       // 0x34
} z_stream;
ASSERT_SIZE(z_stream, 0x38);

#define Z_OK 0
#define Z_STREAM_END 1
#define Z_MEM_ERROR (-4)
#define Z_SYNC_FLUSH 2
#define Z_FINISH 4
#define Z_DEFLATED 8
#define Z_DEFAULT_STRATEGY 0

int deflateInit2_(z_stream* strm, int level, int method, int windowBits, int memLevel, int strategy,
                  const char* version, int stream_size); // 0x007A456C
int deflate(z_stream* strm, int flush); // 0x007AFED0
int deflateEnd(z_stream* strm); // 0x007B112C
unsigned long deflateBound(z_stream* strm, unsigned long sourceLen); // 0x007B102C

} // extern "C"
