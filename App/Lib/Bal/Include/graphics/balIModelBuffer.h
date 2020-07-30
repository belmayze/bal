/*!
 * @file   balIModelBuffer.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once

namespace bal::gfx { class IGraphics; }

// ----------------------------------------------------------------------------
namespace bal::gfx {

class IModelBuffer
{
public:
    //! インデックスバッファのフォーマット
    enum class IndexBufferFormat
    {
        Uint16,
        Uint32
    };
    //! 初期化構造体
    struct InitializeArg
    {
        IGraphics*        mpGraphics         = nullptr;
        const uint8_t*    mpVertexBuffer     = nullptr;
        size_t            mVertexBufferSize  = 0;
        size_t            mVertexStride      = 0;
        const uint8_t*    mpIndexBuffer      = nullptr;
        size_t            mIndexBufferSize   = 0;
        IndexBufferFormat mIndexBufferFormat = IndexBufferFormat::Uint16;
    };

public:
    /*!
     * 初期化の処理を記述します
     * @param[in] arg 初期化構造体
     */
    virtual bool initialize(const InitializeArg& arg) = 0;
};

}
