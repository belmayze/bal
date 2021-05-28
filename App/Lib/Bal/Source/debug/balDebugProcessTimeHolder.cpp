/*!
 * @file   balDebugProcessTimeHolder.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2021 belmayze. All rights reserved.
 */
// bal
#include <debug/process/balDebugProcessTimeHolder.h>

namespace bal::debug {

// ----------------------------------------------------------------------------
ProcessTimeHolder::ProcessTimeHolder()
{
}
// ----------------------------------------------------------------------------
ProcessTimeHolder::~ProcessTimeHolder()
{
}
// ----------------------------------------------------------------------------
void ProcessTimeHolder::initialize(const InitializeArg& arg)
{
    mTreeMap.initialize(arg.maxNode);
    mThreadInfoList.initialize(arg.maxThread);
}
// ----------------------------------------------------------------------------
void ProcessTimeHolder::clear()
{
    std::shared_lock lock(mMutexList);
    for (const ProcessHandle::ThreadInfo& info : mThreadInfoList)
    {
        if (info.mpRootNode)
        {
            mTreeMap.remove(info.mpRootNode->getChild());
        }
    }
}
// ----------------------------------------------------------------------------
ProcessHandle ProcessTimeHolder::addNode(const StringPtr& name)
{
    // ツリー構造に登録
    ProcessHandle handle;

    // 現在のスレッド情報取得
    ProcessHandle::ThreadInfo* p_thread_info = nullptr;
    {
        std::shared_lock lock(mMutexList);
        for (ProcessHandle::ThreadInfo& info : mThreadInfoList)
        {
            if (info.mThreadID == std::this_thread::get_id())
            {
                p_thread_info = &info;
                break;
            }
        }
    }
    if (!p_thread_info)
    {
        std::lock_guard lock(mMutexList);
        // 再検索
        for (ProcessHandle::ThreadInfo& info : mThreadInfoList)
        {
            if (info.mThreadID == std::this_thread::get_id())
            {
                p_thread_info = &info;
                break;
            }
        }
        if (!p_thread_info)
        {
            p_thread_info = &mThreadInfoList.emplaceBack(std::this_thread::get_id());

            std::lock_guard lock(mMutexTreeMap);
            p_thread_info->mpRootNode = mTreeMap.emplaceChild(mTreeMap.getRootNode(), "Thread", p_thread_info);
            p_thread_info->mpCurrentNode = p_thread_info->mpRootNode;
        }
    }

    // ロック
    {
        std::lock_guard lock(mMutexTreeMap);

        if (mTreeMap.capacity() > 0)
        {
            handle.mpNode = mTreeMap.emplaceChild(p_thread_info->mpCurrentNode, name, p_thread_info);
            p_thread_info->mpCurrentNode = handle.mpNode;
        }
    }
    return handle;
}
// ----------------------------------------------------------------------------
void ProcessTimeHolder::end(const ProcessHandle& handle, const TimeSpan& diff)
{
    if (handle.mpNode)
    {
        handle.mpNode->getData().mDiff = diff;
        handle.mpNode->getData().mpThreadInfo->mpCurrentNode = handle.mpNode->getData().mpThreadInfo->mpCurrentNode->getParent();
    }
}
// ----------------------------------------------------------------------------

}
