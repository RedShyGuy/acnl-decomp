#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"
#include "sead/seadDelegateEvent.h"
#include "sead/seadMatrix.h"
#include "sead/seadTickTime.h"

namespace sead {
class Heap;

namespace ptcl {
class EmitterCalc;
class EmitterInstance;
class EmitterSet;
class PtclEditor;
class PtclInstance;
class PtclRenderer;
class PtclResource;
struct BufferLocation;
struct Handle;
struct PtclStripe;

// RTTI N4sead4ptcl10PtclSystemE @ 0x008D2024
// vtable 0x00906AAC (vptr 0x00906AB4), offset_to_top 0, 1 entries
// The particle system: pools of emitter sets, emitters, particles and stripes (the pool sizes are
// powers of two; the allocations search round robin), the resources, the calculators of the emitter
// types and the renderer. Emitters and stripes are kept in lists per group. Names other than the ones
// of the reference symbols are ours.
class PtclSystem : public ::sead::hostio::Node
{
public:
    // the sizes of the system (name and member names are ours)
    struct Config
    {
        Heap* heap;          // 0x0
        Heap* editorHeap;    // 0x4, NULL: no editor
        u8 bufferLocation;   // 0x8, of the quads: 0 main memory, 1 VRAM A, 2 VRAM B
        u8 numEmitterLog2;   // 0x9
        u8 numPtclLog2;      // 0xA
        u8 numEmitterSetLog2;// 0xB
        u8 numResourceMax;   // 0xC, the highest resource ID
        u8 numStripeLog2;    // 0xD
    };

    PtclSystem(const Config& config); // 0x0055104C (name is ours)

    // the particle resource data under resId (name is ours)
    void entryResource(Heap* heap, void* data, s32 resId, BufferLocation location); // 0x0054F97C
    void deleteResource(s32 resId); // 0x0054F92C (name is ours)
    // creates the emitters of mask of an emitter set of a resource into handle
    bool createEmitterSetID(Handle* handle, const Matrix34<f32>& mtx, s32 emitterSetId, s32 resId, u8 groupId,
                            u32 emitterMask); // 0x00550698 | mk7dlp
    // creates the emitters of set again (with the data now in the resource)
    void reCreateEmitter(EmitterSet* set, s32 resId, s32 emitterSetId, u8 groupId); // 0x0054FCC0 | mk7dlp
    // the sets that use one of the emitter data of resData again (or kills them)
    void reCreateEmitter(void** resData, s32 num, s32 resId, s32 emitterSetId, bool killOnly); // 0x005502EC | mk7dlp
    DECOMP_NOINLINE void killEmitter(EmitterInstance* emitter); // 0x0054F6EC | mk7dlp:bytes-fuzzy [tier B]
    void killEmitterGroup(u8 groupId); // 0x00550490 | mk7dlp
    void killAll(); // 0x0054FA8C (name is ours)
    // whether an emitter of the group exists (name is ours)
    bool hasEmitterGroup(u8 groupId) const; // 0x0055047C
    // takes ptcl out of the list of emitter (and its stripe out of its group; name is ours)
    void removePtcl(EmitterInstance* emitter, PtclInstance* ptcl); // 0x0054F84C
    void removeStripe(PtclStripe* stripe); // 0x0054F8A8 | mk7dlp:bytes-fuzzy [tier B]
    PtclInstance* allocPtcl(); // 0x00550F20 | mk7dlp
    PtclStripe* allocAndConnectStripe(EmitterInstance* emitter, PtclInstance* ptcl); // 0x00550B88 | mk7dlp

    // the frame of the calculations (names are ours)
    u32* beginCalc(u32* cmd); // 0x00550FD0
    u32* calcEmitterGroup(u32* cmd, u8 groupId); // 0x00550D34
    u32* endCalc(u32* cmd); // 0x00550EA4
    // the buffers of the stripe vertices: num positions, texture coordinates and directions after each other
    // in vertexBuffer, and the indices (name is ours)
    void setStripeBuffer(void* vertexBuffer, u16* indexBuffer, s32 num); // 0x0055040C
    u32* beginStripeCalc(u32* cmd); // 0x0055687C (name is ours)
    // the stripes of the group (name is ours)
    u32* renderStripeGroup(u32* cmd, u8 groupId); // 0x0054F1B4
    u32* endStripeCalc(u32* cmd); // 0x00556B38 (name is ours)

    u32* beginRender(u32* cmd, const Matrix44<f32>& proj, const Matrix34<f32>& view, const Matrix34<f32>& camera); // 0x0054F690 | mk7dlp:bytes-fuzzy [tier B]
    // the start of the frame with the billboard matrix of camera only (name is ours)
    u32* beginRenderBillboard(u32* cmd, const Matrix34<f32>& camera); // 0x00550CEC
    u32* endRender(u32* cmd); // 0x00556C10 (name is ours)

    // a list of the indices of numQuads quads with four vertices each (two triangles)
    static void initIBO(void* buffer, s32 numQuads); // 0x00550EAC | mk7dlp:bytes [tier B]

    // the handlers of the warnings (without one, the system only notes the time; name is ours)
    static DelegateEvent<const char*> s_WarningEvent;

private:
    void warn_(const char* message); // 0x00550DF4 | mk7dlp

public:
    // @fields PtclSystem
    Heap* mHeap;                             // 0x04
    PtclResource** mResources;               // 0x08
    s32 mNumResource;                        // 0x0C
    PtclEditor* mEditor;                     // 0x10
    EmitterSet* mEmitterSets;                // 0x14
    s32 mNumEmitterSet;                      // 0x18
    s32 mEmitterSetMask;                     // 0x1C
    EmitterInstance* mEmitterGroupHead[256]; // 0x20
    EmitterInstance* mEmitters;              // 0x420
    PtclInstance* mPtcls;                    // 0x424
    PtclStripe* mStripes;                    // 0x428
    PtclStripe* mStripeGroupHead[256];       // 0x42C
    u8 field_0x82C;                          // 0x82C
    u8 _pad_0x82D[0x3];
    s32 mEmitterIx;                          // 0x830
    s32 mPtclIx;                             // 0x834
    s32 mEmitterSetIx;                       // 0x838
    s32 mStripeIx;                           // 0x83C
    s32 mNumEmitter;                         // 0x840
    s32 mNumPtcl;                            // 0x844
    s32 mNumStripe;                          // 0x848
    s32 mEmitterMask;                        // 0x84C
    s32 mPtclMask;                           // 0x850
    s32 mStripeMask;                         // 0x854
    s32 mNumCalcEmitter;                     // 0x858
    s32 mNumCalcEmitterGPU;                  // 0x85C
    s32 mNumCalcPtcl;                        // 0x860
    s32 mNumCalcPtclGPU;                     // 0x864
    s32 mNumEmitterSetAlive;                 // 0x868
    s32 mNumFreeEmitter;                     // 0x86C
    s32 mNumAllocPtcl;                       // 0x870
    s32 mNumCalcStripe;                      // 0x874
    s32 mNumCalcStripeVtx;                   // 0x878
    PtclRenderer* mRenderer;                 // 0x87C
    Matrix34<f32> mViewMtx;                  // 0x880
    u32 mCreateID;                           // 0x8B0
    s32 field_0x8B4;                         // 0x8B4
    TickTime mLastWarnTick;                  // 0x8B8
    f32 field_0x8C0;                         // 0x8C0
    EmitterCalc* mEmitterCalc[3];            // 0x8C4
    // @end
};
} // namespace ptcl
} // namespace sead
