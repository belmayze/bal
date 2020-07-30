/*!
 * @file   balIModelBuffer.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <graphics/balIModelBuffer.h>

// ----------------------------------------------------------------------------
namespace bal::gfx::d3d12 {

class ModelBuffer : public IModelBuffer
{
public:
    /*!
     * 初期化の処理を記述します
     * @param[in] arg 初期化構造体
     */
    virtual bool initialize(const InitializeArg& arg) override;

private:
    // Com の deleter
    struct ComDeleter
    {
        void operator()(IUnknown* ptr) { ptr->Release(); }
    };

private:
    D3D12_VERTEX_BUFFER_VIEW                    mVertexBufferView;
    D3D12_INDEX_BUFFER_VIEW                     mIndexBufferView;
    std::unique_ptr<ID3D12Resource, ComDeleter> mpVertexBuffer;
    std::unique_ptr<ID3D12Resource, ComDeleter> mpIndexBuffer;
};

}
