/*!
 * @file   testTreeMap.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <container/balTreeMap.h>
// app
#include <test/container/testTreeMap.h>

namespace {

class Data
{
public:
    Data(const bal::StringPtr& name) :mName(name) {}
    Data(Data&& data) :mName(data.mName) {}

    void print() const
    {
        BAL_LOG_INFO("Print: %s\n", mName.c_str());
    }

private:
    bal::StringPtr mName;
};

}

namespace app::test {

// ----------------------------------------------------------------------------

void TreeMap::exec()
{
    {
        BAL_LOG_INFO("==== treeMap::push_back ====\n");

        bal::TreeMap<Data> tree_map;
        tree_map.initialize(10);

        auto p_root   = tree_map.addChild(nullptr, Data("Root"));
        auto p_root_1 = tree_map.addChild(p_root, Data("Root - 1"));
        auto p_root_2 = tree_map.addSibling(p_root_1, Data("Root - 2"));
    }
}

// ----------------------------------------------------------------------------

}
