/*!
 * @file   balConstantBufferD3D12.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <graphics/balIConstantBuffer.h>

// ----------------------------------------------------------------------------
namespace bal::gfx::d3d12 {

class ConstantBuffer : public IConstantBuffer
{
public:
    /*!
     * 初期化の処理を記述します
     * @param[in] arg 初期化構造体
     */
    virtual bool initialize(const InitializeArg& arg) override;

    /*!
     * GPU メモリーに値を反映する
     */
    virtual void flush() override;

    /*!
     * ダブルバッファだった場合に入れ替える
     */
    virtual void swapBuffer() override;

public:
    //! GPUアドレスを取得する
    D3D12_GPU_VIRTUAL_ADDRESS getGPUVirtualAddress() const;

    //! バッファーサイズを取得する
    size_t getBufferSize() const;

protected:
    /*!
     * 書き込み先のバッファを取得する
     */
    virtual uint8_t* getBufferPtr_() override;

private:
    // Com の deleter
    struct ComDeleter
    {
        void operator()(IUnknown* ptr) { ptr->Release(); }
    };

private:
    std::unique_ptr<std::unique_ptr<ID3D12Resource, ComDeleter>[]> mpResources;
    std::unique_ptr<uint8_t* []>                                   mpBufferPtrs;
    uint32_t                                                       mBufferCount = 1;
    uint32_t                                                       mBufferIndex = 0;
    size_t                                                         mBufferSize  = 0;
};

}
