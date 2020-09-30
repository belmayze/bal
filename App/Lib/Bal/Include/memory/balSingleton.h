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
#include <memory/balSingletonGfxFinalizer.h>
#include <memory/balUniquePtr.h>

namespace bal { class ApiEntry; }

// ----------------------------------------------------------------------------

namespace bal {

/*!
 * シングルトンを作るクラスです
 */
template <typename T>
class Singleton
{
    friend ApiEntry; // シングルトンを指定したヒープで唯一作れるクラス

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

    /*!
     * グラフィックス関連の破棄処理を登録しておく
     */
    static void AddGfxFinalizer()
    {
        SingletonGfxFinalizer::AddFunc(&Singleton<T>::Destroy_);
    }

private:
    /*!
     * インスタンスを確保するとき、ヒープから確保します
     * @note カレントヒープが存在しない場所で使うことを想定しています
     */
    static T& GetInstance(heap::BlockBase* p_heap)
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
    static void Create_(heap::BlockBase* p_heap)
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

    static inline std::unique_ptr<T> mpInstance;
};

}
