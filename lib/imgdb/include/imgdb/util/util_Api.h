#pragma once

// the helpers of imgdb (names from the reference symbols of nintendogs; "(name is ours)" in util_Api.cpp)

#include "imgdb/imgdb_Types.h"

namespace imgdb {

class ImageInfo;
class ImageDbRecordParam;
class JpegMpBaseDecoder;
class PictureTableRecord;

namespace util {

// a file of a DCF directory: its number (1-9999) and kind
struct FileIndexInfo
{
    u32 m_FileNo : 14;
    u32 m_ImageKind : 3;
    u32 : 15;

    bool operator<(const FileIndexInfo& other) const { return m_FileNo < other.m_FileNo; }
};
ASSERT_SIZE(FileIndexInfo, 0x4);

Result DeleteFile(StorageType storage, const wchar_t* path);
bool IsExistDir(StorageType storage, const wchar_t* path);
Result DeleteImage(StorageType storage, const ImageInfo& info, bool isForce);
s64 GetFileSize(StorageType storage, const wchar_t* path, bool* isExist);
Result SaveImageMp(StorageType storage, const ImageInfo& info, const void* mpData, u32 mpSize, const void* jpegData, u32 jpegSize);
u32 GetOwnBodyId();
void EnumImageFile(DynamicArray<FileIndexInfo, XAllocator<FileIndexInfo> >& files, StorageType storage, const wchar_t* path, ImageKindBit kinds, bool isLegacy);
void EnumImageFile(DynamicArray<FileIndexInfo, XAllocator<FileIndexInfo> >& files, StorageType storage, const wchar_t* path, ImageKind kind, bool isLegacy);
Result SaveImageJpeg(StorageType storage, const ImageInfo& info, const void* data, u32 size);
Result DeleteImageTwl(StorageType storage, const ImageInfo& info, bool isForce);
Result CreateDirectory(StorageType storage, const wchar_t* path, Allocator* allocator);
int CreateImagePath(wchar_t* buffer, int size, StorageType storage, ImageKind kind, const IndexInfo& index, bool isLegacy);
int CreateImagePath(wchar_t* buffer, int size, StorageType storage, const ImageInfo& info, bool isLegacy);
bool IsDbFileReadOnly(StorageType storage);
void EnumImageDirectory(DynamicArray<int, XAllocator<int> >& directories, StorageType storage, const wchar_t* path, bool isLegacy);
s64 GetStorageFreeSize(StorageType storage, bool isLegacy);
int CreateImageFileName(wchar_t* buffer, int size, const IndexInfo& index, bool isLegacy);
const wchar_t* GetPictureDbFileDir(StorageType storage);
const wchar_t* GetPictureDbFilePath(StorageType storage);
int GetSdDriveNameLength();
bool IsAvailableIndexInfo(StorageType storage, const IndexInfo& index, bool isLegacy);
bool CreateTwlJpegFromJpeg(void*& twlData, u32& twlSize, const void* data, u32 size);
bool CreateTwlJpegFromJpeg(void*& twlData, u32& twlSize, const void* data, u32 size, const DateTimeSeconds& dateTime);
bool FindAvailableIndexInfo(IndexInfo& index, StorageType storage, bool isLegacy, const IndexInfo& start);
int ConvertRealToVirtualPath(wchar_t* buffer, int size, const wchar_t* path);
int CreateImageDirectoryName(wchar_t* buffer, int size, const IndexInfo& index, bool isLegacy);
int CreateImageDirectoryPath(wchar_t* buffer, int size, StorageType storage, const ImageInfo& info, bool isLegacy);
int CreateImageDirectoryPath(wchar_t* buffer, int size, StorageType storage, const IndexInfo& index, bool isLegacy);
Result DeleteDirectory(StorageType storage, const wchar_t* path);
int EstimateRemainPictureNum(StorageType storage, u32 pictureSize, bool isLegacy);
int GetFileNoFromDcfFileName(const char* name);
int GetFileNoFromDcfFileName(const wchar_t* name);
bool IsAvailableIndexInfoFile(StorageType storage, const IndexInfo& index, bool isLegacy);
bool IsExistImageDataEvidence(StorageType storage);
bool ValidateDcfDirectoryName(const wchar_t* name, int* no);
int GetCtrNandDriveNameLength();
const wchar_t* GetLegacyPictureDbFileDir(StorageType storage);
int GetTwlNandDriveNameLength();
void ExtractPictureInfoFromExif(PictureTableRecord& record, const JpegMpBaseDecoder& decoder);
bool ExtractPictureInfoFromFile(PictureTableRecord& record, StorageType storage, const ImageInfo& info, bool isLegacy);
const wchar_t* GetLegacyPictureDbFilePath(StorageType storage);
int CreateImageRootDirectoryPath(wchar_t* buffer, int size, StorageType storage, bool isLegacy);
bool FindAvailableIndexInfoInNand(IndexInfo& index, const IndexInfo& start);
nn::Result ThrowFatalErrByNandOperation(nn::Result result);
void CreateDirectoryPathFromFilePath(wchar_t* buffer, int size, const wchar_t* path);
void GetImageDirectoryFlags(u8* flags, StorageType storage, bool isLegacy);
bool ValidateDcfFileNameWithoutLength(const char* name, int* no);
bool ValidateDcfFileName(const wchar_t* name, int* no);
bool ValidateDcfFileNameWithoutLength(const wchar_t* name, int* no);
void StoreImageDbRecordParam2ImageInfo(ImageInfo& info, const ImageDbRecordParam& param);
int GetDirectoryNoFromDcfDirectoryName(const char* name);
int EstimateRemainPictureNumWithFreeSize(StorageType storage, s64 freeSize, u32 pictureSize, bool isLegacy);
bool ValidateDcfDirectoryNameWithoutLength(const char* name, int* no);
bool ValidateDcfDirectoryNameWithoutLength(const wchar_t* name, int* no);
Result DeleteImageCtr(StorageType storage, const ImageInfo& info, bool isForce);
Result DeleteImageFile(StorageType storage, const ImageInfo& info, bool isLegacy, bool isDeleteEmptyDirectory);
bool AToI(int& value, const char* string, int length);
bool AToI(int& value, const wchar_t* string, int length);
int WStricmp(const wchar_t* a, const wchar_t* b);
Result SaveImage(StorageType storage, const ImageInfo& info, const void* data, u32 size, const void* subData, u32 subSize);
void GenerateImageInfoFromImageInfo(ImageInfo& info, const ImageInfo& source, bool isLegacy);
void GenerateImageInfoFromImageInfo(ImageInfo& info, const ImageInfo& source, ImageKind kind, bool isLegacy);
int CreateImageDirectoryName(wchar_t* buffer, int size, const ImageInfo& info);

} // namespace util
} // namespace imgdb
