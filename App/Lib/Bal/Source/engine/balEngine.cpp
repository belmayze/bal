/*!
 * @file   balEngine.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <debug/process/balDebugProcessTimeHolder.h>
#include <engine/balEngine.h>
#include <engine/module/debug/balDebugModule.h>
#include <engine/module/gfx/balGfxModule.h>

namespace bal {

// ----------------------------------------------------------------------------

Engine::Engine() {}

// ----------------------------------------------------------------------------

Engine::~Engine()
{
    // モジュールの終了処理
    for (size_t i_module = 0; i_module < mNumModule; ++i_module)
    {
        mpModules[i_module]->finalize();
    }
}

// ----------------------------------------------------------------------------

bool Engine::initialize(const InitializeArg& arg, ModuleArray&& p_modules, size_t num_module)
{
    // グラフィックス関連の破棄処理を登録しておく
    AddGfxFinalizer();

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
        }
    }

    // @TODO: 依存を構築する


    return true;
}

// ----------------------------------------------------------------------------

void Engine::onUpdate(const UpdateArg& arg)
{
    // デバッグモジュールがあれば処理する
    if (mod::debug::Module* p_module = mod::debug::Module::GetModule())
    {
        p_module->getProcessTimeHolder()->clear();
    }

    // @TODO: 依存をもとにマルチスレッドで実行
    for (size_t i_module = 0; i_module < mNumModule; ++i_module)
    {
        mpModules[i_module]->onUpdate(arg);
    }
}

// ----------------------------------------------------------------------------

void Engine::onDraw(const DrawArg& arg)
{
    // @TODO: 依存をもとにマルチスレッドで実行
    for (size_t i_module = 0; i_module < mNumModule; ++i_module)
    {
        mpModules[i_module]->onDraw(arg);
    }
}

// ----------------------------------------------------------------------------

}
