#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cwchar>

#include "imgdb/util/util_Api.h"
#include "imgdb/util/util_Internal.h"
#include "imgdb/CtrNandArchiveMounter.h"
#include "imgdb/Directory.h"
#include "imgdb/ImageDatabase.h"
#include "imgdb/ImageDbRecordParam.h"
#include "imgdb/ImageFileWriter.h"
#include "imgdb/ImageInfo.h"
#include "imgdb/JpegDecoder.h"
#include "imgdb/JpegEncoder.h"
#include "imgdb/MpDecoder.h"
#include "imgdb/PhotoHeaderImageInfoReader.h"
#include "imgdb/PictureTableRecord.h"
#include "imgdb/SdArchiveMounter.h"
#include "imgdb/SysMakerNote.h"
#include "imgdb/TwlNandArchiveMounter.h"
#include "imgdb/UserMakerNote.h"
#include "imgdb/imgdb_Api.h"
#include "imgdb/imgdb_Constants.h"
#include "nn/cfg/CTR/CTR_Api.h"
#include "nn/err/CTR/CTR_Api.h"
#include "nn/fs/detail/detail_Api.h"
#include "nn/fs/fs_FileInputStream.h"

namespace imgdb {
namespace util {

namespace {

// the paths are built when they are first needed (names are ours)
// 0x0097F104
bool s_IsTwlNandDriveNameReady;
// 0x0097F105
bool s_IsCtrNandDriveNameReady;
// 0x0097F106
bool s_IsSdDriveNameReady;
// 0x0097F107
bool s_IsTwlNandDriveNameLengthReady;
// 0x0097F108
bool s_IsCtrNandDriveNameLengthReady;
// 0x0097F109
bool s_IsSdDriveNameLengthReady;
// 0x0097F10A
bool s_IsLegacyPictureDbFilePathReady;
// 0x0097F10B
bool s_IsPictureDbFilePathReady;
// 0x0097F10C
bool s_IsLegacyPictureDbFileDirReady;
// 0x0097F10D
bool s_IsPictureDbFileDirReady;
// 0x0097F110
int s_TwlNandDriveNameLength;
// 0x0097F114
int s_CtrNandDriveNameLength;
// 0x0097F118
int s_SdDriveNameLength;

// 0x00AEF1EC
wchar_t s_TwlNandDriveName[11];
// 0x00AEF202
wchar_t s_CtrNandDriveName[11];
// 0x00AEF218
wchar_t s_SdDriveName[11];
// 0x00AEF22E
wchar_t s_TwlNandLegacyPictureDbFilePath[59];
// 0x00AEF2A4
wchar_t s_SdLegacyPictureDbFilePath[59];
// 0x00AEF31A
wchar_t s_CtrNandPictureDbFilePath[59];
// 0x00AEF390
wchar_t s_SdPictureDbFilePath[59];
// 0x00AEF406
wchar_t s_TwlNandLegacyPictureDbFileDir[40];
// 0x00AEF456
wchar_t s_SdLegacyPictureDbFileDir[40];
// 0x00AEF4A6
wchar_t s_CtrNandPictureDbFileDir[46];
// 0x00AEF502
wchar_t s_SdPictureDbFileDir[46];

inline bool IsTwlNandMounted()
{
    return TwlNandArchiveMounter::GetInstance()->IsMount();
}

// "<archive>:/"
inline const wchar_t* GetTwlNandDriveName()
{
    if (!s_IsTwlNandDriveNameReady)
    {
        std::wcscpy(s_TwlNandDriveName, s_DriveNamesW[3]);
        std::wcscat(s_TwlNandDriveName, L":/");
        s_IsTwlNandDriveNameReady = true;
    }
    return s_TwlNandDriveName;
}

inline const wchar_t* GetCtrNandDriveName()
{
    if (!s_IsCtrNandDriveNameReady)
    {
        std::wcscpy(s_CtrNandDriveName, s_DriveNamesW[2]);
        std::wcscat(s_CtrNandDriveName, L":/");
        s_IsCtrNandDriveNameReady = true;
    }
    return s_CtrNandDriveName;
}

inline const wchar_t* GetSdDriveName()
{
    if (!s_IsSdDriveNameReady)
    {
        std::wcscpy(s_SdDriveName, s_SdDriveNameW);
        std::wcscat(s_SdDriveName, L":/");
        s_IsSdDriveNameReady = true;
    }
    return s_SdDriveName;
}

// the length of "$phtsd:" and "$_sdmc:"
const int DRIVE_NAME_LENGTH = 7;
// the directory flags: a bit per directory index "a DCF directory has the number", then a bit "it has the suffix
// of the storage"
const int DIRECTORY_FLAGS_SIZE = 113;
// a bit per file index of a directory the database writes
const int FILE_FLAGS_SIZE = 13;

inline bool TestBit(const u8* bits, int index)
{
    return (bits[index / 8] & (1 << (index % 8))) != 0;
}

inline void SetBit(u8* bits, int index)
{
    bits[index / 8] |= 1 << (index % 8);
}

inline void ClearBit(u8* bits, int index)
{
    bits[index / 8] &= ~(1 << (index % 8));
}

// a number no DCF directory has, or one whose directory has the suffix of the storage
inline bool IsUsableDirectoryIndex(const u8* flags, int index)
{
    return !TestBit(flags, index) || TestBit(flags + DIRECTORY_FLAGS_SIZE, index);
}

// the characters of DCF names
template <typename T>
inline bool IsDcfCharacter(T c)
{
    return static_cast<u32>(c - '0') <= 9 || static_cast<u32>(c - 'A') <= 25 || static_cast<u32>(c - 'a') <= 25 ||
           c == '_';
}

// a decimal number of length digits, -1 for another character
template <typename T>
inline int ParseDcfNumber(const T* string, int length)
{
    int value = 0;
    for (int i = 0; i < length; i++)
    {
        int digit = string[i] - '0';
        if (static_cast<u32>(digit) >= 10)
        {
            return -1;
        }
        value = value * 10 + digit;
    }
    return value;
}

// the files of the directory of an index the database writes (numbers 1-100)
inline void GetImageFileFlags(u8* flags, StorageType storage, const IndexInfo& index, bool isLegacy)
{
    wchar_t directoryPath[30];
    CreateImageDirectoryPath(directoryPath, 30, storage, index, isLegacy);
    Directory directory(*GetAllocator(), storage, directoryPath, 6000);
    if (!directory.IsOpen())
    {
        return;
    }
    while (directory.Read())
    {
        if (!directory.IsFile())
        {
            continue;
        }
        int no;
        if (ValidateDcfFileName(directory.GetEntry()->name, &no) &&
            static_cast<u32>(no - 1) < static_cast<u32>(IndexInfo::OFFICIAL_FILE_NUM))
        {
            SetBit(flags, no - 1);
        }
    }
}

// the size of the photos of the camera that the DSi camera can show
const u32 TWL_JPEG_WIDTH = 640;
const u32 TWL_JPEG_HEIGHT = 480;

} // namespace

// (a file that does not exist counts as deleted)
// 0x005ABFB0 | nintendogs:callgraph [tier C]
Result DeleteFile(StorageType storage, const wchar_t* path)
{
    nn::Result result = nn::fs::detail::GetGlobalFileSystemBase()->TryDeleteFile(path);
    if (result.IsSuccess())
    {
        return RESULT_SUCCESS;
    }
    if (storage == STORAGE_TYPE_CTR_NAND)
    {
        ThrowFatalErrByNandOperation(result);
    }
    if (IsFsDescription(result, 260, 279))
    {
        return RESULT_FS_ACCESS_ERROR;
    }
    if (IsFsDescription(result, 130, 179))
    {
        return RESULT_FS_NOT_FOUND;
    }
    if (IsFsDescription(result, 280, 334))
    {
        return RESULT_FS_ARCHIVE_ERROR;
    }
    if (IsFsDescription(result, 100, 179))
    {
        return RESULT_SUCCESS;
    }
    return RESULT_FS_ERROR;
}

// 0x005AC08C | nintendogs:bytes [tier B]
bool IsExistDir(StorageType storage, const wchar_t* path)
{
    nn::fs::Directory directory;
    nn::Result result = directory.TryInitialize(path);
    if (result.IsSuccess())
    {
        directory.Finalize();
    }
    else if (storage == STORAGE_TYPE_CTR_NAND)
    {
        ThrowFatalErrByNandOperation(result);
    }
    return result.IsSuccess();
}

// (with the copy in the DSi photo archive)
// 0x005AC0F4 | nintendogs:bytes [tier B]
Result DeleteImage(StorageType storage, const ImageInfo& info, bool isDeleteEmptyDirectory)
{
    Result result = RESULT_NOT_READY;
    if (info.GetImageKind() == IMAGE_KIND_JPEG)
    {
        result = DeleteImageFile(storage, info, false, isDeleteEmptyDirectory);
        if (storage == STORAGE_TYPE_CTR_NAND && IsTwlNandMounted())
        {
            ImageInfo legacyInfo;
            GenerateImageInfoFromImageInfo(legacyInfo, info, true);
            Result legacyResult = DeleteImageFile(storage, legacyInfo, true, isDeleteEmptyDirectory);
            if (legacyResult != RESULT_SUCCESS)
            {
                result = legacyResult;
            }
        }
    }
    else if (info.GetImageKind() == IMAGE_KIND_MP)
    {
        result = DeleteImageFile(storage, info, false, isDeleteEmptyDirectory);
        if (storage == STORAGE_TYPE_CTR_NAND && IsTwlNandMounted())
        {
            ImageInfo legacyInfo;
            GenerateImageInfoFromImageInfo(legacyInfo, info, IMAGE_KIND_JPEG, true);
            DeleteImageFile(storage, legacyInfo, true, isDeleteEmptyDirectory);
        }
    }
    else if (info.GetImageKind() == IMAGE_KIND_AVI)
    {
        result = DeleteImageFile(storage, info, false, isDeleteEmptyDirectory);
    }
    return result;
}

// 0x005AC250 | nintendogs:callgraph [tier C]
s64 GetFileSize(StorageType storage, const wchar_t* path, bool* isExist)
{
    s64 size = 0;
    nn::fs::FileInputStream file;
    bool exist = true;
    nn::Result result = file.TryInitialize(path);
    if (result.IsSuccess())
    {
        result = file.TryGetSize(&size);
        file.Finalize();
    }
    if (result.IsFailure())
    {
        size = 0;
        exist = false;
        if (storage == STORAGE_TYPE_CTR_NAND)
        {
            ThrowFatalErrByNandOperation(result);
        }
    }
    if (isExist != NULL)
    {
        *isExist = exist;
    }
    return size;
}

// the MPO, its left image as JPEG on the SD card or in the DSi photo archive (from the MPO when the caller has none)
// 0x005AC378 | nintendogs:bytes [tier B]
Result SaveImageMp(StorageType storage, const ImageInfo& info, const void* mpData, u32 mpSize, const void* jpegData,
                   u32 jpegSize)
{
    Result result = RESULT_SUCCESS;
    bool isAllocated = false;
    void* jpeg = const_cast<void*>(jpegData);
    if (jpeg == NULL || jpegSize == 0)
    {
        MpDecoder decoder(mpData, mpSize);
        u32 size = decoder.ComputeExtractJpegSize();
        jpeg = GetAllocator()->Allocate(size, 32);
        isAllocated = true;
        if (!decoder.ExtractJpegL(jpeg, size, jpegSize))
        {
            result = RESULT_INVALID_DATA;
        }
    }

    if (result == RESULT_SUCCESS)
    {
        ImageFileWriter writer(*GetFileAllocator(), storage, info, false, mpData, mpSize);
        result = writer.WriteImage();
    }
    if (result == RESULT_SUCCESS)
    {
        if (storage == STORAGE_TYPE_SD)
        {
            ImageInfo jpegInfo;
            GenerateImageInfoFromImageInfo(jpegInfo, info, IMAGE_KIND_JPEG, false);
            ImageFileWriter writer(*GetFileAllocator(), storage, jpegInfo, false, jpeg, jpegSize);
            result = writer.WriteImage();
        }
        else if (storage == STORAGE_TYPE_CTR_NAND && IsTwlNandMounted())
        {
            ImageInfo legacyInfo;
            GenerateImageInfoFromImageInfo(legacyInfo, info, IMAGE_KIND_JPEG, true);
            u32 twlSize = 0;
            void* twlData = NULL;
            if (CreateTwlJpegFromJpeg(twlData, twlSize, jpeg, jpegSize))
            {
                ImageFileWriter writer(*GetFileAllocator(), storage, legacyInfo, true, twlData, twlSize);
                writer.WriteImage();
            }
            if (twlData != NULL)
            {
                GetAllocator()->Free(twlData);
            }
        }
    }
    if (isAllocated)
    {
        GetAllocator()->Free(jpeg);
        jpegSize = 0;
    }

    if (result != RESULT_SUCCESS)
    {
        DeleteImage(storage, info, true);
    }
    return result;
}

// the id of this console (transferable id 0x204), read once
// 0x005AC654 | nintendogs:bytes [tier B]
u32 GetOwnBodyId()
{
    // 0x0097F100 (guard 0x0097F0FC)
    static u32 s_BodyId = static_cast<u32>(nn::cfg::CTR::GetTransferableId(0x204));
    return s_BodyId;
}

// the image files "HNI_nnnn.ext" of a directory, sorted by their number (one entry per number: an MPO or AVI
// replaces the JPEG of the same number)
// 0x005AC688 | nintendogs:bytes-fuzzy [tier B]
void EnumImageFile(DynamicArray<FileIndexInfo, XAllocator<FileIndexInfo> >& files, StorageType storage,
                   const wchar_t* path, ImageKindBit kinds, bool isLegacy)
{
    files.Clear();
    files.SetMinCapacity(100);
    files.Refresh();

    Directory directory(*GetAllocator(), storage, path, 6000);
    if (!directory.IsOpen())
    {
        return;
    }
    while (directory.Read())
    {
        if (!directory.IsFile())
        {
            continue;
        }
        const wchar_t* name = directory.GetEntry()->name;
        int length = std::wcslen(name);
        int no;
        if (length < 8 || std::wcsncmp(name, s_FileNamePrefixW, 4) != 0 || !AToI(no, name + 4, 4) ||
            !IndexInfo::IsValidFileIndex(no - 1))
        {
            continue;
        }
        for (int i = 0; i < IMAGE_KIND_NUM; i++)
        {
            if ((kinds & (1 << i)) == 0)
            {
                continue;
            }
            u8 kind = i;
            int extLength = s_ImageExtensionLengths[kind];
            if (extLength + 8 != length ||
                std::wcsncmp(name + length - extLength, s_ImageExtensions[kind], extLength) != 0)
            {
                continue;
            }

            FileIndexInfo* file = files.begin();
            for (; file != files.end(); file++)
            {
                if (file->m_FileNo == static_cast<u32>(no))
                {
                    break;
                }
            }
            if (file != files.end())
            {
                if (file->m_ImageKind == IMAGE_KIND_JPEG)
                {
                    file->m_ImageKind = kind;
                }
            }
            else
            {
                FileIndexInfo info = {};
                info.m_FileNo = no;
                info.m_ImageKind = kind;
                files.PushBack(info);
            }
            break;
        }
    }
    std::sort(files.begin(), files.end());
}

// 0x005ACB34 | nintendogs:bytes [tier B]
void EnumImageFile(DynamicArray<FileIndexInfo, XAllocator<FileIndexInfo> >& files, StorageType storage,
                   const wchar_t* path, ImageKind kind, bool isLegacy)
{
    EnumImageFile(files, storage, path, static_cast<ImageKindBit>(1 << kind), isLegacy);
}

// the JPEG and its copy in the DSi photo archive
// 0x005ACB54 | nintendogs:bytes [tier B]
Result SaveImageJpeg(StorageType storage, const ImageInfo& info, const void* data, u32 size)
{
    Result result;
    {
        ImageFileWriter writer(*GetFileAllocator(), storage, info, false, data, size);
        result = writer.WriteImage();
    }
    if (result == RESULT_SUCCESS && storage == STORAGE_TYPE_CTR_NAND && IsTwlNandMounted())
    {
        u32 twlSize = 0;
        void* twlData = NULL;
        if (CreateTwlJpegFromJpeg(twlData, twlSize, data, size))
        {
            ImageInfo legacyInfo;
            GenerateImageInfoFromImageInfo(legacyInfo, info, IMAGE_KIND_JPEG, true);
            ImageFileWriter writer(*GetFileAllocator(), storage, legacyInfo, true, twlData, twlSize);
            writer.WriteImage();
        }
        if (twlData != NULL)
        {
            GetAllocator()->Free(twlData);
        }
    }
    if (result != RESULT_SUCCESS)
    {
        DeleteImage(storage, info, true);
    }
    return result;
}

// 0x005ACD98 | nintendogs:callgraph [tier C]
Result DeleteImageTwl(StorageType storage, const ImageInfo& info, bool isDeleteEmptyDirectory)
{
    return DeleteImageFile(storage, info, true, isDeleteEmptyDirectory);
}

// every missing directory of the path
// 0x005ACDA4 | nintendogs:callgraph [tier C]
Result CreateDirectory(StorageType storage, const wchar_t* path, Allocator* allocator)
{
    Result result = RESULT_SUCCESS;
    if (allocator == NULL)
    {
        allocator = GetAllocator();
    }
    u32 size = (std::wcslen(path) + 1) * sizeof(wchar_t);
    wchar_t* tokens = static_cast<wchar_t*>(allocator->Allocate(size, 32));
    wchar_t* current = static_cast<wchar_t*>(allocator->Allocate(size, 32));
    std::wcscpy(tokens, path);
    current[0] = L'\0';

    wchar_t* context = NULL;
    for (wchar_t* token = std::wcstok(tokens, L"/", &context); token != NULL;
         token = std::wcstok(NULL, L"/", &context))
    {
        if (token == tokens)
        {
            std::wcscpy(current, token);
            // the archive name
            if (current[std::wcslen(current) - 1] == L':')
            {
                continue;
            }
        }
        else
        {
            std::wcscat(current, L"/");
            std::wcscat(current, token);
        }
        if (IsExistDir(storage, current))
        {
            continue;
        }
        nn::fs::detail::GetGlobalFileSystemBase()->TryDeleteDirectory(current);
        nn::Result fsResult = nn::fs::detail::GetGlobalFileSystemBase()->TryCreateDirectory(current);
        if (fsResult.IsFailure())
        {
            if (storage == STORAGE_TYPE_CTR_NAND)
            {
                ThrowFatalErrByNandOperation(fsResult);
            }
            result = ConvertFsResult(fsResult);
            break;
        }
    }
    allocator->Free(tokens);
    allocator->Free(current);
    return result;
}

// 0x005ACFDC | nintendogs:bytes-fuzzy [tier B]
int CreateImagePath(wchar_t* buffer, int size, StorageType storage, ImageKind kind, const IndexInfo& index,
                    bool isLegacy)
{
    int length = CreateImageRootDirectoryPath(buffer, size, storage, isLegacy);
    std::wcscpy(buffer + length, L"/");
    length++;
    length += std::swprintf(buffer + length, size - length, L"%03d%ls", index.m_DirectoryIndex + 100,
                            isLegacy ? s_LegacyDirectorySuffixW : s_DirectorySuffixW);
    std::wcscpy(buffer + length, L"/");
    length++;
    wchar_t* file = buffer + length;
    int fileLength = std::swprintf(file, size - length, L"%ls%04d", s_FileNamePrefixW, index.m_FileIndex + 1);
    std::wcscpy(file + fileLength, s_ImageExtensions[kind]);
    return s_ImageExtensionLengths[kind] + fileLength + length;
}

// 0x005AD0C4 | nintendogs:bytes-fuzzy [tier B]
int CreateImagePath(wchar_t* buffer, int size, StorageType storage, const ImageInfo& info, bool isLegacy)
{
    int length = CreateImageRootDirectoryPath(buffer, size, storage, isLegacy);
    std::wcscpy(buffer + length, L"/");
    length++;
    length += info.GetDirectoryName(buffer + length, size - length);
    std::wcscpy(buffer + length, L"/");
    length++;
    wchar_t* file = buffer + length;
    int rest = size - length;
    int fileLength = info.GetFileName(file, rest);
    file[fileLength] = L'.';
    fileLength++;
    fileLength += info.GetExt(file + fileLength, rest - fileLength);
    return fileLength + length;
}

// (the CTR NAND reports no read-only flag)
// 0x005AD164 | nintendogs:bytes-fuzzy [tier B]
bool IsDbFileReadOnly(StorageType storage)
{
    bool isFound = false;
    nn::fs::Attributes attributes;
    const wchar_t* path = GetPictureDbFilePath(storage);
    int size = std::wcslen(path) + 1;
    wchar_t* directoryPath = static_cast<wchar_t*>(GetAllocator()->Allocate(size * sizeof(wchar_t), 4));
    CreateDirectoryPathFromFilePath(directoryPath, size, path);
    const wchar_t* fileName = path + std::wcslen(directoryPath) + 1;

    Directory directory(*GetAllocator(), storage, directoryPath, 6000);
    if (directory.IsOpen())
    {
        while (directory.Read())
        {
            const nn::fs::DirectoryEntry* entry = directory.GetEntry();
            if (!directory.IsDirectory() && WStricmp(entry->name, fileName) == 0)
            {
                attributes = entry->attributes;
                if (storage != STORAGE_TYPE_SD)
                {
                    const nn::fs::Attributes plain = {attributes.isDirectory, false, false, false};
                    attributes = plain;
                }
                isFound = true;
                break;
            }
        }
    }
    GetAllocator()->Free(directoryPath);
    return isFound ? attributes.isReadOnly : false;
}

// the DCF directories "nnnNIN03" (or "nnnNIN02") of a path, sorted by their number
// 0x005AD2B4 | nintendogs:bytes-fuzzy [tier B]
void EnumImageDirectory(DynamicArray<int, XAllocator<int> >& directories, StorageType storage, const wchar_t* path,
                        bool isLegacy)
{
    directories.Clear();
    directories.SetMinCapacity(IndexInfo::DIRECTORY_INDEX_NUM);
    directories.Refresh();

    Directory directory(*GetAllocator(), storage, path, 1000);
    if (!directory.IsOpen())
    {
        return;
    }
    const wchar_t* suffix = isLegacy ? s_LegacyDirectorySuffixW : s_DirectorySuffixW;
    while (directory.Read())
    {
        if (!directory.IsDirectory())
        {
            continue;
        }
        const wchar_t* name = directory.GetEntry()->name;
        int no;
        if (std::wcslen(name) != 8 || std::wcsncmp(name + 3, suffix, 5) != 0 || !AToI(no, name, 3) ||
            !IndexInfo::IsValidDirectoryIndex(no - 100))
        {
            continue;
        }
        directories.PushBack(no);
    }
    std::sort(directories.begin(), directories.end());
}

// (on the CTR NAND a reserve of blocks stays free)
// 0x005AD684 | nintendogs:bytes [tier B]
s64 GetStorageFreeSize(StorageType storage, bool isLegacy)
{
    switch (storage)
    {
    case STORAGE_TYPE_CTR_NAND:
    {
        if (isLegacy)
        {
            return TwlNandArchiveMounter::GetInstance()->GetFreeSize();
        }
        s64 reservedBlocks = GetUnlimitedCtrNandFreeSpaceFlag() ? 296 : 3660;
        s64 totalBlocks;
        s64 freeBlocks;
        int blockSize;
        if (!CtrNandArchiveMounter::GetInstance()->GetSharedExtSaveDataBlockSize(totalBlocks, freeBlocks, blockSize))
        {
            return 0;
        }
        freeBlocks -= reservedBlocks;
        if (freeBlocks < 0)
        {
            freeBlocks = 0;
        }
        return freeBlocks * blockSize;
    }
    case STORAGE_TYPE_SD:
        return SdArchiveMounter::GetInstance()->GetFreeSize();
    default:
        return 0;
    }
}

// 0x005AD78C | nintendogs:callgraph [tier C]
int CreateImageFileName(wchar_t* buffer, int size, const IndexInfo& index, bool isLegacy)
{
    return std::swprintf(buffer, size, L"%ls%04d", s_FileNamePrefixW, index.m_FileIndex + 1);
}

// 0x005AD7B8 | nintendogs:bytes-fuzzy [tier B]
const wchar_t* GetPictureDbFileDir(StorageType storage)
{
    if (!s_IsPictureDbFileDirReady)
    {
        std::wcscpy(s_CtrNandPictureDbFileDir, GetCtrNandDriveName());
        std::wcscpy(s_CtrNandPictureDbFileDir + GetCtrNandDriveNameLength(), s_CtrNandPictureDbDirW);
        std::wcscpy(s_SdPictureDbFileDir, GetSdDriveName());
        std::wcscpy(s_SdPictureDbFileDir + GetSdDriveNameLength(), s_SdPictureDbDirW);
        s_IsPictureDbFileDirReady = true;
    }
    if (storage == STORAGE_TYPE_CTR_NAND)
    {
        return s_CtrNandPictureDbFileDir;
    }
    return storage == STORAGE_TYPE_SD ? s_SdPictureDbFileDir : L"";
}

// 0x005AD8D0 | nintendogs:bytes-fuzzy [tier B]
const wchar_t* GetPictureDbFilePath(StorageType storage)
{
    if (!s_IsPictureDbFilePathReady)
    {
        std::wcscpy(s_CtrNandPictureDbFilePath, GetPictureDbFileDir(STORAGE_TYPE_CTR_NAND));
        std::wcscat(s_CtrNandPictureDbFilePath, L"/");
        std::wcscat(s_CtrNandPictureDbFilePath, s_PhotoCacheFileName);
        std::wcscpy(s_SdPictureDbFilePath, GetPictureDbFileDir(STORAGE_TYPE_SD));
        std::wcscat(s_SdPictureDbFilePath, L"/");
        std::wcscat(s_SdPictureDbFilePath, s_PhotoCacheFileName);
        s_IsPictureDbFilePathReady = true;
    }
    if (storage == STORAGE_TYPE_CTR_NAND)
    {
        return s_CtrNandPictureDbFilePath;
    }
    return storage == STORAGE_TYPE_SD ? s_SdPictureDbFilePath : L"";
}

// 0x005AD980 | nintendogs:bytes-fuzzy [tier B]
int GetSdDriveNameLength()
{
    if (!s_IsSdDriveNameLengthReady)
    {
        s_SdDriveNameLength = std::wcslen(GetSdDriveName());
        s_IsSdDriveNameLengthReady = true;
    }
    return s_SdDriveNameLength;
}

// an index whose directory and file are free: the directory does not exist (and no other directory has its
// number) or it has no file of the number
// 0x005AD9EC | nintendogs:bytes [tier B]
bool IsAvailableIndexInfo(StorageType storage, const IndexInfo& index, bool isLegacy)
{
    wchar_t directoryPath[30];
    CreateImageDirectoryPath(directoryPath, 30, storage, index, isLegacy);
    if (!IsExistDir(storage, directoryPath))
    {
        wchar_t rootPath[21];
        CreateImageRootDirectoryPath(rootPath, 21, storage, isLegacy);
        Directory directory(*GetAllocator(), storage, rootPath, 1000);
        if (directory.IsOpen())
        {
            while (directory.Read())
            {
                int no;
                if (directory.IsDirectory() && ValidateDcfDirectoryName(directory.GetEntry()->name, &no) &&
                    index.m_DirectoryIndex + 100 == no)
                {
                    return false;
                }
            }
        }
    }
    CreateImageDirectoryPath(directoryPath, 30, storage, index, isLegacy);
    if (IsExistDir(storage, directoryPath) && !IsAvailableIndexInfoFile(storage, index, isLegacy))
    {
        return false;
    }
    return true;
}

// (with the current time for an image without a date)
// 0x005ADB50 | nintendogs:bytes [tier B]
bool CreateTwlJpegFromJpeg(void*& twlData, u32& twlSize, const void* data, u32 size)
{
    return CreateTwlJpegFromJpeg(twlData, twlSize, data, size, DateTimeSeconds::GetCurrentDateTime());
}

// a JPEG for the DSi camera from a 640x480 JPEG of the camera (smaller than 200 KB, with the maker notes of the
// source); the buffer comes from the allocator
// 0x005ADB9C | nintendogs:bytes [tier B]
bool CreateTwlJpegFromJpeg(void*& twlData, u32& twlSize, const void* data, u32 size, const DateTimeSeconds& dateTime)
{
    twlData = NULL;
    twlSize = 0;
    JpegDecoder decoder(data, size);
    if (!decoder.Decode(nn::jpeg::CTR::PIXEL_FORMAT_YUYV8))
    {
        return false;
    }
    if (decoder.GetRealWidth() != TWL_JPEG_WIDTH || decoder.GetRealHeight() != TWL_JPEG_HEIGHT)
    {
        return false;
    }
    JpegEncoder encoder(decoder.GetOutput(), nn::jpeg::CTR::PIXEL_FORMAT_YUYV8, decoder.GetWidth(), decoder.GetHeight());
    if (decoder.GetSysMakerNote() != NULL)
    {
        encoder.SetSysMakerNote(static_cast<const void*>(decoder.GetSysMakerNote()));
    }
    else
    {
        // (as the camera application saves it)
        encoder.m_SysMakerNote.m_SaveProcessType = 1;
        encoder.SetSysMakerNoteBodyId(static_cast<BodyIdType>(0), GetOwnBodyId());
        encoder.SetSysMakerNoteShootingType(static_cast<ShootingType>(0));
        encoder.SetSysMakerNoteTitleUniqueId(UserMakerNote::TITLE_UNIQUE_ID);
    }
    DateTimeSeconds createSecond = dateTime;
    if (createSecond.IsInvalid())
    {
        createSecond.SetCurrentDateTime();
    }
    encoder.SetSysMakerNoteCreateSecond(createSecond);
    if (decoder.GetUserMakerNote() != NULL && decoder.GetUserMakerNoteSize() != 0)
    {
        encoder.SetUserMakerNoteData(decoder.GetUserMakerNote(), decoder.GetUserMakerNoteSize());
    }
    encoder.m_pSoftware = reinterpret_cast<const char*>(decoder.GetSoftware());
    nn::fnd::DateTime nnDateTime;
    if (!decoder.GetDateTime(nnDateTime))
    {
        nnDateTime = dateTime.ConvertToNNDateTime();
    }
    encoder.m_DateTime = nnDateTime;
    if (!encoder.Encode(true))
    {
        return false;
    }
    twlSize = encoder.m_EncodedSize;
    twlData = GetAllocator()->Allocate(twlSize, 4);
    std::memcpy(twlData, encoder.m_pOutput, twlSize);
    return true;
}

// the first index from start on whose directory has a free file number (the directory need not exist); false
// after one round over all directories
// 0x005ADDF0 | nintendogs:callgraph [tier C]
bool FindAvailableIndexInfo(IndexInfo& index, StorageType storage, bool isLegacy, const IndexInfo& start)
{
    wchar_t rootPath[21];
    CreateImageRootDirectoryPath(rootPath, 21, storage, isLegacy);
    if (!IsExistDir(storage, rootPath))
    {
        index = start;
        return true;
    }
    u8 directoryFlags[DIRECTORY_FLAGS_SIZE * 2] = {};
    GetImageDirectoryFlags(directoryFlags, storage, isLegacy);

    IndexInfo current = start;
    bool isFirst = true;
    while (true)
    {
        if (IsUsableDirectoryIndex(directoryFlags, current.m_DirectoryIndex))
        {
            IndexInfo directoryIndex;
            directoryIndex.Initialize();
            directoryIndex.SetDirectoryIndex(current.m_DirectoryIndex);
            u8 fileFlags[FILE_FLAGS_SIZE] = {};
            GetImageFileFlags(fileFlags, storage, directoryIndex, isLegacy);
            for (int file = current.m_FileIndex; file < IndexInfo::OFFICIAL_FILE_NUM; file++)
            {
                if (!TestBit(fileFlags, file))
                {
                    index.SetDirectoryIndex(current.m_DirectoryIndex);
                    index.SetFileIndex(file);
                    return true;
                }
            }
        }
        if (isFirst)
        {
            isFirst = false;
        }
        else if (start.m_DirectoryIndex == current.m_DirectoryIndex)
        {
            return false;
        }
        current.UpdateNextDirectoryIndex();
    }
}

// "$phtsd:" (the archive imgdb mounts) becomes "$_sdmc:"; other paths are copied; the length, 0 when the buffer
// is too small
// 0x005AE040 | nintendogs:callgraph [tier C]
int ConvertRealToVirtualPath(wchar_t* buffer, int size, const wchar_t* path)
{
    if (buffer == NULL)
    {
        return 0;
    }
    if (path == NULL)
    {
        buffer[0] = L'\0';
        return 0;
    }
    if (std::wcsncmp(s_DriveNamesW[1], path, DRIVE_NAME_LENGTH) == 0)
    {
        if (std::wcslen(path) < static_cast<u32>(size))
        {
            std::wcscpy(buffer, path);
            return std::wcslen(buffer);
        }
    }
    else if (std::wcsncmp(path, s_DriveNamesW[0], DRIVE_NAME_LENGTH) == 0)
    {
        if (size > DRIVE_NAME_LENGTH)
        {
            std::wcscpy(buffer, s_DriveNamesW[1]);
            const wchar_t* rest = path + DRIVE_NAME_LENGTH;
            if (std::wcslen(rest) + DRIVE_NAME_LENGTH < static_cast<u32>(size))
            {
                std::wcscpy(buffer + DRIVE_NAME_LENGTH, rest);
                return std::wcslen(rest) + DRIVE_NAME_LENGTH;
            }
        }
    }
    else if (static_cast<u32>(size) > std::wcslen(path))
    {
        std::wcscpy(buffer, path);
        return std::wcslen(buffer);
    }
    buffer[0] = L'\0';
    return 0;
}

// 0x005AE140 | nintendogs:bytes-fuzzy [tier B]
int CreateImageDirectoryName(wchar_t* buffer, int size, const IndexInfo& index, bool isLegacy)
{
    return std::swprintf(buffer, size, L"%03d%ls", index.m_DirectoryIndex + 100,
                         isLegacy ? s_LegacyDirectorySuffixW : s_DirectorySuffixW);
}

// 0x005AE178 | nintendogs:bytes-fuzzy [tier B]
int CreateImageDirectoryPath(wchar_t* buffer, int size, StorageType storage, const ImageInfo& info, bool isLegacy)
{
    int length = CreateImageRootDirectoryPath(buffer, size, storage, isLegacy);
    std::wcscpy(buffer + length, L"/");
    length++;
    return info.GetDirectoryName(buffer + length, size - length) + length;
}

// 0x005AE1C0 | nintendogs:bytes-fuzzy [tier B]
int CreateImageDirectoryPath(wchar_t* buffer, int size, StorageType storage, const IndexInfo& index, bool isLegacy)
{
    int length = CreateImageRootDirectoryPath(buffer, size, storage, isLegacy);
    std::wcscpy(buffer + length, L"/");
    length++;
    return std::swprintf(buffer + length, size - length, L"%03d%ls", index.m_DirectoryIndex + 100,
                         isLegacy ? s_LegacyDirectorySuffixW : s_DirectorySuffixW) +
           length;
}

// a directory with everything in it
// 0x005AE234 (name is ours)
Result DeleteDirectory(StorageType storage, const wchar_t* path)
{
    Result result = RESULT_SUCCESS;
    {
        Directory directory(*GetAllocator(), storage, path, 1000);
        if (directory.IsOpen())
        {
            while (directory.Read())
            {
                const wchar_t* name = directory.GetEntry()->name;
                int length = std::wcslen(path) + std::wcslen(L"/") + std::wcslen(name);
                wchar_t* childPath =
                    static_cast<wchar_t*>(GetAllocator()->Allocate((length + 1) * sizeof(wchar_t), 4));
                std::wcscpy(childPath, path);
                std::wcscat(childPath, L"/");
                std::wcscat(childPath, name);
                if (directory.IsFile())
                {
                    result = DeleteFile(storage, childPath);
                }
                else if (directory.IsDirectory())
                {
                    result = DeleteDirectory(storage, childPath);
                }
                GetAllocator()->Free(childPath);
                if (result != RESULT_SUCCESS)
                {
                    return result;
                }
            }
        }
    }
    nn::Result fsResult = nn::fs::detail::GetGlobalFileSystemBase()->TryDeleteDirectory(path);
    if (fsResult.IsFailure())
    {
        if (storage == STORAGE_TYPE_CTR_NAND)
        {
            ThrowFatalErrByNandOperation(fsResult);
        }
        if (IsFsDescription(fsResult, 260, 279))
        {
            return RESULT_FS_ACCESS_ERROR;
        }
        if (IsFsDescription(fsResult, 130, 179))
        {
            return RESULT_FS_NOT_FOUND;
        }
        if (IsFsDescription(fsResult, 280, 334))
        {
            return RESULT_FS_ARCHIVE_ERROR;
        }
        return RESULT_FS_ERROR;
    }
    return result;
}

// 0x005AE450 | nintendogs:bytes [tier B]
int EstimateRemainPictureNum(StorageType storage, u32 pictureSize, bool isLegacy)
{
    return EstimateRemainPictureNumWithFreeSize(storage, GetStorageFreeSize(storage, isLegacy), pictureSize,
                                                isLegacy);
}

// the number nnnn of "XXXX_nnnn", -1 when it is not one
// 0x005AE488 | nintendogs:bytes [tier B]
int GetFileNoFromDcfFileName(const char* name)
{
    for (int i = 0; i < 4; i++)
    {
        if (*name++ == '\0')
        {
            return -1;
        }
    }
    return ParseDcfNumber(name, 4);
}

// 0x005AE53C | nintendogs:bytes [tier B]
int GetFileNoFromDcfFileName(const wchar_t* name)
{
    for (int i = 0; i < 4; i++)
    {
        if (*name++ == L'\0')
        {
            return -1;
        }
    }
    return ParseDcfNumber(name, 4);
}

// no file of the index exists, with any extension
// 0x005AE5F0 | nintendogs:callgraph [tier C]
bool IsAvailableIndexInfoFile(StorageType storage, const IndexInfo& index, bool isLegacy)
{
    wchar_t path[44];
    int length = CreateImageDirectoryPath(path, 44, storage, index, isLegacy);
    std::wcscpy(path + length, L"/");
    length = std::wcslen(path);
    CreateImageFileName(path + length, 44 - length, index, isLegacy);
    wchar_t* ext = path + std::wcslen(path);
    for (int i = 0; i < IMAGE_KIND_NUM; i++)
    {
        ImageKind kind = static_cast<ImageKind>(i);
        if (kind == IMAGE_KIND_NONE)
        {
            continue;
        }
        std::wcscpy(ext, s_ImageExtensions[kind]);
        nn::fs::FileInputStream file;
        nn::Result result = file.TryInitialize(path);
        if (result.IsSuccess())
        {
            file.Finalize();
        }
        else if (storage == STORAGE_TYPE_CTR_NAND)
        {
            ThrowFatalErrByNandOperation(result);
        }
        if (result.IsSuccess())
        {
            return false;
        }
    }
    return true;
}

// the image root exists (on the CTR NAND also the one of the DSi camera)
// 0x005AE778 | nintendogs:bytes [tier B]
bool IsExistImageDataEvidence(StorageType storage)
{
    wchar_t rootPath[21];
    if (storage == STORAGE_TYPE_CTR_NAND)
    {
        CreateImageRootDirectoryPath(rootPath, 21, storage, false);
        bool isExist = IsExistDir(storage, rootPath);
        bool isLegacyExist = false;
        if (IsTwlNandMounted())
        {
            CreateImageRootDirectoryPath(rootPath, 21, storage, true);
            isLegacyExist = IsExistDir(storage, rootPath);
        }
        return isExist | isLegacyExist;
    }
    if (storage == STORAGE_TYPE_SD)
    {
        CreateImageRootDirectoryPath(rootPath, 21, storage, false);
        return IsExistDir(storage, rootPath);
    }
    return false;
}

// "nnnXXXXX"
// 0x005AE834 | nintendogs:callgraph [tier C]
bool ValidateDcfDirectoryName(const wchar_t* name, int* no)
{
    if (std::wcslen(name) != 8)
    {
        return false;
    }
    return ValidateDcfDirectoryNameWithoutLength(name, no);
}

// 0x005AE864 | nintendogs:bytes [tier B]
int GetCtrNandDriveNameLength()
{
    if (!s_IsCtrNandDriveNameLengthReady)
    {
        s_CtrNandDriveNameLength = std::wcslen(GetCtrNandDriveName());
        s_IsCtrNandDriveNameLengthReady = true;
    }
    return s_CtrNandDriveNameLength;
}

// the photo directories of the DSi camera (on the CTR NAND in its archive, when it is mounted)
// 0x005AE8D0 | nintendogs:bytes-fuzzy [tier B]
const wchar_t* GetLegacyPictureDbFileDir(StorageType storage)
{
    if (!s_IsLegacyPictureDbFileDirReady)
    {
        if (IsTwlNandMounted())
        {
            std::wcscpy(s_TwlNandLegacyPictureDbFileDir, GetTwlNandDriveName());
            std::wcscpy(s_TwlNandLegacyPictureDbFileDir + GetTwlNandDriveNameLength(), s_LegacyPhotoDirectory);
        }
        else
        {
            s_TwlNandLegacyPictureDbFileDir[0] = L'\0';
        }
        std::wcscpy(s_SdLegacyPictureDbFileDir, GetSdDriveName());
        std::wcscpy(s_SdLegacyPictureDbFileDir + GetSdDriveNameLength(), s_LegacyPhotoDirectory);
        s_IsLegacyPictureDbFileDirReady = true;
    }
    if (storage == STORAGE_TYPE_CTR_NAND)
    {
        return s_TwlNandLegacyPictureDbFileDir;
    }
    return storage == STORAGE_TYPE_SD ? s_SdLegacyPictureDbFileDir : L"";
}

// 0x005AEA04 | nintendogs:bytes [tier B]
int GetTwlNandDriveNameLength()
{
    if (!s_IsTwlNandDriveNameLengthReady)
    {
        s_TwlNandDriveNameLength = std::wcslen(GetTwlNandDriveName());
        s_IsTwlNandDriveNameLengthReady = true;
    }
    return s_TwlNandDriveNameLength;
}

// the dates, the database values of the maker note of the system (when they are valid) and the faces of the maker
// note of the camera application
// 0x005AEA70 | nintendogs:bytes [tier B]
void ExtractPictureInfoFromExif(PictureTableRecord& record, const JpegMpBaseDecoder& decoder)
{
    ImageInfo& info = record.m_ImageInfo;
    nn::fnd::DateTime dateTime;
    if (decoder.GetDateTime(dateTime))
    {
        info.m_DateTime[0].ConvertFromNNDateTime(dateTime);
    }
    else
    {
        info.m_DateTime[0] = DateTimeSeconds::GetMinSeconds();
    }
    info.m_DateTime[1] = DateTimeSeconds::GetCurrentDateTime();
    if (decoder.GetSysMakerNote() == NULL)
    {
        return;
    }
    SysMakerNote note;
    std::memcpy(&note, decoder.GetSysMakerNote(), sizeof(note));
    ImageInfo noteInfo;
    noteInfo.Initialize();
    DateTimeSeconds createSecond;
    createSecond.Initialize();
    createSecond.SetSeconds(note.m_CreateSecond.m_Seconds);
    noteInfo.m_DateTime[1] = createSecond;
    noteInfo.m_TitleUniqueId = note.m_TitleUniqueId;
    noteInfo.SetHandleTypeBit(static_cast<HandleTypeBit>(note.m_HandleTypeBit));
    noteInfo.SetShootingType(note.GetShootingType());
    noteInfo.SetDistinctionTypeBit(note.GetDistinctionTypeBit());
    noteInfo.SetSaveProcessType(static_cast<SaveProcessType>(note.m_SaveProcessType));
    for (int i = 0; i < BODY_ID_TYPE_NUM; i++)
    {
        BodyIdType type = static_cast<BodyIdType>(i);
        if (note.m_BodyIdValidBits & (1 << type))
        {
            noteInfo.SetBodyId(type, note.m_BodyIds[type]);
        }
    }
    noteInfo.SetValidityFlag(true);
    if (noteInfo.ValidateWithoutPath())
    {
        info.m_DateTime[1] = noteInfo.m_DateTime[1];
        info.m_TitleUniqueId = noteInfo.m_TitleUniqueId;
        info.SetHandleTypeBit(noteInfo.GetHandleTypeBit());
        info.SetShootingType(noteInfo.GetShootingType());
        info.SetDistinctionTypeBit(noteInfo.GetDistinctionTypeBit());
        info.SetSaveProcessType(noteInfo.GetSaveProcessType());
        for (int i = 0; i < BODY_ID_TYPE_NUM; i++)
        {
            BodyIdType type = static_cast<BodyIdType>(i);
            if (noteInfo.IsValidBodyId(type))
            {
                info.SetBodyId(type, noteInfo.GetBodyId(type));
            }
        }
    }
    if (note.m_TitleUniqueId != UserMakerNote::TITLE_UNIQUE_ID)
    {
        return;
    }
    if (decoder.GetUserMakerNote() == NULL || decoder.GetUserMakerNoteSize() < sizeof(UserMakerNote::Header))
    {
        return;
    }
    UserMakerNote::Header header;
    std::memcpy(&header, decoder.GetUserMakerNote(), sizeof(header));
    if (header.m_Version != UserMakerNote::VERSION || decoder.GetUserMakerNoteSize() != sizeof(UserMakerNote))
    {
        return;
    }
    UserMakerNote userNote;
    std::memcpy(&userNote, decoder.GetUserMakerNote(), sizeof(userNote));
    for (int i = 0; i < UserMakerNote::FACE_NUM; i++)
    {
        if (userNote.GetFaceInfo(i)->Validate())
        {
            record.m_Faces[i] = *userNote.GetFaceInfo(i);
        }
    }
}

// the Exif data of the header of the file (only JPEG and MPO files)
// 0x005AEDA0 | nintendogs:bytes [tier B]
bool ExtractPictureInfoFromFile(PictureTableRecord& record, StorageType storage, const ImageInfo& info, bool isLegacy)
{
    if (info.GetImageKind() == IMAGE_KIND_JPEG)
    {
        PhotoHeaderImageInfoReader reader(*GetAllocator(), storage, info, isLegacy);
        if (reader.ReadImage() == RESULT_SUCCESS)
        {
            JpegDecoder decoder(reader.GetData(), reader.GetDataSize());
            if (decoder.ExtractExif())
            {
                ExtractPictureInfoFromExif(record, decoder);
                return true;
            }
        }
        return false;
    }
    if (info.GetImageKind() == IMAGE_KIND_MP)
    {
        PhotoHeaderImageInfoReader reader(*GetAllocator(), storage, info, isLegacy);
        if (reader.ReadImage() == RESULT_SUCCESS)
        {
            MpDecoder decoder(reader.GetData(), reader.GetDataSize());
            if (decoder.ExtractExif())
            {
                ExtractPictureInfoFromExif(record, decoder);
                return true;
            }
        }
        return false;
    }
    return false;
}

// 0x005AEF54 | nintendogs:bytes-fuzzy [tier B]
const wchar_t* GetLegacyPictureDbFilePath(StorageType storage)
{
    if (!s_IsLegacyPictureDbFilePathReady)
    {
        std::wcscpy(s_TwlNandLegacyPictureDbFilePath, GetLegacyPictureDbFileDir(STORAGE_TYPE_CTR_NAND));
        std::wcscat(s_TwlNandLegacyPictureDbFilePath, L"/");
        std::wcscat(s_TwlNandLegacyPictureDbFilePath, s_PictureDbFileName);
        std::wcscpy(s_SdLegacyPictureDbFilePath, GetLegacyPictureDbFileDir(STORAGE_TYPE_SD));
        std::wcscat(s_SdLegacyPictureDbFilePath, L"/");
        std::wcscat(s_SdLegacyPictureDbFilePath, s_PictureDbFileName);
        s_IsLegacyPictureDbFilePathReady = true;
    }
    if (storage == STORAGE_TYPE_CTR_NAND)
    {
        return s_TwlNandLegacyPictureDbFilePath;
    }
    return storage == STORAGE_TYPE_SD ? s_SdLegacyPictureDbFilePath : L"";
}

// "<archive>:/" and the image root of the storage (the DSi archive for the legacy images on the CTR NAND)
// 0x005AF004 | nintendogs:bytes-fuzzy [tier B]
int CreateImageRootDirectoryPath(wchar_t* buffer, int size, StorageType storage, bool isLegacy)
{
    int length = 0;
    if (storage == STORAGE_TYPE_CTR_NAND)
    {
        if (isLegacy)
        {
            std::wcscpy(buffer, GetTwlNandDriveName());
            buffer += GetTwlNandDriveNameLength();
            length = GetTwlNandDriveNameLength();
        }
        else
        {
            std::wcscpy(buffer, GetCtrNandDriveName());
            buffer += GetCtrNandDriveNameLength();
            length = GetCtrNandDriveNameLength();
        }
    }
    else if (storage == STORAGE_TYPE_SD)
    {
        std::wcscpy(buffer, GetSdDriveName());
        buffer += GetSdDriveNameLength();
        length = GetSdDriveNameLength();
    }
    else
    {
        buffer[0] = L'\0';
    }
    // (the tables have two entries: other storages read past them)
    std::wcscpy(buffer, s_ImageRootDirectoriesW[storage]);
    return s_ImageRootDirectoryLengths[storage] + length;
}

// FindAvailableIndexInfo for the CTR NAND: the index must be free in the directories of the camera and in the
// ones of the DSi camera
// 0x005AF190 | nintendogs:callgraph [tier C]
bool FindAvailableIndexInfoInNand(IndexInfo& index, const IndexInfo& start)
{
    wchar_t rootPath[21];
    CreateImageRootDirectoryPath(rootPath, 21, STORAGE_TYPE_CTR_NAND, false);
    bool isExist = IsExistDir(STORAGE_TYPE_CTR_NAND, rootPath);
    bool isLegacyExist = false;
    if (IsTwlNandMounted())
    {
        CreateImageRootDirectoryPath(rootPath, 21, STORAGE_TYPE_CTR_NAND, true);
        isLegacyExist = IsExistDir(STORAGE_TYPE_CTR_NAND, rootPath);
    }
    if (!(isExist || isLegacyExist))
    {
        index = start;
        return true;
    }
    u8 directoryFlags[DIRECTORY_FLAGS_SIZE * 2] = {};
    u8 legacyDirectoryFlags[DIRECTORY_FLAGS_SIZE * 2] = {};
    GetImageDirectoryFlags(directoryFlags, STORAGE_TYPE_CTR_NAND, false);
    if (IsTwlNandMounted())
    {
        GetImageDirectoryFlags(legacyDirectoryFlags, STORAGE_TYPE_CTR_NAND, true);
    }

    IndexInfo current = start;
    bool isFirst = true;
    while (true)
    {
        if (IsUsableDirectoryIndex(directoryFlags, current.m_DirectoryIndex) &&
            IsUsableDirectoryIndex(legacyDirectoryFlags, current.m_DirectoryIndex))
        {
            IndexInfo directoryIndex;
            directoryIndex.Initialize();
            directoryIndex.SetDirectoryIndex(current.m_DirectoryIndex);
            u8 fileFlags[FILE_FLAGS_SIZE] = {};
            u8 legacyFileFlags[FILE_FLAGS_SIZE] = {};
            GetImageFileFlags(fileFlags, STORAGE_TYPE_CTR_NAND, directoryIndex, false);
            if (IsTwlNandMounted())
            {
                GetImageFileFlags(legacyFileFlags, STORAGE_TYPE_CTR_NAND, directoryIndex, true);
            }
            for (int file = current.m_FileIndex; file < IndexInfo::OFFICIAL_FILE_NUM; file++)
            {
                if (!TestBit(fileFlags, file) && !TestBit(legacyFileFlags, file))
                {
                    index.SetDirectoryIndex(current.m_DirectoryIndex);
                    index.SetFileIndex(file);
                    return true;
                }
            }
        }
        if (isFirst)
        {
            isFirst = false;
        }
        else if (start.m_DirectoryIndex == current.m_DirectoryIndex)
        {
            return false;
        }
        current.UpdateNextDirectoryIndex();
    }
}

// a failed operation on the CTR NAND is fatal, unless the entry is missing or exists or the NAND is full
// 0x005AF5C0 | nintendogs:bytes-fuzzy [tier B]
nn::Result ThrowFatalErrByNandOperation(nn::Result result)
{
    if (result.IsFailure() && !IsFsDescription(result, 100, 179) && !IsFsDescription(result, 180, 199) &&
        !IsFsDescription(result, 200, 219))
    {
        nn::err::CTR::ThrowFatalErrAllIfFailure(result);
    }
    return result;
}

// the part of a path before the last '/' (the size is not checked)
// 0x005AF640 (name after nintendogs, at 0x5AF6B0 in symbols.json)
void CreateDirectoryPathFromFilePath(wchar_t* buffer, int size, const wchar_t* path)
{
    int i = std::wcslen(path) - 1;
    while (i >= 0 && path[i] != L'/')
    {
        i--;
    }
    if (i >= 0)
    {
        std::wmemcpy(buffer, path, i);
        buffer[i] = L'\0';
    }
}

// the DCF directories of the image root (see DIRECTORY_FLAGS_SIZE); the DSi camera uses "NIN02" on the CTR NAND
// 0x005AF6B0 (name is ours)
void GetImageDirectoryFlags(u8* flags, StorageType storage, bool isLegacy)
{
    wchar_t rootPath[21];
    CreateImageRootDirectoryPath(rootPath, 21, storage, isLegacy);
    Directory directory(*GetAllocator(), storage, rootPath, 1000);
    if (!directory.IsOpen())
    {
        return;
    }
    const wchar_t* suffix =
        storage == STORAGE_TYPE_CTR_NAND && isLegacy ? s_LegacyDirectorySuffixW : s_DirectorySuffixW;
    while (directory.Read())
    {
        if (!directory.IsDirectory())
        {
            continue;
        }
        const wchar_t* name = directory.GetEntry()->name;
        int no;
        if (!ValidateDcfDirectoryName(name, &no))
        {
            continue;
        }
        bool isSuffix = std::wcscmp(name + 3, suffix) == 0;
        int index = no - 100;
        if (TestBit(flags, index))
        {
            if (isSuffix)
            {
                SetBit(flags + DIRECTORY_FLAGS_SIZE, index);
            }
        }
        else
        {
            SetBit(flags, index);
            if (isSuffix)
            {
                SetBit(flags + DIRECTORY_FLAGS_SIZE, index);
            }
            else
            {
                ClearBit(flags + DIRECTORY_FLAGS_SIZE, index);
            }
        }
    }
}

// "XXXXnnnn" with a file number 1-9999
// 0x005AF82C | nintendogs:bytes [tier B]
bool ValidateDcfFileNameWithoutLength(const char* name, int* no)
{
    for (int i = 0; i < 4; i++)
    {
        if (!IsDcfCharacter(name[i]))
        {
            return false;
        }
    }
    int fileNo = GetFileNoFromDcfFileName(name);
    if (static_cast<u32>(fileNo - 1) > static_cast<u32>(IndexInfo::FILE_INDEX_MAX))
    {
        return false;
    }
    if (no != NULL)
    {
        *no = fileNo;
    }
    return true;
}

// "XXXXnnnn" with an extension or without
// 0x005AF908 (name is ours)
bool ValidateDcfFileName(const wchar_t* name, int* no)
{
    int length = std::wcslen(name);
    if (length < 8 || (length >= 9 && name[8] != L'.'))
    {
        return false;
    }
    return ValidateDcfFileNameWithoutLength(name, no);
}

// 0x005AF94C | nintendogs:bytes [tier B]
bool ValidateDcfFileNameWithoutLength(const wchar_t* name, int* no)
{
    for (int i = 0; i < 4; i++)
    {
        if (!IsDcfCharacter(name[i]))
        {
            return false;
        }
    }
    int fileNo = GetFileNoFromDcfFileName(name);
    if (static_cast<u32>(fileNo - 1) > static_cast<u32>(IndexInfo::FILE_INDEX_MAX))
    {
        return false;
    }
    if (no != NULL)
    {
        *no = fileNo;
    }
    return true;
}

// 0x005AFA28 | nintendogs:bytes [tier B]
void StoreImageDbRecordParam2ImageInfo(ImageInfo& info, const ImageDbRecordParam& param)
{
    info.m_DateTime[0] = param.m_DateTime;
    info.m_TitleUniqueId = param.m_TitleUniqueId;
    info.SetHandleTypeBit(static_cast<HandleTypeBit>(param.m_HandleTypeBit));
    info.SetShootingType(static_cast<ShootingType>(param.m_ShootingType));
    info.SetDistinctionTypeBit(static_cast<DistinctionTypeBit>(param.m_DistinctionTypeBit));
    info.SetSaveProcessType(static_cast<SaveProcessType>(param.m_SaveProcessType));
    for (int i = 0; i < BODY_ID_TYPE_NUM; i++)
    {
        BodyIdType type = static_cast<BodyIdType>(i);
        if (param.IsValidBodyId(type))
        {
            info.SetBodyId(type, param.GetBodyId(type));
        }
        else
        {
            info.ResetBodyId(type);
        }
    }
}

// the number nnn of "nnnXXXXX", -1 when it is not one
// 0x005AFAD8 | nintendogs:bytes [tier B]
int GetDirectoryNoFromDcfDirectoryName(const char* name)
{
    return ParseDcfNumber(name, 3);
}

// how many images still fit: all empty records of the database when the space suffices, otherwise what fits
// after a reserve (sizes are ours to explain: the SD card keeps twice 600 KB, the CTR NAND 400 KB, the DSi
// archive 200 KB)
// 0x005AFB34 | nintendogs:bytes [tier B]
int EstimateRemainPictureNumWithFreeSize(StorageType storage, s64 freeSize, u32 pictureSize, bool isLegacy)
{
    int emptyNum = ImageDatabase::GetInstance()->GetEmptyImageNum(storage);
    u32 reserve;
    u32 subReserve;
    if (storage == STORAGE_TYPE_SD)
    {
        reserve = 614400;
        subReserve = 614400;
    }
    else
    {
        subReserve = 0;
        reserve = 0;
        if (storage == STORAGE_TYPE_CTR_NAND)
        {
            reserve = isLegacy ? 204800 : 409600;
        }
    }
    if (static_cast<s64>(emptyNum - 1) * pictureSize + reserve + subReserve < freeSize)
    {
        return emptyNum;
    }
    if (freeSize < reserve + subReserve)
    {
        return 0;
    }
    return (freeSize - reserve - subReserve) / pictureSize + 1;
}

// "nnnXXXXX" with a directory number 100-999
// 0x005AFC24 | nintendogs:bytes [tier B]
bool ValidateDcfDirectoryNameWithoutLength(const char* name, int* no)
{
    int directoryNo = ParseDcfNumber(name, 3);
    if (static_cast<u32>(directoryNo - 100) >= static_cast<u32>(IndexInfo::DIRECTORY_INDEX_NUM))
    {
        return false;
    }
    if (no != NULL)
    {
        *no = directoryNo;
    }
    for (int i = 3; i < 8; i++)
    {
        if (!IsDcfCharacter(name[i]))
        {
            return false;
        }
    }
    return true;
}

// 0x005AFD70 | nintendogs:bytes [tier B]
bool ValidateDcfDirectoryNameWithoutLength(const wchar_t* name, int* no)
{
    int directoryNo = ParseDcfNumber(name, 3);
    if (static_cast<u32>(directoryNo - 100) >= static_cast<u32>(IndexInfo::DIRECTORY_INDEX_NUM))
    {
        return false;
    }
    if (no != NULL)
    {
        *no = directoryNo;
    }
    for (int i = 3; i < 8; i++)
    {
        if (!IsDcfCharacter(name[i]))
        {
            return false;
        }
    }
    return true;
}

// 0x005AFEBC | nintendogs:bytes [tier B]
Result DeleteImageCtr(StorageType storage, const ImageInfo& info, bool isDeleteEmptyDirectory)
{
    return DeleteImageFile(storage, info, false, isDeleteEmptyDirectory);
}

// 0x005AFEC8 (name is ours)
Result DeleteImageFile(StorageType storage, const ImageInfo& info, bool isLegacy, bool isDeleteEmptyDirectory)
{
    Result result = RESULT_NOT_READY;
    wchar_t path[44];
    if (info.GetImageKind() == IMAGE_KIND_JPEG || info.GetImageKind() == IMAGE_KIND_AVI)
    {
        CreateImagePath(path, 44, storage, info, isLegacy);
        result = DeleteFile(storage, path);
    }
    else if (info.GetImageKind() == IMAGE_KIND_MP)
    {
        CreateImagePath(path, 44, storage, info, isLegacy);
        result = DeleteFile(storage, path);
        if (storage == STORAGE_TYPE_SD)
        {
            ImageInfo jpegInfo;
            GenerateImageInfoFromImageInfo(jpegInfo, info, IMAGE_KIND_JPEG, isLegacy);
            CreateImagePath(path, 44, storage, jpegInfo, isLegacy);
            DeleteFile(storage, path);
        }
    }
    else
    {
        return result;
    }

    if (result == RESULT_SUCCESS && isDeleteEmptyDirectory)
    {
        // the directory of the image and the root, when they are empty
        wchar_t directoryPath[30];
        CreateImageDirectoryPath(directoryPath, 30, storage, info, isLegacy);
        {
            Directory directory(*GetAllocator(), storage, directoryPath, 1000);
            if (directory.IsOpen() && directory.Read())
            {
                return result;
            }
        }
        DeleteDirectory(storage, directoryPath);
        CreateImageRootDirectoryPath(directoryPath, 21, storage, isLegacy);
        {
            Directory directory(*GetAllocator(), storage, directoryPath, 1000);
            if (directory.IsOpen() && directory.Read())
            {
                return result;
            }
        }
        DeleteDirectory(storage, directoryPath);
    }
    return result;
}

// 0x005B01C4 | nintendogs:bytes [tier B]
bool AToI(int& value, const char* string, int length)
{
    if (length <= 0)
    {
        return false;
    }
    value = 0;
    while (--length >= 0 && *string != 0)
    {
        int c = static_cast<s8>(*string);
        if (static_cast<u32>(c - '0') >= 10)
        {
            return false;
        }
        value = value * 10 + (c - '0');
        string++;
    }
    return true;
}

// 0x005B0230 | nintendogs:bytes [tier B]
bool AToI(int& value, const wchar_t* string, int length)
{
    if (length <= 0)
    {
        return false;
    }
    value = 0;
    while (--length >= 0 && *string != 0)
    {
        int c = static_cast<u16>(*string);
        if (static_cast<u32>(c - '0') >= 10)
        {
            return false;
        }
        value = value * 10 + (c - '0');
        string++;
    }
    return true;
}

// 0x005B029C | nintendogs:bytes [tier B]
int WStricmp(const wchar_t* a, const wchar_t* b)
{
    for (; *a != 0 && *b != 0; a++, b++)
    {
        u16 ca = *a >= 'A' && *a <= 'Z' ? *a + 0x20 : *a;
        u16 cb = *b >= 'A' && *b <= 'Z' ? *b + 0x20 : *b;
        if (ca != cb)
        {
            break;
        }
    }
    u16 ca = *a >= 'A' && *a <= 'Z' ? *a + 0x20 : *a;
    u16 cb = *b >= 'A' && *b <= 'Z' ? *b + 0x20 : *b;
    return ca - cb;
}

// (RESULT_NOT_READY without data or for another kind)
// 0x005B0310 | nintendogs:bytes [tier B]
Result SaveImage(StorageType storage, const ImageInfo& info, const void* data, u32 size, const void* subData,
                 u32 subSize)
{
    Result result = RESULT_NOT_READY;
    if (data != NULL && size != 0)
    {
        if (info.GetImageKind() == IMAGE_KIND_JPEG)
        {
            result = SaveImageJpeg(storage, info, data, size);
        }
        else if (info.GetImageKind() == IMAGE_KIND_MP)
        {
            result = SaveImageMp(storage, info, data, size, subData, subSize);
        }
    }
    return result;
}

} // namespace util
} // namespace imgdb
