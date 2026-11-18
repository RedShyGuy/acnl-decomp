#include "cfl/dCfl.h"
#include <cstring>
#include "Bank/bank_Extern.h"
#include "Other/dHeapUtil.h"
#include "cfl/MiiIcon.h"
#include "cfl/MiiModelSimple.h"
#include "cfl/cfl_Api.h"
#include "cfl/cfl_Database.h"
#include "cfl/cfl_Model.h"
#include "g3d/dFunc.h"
#include "g3d/g3d_System.h"
#include "graphic/dCurrentSetter.h"
#include "graphic/dScreenCamera.h"
#include "nn/gr/CTR/detail/gr_Command.h"
#include "nn/gr/CTR/gr_BindSymbolVSFloat.h"
#include "nn/gr/CTR/gr_FragmentLight.h"
#include "nn/gr/CTR/gr_FrameBuffer.h"
#include "nn/gr/CTR/gr_RenderState.h"
#include "nn/gr/CTR/gr_Shader.h"
#include "nn/gr/CTR/CTR_Api.h"
#include "nn/gx/gx_Api.h"
#include "nn/math/math_Vector4.h"
#include "nw/gfx/gfx_AmbientLight.h"
#include "nw/gfx/gfx_FragmentLight.h"
#include "nw/gfx/gfx_HemiSphereLight.h"
#include "nw/gfx/gfx_RenderContext.h"
#include "nw/gfx/res/gfx_ResFragmentShader.h"
#include "nw/gfx/res/gfx_ResLight.h"
#include "nw/gfx/res/gfx_ResLookupTable.h"
#include "sead/seadExpHeap.h"
#include "sead/seadFrameBufferCtr.h"
#include "sead/seadMathCalcCommon.h"
#include "sead/seadNew.h"
#include "ssys/ma/dFileData.h"
#include "ssys/ma/dHeapAllocator.h"
#include "ssys/st/dList.h"

extern const nn::math::MTX34 MTX34_IDENTITY;

