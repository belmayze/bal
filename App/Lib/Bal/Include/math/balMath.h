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

namespace bal { using Math = MathWin64; }

#endif // BAL_PLATFORM_IS_WIN64
