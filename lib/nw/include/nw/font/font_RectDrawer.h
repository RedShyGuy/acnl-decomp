#pragma once

#include "decomp.h"
#include "nn/math/math_MTX44.h"
#include "nw/font/font_DispStringBuffer.h"
#include "nw/ut/internal/ut_CmdCache.h"

namespace nw {
namespace font {
class CharWriter;

// RTTI N2nw4font10RectDrawerE @ 0x008D0BEC
// vtable 0x00903690 (vptr 0x00903698), offset_to_top 0, 7 entries
//
// Draws the characters of display string buffers (CharWriter) with GPU commands: a vertex shader
// draws up to 26 characters per draw call from a fixed vertex buffer; per character it reads an
// index vector, the text colors and the texture coordinates from float uniforms that the
// commands set in batches. Member names and the names of the virtual functions are ours.
class RectDrawer
{
public:
    // a batch of float uniforms of the vertex shader as GPU commands: the index command, then
    // the first data word with the burst header (the header is set when the batch is written),
    // then the rest of the data
    template <int N>
    struct UniformBlock
    {
        u32 index;          // 0x0 (0x80000000 | the first register: 32 bit floats)
        u32 indexHeader;    // 0x4
        u32 firstData;      // 0x8
        f32 data[N][4];     // 0xC (data[0][0] is the burst header while written)
        u32 padding;
    };

    // the vertex shader registers and the batch sizes
    static const int VERTEX_INDEX_MAX = 26;   // c6 - c31
    static const int MTX_MAX = 3;             // c32 - c34
    static const int COLOR_MAX = 28;          // c35 - c62 (c35 - c38 are the colors of the string)
    static const int TEXCOORD_MAX = 32;       // c64 - c95
    // the size of the vertex buffer: per character six vertices of four floats
    static const u32 VERTEX_BUFFER_SIZE = VERTEX_INDEX_MAX * 6 * 4 * sizeof(f32);

    RectDrawer(); // 0x004D8684 (name is ours)
    virtual ~RectDrawer(); // 0x004D8738 slot 0x00 | mk7dlp:bytes
    // 0x004D8718 slot 0x04 (deleting dtor)
    // the buffers are released
    virtual void Finalize(); // 0x004D8584 slot 0x08
    // the drawing state commands into the global command list (s_pCommandBuffer)
    virtual void DrawBegin(u32 flags); // 0x004D865C slot 0x0C
    virtual u32* DrawBegin(u32* pCommand, u32 flags); // 0x004D85E4 slot 0x10
    virtual void DrawEnd(u32 flags); // 0x004D855C slot 0x14
    virtual u32* DrawEnd(u32* pCommand, u32 flags); // 0x004D8524 slot 0x18

    // the buffers and the shader (the vertex buffer is not written)
    void Initialize(void* pVertexBuffer, void* pCommandBuffer, const void* pShaderBinary, u32 size); // 0x004D6F9C | nintendogs:bytes [tier A]
    // one buffer: the command buffer, then the vertex buffer (it is written)
    void Initialize(void* pBuffer, const void* pShaderBinary, u32 size); // 0x004D6EF4 (name is ours)
    void InitializeCMD(void* pVertexBuffer, void* pCommandBuffer, const void* pShaderBinary, u32 size, bool isInitVertexBuffer); // 0x004D703C | nintendogs:callgraph [tier A]
    // the commands of the projection uniforms; the end
    u32* SetProjectionMtx(u32* pCommand, const nn::math::MTX44& mtx); // 0x004D7FB0 | libgarden [tier A]
    // the commands of the matrix uniforms; the end
    u32* AddUniformMtx(u32* pCommand); // 0x004D6FD0 | libgarden [tier A]
    // the commands of the characters of the display string buffer of the writer
    void BuildTextCommand(nw::font::CharWriter* pWriter); // 0x004D7890 | libgarden [tier A]

    // the vertex buffer of all drawers (built once)
    static const f32* GetVertexBufferData(); // 0x004D8330 | mk7dlp:bytes [tier B]
    // the size of the command buffer for the shader
    static u32 GetCommandBufferSize(const void* pShaderBinary); // 0x004D8404 (name is ours)
    static const u32* GetDrawCommands(); // 0x00744EB8 | nintendogs:callseq-callee [tier A]
    // the offset of the vertex index address: 32 if the vertices of the characters cross a 4 KiB page
    u32 GetVertexIndexAddressOffset(u32 vertexCount) const; // 0x00744EC4 | nintendogs:bytes [tier A]

    // the vector of c4 (name is ours)
    void SetParameter(f32 x, f32 y, f32 z); // 0x004D6FBC (name is ours)

private:
    // the batched uniforms and a draw call into the command buffer of the string (name is ours)
    void FlushUniforms(DispStringBuffer* pBuffer); // 0x004D8000 (name is ours)

public:
    ut::internal::CmdCache m_CommandCache;         // 0x004 the state commands
    ut::internal::CmdCache m_DrawCommandCache;     // 0x014 the draw end commands
    u8 m_VertexIndexCount;                         // 0x024
    u8 m_MtxCount;                                 // 0x025
    u8 m_TexCoordCount;                            // 0x026
    u8 m_ColorCount;                               // 0x027
    UniformBlock<5> m_ProjectionUniforms;          // 0x028 c0 - c4
    UniformBlock<VERTEX_INDEX_MAX> m_VertexIndexUniforms; // 0x088 c6
    UniformBlock<MTX_MAX> m_MtxUniforms;           // 0x238 c32
    UniformBlock<COLOR_MAX> m_ColorUniforms;       // 0x278 c35
    UniformBlock<TEXCOORD_MAX> m_TexCoordUniforms; // 0x448 c64
    f32 m_VertexIndex[4];                          // 0x658 the index vector of the next character
    f32 (*m_pVertexIndexData)[4];                  // 0x668
    f32 (*m_pMtxData)[4];                          // 0x66C
    f32 (*m_pTexCoordData)[4];                     // 0x670
    f32 (*m_pColorData)[4];                        // 0x674
    void* m_pCommandBuffer;                        // 0x678
    void* m_pVertexBuffer;                         // 0x67C
    u32 m_VertexBufferAddress;                     // 0x680 (physical, 16 byte aligned)
    u32* m_pFirstSubroutineEnd;                    // 0x684 the first string subroutine (CharWriter::UseCommandBuffer)
    u32* m_pLastSubroutineEnd;                     // 0x688
    u32* m_pSubroutineSizeAddress;                 // 0x68C the size of the way back of the last one
    bool m_IsColorUniformsSet;                     // 0x690
};
ASSERT_OFFSET(RectDrawer, m_ProjectionUniforms, 0x28);
ASSERT_OFFSET(RectDrawer, m_VertexIndexUniforms, 0x88);
ASSERT_OFFSET(RectDrawer, m_MtxUniforms, 0x238);
ASSERT_OFFSET(RectDrawer, m_ColorUniforms, 0x278);
ASSERT_OFFSET(RectDrawer, m_TexCoordUniforms, 0x448);
ASSERT_OFFSET(RectDrawer, m_VertexIndex, 0x658);
ASSERT_OFFSET(RectDrawer, m_pCommandBuffer, 0x678);
ASSERT_OFFSET(RectDrawer, m_pFirstSubroutineEnd, 0x684);
ASSERT_SIZE(RectDrawer, 0x694);
} // namespace font
} // namespace nw
