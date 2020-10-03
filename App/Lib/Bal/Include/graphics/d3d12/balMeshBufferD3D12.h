/*!
 * @file   balMeshBufferD3D12.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <graphics/balIMeshBuffer.h>

// ----------------------------------------------------------------------------
namespace bal::d3d12 {

class MeshBuffer : public IMeshBuffer
{
public:
    /*!
     * 初期化の処理を記述します
     * @param[in] arg 初期化構造体
     */
    virtual bool initialize(const InitializeArg& arg) override;

public:
    //! 頂点バッファビュー
    inline const D3D12_VERTEX_BUFFER_VIEW* getVertexBufferView() const { return &mVertexBufferView; }
    //! インデックスバッファビュー
    inline const D3D12_INDEX_BUFFER_VIEW* getIndexBufferView() const { return &mIndexBufferView; }
    //! プリミティブ形状
    inline PrimitiveTopology getPrimitiveTopology() const { return mPrimitiveTopology; }
    //! インデックス数
    inline uint32_t getIndexCount() const { return mIndexCount; }

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
    uint32_t                                    mIndexCount        = 0;
    PrimitiveTopology                           mPrimitiveTopology = PrimitiveTopology::Triangles;
};

}
