/*!
 * @file   balSingletonFinalizer.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// c++
#include <array>

// ----------------------------------------------------------------------------
namespace bal {

/*!
 * シングルトンを生成した反対から解放するためのクラスです
 */
class SingletonFinalizer
{
public:
    using DestroyFunc = void(*)();

    /*!
     * 破棄関数を登録します
     * @param[in] func 破棄関数
     */
    static void AddFunc(DestroyFunc func);

    /*!
     * 実際に破棄を行います
     */
    static void Finalize();

private:
    static constexpr int cMaxFinalizerNum = 256;

    static int                                       mIndex;
    static std::array<DestroyFunc, cMaxFinalizerNum> mFunctions;
};

}
