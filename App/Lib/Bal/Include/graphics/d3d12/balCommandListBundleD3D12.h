/*!
 * @file   balCommandListBundleD3D12.h
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
#include <graphics/balICommandListBundle.h>

// ----------------------------------------------------------------------------
namespace bal::gfx::d3d12 {

class CommandListBundle : public ICommandListBundle, public ICommandList
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
