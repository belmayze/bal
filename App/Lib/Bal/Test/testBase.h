/*!
 * @file   testBase.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <container/balStringPtr.h>

// ----------------------------------------------------------------------------
namespace test {

class TestBase
{
public:
    /*! テスト開始 */
    void exec();

protected:
    /*! コンストラクター */
    TestBase(const bal::StringPtr& name) : mName(name) {}

private:
    const bal::StringPtr mName;

    /*! テスト実態 */
    virtual void exec_() = 0;
};

}
