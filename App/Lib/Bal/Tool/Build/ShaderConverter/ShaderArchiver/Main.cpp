/**
 * @file   Main.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <app/balApplicationBase.h>

class Main : public bal::ApplicationBase
{
public:
    /*!
     * bal のブートセットアップ関数です
     * balMain() の前に呼ばれ、起動のセットアップを行います
     * @param[in]     api_entry 起動したときのオプション設定などが入っているクラスです
     * @param[in,out] p_arg     ブートの設定を格納する
     */
    virtual void setupBoot(const bal::ApiEntry& api_entry, bal::ApiEntry::BootArg* p_arg)
    {
        p_arg->mBootMode = bal::ApiEntry::BootMode::Console;
    }

    /*!
     * bal のエントリーポイントです
     * @param[in] api_entry 起動したときのオプション設定などが入っているクラスです
     */
    virtual int main(const bal::ApiEntry& api_entry)
    {
        return 0;
    }

    /*!
     * アプリケーションモードの時、ループ処理が行われる関数です
     * @note Console モードの場合は 1 回しか呼ばれません
     * @return false を返せばループが終了します
     */
    virtual bool onLoop()
    {
        return true;
    }
};

Main gMain;
