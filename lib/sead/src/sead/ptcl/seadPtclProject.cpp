#include "sead/ptcl/seadPtclProject.h"
#include "sead/ptcl/seadPtclProjectEmitterSet.h"

namespace sead {
namespace ptcl {
// 0x005558C8 (name is ours)
sead::ptcl::PtclProject::PtclProject(Heap* heap, PtclSystem* system, PtclEditor* editor, s32 resId)
    : mName(), mDispName(), mMetaName()
{
    mHeap = heap;
    mSystem = system;
    mEditor = editor;
    mResId = resId;
    for (s32 i = 0; i < NUM_EMITTER_SET_MAX; i++) {
        mEmitterSets[i] = nullptr;
    }
}

// 0x00555A54 slot 0x00
// 0x00555A00 (deleting dtor)
sead::ptcl::PtclProject::~PtclProject()
{
    for (s32 i = 0; i < NUM_EMITTER_SET_MAX; i++) {
        if (mEmitterSets[i] != nullptr) {
            delete mEmitterSets[i];
            mEmitterSets[i] = nullptr;
        }
    }
}

// 0x00555708 (name is ours)
void sead::ptcl::PtclProject::removeEmitterSet(PtclProjectEmitterSet* set)
{
    for (s32 i = 0; i < NUM_EMITTER_SET_MAX; i++) {
        if (mEmitterSets[i] == set) {
            if (mEmitterSets[i] != nullptr) {
                delete mEmitterSets[i];
            }
            mEmitterSets[i] = nullptr;
        }
    }
}

// 0x00555754 (name is ours)
void sead::ptcl::PtclProject::initName()
{
    mName.copy(SafeStringBase<char>(""));
    // "エミッタセット一覧" (list of the emitter sets)
    mDispName.copy(SafeStringBase<char>("\xE3\x82\xA8\xE3\x83\x9F\xE3\x83\x83\xE3\x82\xBF\xE3\x82\xBB\xE3\x83\x83\xE3\x83\x88"
                              "\xE4\xB8\x80\xE8\xA6\xA7"));
}
} // namespace ptcl
} // namespace sead
