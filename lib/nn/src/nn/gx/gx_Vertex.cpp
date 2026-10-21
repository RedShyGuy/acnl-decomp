#include "nn/gx/gx_Api.h"
#include "nn/gx/detail/gx_Inline.h"

// The vertex attributes (glVertexAttrib*, glVertexAttribPointer, glDisableVertexAttribArray) and the
// uniform functions, which pass their values on to the uniform setters of the program in use.

using namespace nn::gx::detail;

namespace {
// the dirty flags of the vertex attribute arrays / of the constant attribute values
const u32 DIRTY_VERTEX_ARRAYS = 0x40;
const u32 DIRTY_VERTEX_VALUES = 0x80;

inline u32 BitsOf(f32 value)
{
    union {
        f32 f;
        u32 u;
    } bits;
    bits.f = value;
    return bits.u;
}

} // namespace

namespace nn {
namespace gx {
namespace detail {
// 0x007A101C (name is ours)
void UpdateAttributeBuffer(GLuint index)
{
    BufferTables* pTables = s_BufferManager.m_pTables;
    pTables->m_pAttributeBuffers[index] = pTables->m_pArrayBuffer;
}

// the constant value of an attribute (and its packed form for the GPU)
// 0x007C6618 (name is ours)
void SetVertexAttributeValue(GLuint index, const GLfloat* pValues)
{
    GlState* pState = s_pGlState;
    pState->m_AttributeValues.m_Values[index][0] = pValues[0];
    pState->m_AttributeValues.m_Values[index][1] = pValues[1];
    pState->m_AttributeValues.m_Values[index][2] = pValues[2];
    pState->m_AttributeValues.m_Values[index][3] = pValues[3];
    PackFloat24(pState->m_AttributeValues.m_PackedValues[index], pValues);
    pState->m_DirtyFlags |= DIRTY_VERTEX_VALUES;
}
} // namespace detail
} // namespace gx
} // namespace nn

// 0x007A97D4 | nintendogs:bytes [tier B]
extern "C" void glDisableVertexAttribArray(GLuint index)
{
    GlState* pState = s_pGlState;
    if (pState->m_Attributes[index].m_IsEnabled) {
        pState->m_Attributes[index].m_IsEnabled = false;
        pState->m_DirtyFlags |= DIRTY_VERTEX_ARRAYS;
    }
}

// 0x007AE45C | nintendogs:bytes [tier B]
extern "C" void glVertexAttrib1f(GLuint index, GLfloat x)
{
    GLfloat values[4] = {x, 0.0f, 0.0f, 1.0f};
    SetVertexAttributeValue(index, values);
}

// (the attribute reads the array buffer bound now)
// 0x007AE490 (name after the OpenGL ES API)
extern "C" void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pPointer)
{
    GlState* pState = s_pGlState;
    VertexAttribute* pAttribute = &pState->m_Attributes[index];
    if (pAttribute->m_Size != size) {
        pAttribute->m_Size = size;
        pState->m_DirtyFlags |= DIRTY_VERTEX_ARRAYS;
    }
    if (pAttribute->m_Type != type) {
        pAttribute->m_Type = type;
        pState->m_DirtyFlags |= DIRTY_VERTEX_ARRAYS;
    }
    if (pAttribute->m_IsNormalized != normalized) {
        pAttribute->m_IsNormalized = normalized;
        pState->m_DirtyFlags |= DIRTY_VERTEX_ARRAYS;
    }
    if (pAttribute->m_Stride != stride) {
        pAttribute->m_Stride = stride;
        pState->m_DirtyFlags |= DIRTY_VERTEX_ARRAYS;
    }
    if (pAttribute->m_pPointer != pPointer) {
        pAttribute->m_pPointer = pPointer;
        pState->m_DirtyFlags |= DIRTY_VERTEX_ARRAYS;
    }
    if (pAttribute->m_BufferName != pState->m_ArrayBufferName) {
        pAttribute->m_BufferName = pState->m_ArrayBufferName;
        pState->m_DirtyFlags |= DIRTY_VERTEX_ARRAYS;
    }
    UpdateAttributeBuffer(index);
}

// 0x007ADE1C (name after the OpenGL ES API)
extern "C" void glUniform1f(GLint location, GLfloat x)
{
    SetUniformFloat(location, &x, 1, 1, false, false);
}

// 0x007ADE54 | nintendogs:bytes [tier A]
extern "C" void glUniform1i(GLint location, GLint x)
{
    SetUniformInt(location, &x, 1, 1);
}

// 0x007ADE70 (name after the OpenGL ES API)
extern "C" void glUniform2i(GLint location, GLint x, GLint y)
{
    GLint values[2] = {x, y};
    SetUniformInt(location, values, 2, 1);
}

// 0x007ADE94 (name after the OpenGL ES API)
extern "C" void glUniform4f(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    GLfloat values[4] = {x, y, z, w};
    SetUniformFloat(location, values, 4, 1, false, false);
}

// 0x007ADED8 | nintendogs:bytes [tier B]
extern "C" void glUniform4fv(GLint location, GLsizei count, const GLfloat* pValues)
{
    SetUniformFloat(location, pValues, 4, count, false, false);
}

// (the setter takes the rows of the GPU: an untransposed matrix is transposed)
// 0x007ADF08 (name after the OpenGL ES API)
extern "C" void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* pValues)
{
    SetUniformFloat(location, pValues, 4, count, transpose == 0, true);
}

// 0x007AA200 (name after the OpenGL ES API)
extern "C" void glEnableVertexAttribArray(GLuint index)
{
    GlState* pState = s_pGlState;
    VertexAttribute* pAttribute = &pState->m_Attributes[index];
    if (!pAttribute->m_IsEnabled) {
        pAttribute->m_IsEnabled = true;
        pState->m_DirtyFlags |= DIRTY_VERTEX_ARRAYS;
    }
}
