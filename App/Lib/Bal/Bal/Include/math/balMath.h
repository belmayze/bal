/*!
 * @file   balMath.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// c++
#include <cmath>

// ----------------------------------------------------------------------------
namespace bal {

template <typename T>
class Math
{
public:
    /*!
     * 切り上げ
     * @param[in] a     入力
     * @param[in] times 倍数
     */
    static constexpr int Ceil(T a, T times)
    {
        float t = static_cast<float>(a) / static_cast<float>(times);
        return static_cast<T>(std::ceil(t) * static_cast<float>(times));
    }
}; 

}
