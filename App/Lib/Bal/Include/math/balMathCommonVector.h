/*!
 * @file   balMathCommonVector.h
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

// ----------------------------------------------------------------------------

class alignas(16) MathCommonVector4
{
public:
    //! コンストラクター
    MathCommonVector4()
    {
        m[0] = 0.f; m[1] = 0.f; m[2] = 0.f; m[3] = 0.f;
    }

protected:
    float m[4];
};

// ----------------------------------------------------------------------------

class MathCommonVector3
{
public:
    //! コンストラクター
    MathCommonVector3()
    {
        m[0] = 1.f; m[1] = 0.f; m[2] = 0.f;
    }
    //! コンストラクター
    explicit MathCommonVector3(float x, float y, float z)
    {
        m[0] = x; m[1] = y; m[2] = z;
    }

    //! getter
    inline float getX() const { return m[0]; }
    inline float getY() const { return m[1]; }
    inline float getZ() const { return m[2]; }

protected:
    float m[3];
}; 

}
