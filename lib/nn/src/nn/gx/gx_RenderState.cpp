#include "nn/gx/gx_Api.h"
#include "nn/gx/detail/gx_Inline.h"

// The render state functions of the GL (glEnable / glDisable / glActiveTexture): the flags of the GL
// state; some capabilities write their GPU registers right away (the registers after 3dbrew).

using namespace nn::gx::detail;

namespace {
// the capabilities (the OpenGL ES values; 0x6502, 0x6780 and 0x67A0 are extensions, names ours)
const GLenum CAP_CULL_FACE = 0x0B44;
const GLenum CAP_DEPTH_TEST = 0x0B71;
const GLenum CAP_STENCIL_TEST = 0x0B90;
const GLenum CAP_BLEND = 0x0BE2;
const GLenum CAP_COLOR_LOGIC_OP = 0x0BF2;
const GLenum CAP_SCISSOR_TEST = 0x0C11;
const GLenum CAP_UNKNOWN_0x6502 = 0x6502;
const GLenum CAP_EARLY_DEPTH_TEST = 0x6780;
const GLenum CAP_COPY_FROM_DEPTH = 0x67A0;
const GLenum CAP_POLYGON_OFFSET_FILL = 0x8037;
// the faces (the OpenGL ES values)
const GLenum FACE_CCW = 0x0901;
const GLenum FACE_FRONT = 0x0404;
const GLenum TEXTURE0 = 0x84C0;
// the dirty flags of the framebuffer and the scissor and of the polygon offset
const u32 DIRTY_FRAMEBUFFER_STATE = 0x100;
const u32 DIRTY_SCISSOR = 0x200;
const u32 DIRTY_POLYGON_OFFSET = 0x4;
// the commands (register | mask << 16)
const u32 COMMAND_FACE_CULLING = 0xF0040;
const u32 COMMAND_EARLY_DEPTH_TEST_1 = 0x10062;
const u32 COMMAND_COLOR_OPERATION_MODE = 0x20100;  // (byte 1: 0 logic op, 1 blend)
const u32 COMMAND_BLEND_FUNCTION = 0xF0101;
const u32 COMMAND_STENCIL_TEST = 0x10105;
const u32 COMMAND_DEPTH_COLOR_MASK = 0x10107;
const u32 COMMAND_EARLY_DEPTH_TEST_2 = 0x10118;
// the blend function that leaves the color as it is
const u32 BLEND_FUNCTION_NONE = 0x1010000;
const u32 COLOR_OPERATION_BLEND = 0x100;

// a flag of the GL state is the same (the commands are written again while their generation is
// deferred)
inline bool IsUnchanged(GlState* pState, bool isEnabled, bool value)
{
    return isEnabled == value && !pState->m_IsCommandGenerationDeferred;
}

// the blend function and the color operation mode of blending
inline void PutBlendCommands(bool isBlendEnabled)
{
    PutCommand(isBlendEnabled ? s_BlendFunctionRegister : BLEND_FUNCTION_NONE, COMMAND_BLEND_FUNCTION);
    PutCommand(COLOR_OPERATION_BLEND, COMMAND_COLOR_OPERATION_MODE);
}

// a capability on or off
// 0x007A6448 (name is ours)
DECOMP_NOINLINE void SetCapability(GLenum cap, bool isEnabled)
{
    GlState* pState = s_pGlState;
    switch (cap) {
    case CAP_SCISSOR_TEST:
        if (pState->m_IsScissorTestEnabled == isEnabled) {
            return;
        }
        pState->m_IsScissorTestEnabled = isEnabled;
        pState->m_DirtyFlags |= DIRTY_SCISSOR;
        break;
    case CAP_STENCIL_TEST:
        if (IsUnchanged(pState, pState->m_IsStencilTestEnabled, isEnabled)) {
            return;
        }
        if (isEnabled) {
            s_StencilTestRegister |= 1;
        } else {
            s_StencilTestRegister &= ~1;
        }
        PutCommand(s_StencilTestRegister, COMMAND_STENCIL_TEST);
        pState->m_IsStencilTestEnabled = isEnabled;
        pState->m_DirtyFlags |= DIRTY_FRAMEBUFFER_STATE;
        break;
    case CAP_CULL_FACE:
        if (IsUnchanged(pState, pState->m_IsCullFaceEnabled, isEnabled)) {
            return;
        }
        if (isEnabled) {
            bool isFront = pState->m_CullFaceMode == FACE_FRONT;
            bool isCcw = pState->m_FrontFace == FACE_CCW;
            PutCommand(isFront != isCcw ? 2 : 1, COMMAND_FACE_CULLING);
        } else {
            PutCommand(0, COMMAND_FACE_CULLING);
        }
        pState->m_IsCullFaceEnabled = isEnabled;
        break;
    case CAP_DEPTH_TEST:
        if (IsUnchanged(pState, pState->m_IsDepthTestEnabled, isEnabled)) {
            return;
        }
        if (isEnabled) {
            s_DepthColorMaskRegister |= 1;
        } else {
            s_DepthColorMaskRegister &= ~1;
        }
        PutCommand(s_DepthColorMaskRegister, COMMAND_DEPTH_COLOR_MASK);
        pState->m_IsDepthTestEnabled = isEnabled;
        pState->m_DirtyFlags |= DIRTY_FRAMEBUFFER_STATE;
        break;
    case CAP_BLEND:
        if (IsUnchanged(pState, pState->m_IsBlendEnabled, isEnabled)) {
            return;
        }
        if (pState->m_IsLogicOpEnabled) {
            PutCommand(0, COMMAND_COLOR_OPERATION_MODE);
        } else {
            PutBlendCommands(isEnabled);
        }
        pState->m_IsBlendEnabled = isEnabled;
        pState->m_DirtyFlags |= DIRTY_FRAMEBUFFER_STATE;
        break;
    case CAP_COLOR_LOGIC_OP:
        if (IsUnchanged(pState, pState->m_IsLogicOpEnabled, isEnabled)) {
            return;
        }
        if (isEnabled) {
            PutCommand(0, COMMAND_COLOR_OPERATION_MODE);
        } else {
            PutBlendCommands(pState->m_IsBlendEnabled);
        }
        pState->m_IsLogicOpEnabled = isEnabled;
        pState->m_DirtyFlags |= DIRTY_FRAMEBUFFER_STATE;
        break;
    case CAP_UNKNOWN_0x6502:
        pState->m_Unknown0x1A = isEnabled;
        break;
    case CAP_EARLY_DEPTH_TEST:
        if (IsUnchanged(pState, pState->m_IsEarlyDepthTestEnabled, isEnabled)) {
            return;
        }
        PutCommand(isEnabled ? 1 : 0, COMMAND_EARLY_DEPTH_TEST_1);
        PutCommand(isEnabled ? 1 : 0, COMMAND_EARLY_DEPTH_TEST_2);
        pState->m_IsEarlyDepthTestEnabled = isEnabled;
        pState->m_DirtyFlags |= DIRTY_FRAMEBUFFER_STATE;
        break;
    case CAP_COPY_FROM_DEPTH:
        pState->m_IsCopyFromDepth = isEnabled;
        break;
    case CAP_POLYGON_OFFSET_FILL:
        if (pState->m_IsPolygonOffsetFillEnabled == isEnabled) {
            return;
        }
        pState->m_IsPolygonOffsetFillEnabled = isEnabled;
        pState->m_DirtyFlags |= DIRTY_POLYGON_OFFSET;
        break;
    default:
        break;
    }
}
} // namespace

