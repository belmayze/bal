/*!
 * @file   balShaderD3D12.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <graphics/balIPipeline.h>

// ----------------------------------------------------------------------------
namespace bal::d3d12 {

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

public:
    //! ルートシグネチャ
    inline ID3D12RootSignature* getRootSignature() const { return mpRootSignature.get(); }
    //! パイプラインステート
    inline ID3D12PipelineState* getPipelineState() const { return mpPipelineState.get(); }

private:
    std::unique_ptr<ID3D12RootSignature, ComDeleter> mpRootSignature;
    std::unique_ptr<ID3D12PipelineState, ComDeleter> mpPipelineState;
};

}
