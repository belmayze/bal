/*!
 * @file   testTreeMap.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// std
#include <functional>
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

    const bal::StringPtr& getName() const { return mName; }

private:
    bal::StringPtr mName;
};
class DataLog
{
public:
    DataLog(const bal::StringPtr& name) :mName(name) { BAL_LOG_INFO("DataLog(\"%s\")\n", mName.c_str()); }
    ~DataLog() { BAL_LOG_INFO("~DataLog(\"%s\")\n", mName.c_str()); }

    const bal::StringPtr& getName() const { return mName; }

private:
    bal::StringPtr mName;
};

template <typename T>
void PrintTreeMap(const bal::TreeMap<T>& data)
{
    using Node = typename bal::TreeMap<T>::Node;

    std::function<void(const Node*, int)> func = [&func](const Node* p_node, int indent)
    {
        // 出力
        {
            for (int i = 0; i < indent; ++i) { BAL_LOG_INFO("\t"); }
            BAL_LOG_INFO("%s\n", p_node->getData().getName().c_str());
        }
        if (p_node->getChild())
        {
            func(p_node->getChild(), indent + 1);
        }
        if (p_node->getSibling())
        {
            func(p_node->getSibling(), indent);
        }
    };
    func(data.getRootNode(), 0);
}

}

namespace app::test {

// ----------------------------------------------------------------------------

void TreeMap::exec()
{
    {
        BAL_LOG_INFO("==== treeMap::add ====\n");

        bal::TreeMap<Data> tree_map;
        tree_map.initialize(10);

        {
            auto p_root     = tree_map.addChild(nullptr, Data("Root"));
            auto p_root_1   = tree_map.addChild(p_root, Data("Root - 1"));
            auto p_root_2   = tree_map.addSibling(p_root_1, Data("Root - 2"));
            auto p_root_4   = tree_map.addSibling(p_root_2, Data("Root - 4"));
            auto p_root_3   = tree_map.addSibling(p_root_2, Data("Root - 3"));
            auto p_root_1_1 = tree_map.addChild(p_root_1, Data("Root - 1-1"));
            auto p_root_1_2 = tree_map.addSibling(p_root_1_1, Data("Root - 1-2"));
            auto p_root_3_2 = tree_map.addChild(p_root_3, Data("Root - 3-2"));
            auto p_root_3_1 = tree_map.addChild(p_root_3, Data("Root - 3-1"));
        }

        PrintTreeMap(tree_map);

        tree_map.clear();
        BAL_LOG_INFO("==== treeMap::emplace ====\n");

        bal::TreeMap<DataLog> tree_map_log;
        tree_map_log.initialize(10);

        {
            auto p_root     = tree_map_log.emplaceChild(nullptr, "Root");
            auto p_root_1   = tree_map_log.emplaceChild(p_root, "Root - 1");
            auto p_root_2   = tree_map_log.emplaceSibling(p_root_1, "Root - 2");
            auto p_root_4   = tree_map_log.emplaceSibling(p_root_2, "Root - 4");
            auto p_root_3   = tree_map_log.emplaceSibling(p_root_2, "Root - 3");
            auto p_root_1_1 = tree_map_log.emplaceChild(p_root_1, "Root - 1-1");
            auto p_root_1_2 = tree_map_log.emplaceSibling(p_root_1_1, "Root - 1-2");
            auto p_root_3_2 = tree_map_log.emplaceChild(p_root_3, "Root - 3-2");
            auto p_root_3_1 = tree_map_log.emplaceChild(p_root_3, "Root - 3-1");

            tree_map_log.remove(p_root_1);
        }
        PrintTreeMap(tree_map_log);
    }
}

// ----------------------------------------------------------------------------

}
