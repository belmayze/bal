/*!
 * @file   balMemUtil.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <memory/balMemUtil.h>

namespace bal {

// ----------------------------------------------------------------------------

void MemUtil::Copy(void* p_dst, const void* p_src, size_t size)
{
#if BAL_PLATFORM_IS_WIN
    memcpy_s(p_dst, size, p_src, size);
#else
    memcpy(p_dst, p_src, size);
#endif
}

// ----------------------------------------------------------------------------

}
