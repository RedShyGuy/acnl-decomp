#include "imgdb/imgdb_Constants.h"

namespace imgdb {

// 0x00982690
const wchar_t* s_LegacyPhotoDirectory = L"photo/private/ds/app/484E494A";
// 0x00982694
const wchar_t* s_PictureDbFileName = L"pit.bin";
// 0x00982698
const wchar_t* s_PhotoCacheFileName = L"phtcache.bin";
// 0x0098269C
const char* s_DirectoryMaker = "NIN";
// 0x009826A0
const wchar_t* s_DirectorySuffixW = L"NIN03";
// 0x009826A4
const char* s_DirectorySuffix = "NIN03";
// 0x009826A8
const wchar_t* s_LegacyDirectorySuffixW = L"NIN02";
// 0x009826AC
const char* s_LegacyDirectorySuffix = "NIN02";
// 0x009826B0
const wchar_t* s_FileNamePrefixW = L"HNI_";
// 0x009826B4
const char* s_FileNamePrefix = "HNI_";
// 0x009826B8
const wchar_t* s_ImageExtensions[IMAGE_KIND_NUM] = {L"", L".JPG", L".MPO", L".AVI"};

// 0x008C2464
extern const u32 s_ImageExtensionLengths[IMAGE_KIND_NUM] = {0, 4, 4, 4};

// 0x0097F0C0
const wchar_t* s_SdDriveNameW = L"$phtsd";
// 0x0097F0D0
const wchar_t* s_DriveNamesW[4] = {L"$phtsd:", L"$_sdmc:", L"$_phtsh", L"twlp"};

// 0x00982640
const wchar_t* s_CtrNandPictureDbDirW = L"photo/Nintendo 3DS/Private/00020400";
// 0x00982644
const char* s_CtrNandPictureDbDir = "photo/Nintendo 3DS/Private/00020400";
// 0x00982648
const wchar_t* s_SdPictureDbDirW = L"Nintendo 3DS/Private/00020400";
// 0x0098264C
const char* s_SdPictureDbDir = "Nintendo 3DS/Private/00020400";
// 0x00982650
const wchar_t* s_CtrNandImageRootW = L"photo/DCIM";
// 0x00982654
const char* s_CtrNandImageRoot = "photo/DCIM";
// 0x00982658
const wchar_t* s_SdImageRootW = L"DCIM";
// 0x0098265C
const char* s_SdImageRoot = "DCIM";

// the tables by StorageType copy the pointers above
// 0x00793214 (static initializer)
// 0x00982660
const wchar_t* s_PictureDbDirsW[2] = {s_CtrNandPictureDbDirW, s_SdPictureDbDirW};
// 0x00982668
const char* s_PictureDbDirs[2] = {s_CtrNandPictureDbDir, s_SdPictureDbDir};
// 0x00982670
const wchar_t* s_ImageRootsW[2] = {s_CtrNandImageRootW, s_SdImageRootW};
// 0x00982678
const char* s_ImageRoots[2] = {s_CtrNandImageRoot, s_SdImageRoot};
// 0x00982680
const wchar_t* s_ImageRootDirectoriesW[2] = {s_CtrNandImageRootW, s_SdImageRootW};
// 0x00982688
const char* s_ImageRootDirectories[2] = {s_CtrNandImageRoot, s_SdImageRoot};
// 0x008C245C
extern const u32 s_ImageRootDirectoryLengths[2] = {10, 4};

} // namespace imgdb
