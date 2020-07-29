/*!
 * @file   balShaderD3D12.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// d3d12
#include <d3d12.h>
// bal
#include <graphics/balIPipeline.h>

// ----------------------------------------------------------------------------
namespace bal::gfx::d3d12 {

class Pipeline : public IPipeline
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
    std::unique_ptr<ID3D12RootSignature, ComDeleter> mpRootSignature;
    std::unique_ptr<ID3D12PipelineState, ComDeleter> mpPipelineState;
};

}
