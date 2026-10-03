#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/dbm/dbm_Result.h"

namespace nn {
namespace dbm {

// A read-only hash table in two storages (the RomFs metadata, 3dbrew "RomFS"): the bucket storage
// holds the position of the first element of each bucket, the entry storage the elements
//   key, value, position of the next element of the bucket, size of the aux data, aux data
// (the aux data of the RomFs tables is the UTF-16 name). The class name and the template
// parameters (bucket storage, entry storage, key, value, most aux bytes) are from the symbols,
// the member names are ours.
template <typename BucketStorageT, typename EntryStorageT, typename KeyT, typename ValueT, size_t MaxAuxSize>
class KeyValueRomStorageTemplate
{
public:
    // the end of a bucket chain / an empty bucket
    static const u32 INVALID_POSITION = 0xFFFFFFFF;

    KeyValueRomStorageTemplate()
        : mBucketOffset(0), mBucketCount(0), mBucketStorage(0), mEntryOffset(0), mEntrySize(0),
          mEntryStorage(0), mUnknown20(0), mUnknown24(0)
    {
    }

    // inline (in RomFsArchive::Initialize; name is ours)
    void Initialize(BucketStorageT* bucketStorage, s64 bucketOffset, u32 bucketCount, EntryStorageT* entryStorage,
                    s64 entryOffset, u32 entrySize)
    {
        mBucketOffset = bucketOffset;
        mBucketCount = bucketCount;
        mBucketStorage = bucketStorage;
        mEntryOffset = entryOffset;
        mEntrySize = entrySize;
        mEntryStorage = entryStorage;
    }

    // the element at position (the aux data only if both outAux and outAuxSize are given)
    nn::Result GetByPosition(KeyT* outKey, ValueT* outValue, void* outAux, u32* outAuxSize, u32 position) const;

    // finds the element of key and aux (hash: of both, chooses the bucket)
    nn::Result GetInternal(u32* outPosition, ValueT* outValue, const KeyT& key, u32 hash, const void* aux,
                           u32 auxSize) const;

private:
    struct Element
    {
        KeyT key;
        ValueT value;
        u32 next;   // the next element of the bucket
        u32 size;   // of the aux data behind the element
    };

    // inline (names are ours)
    nn::Result ReadBucket(u32* outPosition, u32 index) const
    {
        return mBucketStorage->ReadBytes(mBucketOffset + index * sizeof(u32), outPosition, sizeof(u32));
    }

    nn::Result ReadKeyValue(Element* element, void* outAux, u32* outAuxSize, u32 position) const
    {
        nn::Result result = mEntryStorage->ReadBytes(mEntryOffset + position, element, sizeof(Element));
        if (result.IsFailure()) {
            return result;
        }
        if (outAux != 0 && outAuxSize != 0) {
            *outAuxSize = element->size;
            if (element->size != 0) {
                result = mEntryStorage->ReadBytes(mEntryOffset + position + sizeof(Element), outAux, element->size);
                if (result.IsFailure()) {
                    return result;
                }
            }
        }
        return nn::Result();
    }

    nn::Result FindInternal(u32* outPosition, Element* outElement, const KeyT& key, u32 hash, const void* aux,
                            u32 auxSize) const
    {
        *outPosition = 0;
        u32 position;
        nn::Result result = ReadBucket(&position, hash % mBucketCount);
        if (result.IsFailure()) {
            return result;
        }
        if (position == INVALID_POSITION) {
            return nn::Result(RESULT_KEY_NOT_FOUND);
        }
        for (;;) {
            u8 buffer[MaxAuxSize];
            u32 size;
            result = ReadKeyValue(outElement, buffer, &size, position);
            if (result.IsFailure()) {
                return result;
            }
            if (key.IsEqual(outElement->key, aux, auxSize, buffer, size)) {
                *outPosition = position;
                return nn::Result();
            }
            position = outElement->next;
            if (position == INVALID_POSITION) {
                return nn::Result(RESULT_KEY_NOT_FOUND);
            }
        }
    }

    s64 mBucketOffset;              // 0x00 in the bucket storage
    u32 mBucketCount;               // 0x08
    BucketStorageT* mBucketStorage; // 0x0C
    s64 mEntryOffset;               // 0x10 in the entry storage
    u32 mEntrySize;                 // 0x18 bytes of the entry storage
    EntryStorageT* mEntryStorage;   // 0x1C
    u32 mUnknown20;                 // 0x20 only set to 0 here
    u32 mUnknown24;                 // 0x24
};

template <typename BucketStorageT, typename EntryStorageT, typename KeyT, typename ValueT, size_t MaxAuxSize>
nn::Result KeyValueRomStorageTemplate<BucketStorageT, EntryStorageT, KeyT, ValueT, MaxAuxSize>::GetByPosition(
    KeyT* outKey, ValueT* outValue, void* outAux, u32* outAuxSize, u32 position) const
{
    Element element;
    nn::Result result = ReadKeyValue(&element, outAux, outAuxSize, position);
    if (result.IsFailure()) {
        return result;
    }
    *outKey = element.key;
    *outValue = element.value;
    return nn::Result();
}

template <typename BucketStorageT, typename EntryStorageT, typename KeyT, typename ValueT, size_t MaxAuxSize>
nn::Result KeyValueRomStorageTemplate<BucketStorageT, EntryStorageT, KeyT, ValueT, MaxAuxSize>::GetInternal(
    u32* outPosition, ValueT* outValue, const KeyT& key, u32 hash, const void* aux, u32 auxSize) const
{
    u32 position;
    Element element;
    nn::Result result = FindInternal(&position, &element, key, hash, aux, auxSize);
    if (result.IsFailure()) {
        return result;
    }
    *outPosition = position;
    *outValue = element.value;
    return nn::Result();
}

} // namespace dbm
} // namespace nn
