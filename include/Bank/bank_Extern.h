#pragma once

#include "decomp.h"

namespace g3d {
class ResourceLoader;
} // namespace g3d

// Game code that the banks call and that is not decompiled yet (names after the addresses).

// shared graphics files that the models of the banks are set up against
g3d::ResourceLoader* Func5C22D8(); // 0x005C22D8
g3d::ResourceLoader* Func536700(); // 0x00536700
// the light set index of the models of the banks
s32 Func1E891C(); // 0x001E891C

class SvFgName;
// the path of the model file of an item (a function-local static string; NULL: no model)
const char* Func179978(const SvFgName* item); // 0x00179978
// the path of the model file of a bottoms item (a function-local static string; NULL: no model)
const char* Func17C3D0(const SvFgName* item); // 0x0017C3D0
// a type of an item (0..7; PlayerBottomsBank loads only items of type 4, others are replaced)
u32 Func76BB44(const SvFgName* item); // 0x0076BB44
// another shared graphics file (PlayerLegsBank without a socks item)
g3d::ResourceLoader* Func5C6794(); // 0x005C6794
struct SvPlayerAppearance;
// the eye color (below 6) and the hair color (below 16) of an appearance (0 otherwise)
u8 Func713778(const SvPlayerAppearance* appearance); // 0x00713778
u8 Func713788(const SvPlayerAppearance* appearance); // 0x00713788
struct ExchangeBankKeywordUnknown3;
namespace nw {
namespace gfx {
namespace res {
struct ResModelData;
} // namespace res
} // namespace gfx
} // namespace nw
// the path of the model file of a tops item (a function-local static string; NULL: no model)
const char* Func1768C8(const SvFgName* item); // 0x001768C8
// whether an item can carry a My Design (for the tops)
bool Func76BA5C(const SvFgName* item); // 0x0076BA5C
// the design item of an item (out: the item; empty: none)
void Func76C8E0(SvFgName* out, const SvFgName* item); // 0x0076C8E0
// a number of an item (PlayerTopsBank takes it modulo 10 as the index of the design)
u32 Func76CAE0(const SvFgName* item); // 0x0076CAE0
// the design index of an owner (the value at +4 of the My Design keyword)
const void* Func7133B4(const void* owner, u32 index); // 0x007133B4

class ExchangeModelBankBase;
namespace g3d {
class SkeletalModel;
} // namespace g3d
namespace nn {
namespace math {
struct MTX34;
} // namespace math
} // namespace nn
// the world matrix of bone of model (the first argument is not used; isVisible: set from a flag of the bone)
const nn::math::MTX34* Func7258B0(const ExchangeModelBankBase* unused, g3d::SkeletalModel* model, u32 bone,
                                  bool* isVisible); // 0x007258B0
// the shown model of a bank to a scene (sceneIdx < 3)
void Func328698(ExchangeModelBankBase* bank, s32 sceneIdx); // 0x00328698
// the shown model of a bank at bone of model
void Func3286CC(ExchangeModelBankBase* bank, g3d::SkeletalModel* model, u32 bone); // 0x003286CC
// the ambient and diffuse value of the shown model resource index of a bank (false: none)
bool Func328808(ExchangeModelBankBase* bank, u32 index, f32 value); // 0x00328808