namespace cfl {
// the helpers of the module (only used here)
DECOMP_NOINLINE void ClearLight(int index);
void MakeIcons();
void SetFragmentLight(const nw::gfx::FragmentLight* light, int index);
void DrawOpaque(nw::gfx::MeshRenderer* renderer, void* userData);
void DrawTranslucent(nw::gfx::MeshRenderer* renderer, void* userData);
void Transform(nn::math::VEC4* out, const nn::math::MTX34& mtx, const nn::math::VEC3& v, bool isDirection);
void SetHemiSphereLight(const nw::gfx::HemiSphereLight* light);
void SetLightPositions(const nn::math::MTX34& view);
void SetupLights();
void SetLightPosition(const nn::math::MTX34& view, const nn::math::VEC3& position, int index, bool isDirectional);
void SetSpotDirection(const nn::math::MTX34& view, const nn::math::VEC3& direction, int index);
void SetDistanceAttenuation(const nw::gfx::res::ResFragmentLight& light, int index);
void Draw(bool isTranslucent);

namespace {
using nn::gr::CTR::detail::CommandHeader;
using nn::gr::CTR::FragmentLight;

// the heaps of the module (sizes from the binary)
const u32 HEAP_SIZE = 0x17A000;
const u32 GRAPHICS_HEAP_SIZE = 0xED800;
// the key of CFLi_EnableSpecialMii
const u32 SPECIAL_MII_KEY = 0x13DE28CF;
// nngxGetCmdlistParameteri: the current address in the command buffer
const GLenum CMDLIST_CURRENT_ADDRESS = 0x208;
// the states of nngxUpdateState/nngxValidateState after the models (all states at the end)
const GLbitfield UPDATE_STATES = 0x1C00;
const GLbitfield ALL_STATES = 0x3FFF;
// the lookup tables of the fragment lighting (PICA LUT ids; SP is not taken from the shader)
enum LutKind
{
    LUT_D0 = 0,
    LUT_D1 = 1,
    LUT_SP = 2,
    LUT_FR = 3,
    LUT_RB = 4,
    LUT_RG = 5,
    LUT_RR = 6,
    LUT_KIND_NUM = 7,
};
const u32 LOOKUP_TABLE_NUM = 10;
// the light sources the hemisphere light takes (sky and ground)
const int HEMISPHERE_SKY_LIGHT = 6;
const int HEMISPHERE_GROUND_LIGHT = 7;
// a light whose attenuation ends this close is off
const f32 DISTANCE_ATTENUATION_END_MIN = 0.1f;

// GL values (the gl headers are not used)
const GLenum TEXTURE0 = 0x84C0;
const GLenum RGBA4 = 0x8056;
// the PICA registers (3dbrew "GPU/Internal Registers")
const u32 REG_FACECULLING_CONFIG = 0x040;
const u32 REG_TEXENV0_SOURCE = 0x0C0;
const u32 REG_TEXENV1_SOURCE = 0x0C8;
const u32 REG_TEXENV2_SOURCE = 0x0D0;
const u32 REG_TEXENV3_SOURCE = 0x0D8;
const u32 REG_TEXENV3_COLOR = 0x0DB;
const u32 REG_TEXENV_UPDATE_BUFFER = 0x0E0;
const u32 REG_TEXENV4_SOURCE = 0x0F0;
const u32 REG_TEXENV5_SOURCE = 0x0F8;
const u32 REG_DEPTH_COLOR_MASK = 0x107;
const u32 REG_LIGHTING_LUT_INDEX = 0x1C5;
const u32 REG_VSH_FLOATUNIFORM_INDEX = 0x2C0;
const u32 REG_VSH_FLOATUNIFORM_DATA = 0x2C1;
// bit 31 of the uniform index: 32-bit floats
const u32 FLOATUNIFORM_FLOAT32 = 0x80000000;
} // namespace

// the result 9 of CFL_InitRes: the broken database was formatted
// 0x009524DC
bool s_IsDatabaseFormatted;
// the register of uViewMtx
// 0x009524DD
u8 s_ViewMtxRegister;
// the heap "dCfl" (resource archive, work, shader, network buffer, the Func)
// 0x009524E0
ssys::ma::HeapAllocator* s_pAllocator;
// the heap "dCflGraphics" (the heap of the main memory for the models and icons)
// 0x009524E4
ssys::ma::Allocator* s_pGraphicsAllocator;
// 0x009524E8
ssys::ma::FileData* s_pShaderFile;
// 0x009524EC
void* s_pNetBuffer;
// 0x009524F0
void* s_pWork;
// 0x009524F4
nw::gfx::res::ResFragmentShader s_FragmentShader;
// the multiplied color of the drawn model (the constant color of the combiner stage 3)
// 0x009524F8
u32 s_ColorCommand[2] = {0, CommandHeader(REG_TEXENV3_COLOR)};
// the symbols of the shader (CFL_SetShaderParameter)
// 0x00952500
nn::gr::CTR::BindSymbolVSFloat s_PosMtxSymbol;
// 0x00952508
nn::gr::CTR::BindSymbolVSFloat s_ProjMtxSymbol;
// 0x00952510
nn::gr::CTR::BindSymbolVSFloat s_ViewMtxSymbol;
// 0x00952518
nn::gr::CTR::BindSymbolVSFloat s_NrmMtxSymbol;
// 0x00952520
nn::gr::CTR::BindSymbolVSFloat s_TexMtxSymbol;
// 0x00952528
nn::gr::CTR::BindSymbolVSInput s_PositionSymbol;
// 0x00952530
nn::gr::CTR::BindSymbolVSInput s_NormalSymbol;
// 0x00952538
nn::gr::CTR::BindSymbolVSInput s_TexCoordSymbol;
// draws the models in the passes of g3d
// 0x00952540
g3d::Func s_Func;
// the uniform uTexMtx: two rows in 32-bit floats (the register is set by Initialize)
// 0x00952548
u32 s_TexMtxCommand[12] = {
    0,
    CommandHeader(REG_VSH_FLOATUNIFORM_INDEX),
    0,
    CommandHeader(REG_VSH_FLOATUNIFORM_DATA, 0xF, 7),
    0,
    0,
    0x39000000,
    0,
    0,
    0x39000000,
    0,
    0,
};
// 0x0099955C
nn::gr::CTR::Shader s_Shader;
// the models to make (MiiModelBase::m_InitNode)
// 0x009ABE44
ssys::st::List s_InitList;
// the models to draw in this frame (MiiModelBase::m_DrawNode)
// 0x009ABE50
ssys::st::List s_DrawList;
// 0x009ABE5C
ssys::st::List s_IconList;
// the lookup tables of the fragment lighting by LutKind
// 0x009ABE68
nw::gfx::res::ResImageLookupTable s_LookupTables[LOOKUP_TABLE_NUM];
// 0x009ABE90
FragmentLight s_FragmentLight;
// 0x009AC040
CFLNetInformation s_NetInformation;

// constructs the shader, the symbols, the lists, the lookup tables, the fragment light and s_Func
// 0x0079FF60 (static initializer)

// the state of the drawing of the models: combiners, culling, the color mask
// 0x0085893C
extern const u32 s_DrawStateCommands[42] = {
    0, CommandHeader(REG_TEXENV_UPDATE_BUFFER, 0x2),
    2, CommandHeader(REG_FACECULLING_CONFIG),
    0x1F41, CommandHeader(REG_DEPTH_COLOR_MASK),
    0x0FF30FF3, CommandHeader(REG_TEXENV0_SOURCE, 0xF, 4, true), 0, 0, 0xFFFFFFFF, 0,
    0x0FFF0F1F, CommandHeader(REG_TEXENV1_SOURCE, 0xF, 4, true), 0, 1, 0xFFFFFFFF, 0,
    0x0FFF0F12, CommandHeader(REG_TEXENV2_SOURCE, 0xF, 4, true), 0, 8, 0xFFFFFFFF, 0,
    0x0FFF0FEF, CommandHeader(REG_TEXENV3_SOURCE, 0xF, 4, true), 0, 1, 0xFFFFFFFF, 0,
    0x0FFF0FFF, CommandHeader(REG_TEXENV4_SOURCE, 0xF, 4, true), 0, 0, 0xFFFFFFFF, 0,
    0x0FFF0FFF, CommandHeader(REG_TEXENV5_SOURCE, 0xF, 4, true), 0, 0, 0xFFFFFFFF, 0,
};

namespace {
// the owner of a node of the lists
inline MiiModelBase* GetOwner(ssys::st::ListNode* node)
{
    return static_cast<MiiModelBase::ListNodeMiiModelBase*>(node)->m_pOwner;
}

// writes the command words between start and command into the command buffer
inline void MoveCommandPointer(const u32* command, const u32* start)
{
    nngxMoveCommandbufferPointer((reinterpret_cast<uptr>(command) - reinterpret_cast<uptr>(start)) & ~3u);
}

// uViewMtx becomes the identity (the models and icons of CFL are drawn without a camera here)
inline void PutIdentityViewMtx()
{
    u32* command;
    nngxGetCmdlistParameteri(CMDLIST_CURRENT_ADDRESS, reinterpret_cast<GLint*>(&command));
    u32* start = command;
    command[0] = s_ViewMtxRegister | FLOATUNIFORM_FLOAT32;
    command[1] = CommandHeader(REG_VSH_FLOATUNIFORM_INDEX);
    command += 2;
    nn::gr::CTR::CopyMtx34WithHeader(reinterpret_cast<f32*>(command), &MTX34_IDENTITY,
                                     CommandHeader(REG_VSH_FLOATUNIFORM_DATA, 0xF, 11));
    command += 14;
    MoveCommandPointer(command, start);
}

// the main-memory heap of the graphics allocator for the MemoryMgr (it takes it in the slot of the first kind)
inline ssys::ma::VramHeap* GetGraphicsHeap()
{
    return reinterpret_cast<ssys::ma::VramHeap*>(s_pGraphicsAllocator->m_pHeap);
}
} // namespace

// 0x004DF670 (name is ours)
bool Initialize(sead::Heap* parent)
{
    s_FragmentShader = nw::gfx::res::ResFragmentShader();
    sead::ExpHeap* heap = HeapUtil::CreateExpHeapWithCName(HEAP_SIZE, parent, "dCfl", 32);
    s_pAllocator = new (heap, 4) ssys::ma::HeapAllocator();
    s_pAllocator->SetHeap(heap);

    const u32 resSize = CFL_GetResFileSize();
    void* res = s_pAllocator->Alloc(resSize, 128);
    CFL_LoadResFile(res);
    s_pWork = s_pAllocator->Alloc(CFL_GetWorkSize(true), 128);
    const s32 result = CFL_InitRes(s_pWork, res, resSize, true);
    s_IsDatabaseFormatted = false;
    if (result != CFL_RESULT_SUCCESS && result == CFL_RESULT_9) {
        s_IsDatabaseFormatted = true;
    }
    CFLi_EnableSpecialMii(SPECIAL_MII_KEY);
    InitializeShader();
    s_TexMtxCommand[0] = s_TexMtxSymbol.m_Start | FLOATUNIFORM_FLOAT32;
    s_ViewMtxRegister = s_ViewMtxSymbol.m_Start;

    s_pNetBuffer = s_pAllocator->Alloc(CFL_GetNetInfoBufferSize(2, 1), 4);
    CFL_InitNetInformation(&s_NetInformation, s_pNetBuffer, 1, 2, 1);
    s_Func.Initialize(s_pAllocator);
    s_Func.SetFunctions(DrawOpaque, DrawTranslucent, NULL);
    s_pAllocator->Adjust();
    s_InitList.Clear();
    s_DrawList.Clear();
    s_IconList.Clear();

    sead::ExpHeap* graphicsHeap = HeapUtil::CreateExpHeapWithCName(GRAPHICS_HEAP_SIZE, parent, "dCflGraphics", 32);
    s_pGraphicsAllocator = new (graphicsHeap, 4) ssys::ma::Allocator();
    s_pGraphicsAllocator->SetHeap(graphicsHeap);
    return true;
}

// 0x004DF894 (name is ours)
void ClearLight(int index)
{
    FragmentLight::Source& source = s_FragmentLight.m_Sources[index];
    source.m_AmbientR = 0;
    source.m_AmbientG = 0;
    source.m_AmbientB = 0;
    source.m_Specular0R = 0;
    source.m_Specular0G = 0;
    source.m_Specular0B = 0;
    source.m_Specular1R = 0;
    source.m_Specular1G = 0;
    source.m_Specular1B = 0;
    source.m_IsTwoSideDiffuse = false;
    s_FragmentLight.m_IsEnableShadow[index] = false;
    s_FragmentLight.m_IsEnableSpot[index] = false;
    s_FragmentLight.m_IsEnableDistanceAttenuation[index] = false;
}

// 0x004DF8E8 slot 0x14 (name is ours)
bool MiiModelBase::Initialize(nw::os::IAllocator* allocator, s32 source, u16 index, Description description)
{
    m_Source = source;
    m_Index = index;
    m_Resolution = description.resolution;
    m_ExpressionMask = description.expressionMask;
    m_Flags = description.flags;
    m_pAllocator = allocator;
    m_pData = description.data;
    m_InitialExpression = description.expression;
    m_Expression = description.expression;
    if (!AllocateBuffers()) {
        return false;
    }
    m_State = STATE_WAITING;
    s_InitList.PushBack(&m_InitNode);
    return true;
}

// 0x004DF950 (name is ours)
void MiiModelBase::SetColor(nw::ut::FloatColor color)
{
    const u32 r = static_cast<u32>(color.r * 255.0f) & 0xFF;
    const u32 g = static_cast<u32>(color.g * 255.0f) & 0xFF;
    const u32 b = static_cast<u32>(color.b * 255.0f) & 0xFF;
    m_Color = r | (g << 8) | (b << 16) | 0xFF000000;
}

// 0x004DF9A0 slot 0x00 (name is ours)
// 0x004DF99C (deleting dtor)
MiiModelBase::ListNodeMiiModelBase::~ListNodeMiiModelBase()
{
    // empty (in the original too)
}

// 0x004DF9A4 (name is ours)
void MiiModelBase::RequestDraw()
{
    if (m_State == STATE_REINIT || m_State == STATE_READY) {
        s_DrawList.PushBack(&m_DrawNode);
    }
}

// 0x004DF9C8 (name is ours)
bool MiiModelBase::Release()
{
    if (m_State == STATE_NONE) {
        return true;
    }
    if (s_InitList.Contains(&m_InitNode)) {
        s_InitList.Erase(&m_InitNode);
    }
    if (s_DrawList.Contains(&m_DrawNode)) {
        m_State = STATE_RELEASING;
        return false;
    }
    FreeBuffers();
    m_State = STATE_NONE;
    return true;
}

// 0x004DFA50 (name is ours)
void MiiModelBase::SetScale(const nn::math::VEC3& scale)
{
    m_Scale.x = scale.x;
    m_Scale.y = scale.y;
    m_Scale.z = scale.z;
}

// 0x004DFA6C (name is ours)
void MiiModelBase::SetMatrix(const nn::math::MTX34& mtx)
{
    sead::Matrix34CalcCtr<f32>::copy(m_Mtx, mtx);
}

// 0x004DFA84 slot 0x00
// 0x004DFA74 (deleting dtor)
MiiModelBase::~MiiModelBase()
{
    // (only the members are destroyed)
}

// 0x004DFA94 (name is ours)
void MakeIcons()
{
    ssys::st::ListNode* next;
    for (ssys::st::ListNode* node = s_IconList.GetFirst(); node != NULL; node = next) {
        MiiIcon* icon = static_cast<MiiIcon*>(node);
        next = icon->m_pNext;
        CFLIconParam param;
        param.width = icon->m_Width;
        param.height = icon->m_Height;
        param.format = icon->m_Format;
        param.background = 1;
        param.color = 0xFFFFFFFF;
        param.textureAlpha = false;
        param.skipOpa = false;
        param.callback = NULL;
        param.callbackArg = NULL;
        {
            graphic::CurrentSetter setter(GetGraphicsHeap(), NULL, NULL);
            PutIdentityViewMtx();
            CFL_MakeIcon(icon->m_Texture, icon->m_Source, icon->m_pData, icon->m_Index, 0, &param);
        }
        s_IconList.Erase(icon);
    }
}

// 0x004DFBB0 slot 0x08 (name is ours)
bool MiiModelSimple::InitModel()
{
    graphic::CurrentSetter setter(GetGraphicsHeap(), NULL, NULL);
    PutIdentityViewMtx();
    CFL_InitCharModel(m_pCharModel, m_Source, m_pData, m_Index, 0x10000, m_pBuffer, m_Resolution, m_ExpressionMask,
                      m_Flags);
    return true;
}

// 0x004DFC80 slot 0x1C (name is ours)
void MiiModelSimple::FreeBuffers()
{
    if (m_pCharModel == NULL) {
        return;
    }
    if (m_State != STATE_WAITING) {
        graphic::CurrentSetter setter(GetGraphicsHeap(), NULL, NULL);
        CFL_DeleteCharModel(m_pCharModel);
    }
    m_pAllocator->Free(m_pCharModel);
    m_pAllocator->Free(m_pBuffer);
    m_pCharModel = NULL;
    m_pBuffer = NULL;
}

// 0x004DFD10 slot 0x10 (name is ours)
CFLCharModel* MiiModelSimple::GetCharModel()
{
    return m_pCharModel;
}

// 0x004DFD18 slot 0x18 (name is ours)
bool MiiModelSimple::AllocateBuffers()
{
    CFLCharModel* model = static_cast<CFLCharModel*>(m_pAllocator->Alloc(sizeof(CFLCharModel), 32));
    if (model == NULL) {
        return false;
    }
    std::memset(static_cast<void*>(model), 0, sizeof(CFLCharModel));
    m_pCharModel = model;
    m_pBuffer = m_pAllocator->Alloc(CFL_GetModelBufferSize(m_ExpressionMask), 4);
    if (m_pBuffer != NULL) {
        return true;
    }
    m_pAllocator->Free(model);
    m_pCharModel = NULL;
    return false;
}

// 0x004DFD9C slot 0x20 (name is ours)
bool MiiModelSimple::SetExpression(const s32& expression)
{
    if (m_pCharModel != NULL && (m_ExpressionMask & (1 << expression)) != 0) {
        m_Expression = expression;
        CFL_SetExpression(m_pCharModel, expression);
        return true;
    }
    return false;
}

// 0x004DFDE0 slot 0x0C (name is ours)
bool MiiModelSimple::IsReady()
{
    return true;
}

// 0x004DFDE8
MiiModelSimple::MiiModelSimple() : m_pCharModel(NULL), m_pBuffer(NULL)
{
}

// 0x004DFE78 slot 0x00
// 0x004DFE68 (deleting dtor)
MiiModelSimple::~MiiModelSimple()
{
    // (only the members are destroyed)
}

// 0x004DFE88 (name is ours)
CFLNetInformation* GetNetInformation()
{
    return &s_NetInformation;
}

// 0x004DFE94 (name is ours)
bool InitializeShader()
{
    s_pShaderFile = new (s_pAllocator->Alloc(sizeof(ssys::ma::FileData), 128)) ssys::ma::FileData();
    s_pShaderFile->Load("CFL/CflShader.shbin", s_pAllocator->m_pHeap, 128);
    s_Shader.SetupBinary(s_pShaderFile->m_pData, 0, -1);
    s_Shader.SearchBindSymbol(&s_ProjMtxSymbol, "uProjMtx");
    s_Shader.SearchBindSymbol(&s_ViewMtxSymbol, "uViewMtx");
    s_Shader.SearchBindSymbol(&s_PosMtxSymbol, "uPosMtx");
    s_Shader.SearchBindSymbol(&s_NrmMtxSymbol, "uNrmMtx");
    s_Shader.SearchBindSymbol(&s_TexMtxSymbol, "uTexMtx");
    s_Shader.SearchBindSymbol(&s_PositionSymbol, "aPosition");
    s_Shader.SearchBindSymbol(&s_NormalSymbol, "aNormal");
    s_Shader.SearchBindSymbol(&s_TexCoordSymbol, "aTexCoord");
    CFLiShaderParam param;
    param.attribute[0] = s_PositionSymbol.m_Start;
    param.attribute[1] = s_NormalSymbol.m_Start;
    param.attribute[2] = s_TexCoordSymbol.m_Start;
    param.textureUnit = TEXTURE0;
    CFL_SetShaderParameter(s_ProjMtxSymbol.m_Start, s_PosMtxSymbol.m_Start, s_NrmMtxSymbol.m_Start,
                           s_TexMtxSymbol.m_Start, false, param);
    return true;
}

// the light source index from the fragment light of the scene
// 0x004E001C (name is ours)
void SetFragmentLight(const nw::gfx::FragmentLight* light, int index)
{
    nw::gfx::res::ResFragmentLight res(light->GetResSceneObject().ptr());
    const nw::gfx::res::ResFragmentLightData* data = res.ptr();
    const u32 kind = data->lightKind;
    FragmentLight::Source& source = s_FragmentLight.m_Sources[index];
    source.m_AmbientR = static_cast<u8>(data->ambient.r * 255.0f);
    source.m_AmbientG = static_cast<u8>(data->ambient.g * 255.0f);
    source.m_AmbientB = static_cast<u8>(data->ambient.b * 255.0f);
    source.m_DiffuseR = static_cast<u8>(data->diffuse.r * 255.0f);
    source.m_DiffuseG = static_cast<u8>(data->diffuse.g * 255.0f);
    source.m_DiffuseB = static_cast<u8>(data->diffuse.b * 255.0f);
    source.m_Specular0R = static_cast<u8>(data->specular0.r * 255.0f);
    source.m_Specular0G = static_cast<u8>(data->specular0.g * 255.0f);
    source.m_Specular0B = static_cast<u8>(data->specular0.b * 255.0f);
    source.m_Specular1R = static_cast<u8>(data->specular1.r * 255.0f);
    source.m_Specular1G = static_cast<u8>(data->specular1.g * 255.0f);
    source.m_Specular1B = static_cast<u8>(data->specular1.b * 255.0f);
    source.m_IsTwoSideDiffuse = (data->flags & nw::gfx::res::ResFragmentLightData::FLAG_TWO_SIDED_DIFFUSE) != 0;
    s_FragmentLight.m_IsEnableShadow[index] = false;
    if (kind == nw::gfx::res::ResFragmentLightData::LIGHT_KIND_DIRECTIONAL) {
        s_FragmentLight.m_IsEnableSpot[index] = false;
        s_FragmentLight.m_IsEnableDistanceAttenuation[index] = false;
    } else if (kind == nw::gfx::res::ResFragmentLightData::LIGHT_KIND_POINT ||
               kind == nw::gfx::res::ResFragmentLightData::LIGHT_KIND_SPOT) {
        SetDistanceAttenuation(res, index);
        s_FragmentLight.m_IsEnableSpot[index] = false;
    }
}

// (g3d::FuncNode::DrawFunc)
// 0x004E0188 (name is ours)
void DrawOpaque(nw::gfx::MeshRenderer* /*renderer*/, void* /*userData*/)
{
    if (s_DrawList.GetFirst() != NULL) {
        Draw(false);
        g3d::SetupRenderContext();
    }
}

// 0x004E01B4 (name is ours)
void DrawTranslucent(nw::gfx::MeshRenderer* /*renderer*/, void* /*userData*/)
{
    if (s_DrawList.GetFirst() != NULL) {
        Draw(true);
        g3d::SetupRenderContext();
    }
}

// v transformed by mtx (a direction: negated, w 0)
// 0x004E01E0 (name is ours)
void Transform(nn::math::VEC4* out, const nn::math::MTX34& mtx, const nn::math::VEC3& v, bool isDirection)
{
    const f32 w = isDirection ? 0.0f : 1.0f;
    out->x = mtx.m[0][0] * v.x + mtx.m[0][1] * v.y + mtx.m[0][2] * v.z + mtx.m[0][3] * w;
    out->y = mtx.m[1][0] * v.x + mtx.m[1][1] * v.y + mtx.m[1][2] * v.z + mtx.m[1][3] * w;
    out->z = mtx.m[2][0] * v.x + mtx.m[2][1] * v.y + mtx.m[2][2] * v.z + mtx.m[2][3] * w;
    out->w = w;
    if (isDirection) {
        const f32 x = out->x;
        const f32 y = out->y;
        const f32 z = out->z;
        out->w = 0.0f;
        out->x = -x;
        out->y = -y;
        out->z = -z;
    }
}

// the hemisphere light as the light sources 6 (sky: diffuse, both halves: ambient) and 7 (ground: diffuse)
// 0x004E0280 (name is ours)
void SetHemiSphereLight(const nw::gfx::HemiSphereLight* light)
{
    if (light == NULL) {
        return;
    }
    const nw::gfx::res::ResHemiSphereLightData* data =
        nw::gfx::res::ResHemiSphereLight(light->GetResSceneObject().ptr()).ptr();
    if (data == NULL) {
        return;
    }
    if (data->direction.x == 0.0f && data->direction.y == 0.0f && data->direction.z == 0.0f) {
        return;
    }
    s_FragmentLight.m_IsEnabled[HEMISPHERE_SKY_LIGHT] = true;
    s_FragmentLight.m_Sources[HEMISPHERE_SKY_LIGHT].m_Id = HEMISPHERE_SKY_LIGHT;
    ClearLight(HEMISPHERE_SKY_LIGHT);
    const f32 skyR = data->skyColor.r * 0.5f;
    const f32 skyG = data->skyColor.g * 0.5f;
    const f32 skyB = data->skyColor.b * 0.5f;
    const f32 groundR = data->groundColor.r * 0.5f;
    const f32 groundG = data->groundColor.g * 0.5f;
    const f32 groundB = data->groundColor.b * 0.5f;
    FragmentLight::Source& sky = s_FragmentLight.m_Sources[HEMISPHERE_SKY_LIGHT];
    sky.m_DiffuseR = static_cast<u8>(skyR * 255.0f);
    sky.m_DiffuseG = static_cast<u8>(skyG * 255.0f);
    sky.m_DiffuseB = static_cast<u8>(skyB * 255.0f);
    sky.m_AmbientR = static_cast<u8>((groundR + skyR) * 255.0f);
    sky.m_AmbientG = static_cast<u8>((groundG + skyG) * 255.0f);
    sky.m_AmbientB = static_cast<u8>((groundB + skyB) * 255.0f);
    s_FragmentLight.m_IsEnabled[HEMISPHERE_GROUND_LIGHT] = true;
    s_FragmentLight.m_Sources[HEMISPHERE_GROUND_LIGHT].m_Id = HEMISPHERE_GROUND_LIGHT;
    ClearLight(HEMISPHERE_GROUND_LIGHT);
    FragmentLight::Source& ground = s_FragmentLight.m_Sources[HEMISPHERE_GROUND_LIGHT];
    ground.m_DiffuseR = static_cast<u8>(groundR * 255.0f);
    ground.m_DiffuseG = static_cast<u8>(groundG * 255.0f);
    ground.m_DiffuseB = static_cast<u8>(groundB * 255.0f);
}

// 0x004E03E0 (name is ours)
bool IsDatabaseFormatted()
{
    return s_IsDatabaseFormatted;
}

namespace {
// (inline) the sampler of the lighting table of the shader for a lookup table
inline nw::gfx::res::ResLightingLookupTable GetLightingSampler(nw::gfx::res::ResFragmentLightingTable table, int kind)
{
    switch (kind) {
    case LUT_D0:
        return table.GetSampler(3);
    case LUT_D1:
        return table.GetSampler(4);
    case LUT_FR:
        return table.GetSampler(5);
    case LUT_RB:
        return table.GetSampler(2);
    case LUT_RG:
        return table.GetSampler(1);
    case LUT_RR:
        return table.GetSampler(0);
    default:
        return nw::gfx::res::ResLightingLookupTable();
    }
}
} // namespace

// 0x004E03F0 (name is ours)
void SetFragmentShader(nw::gfx::res::ResFragmentShader shader)
{
    s_FragmentShader = shader;
    const nw::gfx::res::ResFragmentLightingData& lighting = shader.ptr()->lighting;
    const u32 flags = lighting.flags;
    const u32 config = lighting.config;
    const u32 fresnelSelector = lighting.fresnelSelector;
    s_FragmentLight.m_IsClampHighlights = (flags & nw::gfx::res::ResFragmentLightingData::FLAG_CLAMP_HIGHLIGHTS) != 0;
    s_FragmentLight.m_FresnelSelector = fresnelSelector;
    s_FragmentLight.m_Config = config;
    s_FragmentLight.m_IsEnableLutD0 = (flags & nw::gfx::res::ResFragmentLightingData::FLAG_SAMPLER3) != 0;
    s_FragmentLight.m_IsEnableLutD1 = (flags & nw::gfx::res::ResFragmentLightingData::FLAG_SAMPLER4) != 0;
    s_FragmentLight.m_IsEnableReflection =
        (flags & nw::gfx::res::ResFragmentLightingData::FLAG_CONFIG_SAMPLERS) != 0;
    const u32 geometricFactor0 = (flags & nw::gfx::res::ResFragmentLightingData::FLAG_GEOMETRIC_FACTOR0) >> 3;
    const u32 geometricFactor1 = (flags & nw::gfx::res::ResFragmentLightingData::FLAG_GEOMETRIC_FACTOR1) >> 4;
    for (int i = 0; i < FragmentLight::LIGHT_COUNT; i++) {
        s_FragmentLight.m_Sources[i].m_IsGeometricFactor0 = geometricFactor0;
        s_FragmentLight.m_Sources[i].m_IsGeometricFactor1 = geometricFactor1;
    }
    for (int i = 0; i < static_cast<int>(LOOKUP_TABLE_NUM); i++) {
        s_LookupTables[i] = nw::gfx::res::ResImageLookupTable();
    }
    for (int kind = 0; kind < LUT_KIND_NUM; kind++) {
        if (!s_FragmentShader.IsValid()) {
            continue;
        }
        nw::gfx::res::ResLightingLookupTable sampler =
            GetLightingSampler(s_FragmentShader.GetLightingTable(), kind);
        if (sampler.ptr() == NULL) {
            continue;
        }
        const nw::gfx::res::ResLookupTableData* table =
            static_cast<const nw::gfx::res::ResLookupTableData*>(sampler.ptr()->sampler.ToPtr());
        if (table->typeInfo == nw::gfx::res::ResLookupTable::TYPE_REFERENCE) {
            table = static_cast<const nw::gfx::res::ResLookupTableData*>(
                static_cast<const nw::gfx::res::ResReferenceLookupTableData*>(table)->target.ToPtr());
        } else if (table->typeInfo != nw::gfx::res::ResLookupTable::TYPE_IMAGE) {
            table = NULL;
        }
        s_LookupTables[kind] = nw::gfx::res::ResImageLookupTable(table);
        FragmentLight::LutConfig& lutConfig = s_FragmentLight.m_Luts[kind];
        lutConfig.m_IsAbs = s_LookupTables[kind].ptr()->isAbsolute;
        lutConfig.m_Input = sampler.ptr()->input;
        lutConfig.m_Scale = sampler.ptr()->scale;
    }
}

// the light positions and spot directions of the scene in view space; the hemisphere direction as lights 6 and 7
// 0x004E0664 (name is ours)
void SetLightPositions(const nn::math::MTX34& view)
{
    nw::gfx::SceneEnvironment& environment = g3d::GetRenderContext()->m_SceneEnvironment;
    environment.SetActiveLightSet(Func1E891C());
    const s32 count = environment.m_FragmentLightCount;
    for (s32 i = 0; i < count; i++) {
        nw::gfx::FragmentLight* light = static_cast<nw::gfx::FragmentLight*>(environment.m_FragmentLights[i]);
        switch (light->GetLightKind()) {
        case nw::gfx::res::ResFragmentLightData::LIGHT_KIND_DIRECTIONAL:
            SetLightPosition(view, light->m_Direction, i, true);
            break;
        case nw::gfx::res::ResFragmentLightData::LIGHT_KIND_POINT: {
            const nn::math::VEC3 position(light->m_WorldMatrix.m[0][3], light->m_WorldMatrix.m[1][3],
                                          light->m_WorldMatrix.m[2][3]);
            SetLightPosition(view, position, i, false);
            break;
        }
        case nw::gfx::res::ResFragmentLightData::LIGHT_KIND_SPOT: {
            const nn::math::VEC3 position(light->m_WorldMatrix.m[0][3], light->m_WorldMatrix.m[1][3],
                                          light->m_WorldMatrix.m[2][3]);
            SetLightPosition(view, position, i, false);
            SetSpotDirection(view, light->m_Direction, i);
            break;
        }
        }
    }
    const nw::gfx::HemiSphereLight* hemiSphereLight = environment.m_pHemiSphereLight;
    if (hemiSphereLight == NULL) {
        return;
    }
    const nw::gfx::res::ResHemiSphereLightData* data =
        nw::gfx::res::ResHemiSphereLight(hemiSphereLight->GetResSceneObject().ptr()).ptr();
    if (data == NULL) {
        return;
    }
    const nn::math::VEC3& direction = data->direction;
    if (direction.x == 0.0f && direction.y == 0.0f && direction.z == 0.0f) {
        return;
    }
    const nn::math::VEC3 negative(-direction.x, -direction.y, -direction.z);
    SetLightPosition(view, negative, HEMISPHERE_SKY_LIGHT, true);
    SetLightPosition(view, direction, HEMISPHERE_GROUND_LIGHT, true);
}

// the lights of the scene: the ambient light as the global ambient, the fragment lights as the first sources, the
// hemisphere light as sources 6 and 7
// 0x004E07F0 (name is ours)
void SetupLights()
{
    nw::gfx::SceneEnvironment& environment = g3d::GetRenderContext()->m_SceneEnvironment;
    environment.SetActiveLightSet(Func1E891C());
    const nw::gfx::AmbientLight* ambientLight = environment.m_pAmbientLight;
    if (ambientLight != NULL) {
        const nw::gfx::res::ResAmbientLightData* data =
            nw::gfx::res::ResAmbientLight(ambientLight->GetResSceneObject().ptr()).ptr();
        s_FragmentLight.m_GlobalAmbientR = static_cast<u8>(data->color.r * 255.0f);
        s_FragmentLight.m_GlobalAmbientG = static_cast<u8>(data->color.g * 255.0f);
        s_FragmentLight.m_GlobalAmbientB = static_cast<u8>(data->color.b * 255.0f);
    } else {
        s_FragmentLight.m_GlobalAmbientR = 0;
        s_FragmentLight.m_GlobalAmbientG = 0;
        s_FragmentLight.m_GlobalAmbientB = 0;
    }
    const s32 count = environment.m_FragmentLightCount;
    for (u8 i = 0; i < count; i++) {
        s_FragmentLight.m_IsEnabled[i] = true;
        s_FragmentLight.m_Sources[i].m_Id = i;
        SetFragmentLight(static_cast<nw::gfx::FragmentLight*>(environment.m_FragmentLights[i]), i);
    }
    for (s32 i = count; i < FragmentLight::LIGHT_COUNT; i++) {
        s_FragmentLight.m_IsEnabled[i] = false;
    }
    SetHemiSphereLight(environment.m_pHemiSphereLight);
}

// 0x004E0910 (name is ours)
bool IsAvailableOfficialData(u16 index)
{
    return CFL_IsAvailableOfficialData(index) == true;
}

// the position (or direction) of light source index in view space
// 0x004E0924 (name is ours)
void SetLightPosition(const nn::math::MTX34& view, const nn::math::VEC3& position, int index, bool isDirectional)
{
    nn::math::VEC4 viewPosition;
    Transform(&viewPosition, view, position, isDirectional);
    FragmentLight::Source& source = s_FragmentLight.m_Sources[index];
    source.m_PositionXY = nn::gr::CTR::detail::Float32ToFloat16(viewPosition.x) |
                          nn::gr::CTR::detail::Float32ToFloat16(viewPosition.y) << 16;
    source.m_PositionZ = nn::gr::CTR::detail::Float32ToFloat16(viewPosition.z);
    source.m_IsDirectional = isDirectional;
}

// the spot direction of light source index in view space
// 0x004E0A40 (name is ours)
void SetSpotDirection(const nn::math::MTX34& view, const nn::math::VEC3& direction, int index)
{
    nn::math::VEC4 viewDirection;
    Transform(&viewDirection, view, direction, false);
    FragmentLight::Source& source = s_FragmentLight.m_Sources[index];
    source.m_SpotDirectionXY = nn::gr::CTR::detail::Float32ToFix13Fraction11(-viewDirection.x) |
                               nn::gr::CTR::detail::Float32ToFix13Fraction11(-viewDirection.y) << 16;
    source.m_SpotDirectionZ = nn::gr::CTR::detail::Float32ToFix13Fraction11(-viewDirection.z);
}

// the distance attenuation of light source index (off below an end of 0.1)
// 0x004E0C1C (name is ours)
void SetDistanceAttenuation(const nw::gfx::res::ResFragmentLight& light, int index)
{
    if ((light.ptr()->flags & nw::gfx::res::ResFragmentLightData::FLAG_DISTANCE_ATTENUATION) == 0) {
        s_FragmentLight.m_IsEnableDistanceAttenuation[index] = false;
        return;
    }
    s_FragmentLight.m_IsEnableDistanceAttenuation[index] = true;
    const f32 start = light.ptr()->distanceAttenuationStart;
    const f32 scale = 1.0f / (light.ptr()->distanceAttenuationEnd - start);
    FragmentLight::Source& source = s_FragmentLight.m_Sources[index];
    source.m_DistanceAttenuationScale = nn::gr::CTR::detail::Float32ToFloat20(scale);
    source.m_DistanceAttenuationBias = nn::gr::CTR::detail::Float32ToFloat20(-(start * scale));
    if (light.ptr()->distanceAttenuationEnd <= DISTANCE_ATTENUATION_END_MIN) {
        s_FragmentLight.m_IsEnabled[index] = false;
    }
}

// the models of the draw list (opaque or translucent part)
// 0x004E0D20 (name is ours)
void Draw(bool isTranslucent)
{
    ssys::st::ListNode* node = s_DrawList.GetFirst();
    if (node == NULL) {
        return;
    }
    u32* command;
    nngxGetCmdlistParameteri(CMDLIST_CURRENT_ADDRESS, reinterpret_cast<GLint*>(&command));
    u32* start = command;
    command = s_Shader.MakeFullCommand(command);
    MoveCommandPointer(command, start);

    nn::gr::CTR::RenderState renderState;
    nngxGetCmdlistParameteri(CMDLIST_CURRENT_ADDRESS, reinterpret_cast<GLint*>(&command));
    start = command;
    renderState.m_Blend.m_IsEnable = true;
    command = renderState.m_WBuffer.MakeCommand(command);
    command = renderState.m_Blend.MakeCommand(command, true);
    MoveCommandPointer(command, start);

    nn::math::MTX34 view;
    view = graphic::ScreenCamera::s_Instance.GetViewMatrix(0x400);
    SetupLights();
    nngxAdd3DCommand(s_DrawStateCommands, sizeof(s_DrawStateCommands), true);
    nngxAdd3DCommand(s_TexMtxCommand, sizeof(s_TexMtxCommand), true);
    for (u32 i = 0; i < LOOKUP_TABLE_NUM; i++) {
        if (s_LookupTables[i].ptr() != NULL) {
            u32 indexCommand[2] = {0, CommandHeader(REG_LIGHTING_LUT_INDEX)};
            indexCommand[0] = i << 8;
            nngxAdd3DCommand(indexCommand, sizeof(indexCommand), true);
            const nw::gfx::res::ResImageLookupTableData* lut = s_LookupTables[i].ptr();
            nngxAdd3DCommand(lut->commands.ToPtr(), lut->commandSize, true);
        }
    }
    SetLightPositions(view);

    nngxGetCmdlistParameteri(CMDLIST_CURRENT_ADDRESS, reinterpret_cast<GLint*>(&command));
    start = command;
    command = s_FragmentLight.MakeAllCommand(command, true);
    MoveCommandPointer(command, start);

    for (; node != NULL; node = node->m_pNext) {
        MiiModelBase* model = GetOwner(node);
        if (model->m_State != MiiModelBase::STATE_REINIT && model->m_State != MiiModelBase::STATE_READY) {
            continue;
        }
        CFLCharModel* charModel = model->GetCharModel();
        if (charModel == NULL) {
            continue;
        }
        s_ColorCommand[0] = model->m_Color;
        nngxAdd3DCommand(s_ColorCommand, sizeof(s_ColorCommand), true);
        nn::math::MTX34 scaled(model->m_Mtx);
        sead::Matrix34CalcCtr<f32>::scaleBases(scaled, model->m_Scale.x, model->m_Scale.y, model->m_Scale.z);
        nn::math::MTX34 position(scaled);
        CFL_SetMtxPosNrm(charModel, &position, &model->m_Mtx);
        if (isTranslucent) {
            CFL_DrawXlu(charModel);
        } else {
            CFL_DrawOpa(charModel);
        }
        glGetError();
    }
    nngxUpdateState(UPDATE_STATES);
    nngxValidateState(UPDATE_STATES, true);
    nngxUpdateState(ALL_STATES);
}

// 0x004E0FE8 (name is ours)
void MiiIcon::Set(GLuint texture, GLenum format, u16 width, u16 height, const void* data)
{
    m_Texture = texture;
    m_Source = 5;
    m_Index = 0;
    m_Format = format;
    m_Width = width;
    m_Height = height;
    m_pData = data;
}

// 0x004E1020 (name is ours)
void MiiIcon::Set(GLuint texture, GLenum format, u16 width, u16 height, u16 index)
{
    m_Texture = texture;
    m_pData = NULL;
    m_Format = format;
    m_Source = 0;
    m_Width = width;
    m_Height = height;
    m_Index = index;
}

// 0x004E1050 (name is ours)
void AddIcon(MiiIcon* icon)
{
    if (!s_IconList.Contains(icon)) {
        s_IconList.PushBack(icon);
    }
}

// 0x004E1084
MiiIcon::MiiIcon()
    : m_Texture(0), m_pData(NULL), m_Source(5), m_Format(RGBA4), m_Width(0), m_Height(0), m_Index(0)
{
}

// 0x004E10CC slot 0x00
// 0x004E10C8 (deleting dtor)
MiiIcon::~MiiIcon()
{
    // empty (in the original too)
}

// 0x004E10D0 (name is ours)
void AddToScene()
{
    s_Func.AddToScene(0);
}

// 0x004E10E0 (name is ours)
void Update()
{
    s_DrawList.Clear();
    bool hasReadyModel = false;
    for (ssys::st::ListNode* node = s_InitList.GetFirst(); node != NULL; node = node->m_pNext) {
        if (GetOwner(node)->IsReady()) {
            hasReadyModel = true;
            break;
        }
    }
    const bool hasIcons = s_IconList.GetFirst() != NULL;
    if (!hasReadyModel && !hasIcons) {
        return;
    }
    u32* command;
    nngxGetCmdlistParameteri(CMDLIST_CURRENT_ADDRESS, reinterpret_cast<GLint*>(&command));
    u32* start = command;
    command = s_Shader.MakeFullCommand(command);
    MoveCommandPointer(command, start);
    nngxSplitDrawCmdlist();
    nngxRunCmdlist();
    nngxWaitCmdlistDone();
    nngxStopCmdlist();
    if (hasReadyModel) {
        // (only the first model of the list is made again in the state STATE_REINIT)
        bool isFirst = true;
        ssys::st::ListNode* next;
        for (ssys::st::ListNode* node = s_InitList.GetFirst(); node != NULL; node = next) {
            next = node->m_pNext;
            MiiModelBase* model = GetOwner(node);
            const u8 state = model->m_State;
            if ((state == MiiModelBase::STATE_REINIT || state == MiiModelBase::STATE_WAITING) &&
                (isFirst || state != MiiModelBase::STATE_REINIT) && model->InitModel()) {
                model->m_State = MiiModelBase::STATE_READY;
                s_InitList.Erase(&model->m_InitNode);
            }
            isFirst = false;
        }
    } else if (hasIcons) {
        MakeIcons();
    }
    nngxGetCmdlistParameteri(CMDLIST_CURRENT_ADDRESS, reinterpret_cast<GLint*>(&command));
    start = command;
    command = sead::FrameBufferCtr::s_BoundGrFrameBuffer.MakeCommand(command, 7, true);
    MoveCommandPointer(command, start);
    nngxSplitDrawCmdlist();
    nngxRunCmdlist();
    nngxWaitCmdlistDone();
    nngxStopCmdlist();
    nngxClearCmdlist();
}
} // namespace cfl
