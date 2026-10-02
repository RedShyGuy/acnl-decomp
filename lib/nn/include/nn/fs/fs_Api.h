#pragma once

#include "decomp.h"

namespace nn {
namespace fs {
void InitializeLatencyEmulation(); // 0x0011D228 | fefates:bytes [tier B]
void GetRomRequiredMemorySizeImpl(unsigned int, unsigned int, bool, const nn::fs::CTR::ProgramDataPath&); // 0x001290D8 | fefates:bytes [tier B]
void Initialize(); // 0x0012FDA0 | nintendogs:bytes-fuzzy [tier A]
void GetRomRequiredMemorySize(unsigned int, unsigned int, bool); // 0x0012FE60 | fefates:bytes [tier B]
void MountRom(unsigned int, unsigned int, void*, unsigned int, bool); // 0x0013059C | fefates:bytes [tier B]
void GetPriority(int*); // 0x0013614C | nintendogs:bytes [tier A]
void Unmount(const char*); // 0x00136254 | nintendogs:callgraph [tier A]
void MountRom(const char*, unsigned int, unsigned int, void*, unsigned int, bool); // 0x001363B4 | fefates:bytes [tier B]
void MountContent(const char*, nn::fs::MediaType, unsigned long long, unsigned int, unsigned int, unsigned int, void*, unsigned int, bool); // 0x0034601C | fefates:bytes [tier B]
void MountSaveData(const char*); // 0x003460E4 | nintendogs:callseq [tier A]
void CommitSaveData(const char*); // 0x003461B0 | fefates:bytes [tier B]
void FormatSaveData(unsigned, unsigned, bool); // 0x0034620C | nintendogs:callseq [tier A]
void IsSdmcInserted(); // 0x00346288 | nintendogs:callgraph [tier A]
void MountExtSaveData(const char*, unsigned long long); // 0x0034654C | nintendogs:callseq [tier A]
void CreateExtSaveData(unsigned long long, const void*, unsigned, unsigned, unsigned); // 0x003465E8 | nintendogs:bytes-fuzzy [tier A]
void DeleteExtSaveData(unsigned long long); // 0x0034667C | nintendogs:bytes [tier A]
void GetFileSystemSizeCore(long long*, long long*, nn::fs::SystemMediaType); // 0x00346744 | nintendogs:bytes [tier A]
void GetContentRequiredMemorySize(nn::fs::MediaType, unsigned long long, unsigned int, unsigned int, unsigned int); // 0x0034691C | fefates:bytes [tier B]
void GetSharedExtSaveDataBlockSize(long long*, long long*, int*, unsigned); // 0x00346934 | nintendogs:bytes [tier B]
} // namespace fs
} // namespace nn
