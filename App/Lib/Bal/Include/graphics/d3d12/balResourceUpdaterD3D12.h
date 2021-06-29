/*!
 * @file   balResourceUpdaterD3D12.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <container/balList.h>

namespace bal::d3d12 { class CommandListCopy; }

// ----------------------------------------------------------------------------
namespace bal::d3d12 {

class ResourceUpdater
{
public:
    //! 初期化引数
    struct InitializeArg
    {
        uint32_t   mNumCmdList = 2;       //!< 数が多いほど同時にコピーできます
        IGraphics* mpGraphics  = nullptr;
    };
    //! アップデート引数
    struct UpdateArg
    {
        IGraphics*  mpGraphics    = nullptr;
        void*       mpDstResource = nullptr;
        const void* mpSrcResource = nullptr;
    };

public:
    /*!
     * 初期化
     * @param[in] arg 初期化引数
     */
    void initialize(const InitializeArg& arg);

    /*!
     * リソースをコピーして終了まで待機します
     * @param[in] arg 更新引数
     */
    void updateResource(const UpdateArg& arg);

private:
    //! コンテキスト
    struct Context
    {
        std::unique_ptr<CommandListCopy> mpCommandList;
        ~Context();
    };

private:
    std::unique_ptr<Context[]> mpContexts;
    List<Context*>             mpFreeList;
    std::mutex                 mMutex;
};

}
