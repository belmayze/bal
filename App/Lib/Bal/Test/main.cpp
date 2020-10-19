/*!
 * @file   main.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <app/balApplicationBase.h>
#include <engine/balEngine.h>
#include <framework/balFramework.h>
#include <io/balLog.h>
#include <container/balArray.h>
#include <container/balString.h>
#include <memory/balUniquePtr.h>
// bal::mod
#include <engine/module/cntl/balCntlModule.h>
#include <engine/module/gfx/balGfxModule.h>

namespace app {

class Main : public bal::ApplicationBase
{
public:
    /*!
     * bal のブートセットアップ関数です
     * balMain() の前に呼ばれ、起動のセットアップを行います
     * @param[in,out] p_arg ブートの設定を格納する
     */
    virtual void setupBoot(bal::ApiEntry::BootArg* p_arg)
    {
        p_arg->mBootMode = bal::ApiEntry::BootMode::Application;
        p_arg->mRootHeapSize = 100 * 1024 * 1024;
    }

    /*!
     * bal のエントリーポイントです
     * @param[in] api_entry 起動したときのオプション設定などが入っているクラスです
     */
    virtual int main(const bal::ApiEntry& api_entry)
    {
        // フレームワーク
        bal::Framework framework;
        {
            bal::Framework::InitializeArg init_arg;
            init_arg.mTitle             = "Test";
            init_arg.mRenderSize        = bal::MathSize(1280, 720);
            init_arg.mRenderBufferCount = 3;
            init_arg.mpApplication      = this;
            framework.initialize(api_entry, init_arg);
        }

        // 必要なモジュールを作成します
        constexpr size_t cNumModule = 2;
        bal::Engine::ModuleArray p_modules = bal::make_unique<std::unique_ptr<bal::mod::IModule>[]>(nullptr, cNumModule);
        {
            int module_index = 0;
            // グラフィックス
            p_modules[module_index++] = bal::make_unique<bal::mod::gfx::Module>(nullptr);
            // コントローラー
            p_modules[module_index++] = bal::make_unique<bal::mod::cntl::Module>(nullptr);
        }

        // エンジン
        {
            bal::Engine::InitializeArg init_arg;
            init_arg.mpGraphics        = framework.getGraphics();
            init_arg.mRenderBufferSize = bal::MathSize(1280, 720);
            bal::Engine::GetInstance().AddGfxFinalizer();
            bal::Engine::GetInstance().initialize(init_arg, std::move(p_modules), cNumModule);
        }

        return framework.enterApplicationLoop(&bal::Engine::GetInstance());
    }

};

Main gMain;

} // app::