// (falls into SetCapability)
// 0x007A6440 | nintendogs:bytes [tier B]
extern "C" void glDisable(GLenum cap)
{
    SetCapability(cap, false);
}

// 0x007AA1F8 (name after the OpenGL ES API)
extern "C" void glEnable(GLenum cap)
{
    SetCapability(cap, true);
}

// 0x007A7750 (name after the OpenGL ES API)
extern "C" void glActiveTexture(GLenum texture)
{
    s_pGlState->m_ActiveTextureUnit = texture - TEXTURE0;
}

namespace {
// the commands of the viewport (register | mask << 16)
const u32 COMMAND_VIEWPORT_WIDTH = 0xF0041;
const u32 COMMAND_VIEWPORT_INVERSE_WIDTH = 0xF0042;
const u32 COMMAND_VIEWPORT_HEIGHT = 0xF0043;
const u32 COMMAND_VIEWPORT_XY = 0xF0068;
const u32 COMMAND_EARLY_DEPTH_FUNCTION = 0x10061;
const u32 COMMAND_EARLY_DEPTH_DATA = 0x7006A;
const u32 COMMAND_FRAMEBUFFER_BLOCK32 = 0x1011B;
const u32 COMMAND_DEPTH_COLOR_MASK_ALL = 0x30107;
const u32 COMMAND_LOGIC_OPERATION = 0xF0102;
const u32 COMMAND_BLEND_COLOR = 0xF0103;
const u32 COMMAND_STENCIL_TEST_ALL = 0xF0105;
const u32 COMMAND_STENCIL_OPERATION = 0xF0106;
const u32 COMMAND_GAS_DELTAZ_DEPTH = 0x80126;

// a float as the 31 bit float of the GPU (1.7.23)
inline void ToFloat31(u32* pResult, f32 value)
{
    union {
        f32 f;
        u32 u;
    } pun;
    pun.f = value;
    u32 bits = pun.u;
    s32 exponent = 0;
    if ((bits & 0x7FFFFFFF) != 0) {
        exponent = static_cast<s32>((bits << 1) >> 24) - 64;
    }
    u32 mantissa = (bits << 9) >> 9;
    u32 sign = bits >> 31;
    if (exponent >= 0) {
        *pResult = mantissa | (exponent << 23) | (sign << 30);
    } else {
        *pResult = sign << 30;
    }
}
// the viewport: half the size as 24 bit floats, 2 / size as 31 bit floats (the inverses of the usual
// screen sizes are constants), the position
DECOMP_ALWAYS_INLINE void PutViewportCommands(GLint x, GLint y, GLsizei width, GLsizei height)
{
    PutCommand(Float32ToFloat24(static_cast<f32>(width) * 0.5f), COMMAND_VIEWPORT_WIDTH);
    PutCommand(Float32ToFloat24(static_cast<f32>(height) * 0.5f), COMMAND_VIEWPORT_HEIGHT);
    if (width != 0 && height != 0) {
        u32 inverses[2] = {static_cast<u32>(width), static_cast<u32>(height)};
        for (s32 i = 0; i < 2; i++) {
            switch (static_cast<s32>(inverses[i])) {
            case 240:
                inverses[i] = 0x38111111;
                break;
            case 320:
                inverses[i] = 0x37999999;
                break;
            case 400:
                inverses[i] = 0x3747AE14;
                break;
            case 480:
                inverses[i] = 0x37111111;
                break;
            case 600:
                inverses[i] = 0x36B4E81B;
                break;
            case 640:
                inverses[i] = 0x36999999;
                break;
            case 800:
                inverses[i] = 0x3647AE14;
                break;
            case 1024:
                inverses[i] = 0x36000000;
                break;
            case 1200:
                inverses[i] = 0x35B4E81B;
                break;
            case 1280:
                inverses[i] = 0x35999999;
                break;
            default:
                ToFloat31(&inverses[i], 2.0f / static_cast<f32>(inverses[i]));
                inverses[i] <<= 1;
                break;
            }
            PutCommand(inverses[i], COMMAND_VIEWPORT_INVERSE_WIDTH + i * 2);
        }
    }
    PutCommand(x | (y << 16), COMMAND_VIEWPORT_XY);
}
} // namespace

