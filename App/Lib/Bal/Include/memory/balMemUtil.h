/*!
 * @file   balMemUtil.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once

// ----------------------------------------------------------------------------
namespace bal {

/*!
 * メモリー操作ユーティリティ
 */
class MemUtil
{
public:
    /*! memcpy */
    static void Copy(void* p_dst, const void* p_src, std::size_t size);
};

}
