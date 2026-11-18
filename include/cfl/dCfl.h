#pragma once

#include "decomp.h"
#include "cfl/cfl_Net.h"

namespace sead {
class Heap;
} // namespace sead

#include "nw/gfx/res/gfx_ResFragmentShader.h"

namespace cfl {
class MiiIcon;

// The Mii module of the game (file name after the heaps "dCfl"/"dCflGraphics"; all function names are ours): it starts
// CFL with its resource archive and shader (CFL/CflShader.shbin), makes the queued Mii models and icons in Update and
// draws the models of the frame in the passes of g3d (a g3d::Func in scene 0), lit by the fragment lights of the scene.

// starts CFL and the module in heaps under parent; always true
bool Initialize(sead::Heap* parent); // 0x004DF670
// the network information of CFL (2 players, 1 Mii each)
CFLNetInformation* GetNetInformation(); // 0x004DFE88
// loads the shader and binds its symbols; always true
bool InitializeShader(); // 0x004DFE94
// the drawing node joins scene 0
void AddToScene(); // 0x004E10D0
// makes the queued models and icons (once per frame)
void Update(); // 0x004E10E0
// whether CFL_InitRes formatted a broken database
bool IsDatabaseFormatted(); // 0x004E03E0
// the lighting settings and lookup tables of the fragment shader for the models
void SetFragmentShader(nw::gfx::res::ResFragmentShader shader); // 0x004E03F0
// whether the official Mii index is available
bool IsAvailableOfficialData(u16 index); // 0x004E0910
// the icon is drawn in the next Update
void AddIcon(MiiIcon* icon); // 0x004E1050
} // namespace cfl
