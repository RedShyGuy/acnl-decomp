#pragma once

#include "decomp.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Vector3.h"
#include "nw/ut/ut_FloatColor.h"
#include "sead/seadMathCalcCommon.h"
#include "ssys/st/dListNode.h"

struct CFLCharModel;

namespace nw {
namespace os {
class IAllocator;
} // namespace os
} // namespace nw

namespace cfl {
// A Mii model of the game, drawn by the Mii module (cfl/dCfl.h) with the shader of CFL (class names from the RTTI; the
// members and the slot names are ours). Initialize allocates the buffers and queues the model; the module makes the
// CFL model in its next Update (InitModel), RequestDraw puts it into the draw list of the frame.
// RTTI N3cfl12MiiModelBaseE @ 0x008D0C44
// vtable 0x009038BC (vptr 0x009038C4), offset_to_top 0, 9 entries
class MiiModelBase
{
public:
    // a node of the lists of the module (the model is its owner)
    // RTTI N3cfl12MiiModelBase20ListNodeMiiModelBaseE @ 0x008D0C38
    // vtable 0x009038AC (vptr 0x009038B4), offset_to_top 0, 2 entries
    class ListNodeMiiModelBase : public ::ssys::st::ListNode
    {
    public:
        explicit ListNodeMiiModelBase(MiiModelBase* owner) : m_pOwner(owner) {}
        virtual ~ListNodeMiiModelBase() override;
        // 0x004DF99C (deleting dtor)

        MiiModelBase* m_pOwner; // 0xC
    };
    ASSERT_SIZE(ListNodeMiiModelBase, 0x10);

    // m_State
    enum State : u8
    {
        STATE_NONE = 0,
        STATE_WAITING = 1,   // in the init list: the CFL model is made in the next Update
        STATE_REINIT = 2,    // drawn; the first model of the init list is made again (nothing in the known code sets it)
        STATE_READY = 3,     // the CFL model is made, it is drawn
        STATE_RELEASING = 4, // Release was called while the model was in the draw list
    };

    // what Initialize takes by value (name ours; the fields go to CFL_InitCharModel)
    struct Description
    {
        s32 resolution;     // 0x00
        u32 expressionMask; // 0x04 (bit per expression)
        u32 flags;          // 0x08
        const void* data;   // 0x0C the Mii data (CFL_InitCharModel)
        s32 expression;     // 0x10
    };
    ASSERT_SIZE(Description, 0x14);

    MiiModelBase();
    virtual ~MiiModelBase();

    // makes the CFL model (true: done)
    virtual bool InitModel() = 0; // slot 0x08
    // whether the model can be made now
    virtual bool IsReady() = 0; // slot 0x0C
    virtual CFLCharModel* GetCharModel() = 0; // slot 0x10
    // the model of the Mii index of the data of description from source (the buffers in allocator; false: no memory)
    virtual bool Initialize(nw::os::IAllocator* allocator, s32 source, u16 index, Description description); // slot 0x14
    virtual bool AllocateBuffers() = 0; // slot 0x18
    virtual void FreeBuffers() = 0; // slot 0x1C
    // (false: the model has no such expression)
    virtual bool SetExpression(const s32& expression) = 0; // slot 0x20

    // the multiplied color (r, g, b in [0, 1]; the alpha is not used)
    void SetColor(nw::ut::FloatColor color); // 0x004DF950
    // the model is drawn in this frame (if it is made)
    void RequestDraw(); // 0x004DF9A4
    // the buffers are freed (false: the model is drawn in this frame, try again in the next one)
    bool Release(); // 0x004DF9C8
    void SetScale(const nn::math::VEC3& scale); // 0x004DFA50
    void SetMatrix(const nn::math::MTX34& mtx); // 0x004DFA6C

    ListNodeMiiModelBase m_DrawNode;      // 0x04 in the draw list of the frame
    ListNodeMiiModelBase m_InitNode;      // 0x14 in the init list
    nw::os::IAllocator* m_pAllocator;     // 0x24
    const void* m_pData;                  // 0x28
    s32 m_Source;                         // 0x2C where the Mii data comes from (CFL_InitCharModel)
    s32 m_Resolution;                     // 0x30
    u32 m_ExpressionMask;                 // 0x34
    u32 m_Flags;                          // 0x38
    s32 m_InitialExpression;              // 0x3C
    s32 m_Expression;                     // 0x40
    nn::math::MTX34 m_Mtx;                // 0x44
    nn::math::VEC3 m_Scale;               // 0x74
    u32 m_Color;                          // 0x80 RGBA8 (the constant color of the third combiner stage)
    u16 m_Index;                          // 0x84
    u8 m_State;                           // 0x86 State
};
ASSERT_SIZE(MiiModelBase, 0x88);

// (inline, in the constructors of the derived models)
inline MiiModelBase::MiiModelBase() : m_DrawNode(this), m_InitNode(this)
{
    m_pAllocator = NULL;
    m_State = STATE_NONE;
    sead::Matrix34CalcCtr<f32>::makeIdentity(m_Mtx);
    m_Scale.x = 1.0f;
    m_Scale.y = 1.0f;
    m_Scale.z = 1.0f;
    m_Color = 0xFFFFFFFF;
}
} // namespace cfl