namespace nn {
namespace gx {
namespace detail {
// 0x007C6768 (name is ours)
void ValidateViewport()
{
    GlState* pState = s_pGlState;
    PutViewportCommands(pState->m_ViewportX, pState->m_ViewportY, pState->m_ViewportWidth, pState->m_ViewportHeight);
    PutCommand(s_DepthColorMaskRegister, COMMAND_DEPTH_COLOR_MASK_ALL);
    PutCommand(s_GasDeltaZDepthRegister, COMMAND_GAS_DELTAZ_DEPTH);
    if (pState->m_IsEarlyDepthTestEnabled) {
        PutCommand(1, COMMAND_EARLY_DEPTH_TEST_1);
        PutCommand(1, COMMAND_EARLY_DEPTH_TEST_2);
    } else {
        PutCommand(0, COMMAND_EARLY_DEPTH_TEST_1);
        PutCommand(0, COMMAND_EARLY_DEPTH_TEST_2);
    }
    PutCommand(s_EarlyDepthFunctionRegister, COMMAND_EARLY_DEPTH_FUNCTION);
    PutCommand(s_EarlyDepthDataRegister, COMMAND_EARLY_DEPTH_DATA);
    PutCommand(s_LogicOperationRegister, COMMAND_LOGIC_OPERATION);
    if (pState->m_IsLogicOpEnabled) {
        PutCommand(0, COMMAND_COLOR_OPERATION_MODE);
    } else {
        PutBlendCommands(pState->m_IsBlendEnabled);
    }
    if (pState->m_IsCullFaceEnabled) {
        bool isFront = pState->m_CullFaceMode == FACE_FRONT;
        bool isCcw = pState->m_FrontFace == FACE_CCW;
        PutCommand(isFront != isCcw ? 2 : 1, COMMAND_FACE_CULLING);
    } else {
        PutCommand(0, COMMAND_FACE_CULLING);
    }
    PutCommand(pState->m_IsBlock32 ? 1 : 0, COMMAND_FRAMEBUFFER_BLOCK32);
    PutCommand(s_StencilTestRegister, COMMAND_STENCIL_TEST_ALL);
    PutCommand(s_StencilOperationRegister, COMMAND_STENCIL_OPERATION);
    PutCommand(s_BlendColorRegister, COMMAND_BLEND_COLOR);
    pState->m_DirtyFlags |= DIRTY_FRAMEBUFFER_STATE;
}
} // namespace detail
} // namespace gx
} // namespace nn

