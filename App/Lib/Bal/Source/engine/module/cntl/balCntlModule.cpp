/*!
 * @file   balCntlModule.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <engine/module/cntl/balCntlModule.h>
#include <engine/module/cntl/balController.h>

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
    // コントローラー初期化
    mNumController = 4;
    mpControllers = make_unique<Controller[]>(nullptr, mNumController);

    for (uint32_t i_controller = 0; i_controller < mNumController; ++i_controller)
    {
        Controller::InitializeArg init_arg;
        init_arg.mIndex = i_controller;
        mpControllers[i_controller].initialize(init_arg);
    }
}

// ----------------------------------------------------------------------------

void Module::finalize()
{
}

// ----------------------------------------------------------------------------

void Module::onUpdate(const FrameworkCallback::UpdateArg& arg)
{
    // COM
    if (FAILED(CoInitialize(nullptr)))
    {
        return;
    }

    for (uint32_t i_controller = 0; i_controller < mNumController; ++i_controller)
    {
        mpControllers[i_controller].update();
    }

    // COM
    CoUninitialize();
}

// ----------------------------------------------------------------------------

}
