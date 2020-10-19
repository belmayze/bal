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

    /*!
     * 最小を返す
     * @param[in] x0 数値0
     * @param[in] x1 数値1
     */
    template <typename T>
    static inline T Min(T x0, T x1) { return std::min(x0, x1); }

    /*!
     * 最大を返す
     * @param[in] x0 数値0
     * @param[in] x1 数値1
     */
    template <typename T>
    static inline T Max(T x0, T x1) { return std::max(x0, x1); }

    /*!
     * クランプされた値を返す
     * @param[in] x   数値
     * @param[in] min 最小値
     * @param[in] max 最大値
     */
    template <typename T>
    static inline T Clamp(T x, T min, T max) { return Min(Max(x, min), max); }
}; 

}
