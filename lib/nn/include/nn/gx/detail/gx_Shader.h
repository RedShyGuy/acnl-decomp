#pragma once

// The shader manager of the GL functions (__shm_initializeShaderManager): programs and shaders in
// hash tables of 512 lists sorted by name, the shader binaries in a list. The layouts come from
// the accesses of the gl* functions; all names are ours.

#include "decomp.h"
#include "nn/gx/gx_Types.h"

namespace nn {
namespace gx {
namespace detail {
// the hash tables: the lower 9 bits of the name
const u32 SHADER_BUCKET_COUNT = 512;
// the registers the shader validator keeps (their numbers in s_ShaderRegisters)
const u32 SHADER_REGISTER_COUNT = 189;

// the headers of a shader binary (SHBIN after 3dbrew): DVLB (the executables), DVLP (the code and
// the operand descriptors), DVLE (an executable)
struct DvlbHeader
{
    u32 m_Magic;            // 0x0 ("DVLB")
    u32 m_DvleCount;        // 0x4
    u32 m_DvleOffsets[1];   // 0x8 (from the start of the DVLB)
};

struct DvlpHeader
{
    u32 m_Magic;                    // 0x00 ("DVLP")
    u32 m_Version;                  // 0x04
    u32 m_CodeOffset;               // 0x08
    u32 m_CodeSize;                 // 0x0C (words)
    u32 m_OperandDescriptorOffset;  // 0x10
    u32 m_OperandDescriptorCount;   // 0x14 (two words each)
};

struct DvleHeader
{
    u32 m_Magic;                 // 0x00 ("DVLE")
    u16 m_Version;               // 0x04
    u8 m_ShaderType;             // 0x06 (0 vertex, 1 geometry)
    u8 m_MergeOutmaps;           // 0x07
    u32 m_MainOffset;            // 0x08
    u32 m_EndOffset;             // 0x0C
    u16 m_InputMask;             // 0x10
    u16 m_OutputMask;            // 0x12
    u8 m_GeometryType;           // 0x14
    u8 m_StartFloatRegister;     // 0x15
    u8 m_FullVertexCount;        // 0x16
    u8 m_VariableVertexCount;    // 0x17
    u32 m_ConstantOffset;        // 0x18 (entries of 20 bytes)
    u32 m_ConstantCount;         // 0x1C
    u32 m_LabelOffset;           // 0x20
    u32 m_LabelCount;            // 0x24
    u32 m_OutputOffset;          // 0x28 (entries of 8 bytes)
    u32 m_OutputCount;           // 0x2C
    u32 m_UniformOffset;         // 0x30 (entries of 8 bytes)
    u32 m_UniformCount;          // 0x34
    u32 m_SymbolOffset;          // 0x38
    u32 m_SymbolSize;            // 0x3C
};

// a constant of a DVLE (type 0 bool, 1 integer, 2 float24 vector)
struct DvleConstant
{
    u16 m_Type;      // 0x0
    u16 m_Id;        // 0x2
    u32 m_Values[4]; // 0x4
};

// an output register of a DVLE (the output types after 3dbrew; 9 is a dummy output)
struct DvleOutput
{
    u16 m_Type;      // 0x0
    u16 m_Register;  // 0x2
    u16 m_Mask;      // 0x4
    u16 _unknown0x6;
};

// a uniform of a DVLE (registers 0x00-0x0F inputs, 0x10-0x6F floats, 0x70-0x77 integers, 0x78-0x87
// bools)
struct DvleUniform
{
    u32 m_SymbolOffset;   // 0x0
    u16 m_StartRegister;  // 0x4
    u16 m_EndRegister;    // 0x6
};

// a uniform of an executable (20 bytes)
struct ShaderUniform
{
    GLenum m_Type;         // 0x00
    u32 m_Register;        // 0x04 (from the first register of its kind)
    u32 m_Component;       // 0x08 (the first component of a swizzle)
    u32 m_NameOffset;      // 0x0C (in the symbols)
    s32 m_RegisterCount;   // 0x10
};

// an input of an executable
struct ShaderInput
{
    GLenum m_Type;         // 0x0
    u32 m_NameOffset;      // 0x4 (in the symbols)
};

// an executable of a shader binary (0xE8 bytes)
struct ShaderBinaryEntry
{
    u8 m_ShaderType;              // 0x00
    u8 m_MergeOutmaps;            // 0x01
    u8 m_GeometryType;            // 0x02
    u8 m_StartFloatRegister;      // 0x03
    u8 m_FullVertexCount;         // 0x04
    u8 m_VariableVertexCount;     // 0x05
    u16 m_InputMask;              // 0x06
    u16 m_OutputMask;             // 0x08
    u8 _unknown0x0A[2];
    s32 m_InputCount;             // 0x0C
    s32 m_OutputCount;            // 0x10
    u32 m_MainOffset;             // 0x14
    u32 m_EndOffset;              // 0x18
    u32 m_BoolConstants;          // 0x1C (one bit per bool register)
    u32 m_IntConstants[4];        // 0x20
    u32* m_pFloatConstants;       // 0x30 (register, three words of float24)
    s32 m_FloatConstantCount;     // 0x34
    u32 m_Outmaps[7];             // 0x38 (the semantics of the output registers)
    u32 m_OutmapMask;             // 0x54 (the outputs the rasterizer needs)
    ShaderUniform* m_pUniforms;   // 0x58
    s32 m_UniformCount;           // 0x5C
    ShaderInput m_Inputs[16];     // 0x60
    char* m_pSymbols;             // 0xE0
    u32 m_SymbolSize;             // 0xE4
};
ASSERT_OFFSET(ShaderBinaryEntry, m_pFloatConstants, 0x30);
ASSERT_OFFSET(ShaderBinaryEntry, m_pUniforms, 0x58);
ASSERT_OFFSET(ShaderBinaryEntry, m_pSymbols, 0xE0);
ASSERT_SIZE(ShaderBinaryEntry, 0xE8);

// a shader binary of glShaderBinary (shared by its shaders)
struct ShaderBinary
{
    u32* m_pCode;                    // 0x00
    u32 m_CodeSize;                  // 0x04 (words)
    u32* m_pOperandDescriptors;      // 0x08
    u32 m_OperandDescriptorCount;    // 0x0C
    ShaderBinaryEntry* m_pEntries;   // 0x10
    u32 m_EntryCount;                // 0x14
    s32 m_ReferenceCount;            // 0x18
    ShaderBinary* m_pPrevious;       // 0x1C
    ShaderBinary* m_pNext;           // 0x20
};
ASSERT_SIZE(ShaderBinary, 0x24);

// a shader (glCreateShader)
struct Shader
{
    ShaderBinary* m_pBinary;         // 0x00
    u32 m_EntryIndex;                // 0x04 (the executable of the binary)
    GLuint m_Name;                   // 0x08
    GLenum m_Type;                   // 0x0C (0x8B31 vertex, 0x6001 geometry)
    s32 m_AttachCount;               // 0x10
    bool m_IsDeletePending;          // 0x14 (glDeleteShader while attached)
    u8 _unknown0x15[3];
    Shader* m_pNext;                 // 0x18
};
ASSERT_SIZE(Shader, 0x1C);

// a node of a list of a program: an attribute binding of glBindAttribLocation (its name is freed
// with it)
struct ProgramListNode
{
    char* m_pName;                   // 0x0
    GLuint m_Index;                  // 0x4
    ProgramListNode* m_pNext;        // 0x8
};

// a uniform location of a linked program (location: program name << 19 | uniform << 7 | kind; info:
// components - 1 << 13, matrix 0x8000, integer 0x4000 / bool 0x10000, geometry 0x400, swizzle
// << 11, registers << 17, the slot of its first register << 24; the reserved uniforms 0x40000 |
// index << 2 | components - 1)
struct UniformLocation
{
    GLint m_Location;     // 0x0
    u32 m_NameOffset;     // 0x4 (the reserved uniforms: their index)
    u32 m_Info;           // 0x8
};
ASSERT_SIZE(UniformLocation, 0xC);

// an input bound to a vertex attribute
struct AttributeBinding
{
    GLenum m_Type;        // 0x0
    s32 m_Register;       // 0x4 (-1: none)
    u32 m_NameOffset;     // 0x8
};
ASSERT_SIZE(AttributeBinding, 0xC);

// the float registers a program uses
const u32 FLOAT_REGISTER_COUNT = 96;

// a program (glCreateProgram; 0xF1C bytes)
struct Program
{
    Program* m_pNext;                // 0x000
    GLuint m_Name;                   // 0x004
    GLint m_Unknown0x08;             // 0x008 (glAttachShader / glDetachShader with the shader -1)
    Shader* m_pVertexShader;         // 0x00C
    Shader* m_pGeometryShader;       // 0x010
    bool m_IsDirty;                  // 0x014 (a shader was attached or detached)
    bool m_IsDeletePending;          // 0x015 (glDeleteProgram while in use)
    bool m_IsLinked;                 // 0x016
    bool m_WasDirty;                 // 0x017 (the dirty flag of the last link)
    ProgramListNode* m_pList;        // 0x018 (glBindAttribLocation)
    UniformLocation* m_pLocations;   // 0x01C
    u32 m_LocationCount;             // 0x020 (with the reserved uniforms)
    u32 m_VertexUniformCount;        // 0x024
    u32 m_UniformCount;              // 0x028
    u32* m_pVertexFloatValues;       // 0x02C (four words per register)
    u32 m_VertexFloatRegisters[FLOAT_REGISTER_COUNT]; // 0x030
    u32 m_VertexFloatRegisterCount;  // 0x1B0
    s32 m_Unknown0x1B4[3];           // 0x1B4
    u32* m_pGeometryFloatValues;     // 0x1C0
    u32 m_GeometryFloatRegisters[FLOAT_REGISTER_COUNT]; // 0x1C4
    u32 m_GeometryFloatRegisterCount; // 0x344
    s32 m_Unknown0x348[3];           // 0x348
    AttributeBinding m_Bindings[16]; // 0x354
    ShaderBinary* m_pLinkedBinary;   // 0x414
    u32 m_VertexEntryIndex;          // 0x418
    u32 m_GeometryEntryIndex;        // 0x41C
    u32 m_OutmapMask;                // 0x420
    bool m_HasGeometryShader;        // 0x424
    u8 _unknown0x425;
    u8 m_RegisterMasks[SHADER_REGISTER_COUNT];   // 0x426
    u8 _unknown0x4E3;
    u32 m_RegisterValues[SHADER_REGISTER_COUNT]; // 0x4E4
    u32 m_DirtyRegisters[6];         // 0x7D8 (one bit per register)
    u8 _unknown0x7F0[0x9EC - 0x7F0];
    u8 m_ReservedValues[0xE2C - 0x9EC]; // 0x9EC (the values of the reserved uniforms)
    GLenum m_SamplerTypes[3];        // 0xE2C
    GLenum m_Mode;                   // 0xE38 (0x6051: gas, see __shv_preDraw; 0x6030 at the start)
    u8 m_ReservedValues2[0xE74 - 0xE3C]; // 0xE3C
    bool m_Unknown0xE74;             // 0xE74 (__shv_preDraw marks the gas accumulation)
    u8 m_ReservedValues3[0xF1C - 0xE75]; // 0xE75
};
ASSERT_OFFSET(Program, m_VertexFloatRegisterCount, 0x1B0);
ASSERT_OFFSET(Program, m_GeometryFloatRegisterCount, 0x344);
ASSERT_OFFSET(Program, m_Bindings, 0x354);
ASSERT_OFFSET(Program, m_RegisterMasks, 0x426);
ASSERT_OFFSET(Program, m_RegisterValues, 0x4E4);
ASSERT_OFFSET(Program, m_DirtyRegisters, 0x7D8);
ASSERT_OFFSET(Program, m_Mode, 0xE38);
ASSERT_SIZE(Program, 0xF1C);

// the reserved uniforms of the fixed function parts
const u32 RESERVED_UNIFORM_COUNT = 297;
// the words of a set of shader registers (one bit per register)
const u32 REGISTER_SET_WORDS = 6;

// a reserved uniform (12 bytes)
struct UniformInfo
{
    s32 m_Count;          // 0x0 (the elements of an array)
    GLenum m_Type;        // 0x4 (the OpenGL ES type)
    const char* m_pName;  // 0x8
};
ASSERT_SIZE(UniformInfo, 0xC);

// the tables of the shader manager (in the memory of the GL state; 0x1774 bytes)
struct ShaderTables
{
    Program* m_pCurrentProgram;                     // 0x0000 (glUseProgram)
    u32 _unknown0x0004;
    Program* m_pPrograms[SHADER_BUCKET_COUNT];      // 0x0008
    Shader* m_pShaders[SHADER_BUCKET_COUNT];        // 0x0808
    ShaderBinary* m_pBinaries;                      // 0x1008
    u32 m_DefaultRegisterValues[SHADER_REGISTER_COUNT]; // 0x100C (the values at the start)
    u32 m_RegisterValues[SHADER_REGISTER_COUNT];    // 0x1300
    u8 m_DefaultRegisterMasks[SHADER_REGISTER_COUNT];   // 0x15F4
    u8 m_RegisterMasks[SHADER_REGISTER_COUNT];      // 0x16B1 (the byte masks of the commands)
    u8 _unknown0x176E[2];
    GLenum m_LastMode;                              // 0x1770 (the mode of the program of the last draw)
};
ASSERT_OFFSET(ShaderTables, m_pShaders, 0x808);
ASSERT_OFFSET(ShaderTables, m_pBinaries, 0x1008);
ASSERT_OFFSET(ShaderTables, m_RegisterValues, 0x1300);
ASSERT_OFFSET(ShaderTables, m_RegisterMasks, 0x16B1);
ASSERT_SIZE(ShaderTables, 0x1774);

// the shader manager
struct ShaderManager
{
    GLuint m_NextProgramName;        // 0x0
    GLuint m_NextShaderName;         // 0x4
    ShaderTables* m_pTables;         // 0x8
    s32 m_Unknown0xC;                // 0xC (-1 at the start)
};

extern ShaderManager s_ShaderManager;
extern UniformInfo s_ReservedUniforms[RESERVED_UNIFORM_COUNT];
extern u32 s_ShaderRegisters[SHADER_REGISTER_COUNT];
// the lists of shader registers of the parts of the validation (up to SHADER_REGISTER_COUNT) and their
// sets (the bits of lists 1 and 2 are in one set)
extern s32 s_RegisterList0[24];
extern s32 s_RegisterList1[6];
extern s32 s_RegisterList2[6];
extern s32 s_RegisterList3[157];
extern u32 s_RegisterSet0[REGISTER_SET_WORDS];
extern u32 s_RegisterSet1[REGISTER_SET_WORDS];
extern u32 s_RegisterSet2[REGISTER_SET_WORDS];

// a program is destroyed (its shaders detached, its memory freed)
void DestroyProgram(GLuint name); // 0x007B95F0 (name is ours)
// a shader binary without shaders is freed
void ReleaseShaderBinary(ShaderBinary* pBinary); // 0x007B943C (name is ours)
} // namespace detail
} // namespace gx
} // namespace nn
