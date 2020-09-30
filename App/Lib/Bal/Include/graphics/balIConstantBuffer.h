/*!
 * @file   balIConstantBuffer.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once

namespace bal { class IGraphics; }

// ----------------------------------------------------------------------------
namespace bal {

class IConstantBuffer
{
public:
    //! 初期化構造体
    struct InitializeArg
    {
        IGraphics* mpGraphics   = nullptr;
        size_t     mBufferSize  = 0;
        uint32_t   mBufferCount = 1;
    };

public:
    //! デストラクター
    virtual ~IConstantBuffer() {}

    /*!
     * 初期化の処理を記述します
     * @param[in] arg 初期化構造体
     */
    virtual bool initialize(const InitializeArg& arg) = 0;

    /*!
     * GPU メモリーに値を反映する
     */
    virtual void flush() = 0;

    /*!
     * ダブルバッファだった場合に入れ替える
     */
    virtual void swapBuffer() = 0;

    /*!
     * 書き込み先のバッファを取得する
     */
    template <typename T>
    T* getBufferPtr() { return reinterpret_cast<T*>(getBufferPtr_()); }
    
protected:
    /*!
     * 書き込み先のバッファを取得する
     */
    virtual uint8_t* getBufferPtr_() = 0;
};

}
