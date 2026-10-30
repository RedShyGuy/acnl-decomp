#pragma once

#include "imgdb/imgdb_Types.h"

namespace imgdb {

// the library with one allocator for everything (the NAND is not mounted)
bool Initialize(nn::fnd::IAllocator& allocator);
// mounts the archives and loads (or restores / creates) the databases of the CTR NAND and the SD card; the results
// per storage
bool InitializeEx(nn::fnd::IAllocator& allocator, nn::fnd::IAllocator& fileAllocator, bool isMountNand,
                  bool isMountSd, u32 limitFileSize, Result& nandResult, Result& sdResult);
// the allocators, the singletons and the archives
void InitializeSystem(nn::fnd::IAllocator& allocator, nn::fnd::IAllocator& fileAllocator, bool isMountNand,
                      bool isMountSd, u32 limitFileSize);
bool GetUnlimitedCtrNandFreeSpaceFlag();
// the reverse of Initialize / InitializeEx / InitializeSystem / SetAllocator (names are ours)
void Finalize();
void FinalizeEx();
void FinalizeSystem();
void DestroyRecordLinkList();
void ResetAllocator();

} // namespace imgdb
