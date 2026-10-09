#pragma once

#include "decomp.h"

// The PICA register values that gr passes around. The type names are from the symbols; the
// values are the GPU register encodings documented on 3dbrew ("GPU/Internal Registers"), so the
// code below compares against plain numbers with a comment instead of inventing enumerator names.

// color buffer format: 0 RGBA8, 1 RGB8, 2 RGB5A1, 3 RGB565, 4 RGBA4
enum PicaDataColor : u8
{
};

// depth buffer format: 0 D16, 2 D24, 3 D24S8
enum PicaDataDepth : u8
{
};

// primitive mode of the geometry pipeline: 3 = geometry shader primitives
enum PicaDataDrawMode : u8
{
};

// vertex attribute format: (component count - 1) << 2 | component type
// (0 s8, 1 u8, 2 s16, 3 f32)
enum PicaDataVertexAttrType : u8
{
};

namespace nn {
namespace gr {
namespace CTR {
// the two command buffer channels of the GPU (0x23C/0x23D are their kick registers)
enum CommandBufferChannel : u8
{
};
} // namespace CTR
} // namespace gr
} // namespace nn
