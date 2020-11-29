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

namespace bal { class IGraphics; }

// ----------------------------------------------------------------------------
namespace bal::d3d12 {

class DescriptorHeapManager : public Singleton<DescriptorHeapManager>
{
public:
    //! 初期化引数
    struct InitializeArg
    {
        IGraphics* mpGraphics    = nullptr;
        uint32_t   mNumCbvSrvUav = 1000;
        uint32_t   mNumSampler   = 128;
    };

public:
    /*!
     * 初期化します
     * @param[in] arg 引数
     */
    bool initialize(const InitializeArg& arg);

private:
    // Com の deleter
    struct ComDeleter
    {
        void operator()(IUnknown* ptr) { ptr->Release(); }
    };

private:
    std::unique_ptr<ID3D12DescriptorHeap, ComDeleter> mpCbvSrvUavDescriptorHeap;
    std::unique_ptr<ID3D12DescriptorHeap, ComDeleter> mpSamplerDescriptorHeap;
};

}
