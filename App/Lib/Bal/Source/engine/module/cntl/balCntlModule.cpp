/*!
 * @file   balCntlModule.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <engine/module/cntl/balCntlModule.h>

namespace bal::mod::cntl {

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
}

// ----------------------------------------------------------------------------

void Module::onUpdate(const FrameworkCallback::UpdateArg& arg)
{
}

// ----------------------------------------------------------------------------

}
