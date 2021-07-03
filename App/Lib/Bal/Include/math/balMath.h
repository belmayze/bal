/*!
 * @file   balMath.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once

// namespace
#if BAL_ARCH_IS_X64
namespace bal { class MathX64; }
namespace bal { class MathX64Vector2; }
namespace bal { class MathX64Vector3; }
namespace bal { class MathX64Vector4; }
namespace bal { class MathX64Matrix44; }
namespace bal { class MathX64Matrix34; }
namespace bal { class MathX64Size; }
namespace bal { class MathX64Color; }

namespace bal { using Math         = MathX64;         }
namespace bal { using MathVector2  = MathX64Vector2;  }
namespace bal { using MathVector3  = MathX64Vector3;  }
namespace bal { using MathVector4  = MathX64Vector4;  }
namespace bal { using MathMatrix44 = MathX64Matrix44; }
namespace bal { using MathMatrix34 = MathX64Matrix34; }
namespace bal { using MathSize     = MathX64Size;     }
namespace bal { using MathColor    = MathX64Color;    }
#else
namespace bal { class MathCommon; }
namespace bal { class MathCommonVector2; }
namespace bal { class MathCommonVector3; }
namespace bal { class MathCommonVector4; }
namespace bal { class MathCommonMatrix44; }
namespace bal { class MathCommonSize; }
namespace bal { class MathCommonColor; }

namespace bal { using Math         = MathCommon;         }
namespace bal { using MathVector2  = MathCommonVector2;  }
namespace bal { using MathVector3  = MathCommonVector3;  }
namespace bal { using MathVector4  = MathCommonVector4;  }
namespace bal { using MathMatrix44 = MathCommonMatrix44; }
namespace bal { using MathMatrix34 = MathCommonMatrix34; }
namespace bal { using MathSize     = MathCommonSize;     }
namespace bal { using MathColor    = MathCommonColor;    }
#endif

// includes
#include <math/balDegree.h>
#include <math/balRadian.h>

#if BAL_ARCH_IS_X64
#   include <math/archX64/balMathX64.h>
#   include <math/archX64/balMathX64Vector.h>
#   include <math/archX64/balMathX64Matrix.h>
#   include <math/archX64/balMathX64Size.h>
#   include <math/archX64/balMathX64Color.h>
#else
#   include <math/balMathCommon.h>
#   include <math/balMathCommonVector.h>
#   include <math/balMathCommonMatrix.h>
#   include <math/balMathCommonSize.h>
#   include <math/balMathCommonColor.h>
#endif // BAL_PLATFORM_IS_WIN64
