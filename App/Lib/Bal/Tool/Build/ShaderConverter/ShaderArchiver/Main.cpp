/**
 * @file   Main.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <app/balApplicationBase.h>
#include <framework/balFramework.h>
#include <io/balFile.h>

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
        p_arg->mBootMode = bal::ApiEntry::BootMode::Console;
    }

    /*!
     * bal のエントリーポイントです
     * @param[in] api_entry 起動したときのオプション設定などが入っているクラスです
     */
    virtual int main(const bal::ApiEntry& api_entry)
    {
        bal::Framework framework;
        {
            bal::Framework::InitializeArg init_arg;
            init_arg.mpApplication      = this;
            framework.initialize(api_entry, init_arg);
        }

        framework.enterApplicationLoop();

        // オプション検索
        bool success = false;
        for (uint32_t i_option = 0; i_option < api_entry.getNumOption(); ++i_option)
        {
            if (api_entry.getOptions()[i_option].first.isEquals("-archive-list"))
            {
                success = archive_(api_entry.getOptions()[i_option].second.c_str());
                break;
            }
        }

        return success ? 0 : 1;
    }

private:
    /*!
     * アーカイブ処理を行います
     * @param[in] filepath アーカイブ対象が含まれているテキストファイル
     */
    bool archive_(bal::StringPtr filepath)
    {
        // テキストファイル読み込み
        bal::File arg_file;
        if (!arg_file.loadFromFile(filepath))
        {
            return false;
        }
        bal::StringPtr arg_txt(reinterpret_cast<const char*>(arg_file.getBuffer()), static_cast<int32_t>(arg_file.getBufferSize()));
        auto [lines, num_line] = arg_txt.getLines();

        for (uint32_t i_line = 0; i_line < num_line; ++i_line)
        {
            // @TODO: オプション解析

        }

        return true;
    }
};

Main gMain;
