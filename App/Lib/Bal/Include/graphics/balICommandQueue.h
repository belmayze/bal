/*!
 * @file   balICommandQueue.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once

namespace bal::gfx { class ICommandList; }
namespace bal::gfx { class IGraphics; }

// ----------------------------------------------------------------------------
namespace bal::gfx {

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
        IGraphics* mpGraphics = nullptr;
        Type       mType      = Type::Graphics;
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
     * @param[in] p_cmd_list 実行するコマンドリスト
     */
    virtual void execute(const ICommandList* p_cmd_list) = 0;

    /*!
     * コマンドの実行まで待機します
     */
    virtual void waitExecuted() = 0;
};

}
