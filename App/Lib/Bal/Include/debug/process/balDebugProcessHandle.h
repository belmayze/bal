/*!
 * @file   balDebugProcessHandle.h
 * @brief  処理計測のハンドル
 * @author belmayze
 *
 * Copyright (c) 2021 belmayze. All rights reserved.
 */
#pragma once
// std
#include <thread>
// bal
#include <container/balString.h>
#include <container/balTreeMap.h>
#include <time/balTimeSpan.h>

namespace bal::debug { class ProcessTimeHolder; }

// ----------------------------------------------------------------------------
namespace bal::debug {

class ProcessHandle
{
    friend ProcessTimeHolder;
    struct Data;
    using TreeMapNode = TreeMap<Data>::Node;

private:
    //! スレッド情報
    struct ThreadInfo
    {
        ThreadInfo(std::thread::id thread_id) :mThreadID(thread_id) {}
        std::thread::id  mThreadID;
        TreeMapNode* mpCurrentNode = nullptr;
        TreeMapNode* mpRootNode = nullptr;
    };

    //! データ
    struct Data
    {
        Data(const StringPtr& name, const MathColor& color, ThreadInfo* p_info) :mName(name), mColor(color), mpThreadInfo(p_info) {}

        String<128> mName;
        TimeSpan    mDiff;
        MathColor   mColor;
        ThreadInfo* mpThreadInfo;
    };

private:
    TreeMapNode* mpNode = nullptr;
};

}
