#include "nn/gx/detail/gx_Shader.h"
#include "nn/gx/gx_Api.h"
#include <cstring>

// Programs and shaders of the GL functions (glCreate*/glDelete*/glAttachShader/glDetachShader) in
// the tables of the shader manager.

using namespace nn::gx::detail;

namespace nn {
namespace gx {
namespace detail {
// 0x0097E490 (name is ours)
ShaderManager s_ShaderManager;
// 0x0097E4A0 (name is ours)
s32 s_RegisterList0[24] = {0, 6, 7, 8, 14, 15, 16, 17, 18, 19, 20, 21, 22, 25, 26, 27, 28, 29, 30, 31, 32, 38, 40, 189};
// 0x0097E500 (name is ours)
s32 s_RegisterList1[6] = {9, 10, 11, 12, 13, 189};
// 0x0097E518 (name is ours)
s32 s_RegisterList2[6] = {1, 2, 3, 4, 5, 189};
// 0x0097E530 (name is ours)
s32 s_RegisterList3[157] = {
    23,  24,  33,  34,  35,  36,  37,  39,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,
    57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,  84,
    80,  81,  82,  83,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,  96,  97,  98,  99,  100, 101, 102, 103, 104,
    105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128,
    129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152,
    153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176,
    177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189};
// 0x00AE832C (name is ours)
UniformInfo s_ReservedUniforms[RESERVED_UNIFORM_COUNT];
// 0x00AE9118 (name is ours)
u32 s_ShaderRegisters[SHADER_REGISTER_COUNT];
// 0x00AE940C (name is ours)
u32 s_RegisterSet0[REGISTER_SET_WORDS];
// 0x00AE9424 (name is ours)
u32 s_RegisterSet1[REGISTER_SET_WORDS];
// 0x00AE943C (name is ours)
u32 s_RegisterSet2[REGISTER_SET_WORDS];
} // namespace detail
} // namespace gx
} // namespace nn

namespace {
// the shader types (the OpenGL ES value; the geometry shader of the CTR GPU)
const GLenum SHADER_TYPE_VERTEX = 0x8B31;
const GLenum SHADER_TYPE_GEOMETRY = 0x6001;

inline u32 GetBucket(GLuint name)
{
    return name % SHADER_BUCKET_COUNT;
}

inline Program* FindProgram(GLuint name)
{
    Program* p = s_ShaderManager.m_pTables->m_pPrograms[GetBucket(name)];
    while (p != nullptr && p->m_Name != name) {
        p = p->m_pNext;
    }
    return p;
}

inline Shader* FindShader(GLuint name)
{
    Shader* p = s_ShaderManager.m_pTables->m_pShaders[GetBucket(name)];
    while (p != nullptr && p->m_Name != name) {
        p = p->m_pNext;
    }
    return p;
}

// a new object with the name in its list (sorted); false if the name is taken
template <typename T>
inline bool InsertNamed(T** ppBucket, T* pObject, GLuint name)
{
    T* pHead = *ppBucket;
    if (pHead == nullptr) {
        pObject->m_Name = name;
        pObject->m_pNext = nullptr;
        *ppBucket = pObject;
        return true;
    }
    if (pHead->m_Name == name) {
        return false;
    }
    if (pHead->m_Name > name) {
        pObject->m_Name = name;
        pObject->m_pNext = pHead;
        *ppBucket = pObject;
        return true;
    }
    T* pPrev = pHead;
    for (T* p = pHead->m_pNext; p != nullptr; p = p->m_pNext) {
        if (p->m_Name == name) {
            return false;
        }
        if (p->m_Name > name) {
            pObject->m_Name = name;
            pPrev->m_pNext = pObject;
            pObject->m_pNext = p;
            return true;
        }
        pPrev = p;
    }
    pPrev->m_pNext = pObject;
    pObject->m_Name = name;
    pObject->m_pNext = nullptr;
    return true;
}

inline void* AllocateObject(GLsizei size)
{
    GxAllocateFunction pAllocate = s_Allocator.m_pAllocate;
    return pAllocate != nullptr ? pAllocate(MEMORY_AREA_MAIN, MEMORY_PURPOSE_OBJECT, 0, size) : nullptr;
}

inline void FreeObject(void* p)
{
    if (s_Allocator.m_pFree != nullptr) {
        s_Allocator.m_pFree(MEMORY_AREA_MAIN, MEMORY_PURPOSE_OBJECT, 0, p);
    }
}

inline void FreeBuffer(void* p)
{
    if (p != nullptr && s_Allocator.m_pFree != nullptr) {
        s_Allocator.m_pFree(MEMORY_AREA_MAIN, MEMORY_PURPOSE_OBJECT, 0, p);
    }
}

// the shader leaves its list and is freed (its binary with its last shader)
inline void DestroyShaderInline(GLuint name)
{
    ShaderTables* pTables = s_ShaderManager.m_pTables;
    Shader* pPrev = nullptr;
    Shader* pShader = pTables->m_pShaders[GetBucket(name)];
    while (pShader != nullptr && pShader->m_Name != name) {
        pPrev = pShader;
        pShader = pShader->m_pNext;
    }
    if (pPrev != nullptr) {
        pPrev->m_pNext = pShader->m_pNext;
    } else {
        pTables->m_pShaders[GetBucket(name)] = pTables->m_pShaders[GetBucket(name)]->m_pNext;
    }
    if (s_ShaderManager.m_NextShaderName > name) {
        s_ShaderManager.m_NextShaderName = name;
    }
    if (pShader->m_pBinary != nullptr && --pShader->m_pBinary->m_ReferenceCount == 0) {
        ReleaseShaderBinary(pShader->m_pBinary);
    }
    FreeObject(pShader);
}
} // namespace

// 0x007A9098 | nintendogs:bytes [tier B]
extern "C" GLuint glCreateProgram()
{
    Program* pProgram = static_cast<Program*>(AllocateObject(sizeof(Program)));
    memset(pProgram, 0, sizeof(Program));
    GLuint name = s_ShaderManager.m_NextProgramName;
    while (!InsertNamed(&s_ShaderManager.m_pTables->m_pPrograms[GetBucket(name)], pProgram, name)) {
        name++;
    }
    s_ShaderManager.m_NextProgramName = name + 1;
    return name;
}

// 0x007A91B0 | nintendogs:bytes [tier B]
extern "C" GLuint glCreateShader(GLenum type)
{
    Shader* pShader = static_cast<Shader*>(AllocateObject(sizeof(Shader)));
    pShader->m_pBinary = nullptr;
    pShader->m_EntryIndex = 0;
    pShader->m_Name = 0;
    pShader->m_AttachCount = 0;
    pShader->m_IsDeletePending = false;
    pShader->m_Type = type;
    pShader->m_pNext = nullptr;
    GLuint name = s_ShaderManager.m_NextShaderName;
    while (!InsertNamed(&s_ShaderManager.m_pTables->m_pShaders[GetBucket(name)], pShader, name)) {
        name++;
    }
    s_ShaderManager.m_NextShaderName = name + 1;
    return name;
}

// (the program in use is deleted when it is not used any more)
// 0x007A94DC | nintendogs:bytes [tier A]
extern "C" void glDeleteProgram(GLuint program)
{
    if (program == 0) {
        return;
    }
    Program* pCurrent = s_ShaderManager.m_pTables->m_pCurrentProgram;
    if (pCurrent != nullptr && pCurrent->m_Name == program) {
        pCurrent->m_IsDeletePending = true;
        return;
    }
    DestroyProgram(program);
}

// (a shader that is attached is deleted with its last detach)
// 0x007A9548 | nintendogs:bytes [tier B]
extern "C" void glDeleteShader(GLuint shader)
{
    Shader* pShader = FindShader(shader);
    if (pShader->m_AttachCount != 0) {
        pShader->m_IsDeletePending = true;
        return;
    }
    DestroyShaderInline(shader);
}

// 0x007A776C | nintendogs:bytes [tier B]
extern "C" void glAttachShader(GLuint program, GLuint shader)
{
    Program* pProgram = FindProgram(program);
    pProgram->m_IsDirty = true;
    if (shader == static_cast<GLuint>(-1)) {
        pProgram->m_Unknown0x08 = shader;
        return;
    }
    Shader* pShader = FindShader(shader);
    if (pShader->m_Type == SHADER_TYPE_VERTEX) {
        pShader->m_AttachCount++;
        pProgram->m_pVertexShader = pShader;
    } else if (pShader->m_Type == SHADER_TYPE_GEOMETRY) {
        pShader->m_AttachCount++;
        pProgram->m_pGeometryShader = pShader;
    }
}

// 0x007A9638 | nintendogs:bytes [tier B]
extern "C" void glDetachShader(GLuint program, GLuint shader)
{
    Program* pProgram = FindProgram(program);
    pProgram->m_IsDirty = true;
    if (shader == static_cast<GLuint>(-1)) {
        pProgram->m_Unknown0x08 = 0;
        return;
    }
    Shader* pShader = FindShader(shader);
    if (pShader->m_Type == SHADER_TYPE_VERTEX) {
        pShader->m_AttachCount--;
        pProgram->m_pVertexShader = nullptr;
    } else if (pShader->m_Type == SHADER_TYPE_GEOMETRY) {
        pShader->m_AttachCount--;
        pProgram->m_pGeometryShader = nullptr;
    } else {
        return;
    }
    if (pShader->m_AttachCount != 0 || !pShader->m_IsDeletePending || shader == 0) {
        return;
    }
    DestroyShaderInline(shader);
}

namespace nn {
namespace gx {
namespace detail {
// 0x007B95F0 (name is ours)
void DestroyProgram(GLuint name)
{
    ShaderTables* pTables = s_ShaderManager.m_pTables;
    Program* pPrev = nullptr;
    Program* pProgram = pTables->m_pPrograms[GetBucket(name)];
    while (pProgram != nullptr && pProgram->m_Name != name) {
        pPrev = pProgram;
        pProgram = pProgram->m_pNext;
    }
    if (pProgram->m_pVertexShader != nullptr) {
        glDetachShader(name, pProgram->m_pVertexShader->m_Name);
    }
    if (pProgram->m_pGeometryShader != nullptr) {
        glDetachShader(name, pProgram->m_pGeometryShader->m_Name);
    }
    if (pPrev != nullptr) {
        pPrev->m_pNext = pProgram->m_pNext;
    } else {
        s_ShaderManager.m_pTables->m_pPrograms[GetBucket(name)] = s_ShaderManager.m_pTables->m_pPrograms[GetBucket(name)]->m_pNext;
    }
    if (s_ShaderManager.m_NextProgramName > name) {
        s_ShaderManager.m_NextProgramName = name;
    }
    FreeBuffer(pProgram->m_pVertexFloatValues);
    FreeBuffer(pProgram->m_pGeometryFloatValues);
    FreeBuffer(pProgram->m_pLocations);
    for (ProgramListNode* pNode = pProgram->m_pList; pNode != nullptr;) {
        ProgramListNode* pNext = pNode->m_pNext;
        if (s_Allocator.m_pFree != nullptr) {
            s_Allocator.m_pFree(MEMORY_AREA_MAIN, MEMORY_PURPOSE_OBJECT, 0, pNode->m_pName);
            FreeObject(pNode);
        }
        pNode = pNext;
    }
    FreeObject(pProgram);
}

// 0x007B943C (name is ours)
void ReleaseShaderBinary(ShaderBinary* pBinary)
{
    ShaderTables* pTables = s_ShaderManager.m_pTables;
    if (pTables->m_pBinaries == pBinary) {
        pTables->m_pBinaries = pBinary->m_pNext;
        if (pBinary->m_pNext != nullptr) {
            pBinary->m_pNext->m_pPrevious = nullptr;
        }
    } else {
        pBinary->m_pPrevious->m_pNext = pBinary->m_pNext;
        if (pBinary->m_pNext != nullptr) {
            pBinary->m_pNext->m_pPrevious = pBinary->m_pPrevious;
        }
    }
    FreeBuffer(pBinary->m_pCode);
    FreeBuffer(pBinary->m_pOperandDescriptors);
    if (pBinary->m_pEntries != nullptr) {
        for (u32 i = 0; i < pBinary->m_EntryCount; i++) {
            FreeBuffer(pBinary->m_pEntries[i].m_pFloatConstants);
            FreeBuffer(pBinary->m_pEntries[i].m_pUniforms);
            FreeBuffer(pBinary->m_pEntries[i].m_pSymbols);
        }
        if (s_Allocator.m_pFree == nullptr) {
            return;
        }
        s_Allocator.m_pFree(MEMORY_AREA_MAIN, MEMORY_PURPOSE_OBJECT, 0, pBinary->m_pEntries);
    }
    FreeObject(pBinary);
}
} // namespace detail
} // namespace gx
} // namespace nn

