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
#include <memory/balUniquePtr.h>

namespace bal { class Framework; }

// ----------------------------------------------------------------------------

namespace bal {

/*!
 * シングルトンを作るクラスです
 */
template <typename T>
class Singleton
{
    friend Framework; // シングルトンを指定したヒープで唯一作れるクラス

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
     * インスタンスを確保するとき、ヒープから確保します
     * @note カレントヒープが存在しない場所で使うことを想定しています
     */
    static T& GetInstance(HeapBase* p_heap)
    {
        if (!mpInstance) { Create_(p_heap); }
        return GetInstance();
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
     * インスタンスを生成します
     */
    static void Create_(HeapBase* p_heap)
    {
        mpInstance = make_unique<T>(p_heap);
        SingletonFinalizer::AddFunc(&Singleton<T>::Destroy_);
    }

    /*!
     * インスタンスを破棄します
     */
    static void Destroy_()
    {
        mpInstance.reset();
    }

    static UniquePtr<T> mpInstance;
};

}
