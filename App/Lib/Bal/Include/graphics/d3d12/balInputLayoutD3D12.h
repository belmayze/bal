/*!
 * @file   balInputLayoutD3D12.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <graphics/balIInputLayout.h>

// ----------------------------------------------------------------------------
namespace bal::d3d12 {

class InputLayout : public IInputLayout
{
public:
    /*!
     * 初期化の処理を記述します
     * @param[in] arg 初期化構造体
     */
    virtual bool initialize(const InitializeArg& arg) override;

public:
    //! インプットレイアウトの数
    uint32_t getNumInputElementDesc() const { return mNumInputElementDesc; }
    //! インプットレイアウト
    D3D12_INPUT_ELEMENT_DESC* getInputElementDescs() const { return mpInputElementDescs.get(); }

private:
    std::unique_ptr<D3D12_INPUT_ELEMENT_DESC[]> mpInputElementDescs;
    uint32_t                                    mNumInputElementDesc = 0;

private:
    //! フォーマットを変換します
    DXGI_FORMAT convertFormat_(Type type) const;
};

}
