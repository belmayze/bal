/*!
 * @file   balMath.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once

// namespace
#if BAL_PLATFORM_IS_WIN64
namespace bal { class MathWin64; }
namespace bal { class MathWin64Vector2; }
namespace bal { class MathWin64Vector3; }
namespace bal { class MathWin64Vector4; }
namespace bal { class MathWin64Matrix44; }
namespace bal { class MathWin64Size; }

namespace bal { using Math         = MathWin64;         }
namespace bal { using MathVector2  = MathWin64Vector2;  }
namespace bal { using MathVector3  = MathWin64Vector3;  }
namespace bal { using MathVector4  = MathWin64Vector4;  }
namespace bal { using MathMatrix44 = MathWin64Matrix44; }
namespace bal { using MathSize     = MathWin64Size;     }
#else
namespace bal { class MathCommon; }
namespace bal { class MathCommonVector2; }
namespace bal { class MathCommonVector3; }
namespace bal { class MathCommonVector4; }
namespace bal { class MathCommonMatrix44; }
namespace bal { class MathCommonSize; }

namespace bal { using Math         = MathCommon;         }
namespace bal { using MathVector2  = MathCommonVector2;  }
namespace bal { using MathVector3  = MathCommonVector3;  }
namespace bal { using MathVector4  = MathCommonVector4;  }
namespace bal { using MathMatrix44 = MathCommonMatrix44; }
namespace bal { using MathSize     = MathCommonSize;     }
#endif

// includes
#include <math/balDegree.h>
#include <math/balRadian.h>

#if BAL_PLATFORM_IS_WIN64
#   include <math/win64/balMathWin64.h>
#   include <math/win64/balMathWin64Vector.h>
#   include <math/win64/balMathWin64Matrix.h>
#   include <math/win64/balMathWin64Size.h>
#else
#   include <math/balMathCommon.h>
#   include <math/balMathCommonVector.h>
#   include <math/balMathCommonMatrix.h>
#   include <math/balMathCommonSize.h>
#endif // BAL_PLATFORM_IS_WIN64
