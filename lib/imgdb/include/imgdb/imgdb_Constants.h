#pragma once

// the names of the files and directories of imgdb (pointers in .data; names are ours)

#include "imgdb/imgdb_Types.h"

namespace imgdb {

extern const wchar_t* s_LegacyPhotoDirectory;   // L"photo/private/ds/app/484E494A" (the DSi camera)
extern const wchar_t* s_PictureDbFileName;      // L"pit.bin"
extern const wchar_t* s_PhotoCacheFileName;     // L"phtcache.bin"
extern const char* s_DirectoryMaker;            // "NIN"
extern const wchar_t* s_DirectorySuffixW;       // L"NIN03" (the directories of the 3DS)
extern const char* s_DirectorySuffix;           // "NIN03"
extern const wchar_t* s_LegacyDirectorySuffixW; // L"NIN02" (the directories of the DSi)
extern const char* s_LegacyDirectorySuffix;     // "NIN02"
extern const wchar_t* s_FileNamePrefixW;        // L"HNI_"
extern const char* s_FileNamePrefix;            // "HNI_"
extern const wchar_t* s_ImageExtensions[IMAGE_KIND_NUM]; // by ImageKind: L"", L".JPG", L".MPO", L".AVI"
extern const u32 s_ImageExtensionLengths[IMAGE_KIND_NUM]; // their lengths

// the archive names of the paths (0x0097F0C0..; names are ours)
extern const wchar_t* s_SdDriveNameW;          // L"$phtsd"
extern const wchar_t* s_DriveNamesW[4];        // L"$phtsd:", L"$_sdmc:", L"$_phtsh" (CTR NAND), L"twlp" (DSi photos)

// the directories of the databases and of the images on the CTR NAND and the SD card
extern const wchar_t* s_CtrNandPictureDbDirW;  // L"photo/Nintendo 3DS/Private/00020400"
extern const char* s_CtrNandPictureDbDir;
extern const wchar_t* s_SdPictureDbDirW;       // L"Nintendo 3DS/Private/00020400"
extern const char* s_SdPictureDbDir;
extern const wchar_t* s_CtrNandImageRootW;     // L"photo/DCIM"
extern const char* s_CtrNandImageRoot;
extern const wchar_t* s_SdImageRootW;          // L"DCIM"
extern const char* s_SdImageRoot;
// the same by StorageType (built by the static initializer)
extern const wchar_t* s_PictureDbDirsW[2];
extern const char* s_PictureDbDirs[2];
extern const wchar_t* s_ImageRootsW[2];
extern const char* s_ImageRoots[2];
extern const wchar_t* s_ImageRootDirectoriesW[2];
extern const char* s_ImageRootDirectories[2];
extern const u32 s_ImageRootDirectoryLengths[2];

} // namespace imgdb
