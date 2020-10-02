/*!
 * @file   balIShapeBuffer.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once

namespace bal { class IGraphics; }

// ----------------------------------------------------------------------------
namespace bal {

class IShapeBuffer
{
public:
    //! インデックスバッファのフォーマット
    enum class IndexBufferFormat
    {
        Uint16,
        Uint32
    };
    //! プリミティブ形状
    enum class PrimitiveTopology
    {
        Triangles
    };
    //! 初期化構造体
    struct InitializeArg
    {
        IGraphics*        mpGraphics         = nullptr;
        const void*       mpVertexBuffer     = nullptr;
        size_t            mVertexBufferSize  = 0;
        size_t            mVertexStride      = 0;
        const void*       mpIndexBuffer      = nullptr;
        size_t            mIndexBufferSize   = 0;
        uint32_t          mIndexCount        = 0;
        IndexBufferFormat mIndexBufferFormat = IndexBufferFormat::Uint16;
        PrimitiveTopology mPrimitiveTopology = PrimitiveTopology::Triangles;
    };

public:
    //! デストラクター
    virtual ~IShapeBuffer() {}

    /*!
     * 初期化の処理を記述します
     * @param[in] arg 初期化構造体
     */
    virtual bool initialize(const InitializeArg& arg) = 0;
};

}
