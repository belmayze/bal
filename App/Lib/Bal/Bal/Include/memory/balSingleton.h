/*!
 * @file   balSingleton.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <memory/balSingletonFinalizer.h>

// ----------------------------------------------------------------------------

namespace bal {

/*!
 * シングルトンを作るクラスです
 */
template <typename T>
class Singleton
{
public:
    /*!
     * インスタンスを取得します
     * @note インスタンスが存在しない場合は assert で止まります
     */
    static T& GetInstance()
    {
        if (!mpInstance) { Create_(); }
        BAL_ASSERT(mpInstance);
        return *mpInstance.get();
    }

private:
    /*!
     * インスタンスを生成します
     */
    static void Create_()
    {
        mpInstance = std::make_unique<T>();
        SingletonFinalizer::AddFunc(&Singleton<T>::Destroy_);
    }

    /*!
     * インスタンスを破棄します
     */
    static void Destroy_()
    {
        mpInstance.reset();
    }

    static std::unique_ptr<T> mpInstance;
};

}
