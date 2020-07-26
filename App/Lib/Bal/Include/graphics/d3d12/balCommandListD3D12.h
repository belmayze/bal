/*!
 * @file   balCommandListD3D12.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// d3d12
#include <d3d12.h>
// bal
#include <graphics/balICommandList.h>

// ----------------------------------------------------------------------------
namespace bal::gfx::d3d12 {

class CommandList : public ICommandList
{
public:
    /*!
     * 初期化の処理を記述します
     * @param[in] arg 初期化構造体
     */
    virtual bool initialize(const InitializeArg& arg) override;

    /*!
     * 記録の開始
     */
    virtual void reset() override;

    /*!
     * 記録の終了
     */
    virtual void close() override;

public:
    /*!
     * ビューポートを変更します
     * @param[in] vp ビューポート
     */
    virtual void setViewport(const Viewport& vp) override;

    /*!
     * レンダーバッファをクリアします
     * @param[in] p_render_target レンダーターゲット
     * @param[in] color           クリアカラー
     */
    virtual void clearColor(IRenderTargetColor* p_render_target, const MathColor& color) override;

    /*!
     * デプスバッファをクリアします
     * @param[in] p_render_target レンダーターゲット
     * @param[in] clear_flag      クリアフラグ
     * @param[in] depth           クリアデプス
     * @param[in] stencil         クリアステンシル
     */
    virtual void clearDepthStencil(IRenderTargetDepth* p_render_target, DepthClearFlag clear_flag, float depth, uint32_t stencil) override;

    /*!
     * リソースのバリアを入れます
     * @param[in] p_resource    バリア対象のリソース
     * @param[in] before_status 遷移前のステータス
     * @param[in] after_status  遷移後のステータス
     */
    virtual void resourceBarrier(void* p_resource, int before_status, int after_status) override;

public:
    //! コマンドリスト
    ID3D12GraphicsCommandList* getCommandList() const { return mpCmdList.get(); }

private:
    // Com の deleter
    struct ComDeleter
    {
        void operator()(IUnknown* ptr) { ptr->Release(); }
    };

private:
    std::unique_ptr<ID3D12CommandAllocator, ComDeleter>    mpCmdAllocator;
    std::unique_ptr<ID3D12GraphicsCommandList, ComDeleter> mpCmdList;
};

}
