/*!
 * @file   balApiEntry.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <container/balStringPtr.h>
#include <heap/balHeapRootBlock.h>

namespace bal { class ApplicationBase; }
namespace bal { class Framework; }
namespace bal { class OptionParser; }

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
    int boot(ApplicationBase* p_application, bool enable_window, int argc, const char* argv[]);

public:
    //! 起動モードを取得します
    BootMode getBootMode() const { return mBootArg.mBootMode; }
    //! ルートヒープを取得
    heap::RootBlock* getRootHeap() const { return mpRootHeap.get(); }
    //! オプションの個数を取得します
    uint32_t getNumOption() const;
    //! オプションを取得します
    const std::pair<StringPtr, StringPtr>* getOptions() const;

private:
    std::unique_ptr<OptionParser>                              mpOptionParser; //!< オプションパーサー
    std::unique_ptr<heap::RootBlock, heap::RootBlock::Deleter> mpRootHeap;     //!< ルートヒープ
    BootArg                                                    mBootArg;       //!< 起動引数

private:
    /*!
     * ルートヒープより先に開放すべき処理
     */
    void destroy_();
};

}