namespace {
// the blend equations and factors (the OpenGL ES values) and their values in the blend function
// register (3dbrew)
const GLenum EQUATION_ADD = 0x8006;
const GLenum EQUATION_MIN = 0x8007;
const GLenum EQUATION_MAX = 0x8008;
const GLenum EQUATION_SUBTRACT = 0x800A;
const GLenum EQUATION_REVERSE_SUBTRACT = 0x800B;
const u32 COMMAND_BLEND_EQUATION = 0x30101;
const u32 COMMAND_BLEND_FACTORS = 0xC0101;

// false for an equation the GPU does not have
inline bool ConvertEquation(GLenum mode, u32* pValue)
{
    switch (mode) {
    case EQUATION_ADD:
        *pValue = 0;
        return true;
    case EQUATION_SUBTRACT:
        *pValue = 1;
        return true;
    case EQUATION_REVERSE_SUBTRACT:
        *pValue = 2;
        return true;
    case EQUATION_MIN:
        *pValue = 3;
        return true;
    case EQUATION_MAX:
        *pValue = 4;
        return true;
    default:
        return false;
    }
}

// false for a factor the GPU does not have
inline bool ConvertFactor(GLenum factor, u32* pValue)
{
    switch (factor) {
    case 0x0000: // GL_ZERO
        *pValue = 0;
        return true;
    case 0x0001: // GL_ONE
        *pValue = 1;
        return true;
    case 0x0300: // GL_SRC_COLOR
        *pValue = 2;
        return true;
    case 0x0301: // GL_ONE_MINUS_SRC_COLOR
        *pValue = 3;
        return true;
    case 0x0306: // GL_DST_COLOR
        *pValue = 4;
        return true;
    case 0x0307: // GL_ONE_MINUS_DST_COLOR
        *pValue = 5;
        return true;
    case 0x0302: // GL_SRC_ALPHA
        *pValue = 6;
        return true;
    case 0x0303: // GL_ONE_MINUS_SRC_ALPHA
        *pValue = 7;
        return true;
    case 0x0304: // GL_DST_ALPHA
        *pValue = 8;
        return true;
    case 0x0305: // GL_ONE_MINUS_DST_ALPHA
        *pValue = 9;
        return true;
    case 0x8001: // GL_CONSTANT_COLOR
        *pValue = 10;
        return true;
    case 0x8002: // GL_ONE_MINUS_CONSTANT_COLOR
        *pValue = 11;
        return true;
    case 0x8003: // GL_CONSTANT_ALPHA
        *pValue = 12;
        return true;
    case 0x8004: // GL_ONE_MINUS_CONSTANT_ALPHA
        *pValue = 13;
        return true;
    case 0x0308: // GL_SRC_ALPHA_SATURATE
        *pValue = 14;
        return true;
    default:
        return false;
    }
}

// the blend function goes to the GPU while blending is on (while the generation of the commands
// is deferred, the function without blending)
inline void PutBlendFunction(GlState* pState, u32 header)
{
    if (pState->m_IsBlendEnabled && !pState->m_IsLogicOpEnabled) {
        PutCommand(s_BlendFunctionRegister, header);
    } else if (pState->m_IsCommandGenerationDeferred) {
        PutCommand(BLEND_FUNCTION_NONE, header);
    }
}
} // namespace

// (falls into glBlendEquationSeparate)
// 0x007A85F4 (name after the OpenGL ES API)
extern "C" void glBlendEquation(GLenum mode)
{
    glBlendEquationSeparate(mode, mode);
}

// 0x007A85FC (name after the OpenGL ES API)
extern "C" void glBlendEquationSeparate(GLenum modeRgb, GLenum modeAlpha)
{
    GlState* pState = s_pGlState;
    u32 rgb = 0;
    u32 alpha = 0;
    if (pState->m_BlendEquationRgb == modeRgb && pState->m_BlendEquationAlpha == modeAlpha && !pState->m_IsCommandGenerationDeferred) {
        return;
    }
    if (!ConvertEquation(modeRgb, &rgb) || !ConvertEquation(modeAlpha, &alpha)) {
        return;
    }
    s_BlendFunctionRegister = (s_BlendFunctionRegister & 0xFFFF0000) | rgb | (alpha << 8);
    PutBlendFunction(pState, COMMAND_BLEND_EQUATION);
    pState->m_BlendEquationRgb = modeRgb;
    pState->m_BlendEquationAlpha = modeAlpha;
}

