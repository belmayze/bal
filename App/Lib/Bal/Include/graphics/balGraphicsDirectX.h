/*!
 * @file   balGraphicsDirectX.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// d3d12
#include <d3d12.h>
#include <dxgi1_6.h>
// bal
#include <graphics/balIGraphics.h>

// ----------------------------------------------------------------------------
namespace bal::gfx {

class DirectX : public IGraphics
{
public:
    /*!
     * 初期化の処理を記述します
     * @param[in] arg 初期化構造体
     */
    virtual bool initialize(const InitializeArg& arg) override;

    /*!
     * 破棄の処理を記述します
     */
    virtual bool destroy() override;

private:
    // Com の deleter
    template <class T>
    struct ComDeleter
    {
        void operator()(T* ptr) { ptr->Release(); }
    };

private:
};

}
