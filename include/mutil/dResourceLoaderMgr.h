#pragma once

#include "decomp.h"
#include "g3d/dResourceLoader.h"
#include "mutil/dBaseResourceMgr.h"

// the managers of graphics files and the instances whose functions are out of line in the original (their copies lie
// in the template area; the other instances are inline everywhere)
namespace mutil {
template <>
DECOMP_NOINLINE void BaseResourceMgr<g3d::ResourceLoader, 2>::Initialize(sead::Heap* parent, u32 size);
template <>
DECOMP_NOINLINE ResourceKey BaseResourceMgr<g3d::ResourceLoader, 2>::Load(const char* name);
template <>
DECOMP_NOINLINE ResourceKey BaseResourceMgr<g3d::ResourceLoader, 3>::Load(const char* name);
template <>
DECOMP_NOINLINE ResourceKey BaseResourceMgr<g3d::ResourceLoader, 4>::Load(const char* name);
} // namespace mutil
