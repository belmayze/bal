/*!
 * @file   balDescriptorHeapManagerD3D12.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <graphics/d3d12/balDescriptorHeapManagerD3D12.h>

namespace bal::d3d12 {

// ----------------------------------------------------------------------------

void DescriptorHeapManager::initialize(const InitializeArg& arg)
{
    // デスクリプターヒープは同時に複数のヒープを設定することができない
    // また、ヒープの切り替え処理は重いため1フレーム中に何度も呼ぶべきではない
    //
    // そのため大量のヒープをあらかじめ確保しておいて、それをそのまま使用することにします
    // 実際には DescriptorTable でオフセットを覚えておき設定します
}

// ----------------------------------------------------------------------------

}
