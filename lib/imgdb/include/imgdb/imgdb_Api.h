#pragma once

#include "decomp.h"

namespace imgdb {
void Initialize(nn::fnd::IAllocator&); // 0x005A49F0 | nintendogs:bytes [tier B]
void InitializeEx(nn::fnd::IAllocator&, nn::fnd::IAllocator&, bool, bool, unsigned, imgdb::Result&, imgdb::Result&); // 0x005A57DC | nintendogs:bytes [tier B]
void SetAllocator(nn::fnd::IAllocator*, nn::fnd::IAllocator*); // 0x005A5AB8 | nintendogs:bytes [tier B]
void InitializeSystem(nn::fnd::IAllocator&, nn::fnd::IAllocator&, bool, bool, unsigned); // 0x005AACA4 | nintendogs:bytes [tier B]
} // namespace imgdb
