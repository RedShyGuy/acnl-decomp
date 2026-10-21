#pragma once

#include "decomp.h"

namespace nn {
namespace util {
namespace ADLFireWall {
// Instantiations found in the binary:
//   nn::util::ADLFireWall::NonCopyable<imgdb::CtrNandArchiveMounter>  typeinfo 0x008D03EC
//   nn::util::ADLFireWall::NonCopyable<imgdb::FileReader>  typeinfo 0x008D03C4
//   nn::util::ADLFireWall::NonCopyable<imgdb::FileWriter>  typeinfo 0x008D03CC
//   nn::util::ADLFireWall::NonCopyable<imgdb::JpegDecoder>  typeinfo 0x008D03D4
//   nn::util::ADLFireWall::NonCopyable<imgdb::JpegEncoder>  typeinfo 0x008D03DC
//   nn::util::ADLFireWall::NonCopyable<imgdb::JpegSaver>  typeinfo 0x008D0404
//   nn::util::ADLFireWall::NonCopyable<imgdb::MpDecoder>  typeinfo 0x008D040C
//   nn::util::ADLFireWall::NonCopyable<imgdb::MpEncoder>  typeinfo 0x008D0414
//   nn::util::ADLFireWall::NonCopyable<imgdb::MpSaver>  typeinfo 0x008D03FC
//   nn::util::ADLFireWall::NonCopyable<imgdb::SdArchiveMounter>  typeinfo 0x008D03E4
//   nn::util::ADLFireWall::NonCopyable<imgdb::TwlNandArchiveMounter>  typeinfo 0x008D03F4
//   nn::util::ADLFireWall::NonCopyable<nn::fnd::IntrusiveLinkedList<nn::fnd::HeapBase, void>::Item>  typeinfo 0x008D045C
//   nn::util::ADLFireWall::NonCopyable<nn::fnd::IntrusiveLinkedList<nn::srv::NotificationHandler, void>::Item>  typeinfo 0x008D0464
//   nn::util::ADLFireWall::NonCopyable<nn::fs::Directory>  typeinfo 0x008D0444
//   nn::util::ADLFireWall::NonCopyable<nn::fs::FileInputStream>  typeinfo 0x008D0424
//   nn::util::ADLFireWall::NonCopyable<nn::fs::FileOutputStream>  typeinfo 0x008D042C
//   nn::util::ADLFireWall::NonCopyable<nn::fs::FileStream>  typeinfo 0x008D041C
//   nn::util::ADLFireWall::NonCopyable<nn::fs::detail::DirectoryBaseImpl>  typeinfo 0x008D043C
//   nn::util::ADLFireWall::NonCopyable<nn::fs::detail::FileBaseImpl>  typeinfo 0x008D0434
//   nn::util::ADLFireWall::NonCopyable<nn::http::Connection>  typeinfo 0x008D046C
//   nn::util::ADLFireWall::NonCopyable<nn::os::HandleObject>  typeinfo 0x008D0454
//   nn::util::ADLFireWall::NonCopyable<nn::os::ThreadPool>  typeinfo 0x008D044C
//
// The base of the classes that cannot be copied (empty; the copy constructor and the assignment
// are private and not defined, like nn::nex::NonCopyable). The template argument is the derived
// class, so that each class gets its own empty base.
template <typename T0>
class NonCopyable
{
protected:
    // (inline)
    NonCopyable() {}

private:
    NonCopyable(const NonCopyable& other);
    NonCopyable& operator=(const NonCopyable& other);
};
} // namespace ADLFireWall
} // namespace util
} // namespace nn
