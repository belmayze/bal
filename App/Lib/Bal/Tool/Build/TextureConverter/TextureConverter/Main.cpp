/**
 * @file   Main.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// Windows
#include <wincodec.h>
// bal
#include <app/balApplicationBase.h>
#include <app/balOptionParser.h>
#include <framework/balFramework.h>
#include <io/balFile.h>
// DirectXTex
#include <DirectXTex.h>

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
        bal::StringPtr format_str;

        for (uint32_t i_option = 0; i_option < api_entry.getNumOption(); ++i_option)
        {
            if (api_entry.getOptions()[i_option].first.isEquals("-input"))
            {
                input_filepath = api_entry.getOptions()[i_option].second;
            }
            else if (api_entry.getOptions()[i_option].first.isEquals("-output"))
            {
                output_filepath = api_entry.getOptions()[i_option].second;
            }
            else if (api_entry.getOptions()[i_option].first.isEquals("-format"))
            {
                format_str = api_entry.getOptions()[i_option].second;
            }
        }

        if (input_filepath.isEmpty() || output_filepath.isEmpty() || format_str.isEmpty())
        {
            return 1;
        }

        // オプションチェック
        DXGI_FORMAT compress_format;

        // BC1: 4bpp（3チャンネル + 1ビット）
        if      (format_str.isEquals("bc1_srgb"))  { compress_format = DXGI_FORMAT_BC1_UNORM_SRGB; }
        else if (format_str.isEquals("bc1_unorm")) { compress_format = DXGI_FORMAT_BC1_UNORM; }
        // BC2: 8bpp（3チャンネル + 非圧縮16ビット）
        else if (format_str.isEquals("bc2_srgb"))  { compress_format = DXGI_FORMAT_BC2_UNORM_SRGB; }
        else if (format_str.isEquals("bc2_unorm")) { compress_format = DXGI_FORMAT_BC2_UNORM; }
        // BC3: 8bpp（4チャンネル）
        else if (format_str.isEquals("bc3_srgb"))  { compress_format = DXGI_FORMAT_BC3_UNORM_SRGB; }
        else if (format_str.isEquals("bc3_unorm")) { compress_format = DXGI_FORMAT_BC3_UNORM; }
        // BC4: 4bpp（1チャンネル）
        else if (format_str.isEquals("bc4_unorm")) { compress_format = DXGI_FORMAT_BC4_UNORM; }
        else if (format_str.isEquals("bc4_snorm")) { compress_format = DXGI_FORMAT_BC4_SNORM; }
        // BC5: 8bpp（2チャンネル）
        else if (format_str.isEquals("bc5_unorm")) { compress_format = DXGI_FORMAT_BC5_UNORM; }
        else if (format_str.isEquals("bc5_snorm")) { compress_format = DXGI_FORMAT_BC5_SNORM; }
        // BC6: 8bpp（3チャンネル HDR）
        else if (format_str.isEquals("bc6h_sf"))   { compress_format = DXGI_FORMAT_BC6H_SF16; }
        else if (format_str.isEquals("bc6h_uf"))   { compress_format = DXGI_FORMAT_BC6H_UF16; }
        // BC7: 8bpp（4チャンネル） -> BC3の後継
        else if (format_str.isEquals("bc7_srgb"))  { compress_format = DXGI_FORMAT_BC7_UNORM_SRGB; }
        else if (format_str.isEquals("bc7_unorm")) { compress_format = DXGI_FORMAT_BC7_UNORM; }
        // float
        else if (format_str.isEquals("r32g32b32a32_float")) { compress_format = DXGI_FORMAT_R32G32B32A32_FLOAT; }
        else if (format_str.isEquals("r32g32b32_float"))    { compress_format = DXGI_FORMAT_R32G32B32_FLOAT; }
        else if (format_str.isEquals("r32g32_float"))       { compress_format = DXGI_FORMAT_R32G32_FLOAT; }
        else if (format_str.isEquals("r32_float"))          { compress_format = DXGI_FORMAT_R32_FLOAT; }
        else if (format_str.isEquals("r16g16b16a16_float")) { compress_format = DXGI_FORMAT_R16G16B16A16_FLOAT; }
        else if (format_str.isEquals("r16g16_float"))       { compress_format = DXGI_FORMAT_R16G16_FLOAT; }
        else if (format_str.isEquals("r11g11b10_float"))    { compress_format = DXGI_FORMAT_R11G11B10_FLOAT; }
        // RGBA
        else if (format_str.isEquals("r8g8b8a8_unorm")) { compress_format = DXGI_FORMAT_R8G8B8A8_UNORM; }
        else if (format_str.isEquals("r8g8b8a8_srgb"))  { compress_format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; }
        // BGRA
        else if (format_str.isEquals("b5g5r5a1_unorm")) { compress_format = DXGI_FORMAT_B5G5R5A1_UNORM; }
        else if (format_str.isEquals("b4g4r4a4_unorm")) { compress_format = DXGI_FORMAT_B4G4R4A4_UNORM; }
        // RGB
        else if (format_str.isEquals("r5g6b5_unorm")) { compress_format = DXGI_FORMAT_B5G6R5_UNORM; }
        // RG
        else if (format_str.isEquals("r8g8_snorm")) { compress_format = DXGI_FORMAT_R8G8_SNORM; }
        else if (format_str.isEquals("r8g8_unorm")) { compress_format = DXGI_FORMAT_R8G8_UNORM; }
        // R
        else if (format_str.isEquals("r8_snorm")) { compress_format = DXGI_FORMAT_R8_SNORM; }
        else if (format_str.isEquals("r8_unorm")) { compress_format = DXGI_FORMAT_R8_UNORM; }

        bool success = convert_(input_filepath, output_filepath, compress_format);

        return success ? 0 : 1;
    }

private:
    /*!
     * コンバート処理を行います
     * @param[in] input_filepath  変換するテクスチャーファイルパス
     * @param[in] output_filepath 出力先のファイルパス
     * @param[in] format          フォーマット
     */
    bool convert_(const bal::StringPtr& input_filepath, const bal::StringPtr& output_filepath, DXGI_FORMAT format)
    {
        HRESULT hr = S_OK;

        // COMコンポーネントの初期化
        CoInitialize(nullptr);

        // WIC Interface 設定
        IWICImagingFactory* p_factory = nullptr;
        hr = CoCreateInstance(
            CLSID_WICImagingFactory,
            nullptr,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&p_factory)
        );
        if (FAILED(hr))
        {
            return false;
        }
        DirectX::SetWICFactory(p_factory);

        // 文字列置換
        std::unique_ptr<WCHAR[]> p_input_filepath = bal::make_unique<WCHAR[]>(nullptr, MAX_PATH);
        size_t ret;
        mbstowcs_s(&ret, p_input_filepath.get(), MAX_PATH, input_filepath.c_str(), input_filepath.size());

        std::unique_ptr<WCHAR[]> p_output_filepath = bal::make_unique<WCHAR[]>(nullptr, MAX_PATH);
        mbstowcs_s(&ret, p_output_filepath.get(), MAX_PATH, output_filepath.c_str(), output_filepath.size());

        // ファイル読み込み
        DirectX::ScratchImage raw_image;
        hr = DirectX::LoadFromWICFile(
            p_input_filepath.get(),
            DirectX::WIC_FLAGS_NONE,
            nullptr,
            raw_image
        );
        if (FAILED(hr))
        {
            BAL_LOG_ERROR("ファイルが読み込めませんでした. [%s]", input_filepath.c_str());
            return false;
        }

        // 同じフォーマットならそのまま書き出す
        if (raw_image.GetImages()->format == format)
        {
            // 書き出し
            hr = DirectX::SaveToDDSFile(
                raw_image.GetImages(),
                raw_image.GetImageCount(),
                raw_image.GetMetadata(),
                DirectX::DDS_FLAGS_NONE,
                p_output_filepath.get()
            );
            if (FAILED(hr))
            {
                return false;
            }
        }
        else
        {
            // 圧縮処理
            DirectX::ScratchImage converted_image;
            if (DirectX::IsCompressed(format))
            {
                hr = DirectX::Compress(
                    raw_image.GetImages(),
                    raw_image.GetImageCount(),
                    raw_image.GetMetadata(),
                    format,
                    DirectX::TEX_COMPRESS_DEFAULT,
                    0.f,
                    converted_image
                );
                if (FAILED(hr))
                {
                    return false;
                }
            }
            else
            {
                hr = DirectX::Convert(
                    *raw_image.GetImages(),
                    format,
                    DirectX::TEX_FILTER_CUBIC,
                    0.f,
                    converted_image
                );
                if (FAILED(hr))
                {
                    return false;
                }
            }

            // 書き出し
            hr = DirectX::SaveToDDSFile(
                converted_image.GetImages(),
                converted_image.GetImageCount(),
                converted_image.GetMetadata(),
                DirectX::DDS_FLAGS_NONE,
                p_output_filepath.get()
            );
            if (FAILED(hr))
            {
                return false;
            }
        }

        DirectX::SetWICFactory(nullptr);
        p_factory->Release();

        // COMコンポーネントの終了
        CoUninitialize();

        return true;
    }
};

Main gMain;

}
