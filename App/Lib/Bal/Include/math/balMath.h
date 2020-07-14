/*!
 * @file   balMath.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once

#if BAL_PLATFORM_IS_WIN64

#   include <math/win64/balMathWin64.h>
#   include <math/win64/balMathWin64Matrix.h>
#   include <math/win64/balMathWin64Vector.h>

namespace bal { using Math         = MathWin64;         }
namespace bal { using MathMatrix44 = MathWin64Matrix44; }
namespace bal { using MathVector4  = MathWin64Vector4;  }
namespace bal { using MathVector3  = MathWin64Vector3;  }

#endif // BAL_PLATFORM_IS_WIN64
