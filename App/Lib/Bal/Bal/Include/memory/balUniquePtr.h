/*!
 * @file   balUniquePtr.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// C++
#include <memory>

// ----------------------------------------------------------------------------
namespace bal {

template <class T, class D = std::default_delete<T>>
using UniquePtr = std::unique_ptr<T, D>;

}
