#include "nw/lyt/lyt_Layout.h"

#include "nw/lyt/internal/internal_Api.h"
#include "nw/lyt/lyt_AnimResource.h"
#include "nw/lyt/lyt_AnimTransformBasic.h"
#include "nw/lyt/lyt_Bounding.h"
#include "nw/lyt/lyt_DrawInfo.h"
#include "nw/lyt/lyt_GraphicsResource.h"
#include "nw/lyt/lyt_Group.h"
#include "nw/lyt/lyt_GroupContainer.h"
#include "nw/lyt/lyt_Pane.h"
#include "nw/lyt/lyt_Picture.h"
#include "nw/lyt/lyt_ResourceFormat.h"
#include "nw/lyt/lyt_TextBox.h"
#include "nw/lyt/lyt_Window.h"

namespace nw {
namespace lyt {
namespace {
// 0x004B7120 (name is ours)
// the tag processor of the text boxes of the pane tree
void SetTagProcessorImpl(Pane* pPane, TextBox::TagProcessor* pTagProcessor)
{
    TextBox* pTextBox = DynamicCast<TextBox>(pPane);
    if (pTextBox != nullptr) {
        pTextBox->SetTagProcessor(pTagProcessor);
    }
    for (Pane::PaneList::iterator it = pPane->m_ChildList.begin(); it != pPane->m_ChildList.end(); ++it) {
        SetTagProcessorImpl(Pane::FromLink(it.GetNode()), pTagProcessor);
    }
}

// (inline) the pane (or, with bDescendingBind, one of its ancestors) is in one of the groups
bool IsIncludeAnimationGroupList(GroupContainer* pGroupContainer, Pane* pTargetPane, const res::AnimationGroupRef* groupRefs, u16 bindGroupNum,
                                 bool bDescendingBind)
{
    for (u16 grpIdx = 0; grpIdx < bindGroupNum; ++grpIdx) {
        Group* pGroup = pGroupContainer->FindGroupByName(groupRefs[grpIdx].name);
        Group::PaneLinkList& paneList = pGroup->GetPaneList();
        for (Group::PaneLinkList::iterator it = paneList.begin(); it != paneList.end(); ++it) {
            Pane* pGroupPane = Group::PaneLinkList::GetPointerFromNode(it.GetNode())->m_pTarget;
            if (pGroupPane == pTargetPane) {
                return true;
            }
            if (bDescendingBind) {
                for (Pane* pParent = pTargetPane->m_pParent; pParent != nullptr; pParent = pParent->m_pParent) {
                    if (pGroupPane == pParent) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
} // namespace

// 0x00976158 (name is ours)
os::IAllocator* nw::lyt::Layout::s_pAllocator = nullptr;
// 0x0097615C (name is ours)
os::IAllocator* nw::lyt::Layout::s_pDeviceMemoryAllocator = nullptr;

// 0x004B4AFC (name is ours)
void Initialize(os::IAllocator* pAllocator, os::IAllocator* pDeviceAllocator)
{
    Layout::SetAllocator(pAllocator);
    Layout::SetDeviceMemoryAllocator(pDeviceAllocator);
}

// 0x004BA1DC (name is ours)
void nw::lyt::Layout::SetAllocator(os::IAllocator* pAllocator)
{
    s_pAllocator = pAllocator;
}

// 0x004BA720 (name is ours)
void nw::lyt::Layout::SetDeviceMemoryAllocator(os::IAllocator* pAllocator)
{
    s_pDeviceMemoryAllocator = pAllocator;
}

// 0x004BA010 | libgarden [tier A]
void* nw::lyt::Layout::AllocMemory(u32 size, u8 alignment)
{
    return s_pAllocator->Alloc(size, alignment);
}

// 0x0013BB1C | libgarden [tier A]
void nw::lyt::Layout::FreeMemory(void* ptr)
{
    s_pAllocator->Free(ptr);
}

// 0x004BA278 | nintendogs:bytes [tier A]
void* nw::lyt::Layout::AllocDeviceMemory(u32 size, u8 alignment)
{
    return s_pDeviceMemoryAllocator->Alloc(size, alignment);
}

// 0x004BA25C (name is ours)
void nw::lyt::Layout::FreeDeviceMemory(void* ptr)
{
    s_pDeviceMemoryAllocator->Free(ptr);
}

// 0x001322C8 | nintendogs:bytes [tier A]
nw::lyt::Layout::Layout() : m_pRootPane(nullptr), m_pGroupContainer(nullptr), m_LayoutSize(0.0f, 0.0f)
{
}

// 0x004BAA68 slot 0x00 | nintendogs:bytes
// 0x004BAA54 (deleting dtor)
nw::lyt::Layout::~Layout()
{
    DeleteObj(m_pGroupContainer);

    if (m_pRootPane != nullptr && !m_pRootPane->IsUserAllocated()) {
        DeleteObj(m_pRootPane);
    }

    for (AnimTransformList::iterator it = m_AnimTransList.begin(); it != m_AnimTransList.end();) {
        AnimTransformList::iterator currIt = it;
        ++it;
        m_AnimTransList.erase(currIt);
        DeleteObj(AnimTransformList::GetPointerFromNode(currIt.GetNode()));
    }
}

// 0x004BA794 slot 0x08 | nintendogs:bytes
bool nw::lyt::Layout::Build(const void* lytResBuf, ResourceAccessor* pResAcsr)
{
    const ut::BinaryFileHeader* pFileHead = static_cast<const ut::BinaryFileHeader*>(lytResBuf);
    ut::IsValidBinaryFile(pFileHead, res::FILE_SIGNATURE_LAYOUT, res::FILE_VERSION, 1);

    bool bReadRootGroup = false;
    int groupNestLevel = 0;
    ResBlockSet resBlockSet = {nullptr, nullptr, nullptr, pResAcsr};
    Pane* pParentPane = nullptr;
    Pane* pLastPane = nullptr;

    const void* dataPtr = reinterpret_cast<const void*>(reinterpret_cast<uptr>(lytResBuf) + pFileHead->headerSize);
    for (int i = 0; i < pFileHead->dataBlocks; ++i) {
        const ut::BinaryBlockHeader* pDataBlockHead = static_cast<const ut::BinaryBlockHeader*>(dataPtr);
        const u32 kind = pDataBlockHead->kind;
        switch (kind) {
        case res::BLOCK_KIND_LAYOUT:
            m_LayoutSize = static_cast<const res::Layout*>(dataPtr)->layoutSize;
            break;
        case res::BLOCK_KIND_TEXTURE_LIST:
            resBlockSet.pTextureList = static_cast<const res::TextureList*>(dataPtr);
            break;
        case res::BLOCK_KIND_FONT_LIST:
            resBlockSet.pFontList = static_cast<const res::FontList*>(dataPtr);
            break;
        case res::BLOCK_KIND_MATERIAL_LIST:
            resBlockSet.pMaterialList = static_cast<const res::MaterialList*>(dataPtr);
            break;
        case res::BLOCK_KIND_PANE:
        case res::BLOCK_KIND_PICTURE:
        case res::BLOCK_KIND_TEXTBOX:
        case res::BLOCK_KIND_WINDOW:
        case res::BLOCK_KIND_BOUNDING: {
            Pane* pPane = BuildPaneObj(kind, dataPtr, resBlockSet);
            if (pPane != nullptr) {
                if (m_pRootPane == nullptr) {
                    m_pRootPane = pPane;
                }
                if (pParentPane != nullptr) {
                    pParentPane->AppendChild(pPane);
                }
                pLastPane = pPane;
            }
            break;
        }
        case res::BLOCK_KIND_USER_DATA_LIST:
            pLastPane->m_pExtUserDataList = dataPtr;
            break;
        case res::BLOCK_KIND_PANE_CHILD_START:
            pParentPane = pLastPane;
            break;
        case res::BLOCK_KIND_PANE_CHILD_END:
            pLastPane = pParentPane;
            pParentPane = pLastPane->m_pParent;
            break;
        case res::BLOCK_KIND_GROUP:
            if (!bReadRootGroup) {
                bReadRootGroup = true;
                m_pGroupContainer = NewObj<GroupContainer>();
            } else if (m_pGroupContainer != nullptr && groupNestLevel == 1) {
                Group* pGroup = NewObj<Group>(static_cast<const res::Group*>(dataPtr), m_pRootPane);
                if (pGroup != nullptr) {
                    m_pGroupContainer->AppendGroup(pGroup);
                }
            }
            break;
        case res::BLOCK_KIND_GROUP_CHILD_START:
            ++groupNestLevel;
            break;
        case res::BLOCK_KIND_GROUP_CHILD_END:
            --groupNestLevel;
            break;
        }
        dataPtr = reinterpret_cast<const void*>(reinterpret_cast<uptr>(dataPtr) + pDataBlockHead->size);
    }
    return true;
}

// 0x004BA678 slot 0x0C | nintendogs:bytes
AnimTransform* nw::lyt::Layout::CreateAnimTransform()
{
    AnimTransformBasic* pAnimTrans = NewObj<AnimTransformBasic>();
    if (pAnimTrans != nullptr) {
        m_AnimTransList.PushBack(pAnimTrans);
    }
    return pAnimTrans;
}

// 0x004BA5F0 slot 0x10 | nintendogs:bytes
AnimTransform* nw::lyt::Layout::CreateAnimTransform(const void* animResBuf, ResourceAccessor* pResAcsr)
{
    const AnimResource animRes(animResBuf);
    return CreateAnimTransform(animRes, pResAcsr);
}

// 0x004BA62C slot 0x14 | libgarden
AnimTransform* nw::lyt::Layout::CreateAnimTransform(const AnimResource& animRes, ResourceAccessor* pResAcsr)
{
    const res::AnimationBlock* pAnimBlock = animRes.GetResourceBlock();
    if (pAnimBlock == nullptr) {
        return nullptr;
    }
    AnimTransform* pAnimTrans = CreateAnimTransform();
    if (pAnimTrans != nullptr) {
        pAnimTrans->SetResource(pAnimBlock, pResAcsr);
    }
    return pAnimTrans;
}

// 0x004BA6D0 slot 0x18 (name is ours)
void nw::lyt::Layout::DeleteAnimTransform(AnimTransform* pAnimTrans)
{
    m_AnimTransList.Erase(pAnimTrans);
    DeleteObj(pAnimTrans);
}

// 0x004BA200 slot 0x1C | nintendogs:bytes
void nw::lyt::Layout::BindAnimation(AnimTransform* pAnimTrans)
{
    if (m_pRootPane != nullptr) {
        m_pRootPane->BindAnimation(pAnimTrans, true, false);
    }
}

// 0x004BA224 slot 0x20 | nintendogs:bytes
void nw::lyt::Layout::UnbindAnimation(AnimTransform* pAnimTrans)
{
    if (m_pRootPane != nullptr) {
        m_pRootPane->UnbindAnimation(pAnimTrans, true);
    }
}

// 0x00137B18 slot 0x24
void nw::lyt::Layout::UnbindAllAnimation()
{
    UnbindAnimation(nullptr);
}

// 0x004BA29C slot 0x28 (name is ours)
bool nw::lyt::Layout::BindAnimationAuto(const AnimResource& animRes, ResourceAccessor* pResAcsr)
{
    if (m_pRootPane == nullptr) {
        return false;
    }
    if (animRes.GetResourceBlock() == nullptr) {
        return false;
    }
    AnimTransform* pAnimTrans = CreateAnimTransform();
    if (pAnimTrans == nullptr) {
        return false;
    }

    bool bResult = true;
    const u16 bindGroupNum = animRes.GetGroupNum();
    u16 animNum = 0;
    if (bindGroupNum == 0) {
        animNum = animRes.GetResourceBlock()->animContNum;
        pAnimTrans->SetResource(animRes.GetResourceBlock(), pResAcsr, animNum);
        m_pRootPane->BindAnimation(pAnimTrans, true, true);
    } else {
        const res::AnimationGroupRef* groupRefs = animRes.GetGroupArray();
        for (int grpIdx = 0; grpIdx < bindGroupNum; ++grpIdx) {
            Group* pGroup = GetGroupContainer()->FindGroupByName(groupRefs[grpIdx].name);
            if (pGroup == nullptr) {
                bResult = false;
                continue;
            }
            animNum += animRes.CalcAnimationNum(pGroup, animRes.IsDescendingBind());
        }
        pAnimTrans->SetResource(animRes.GetResourceBlock(), pResAcsr, animNum);
        for (int grpIdx = 0; grpIdx < bindGroupNum; ++grpIdx) {
            Group* pGroup = GetGroupContainer()->FindGroupByName(groupRefs[grpIdx].name);
            if (pGroup != nullptr) {
                lyt::BindAnimation(pGroup, pAnimTrans, animRes.IsDescendingBind(), true);
            }
        }
    }

    // the panes of the groups that share the animations of a pane
    const u16 animShareInfoNum = animRes.GetAnimationShareInfoNum();
    if (animShareInfoNum > 0) {
        const res::AnimationShareInfo* animShareInfoAry = animRes.GetAnimationShareInfoArray();
        for (int i = 0; i < animShareInfoNum; ++i) {
            Pane* pSrcPane = m_pRootPane->FindPaneByName(animShareInfoAry[i].srcPaneName, true);
            if (pSrcPane == nullptr) {
                bResult = false;
                continue;
            }
            internal::AnimPaneTree animPaneTree(pSrcPane, animRes);
            if (!animPaneTree.IsEnabled()) {
                continue;
            }
            Group* pGroup = GetGroupContainer()->FindGroupByName(animShareInfoAry[i].targetGroupName);
            if (pGroup == nullptr) {
                bResult = false;
                continue;
            }
            Group::PaneLinkList& paneList = pGroup->GetPaneList();
            for (Group::PaneLinkList::iterator it = paneList.begin(); it != paneList.end(); ++it) {
                Pane* pTargetPane = Group::PaneLinkList::GetPointerFromNode(it.GetNode())->m_pTarget;
                if (pTargetPane == pSrcPane) {
                    continue;
                }
                if (bindGroupNum > 0 &&
                    !IsIncludeAnimationGroupList(GetGroupContainer(), pTargetPane, animRes.GetGroupArray(), bindGroupNum, animRes.IsDescendingBind())) {
                    continue;
                }
                animPaneTree.Bind(this, pTargetPane, pResAcsr);
            }
        }
    }
    return bResult;
}

// 0x004BA5D0 slot 0x2C (name is ours)
void nw::lyt::Layout::SetAnimationEnable(AnimTransform* pAnimTrans, bool bEnable)
{
    if (m_pRootPane != nullptr) {
        m_pRootPane->SetAnimationEnable(pAnimTrans, bEnable, true);
    }
}

// 0x004BAA38 slot 0x30 (name is ours)
void nw::lyt::Layout::Animate(u32 option)
{
    if (m_pRootPane != nullptr) {
        m_pRootPane->Animate(option);
    }
}

// 0x004BA1C0 slot 0x34
void nw::lyt::Layout::CalculateMtx(const DrawInfo& drawInfo)
{
    if (m_pRootPane != nullptr) {
        m_pRootPane->CalculateMtx(drawInfo);
    }
}

// 0x004BA730 slot 0x38 (name is ours)
void nw::lyt::Layout::Draw(DrawInfo& drawInfo)
{
    if (m_pRootPane == nullptr || drawInfo.m_pGraphicsResource == nullptr) {
        return;
    }
    GraphicsResource& graphicsResource = *drawInfo.m_pGraphicsResource;
    graphicsResource.ResetGlState();
    graphicsResource.ResetGlProgramState();
    graphicsResource.SetProjectionMtx(drawInfo.m_ProjMtx);
    drawInfo.m_pLayout = this;
    m_pRootPane->Draw(drawInfo);
    GraphicsResource::FinalizeGraphics();
    drawInfo.m_pLayout = nullptr;
}

// 0x004B7118 slot 0x3C (name is ours)
void nw::lyt::Layout::SetTagProcessor(font::TagProcessorBase<wchar_t>* pTagProcessor)
{
    SetTagProcessorImpl(m_pRootPane, pTagProcessor);
}

// 0x004BA034 slot 0x40 | nintendogs:callseq
Pane* nw::lyt::Layout::BuildPaneObj(s32 kind, const void* dataPtr, const ResBlockSet& resBlockSet)
{
    switch (kind) {
    case res::BLOCK_KIND_PANE:
        return NewObj<Pane>(static_cast<const res::Pane*>(dataPtr));
    case res::BLOCK_KIND_PICTURE:
        return NewObj<Picture>(static_cast<const res::Picture*>(dataPtr), resBlockSet);
    case res::BLOCK_KIND_TEXTBOX:
        return NewObj<TextBox>(static_cast<const res::TextBox*>(dataPtr), resBlockSet);
    case res::BLOCK_KIND_WINDOW:
        return NewObj<Window>(static_cast<const res::Window*>(dataPtr), resBlockSet);
    case res::BLOCK_KIND_BOUNDING:
        return NewObj<Bounding>(static_cast<const res::Bounding*>(dataPtr), resBlockSet);
    default:
        return nullptr;
    }
}

// 0x0073D320 (name is ours)
ut::Rect nw::lyt::Layout::GetLayoutRect() const
{
    return ut::Rect(m_LayoutSize.width * -0.5f, m_LayoutSize.height * 0.5f, m_LayoutSize.width * 0.5f, m_LayoutSize.height * -0.5f);
}

} // namespace lyt
} // namespace nw
