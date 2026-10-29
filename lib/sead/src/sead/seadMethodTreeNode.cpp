#include "sead/seadMethodTreeNode.h"

namespace sead {
// 0x0012C964 | nintendogs:callgraph [tier A]
void sead::MethodTreeNode::lock_()
{
    if (mCriticalSection != nullptr) {
        mCriticalSection->lock();
    }
}

// 0x0012C9E0 | nintendogs:callgraph [tier A]
void sead::MethodTreeNode::unlock_()
{
    if (mCriticalSection != nullptr) {
        mCriticalSection->unlock();
    }
}

// 0x00545154 | nintendogs:callgraph [tier A]
void sead::MethodTreeNode::pushBackChild(MethodTreeNode* node)
{
    if (mCriticalSection != nullptr) {
        mCriticalSection->lock();
    }
    node->detachSubTree();
    node->mCriticalSection = mCriticalSection;
    if (node->child() != nullptr && node->child()->val() != nullptr) {
        node->child()->val()->attachMutexRec_(mCriticalSection);
    }
    TTreeNode<MethodTreeNode*>::pushBackChild(node);
    if (mCriticalSection != nullptr) {
        mCriticalSection->unlock();
    }
}

// 0x005451F4 | nintendogs:callseq [tier A]
void sead::MethodTreeNode::pushFrontChild(MethodTreeNode* node)
{
    if (mCriticalSection != nullptr) {
        mCriticalSection->lock();
    }
    node->detachSubTree();
    node->mCriticalSection = mCriticalSection;
    if (node->child() != nullptr && node->child()->val() != nullptr) {
        node->child()->val()->attachMutexRec_(mCriticalSection);
    }
    TTreeNode<MethodTreeNode*>::pushFrontChild(node);
    if (mCriticalSection != nullptr) {
        mCriticalSection->unlock();
    }
}

// 0x0054529C | nintendogs:bytes [tier A]
void sead::MethodTreeNode::call()
{
    if (mCriticalSection != nullptr) {
        mCriticalSection->lock();
    }
    callDelegate_();
    TTreeNode<MethodTreeNode*>* node = child();
    if (node != nullptr && (mPauseFlag & PAUSE_FLAG_CHILDREN) == 0) {
        do {
            node->val()->callRec_();
            node = node->val()->next();
        } while (node != nullptr);
    }
    if (mCriticalSection != nullptr) {
        mCriticalSection->unlock();
    }
}

// 0x00545314 | nintendogs:bytes [tier A]
void sead::MethodTreeNode::callRec_()
{
    callDelegate_();
    TTreeNode<MethodTreeNode*>* node = child();
    if (node != nullptr && (mPauseFlag & PAUSE_FLAG_CHILDREN) == 0) {
        do {
            node->val()->callRec_();
            node = node->val()->next();
        } while (node != nullptr);
    }
}

// 0x00545374 | nintendogs:bytes [tier A]
void sead::MethodTreeNode::detachAll()
{
    CriticalSection* criticalSection = mCriticalSection;
    attachMutexRec_(nullptr);
    mCriticalSection = criticalSection;
    lock_();
    TTreeNode<MethodTreeNode*>::detachAll();
    unlock_();
    mCriticalSection = nullptr;
}

// 0x00545410 slot 0x00 | slot vf_0x00 of sead::IDisposer
// 0x005453E4 (deleting dtor)
sead::MethodTreeNode::~MethodTreeNode()
{
    detachAll();
}
} // namespace sead
