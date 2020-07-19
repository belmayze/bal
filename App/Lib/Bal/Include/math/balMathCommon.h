/*!
 * @file   balMathCommon.h
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

class MathCommon
{
public:
    //! π
    static constexpr float Pi() { return 3.14159265358979323f; }

public:
    /*!
     * 平方根
     * @param[in] x 数値
     */
    static inline float Sqrt(float x) { return std::sqrt(x); }

    /*!
     * 正弦
     * @param[in] x 数値
     */
    static inline float Sin(const Radian& x) { return std::sin(x); }

    /*!
     * 余弦
     * @param[in] x 数値
     */
    static inline float Cos(const Radian& x) { return std::cos(x); }

    /*!
     * 正接
     * @param[in] x 数値
     */
    static inline float Tan(const Radian& x) { return std::tan(x); }

    /*!
     * 切り上げ
     * @param[in] a     入力
     * @param[in] times 倍数
     */
    static inline int Ceil(float a, float times)
    {
        float t = a / times;
        return static_cast<int>(std::ceil(t) * times);
    }
    static inline int Ceil(int a, int times)
    {
        float t = static_cast<float>(a) / static_cast<float>(times);
        return static_cast<int>(std::ceil(t) * times);
    }
    static inline size_t Ceil(size_t a, size_t times)
    {
        double t = static_cast<double>(a) / static_cast<double>(times);
        return static_cast<size_t>(std::ceil(t) * times);
    }
}; 

}
