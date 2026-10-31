#pragma once

#include "decomp.h"
#include "nn/nex/_Type_stationurl/_Type_stationurl_Api.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"
#include "nn/nex/nex_Message.h"
#include "nn/nex/nex_qList.h"
#include "nn/nex/nex_qMap.h"
#include "nn/nex/nex_qVector.h"
#include <algorithm>

namespace nn {
namespace nex {
// The header of the DDL structures (all inline): the version (written as at most the one the client
// knows, 0) and the size of the members, so that a reader skips the members it does not know. It is
// left out when the message has m_IsWithoutStructureHeader. The class and function names are ours.
class DDLStructure
{
public:
    // the version (at most the highest revision of the structure) and a size patched by EndAdd; the
    // offset of the size. The members of a revision are only written with the header and when the
    // version is at least the revision (seen in _DDL_MatchmakeSession::Add). The constructors set
    // the version to the highest revision (seen in RVConnectionData, DataStoreSearchParam and
    // MatchmakeSession).
    static u32 BeginAdd(Message* pMessage, u8 version, u8 maxVersion = 0)
    {
        u8 current = version;
        pMessage->AppendRaw(&std::min(current, maxVersion), sizeof(u8));
        u32 start = pMessage->m_pBuffer->m_ContentSize;
        u32 size = 0;
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&size), sizeof(size));
        return start;
    }
    static void EndAdd(Message* pMessage, u32 start)
    {
        u32 end = pMessage->m_pBuffer->m_ContentSize;
        pMessage->SetLength(start);
        u32 size = end - start - sizeof(size);
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&size), sizeof(size));
        pMessage->SetLength(end);
    }
    // the version and the size (false on an error; the byte read out of line or inline)
    static bool BeginExtract(Message* pMessage, u8* pVersion, u32* pSize)
    {
        pMessage->ExtractPrimitive(pVersion);
        if (pMessage->m_IsError) {
            return false;
        }
        pMessage->ExtractPrimitive(pSize);
        return !pMessage->m_IsError;
    }
    static bool BeginExtractInline(Message* pMessage, u8* pVersion, u32* pSize)
    {
        pMessage->ExtractPrimitiveInline(pVersion);
        if (pMessage->m_IsError) {
            return false;
        }
        pMessage->ExtractPrimitive(pSize);
        return !pMessage->m_IsError;
    }
    static bool BeginExtractFullyInline(Message* pMessage, u8* pVersion, u32* pSize)
    {
        pMessage->ExtractPrimitiveInline(pVersion);
        if (pMessage->m_IsError) {
            return false;
        }
        pMessage->ExtractPrimitiveInline(pSize);
        return !pMessage->m_IsError;
    }
    // the reader continues after the members of the size (the unknown ones are skipped)
    static void EndExtract(Message* pMessage, u32 start, u32 size)
    {
        u32 end = start + size;
        if (pMessage->m_Position <= end && pMessage->m_pBuffer->m_ContentSize >= end) {
            pMessage->m_Position = end;
            return;
        }
        pMessage->m_IsError = true;
    }
};

// The DDL types of the elements of containers (the class names are from the binary).
class _Type_byte
{
public:
    typedef u8 Type;
    static void Add(Message* pMessage, const u8& value) { pMessage->AppendRaw(&value, sizeof(value)); }
    static void Extract(Message* pMessage, u8* pValue) { pMessage->ExtractPrimitiveInline(pValue); }
};
class _Type_uint16
{
public:
    typedef u16 Type;
    static void Add(Message* pMessage, const u16& value) { pMessage->AppendRaw(reinterpret_cast<const u8*>(&value), sizeof(value)); }
    static void Extract(Message* pMessage, u16* pValue) { pMessage->ExtractPrimitiveInline(pValue); }
};
class _Type_uint64
{
public:
    typedef u64 Type;
    static void Add(Message* pMessage, const u64& value) { pMessage->AppendRaw(reinterpret_cast<const u8*>(&value), sizeof(value)); }
    static void Extract(Message* pMessage, u64* pValue) { pMessage->ExtractPrimitiveInline(pValue); }
};
class _Type_uint32
{
public:
    typedef u32 Type;
    static void Add(Message* pMessage, const u32& value) { pMessage->AppendRaw(reinterpret_cast<const u8*>(&value), sizeof(value)); }
    static void Extract(Message* pMessage, u32* pValue) { pMessage->ExtractPrimitiveInline(pValue); }
};

// A vector: the number of elements and the elements (a failed element empties it).
template <typename E>
class _Type_qvector
{
public:
    typedef qVector<typename E::Type> Type;

