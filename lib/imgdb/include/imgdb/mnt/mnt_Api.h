#pragma once

#include "imgdb/imgdb_Types.h"

namespace imgdb {
namespace mnt {

// the SD card archive of the images (SdArchiveMounter)
bool MountSdmc();
void UnmountSdmc();

} // namespace mnt
} // namespace imgdb
