/*!
 * @file   balIApplication.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <app/balApiEntry.h>

namespace bal
{

// ============================================================================
// アプリケーションの抽象クラス
// 純粋仮想関数はアプリケーション側で必ず用意してください
// ============================================================================
class ApplicationBase
{
public:
    //! コンストラクター
    ApplicationBase();

    //! デストラクター
    virtual ~ApplicationBase();

    /*!
     * bal のブートセットアップ関数です
     * balMain() の前に呼ばれ、起動のセットアップを行います
     * @param[in,out] p_arg ブートの設定を格納する
     */
    virtual void setupBoot(bal::ApiEntry::BootArg* p_arg) = 0;

    /*!
     * bal のエントリーポイントです
     * @param[in] api_entry 起動したときのオプション設定などが入っているクラスです
     */
    virtual int main(const bal::ApiEntry& api_entry) = 0;
};

// 実態
extern bal::ApplicationBase* gpApplication;

}