// (falls into glBlendFuncSeparate)
// 0x007A8774 (name after the OpenGL ES API)
extern "C" void glBlendFunc(GLenum sourceFactor, GLenum destinationFactor)
{
    glBlendFuncSeparate(sourceFactor, destinationFactor, sourceFactor, destinationFactor);
}

// 0x007A8780 (name after the OpenGL ES API)
extern "C" void glBlendFuncSeparate(GLenum sourceRgb, GLenum destinationRgb, GLenum sourceAlpha, GLenum destinationAlpha)
{
    GlState* pState = s_pGlState;
    u32 sourceRgbValue = 0;
    u32 destinationRgbValue = 0;
    u32 sourceAlphaValue = 0;
    u32 destinationAlphaValue = 0;
    if (pState->m_BlendSourceRgb == sourceRgb && pState->m_BlendDestinationRgb == destinationRgb && pState->m_BlendSourceAlpha == sourceAlpha &&
        pState->m_BlendDestinationAlpha == destinationAlpha && !pState->m_IsCommandGenerationDeferred) {
        return;
    }
    if (!ConvertFactor(sourceRgb, &sourceRgbValue) || !ConvertFactor(destinationRgb, &destinationRgbValue) ||
        !ConvertFactor(sourceAlpha, &sourceAlphaValue) || !ConvertFactor(destinationAlpha, &destinationAlphaValue)) {
        return;
    }
    s_BlendFunctionRegister = (s_BlendFunctionRegister & 0xFFFF) | (sourceRgbValue << 16) | (destinationRgbValue << 20) | (sourceAlphaValue << 24) |
                              (destinationAlphaValue << 28);
    pState->m_BlendSourceRgb = sourceRgb;
    pState->m_BlendSourceAlpha = sourceAlpha;
    pState->m_BlendDestinationRgb = destinationRgb;
    pState->m_BlendDestinationAlpha = destinationAlpha;
    PutBlendFunction(pState, COMMAND_BLEND_FACTORS);
}

// 0x007AE564 | nintendogs:bytes [tier B]
extern "C" void glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
    GlState* pState = s_pGlState;
    PutViewportCommands(x, y, width, height);
    pState->m_ViewportX = x;
    pState->m_ViewportY = y;
    pState->m_ViewportWidth = width;
    pState->m_ViewportHeight = height;
    if (!pState->m_IsViewportSet) {
        pState->m_IsViewportSet = true;
    }
}

// 0x007AC2D4 (name after the OpenGL ES API)
extern "C" void glLogicOp(GLenum opcode)
{
    GlState* pState = s_pGlState;
    if (pState->m_LogicOperation == opcode && !pState->m_IsCommandGenerationDeferred) {
        return;
    }
    // the operations of the GPU (3dbrew)
    u32 value;
    switch (opcode) {
    case 0x1500: // GL_CLEAR
        value = 0;
        break;
    case 0x1501: // GL_AND
        value = 1;
        break;
    case 0x1502: // GL_AND_REVERSE
        value = 2;
        break;
    case 0x1503: // GL_COPY
        value = 3;
        break;
    case 0x150F: // GL_SET
        value = 4;
        break;
    case 0x150C: // GL_COPY_INVERTED
        value = 5;
        break;
    case 0x1505: // GL_NOOP
        value = 6;
        break;
    case 0x150A: // GL_INVERT
        value = 7;
        break;
    case 0x150E: // GL_NAND
        value = 8;
        break;
    case 0x1507: // GL_OR
        value = 9;
        break;
    case 0x1508: // GL_NOR
        value = 10;
        break;
    case 0x1506: // GL_XOR
        value = 11;
        break;
    case 0x1509: // GL_EQUIV
        value = 12;
        break;
    case 0x1504: // GL_AND_INVERTED
        value = 13;
        break;
    case 0x150B: // GL_OR_REVERSE
        value = 14;
        break;
    case 0x150D: // GL_OR_INVERTED
        value = 15;
        break;
    default:
        return;
    }
    PutCommand(value, COMMAND_LOGIC_OPERATION);
    s_LogicOperationRegister = value;
    pState->m_LogicOperation = opcode;
}
