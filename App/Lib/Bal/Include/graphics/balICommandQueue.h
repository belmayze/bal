/*!
 * @file   balICommandQueue.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once

namespace bal { class ICommandListDirect; }
namespace bal { class IGraphics; }

// ----------------------------------------------------------------------------
namespace bal {

class ICommandQueue
{
public:
    // 種類
    enum class Type
    {
        Graphics, //!< グラフィックス用
        Compute   //!< 非同期コンピュート用
    };

    // 初期化構造体
    struct InitializeArg
    {
        IGraphics* mpGraphics   = nullptr;
        Type       mType        = Type::Graphics;
        uint32_t   mBufferCount = 1;
    };

public:
    //! デストラクター
    virtual ~ICommandQueue() {}

    /*!
     * 初期化の処理を記述します
     * @param[in] arg 初期化構造体
     */
    virtual bool initialize(const InitializeArg& arg) = 0;

    /*!
     * コマンドリストを実行します
     * @param[in] cmd_list     実行するコマンドリスト
     * @param[in] buffer_index バッファー番号
     */
    virtual void execute(const ICommandListDirect& cmd_list, uint32_t buffer_index) = 0;

    /*!
     * コマンドの実行まで待機します
     * @param[in] buffer_index バッファー番号
     */
    virtual void waitExecuted(uint32_t buffer_index) = 0;

    /*!
     * すべてのコマンド実行を待機します
     */
    virtual void waitExecutedAll() = 0;
};

}
