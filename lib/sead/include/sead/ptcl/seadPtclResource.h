#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"
#include "sead/seadFixedSafeString.h"

namespace sead {
class Heap;
template <typename T>
class SafeStringBase;

namespace ptcl {
class PtclSystem;
struct SimpleEmitterData;
struct TextureHandle;
struct TextureRes;

// where the textures of a resource are: 0 where the data is, 1 VRAM A, 2 VRAM B (name is ours apart from
// the class name of the reference symbols)
struct BufferLocation
{
    u32 value;
};

// an emitter of an emitter set in the data: the offset of its data, then its address (names are ours)
struct EmitterTblData
{
    u32 dataOffset;            // 0x0, 0: none
    SimpleEmitterData* data;   // 0x4
};
ASSERT_SIZE(EmitterTblData, 0x8);

// an emitter set in the data (names are ours)
struct EmitterSetData
{
    u32 userData;          // 0x00
    u32 field_0x04;
    u32 nameOffset;        // 0x08
    const char* name;      // 0x0C
    s32 numEmitter;        // 0x10
    u32 emitterTblOffset;  // 0x14, 0: none
    u32 field_0x18;
};
ASSERT_SIZE(EmitterSetData, 0x1C);

// the start of the data of a resource ("SPBD"; names are ours)
struct ResourceHeader
{
    char magic[4];             // 0x00
    u32 version;               // 0x04
    s32 numEmitterSet;         // 0x08
    u32 field_0x0C;
    u32 strTblOffset;          // 0x10
    u32 textureTblOffset;      // 0x14
    u32 textureTblSize;        // 0x18
    EmitterSetData sets[1];    // 0x1C
};

// an emitter set of a resource as it is used now (the editor can bind other emitters; names are ours)
struct EmitterSetTbl
{
    EmitterSetData* data;          // 0x00
    EmitterTblData* emitterTbl;    // 0x04
    s32 numEmitter;                // 0x08
    u32 userData;                  // 0x0C
    EmitterSetData* orgData;       // 0x10, as in the data
    EmitterTblData* orgEmitterTbl; // 0x14
    s32 orgNumEmitter;             // 0x18
    u32 orgUserData;               // 0x1C
    const char* name;              // 0x20
    bool field_0x24;
};
ASSERT_SIZE(EmitterSetTbl, 0x28);

// what the editor bound to an emitter set, to undo it (the struct name is from the reference symbols,
// the member names are ours)
struct ResourceBind
{
    EmitterSetData* setData;     // 0x00, NULL: nothing bound
    s32 resourceId;              // 0x04
    s32 emitterSetId;            // 0x08
    const char* name;            // 0x0C
    s32 numEmitter;              // 0x10
    EmitterTblData* emitterTbl;  // 0x14
    u32 userData;                // 0x18
};
ASSERT_SIZE(ResourceBind, 0x1C);

// RTTI N4sead4ptcl12PtclResourceE @ 0x008D2060
// vtable 0x00906B0C (vptr 0x00906B14), offset_to_top 0, 3 entries
// The data of particles: the emitter sets with their emitters and the textures (member names are ours).
class PtclResource : public ::sead::hostio::Node
{
public:
    PtclResource(Heap* heap, void* data, s32 resId, PtclSystem* system, BufferLocation location); // 0x00557AA0 | mk7dlp
    virtual ~PtclResource(); // 0x00557D7C slot 0x04 | mk7dlp
    // 0x00557D6C slot 0x08 (deleting dtor)

    // the emitters of the editor for an emitter set (bind keeps what was there)
    void bindTarget(s32 emitterSetId, ResourceBind* bind, EmitterTblData* emitterTbl, s32 numEmitter,
                    const SafeStringBase<char>& name, u32 userData); // 0x00557578 | mk7dlp:bytes [tier B]
    // what bind kept back; creates the emitters again and/or kills them
    void unbind(ResourceBind* bind, bool reCreate, bool kill); // 0x00557988 | mk7dlp:bytes [tier B]
    // the data of a resource with num empty emitter sets for the editor
    static void* createEmtpryData(Heap* heap, s32 num); // 0x00557698 | mk7dlp
    static TextureHandle* createTextureHandle(Heap* heap, void* image, const TextureRes& res); // 0x00557868 | mk7dlp

    PtclSystem* mSystem;              // 0x04
    s32 mResourceID;                  // 0x08
    ResourceHeader* mHeader;          // 0x0C
    const char* mStrTbl;              // 0x10
    u8* mTextureTbl;                  // 0x14
    EmitterSetTbl* mEmitterSetTbl;    // 0x18
    FixedSafeString<128> mName;       // 0x1C
};
ASSERT_SIZE(PtclResource, 0xA8);
} // namespace ptcl
} // namespace sead
