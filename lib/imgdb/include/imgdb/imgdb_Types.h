#pragma once

// The basic types of imgdb, the image database of the photos and screenshots the game saves (DCF folders "nnnNIN03"
// with files "HNI_nnnn.JPG/.MPO" on the SD card, a database file per storage). The class and function names come
// from the reference symbols of nintendogs; enum values, members and helper names are ours.

#include <new>

#include "decomp.h"
#include "nn/fnd/fnd_DateTime.h"
#include "nn/fnd/fnd_IAllocator.h"

namespace imgdb {

// results of the database functions (the file errors by the description ranges of nn::fs)
enum Result : s8
{
    RESULT_SUCCESS = 0,
    RESULT_NOT_FOUND = -1,
    RESULT_NOT_READY = 1,          // FileReader / FileWriter before the first call, an encoder at work
    RESULT_INVALID_PARAM = 2,      // the parameters of a saver, a failed encode
    RESULT_DATABASE_FULL = 3,      // no empty record
    RESULT_NO_FILE_INDEX = 4,      // empty records but no free file number
    RESULT_FS_ERROR = 5,           // any other file system error
    RESULT_INVALID_DATA = 6,       // an image that cannot be decoded
    RESULT_FILE_TOO_LARGE = 7,
    RESULT_FS_NOT_FOUND = 8,       // descriptions 130-179
    RESULT_FS_ACCESS_ERROR = 9,    // descriptions 260-279
    RESULT_FS_NOT_ENOUGH_SPACE = 10, // descriptions 200-219
    RESULT_DATABASE_BROKEN = 11,
    RESULT_DATABASE_READ_ONLY = 13,  // the file of the picture table is read-only
    RESULT_FS_ARCHIVE_ERROR = 14,  // descriptions 280-334
};

// where the images are
enum StorageType : u8
{
    STORAGE_TYPE_CTR_NAND = 0,
    STORAGE_TYPE_SD = 1,
    STORAGE_TYPE_TWL_NAND = 2,
};

enum ImageKind : u8
{
    IMAGE_KIND_NONE = 0,
    IMAGE_KIND_JPEG = 1,
    IMAGE_KIND_MP = 2,  // MPO (stereo)
    IMAGE_KIND_AVI = 3,
    IMAGE_KIND_NUM = 4,
};

enum ImageKindBit : u8
{
    IMAGE_KIND_BIT_JPEG = 1 << 1,  // 1 << IMAGE_KIND_JPEG
    IMAGE_KIND_BIT_MP = 1 << 2,
    IMAGE_KIND_BIT_AVI = 1 << 3,
};

// the consoles that touched an image (ImageInfo / ImageDbRecordParam keep one id per type)
enum BodyIdType : u8
{
    BODY_ID_TYPE_NUM = 4,
};

enum ShootingType : u8
{
};

enum HandleTypeBit : u8
{
};

enum DistinctionTypeBit : u8
{
};

enum SaveProcessType : u8
{
};

// an nn::fnd::IAllocator (the global ones: GetAllocator / GetFileAllocator)
class Allocator
{
public:
    Allocator() : m_pAllocator(NULL) {}
    ~Allocator();

    void* Allocate(u32 size, s32 alignment) { return m_pAllocator->Allocate(size, alignment); }
    void Free(void* ptr) { m_pAllocator->Free(ptr); }

    nn::fnd::IAllocator* m_pAllocator;  // +0x00
};
ASSERT_SIZE(Allocator, 0x4);

Allocator* GetAllocator();
Allocator* GetFileAllocator();
void SetAllocator(nn::fnd::IAllocator* allocator, nn::fnd::IAllocator* fileAllocator);

// the allocator of the containers: the global one
template <typename T>
class XAllocator
{
public:
    T* allocate(u32 num) { return static_cast<T*>(GetAllocator()->Allocate(num * sizeof(T), 32)); }
    void deallocate(T* ptr) { GetAllocator()->Free(ptr); }
};

// a growable array
template <typename T, typename A>
class DynamicArray
{
public:
    static const u32 DEFAULT_GROW_STEP = 32;

    DynamicArray()
        : m_GrowStep(DEFAULT_GROW_STEP), m_Capacity(0), m_MinCapacity(0), m_pBegin(NULL), m_pEnd(NULL)
    {
    }
    ~DynamicArray() { Release(); }

    u32 GetSize() const { return m_pEnd - m_pBegin; }
    T& operator[](u32 index) { return m_pBegin[index]; }
    const T& operator[](u32 index) const { return m_pBegin[index]; }
    T* begin() { return m_pBegin; }
    T* end() { return m_pEnd; }
    const T* begin() const { return m_pBegin; }
    const T* end() const { return m_pEnd; }

    // drops the elements from size on
    void Truncate(u32 size)
    {
        if (size < GetSize())
        {
            m_pEnd -= GetSize() - size;
            Refresh();
        }
    }
    void Clear() { Truncate(0); }
    // the first size elements, new ones are copies of value
    void Resize(u32 size, const T& value)
    {
        if (size < GetSize())
        {
            Truncate(size);
            return;
        }
        while (GetSize() < size)
        {
            PushBack(value);
        }
    }
    void SetMinCapacity(u32 capacity)
    {
        m_MinCapacity = capacity;
        if (m_Capacity < capacity)
        {
            Recreate(capacity);
        }
    }
    void Insert(u32 index, const T& value)
    {
        if (GetSize() + 1 > m_Capacity)
        {
            Recreate(GetSize() + 1);
        }
        for (u32 i = GetSize() - index; i != 0; i--)
        {
            m_pBegin[index + i] = m_pBegin[index + i - 1];
        }
        m_pEnd++;
        m_pBegin[index] = value;
    }
    void PushBack(const T& value) { Insert(GetSize(), value); }
    void Erase(T* position)
    {
        T* destination = position;
        T* source = position + 1;
        while (source != m_pEnd)
        {
            *destination++ = *source++;
        }
        m_pEnd--;
        Refresh();
    }

