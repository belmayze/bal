/*!
 * @file   balDebugProcessTime.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2021 belmayze. All rights reserved.
 */
// bal
#include <debug/process/balDebugProcessTime.h>
#include <debug/process/balDebugProcessTimeHolder.h>
#include <engine/module/debug/balDebugModule.h>

namespace bal::debug {

// ----------------------------------------------------------------------------

ProcessTime::ProcessTime(const StringPtr& name, ProcessHandle* p_parent)
{
    if (mod::debug::Module* p_module = mod::debug::Module::getModule())
    {
        mHandle = p_module->getProcessTimeHolder()->addNode(name, p_parent);
    }
    // 処理計測開始
    mStopwatch.start();
}

// ----------------------------------------------------------------------------

ProcessTime::~ProcessTime()
{
    // 処理計測結果を伝える
    mStopwatch.stop();

    if (mod::debug::Module* p_module = mod::debug::Module::getModule())
    {
        p_module->getProcessTimeHolder()->setTime(mHandle, mStopwatch.getDiff());
    }
}

// ----------------------------------------------------------------------------

}
