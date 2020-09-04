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
// test
#include "testHeap.h"

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
        //p_arg->mBootMode = bal::ApiEntry::BootMode::Console;
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

        // エンジン
        bal::Engine engine;
        {
            bal::Engine::InitializeArg init_arg;
            init_arg.mpGraphics        = framework.getGraphics();
            init_arg.mRenderBufferSize = bal::MathSize(1280, 720);
            engine.initialize(init_arg);
        }

        return framework.enterApplicationLoop(&engine);
    }

    /*!
     * アプリケーションモードの時、ループ処理が行われる関数です
     * @note Console モードの場合は 1 回しか呼ばれません
     * @return false を返せばループが終了します
     */
    virtual bool onLoop()
    {
        //bal::Array<std::unique_ptr<test::TestBase>, 1> arr = {
        //    bal::make_unique<test::TestHeap>(nullptr, "TestHeap")
        //};
        //for (auto& test : arr)
        //{
        //    test->exec();
        //}

        return true;
    }

};

Main gMain;