    // (inline in most users; the instances out of line in the original specialize them with the
    // bodies of AddInline / ExtractInline)
    static DECOMP_ALWAYS_INLINE void Add(Message* pMessage, const Type& vector) { AddInline(pMessage, vector); }
    static DECOMP_ALWAYS_INLINE void Extract(Message* pMessage, Type* pVector) { ExtractInline(pMessage, pVector); }
    static DECOMP_ALWAYS_INLINE void AddInline(Message* pMessage, const Type& vector)
    {
        u32 count = vector.size();
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&count), sizeof(count));
        for (const typename E::Type* p = vector.begin(); p != vector.end(); p++) {
            E::Add(pMessage, *p);
        }
    }
    static DECOMP_ALWAYS_INLINE void ExtractInline(Message* pMessage, Type* pVector)
    {
        pVector->clear();
        u32 count;
        if (!pMessage->ExtractPrimitive(&count)) {
            return;
        }
        pVector->reserve(count);
        for (u32 i = 0; i < count && !pMessage->m_IsError; i++) {
            typename E::Type value;
            E::Extract(pMessage, &value);
            if (pMessage->m_IsError) {
                pVector->clear();
                return;
            }
            pVector->push_back(value);
        }
    }
};

// A list: the number of elements and the elements (_Type_std_list is the same for qList).
template <typename E>
class _Type_qlist
{
public:
    typedef qList<typename E::Type> Type;

    // (out of line in some instances: they are specialized with the body of AddInline)
    static void Add(Message* pMessage, const Type& list) { AddInline(pMessage, list); }
    static void AddInline(Message* pMessage, const Type& list)
    {
        u32 count = list.GetSize();
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&count), sizeof(count));
        for (typename Type::Node* pNode = list.GetFirst(); !list.IsEnd(pNode); pNode = pNode->m_pNext) {
            E::Add(pMessage, pNode->m_Value);
        }
    }
    // a failed element empties the list
    static void Extract(Message* pMessage, Type* pList)
    {
        pList->ClearInline();
        u32 count;
        if (!pMessage->ExtractPrimitiveInline(&count)) {
            return;
        }
        for (u32 i = 0; i < count && !pMessage->m_IsError; i++) {
            typename E::Type value;
            E::Extract(pMessage, &value);
            if (pMessage->m_IsError) {
                pList->ClearInline();
                return;
            }
            pList->PushBackInline(value);
        }
    }
};
template <typename E>
class _Type_std_list
{
public:
    typedef qList<typename E::Type> Type;

    // (out of line in some instances: they are specialized with the body of AddInline)
    static void Add(Message* pMessage, const Type& list) { AddInline(pMessage, list); }
    static void AddInline(Message* pMessage, const Type& list)
    {
        u32 count = list.GetSize();
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&count), sizeof(count));
        for (typename Type::Node* pNode = list.GetFirst(); !list.IsEnd(pNode); pNode = pNode->m_pNext) {
            E::Add(pMessage, pNode->m_Value);
        }
    }
    static void Extract(Message* pMessage, Type* pList)
    {
        pList->clear();
        u32 count = 0;
        pMessage->ExtractPrimitive(&count);
        for (u32 i = 0; i < count; i++) {
            typename E::Type value;
            E::Extract(pMessage, &value);
            pList->push_back(value);
        }
    }
};

// A map: the number of pairs, then key and value of each (inline; the pairs are put in with
// operator[]).
template <typename K, typename V>
class _Type_map
{
public:
    typedef qMap<typename K::Type, typename V::Type> Type;

    static DECOMP_ALWAYS_INLINE void Add(Message* pMessage, const Type& map)
    {
        u32 count = map.size();
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&count), sizeof(count));
        for (typename Type::iterator it = map.begin(); it != map.end(); ++it) {
            K::Add(pMessage, it->first);
            V::Add(pMessage, it->second);
        }
    }
    static DECOMP_ALWAYS_INLINE void Extract(Message* pMessage, Type* pMap)
    {
        pMap->clear();
        u32 count = 0;
        pMessage->ExtractPrimitive(&count);
        for (u32 i = 0; i < count; i++) {
            typename K::Type key;
            typename V::Type value;
            K::Extract(pMessage, &key);
            V::Extract(pMessage, &value);
            (*pMap)[key] = value;
        }
    }
};

// (out of line)
template <>
DECOMP_NOINLINE void _Type_qvector<_Type_byte>::Extract(Message* pMessage, Type* pVector);
template <>
DECOMP_NOINLINE void _Type_qvector<_Type_uint32>::Extract(Message* pMessage, Type* pVector);
template <>
DECOMP_NOINLINE void _Type_qvector<_Type_string>::Extract(Message* pMessage, Type* pVector);

// A DDL structure as a member or element: the Add/Extract of the structure (inline; the instances
// out of line in the original are specialized).
template <typename T>
class DDLClassMetaType
{
public:
    typedef T Type;
    static void Add(Message* pMessage, const T& value) { T::Add(pMessage, value); }
    static void Extract(Message* pMessage, T* pValue) { T::Extract(pMessage, pValue); }
};
// (out of line in the original; nex_DDLTypes.cpp)
template <>
DECOMP_NOINLINE void _Type_std_list<_Type_stationurl>::Add(Message* pMessage, const Type& list);
template <>
DECOMP_NOINLINE void _Type_qlist<_Type_uint32>::Add(Message* pMessage, const Type& list);
} // namespace nex
} // namespace nn
