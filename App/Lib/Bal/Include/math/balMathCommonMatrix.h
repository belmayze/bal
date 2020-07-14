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

class MathCommonMatrix44
{
public:
    float m[4][4];;

public:
    //! コンストラクター
    MathCommonMatrix44()
    {
        m[0][0] = 1.f; m[0][1] = 0.f; m[0][2] = 0.f; m[0][3] = 0.f;
        m[1][0] = 0.f; m[1][1] = 1.f; m[1][2] = 0.f; m[1][3] = 0.f;
        m[2][0] = 0.f; m[2][1] = 0.f; m[2][2] = 1.f; m[2][3] = 0.f;
        m[3][0] = 0.f; m[3][1] = 0.f; m[3][2] = 0.f; m[3][3] = 1.f;
    }

    /*!
     * 平行移動をセットします
     */
    void setTranslate(float x, float y, float z)
    {
        m[0][3] = x; m[1][3] = y; m[2][3] = z;
    }

    /*!
     * 行列を倒置にします
     */
    void setTranspose()
    {
        float tmp;

        tmp = m[0][1]; m[0][1] = m[1][0]; m[1][0] = tmp;
        tmp = m[0][2]; m[0][2] = m[2][0]; m[2][0] = tmp;
        tmp = m[0][3]; m[0][3] = m[3][0]; m[3][0] = tmp;

        tmp = m[1][2]; m[1][2] = m[2][1]; m[2][1] = tmp;
        tmp = m[1][3]; m[1][3] = m[3][1]; m[3][1] = tmp;

        tmp = m[2][3]; m[2][3] = m[3][2]; m[3][2] = tmp;
    }

}; 

}
