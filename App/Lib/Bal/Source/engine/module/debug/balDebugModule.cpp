/*!
 * @file   balDebugProcessTime.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2021 belmayze. All rights reserved.
 */
// bal
#include <engine/module/debug/balDebugModule.h>
#include <debug/process/balDebugProcessTimeHolder.h>

namespace bal::mod::debug {

// ----------------------------------------------------------------------------

Module::Module() {}

// ----------------------------------------------------------------------------

Module::~Module() {}

// ----------------------------------------------------------------------------

void Module::setCustomModule(std::unique_ptr<mod::ICustomModule>&& p_custom_module)
{
}

// ----------------------------------------------------------------------------

void Module::initialize(const InitializeArg& arg)
{
    // 処理負荷計測
    mpProcessTimeHolder = make_unique<bal::debug::ProcessTimeHolder>(nullptr);
}

// ----------------------------------------------------------------------------

void Module::finalize()
{
}

// ----------------------------------------------------------------------------

void Module::onUpdate(const FrameworkCallback::UpdateArg& arg)
{
}

// ----------------------------------------------------------------------------

}
