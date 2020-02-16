/*!
 * @file   testHeap.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// test
#include "testBase.h"

// ----------------------------------------------------------------------------
namespace test {

class TestHeap : public TestBase
{
public:
    TestHeap(const bal::StringPtr& name) : TestBase(name) {}
    virtual void exec_() override;
};

}
