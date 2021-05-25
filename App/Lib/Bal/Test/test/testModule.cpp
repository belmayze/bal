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
#include <test/container/testTreeMap.h>

namespace app::test {

// ----------------------------------------------------------------------------

void Module::exec()
{
    // container
    {
        // list
        {
            List v;
            v.exec();
        }
        // treemap
        {
            TreeMap v;
            v.exec();
        }
    }
}

// ----------------------------------------------------------------------------

}
