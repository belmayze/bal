/**
 * @file   Main.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <app/balApplicationBase.h>
#include <app/balOptionParser.h>
#include <framework/balFramework.h>
#include <io/balFile.h>
// app
#include "Archiver.h"

namespace app
{

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
            init_arg.mpApplication = this;
            framework.initialize(api_entry, init_arg);
        }

        framework.enterApplicationLoop(nullptr);

        // オプション検索
        bal::StringPtr input_filepath;
        bal::StringPtr output_filepath;

        for (uint32_t i_option = 0; i_option < api_entry.getNumOption(); ++i_option)
        {
            if (api_entry.getOptions()[i_option].first.isEquals("-archive-list"))
            {
                input_filepath = api_entry.getOptions()[i_option].second;
            }
            else if (api_entry.getOptions()[i_option].first.isEquals("-output"))
            {
                output_filepath = api_entry.getOptions()[i_option].second;
            }
        }

        bool success = archive_(input_filepath, output_filepath);

        return success ? 0 : 1;
    }

private:
    /*!
     * アーカイブ処理を行います
     * @param[in] filepath        アーカイブ対象が含まれているテキストファイル
     * @param[in] output_filepath 出力先のファイルパス
     */
    bool archive_(const bal::StringPtr& filepath, const bal::StringPtr& output_filepath)
    {
        // テキストファイル読み込み
        bal::File arg_file;
        if (!arg_file.loadFromFile(filepath))
        {
            return false;
        }
        bal::StringPtr arg_txt(reinterpret_cast<const char*>(arg_file.getBuffer()), static_cast<int32_t>(arg_file.getBufferSize()));
        auto [num_line, lines] = arg_txt.getLines();

        // アーカイバーの初期化
        Archiver archiver;
        {
            Archiver::InitializeArg init_arg;
            init_arg.mNumShader = num_line;
            archiver.initialize(init_arg);
        }

        for (uint32_t i_line = 0; i_line < num_line; ++i_line)
        {
            // オプション解析
            auto [argc, ptr] = lines[i_line].getArgv();
            std::unique_ptr<const char* []> argv = bal::make_unique<const char* []>(nullptr, argc);
            for (uint32_t i_arg = 0; i_arg < argc; ++i_arg)
            {
                argv[i_arg] = ptr[i_arg].c_str();
            }

            bal::OptionParser parser;
            parser.parse(argc, argv.get());

            // オプションをもとに実行
            // <-vs> 頂点シェーダー
            // <-ps> ピクセルシェーダー
            // <>    シェーダー名
            Archiver::ShaderArg shader_arg;
            shader_arg.mShaderIndex = i_line;

            for (uint32_t i_option = 0; i_option < parser.getNumOption(); ++i_option)
            {
                const std::pair<bal::StringPtr, bal::StringPtr>& option = parser.getOptions()[i_option];

                if (option.first.isEquals("-vs"))
                {
                    shader_arg.mVertexShaderFilepath = option.second;
                }
                else if (option.first.isEquals("-gs"))
                {
                    shader_arg.mGeometryShaderFilepath = option.second;
                }
                else if (option.first.isEquals("-ps"))
                {
                    shader_arg.mPixelShaderFilepath = option.second;
                }
                else if (option.first.isEquals("-cs"))
                {
                    shader_arg.mComputeShaderFilepath = option.second;
                }
                else if (option.first.isEquals("-ds"))
                {
                    shader_arg.mDomainShaderFilepath = option.second;
                }
                else if (option.first.isEquals("-hs"))
                {
                    shader_arg.mHullShaderFilepath = option.second;
                }
                else if (option.first.isEquals("-as"))
                {
                    shader_arg.mAmplificationShaderFilepath = option.second;
                }
                else if (option.first.isEquals("-ms"))
                {
                    shader_arg.mMeshShaderFilepath = option.second;
                }
                else if (option.first.isEmpty())
                {
                    shader_arg.mShaderName = option.second;
                }
            }

            archiver.setShader(shader_arg);
        }

        // アーカイブを行い出力する
        archiver.archive(output_filepath);

        return true;
    }
};

Main gMain;

}
