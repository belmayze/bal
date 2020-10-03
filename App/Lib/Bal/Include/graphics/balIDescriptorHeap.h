/*!
 * @file   balIDescriptorHeap.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once

namespace bal { class IGraphics; }
namespace bal { class ITexture; }
namespace bal { class IConstantBuffer; }

// ----------------------------------------------------------------------------
namespace bal {

class IDescriptorHeap
{
public:
    //! 初期化構造体
    struct InitializeArg
    {
        IGraphics*              mpGraphics         = nullptr;
        uint32_t                mTextureRangeBase  = 0;
        uint32_t                mNumTexture        = 0;
        const ITexture**        mpTextures         = nullptr;
        uint32_t                mConstantRangeBase = 0;
        uint32_t                mNumConstantBuffer = 0;
        const IConstantBuffer** mpConstantBuffers  = nullptr;
    };

public:
    //! デストラクター
    virtual ~IDescriptorHeap() {}

    /*!
     * 初期化の処理を記述します
     * @param[in] arg 初期化構造体
     */
    virtual bool initialize(const InitializeArg& arg) = 0;

    /*!
     * テクスチャーのレンジベース
     */
    virtual uint32_t getTextureRangeBase() const = 0;

    /*!
     * テクスチャー枚数
     */
    virtual uint32_t getNumTexture() const = 0;

    /*!
     * 定数バッファのレンジベース
     */
    virtual uint32_t getConstantRangeBase() const = 0;

    /*!
     * 定数バッファ数
     */
    virtual uint32_t getNumConstantBuffer() const = 0;
};

}
