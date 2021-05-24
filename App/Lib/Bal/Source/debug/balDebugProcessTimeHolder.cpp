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
void ProcessTimeHolder::clear()
{
    // @TODO: ツリーをクリア
}
// ----------------------------------------------------------------------------
ProcessHandle ProcessTimeHolder::addNode(const StringPtr& name, const ProcessHandle* p_parent)
{
    // @TODO: ツリー構造に登録
    return ProcessHandle();
}
// ----------------------------------------------------------------------------
void ProcessTimeHolder::setTime(const ProcessHandle& handle, const TimeSpan& diff)
{
    // @TODO: ツリーに登録
}
// ----------------------------------------------------------------------------

}