namespace {
// the types of the reserved uniforms (the OpenGL ES values)
const GLenum TYPE_INT = 0x1404;
const GLenum TYPE_FLOAT = 0x1406;
const GLenum TYPE_FLOAT_VEC3 = 0x8B51;
const GLenum TYPE_FLOAT_VEC4 = 0x8B52;
const GLenum TYPE_INT_VEC2 = 0x8B53;
const GLenum TYPE_INT_VEC3 = 0x8B54;
const GLenum TYPE_BOOL = 0x8B56;
const GLenum TYPE_SAMPLER_1D = 0x8B5D;

struct UniformDefinition
{
    s32 m_Index;
    UniformInfo m_Info;
};

struct RegisterDefinition
{
    s32 m_Index;
    u32 m_Register;
};

// the reserved uniforms of the fixed function parts (index into s_ReservedUniforms, count, type,
// name; the names are strings of the binary), up to the index -1
// 0x008B19A0 (name is ours)
const UniformDefinition s_UniformDefinitions[] = {
    {0, {1, TYPE_BOOL, "dmp_Texture[0].perspectiveShadow"}},
    {1, {1, TYPE_FLOAT, "dmp_Texture[0].shadowZBias"}},
    {2, {1, TYPE_FLOAT, "dmp_Texture[0].shadowZScale"}},
    {3, {1, TYPE_INT, "dmp_Texture[0].samplerType"}},
    {4, {1, TYPE_INT, "dmp_Texture[1].samplerType"}},
    {5, {1, TYPE_INT, "dmp_Texture[2].samplerType"}},
    {6, {1, TYPE_INT, "dmp_Texture[3].samplerType"}},
    {7, {1, TYPE_INT, "dmp_Texture[2].texcoord"}},
    {8, {1, TYPE_INT, "dmp_Texture[3].texcoord"}},
    {9, {1, TYPE_INT, "dmp_Texture[3].ptRgbMap"}},
    {10, {1, TYPE_INT, "dmp_Texture[3].ptAlphaMap"}},
    {11, {1, TYPE_BOOL, "dmp_Texture[3].ptAlphaSeparate"}},
    {12, {1, TYPE_INT, "dmp_Texture[3].ptClampU"}},
    {13, {1, TYPE_INT, "dmp_Texture[3].ptClampV"}},
    {14, {1, TYPE_INT, "dmp_Texture[3].ptShiftU"}},
    {15, {1, TYPE_INT, "dmp_Texture[3].ptShiftV"}},
    {16, {1, TYPE_INT, "dmp_Texture[3].ptMinFilter"}},
    {17, {1, TYPE_INT, "dmp_Texture[3].ptTexWidth"}},
    {18, {1, TYPE_INT, "dmp_Texture[3].ptTexOffset"}},
    {19, {1, TYPE_FLOAT, "dmp_Texture[3].ptTexBias"}},
    {20, {1, TYPE_BOOL, "dmp_Texture[3].ptNoiseEnable"}},
    {21, {1, TYPE_FLOAT_VEC3, "dmp_Texture[3].ptNoiseU"}},
    {22, {1, TYPE_FLOAT_VEC3, "dmp_Texture[3].ptNoiseV"}},
    {23, {1, TYPE_SAMPLER_1D, "dmp_Texture[3].ptSamplerRgbMap"}},
    {24, {1, TYPE_SAMPLER_1D, "dmp_Texture[3].ptSamplerAlphaMap"}},
    {25, {1, TYPE_SAMPLER_1D, "dmp_Texture[3].ptSamplerNoiseMap"}},
    {26, {1, TYPE_SAMPLER_1D, "dmp_Texture[3].ptSamplerR"}},
    {27, {1, TYPE_SAMPLER_1D, "dmp_Texture[3].ptSamplerG"}},
    {28, {1, TYPE_SAMPLER_1D, "dmp_Texture[3].ptSamplerB"}},
    {29, {1, TYPE_SAMPLER_1D, "dmp_Texture[3].ptSamplerA"}},
    {30, {1, TYPE_INT, "dmp_FragOperation.mode"}},
    {31, {1, TYPE_FLOAT, "dmp_FragOperation.penumbraScale"}},
    {32, {1, TYPE_FLOAT, "dmp_FragOperation.penumbraBias"}},
    {33, {1, TYPE_FLOAT, "dmp_FragOperation.wScale"}},
    {34, {1, TYPE_BOOL, "dmp_FragOperation.enableClippingPlane"}},
    {35, {1, TYPE_FLOAT_VEC4, "dmp_FragOperation.clippingPlane"}},
    {36, {1, TYPE_BOOL, "dmp_FragOperation.enableAlphaTest"}},
    {37, {1, TYPE_INT, "dmp_FragOperation.alphaTestFunc"}},
    {38, {1, TYPE_FLOAT, "dmp_FragOperation.alphaRefValue"}},
    {39, {1, TYPE_FLOAT_VEC3, "dmp_Gas.lightXY"}},
    {40, {1, TYPE_FLOAT_VEC4, "dmp_Gas.lightZ"}},
    {41, {1, TYPE_FLOAT, "dmp_Gas.deltaZ"}},
    {42, {1, TYPE_FLOAT, "dmp_Gas.accMax"}},
    {43, {1, TYPE_BOOL, "dmp_Gas.autoAcc"}},
    {44, {1, TYPE_FLOAT, "dmp_Gas.attenuation"}},
    {45, {1, TYPE_INT, "dmp_Gas.colorLutInput"}},
    {46, {1, TYPE_INT, "dmp_Gas.shadingDensitySrc"}},
    {47, {1, TYPE_SAMPLER_1D, "dmp_Gas.samplerTR"}},
    {48, {1, TYPE_SAMPLER_1D, "dmp_Gas.samplerTG"}},
    {49, {1, TYPE_SAMPLER_1D, "dmp_Gas.samplerTB"}},
    {50, {1, TYPE_BOOL, "dmp_FragmentLighting.enabled"}},
    {51, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLighting.ambient"}},
    {52, {1, TYPE_FLOAT_VEC4, "dmp_FragmentMaterial.emission"}},
    {53, {1, TYPE_FLOAT_VEC4, "dmp_FragmentMaterial.ambient"}},
    {54, {1, TYPE_FLOAT_VEC4, "dmp_FragmentMaterial.diffuse"}},
    {55, {1, TYPE_FLOAT_VEC4, "dmp_FragmentMaterial.specular0"}},
    {56, {1, TYPE_FLOAT_VEC4, "dmp_FragmentMaterial.specular1"}},
    {57, {1, TYPE_BOOL, "dmp_FragmentLightSource[0].enabled"}},
    {58, {1, TYPE_BOOL, "dmp_FragmentLightSource[1].enabled"}},
    {59, {1, TYPE_BOOL, "dmp_FragmentLightSource[2].enabled"}},
    {60, {1, TYPE_BOOL, "dmp_FragmentLightSource[3].enabled"}},
    {61, {1, TYPE_BOOL, "dmp_FragmentLightSource[4].enabled"}},
    {62, {1, TYPE_BOOL, "dmp_FragmentLightSource[5].enabled"}},
    {63, {1, TYPE_BOOL, "dmp_FragmentLightSource[6].enabled"}},
    {64, {1, TYPE_BOOL, "dmp_FragmentLightSource[7].enabled"}},
    {65, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[0].ambient"}},
    {66, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[1].ambient"}},
    {67, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[2].ambient"}},
    {68, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[3].ambient"}},
    {69, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[4].ambient"}},
    {70, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[5].ambient"}},
    {71, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[6].ambient"}},
    {72, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[7].ambient"}},
    {73, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[0].diffuse"}},
    {74, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[1].diffuse"}},
    {75, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[2].diffuse"}},
    {76, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[3].diffuse"}},
    {77, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[4].diffuse"}},
    {78, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[5].diffuse"}},
    {79, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[6].diffuse"}},
    {80, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[7].diffuse"}},
    {81, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[0].specular0"}},
    {82, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[1].specular0"}},
    {83, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[2].specular0"}},
    {84, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[3].specular0"}},
    {85, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[4].specular0"}},
    {86, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[5].specular0"}},
    {87, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[6].specular0"}},
    {88, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[7].specular0"}},
    {89, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[0].specular1"}},
    {90, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[1].specular1"}},
    {91, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[2].specular1"}},
    {92, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[3].specular1"}},
    {93, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[4].specular1"}},
    {94, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[5].specular1"}},
    {95, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[6].specular1"}},
    {96, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[7].specular1"}},
    {97, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[0].position"}},
    {98, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[1].position"}},
    {99, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[2].position"}},
    {100, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[3].position"}},
    {101, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[4].position"}},
    {102, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[5].position"}},
    {103, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[6].position"}},
    {104, {1, TYPE_FLOAT_VEC4, "dmp_FragmentLightSource[7].position"}},
    {105, {1, TYPE_FLOAT_VEC3, "dmp_FragmentLightSource[0].spotDirection"}},
    {106, {1, TYPE_FLOAT_VEC3, "dmp_FragmentLightSource[1].spotDirection"}},
    {107, {1, TYPE_FLOAT_VEC3, "dmp_FragmentLightSource[2].spotDirection"}},
    {108, {1, TYPE_FLOAT_VEC3, "dmp_FragmentLightSource[3].spotDirection"}},
    {109, {1, TYPE_FLOAT_VEC3, "dmp_FragmentLightSource[4].spotDirection"}},
    {110, {1, TYPE_FLOAT_VEC3, "dmp_FragmentLightSource[5].spotDirection"}},
    {111, {1, TYPE_FLOAT_VEC3, "dmp_FragmentLightSource[6].spotDirection"}},
    {112, {1, TYPE_FLOAT_VEC3, "dmp_FragmentLightSource[7].spotDirection"}},
    {113, {1, TYPE_BOOL, "dmp_FragmentLightSource[0].shadowed"}},
    {114, {1, TYPE_BOOL, "dmp_FragmentLightSource[1].shadowed"}},
    {115, {1, TYPE_BOOL, "dmp_FragmentLightSource[2].shadowed"}},
    {116, {1, TYPE_BOOL, "dmp_FragmentLightSource[3].shadowed"}},
    {117, {1, TYPE_BOOL, "dmp_FragmentLightSource[4].shadowed"}},
    {118, {1, TYPE_BOOL, "dmp_FragmentLightSource[5].shadowed"}},
    {119, {1, TYPE_BOOL, "dmp_FragmentLightSource[6].shadowed"}},
    {120, {1, TYPE_BOOL, "dmp_FragmentLightSource[7].shadowed"}},
    {121, {1, TYPE_BOOL, "dmp_FragmentLightSource[0].geomFactor0"}},
    {122, {1, TYPE_BOOL, "dmp_FragmentLightSource[1].geomFactor0"}},
    {123, {1, TYPE_BOOL, "dmp_FragmentLightSource[2].geomFactor0"}},
    {124, {1, TYPE_BOOL, "dmp_FragmentLightSource[3].geomFactor0"}},
    {125, {1, TYPE_BOOL, "dmp_FragmentLightSource[4].geomFactor0"}},
    {126, {1, TYPE_BOOL, "dmp_FragmentLightSource[5].geomFactor0"}},
    {127, {1, TYPE_BOOL, "dmp_FragmentLightSource[6].geomFactor0"}},
    {128, {1, TYPE_BOOL, "dmp_FragmentLightSource[7].geomFactor0"}},
    {129, {1, TYPE_BOOL, "dmp_FragmentLightSource[0].geomFactor1"}},
    {130, {1, TYPE_BOOL, "dmp_FragmentLightSource[1].geomFactor1"}},
    {131, {1, TYPE_BOOL, "dmp_FragmentLightSource[2].geomFactor1"}},
    {132, {1, TYPE_BOOL, "dmp_FragmentLightSource[3].geomFactor1"}},
    {133, {1, TYPE_BOOL, "dmp_FragmentLightSource[4].geomFactor1"}},
    {134, {1, TYPE_BOOL, "dmp_FragmentLightSource[5].geomFactor1"}},
    {135, {1, TYPE_BOOL, "dmp_FragmentLightSource[6].geomFactor1"}},
    {136, {1, TYPE_BOOL, "dmp_FragmentLightSource[7].geomFactor1"}},
    {137, {1, TYPE_BOOL, "dmp_FragmentLightSource[0].twoSideDiffuse"}},
    {138, {1, TYPE_BOOL, "dmp_FragmentLightSource[1].twoSideDiffuse"}},
    {139, {1, TYPE_BOOL, "dmp_FragmentLightSource[2].twoSideDiffuse"}},
    {140, {1, TYPE_BOOL, "dmp_FragmentLightSource[3].twoSideDiffuse"}},
    {141, {1, TYPE_BOOL, "dmp_FragmentLightSource[4].twoSideDiffuse"}},
    {142, {1, TYPE_BOOL, "dmp_FragmentLightSource[5].twoSideDiffuse"}},
    {143, {1, TYPE_BOOL, "dmp_FragmentLightSource[6].twoSideDiffuse"}},
    {144, {1, TYPE_BOOL, "dmp_FragmentLightSource[7].twoSideDiffuse"}},
    {145, {1, TYPE_SAMPLER_1D, "dmp_FragmentLightSource[0].samplerSP"}},
    {146, {1, TYPE_SAMPLER_1D, "dmp_FragmentLightSource[1].samplerSP"}},
    {147, {1, TYPE_SAMPLER_1D, "dmp_FragmentLightSource[2].samplerSP"}},
    {148, {1, TYPE_SAMPLER_1D, "dmp_FragmentLightSource[3].samplerSP"}},
    {149, {1, TYPE_SAMPLER_1D, "dmp_FragmentLightSource[4].samplerSP"}},
    {150, {1, TYPE_SAMPLER_1D, "dmp_FragmentLightSource[5].samplerSP"}},
    {151, {1, TYPE_SAMPLER_1D, "dmp_FragmentLightSource[6].samplerSP"}},
    {152, {1, TYPE_SAMPLER_1D, "dmp_FragmentLightSource[7].samplerSP"}},
    {153, {1, TYPE_BOOL, "dmp_FragmentLightSource[0].spotEnabled"}},
    {154, {1, TYPE_BOOL, "dmp_FragmentLightSource[1].spotEnabled"}},
    {155, {1, TYPE_BOOL, "dmp_FragmentLightSource[2].spotEnabled"}},
    {156, {1, TYPE_BOOL, "dmp_FragmentLightSource[3].spotEnabled"}},
    {157, {1, TYPE_BOOL, "dmp_FragmentLightSource[4].spotEnabled"}},
    {158, {1, TYPE_BOOL, "dmp_FragmentLightSource[5].spotEnabled"}},
    {159, {1, TYPE_BOOL, "dmp_FragmentLightSource[6].spotEnabled"}},
    {160, {1, TYPE_BOOL, "dmp_FragmentLightSource[7].spotEnabled"}},
    {161, {1, TYPE_FLOAT, "dmp_FragmentLightSource[0].distanceAttenuationBias"}},
    {162, {1, TYPE_FLOAT, "dmp_FragmentLightSource[1].distanceAttenuationBias"}},
    {163, {1, TYPE_FLOAT, "dmp_FragmentLightSource[2].distanceAttenuationBias"}},
    {164, {1, TYPE_FLOAT, "dmp_FragmentLightSource[3].distanceAttenuationBias"}},
    {165, {1, TYPE_FLOAT, "dmp_FragmentLightSource[4].distanceAttenuationBias"}},
    {166, {1, TYPE_FLOAT, "dmp_FragmentLightSource[5].distanceAttenuationBias"}},
    {167, {1, TYPE_FLOAT, "dmp_FragmentLightSource[6].distanceAttenuationBias"}},
    {168, {1, TYPE_FLOAT, "dmp_FragmentLightSource[7].distanceAttenuationBias"}},
    {169, {1, TYPE_FLOAT, "dmp_FragmentLightSource[0].distanceAttenuationScale"}},
    {170, {1, TYPE_FLOAT, "dmp_FragmentLightSource[1].distanceAttenuationScale"}},
    {171, {1, TYPE_FLOAT, "dmp_FragmentLightSource[2].distanceAttenuationScale"}},
    {172, {1, TYPE_FLOAT, "dmp_FragmentLightSource[3].distanceAttenuationScale"}},
    {173, {1, TYPE_FLOAT, "dmp_FragmentLightSource[4].distanceAttenuationScale"}},
    {174, {1, TYPE_FLOAT, "dmp_FragmentLightSource[5].distanceAttenuationScale"}},
    {175, {1, TYPE_FLOAT, "dmp_FragmentLightSource[6].distanceAttenuationScale"}},
    {176, {1, TYPE_FLOAT, "dmp_FragmentLightSource[7].distanceAttenuationScale"}},
    {177, {1, TYPE_BOOL, "dmp_FragmentLightSource[0].distanceAttenuationEnabled"}},
    {178, {1, TYPE_BOOL, "dmp_FragmentLightSource[1].distanceAttenuationEnabled"}},
    {179, {1, TYPE_BOOL, "dmp_FragmentLightSource[2].distanceAttenuationEnabled"}},
    {180, {1, TYPE_BOOL, "dmp_FragmentLightSource[3].distanceAttenuationEnabled"}},
    {181, {1, TYPE_BOOL, "dmp_FragmentLightSource[4].distanceAttenuationEnabled"}},
    {182, {1, TYPE_BOOL, "dmp_FragmentLightSource[5].distanceAttenuationEnabled"}},
    {183, {1, TYPE_BOOL, "dmp_FragmentLightSource[6].distanceAttenuationEnabled"}},
    {184, {1, TYPE_BOOL, "dmp_FragmentLightSource[7].distanceAttenuationEnabled"}},
    {185, {1, TYPE_SAMPLER_1D, "dmp_FragmentLightSource[0].samplerDA"}},
    {186, {1, TYPE_SAMPLER_1D, "dmp_FragmentLightSource[1].samplerDA"}},
    {187, {1, TYPE_SAMPLER_1D, "dmp_FragmentLightSource[2].samplerDA"}},
    {188, {1, TYPE_SAMPLER_1D, "dmp_FragmentLightSource[3].samplerDA"}},
    {189, {1, TYPE_SAMPLER_1D, "dmp_FragmentLightSource[4].samplerDA"}},
    {190, {1, TYPE_SAMPLER_1D, "dmp_FragmentLightSource[5].samplerDA"}},
    {191, {1, TYPE_SAMPLER_1D, "dmp_FragmentLightSource[6].samplerDA"}},
    {192, {1, TYPE_SAMPLER_1D, "dmp_FragmentLightSource[7].samplerDA"}},
    {193, {1, TYPE_BOOL, "dmp_LightEnv.absLutInputD0"}},
    {194, {1, TYPE_BOOL, "dmp_LightEnv.absLutInputD1"}},
    {195, {1, TYPE_BOOL, "dmp_LightEnv.absLutInputSP"}},
    {196, {1, TYPE_BOOL, "dmp_LightEnv.absLutInputFR"}},
    {197, {1, TYPE_BOOL, "dmp_LightEnv.absLutInputRB"}},
    {198, {1, TYPE_BOOL, "dmp_LightEnv.absLutInputRG"}},
    {199, {1, TYPE_BOOL, "dmp_LightEnv.absLutInputRR"}},
    {200, {1, TYPE_INT, "dmp_LightEnv.lutInputD0"}},
    {201, {1, TYPE_INT, "dmp_LightEnv.lutInputD1"}},
    {202, {1, TYPE_INT, "dmp_LightEnv.lutInputSP"}},
    {203, {1, TYPE_INT, "dmp_LightEnv.lutInputFR"}},
    {204, {1, TYPE_INT, "dmp_LightEnv.lutInputRB"}},
    {205, {1, TYPE_INT, "dmp_LightEnv.lutInputRG"}},
    {206, {1, TYPE_INT, "dmp_LightEnv.lutInputRR"}},
    {207, {1, TYPE_FLOAT, "dmp_LightEnv.lutScaleD0"}},
    {208, {1, TYPE_FLOAT, "dmp_LightEnv.lutScaleD1"}},
    {209, {1, TYPE_FLOAT, "dmp_LightEnv.lutScaleSP"}},
    {210, {1, TYPE_FLOAT, "dmp_LightEnv.lutScaleFR"}},
    {211, {1, TYPE_FLOAT, "dmp_LightEnv.lutScaleRB"}},
    {212, {1, TYPE_FLOAT, "dmp_LightEnv.lutScaleRG"}},
    {213, {1, TYPE_FLOAT, "dmp_LightEnv.lutScaleRR"}},
    {214, {1, TYPE_SAMPLER_1D, "dmp_FragmentMaterial.samplerD0"}},
    {215, {1, TYPE_SAMPLER_1D, "dmp_FragmentMaterial.samplerD1"}},
    {216, {1, TYPE_SAMPLER_1D, "dmp_FragmentMaterial.samplerFR"}},
    {217, {1, TYPE_SAMPLER_1D, "dmp_FragmentMaterial.samplerRB"}},
    {218, {1, TYPE_SAMPLER_1D, "dmp_FragmentMaterial.samplerRG"}},
    {219, {1, TYPE_SAMPLER_1D, "dmp_FragmentMaterial.samplerRR"}},
    {220, {1, TYPE_INT, "dmp_LightEnv.shadowSelector"}},
    {221, {1, TYPE_INT, "dmp_LightEnv.bumpSelector"}},
    {222, {1, TYPE_INT, "dmp_LightEnv.bumpMode"}},
    {223, {1, TYPE_BOOL, "dmp_LightEnv.bumpRenorm"}},
    {224, {1, TYPE_INT, "dmp_LightEnv.config"}},
    {225, {1, TYPE_BOOL, "dmp_LightEnv.invertShadow"}},
    {226, {1, TYPE_BOOL, "dmp_LightEnv.shadowPrimary"}},
    {227, {1, TYPE_BOOL, "dmp_LightEnv.shadowSecondary"}},
    {228, {1, TYPE_BOOL, "dmp_LightEnv.shadowAlpha"}},
    {229, {1, TYPE_INT, "dmp_LightEnv.fresnelSelector"}},
    {230, {1, TYPE_BOOL, "dmp_LightEnv.clampHighlights"}},
    {231, {1, TYPE_BOOL, "dmp_LightEnv.lutEnabledD0"}},
    {232, {1, TYPE_BOOL, "dmp_LightEnv.lutEnabledD1"}},
    {233, {1, TYPE_BOOL, "dmp_LightEnv.lutEnabledRefl"}},
    {234, {1, TYPE_INT, "dmp_TexEnv[0].combineRgb"}},
    {235, {1, TYPE_INT, "dmp_TexEnv[1].combineRgb"}},
    {236, {1, TYPE_INT, "dmp_TexEnv[2].combineRgb"}},
    {237, {1, TYPE_INT, "dmp_TexEnv[3].combineRgb"}},
    {238, {1, TYPE_INT, "dmp_TexEnv[4].combineRgb"}},
    {239, {1, TYPE_INT, "dmp_TexEnv[5].combineRgb"}},
    {240, {1, TYPE_INT, "dmp_TexEnv[0].combineAlpha"}},
    {241, {1, TYPE_INT, "dmp_TexEnv[1].combineAlpha"}},
    {242, {1, TYPE_INT, "dmp_TexEnv[2].combineAlpha"}},
    {243, {1, TYPE_INT, "dmp_TexEnv[3].combineAlpha"}},
    {244, {1, TYPE_INT, "dmp_TexEnv[4].combineAlpha"}},
    {245, {1, TYPE_INT, "dmp_TexEnv[5].combineAlpha"}},
    {246, {1, TYPE_INT_VEC3, "dmp_TexEnv[0].srcRgb"}},
    {247, {1, TYPE_INT_VEC3, "dmp_TexEnv[1].srcRgb"}},
    {248, {1, TYPE_INT_VEC3, "dmp_TexEnv[2].srcRgb"}},
    {249, {1, TYPE_INT_VEC3, "dmp_TexEnv[3].srcRgb"}},
    {250, {1, TYPE_INT_VEC3, "dmp_TexEnv[4].srcRgb"}},
    {251, {1, TYPE_INT_VEC3, "dmp_TexEnv[5].srcRgb"}},
    {252, {1, TYPE_INT_VEC3, "dmp_TexEnv[0].srcAlpha"}},
    {253, {1, TYPE_INT_VEC3, "dmp_TexEnv[1].srcAlpha"}},
    {254, {1, TYPE_INT_VEC3, "dmp_TexEnv[2].srcAlpha"}},
    {255, {1, TYPE_INT_VEC3, "dmp_TexEnv[3].srcAlpha"}},
    {256, {1, TYPE_INT_VEC3, "dmp_TexEnv[4].srcAlpha"}},
    {257, {1, TYPE_INT_VEC3, "dmp_TexEnv[5].srcAlpha"}},
    {258, {1, TYPE_INT_VEC3, "dmp_TexEnv[0].operandRgb"}},
    {259, {1, TYPE_INT_VEC3, "dmp_TexEnv[1].operandRgb"}},
    {260, {1, TYPE_INT_VEC3, "dmp_TexEnv[2].operandRgb"}},
    {261, {1, TYPE_INT_VEC3, "dmp_TexEnv[3].operandRgb"}},
    {262, {1, TYPE_INT_VEC3, "dmp_TexEnv[4].operandRgb"}},
    {263, {1, TYPE_INT_VEC3, "dmp_TexEnv[5].operandRgb"}},
    {264, {1, TYPE_INT_VEC3, "dmp_TexEnv[0].operandAlpha"}},
    {265, {1, TYPE_INT_VEC3, "dmp_TexEnv[1].operandAlpha"}},
    {266, {1, TYPE_INT_VEC3, "dmp_TexEnv[2].operandAlpha"}},
    {267, {1, TYPE_INT_VEC3, "dmp_TexEnv[3].operandAlpha"}},
    {268, {1, TYPE_INT_VEC3, "dmp_TexEnv[4].operandAlpha"}},
    {269, {1, TYPE_INT_VEC3, "dmp_TexEnv[5].operandAlpha"}},
    {270, {1, TYPE_FLOAT, "dmp_TexEnv[0].scaleRgb"}},
    {271, {1, TYPE_FLOAT, "dmp_TexEnv[1].scaleRgb"}},
    {272, {1, TYPE_FLOAT, "dmp_TexEnv[2].scaleRgb"}},
    {273, {1, TYPE_FLOAT, "dmp_TexEnv[3].scaleRgb"}},
    {274, {1, TYPE_FLOAT, "dmp_TexEnv[4].scaleRgb"}},
    {275, {1, TYPE_FLOAT, "dmp_TexEnv[5].scaleRgb"}},
    {276, {1, TYPE_FLOAT, "dmp_TexEnv[0].scaleAlpha"}},
    {277, {1, TYPE_FLOAT, "dmp_TexEnv[1].scaleAlpha"}},
    {278, {1, TYPE_FLOAT, "dmp_TexEnv[2].scaleAlpha"}},
    {279, {1, TYPE_FLOAT, "dmp_TexEnv[3].scaleAlpha"}},
    {280, {1, TYPE_FLOAT, "dmp_TexEnv[4].scaleAlpha"}},
    {281, {1, TYPE_FLOAT, "dmp_TexEnv[5].scaleAlpha"}},
    {282, {1, TYPE_FLOAT_VEC4, "dmp_TexEnv[0].constRgba"}},
    {283, {1, TYPE_FLOAT_VEC4, "dmp_TexEnv[1].constRgba"}},
    {284, {1, TYPE_FLOAT_VEC4, "dmp_TexEnv[2].constRgba"}},
    {285, {1, TYPE_FLOAT_VEC4, "dmp_TexEnv[3].constRgba"}},
    {286, {1, TYPE_FLOAT_VEC4, "dmp_TexEnv[4].constRgba"}},
    {287, {1, TYPE_FLOAT_VEC4, "dmp_TexEnv[5].constRgba"}},
    {288, {1, TYPE_FLOAT_VEC4, "dmp_TexEnv[0].bufferColor"}},
    {289, {1, TYPE_INT_VEC2, "dmp_TexEnv[1].bufferInput"}},
    {290, {1, TYPE_INT_VEC2, "dmp_TexEnv[2].bufferInput"}},
    {291, {1, TYPE_INT_VEC2, "dmp_TexEnv[3].bufferInput"}},
    {292, {1, TYPE_INT_VEC2, "dmp_TexEnv[4].bufferInput"}},
    {293, {1, TYPE_INT, "dmp_Fog.mode"}},
    {294, {1, TYPE_FLOAT_VEC3, "dmp_Fog.color"}},
    {295, {1, TYPE_BOOL, "dmp_Fog.zFlip"}},
    {296, {1, TYPE_SAMPLER_1D, "dmp_Fog.sampler"}},
    {-1, {0, 0, nullptr}},
};

// the GPU registers the shader tables keep (index into s_ShaderRegisters, register), up to the
// index -1
// 0x008B2C40 (name is ours)
const RegisterDefinition s_RegisterDefinitions[] = {
    {0, 0x229}, {1, 0x280}, {2, 0x281}, {3, 0x282}, {4, 0x283}, {5, 0x284},
    {6, 0x289}, {7, 0x28a}, {8, 0x28d}, {9, 0x2b0}, {10, 0x2b1}, {11, 0x2b2},
    {12, 0x2b3}, {13, 0x2b4}, {14, 0x2b9}, {15, 0x2ba}, {16, 0x2bd}, {17, 0x251},
    {18, 0x252}, {19, 0x254}, {20, 0x242}, {21, 0x24a}, {22, 0x25e}, {23, 0x4d},
    {24, 0x4e}, {25, 0x4f}, {26, 0x50}, {27, 0x51}, {28, 0x52}, {29, 0x53},
    {30, 0x54}, {31, 0x55}, {32, 0x56}, {33, 0x47}, {34, 0x48}, {35, 0x49},
    {36, 0x4a}, {37, 0x4b}, {38, 0x64}, {39, 0x6d}, {40, 0x6f}, {41, 0x80},
    {42, 0x8b}, {43, 0x8f}, {44, 0xa8}, {45, 0xa9}, {46, 0xaa}, {47, 0xab},
    {48, 0xac}, {49, 0xad}, {50, 0xc0}, {51, 0xc1}, {52, 0xc2}, {53, 0xc3},
    {54, 0xc4}, {55, 0xc8}, {56, 0xc9}, {57, 0xca}, {58, 0xcb}, {59, 0xcc},
    {60, 0xd0}, {61, 0xd1}, {62, 0xd2}, {63, 0xd3}, {64, 0xd4}, {65, 0xd8},
    {66, 0xd9}, {67, 0xda}, {68, 0xdb}, {69, 0xdc}, {70, 0xf0}, {71, 0xf1},
    {72, 0xf2}, {73, 0xf3}, {74, 0xf4}, {75, 0xf8}, {76, 0xf9}, {77, 0xfa},
    {78, 0xfb}, {79, 0xfc}, {84, 0xfd}, {80, 0xe0}, {81, 0xe1}, {82, 0xe4},
    {83, 0xe5}, {85, 0x120}, {86, 0x121}, {87, 0x122}, {88, 0x126}, {89, 0x100},
    {90, 0x104}, {91, 0x130}, {92, 0x140}, {93, 0x141}, {94, 0x142}, {95, 0x143},
    {96, 0x144}, {97, 0x145}, {98, 0x146}, {99, 0x147}, {100, 0x149}, {101, 0x14a},
    {102, 0x14b}, {103, 0x150}, {104, 0x151}, {105, 0x152}, {106, 0x153}, {107, 0x154},
    {108, 0x155}, {109, 0x156}, {110, 0x157}, {111, 0x159}, {112, 0x15a}, {113, 0x15b},
    {114, 0x160}, {115, 0x161}, {116, 0x162}, {117, 0x163}, {118, 0x164}, {119, 0x165},
    {120, 0x166}, {121, 0x167}, {122, 0x169}, {123, 0x16a}, {124, 0x16b}, {125, 0x170},
    {126, 0x171}, {127, 0x172}, {128, 0x173}, {129, 0x174}, {130, 0x175}, {131, 0x176},
    {132, 0x177}, {133, 0x179}, {134, 0x17a}, {135, 0x17b}, {136, 0x180}, {137, 0x181},
    {138, 0x182}, {139, 0x183}, {140, 0x184}, {141, 0x185}, {142, 0x186}, {143, 0x187},
    {144, 0x189}, {145, 0x18a}, {146, 0x18b}, {147, 0x190}, {148, 0x191}, {149, 0x192},
    {150, 0x193}, {151, 0x194}, {152, 0x195}, {153, 0x196}, {154, 0x197}, {155, 0x199},
    {156, 0x19a}, {157, 0x19b}, {158, 0x1a0}, {159, 0x1a1}, {160, 0x1a2}, {161, 0x1a3},
    {162, 0x1a4}, {163, 0x1a5}, {164, 0x1a6}, {165, 0x1a7}, {166, 0x1a9}, {167, 0x1aa},
    {168, 0x1ab}, {169, 0x1b0}, {170, 0x1b1}, {171, 0x1b2}, {172, 0x1b3}, {173, 0x1b4},
    {174, 0x1b5}, {175, 0x1b6}, {176, 0x1b7}, {177, 0x1b9}, {178, 0x1ba}, {179, 0x1bb},
    {180, 0x1c0}, {181, 0x1c2}, {182, 0x1c3}, {183, 0x1c4}, {184, 0x1c6}, {185, 0x1d0},
    {186, 0x1d1}, {187, 0x1d2}, {188, 0x1d9}, {-1, 0},
};

// the registers of the eight lights (11 each from LIGHT_FIRST on) and of the six texture combiner
// stages (5 each from COMBINER_FIRST on)
const s32 LIGHT_FIRST = 92;
const s32 LIGHT_LAST = 179;
const s32 COMBINER_FIRST = 50;
const s32 COMBINER_COUNT = 30;

// a register of the shader tables with its byte mask
inline void SetRegister(ShaderTables* pTables, s32 index, u32 value, u8 mask)
{
    pTables->m_RegisterValues[index] = value;
    pTables->m_RegisterMasks[index] = mask;
}

inline void SetBits(u32* pSet, const s32* pList)
{
    for (const s32* p = pList; *p != static_cast<s32>(SHADER_REGISTER_COUNT); p++) {
        pSet[*p >> 5] |= 1 << (*p & 31);
    }
}
} // namespace

extern "C" {
// (the reserved uniforms and the registers from their tables, the registers of the shader tables
// with their values at the start, then the register sets of the lists)
// 0x00126DFC | nintendogs:bytes [tier B]
int __shm_initializeShaderManager(void* pMemory)
{
    s_ShaderManager.m_pTables = static_cast<ShaderTables*>(pMemory);
    memset(pMemory, 0, sizeof(ShaderTables));
    int result = 0;
    s_ShaderManager.m_NextShaderName = 1;
    s_ShaderManager.m_LastGeometryShaderState = -1;
    s_ShaderManager.m_NextProgramName = 1;
    for (const UniformDefinition* p = s_UniformDefinitions; p->m_Index != -1; p++) {
        s_ReservedUniforms[p->m_Index] = p->m_Info;
    }
    for (const RegisterDefinition* p = s_RegisterDefinitions; p->m_Index != -1; p++) {
        s_ShaderRegisters[p->m_Index] = p->m_Register;
    }
    ShaderTables* pTables = s_ShaderManager.m_pTables;
    SetRegister(pTables, 41, 0x1000, 0x2);
    SetRegister(pTables, 42, 0, 0xF);
    SetRegister(pTables, 43, 0, 0xF);
    SetRegister(pTables, 44, 9, 0xF);
    SetRegister(pTables, 45, 0, 0xF);
    SetRegister(pTables, 46, 0, 0xF);
    SetRegister(pTables, 47, 0, 0xF);
    SetRegister(pTables, 48, 0x1C00301, 0xF);
    SetRegister(pTables, 49, 0xE0C08000, 0xF);
    SetRegister(pTables, 91, 0x3C00, 0xF);
    SetRegister(pTables, 25, 0, 0xF);
    for (s32 i = 26; i <= 32; i++) {
        SetRegister(pTables, i, 0x1F1F1F1F, 0xF);
    }
    SetRegister(pTables, 39, 1, 0xF);
    SetRegister(pTables, 23, 0xBF0000, 0xF);
    SetRegister(pTables, 24, 0, 0xF);
    for (s32 i = 33; i <= 37; i++) {
        SetRegister(pTables, i, 0, 0xF);
    }
    for (s32 i = LIGHT_FIRST; i <= LIGHT_LAST; i++) {
        switch (i) {
        case LIGHT_FIRST + 5:
        case LIGHT_FIRST + 16:
        case LIGHT_FIRST + 27:
        case LIGHT_FIRST + 38:
        case LIGHT_FIRST + 49:
        case LIGHT_FIRST + 60:
        case LIGHT_FIRST + 71:
        case LIGHT_FIRST + 82:
            SetRegister(pTables, i, 0x3C00, 0xF);
            break;
        case LIGHT_FIRST + 7:
        case LIGHT_FIRST + 18:
        case LIGHT_FIRST + 29:
        case LIGHT_FIRST + 40:
        case LIGHT_FIRST + 51:
        case LIGHT_FIRST + 62:
        case LIGHT_FIRST + 73:
        case LIGHT_FIRST + 84:
            SetRegister(pTables, i, 0x800, 0xF);
            break;
        case LIGHT_FIRST + 8:
        case LIGHT_FIRST + 19:
        case LIGHT_FIRST + 30:
        case LIGHT_FIRST + 41:
        case LIGHT_FIRST + 52:
        case LIGHT_FIRST + 63:
        case LIGHT_FIRST + 74:
        case LIGHT_FIRST + 85:
            SetRegister(pTables, i, 1, 0xF);
            break;
        case LIGHT_FIRST + 10:
        case LIGHT_FIRST + 21:
        case LIGHT_FIRST + 32:
        case LIGHT_FIRST + 43:
        case LIGHT_FIRST + 54:
        case LIGHT_FIRST + 65:
        case LIGHT_FIRST + 76:
        case LIGHT_FIRST + 87:
            SetRegister(pTables, i, 0x3F000, 0xF);
            break;
        default:
            SetRegister(pTables, i, 0, 0xF);
            break;
        }
    }
    SetRegister(pTables, 94, 0xCC330CC, 0xF);
    SetRegister(pTables, 180, 0xA0280A, 0xF);
    SetRegister(pTables, 181, 0, 0xF);
    SetRegister(pTables, 184, 1, 0xF);
    SetRegister(pTables, 182, 0x88000400, 0xF);
    SetRegister(pTables, 183, 0xFFFFFFFF, 0xF);
    SetRegister(pTables, 186, 0, 0xF);
    SetRegister(pTables, 187, 0, 0xF);
    SetRegister(pTables, 185, 0x2222222, 0xF);
    SetRegister(pTables, 188, 0, 0xF);
    SetRegister(pTables, 80, 0, 0xF);
    SetRegister(pTables, 81, 0, 0xF);
    SetRegister(pTables, 82, 0x3C00, 0xF);
    SetRegister(pTables, 85, 0, 0xF);
    SetRegister(pTables, 86, 0, 0xF);
    SetRegister(pTables, 87, 0x100, 0xF);
    SetRegister(pTables, 88, 0xA00, 0x7);
    SetRegister(pTables, 83, 0x3C00, 0xF);
    SetRegister(pTables, 89, 0xE40100, 0xD);
    SetRegister(pTables, 90, 0x10, 0xF);
    for (s32 i = 0; i < COMBINER_COUNT; i++) {
        switch (i) {
        case 0:
            SetRegister(pTables, COMBINER_FIRST + i, 0xEEE0EEE, 0xF);
            break;
        case 5:
        case 10:
        case 15:
        case 20:
        case 25:
            SetRegister(pTables, COMBINER_FIRST + i, 0xFFF0FFF, 0xF);
            break;
        default:
            SetRegister(pTables, COMBINER_FIRST + i, 0, 0xF);
            break;
        }
    }
    SetRegister(pTables, 84, 0, 0xF);
    memcpy(pTables->m_SentRegisterValues, pTables->m_RegisterValues, sizeof(pTables->m_RegisterValues));
    memcpy(s_ShaderManager.m_pTables->m_DefaultRegisterMasks, s_ShaderManager.m_pTables->m_RegisterMasks, sizeof(pTables->m_RegisterMasks));
    memset(s_RegisterSet0, 0, sizeof(s_RegisterSet0));
    SetBits(s_RegisterSet0, s_RegisterList0);
    memset(s_RegisterSet1, 0, sizeof(s_RegisterSet1));
    SetBits(s_RegisterSet1, s_RegisterList1);
    SetBits(s_RegisterSet1, s_RegisterList2);
    memset(s_RegisterSet2, 0, sizeof(s_RegisterSet2));
    SetBits(s_RegisterSet2, s_RegisterList3);
    return result;
}
} // extern "C"

namespace {
// the types of the uniforms and inputs (the OpenGL ES values)
const GLenum TYPE_FLOAT_VEC2 = 0x8B50;
const GLenum TYPE_FLOAT_MAT2 = 0x8B5A;
const GLenum TYPE_FLOAT_MAT3 = 0x8B5B;
const GLenum TYPE_FLOAT_MAT4 = 0x8B5C;
// the register ranges of the uniform table of a DVLE
const u16 REGISTER_FIRST_FLOAT = 0x10;
const u16 REGISTER_FIRST_INT = 0x70;
const u16 REGISTER_FIRST_BOOL = 0x78;
// the constant types of a DVLE
const u16 CONSTANT_BOOL = 0;
const u16 CONSTANT_INT = 1;
const u16 CONSTANT_FLOAT = 2;
// the output types of a DVLE (3dbrew)
const u16 OUTPUT_TEXCOORD0 = 3;
const u16 OUTPUT_TEXCOORD0W = 4;
const u16 OUTPUT_TEXCOORD1 = 5;
const u16 OUTPUT_TEXCOORD2 = 6;
const u16 OUTPUT_VIEW = 8;
const u16 OUTPUT_DUMMY = 9;

inline s32 CountBits16(u16 mask)
{
    s32 count = 0;
    for (u32 i = 0; i < 16; i++) {
        if ((mask & (1 << i)) != 0) {
            count++;
        }
    }
    return count;
}

// the first semantic of an output type in the output map (3dbrew; -1 for the dummy)
inline s32 GetOutputSemantic(u16 type)
{
    switch (type) {
    case 0:
        return 0x00; // position
    case 1:
        return 0x04; // normal quaternion
    case 2:
        return 0x08; // color
    case 3:
        return 0x0C; // texcoord 0
    case 4:
        return 0x10; // texcoord 0 w
    case 5:
        return 0x0E; // texcoord 1
    case 6:
        return 0x16; // texcoord 2
    case 8:
        return 0x12; // view
    case OUTPUT_DUMMY:
        return -1;
    default:
        return 0;
    }
}

// the outputs the rasterizer needs for a semantic
inline u32 GetOutmapMaskBit(s32 semantic)
{
    switch (semantic) {
    case 0x02:
        return 0x1;
    case 0x04:
        return 0x1000000;
    case 0x08:
        return 0x2;
    case 0x0C:
        return 0x100;
    case 0x0E:
        return 0x200;
    case 0x10:
        return 0x10000;
    case 0x12:
        return 0x1000000;
    case 0x16:
        return 0x400;
    default:
        return 0;
    }
}

// the components a uniform name gives with a swizzle (".xy" = 2; none = 4); the position of the
// swizzle or -1
inline s32 CountSwizzleComponents(const char* pName, s32* pDot)
{
    s32 dot = -1;
    s32 components = 0;
    if (pName[0] == '\0') {
        *pDot = dot;
        return 4;
    }
    for (s32 i = 0; pName[i] != '\0'; i++) {
        char c = pName[i];
        if (c == '.') {
            dot = i;
        } else if (dot != -1) {
            if (c == 'x' || c == 'y' || c == 'z' || c == 'w') {
                components++;
            } else {
                dot = -1;
                components = 0;
            }
        }
    }
    *pDot = dot;
    return components != 0 ? components : 4;
}

// the executable of a DVLE (false if memory is missing)
inline bool ReadDvle(ShaderBinaryEntry* pEntry, const DvleHeader* pDvle)
{
    const u8* pBase = reinterpret_cast<const u8*>(pDvle);
    pEntry->m_ShaderType = pDvle->m_ShaderType;
    pEntry->m_MergeOutmaps = pDvle->m_MergeOutmaps & 1;
    pEntry->m_GeometryType = pDvle->m_GeometryType;
    pEntry->m_StartFloatRegister = pDvle->m_StartFloatRegister;
    pEntry->m_FullVertexCount = pDvle->m_FullVertexCount;
    pEntry->m_VariableVertexCount = pDvle->m_VariableVertexCount;
    pEntry->m_InputMask = pDvle->m_InputMask;
    pEntry->m_OutputMask = pDvle->m_OutputMask;
    pEntry->m_MainOffset = pDvle->m_MainOffset;
    pEntry->m_EndOffset = pDvle->m_EndOffset;

    // the constants
    const DvleConstant* pConstants = reinterpret_cast<const DvleConstant*>(pBase + pDvle->m_ConstantOffset);
    for (u32 i = 0; i < pDvle->m_ConstantCount; i++) {
        if (pConstants[i].m_Type == CONSTANT_FLOAT) {
            pEntry->m_FloatConstantCount++;
        }
    }
    if (pEntry->m_FloatConstantCount != 0) {
        pEntry->m_pFloatConstants = static_cast<u32*>(AllocateObject(pEntry->m_FloatConstantCount * 16));
        if (pEntry->m_pFloatConstants == nullptr) {
            return false;
        }
        pEntry->m_FloatConstantCount = 0;
    }
    for (u32 i = 0; i < pDvle->m_ConstantCount; i++) {
        const DvleConstant* p = &pConstants[i];
        switch (p->m_Type) {
        case CONSTANT_BOOL:
            if (p->m_Values[0] != 0) {
                pEntry->m_BoolConstants |= 1 << p->m_Id;
            }
            break;
        case CONSTANT_INT:
            if (p->m_Id < 4) {
                pEntry->m_IntConstants[p->m_Id] = p->m_Values[0];
            }
            break;
        case CONSTANT_FLOAT: {
            u32* pFloat = &pEntry->m_pFloatConstants[pEntry->m_FloatConstantCount * 4];
            pFloat[0] = p->m_Id;
            pFloat[1] = (p->m_Values[3] << 8) | (p->m_Values[2] >> 16);
            pFloat[2] = (p->m_Values[2] << 16) | (p->m_Values[1] >> 8);
            pFloat[3] = p->m_Values[0] | (p->m_Values[1] << 24);
            pEntry->m_FloatConstantCount++;
            break;
        }
        default:
            break;
        }
    }
    pEntry->m_InputCount = CountBits16(pDvle->m_InputMask);
    pEntry->m_OutputCount = CountBits16(pDvle->m_OutputMask);

    // the output map (the dummy outputs take no register)
    const DvleOutput* pOutputs = reinterpret_cast<const DvleOutput*>(pBase + pDvle->m_OutputOffset);
    u16 outputMask = pDvle->m_OutputMask;
    for (u32 i = 0; i < pDvle->m_OutputCount; i++) {
        if (pOutputs[i].m_Type == OUTPUT_DUMMY) {
            outputMask &= ~(1 << pOutputs[i].m_Register);
        }
    }
    for (u32 i = 0; i < 7; i++) {
        pEntry->m_Outmaps[i] = 0x1F1F1F1F;
    }
    for (u32 i = 0; i < pDvle->m_OutputCount; i++) {
        const DvleOutput* p = &pOutputs[i];
        s32 semantic = GetOutputSemantic(p->m_Type);
        if (semantic < 0) {
            continue;
        }
        s32 slot = 0;
        for (u32 j = 0; j < p->m_Register; j++) {
            if (((outputMask >> j) & 1) != 0) {
                slot++;
            }
        }
        s32 count = 0;
        for (s32 component = 0; component < 4; component++) {
            if ((p->m_Mask & (1 << component)) != 0) {
                pEntry->m_Outmaps[slot] &= ~(0xFF << (component * 8));
                pEntry->m_Outmaps[slot] |= semantic << (component * 8);
                count++;
                pEntry->m_OutmapMask |= GetOutmapMaskBit(semantic);
                semantic++;
            }
            // the texture coordinates have two components (w one), the view three
            if (((p->m_Type == OUTPUT_TEXCOORD0 || p->m_Type == OUTPUT_TEXCOORD1 || p->m_Type == OUTPUT_TEXCOORD2) && count == 2) ||
                (p->m_Type == OUTPUT_TEXCOORD0W && count == 1) || (p->m_Type == OUTPUT_VIEW && count == 3)) {
                break;
            }
        }
    }

    // the symbols
    pEntry->m_pSymbols = static_cast<char*>(AllocateObject(pDvle->m_SymbolSize));
    if (pEntry->m_pSymbols == nullptr) {
        return false;
    }
    memcpy(pEntry->m_pSymbols, pBase + pDvle->m_SymbolOffset, pDvle->m_SymbolSize);
    pEntry->m_SymbolSize = pDvle->m_SymbolSize;

    // the uniforms and inputs
    const DvleUniform* pUniforms = reinterpret_cast<const DvleUniform*>(pBase + pDvle->m_UniformOffset);
    for (u32 i = 0; i < pDvle->m_UniformCount; i++) {
        if (pUniforms[i].m_StartRegister >= REGISTER_FIRST_FLOAT) {
            pEntry->m_UniformCount++;
        }
    }
    if (pEntry->m_UniformCount != 0) {
        pEntry->m_pUniforms = static_cast<ShaderUniform*>(AllocateObject(pEntry->m_UniformCount * sizeof(ShaderUniform)));
        if (pEntry->m_pUniforms == nullptr) {
            return false;
        }
        pEntry->m_UniformCount = 0;
    }
    for (u32 i = 0; i < pDvle->m_UniformCount; i++) {
        const DvleUniform* p = &pUniforms[i];
        char* pName = pEntry->m_pSymbols + p->m_SymbolOffset;
        s32 dot;
        s32 components = CountSwizzleComponents(pName, &dot);
        if (p->m_EndRegister < REGISTER_FIRST_FLOAT) {
            // an input
            ShaderInput* pInput = &pEntry->m_Inputs[p->m_StartRegister];
            switch (p->m_EndRegister - p->m_StartRegister) {
            case 0:
                switch (components) {
                case 1:
                    pInput->m_Type = TYPE_FLOAT;
                    break;
                case 2:
                    pInput->m_Type = TYPE_FLOAT_VEC2;
                    break;
                case 3:
                    pInput->m_Type = TYPE_FLOAT_VEC3;
                    break;
                case 4:
                    pInput->m_Type = TYPE_FLOAT_VEC4;
                    break;
                default:
                    break;
                }
                break;
            case 1:
                pInput->m_Type = TYPE_FLOAT_MAT2;
                break;
            case 2:
                pInput->m_Type = TYPE_FLOAT_MAT3;
                break;
            case 3:
                pInput->m_Type = TYPE_FLOAT_MAT4;
                break;
            default:
                break;
            }
            pInput->m_NameOffset = p->m_SymbolOffset;
            if (dot != -1) {
                pName[dot] = '\0';
            }
            continue;
        }
        ShaderUniform* pUniform = &pEntry->m_pUniforms[pEntry->m_UniformCount];
        if (p->m_EndRegister < REGISTER_FIRST_INT) {
            // floats: vectors, or matrices of their rows
            s32 count = p->m_EndRegister - p->m_StartRegister + 1;
            pUniform->m_RegisterCount = count;
            switch (components) {
            case 1:
                pUniform->m_Type = TYPE_FLOAT;
                break;
            case 2:
                pUniform->m_Type = (count & 1) != 0 ? TYPE_FLOAT_VEC2 : TYPE_FLOAT_MAT2;
                break;
            case 3:
                pUniform->m_Type = count % 3 != 0 ? TYPE_FLOAT_VEC3 : TYPE_FLOAT_MAT3;
                break;
            case 4:
                pUniform->m_Type = (count & 3) != 0 ? TYPE_FLOAT_VEC4 : TYPE_FLOAT_MAT4;
                break;
            default:
                break;
            }
            pUniform->m_Register = p->m_StartRegister - REGISTER_FIRST_FLOAT;
            if (dot != -1) {
                switch (pName[dot + 1]) {
                case 'w':
                    pUniform->m_Component = 3;
                    break;
                case 'x':
                    pUniform->m_Component = 0;
                    break;
                case 'y':
                    pUniform->m_Component = 1;
                    break;
                case 'z':
                    pUniform->m_Component = 2;
                    break;
                default:
                    pUniform->m_Component = 0;
                    break;
                }
                pName[dot] = '\0';
            } else {
                pUniform->m_Component = 0;
            }
            pUniform->m_NameOffset = p->m_SymbolOffset;
        } else if (p->m_EndRegister < REGISTER_FIRST_BOOL) {
            pUniform->m_Type = TYPE_INT_VEC3;
            pUniform->m_Register = p->m_StartRegister - REGISTER_FIRST_INT;
            pUniform->m_Component = 0;
            pUniform->m_NameOffset = p->m_SymbolOffset;
            pUniform->m_RegisterCount = p->m_EndRegister - p->m_StartRegister + 1;
        } else {
            pUniform->m_Type = TYPE_BOOL;
            pUniform->m_Register = p->m_StartRegister - REGISTER_FIRST_BOOL;
            pUniform->m_Component = 0;
            pUniform->m_NameOffset = p->m_SymbolOffset;
            pUniform->m_RegisterCount = p->m_EndRegister - p->m_StartRegister + 1;
        }
        pEntry->m_UniformCount++;
    }
    return true;
}
} // namespace

// (shader i gets executable i of the DVLB; the format and the length are not checked)
// 0x007AC410 | nintendogs:bytes [tier B]
extern "C" void glShaderBinary(GLsizei n, const GLuint* pShaders, GLenum binaryFormat, const GLvoid* pBinary, GLsizei length)
{
    const DvlbHeader* pDvlb = static_cast<const DvlbHeader*>(pBinary);
    const DvlpHeader* pDvlp = reinterpret_cast<const DvlpHeader*>(reinterpret_cast<const u8*>(pBinary) + 8 + pDvlb->m_DvleCount * 4);
    const u8* pDvlpBase = reinterpret_cast<const u8*>(pDvlp);
    ShaderBinary* pShaderBinary = static_cast<ShaderBinary*>(AllocateObject(sizeof(ShaderBinary)));
    if (pShaderBinary == nullptr) {
        return;
    }
    memset(pShaderBinary, 0, sizeof(ShaderBinary));
    pShaderBinary->m_pCode = static_cast<u32*>(AllocateObject(pDvlp->m_CodeSize * 4));
    pShaderBinary->m_CodeSize = pDvlp->m_CodeSize;
    pShaderBinary->m_pOperandDescriptors = static_cast<u32*>(AllocateObject(pDvlp->m_OperandDescriptorCount * 4));
    pShaderBinary->m_OperandDescriptorCount = pDvlp->m_OperandDescriptorCount;
    if (pShaderBinary->m_pCode == nullptr || pShaderBinary->m_pOperandDescriptors == nullptr) {
        return;
    }
    const u32* pCode = reinterpret_cast<const u32*>(pDvlpBase + pDvlp->m_CodeOffset);
    for (u32 i = 0; i < pDvlp->m_CodeSize; i++) {
        pShaderBinary->m_pCode[i] = pCode[i];
    }
    const u32* pDescriptors = reinterpret_cast<const u32*>(pDvlpBase + pDvlp->m_OperandDescriptorOffset);
    for (u32 i = 0; i < pDvlp->m_OperandDescriptorCount; i++) {
        pShaderBinary->m_pOperandDescriptors[i] = pDescriptors[i * 2];
    }
    pShaderBinary->m_pEntries = static_cast<ShaderBinaryEntry*>(AllocateObject(pDvlb->m_DvleCount * sizeof(ShaderBinaryEntry)));
    pShaderBinary->m_EntryCount = pDvlb->m_DvleCount;
    if (pShaderBinary->m_pEntries == nullptr) {
        return;
    }
    memset(pShaderBinary->m_pEntries, 0, pDvlb->m_DvleCount * sizeof(ShaderBinaryEntry));
    for (u32 i = 0; i < pDvlb->m_DvleCount; i++) {
        const DvleHeader* pDvle = reinterpret_cast<const DvleHeader*>(reinterpret_cast<const u8*>(pBinary) + pDvlb->m_DvleOffsets[i]);
        if (!ReadDvle(&pShaderBinary->m_pEntries[i], pDvle)) {
            return;
        }
    }
    // the shaders share the binary (their old binaries are released)
    for (s32 i = 0; i < n; i++) {
        Shader* pShader = FindShader(pShaders[i]);
        if (pShader->m_pBinary != nullptr && --pShader->m_pBinary->m_ReferenceCount == 0) {
            ReleaseShaderBinary(pShader->m_pBinary);
        }
        pShader->m_EntryIndex = i;
        pShader->m_pBinary = pShaderBinary;
    }
    ShaderTables* pTables = s_ShaderManager.m_pTables;
    pShaderBinary->m_ReferenceCount = n;
    pShaderBinary->m_pPrevious = nullptr;
    pShaderBinary->m_pNext = pTables->m_pBinaries;
    if (pTables->m_pBinaries != nullptr) {
        pTables->m_pBinaries->m_pPrevious = pShaderBinary;
    }
    pTables->m_pBinaries = pShaderBinary;
}

namespace {
// the most uniforms of a program
const u32 MAX_PROGRAM_UNIFORMS = 2048;
// the bits of a uniform location and of its info (see UniformLocation)
const u32 LOCATION_RESERVED = 0x40000;
const u32 INFO_GEOMETRY = 0x400;
const u32 INFO_NOT_FLOAT = 0x10000;
// the outputs that need the second output map mode (texture coordinate 0 w, 1, 2)
const u32 OUTMAP_MASK_TEXCOORDS = 0x10700;
const u32 OUTMAP_UNUSED = 0x1F1F1F1F;
// the dirty flags of the program state when the program in use is linked again
const u32 DIRTY_PROGRAM_BINARY = 0x100000;
const u32 DIRTY_PROGRAM = 0x1E08000;
const u32 DIRTY_PROGRAM_REGISTERS = 0x200013C;
const u32 DIRTY_TEXTURE_UNIT_0 = 0x400;

// the float registers the uniforms of an executable use (not the integer and bool ones)
inline void CollectFloatRegisters(const ShaderBinaryEntry* pEntry, u32* pSet)
{
    for (s32 i = 0; i < pEntry->m_UniformCount; i++) {
        const ShaderUniform* p = &pEntry->m_pUniforms[i];
        if (p->m_Type == TYPE_INT_VEC3 || p->m_Type == TYPE_BOOL || p->m_RegisterCount == 0) {
            continue;
        }
        for (s32 j = 0; j < p->m_RegisterCount; j++) {
            u32 reg = p->m_Register + j;
            pSet[static_cast<s32>(reg) >> 5] |= 1 << (reg & 31);
        }
    }
}

// the registers of a set as a list
inline void ListRegisters(const u32* pSet, u32* pList, u32* pCount)
{
    for (u32 word = 0; word < 3; word++) {
        u32 bits = pSet[word];
        for (u32 bit = 0; bits != 0 && bit < 32; bit++, bits >>= 1) {
            if ((bits & 1) != 0) {
                pList[(*pCount)++] = bit + word * 32;
            }
        }
    }
}

// the info of a uniform of an executable (its first register as the slot in the list of the float
// registers)
inline u32 MakeUniformInfo(const ShaderUniform* p, u32 flags, const u32* pRegisters, u32 registerCount)
{
    u32 info = flags | ((p->m_Component << 11) & 0x1800) | ((p->m_RegisterCount << 17) & 0xFE0000);
    switch (p->m_Type) {
    case TYPE_FLOAT_VEC2:
        info = (info & ~0x6000) | 0x2000;
        break;
    case TYPE_FLOAT_VEC3:
        info = (info & ~0x6000) | 0x4000;
        break;
    case TYPE_FLOAT_VEC4:
        info |= 0x6000;
        break;
    case TYPE_INT_VEC3:
        info = (info & ~0xFF000000 & ~0x6000) | 0x14000 | (p->m_Register << 24);
        break;
    case TYPE_BOOL:
        info = (info & ~0xFF000000) | INFO_NOT_FLOAT | (p->m_Register << 24);
        break;
    case TYPE_FLOAT_MAT2:
        info = (info & ~0x6000) | 0x2000 | 0x8000;
        break;
    case TYPE_FLOAT_MAT3:
        info = (info & ~0x6000) | 0x4000 | 0x8000;
        break;
    case TYPE_FLOAT_MAT4:
        info |= 0xE000;
        break;
    default:
        break;
    }
    if ((info & INFO_NOT_FLOAT) == 0) {
        for (u32 i = 0; i < registerCount; i++) {
            if (p->m_Register == pRegisters[i]) {
                info = (info & ~0xFF000000) | (i << 24);
                break;
            }
        }
    }
    return info;
}

// a register of the program (its dirty bit if it changes)
inline void UpdateRegister(Program* pProgram, u32 index, u32 value)
{
    if (pProgram->m_RegisterValues[index] != value) {
        pProgram->m_RegisterValues[index] = value;
        pProgram->m_DirtyRegisters[index >> 5] |= 1 << (index & 31);
    }
}

// the low byte of a register
inline void UpdateRegisterByte0(Program* pProgram, u32 index, u32 value)
{
    u32 old = pProgram->m_RegisterValues[index];
    if ((old & 0xFF) != (value & 0xFF)) {
        pProgram->m_RegisterValues[index] = (old & ~0xFF) | (value & 0xFF);
        pProgram->m_DirtyRegisters[index >> 5] |= 1 << (index & 31);
    }
}

// bytes 1 and 3 of a register
inline void UpdateRegisterBytes13(Program* pProgram, u32 index, u32 value)
{
    u32 old = pProgram->m_RegisterValues[index];
    if ((old & 0xFF00FF00) != value) {
        pProgram->m_RegisterValues[index] = (old & 0x00FF00FF) | value;
        pProgram->m_DirtyRegisters[index >> 5] |= 1 << (index & 31);
    }
}

inline void SetReservedWord(Program* pProgram, u32 offset, u32 value)
{
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(pProgram) + offset) = value;
}

inline void SetReservedFloat(Program* pProgram, u32 offset, f32 value)
{
    *reinterpret_cast<f32*>(reinterpret_cast<u8*>(pProgram) + offset) = value;
}

// the values of the reserved uniforms at the start (offsets in the program)
inline void InitializeReservedValues(Program* pProgram)
{
    memset(pProgram->m_ReservedValues, 0, 0xF1C - 0x9EC);
    pProgram->m_ReservedValues[0] = true;
    for (u32 i = 0; i < 6; i++) {
        SetReservedWord(pProgram, 0x9F4 + i * 4, 0xFFFFFFFF);
    }
    SetReservedWord(pProgram, 0xA0C, 97);
    SetReservedFloat(pProgram, 0xA10, 0.2f);
    SetReservedFloat(pProgram, 0xA14, 0.2f);
    SetReservedFloat(pProgram, 0xA18, 0.2f);
    SetReservedFloat(pProgram, 0xA1C, 1.0f);
    for (u32 i = 0; i < 8; i++) {
        SetReservedFloat(pProgram, 0xA34 + i * 4, 1.0f);
    }
    // the eight lights
    for (u32 i = 0; i < 8; i++) {
        u32 light = i * 112;
        SetReservedFloat(pProgram, 0xA6C + light, 1.0f);
        SetReservedFloat(pProgram, 0xA7C + light, -1.0f);
        SetReservedWord(pProgram, 0xA80 + light, 0xFFFFFFFF);
        SetReservedWord(pProgram, 0xA8C + light, 0xFFFFFFFF);
        SetReservedFloat(pProgram, 0xA88 + light, 1.0f);
    }
    SetReservedFloat(pProgram, 0xDA0, 0.2f);
    SetReservedFloat(pProgram, 0xDB0, 0.8f);
    SetReservedFloat(pProgram, 0xDA4, 0.2f);
    SetReservedFloat(pProgram, 0xDB4, 0.8f);
    SetReservedFloat(pProgram, 0xDA8, 0.2f);
    SetReservedFloat(pProgram, 0xDB8, 0.8f);
    SetReservedFloat(pProgram, 0xDAC, 1.0f);
    SetReservedFloat(pProgram, 0xDBC, 1.0f);
    SetReservedFloat(pProgram, 0xDCC, 1.0f);
    SetReservedFloat(pProgram, 0xDDC, 1.0f);
    SetReservedFloat(pProgram, 0xDEC, 1.0f);
    for (u32 i = 0; i < 7; i++) {
        SetReservedWord(pProgram, 0xDF0 + i * 4, 0xFFFFFFFF);
    }
    SetReservedFloat(pProgram, 0xE00, 0.5f);
    pProgram->m_Mode = 0x6030;
    SetReservedFloat(pProgram, 0xE1C, 1.0f);
    SetReservedWord(pProgram, 0xE64, 0xFFFFFFFF);
    pProgram->m_IsGasAutoAcc = true;
    SetReservedWord(pProgram, 0xE78, 0xFFFFFFFF);
    SetReservedWord(pProgram, 0xE7C, 0xFFFFFFFF);
    SetReservedWord(pProgram, 0xE80, 0xFFFFFFFF);
    SetReservedFloat(pProgram, 0xEA4, 1.0f);
    SetReservedFloat(pProgram, 0xEA8, 1.0f);
    SetReservedFloat(pProgram, 0xEA0, 10.0f);
}

// the shader registers of the program (the registers after 3dbrew: entry points, bool and integer
// uniforms, attribute and output counts, output maps, geometry shader mode)
inline void SetProgramRegisters(Program* pProgram, const ShaderBinaryEntry* pVertex, const ShaderBinaryEntry* pGeometry, const u32* pOutmaps,
                                u32 outputMask, u32 outputCount)
{
    UpdateRegister(pProgram, 15, pVertex->m_MainOffset | 0x7FFF0000);
    pProgram->m_RegisterMasks[15] = 0xF;
    UpdateRegister(pProgram, 9, pVertex->m_BoolConstants | 0x7FFF0000);
    pProgram->m_RegisterMasks[9] = 0xF;
    for (u32 i = 0; i < 4; i++) {
        UpdateRegister(pProgram, 10 + i, pVertex->m_IntConstants[i]);
        pProgram->m_RegisterMasks[10 + i] = 0xF;
    }
    UpdateRegister(pProgram, 16, pVertex->m_OutputMask);
    pProgram->m_RegisterMasks[16] = 0xF;
    UpdateRegisterByte0(pProgram, 14, pVertex->m_InputCount - 1);
    pProgram->m_RegisterMasks[14] |= 0x1;
    if (pProgram->m_pGeometryShader != nullptr) {
        UpdateRegisterByte0(pProgram, 6, pVertex->m_OutputCount - 1);
        pProgram->m_RegisterMasks[6] |= 0x1;
        UpdateRegister(pProgram, 7, pGeometry->m_MainOffset | 0x7FFF0000);
        pProgram->m_RegisterMasks[7] = 0xF;
        UpdateRegister(pProgram, 1, pGeometry->m_BoolConstants | 0x7FFF0000);
        pProgram->m_RegisterMasks[1] = 0xF;
        for (u32 i = 0; i < 4; i++) {
            UpdateRegister(pProgram, 2 + i, pGeometry->m_IntConstants[i]);
            pProgram->m_RegisterMasks[2 + i] = 0xF;
        }
        UpdateRegister(pProgram, 8, outputMask);
        pProgram->m_RegisterMasks[8] = 0xF;
        UpdateRegister(pProgram, 25, outputCount);
        pProgram->m_RegisterMasks[25] = 0xF;
        for (u32 i = 0; i < 7; i++) {
            UpdateRegister(pProgram, 26 + i, pOutmaps[i]);
            pProgram->m_RegisterMasks[26 + i] = 0xF;
        }
        UpdateRegister(pProgram, 40, pProgram->m_OutmapMask);
        pProgram->m_RegisterMasks[40] = 0xF;
        UpdateRegister(pProgram, 38, (pProgram->m_OutmapMask & OUTMAP_MASK_TEXCOORDS) != 0 ? 1 : 0);
        pProgram->m_RegisterMasks[38] = 0xF;
        switch (pGeometry->m_GeometryType) {
        case 0:
            if ((pProgram->m_RegisterValues[0] & 0xFF000000) != 0) {
                UpdateRegister(pProgram, 0, pProgram->m_RegisterValues[0] & ~0xFF000000);
            }
            pProgram->m_RegisterMasks[0] |= 0x8;
            UpdateRegister(pProgram, 18, 0);
            pProgram->m_RegisterMasks[18] = 0xF;
            UpdateRegisterBytes13(pProgram, 6, 0x08000000);
            pProgram->m_RegisterMasks[6] |= 0xA;
            break;
        case 1:
            if ((pProgram->m_RegisterValues[0] & 0xFF000000) != 0x80000000) {
                UpdateRegister(pProgram, 0, (pProgram->m_RegisterValues[0] & ~0xFF000000) | 0x80000000);
            }
            pProgram->m_RegisterMasks[0] |= 0x8;
            UpdateRegister(pProgram, 18, 1);
            pProgram->m_RegisterMasks[18] = 0xF;
            UpdateRegisterBytes13(pProgram, 6, 0x08000100);
            pProgram->m_RegisterMasks[6] |= 0xA;
            UpdateRegister(pProgram, 19, pGeometry->m_FullVertexCount - 1);
            pProgram->m_RegisterMasks[19] = 0xF;
            break;
        case 2:
            if ((pProgram->m_RegisterValues[0] & 0xFF000000) != 0) {
                UpdateRegister(pProgram, 0, pProgram->m_RegisterValues[0] & ~0xFF000000);
            }
            pProgram->m_RegisterMasks[0] |= 0x8;
            UpdateRegister(pProgram, 18, ((pVertex->m_OutputCount - 1) << 12) | ((pGeometry->m_VariableVertexCount - 1) << 8) |
                                             (pGeometry->m_StartFloatRegister << 16) | 0x1000000 | 2);
            pProgram->m_RegisterMasks[18] = 0xF;
            UpdateRegisterBytes13(pProgram, 6, 0x08000100);
            pProgram->m_RegisterMasks[6] |= 0xA;
            break;
        default:
            break;
        }
        UpdateRegister(pProgram, 21, pVertex->m_OutputCount - 1);
        pProgram->m_RegisterMasks[21] = 0xF;
        UpdateRegisterByte0(pProgram, 22, outputCount - 1);
        pProgram->m_RegisterMasks[22] |= 0x1;
        UpdateRegister(pProgram, 17, pVertex->m_OutputCount - 1);
        pProgram->m_RegisterMasks[17] = 0xF;
        UpdateRegister(pProgram, 20, pVertex->m_InputCount - 1);
        pProgram->m_RegisterMasks[20] = 0xF;
        UpdateRegisterBytes13(pProgram, 14, 0xA0000000);
        pProgram->m_RegisterMasks[14] |= 0xA;
    } else {
        UpdateRegister(pProgram, 25, pVertex->m_OutputCount);
        pProgram->m_RegisterMasks[25] = 0xF;
        for (u32 i = 0; i < 7; i++) {
            UpdateRegister(pProgram, 26 + i, pOutmaps[i]);
            pProgram->m_RegisterMasks[26 + i] = 0xF;
        }
        UpdateRegister(pProgram, 40, pProgram->m_OutmapMask);
        pProgram->m_RegisterMasks[40] = 0xF;
        UpdateRegister(pProgram, 38, (pProgram->m_OutmapMask & OUTMAP_MASK_TEXCOORDS) != 0 ? 1 : 0);
        pProgram->m_RegisterMasks[38] = 0xF;
        if ((pProgram->m_RegisterValues[0] & 0xFF000000) != 0) {
            UpdateRegister(pProgram, 0, pProgram->m_RegisterValues[0] & ~0xFF000000);
        }
        pProgram->m_RegisterMasks[0] |= 0x8;
        UpdateRegister(pProgram, 18, 0);
        pProgram->m_RegisterMasks[18] = 0xF;
        UpdateRegisterBytes13(pProgram, 14, 0xA0000000);
        pProgram->m_RegisterMasks[14] |= 0xA;
        UpdateRegister(pProgram, 21, pVertex->m_OutputCount - 1);
        pProgram->m_RegisterMasks[21] = 0xF;
        UpdateRegisterByte0(pProgram, 22, pVertex->m_OutputCount - 1);
        pProgram->m_RegisterMasks[22] |= 0x1;
        UpdateRegister(pProgram, 17, pVertex->m_OutputCount - 1);
        pProgram->m_RegisterMasks[17] = 0xF;
        UpdateRegister(pProgram, 20, pVertex->m_InputCount - 1);
        pProgram->m_RegisterMasks[20] = 0xF;
    }
}
} // namespace

// (the uniform locations, the attribute bindings and the shader registers of the program; the
// vertex shader and the geometry shader must come from one binary)
// 0x007AA83C | nintendogs:bytes [tier A]
extern "C" void glLinkProgram(GLuint program)
{
    Program* pProgram = FindProgram(program);
    pProgram->m_IsLinked = false;
    if (pProgram->m_IsDeletePending) {
        return;
    }
    Shader* pVertexShader = pProgram->m_pVertexShader;
    if (pVertexShader == nullptr || pProgram->m_Unknown0x08 == 0 || pVertexShader->m_pBinary == nullptr) {
        return;
    }
    Shader* pGeometryShader = pProgram->m_pGeometryShader;
    if (pGeometryShader != nullptr && pGeometryShader->m_pBinary != pVertexShader->m_pBinary) {
        return;
    }
    const ShaderBinaryEntry* pVertex = &pVertexShader->m_pBinary->m_pEntries[pVertexShader->m_EntryIndex];
    const ShaderBinaryEntry* pGeometry = nullptr;
    if (pGeometryShader != nullptr) {
        pGeometry = &pGeometryShader->m_pBinary->m_pEntries[pGeometryShader->m_EntryIndex];
    }

    // the float registers of both executables
    Program work;
    work.m_pGeometryFloatValues = nullptr;
    work.m_GeometryFloatRegisterCount = 0;
    work.m_pLocations = nullptr;
    work.m_VertexFloatRegisterCount = 0;
    work.m_pVertexFloatValues = nullptr;
    work.m_VertexUniformCount = 0;
    u32 vertexSet[3] = {0, 0, 0};
    u32 geometrySet[3] = {0, 0, 0};
    CollectFloatRegisters(pVertex, vertexSet);
    ListRegisters(vertexSet, work.m_VertexFloatRegisters, &work.m_VertexFloatRegisterCount);
    work.m_LocationCount = pVertex->m_UniformCount;
    if (work.m_VertexFloatRegisterCount != 0) {
        work.m_pVertexFloatValues = static_cast<u32*>(AllocateObject(work.m_VertexFloatRegisterCount * 16));
        memset(work.m_pVertexFloatValues, 0, work.m_VertexFloatRegisterCount * 16);
    }
    if (pProgram->m_pGeometryShader != nullptr) {
        CollectFloatRegisters(pGeometry, geometrySet);
        ListRegisters(geometrySet, work.m_GeometryFloatRegisters, &work.m_GeometryFloatRegisterCount);
        work.m_VertexUniformCount = work.m_LocationCount;
        work.m_LocationCount = pGeometry->m_UniformCount + work.m_VertexUniformCount;
        if (work.m_GeometryFloatRegisterCount != 0) {
            work.m_pGeometryFloatValues = static_cast<u32*>(AllocateObject(work.m_GeometryFloatRegisterCount * 16));
            memset(work.m_pGeometryFloatValues, 0, work.m_GeometryFloatRegisterCount * 16);
        }
    }

    // the location table (the uniforms of the executables, then the reserved ones)
    if (work.m_LocationCount <= MAX_PROGRAM_UNIFORMS) {
        work.m_UniformCount = work.m_LocationCount;
        work.m_LocationCount += RESERVED_UNIFORM_COUNT;
        work.m_pLocations = static_cast<UniformLocation*>(AllocateObject(work.m_LocationCount * sizeof(UniformLocation)));
        if (work.m_pLocations == nullptr) {
            goto fail;
        }
        memset(work.m_pLocations, 0, work.m_LocationCount * sizeof(UniformLocation));
    }
    if (work.m_pLocations == nullptr) {
        goto fail;
    }
    FreeBuffer(pProgram->m_pLocations);
    pProgram->m_pLocations = nullptr;
    FreeBuffer(pProgram->m_pVertexFloatValues);
    pProgram->m_pVertexFloatValues = nullptr;
    pProgram->m_VertexFloatRegisterCount = 0;
    pProgram->m_VertexDirtyFloatRegisters[0] = 0;
    pProgram->m_VertexDirtyFloatRegisters[1] = 0;
    pProgram->m_VertexDirtyFloatRegisters[2] = 0;
    FreeBuffer(pProgram->m_pGeometryFloatValues);
    pProgram->m_pGeometryFloatValues = nullptr;
    pProgram->m_GeometryFloatRegisterCount = 0;
    pProgram->m_GeometryDirtyFloatRegisters[0] = 0;
    pProgram->m_GeometryDirtyFloatRegisters[1] = 0;
    pProgram->m_GeometryDirtyFloatRegisters[2] = 0;
    for (u32 i = 0; i < 16; i++) {
        pProgram->m_Bindings[i].m_Register = -1;
    }
    pProgram->m_VertexFloatRegisterCount = work.m_VertexFloatRegisterCount;
    for (u32 i = 0; i < pProgram->m_VertexFloatRegisterCount; i++) {
        pProgram->m_VertexFloatRegisters[i] = work.m_VertexFloatRegisters[i];
    }
    pProgram->m_pVertexFloatValues = work.m_pVertexFloatValues;
    pProgram->m_GeometryFloatRegisterCount = work.m_GeometryFloatRegisterCount;
    for (u32 i = 0; i < pProgram->m_GeometryFloatRegisterCount; i++) {
        pProgram->m_GeometryFloatRegisters[i] = work.m_GeometryFloatRegisters[i];
    }
    pProgram->m_pGeometryFloatValues = work.m_pGeometryFloatValues;
    {
        u32 base = program << 19;
        pProgram->m_VertexUniformCount = work.m_VertexUniformCount;
        pProgram->m_UniformCount = work.m_UniformCount;
        pProgram->m_LocationCount = work.m_LocationCount;
        pProgram->m_pLocations = work.m_pLocations;
        for (s32 i = 0; i < pVertex->m_UniformCount; i++) {
            const ShaderUniform* p = &pVertex->m_pUniforms[i];
            UniformLocation* pLocation = &pProgram->m_pLocations[i];
            pLocation->m_Location = base | ((i << 7) & 0x3FFFF);
            pLocation->m_NameOffset = p->m_NameOffset;
            pLocation->m_Info = MakeUniformInfo(p, 0, pProgram->m_VertexFloatRegisters, pProgram->m_VertexFloatRegisterCount);
        }
        if (pProgram->m_pGeometryShader != nullptr) {
            for (s32 i = 0; i < pGeometry->m_UniformCount; i++) {
                const ShaderUniform* p = &pGeometry->m_pUniforms[i];
                u32 index = pProgram->m_VertexUniformCount + i;
                UniformLocation* pLocation = &pProgram->m_pLocations[index];
                pLocation->m_Location = base | ((index << 7) & 0x3FFFF);
                pLocation->m_NameOffset = p->m_NameOffset;
                pLocation->m_Info = MakeUniformInfo(p, INFO_GEOMETRY, pProgram->m_GeometryFloatRegisters, pProgram->m_GeometryFloatRegisterCount);
            }
        }
        for (u32 i = pProgram->m_UniformCount; i < pProgram->m_LocationCount; i++) {
            u32 index = i - pProgram->m_UniformCount;
            u32 location = (static_cast<u16>(index) << 2) | (program << 19) | LOCATION_RESERVED;
            switch (s_ReservedUniforms[index].m_Type) {
            case TYPE_FLOAT_VEC2:
            case TYPE_INT_VEC2:
                location = (location & ~3) | 1;
                break;
            case TYPE_FLOAT_VEC3:
            case TYPE_INT_VEC3:
                location = (location & ~3) | 2;
                break;
            case TYPE_FLOAT_VEC4:
            case 0x8B55: // GL_INT_VEC4
                location |= 3;
                break;
            default:
                location &= ~3;
                break;
            }
            pProgram->m_pLocations[i].m_Location = location;
            pProgram->m_pLocations[i].m_NameOffset = index;
            pProgram->m_pLocations[i].m_Info = 0;
        }
    }

    // the inputs: first the bindings of glBindAttribLocation, then the free slots
    {
        u32 bound = 0;
        for (ProgramListNode* pNode = pProgram->m_pList; pNode != nullptr; pNode = pNode->m_pNext) {
            for (u32 i = 0; i < 16; i++) {
                const ShaderInput* pInput = &pVertex->m_Inputs[i];
                if (pInput->m_Type != 0 && strcmp(pVertex->m_pSymbols + pInput->m_NameOffset, pNode->m_pName) == 0) {
                    bound |= 1 << i;
                    pProgram->m_Bindings[pNode->m_Index].m_Register = i;
                    pProgram->m_Bindings[pNode->m_Index].m_Type = pInput->m_Type;
                    pProgram->m_Bindings[pNode->m_Index].m_NameOffset = pInput->m_NameOffset;
                    break;
                }
            }
        }
        for (u32 i = 0; i < 16; i++) {
            const ShaderInput* pInput = &pVertex->m_Inputs[i];
            if (pInput->m_Type == 0 || (bound & (1 << i)) != 0) {
                continue;
            }
            for (u32 j = 0; j < 16; j++) {
                if (pProgram->m_Bindings[j].m_Register == -1) {
                    bound |= 1 << i;
                    pProgram->m_Bindings[j].m_Register = i;
                    pProgram->m_Bindings[j].m_Type = pInput->m_Type;
                    pProgram->m_Bindings[j].m_NameOffset = pInput->m_NameOffset;
                    break;
                }
            }
        }
    }

    // the output maps (of a geometry shader merged with those of the vertex shader)
    {
        // (one more: the vertex outputs are added once more after seven geometry outputs)
        u32 outmaps[8];
        u32 outputCount = 0;
        u32 outputMask = 0;
        if (pProgram->m_pGeometryShader == nullptr) {
            for (u32 i = 0; i < 7; i++) {
                outmaps[i] = pVertex->m_Outmaps[i];
            }
            pProgram->m_OutmapMask = pVertex->m_OutmapMask;
        } else if (!pGeometry->m_MergeOutmaps) {
            for (u32 i = 0; i < 7; i++) {
                outmaps[i] = pGeometry->m_Outmaps[i];
            }
            outputMask = pGeometry->m_OutputMask;
            outputCount = pGeometry->m_OutputCount;
            pProgram->m_OutmapMask = pGeometry->m_OutmapMask;
        } else {
            u32 usedGeometry = 0;
            u32 usedVertex = 0;
            u32 count = 0;
            for (u32 i = 0; i < 7; i++) {
                u32 value = pGeometry->m_Outmaps[i];
                if (value == OUTMAP_UNUSED) {
                    break;
                }
                for (u32 j = 0; j < 7; j++) {
                    if (value == pVertex->m_Outmaps[j]) {
                        outmaps[count++] = value;
                        usedGeometry |= 1 << i;
                        usedVertex |= 1 << j;
                        break;
                    }
                }
            }
            for (u32 i = 0; i < 7; i++) {
                u32 value = pGeometry->m_Outmaps[i];
                if (value == OUTMAP_UNUSED) {
                    break;
                }
                if ((usedGeometry & (1 << i)) == 0) {
                    outmaps[count++] = value;
                    if (count == 7) {
                        break;
                    }
                }
            }
            for (u32 i = 0; i < 7; i++) {
                u32 value = pVertex->m_Outmaps[i];
                if (value == OUTMAP_UNUSED) {
                    break;
                }
                if ((usedVertex & (1 << i)) == 0) {
                    outmaps[count++] = value;
                    if (count == 7) {
                        break;
                    }
                }
            }
            for (u32 i = count; i < 7; i++) {
                outmaps[i] = OUTMAP_UNUSED;
            }
            // the outputs of the geometry shader up to that count
            u32 mask = pGeometry->m_OutputMask;
            u32 bits = 0;
            outputCount = count;
            for (u32 rest = count; rest != 0 && mask != 0; mask >>= 1, bits++) {
                if ((mask & 1) != 0) {
                    rest--;
                }
            }
            outputMask = pGeometry->m_OutputMask & ((1 << bits) - 1);
            pProgram->m_OutmapMask = pGeometry->m_OutmapMask | pVertex->m_OutmapMask;
        }
        ShaderTables* pTables = s_ShaderManager.m_pTables;
        memcpy(pProgram->m_RegisterValues, pTables->m_RegisterValues, sizeof(pProgram->m_RegisterValues));
        memcpy(pProgram->m_RegisterMasks, s_ShaderManager.m_pTables->m_RegisterMasks, sizeof(pProgram->m_RegisterMasks));
        InitializeReservedValues(pProgram);
        SetProgramRegisters(pProgram, pVertex, pGeometry, outmaps, outputMask, outputCount);
    }

    pProgram->m_WasDirty = pProgram->m_IsDirty;
    pProgram->m_IsLinked = true;
    pProgram->m_IsDirty = false;
    if (pProgram->m_pGeometryShader != nullptr) {
        pProgram->m_HasGeometryShader = true;
        pProgram->m_GeometryEntryIndex = pProgram->m_pGeometryShader->m_EntryIndex;
    } else {
        pProgram->m_HasGeometryShader = false;
    }
    pProgram->m_VertexEntryIndex = pProgram->m_pVertexShader->m_EntryIndex;
    if (s_ShaderManager.m_pTables->m_pCurrentProgram == pProgram) {
        // the program in use: all of its state again
        GlState* pState = s_pGlState;
        if (pProgram->m_pLinkedBinary != pProgram->m_pVertexShader->m_pBinary) {
            pState->m_DirtyFlags |= DIRTY_PROGRAM_BINARY;
        }
        pState->m_DirtyFlags |= DIRTY_PROGRAM;
        pProgram->m_VertexDirtyFloatRegisters[0] = 0xFFFFFFFF;
        pProgram->m_VertexDirtyFloatRegisters[1] = 0xFFFFFFFF;
        pProgram->m_VertexDirtyFloatRegisters[2] = 0xFFFFFFFF;
        if (pProgram->m_pGeometryShader != nullptr) {
            pProgram->m_GeometryDirtyFloatRegisters[0] = 0xFFFFFFFF;
            pProgram->m_GeometryDirtyFloatRegisters[1] = 0xFFFFFFFF;
            pProgram->m_GeometryDirtyFloatRegisters[2] = 0xFFFFFFFF;
        }
        for (u32 i = 0; i < 6; i++) {
            pProgram->m_DirtyRegisters[i] = 0xFFFFFFFF;
        }
        pState->m_DirtyFlags |= DIRTY_PROGRAM_REGISTERS;
        for (u32 unit = 0; unit < TEXTURE_UNIT_COUNT; unit++) {
            if (pState->m_SamplerTypes[unit] != pProgram->m_SamplerTypes[unit]) {
                pState->m_IsTexture2dEnabled[unit] = false;
                if (unit == 0) {
                    pState->m_IsTextureCubeEnabled[0] = false;
                }
                pState->m_DirtyFlags |= DIRTY_TEXTURE_UNIT_0 << unit;
                pState->m_SamplerTypes[unit] = 0;
            }
        }
        pProgram->m_WasDirty = false;
        pState->m_HasGeometryShader = pProgram->m_HasGeometryShader;
    }
    pProgram->m_pLinkedBinary = pProgram->m_pVertexShader->m_pBinary;
    return;

fail:
    if (work.m_pGeometryFloatValues != nullptr && s_Allocator.m_pFree != nullptr) {
        s_Allocator.m_pFree(MEMORY_AREA_MAIN, MEMORY_PURPOSE_OBJECT, 0, work.m_pGeometryFloatValues);
    }
    if (work.m_pVertexFloatValues != nullptr && s_Allocator.m_pFree != nullptr) {
        s_Allocator.m_pFree(MEMORY_AREA_MAIN, MEMORY_PURPOSE_OBJECT, 0, work.m_pVertexFloatValues);
    }
}

// (an existing binding of the name gets the new index)
// 0x007A7828 | nintendogs:bytes [tier B]
extern "C" void glBindAttribLocation(GLuint program, GLuint index, const GLchar* pName)
{
    Program* pProgram = FindProgram(program);
    ProgramListNode* pNode = static_cast<ProgramListNode*>(AllocateObject(sizeof(ProgramListNode)));
    pNode->m_pName = static_cast<char*>(AllocateObject(strlen(pName) + 1));
    for (ProgramListNode* p = pProgram->m_pList; p != nullptr; p = p->m_pNext) {
        if (strcmp(p->m_pName, pName) == 0) {
            FreeObject(pNode->m_pName);
            FreeObject(pNode);
            p->m_Index = index;
            return;
        }
    }
    strcpy(pNode->m_pName, pName);
    pNode->m_Index = index;
    pNode->m_pNext = pProgram->m_pList;
    pProgram->m_pList = pNode;
}

// (the uniforms of the vertex shader, of the geometry shader, then the reserved ones; -1 if none
// has the name)
// 0x007AA6C4 | nintendogs:bytes [tier B]
extern "C" GLint glGetUniformLocation(GLuint program, const GLchar* pName)
{
    Program* pProgram = FindProgram(program);
    u32 i;
    for (i = 0; i < pProgram->m_LocationCount; i++) {
        const char* pUniformName;
        const ShaderBinaryEntry* pEntries = pProgram->m_pLinkedBinary->m_pEntries;
        if (pProgram->m_HasGeometryShader && i < pProgram->m_VertexUniformCount) {
            pUniformName = pEntries[pProgram->m_VertexEntryIndex].m_pSymbols + pProgram->m_pLocations[i].m_NameOffset;
        } else if (i < pProgram->m_UniformCount) {
            u32 entry = pProgram->m_HasGeometryShader ? pProgram->m_GeometryEntryIndex : pProgram->m_VertexEntryIndex;
            pUniformName = pEntries[entry].m_pSymbols + pProgram->m_pLocations[i].m_NameOffset;
        } else {
            pUniformName = s_ReservedUniforms[i - pProgram->m_UniformCount].m_pName;
        }
        if (strcmp(pName, pUniformName) == 0) {
            break;
        }
    }
    return i != pProgram->m_LocationCount ? pProgram->m_pLocations[i].m_Location : -1;
}

namespace {
// the dirty flags of glUseProgram
const u32 DIRTY_PROGRAM_CHANGED = 0x8000;
const u32 DIRTY_PROGRAM_ALL = 0x1F00000;
const u32 DIRTY_VERTEX_ENTRY = 0x200000;
const u32 DIRTY_GEOMETRY_ENTRY = 0x400000;
const u32 DIRTY_GEOMETRY_ADDED = 0xC00000;
const u32 DIRTY_GEOMETRY_REMOVED = 0x1200000;
const u32 DIRTY_TEXTURE_UNIT_1 = 0x800;
const u32 DIRTY_TEXTURE_UNIT_2 = 0x1000;
// the sampler types (the uniforms dmp_Texture[i].samplerType; the OpenGL ES values, 0x6E00-0x6E02
// extensions)
const GLenum SAMPLER_TEXTURE_2D = 0x0DE1;
const GLenum SAMPLER_0x6E00 = 0x6E00;
const GLenum SAMPLER_0x6E01 = 0x6E01;
const GLenum SAMPLER_0x6E02 = 0x6E02;
const GLenum SAMPLER_TEXTURE_CUBE_MAP = 0x8513;

inline void SetFlag(GlState* pState, bool* pFlag, bool value, u32 dirty)
{
    if (*pFlag != value) {
        *pFlag = value;
        pState->m_DirtyFlags |= dirty;
    }
}

// all registers and values of the program go to the GPU again
inline void MarkProgramDirty(Program* pProgram)
{
    pProgram->m_VertexDirtyFloatRegisters[0] = 0xFFFFFFFF;
    pProgram->m_VertexDirtyFloatRegisters[1] = 0xFFFFFFFF;
    pProgram->m_VertexDirtyFloatRegisters[2] = 0xFFFFFFFF;
    if (pProgram->m_HasGeometryShader) {
        pProgram->m_GeometryDirtyFloatRegisters[0] = 0xFFFFFFFF;
        pProgram->m_GeometryDirtyFloatRegisters[1] = 0xFFFFFFFF;
        pProgram->m_GeometryDirtyFloatRegisters[2] = 0xFFFFFFFF;
    }
    for (u32 i = 0; i < 6; i++) {
        pProgram->m_DirtyRegisters[i] = 0xFFFFFFFF;
    }
}
} // namespace

// (program 0: none; a program deleted while in use is destroyed when another one replaces it)
// 0x007ADF48 | nintendogs:bytes [tier B]
extern "C" void glUseProgram(GLuint program)
{
    ShaderTables* pTables = s_ShaderManager.m_pTables;
    GlState* pState = s_pGlState;
    if (program == 0) {
        Program* pCurrent = pTables->m_pCurrentProgram;
        if (pCurrent != nullptr && pCurrent->m_IsDeletePending) {
            DestroyProgram(pCurrent->m_Name);
            s_ShaderManager.m_pTables->m_pAttributeProgram = nullptr;
        }
        s_ShaderManager.m_pTables->m_pCurrentProgram = nullptr;
        pState->m_ProgramName = 0;
        pState->m_HasGeometryShader = false;
        return;
    }
    Program* pProgram = FindProgram(program);
    Program* pCurrent = pTables->m_pCurrentProgram;
    u32 dirty = pState->m_DirtyFlags;
    if (pCurrent != pProgram) {
        dirty |= DIRTY_PROGRAM_CHANGED;
    }
    if (pCurrent == nullptr || pProgram->m_pLinkedBinary != pCurrent->m_pLinkedBinary) {
        dirty |= DIRTY_PROGRAM_ALL;
    } else {
        if (pProgram->m_VertexEntryIndex != pCurrent->m_VertexEntryIndex) {
            dirty |= DIRTY_VERTEX_ENTRY;
        }
        if (pProgram->m_HasGeometryShader) {
            if (!pCurrent->m_HasGeometryShader) {
                dirty |= DIRTY_GEOMETRY_ADDED;
            } else if (pProgram->m_GeometryEntryIndex != pCurrent->m_GeometryEntryIndex) {
                dirty |= DIRTY_GEOMETRY_ENTRY;
            }
        } else if (pCurrent->m_HasGeometryShader) {
            dirty |= DIRTY_GEOMETRY_REMOVED;
        }
    }
    pState->m_DirtyFlags = dirty;
    pCurrent = pTables->m_pCurrentProgram;
    if (pCurrent == nullptr || (pCurrent == pProgram && pProgram->m_WasDirty)) {
        MarkProgramDirty(pProgram);
        pState->m_DirtyFlags |= DIRTY_PROGRAM_REGISTERS;
    } else if (pCurrent != pProgram) {
        MarkProgramDirty(pProgram);
        if ((pProgram->m_RegisterValues[43] & 1) != 0) {
            pState->m_DirtyFlags |= 0x8;
        }
        if (*reinterpret_cast<u32*>(reinterpret_cast<u8*>(pProgram) + 0xE0C) != 0) {
            pState->m_DirtyFlags |= 0x20;
        }
        if ((pProgram->m_RegisterValues[80] & 1) != 0) {
            pState->m_DirtyFlags |= 0x10;
        }
        if ((pProgram->m_RegisterValues[80] & 2) != 0) {
            pState->m_DirtyFlags |= 0x2000000;
        }
        if (pState->m_IsDepthTestEnabled) {
            pState->m_DirtyFlags |= 0x100;
        }
        if (*reinterpret_cast<f32*>(reinterpret_cast<u8*>(pProgram) + 0xE4C) == 0.0f) {
            pState->m_DirtyFlags |= 0x4;
        }
    }
    // the texture units follow the sampler types of the program
    switch (pProgram->m_SamplerTypes[0]) {
    case 0:
        SetFlag(pState, &pState->m_IsTexture2dEnabled[0], false, DIRTY_TEXTURE_UNIT_0);
        SetFlag(pState, &pState->m_IsTextureCubeEnabled[0], false, DIRTY_TEXTURE_UNIT_0);
        break;
    case SAMPLER_TEXTURE_2D:
    case SAMPLER_0x6E00:
    case SAMPLER_0x6E01:
        SetFlag(pState, &pState->m_IsTexture2dEnabled[0], true, DIRTY_TEXTURE_UNIT_0);
        SetFlag(pState, &pState->m_IsTextureCubeEnabled[0], false, DIRTY_TEXTURE_UNIT_0);
        break;
    case SAMPLER_0x6E02:
    case SAMPLER_TEXTURE_CUBE_MAP:
        SetFlag(pState, &pState->m_IsTexture2dEnabled[0], false, DIRTY_TEXTURE_UNIT_0);
        SetFlag(pState, &pState->m_IsTextureCubeEnabled[0], true, DIRTY_TEXTURE_UNIT_0);
        break;
    default:
        break;
    }
    if (pState->m_SamplerTypes[0] != pProgram->m_SamplerTypes[0]) {
        pState->m_SamplerTypes[0] = pProgram->m_SamplerTypes[0];
        pState->m_DirtyFlags |= DIRTY_TEXTURE_UNIT_0;
    }
    if (pProgram->m_SamplerTypes[1] == 0) {
        SetFlag(pState, &pState->m_IsTexture2dEnabled[1], false, DIRTY_TEXTURE_UNIT_1);
    } else if (pProgram->m_SamplerTypes[1] == SAMPLER_TEXTURE_2D) {
        SetFlag(pState, &pState->m_IsTexture2dEnabled[1], true, DIRTY_TEXTURE_UNIT_1);
    }
    if (pState->m_SamplerTypes[1] != pProgram->m_SamplerTypes[1]) {
        pState->m_SamplerTypes[1] = pProgram->m_SamplerTypes[1];
        pState->m_DirtyFlags |= DIRTY_TEXTURE_UNIT_1;
    }
    if (pProgram->m_SamplerTypes[2] == 0) {
        SetFlag(pState, &pState->m_IsTexture2dEnabled[2], false, DIRTY_TEXTURE_UNIT_2);
    } else if (pProgram->m_SamplerTypes[2] == SAMPLER_TEXTURE_2D) {
        SetFlag(pState, &pState->m_IsTexture2dEnabled[2], true, DIRTY_TEXTURE_UNIT_2);
    }
    if (pState->m_SamplerTypes[2] != pProgram->m_SamplerTypes[2]) {
        pState->m_SamplerTypes[2] = pProgram->m_SamplerTypes[2];
        pState->m_DirtyFlags |= DIRTY_TEXTURE_UNIT_2;
    }
    pProgram->m_WasDirty = false;
    if (pTables->m_pCurrentProgram != nullptr) {
        if (pTables->m_pCurrentProgram != pProgram && pTables->m_pCurrentProgram->m_Mode != pProgram->m_Mode) {
            pState->m_DirtyFlags |= 0x100;
        }
        Program* pOld = pTables->m_pCurrentProgram;
        if (pOld != nullptr && pOld->m_IsDeletePending && pOld != pProgram) {
            DestroyProgram(pOld->m_Name);
        }
    }
    s_ShaderManager.m_pTables->m_pCurrentProgram = pProgram;
    pState->m_ProgramName = program;
    pState->m_HasGeometryShader = pProgram->m_HasGeometryShader;
}