    void Refresh();
    void Recreate(u32 size);

private:
    void Release()
    {
        if (m_pBegin != NULL)
        {
            for (u32 i = 0; i < GetSize(); i++)
            {
            }
            m_Allocator.deallocate(m_pBegin);
            m_pEnd = NULL;
            m_pBegin = NULL;
            m_Capacity = 0;
        }
    }

    A m_Allocator;       // +0x00
    u32 m_GrowStep;      // +0x04
    u32 m_Capacity;      // +0x08
    u32 m_MinCapacity;   // +0x0C
    T* m_pBegin;         // +0x10
    T* m_pEnd;           // +0x14
};

// gives the memory back when more than two grow steps are unused
template <typename T, typename A>
void DynamicArray<T, A>::Refresh()
{
    if (m_Capacity - GetSize() < m_GrowStep * 2)
    {
        return;
    }
    if (GetSize() != 0 || m_MinCapacity > m_GrowStep)
    {
        Recreate(GetSize());
        return;
    }
    Release();
}

// a new buffer for size elements and a grow step (the first size elements are kept)
template <typename T, typename A>
void DynamicArray<T, A>::Recreate(u32 size)
{
    u32 capacity = m_GrowStep + size;
    if (m_GrowStep < m_MinCapacity && capacity < m_MinCapacity)
    {
        capacity = m_MinCapacity;
    }
    if (m_Capacity == capacity)
    {
        m_pEnd = m_pBegin + size;
        return;
    }

    T* buffer = NULL;
    u32 num = 0;
    if (capacity != 0)
    {
        buffer = m_Allocator.allocate(capacity);
        num = size < GetSize() ? size : GetSize();
        for (u32 i = 0; i < num; i++)
        {
            buffer[i] = m_pBegin[i];
        }
    }
    Release();
    m_Capacity = capacity;
    m_pBegin = buffer;
    m_pEnd = buffer + num;
}

// the single instance of T, made with the global allocator
template <typename T>
class Singleton
{
public:
    virtual ~Singleton() {}

    static T* GetInstance() { return s_pInstance; }

    static void CreateInstance(Allocator* allocator = NULL)
    {
        if (s_pInstance == NULL)
        {
            s_pAllocator = allocator;
            if (s_pAllocator == NULL)
            {
                s_pAllocator = GetAllocator();
            }
            void* p = s_pAllocator->Allocate(sizeof(T), 4);
            s_pInstance = p != NULL ? new (p) T : NULL;
        }
    }

    static void DestroyInstance()
    {
        if (s_pInstance != NULL)
        {
            T* instance = s_pInstance;
            s_pInstance = NULL;
            Allocator* allocator = s_pAllocator;
            instance->~T();
            allocator->Free(instance);
        }
    }

protected:
    static T* s_pInstance;
    static Allocator* s_pAllocator;
};

// a DCF directory and file number pair: directory nnn = 100 + index, file nnnn = 1 + index
class IndexInfo
{
public:
    static const s32 DIRECTORY_INDEX_NUM = 900;
    static const s32 FILE_INDEX_MAX = 9998;
    static const s32 OFFICIAL_FILE_NUM = 100;  // files per directory the database writes

    void Initialize();
    bool SetFileIndex(int index);
    static bool IsValidFileIndex(int index);
    bool SetDirectoryIndex(int index);
    static bool IsValidDirectoryIndex(int index);
    void UpdateNextIndexByOfficial();
    void UpdateNextDirectoryIndex();
    bool IsValidFileIndex() const;
    bool IsValidDirectoryIndex() const;
    bool operator==(const IndexInfo& other) const;
    bool operator!=(const IndexInfo& other) const;

    u16 m_DirectoryIndex;  // +0x00
    u16 m_FileIndex;       // +0x02
};
ASSERT_SIZE(IndexInfo, 0x4);

// an s64 that is only 4 byte aligned (DateTimeSeconds lies at +0x14 of ImageInfo)
typedef s64 s64_align4 __attribute__((aligned(4)));

// a point in time in seconds since 2000-01-01 (as nn::fnd::DateTime counts the days)
class DateTimeSeconds
{
public:
    static const s32 SECONDS_PER_DAY = 86400;

    void Initialize();
    void SetSeconds(s64 seconds);
    static DateTimeSeconds GetMaxSeconds();
    static DateTimeSeconds GetMinSeconds();
    bool ConvertFromParam(int year, int month, int day, int hour, int minute, int second);
    static DateTimeSeconds GetCurrentDateTime();
    void SetCurrentDateTime();
    bool ConvertFromNNDateTime(const nn::fnd::DateTime& dateTime);
    bool ConvertFromDateTimeString(const char* string);
    s32 GetDateDays() const;
    void ConvertToParam(int* year, int* month, int* day, int* hour, int* minute, int* second) const;
    nn::fnd::DateTime ConvertToNNDateTime() const;
    void ConvertToDateTimeString(char* buffer, int size) const;
    bool IsInvalid() const;

    s64_align4 m_Seconds;  // +0x00
};
ASSERT_SIZE(DateTimeSeconds, 0x8);

} // namespace imgdb
