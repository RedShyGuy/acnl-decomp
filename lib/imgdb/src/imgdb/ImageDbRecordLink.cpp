#include "imgdb/ImageDbRecordLink.h"

namespace imgdb {

// 0x005AAF38 | nintendogs:callgraph [tier C]
void imgdb::ImageDbRecordLink::Initialize()
{
    std::memset(this, 0, sizeof(*this));
}

// 0x005AAF48 | nintendogs:bytes [tier B]
void imgdb::ImageDbRecordLink::SetImageKind(ImageKind kind)
{
    m_ImageKind = kind;
}

// 0x005AAF64 | nintendogs:bytes [tier B]
void imgdb::ImageDbRecordLink::SetIndexInfo(const IndexInfo& index)
{
    m_DirectoryIndex = index.m_DirectoryIndex;
    m_FileIndex = index.m_FileIndex;
}

// 0x005AAFA4 | nintendogs:bytes [tier B]
void imgdb::ImageDbRecordLink::SetStorageType(StorageType type)
{
    m_StorageType = type;
}

// 0x005AAFBC | nintendogs:bytes [tier B]
void imgdb::ImageDbRecordLink::SetDbRecordIndex(int index)
{
    m_DbRecordIndex = index;
}

// 0x005AAFD4 | nintendogs:bytes [tier B]
void imgdb::ImageDbRecordLink::SetDays(int days)
{
    m_Days = days;
}

// 0x00755B6C | nintendogs:callgraph [tier C]
imgdb::StorageType imgdb::ImageDbRecordLink::GetStorageType() const
{
    return static_cast<StorageType>(m_StorageType);
}

// 0x00755B78 | nintendogs:callgraph [tier C]
int imgdb::ImageDbRecordLink::GetDbRecordIndex() const
{
    return m_DbRecordIndex;
}

// 0x00755B84 (name is ours)
int imgdb::ImageDbRecordLink::GetDays() const
{
    return m_Days;
}

// 0x00755B5C (name is ours)
imgdb::ImageKind imgdb::ImageDbRecordLink::GetImageKind() const
{
    return static_cast<ImageKind>(m_ImageKind);
}

} // namespace imgdb
