#pragma once

#include "decomp.h"
#include "Object/dObjectResource.h"
#include "g3d/dSkeletalAnim.h"
#include "g3d/dResourceLoader.h"
#include "g3d/dSkeletalModel.h"

// The resource of an object with a skeletal model from a file of LoadFile (the derived classes, or the owner's): the
// first model of the file with its first skeletal animation; SetAnim switches to the animation of a kind by name
// (names are ours; the kinds after the animation names of the table).
// RTTI 19ResourceGetSkeletal @ 0x008CCA5C
// vtable 0x008F52A4 (vptr 0x008F52AC), offset_to_top 0, 9 entries
// vtable 0x008F52F8 (vptr 0x008F5300), offset_to_top -48, 11 entries
// (the other vtables are the construction vtables of the derived classes)
class ResourceGetSkeletal : public virtual ::ObjectResource
{
public:
    // the animations by their names in the files (s_AnimNames; the same name can appear twice)
    enum AnimKind
    {
        ANIM_WAIT = 0,
        ANIM_STOP = 1,
        ANIM_FLY = 2,
        ANIM_GET = 3,
        ANIM_BOOK = 4,
        ANIM_SWIM = 5,
        ANIM_BOOK_6 = 6,
        ANIM_GET_7 = 7,
        ANIM_DIE0 = 8,
        ANIM_DIE1 = 9,
        ANIM_START = 10,
        ANIM_WAIT_11 = 11,
        ANIM_FINISH = 12,
        ANIM_INOUT = 13,
        ANIM_WAIT_14 = 14,
        ANIM_OUT = 15,
        ANIM_ATTACK = 16,
        ANIM_IN = 17,
        ANIM_INOUT_18 = 18,
        ANIM_ESCAPE = 19,
        ANIM_DEFAULT = 20,
        ANIM_BREAK = 21,
        ANIM_NUM = 22, // none (m_AnimKind before SetAnim)
    };

    ResourceGetSkeletal(); // 0x002FE20C
    virtual ~ResourceGetSkeletal(); // 0x002FE28C slot 0x00
    // 0x002FE250 (deleting dtor)
    bool Create(const char* path) override; // 0x002FDC48 slot 0x08
    void Destroy() override; // 0x002FDE2C slot 0x0C
    void Update(const nn::math::MTX34& matrix) override; // 0x002FD090 slot 0x10
    void UpdateFrames() override; // 0x002FD3DC slot 0x14
    void AddToScene() override; // 0x002FD3D0 slot 0x18
    // the model into the second scene of g3d
    virtual void AddToSubScene(); // 0x002FDF18 slot 0x1C
    g3d::Model* GetModel() override; // 0x002FDF24 slot 0x20

    // the skeletal animation of the kind (nothing for the current kind or a missing animation)
    void SetAnim(u32 kind); // 0x002FDEA8
    void SetAnimFrame(f32 frame); // 0x002FDF2C
    void SetAnimSpeed(f32 speed); // 0x002FDF44
    // the alpha of the constant color 1 of the material "m_main" (and of "hire") of the model (isOriginal: of the
    // resource of the material, else of its buffer)
    void SetMainAlpha(s32 alpha, bool withHire, bool isOriginal); // 0x002FDF90
    // the same through Func174BB0 (only the first two materials, of the buffer)
    void SetMainAlphaOfBuffer(s32 alpha); // 0x002FE1C0
    // the alpha of the constant color 0 of the material "sdw_sea" of a model (of the buffer)
    static void SetSeaShadowAlpha(g3d::Model* model, s32 alpha); // 0x002FE1E8

    // the names of the kinds
    static const char* s_AnimNames[ANIM_NUM];

protected:
    // (inline) the model of the first entry of the file and its first skeletal animation (if the file has one)
    void CreateModelAndSkeletalAnim();

public:
    g3d::SkeletalModel m_Model;        // 0x04
    g3d::SkeletalAnim m_SkeletalAnim;  // 0x10
    u32 m_AnimKind;                    // 0x28 AnimKind
    u8 m_PlayMode;                     // 0x2C g3d::FrameController (1: loop)
    bool m_IsUpdateAnims;              // 0x2D Update updates the animations
};

DECOMP_ALWAYS_INLINE void ResourceGetSkeletal::CreateModelAndSkeletalAnim()
{
    m_Model.Create(nw::gfx::res::ResSceneObject(m_pResourceLoader->GetGraphicsFile().GetModels(0)), &m_Allocator,
                   &m_Allocator, m_BufferOption, true, 1);
    if (m_pResourceLoader->GetGraphicsFile().GetSkeletalAnimsCount() != 0) {
        m_SkeletalAnim.Build(&m_Model,
                             nw::anim::res::ResAnim(m_pResourceLoader->GetGraphicsFile().GetSkeletalAnims(0)),
                             &m_Allocator, 0);
        m_Model.SetSkeletalAnim(&m_SkeletalAnim);
        m_SkeletalAnim.SetPlayMode(m_PlayMode);
        if (m_SkeletalAnim.m_pAnimObject != NULL) {
            m_SkeletalAnim.SetSpeed(1.0f);
        }
    }
}

// (game code that is not decompiled yet; name after the address) the alpha of the constant color colorIdx of the
// material of the name (only the first two materials) of the resource (isOriginal) or of the buffer of the material
void Func174BB0(g3d::Model* model, s32 alpha, const char* materialName, bool isOriginal, s32 colorIdx); // 0x00174BB0
