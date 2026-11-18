#pragma once

#include "decomp.h"
#include "Bank/dBankVramConfig.h"
#include "bg/bg_Extern.h"
#include "bg/dExchangeBank.h"
#include "g3d/dModel.h"
#include "nn/math/math_Vector2.h"
#include "nn/math/math_Vector3.h"
#include "sead/seadCheckedArray.h"
#include "ssys/ma/dHeapAllocator.h"

namespace g3d {
class ResourceLoader;
} // namespace g3d
namespace nw {
namespace gfx {
namespace res {
struct ResMaterialData;
} // namespace res
} // namespace gfx
} // namespace nw

// The parts of the room view of BsCharRoomViewMgr (no RTTI; all names are ours): the floor with the flooring, the four
// walls and the four landings with the wallpaper. The textures come from bg::ExchangeBank banks (the bank names are
// Japanese: "絨毯" carpet, "壁紙" wallpaper, "踊り場の壁紙" wallpaper of the landing). The directions of the walls and
// landings are 0 south, 1 east, 2 north, 3 west (after the bone and model names of the room file).

// the floor (model "carpet" of the room file, material "m_carpet")
class CharRoomFloor
{
public:
    typedef bg::ExchangeBank<BankVramConfig<66000, 1, 0, 0, false>, 66000> Bank;

    CharRoomFloor(); // 0x00582DAC

    // the heaps, the bank and the model (the height is added to the position of the bone pos_carpet; the covering is
    // not used)
    void Initialize(g3d::ResourceLoader* room, const RoomCovering* covering, f32 height); // 0x00582A2C
    // shows the flooring (true when its texture changed)
    bool Request(const RoomCovering* covering); // 0x00582518
    // (inline in BsCharRoomViewMgr::UpdateFinalize)
    void Finalize()
    {
        m_Bank.FinalizeBank();
        m_Model.Finalize();
        m_Allocator.DestroyHeap();
    }

    ssys::ma::HeapAllocator m_Allocator; // 0x000 "絨毯"
    Bank m_Bank;                         // 0x008
    g3d::Model m_Model;                  // 0x294
    nn::math::VEC3 m_Position;          // 0x2A0 the bone pos_carpet
    bool m_IsInitialized;                // 0x2AC
};
ASSERT_SIZE(CharRoomFloor, 0x2B0);

// the walls (models wall_s/e/n/w with the material m_wall; bones pos_wallS/E/N/W)
class CharRoomWall
{
public:
    typedef bg::ExchangeBank<BankVramConfig<8320, 1, 0, 0, false>, 8600> Bank;
    static const s32 PART_NUM = 4;

    struct Part
    {
        DECOMP_EMIT_COPY Part() : m_IsVisible(false), m_Angle(0), m_FadeAngle(0), m_pFadeMaterial(NULL) {} // 0x0052BB44
        DECOMP_EMIT_COPY ~Part() {} // 0x0052BB70

        // the heap ("WallParts") and the model of direction *direction
        void Initialize(const u32* direction, g3d::ResourceLoader* room, f32 height); // 0x0052B734

        bool m_IsVisible;                                   // 0x00 (false: faded out and turned away from the camera)
        ssys::ma::HeapAllocator m_Allocator;                // 0x04
        g3d::Model m_Model;                                 // 0x0C
        nn::math::VEC3 m_Normal;                           // 0x18
        nn::math::VEC3 m_Position;                         // 0x24 the bone of the wall
        s16 m_Angle;                                        // 0x30 the rotation of the model
        s16 m_FadeAngle;                                    // 0x32
        nn::math::VEC2 m_TexTranslate;                     // 0x34 of m_wall
        nw::gfx::res::ResMaterialData* m_pFadeMaterial;     // 0x3C m_fade
    };

    // (inline in the constructor of BsCharRoomViewMgr)
    CharRoomWall() : m_pWindowMaterialE(NULL), m_pWindowMaterialW(NULL), m_IsInitialized(false) {}

    // (the covering is not used)
    void Initialize(g3d::ResourceLoader* room, const RoomCovering* covering, f32 height); // 0x0052B404
    // shows the wallpaper (true when its texture changed)
    bool Request(const RoomCovering* covering); // 0x0052B284
    // the fade of the walls to the camera
    void Calc(); // 0x0052B4E0
    void Finalize(); // 0x0052B630

    Bank m_Bank;                                            // 0x000 "壁紙"
    sead::CheckedArray<Part, PART_NUM> m_Parts;             // 0x28C
    nw::gfx::res::ResMaterialData* m_pWindowMaterialE;      // 0x38C m_window of wall_e
    nw::gfx::res::ResMaterialData* m_pWindowMaterialW;      // 0x390 m_window of wall_w
    bool m_IsInitialized;                                   // 0x394
};
ASSERT_SIZE(CharRoomWall::Part, 0x40);
ASSERT_SIZE(CharRoomWall, 0x398);

// the landings (models stairs_s/e/n/w with the materials m_fade, m_fade_wall and m_stairs; bones pos_stairsS/E/N/W)
class CharRoomLanding
{
public:
    typedef bg::ExchangeBank<BankVramConfig<8320, 1, 0, 0, false>, 8600> Bank;
    static const s32 PART_NUM = 4;

    struct Part
    {
        DECOMP_EMIT_COPY Part() : m_FadeAngle(0) {} // 0x0060E084
        DECOMP_EMIT_COPY ~Part() {} // 0x0060E0A4

        // the heap ("LandingParts") and the model of direction *direction
        void Initialize(const u32* direction, g3d::ResourceLoader* room, f32 height); // 0x0060DD94

        ssys::ma::HeapAllocator m_Allocator; // 0x00
        g3d::Model m_Model;                  // 0x08
        s16 m_FadeAngle;                     // 0x14
        nn::math::VEC3 m_Position;          // 0x18 the bone of the landing
        s32 m_FadeIndex;                     // 0x24 the material m_fade (-1: none)
        s32 m_FadeWallIndex;                 // 0x28 the material m_fade_wall (-1: none)
        nn::math::VEC2 m_TexTranslate;      // 0x2C of m_fade_wall
    };

    // (inline in the constructor of BsCharRoomViewMgr)
    CharRoomLanding() : m_IsInitialized(false) {}

    // the bank and the parts; in the largest houses the landing texture of extra is used
    void Initialize(g3d::ResourceLoader* room, g3d::ResourceLoader* extra, f32 height); // 0x0060E340
    // shows the wallpaper (true when its texture changed)
    bool Request(const RoomCovering* covering); // 0x0060E0BC
    // the fade of the landings to the camera
    void Calc(); // 0x0060E61C
    void Finalize(); // 0x0060E7D4

    sead::CheckedArray<Part, PART_NUM> m_Parts; // 0x000
    Bank m_Bank;                                // 0x0D0 "踊り場の壁紙"
    bool m_IsInitialized;                       // 0x35C
};
ASSERT_SIZE(CharRoomLanding::Part, 0x34);
ASSERT_SIZE(CharRoomLanding, 0x360);
