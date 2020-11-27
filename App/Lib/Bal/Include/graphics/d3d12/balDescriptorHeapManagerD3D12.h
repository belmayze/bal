/*!
 * @file   balDescriptorHeapManagerD3D12.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <memory/balSingleton.h>

// ----------------------------------------------------------------------------
namespace bal::d3d12 {

class DescriptorHeapManager : public Singleton<DescriptorHeapManager>
{
public:
    //! 初期化引数
    struct InitializeArg
    {
        size_t mNumCbvSrvUav = 1000;
        size_t mNumSampler   = 128;
    };

public:
    /*!
     * 初期化します
     * @param[in] arg 引数
     */
    void initialize(const InitializeArg& arg);
};

}
