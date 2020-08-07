/*!
 * @file   balApiEntry.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// std
#include <memory>
// bal
#include <container/balStringPtr.h>
#include <heap/balHeapRootBlock.h>

namespace bal { class ApplicationBase; }
namespace bal { class Framework; }

namespace bal
{

class ApiEntry
{
public:
    //! 起動モード
    enum class BootMode
    {
        Console,
        Application
    };

    //! 起動引数
    struct BootArg
    {
        BootMode mBootMode     = BootMode::Console; //!< 起動モード
        uint32_t mRootHeapSize = 1024 * 1024;       //!< ヒープサイズ
    };

    /*!
     * デストラクター
     */
    ~ApiEntry();

    /*!
     * ブート開始
     * @param[in] p_application アプリケーション
     * @param[in] enable_window ウィンドウ生成フラグ
     * @param[in] argc          オプションの数
     * @param[in] argv          オプションのリスト
     */
    int boot(ApplicationBase* p_application, bool enable_window, int argc, char* argv[]);

public:
    //! 起動モードを取得します
    BootMode getBootMode() const { return mBootArg.mBootMode; }
    //! ルートヒープを取得
    heap::RootBlock* getRootHeap() const { return mpRootHeap.get(); }
    //! オプションの個数を取得します
    uint32_t getNumOption() const { return mNumOption; }
    //! オプションを取得します
    const std::pair<StringPtr, StringPtr>* getOptions() const { return mpOptions.get(); }

private:
    std::unique_ptr<std::pair<StringPtr, StringPtr>[]>         mpOptions;      //!< 起動オプション
    uint32_t                                                   mNumOption = 0; //!< オプションの数
    std::unique_ptr<heap::RootBlock, heap::RootBlock::Deleter> mpRootHeap;     //!< ルートヒープ
    BootArg                                                    mBootArg;       //!< 起動引数

private:
    /*!
     * 起動オプションを解析します
     * @param[in] argc オプションの数
     * @param[in] argv オプションのリスト
     */
    void setupOptions_(int argc, char* argv[]);

    /*!
     * ルートヒープより先に開放すべき処理
     */
    void destroy_();
};

}
