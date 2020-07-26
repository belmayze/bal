/*!
 * @file   balCommandQueueD3D12.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// d3d12
#include <d3d12.h>
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
     * @param[in] p_cmd_list 実行するコマンドリスト
     */
    virtual void execute(const ICommandList* p_cmd_list) override;

    /*!
     * コマンドの実行まで待機します
     */
    virtual void waitExecuted() override;

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
    std::unique_ptr<ID3D12CommandQueue, ComDeleter> mpCmdQueue;
    std::unique_ptr<ID3D12Fence, ComDeleter>        mpFence;
    HANDLE                                          mEventHandle;
    uint64_t                                        mSignalValue = 0;
};

}
