/*!
 * @file   balEngine.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <engine/balEngine.h>
#include <engine/module/gfx/balGfxModule.h>

namespace bal {

// ----------------------------------------------------------------------------

Engine::Engine() {}

// ----------------------------------------------------------------------------

Engine::~Engine() {}

// ----------------------------------------------------------------------------

bool Engine::initialize(const InitializeArg& arg, ModuleArray&& p_modules, size_t num_module)
{
    // モジュールを保持
    mpModules  = std::move(p_modules);
    mNumModule = num_module;

    // システムを取得
    mpGraphicsSystem = arg.mpGraphics;

    // モジュールを初期化
    {
        mod::IModule::InitializeArg init_arg;

        for (size_t i_module = 0; i_module < mNumModule; ++i_module)
        {
            mpModules[i_module]->initialize(init_arg);

            // グラフィックスモジュールは描画を行う特殊なモジュールなので検索して保持しておく
            if (mpModules[i_module]->getModuleType() == mod::ModuleType::Graphics)
            {
                mpGraphicsModule = static_cast<mod::gfx::Module*>(mpModules[i_module].get());
            }
        }
    }

    // @TODO: 依存を構築する


    return true;
}

// ----------------------------------------------------------------------------

void Engine::onUpdate(const UpdateArg& arg)
{
    // @TODO: 依存をもとにマルチスレッドで実行
    for (size_t i_module = 0; i_module < mNumModule; ++i_module)
    {
        mpModules[i_module]->onUpdate(arg);
    }
}

// ----------------------------------------------------------------------------

void Engine::onDraw(const DrawArg& arg)
{
    if (mpGraphicsModule)
    {
        mpGraphicsModule->onDraw(arg);
    }
}

// ----------------------------------------------------------------------------

}
