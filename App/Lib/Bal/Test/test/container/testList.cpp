/*!
 * @file   testList.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2021 belmayze. All rights reserved.
 */
// bal
#include <container/balList.h>
// app
#include <test/container/testList.h>

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

void List::exec()
{
    {
        BAL_LOG_INFO("==== list::push_back ====\n");

        bal::List<Data> list;
        list.initialize(10);

        list.pushBack(Data("1"));
        list.pushBack(Data("2"));
        list.pushBack(Data("3"));
        list.pushBack(Data("4"));
        list.pushBack(Data("5"));
        list.pushBack(Data("6"));
        list.pushBack(Data("7"));
        list.pushBack(Data("8"));
        list.pushBack(Data("9"));
        list.pushBack(Data("10"));

        for (const Data& data : list) { data.print(); }

        list.clear();
        BAL_LOG_INFO("==== list::push_front ====\n");

        list.pushFront(Data("1"));
        list.pushFront(Data("2"));
        list.pushFront(Data("3"));
        list.pushFront(Data("4"));
        list.pushFront(Data("5"));
        list.pushFront(Data("6"));
        list.pushFront(Data("7"));
        list.pushFront(Data("8"));
        list.pushFront(Data("9"));
        list.pushFront(Data("10"));

        for (const Data& data : list) { data.print(); }

        list.clear();
        BAL_LOG_INFO("==== list::pop_back ====\n");

        list.pushBack(Data("1"));
        list.pushBack(Data("2"));
        list.pushBack(Data("3"));
        list.pushBack(Data("4"));
        list.pushBack(Data("5"));
        list.pushBack(Data("6"));
        list.pushBack(Data("7"));
        list.pushBack(Data("8"));
        list.pushBack(Data("9"));
        list.pushBack(Data("10"));

        list.popBack();
        list.popBack();
        list.popBack();
        list.popBack();
        list.popBack();

        for (const Data& data : list) { data.print(); }

        list.clear();
        BAL_LOG_INFO("==== list::pop_front ====\n");

        list.pushBack(Data("1"));
        list.pushBack(Data("2"));
        list.pushBack(Data("3"));
        list.pushBack(Data("4"));
        list.pushBack(Data("5"));
        list.pushBack(Data("6"));
        list.pushBack(Data("7"));
        list.pushBack(Data("8"));
        list.pushBack(Data("9"));
        list.pushBack(Data("10"));

        list.popFront();
        list.popFront();
        list.popFront();
        list.popFront();
        list.popFront();

        for (const Data& data : list) { data.print(); }

        list.clear();
        BAL_LOG_INFO("==== list::access ====\n");

        list.pushBack(Data("1"));
        list.pushBack(Data("2"));
        list.pushBack(Data("3"));
        list.pushBack(Data("4"));
        list.pushBack(Data("5"));
        list.pushBack(Data("6"));
        list.pushBack(Data("7"));
        list.pushBack(Data("8"));
        list.pushBack(Data("9"));
        list.pushBack(Data("10"));

        list.front().print();
        list.back().print();

        list.clear();
        BAL_LOG_INFO("==== list::emplace ====\n");

        list.emplaceBack("6");
        list.emplaceBack("7");
        list.emplaceBack("8");
        list.emplaceBack("9");
        list.emplaceBack("10");
        list.emplaceFront("5");
        list.emplaceFront("4");
        list.emplaceFront("3");
        list.emplaceFront("2");
        list.emplaceFront("1");

        for (const Data& data : list) { data.print(); }
    }
}

// ----------------------------------------------------------------------------

}
