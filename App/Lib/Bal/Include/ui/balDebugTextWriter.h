/*!
 * @file   balDebugTextWriter.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2021 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <graphics/balITexture.h>
#include <math/balMath.h>

namespace bal { class StringBase; }
namespace bal { class ICommandListDirect; }

// ----------------------------------------------------------------------------
namespace bal::ui {

// @TODO: 非デバッグ時には空クラスにする
class TextWriter
{
public:
    //! 初期化引数
    struct InitializeArg
    {
        IGraphics*       mpGraphics = nullptr;
        ITexture::Format mOutputFormat;
        uint32_t         mMaxText   = 1024;
    };

private:
    /*!
     * 初期化
     * @param[in] arg 初期化引数
     */
    void initialize(const InitializeArg& arg);

    /*!
     * 文字列セットアップ
     * @param[in] str 文字列
     */
    void setupText(const StringBase& str);

    /*!
     * 描画
     * @param[in] cmd_list コマンドリスト
     */
    void draw(ICommandListDirect& cmd_list) const;

private:
    //! 定数バッファ
    struct CBInstanceData
    {
        MathVector2 mOffset;
        MathVector2 mTexcoord;
    };
    struct CBData
    {
        MathVector2 mBaseMatrix0;
        MathVector2 mBaseMatrix1;
        MathVector2 mBaseTranslate;
        MathVector2 mInvTexcoordScale;
        CBInstanceData mInstanceData[1024];
    };

private:
    std::unique_ptr<bal::IPipeline>       mpPipeline;
    std::unique_ptr<bal::IDescriptorHeap> mpDescriptorHeap;
    std::unique_ptr<bal::IConstantBuffer> mpConstantBuffer;
    std::unique_ptr<bal::ITexture>        mpTexture;
};

}
