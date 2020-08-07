/*!
 * @file   balCommandQueueD3D12.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <graphics/balICommandQueue.h>

// ----------------------------------------------------------------------------
namespace bal::gfx::d3d12 {

class CommandQueue : public ICommandQueue
{
public:
    /*!
     * 初期化の処理を記述します
     * @param[in] arg 初期化構造体
     */
    virtual bool initialize(const InitializeArg& arg) override;

    /*!
     * コマンドリストを実行します
     * @param[in] cmd_list     実行するコマンドリスト
     * @param[in] buffer_index バッファー番号
     */
    virtual void execute(const ICommandListDirect& cmd_list, uint32_t buffer_index) override;

    /*!
     * コマンドの実行まで待機します
     * @param[in] buffer_index バッファー番号
     */
    virtual void waitExecuted(uint32_t buffer_index) override;

    /*!
     * すべてのコマンド実行を待機します
     */
    virtual void waitExecutedAll() override;

public:
    //! コマンドキュー
    ID3D12CommandQueue* getCommandQueue() const { return mpCmdQueue.get(); }

private:
    // Com の deleter
    struct ComDeleter
    {
        void operator()(IUnknown* ptr) { ptr->Release(); }
    };

private:
    std::unique_ptr<ID3D12CommandQueue, ComDeleter>             mpCmdQueue;
    std::unique_ptr<std::unique_ptr<ID3D12Fence, ComDeleter>[]> mpFences;
    uint32_t                                                    mFenceNum = 1;
};

}
