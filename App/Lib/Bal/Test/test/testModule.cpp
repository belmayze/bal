/*!
 * @file   testModule.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// app
#include <test/testModule.h>

#include <test/container/testList.h>

namespace app::test {

// ----------------------------------------------------------------------------

void Module::exec()
{
    {
        List list;
        list.exec();
    }
}

// ----------------------------------------------------------------------------

}
