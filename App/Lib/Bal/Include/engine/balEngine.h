/*!
 * @file   balEngine.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <framework/balFrameworkCallback.h>
#include <math/balMath.h>
#include <memory/balSingleton.h>

namespace bal { class IGraphics; }
namespace bal::mod { class IModule; }
namespace bal::mod::gfx { class Module; }

// ----------------------------------------------------------------------------
namespace bal {

/*!
 * RE ENGINE の思考ベースに構築
 * Engine はベースとし、Module を追加していく方式です
 * 
 * Engine が主に Module をマルチスレッドで動作させます
 * 依存関係を張ることで、update の順序を制御できます
 * 
 * Engine ┬ Module0 ─ Module1
 *        ├ Module2 ─ Module3
 *        └ Module4 ─ Module5
 * 
 * @ref: https://cgworld.jp/feature/201910-re2019-01.html
 */
class Engine : public FrameworkCallback, public Singleton<Engine>
{
public:
    //! 初期化構造体
    struct InitializeArg
    {
        IGraphics* mpGraphics = nullptr;
        MathSize   mRenderBufferSize;
    };
    //! モジュールリスト
    using ModuleArray = std::unique_ptr<std::unique_ptr<mod::IModule>[]>;

public:
    /*! コンストラクター */
    Engine();

    /*! デストラクター */
    virtual ~Engine();

    /*!
     * エンジンの初期化
     * @param[in] arg        引数構造体
     * @param[in] p_modules  モジュールリスト
     * @param[in] num_module モジュールの数
     */
    bool initialize(const InitializeArg& arg, ModuleArray&& p_modules, size_t num_module);

public:
    /*!
     * 計算処理するコールバック
     */
    virtual void onUpdate(const UpdateArg& arg) override;

    /*!
     * 描画処理するコールバック
     */
    virtual void onDraw(const DrawArg& arg) override;

public:
    //! グラフィックスシステム取得
    IGraphics* getGraphicsSystem() const { return mpGraphicsSystem; }

private:
    ModuleArray       mpModules;
    mod::gfx::Module* mpGraphicsModule = nullptr;
    IGraphics*        mpGraphicsSystem = nullptr;
    size_t            mNumModule = 0;
};

}
