#include "nw/snd/internal/driver/snd_StreamSoundLoader_StreamHeaderLoadTask.h"

#include "nw/snd/internal/driver/snd_StreamSoundPlayer.h"
#include "nw/snd/internal/snd_DriverCommandManager.h"
#include "nw/snd/internal/snd_DriverCommandPlayer.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// 0x004CF724 slot 0x00
// 0x004CF714 (deleting dtor)
nw::snd::internal::driver::StreamSoundLoader::StreamHeaderLoadTask::~StreamHeaderLoadTask()
{
    // empty (in the original too)
}

// 0x004CF680 slot 0x08 | fefates:bytes
void nw::snd::internal::driver::StreamSoundLoader::StreamHeaderLoadTask::Execute()
{
    StreamSoundLoader* pLoader = m_pLoader;
    DriverCommandManager& manager = DriverCommandManager::GetInstanceForTaskThread();
    DriverCommandStreamSoundLoadHeader* pCommand = manager.AllocCommand<DriverCommandStreamSoundLoadHeader>();
    pCommand->id = DriverCommand::ID_STRMSOUND_LOAD_HEADER;
    bool result;
    switch (pLoader->m_FileType) {
    case FILE_TYPE_BCSTM:
        result = pLoader->LoadHeader1(pCommand);
        break;
    case FILE_TYPE_EXTERNAL:
        result = pLoader->LoadHeader2(pCommand);
        break;
    default:
        result = false;
        break;
    }
    pCommand->result = result;
    manager.PushCommand(pCommand);
    manager.FlushCommand(true);
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
