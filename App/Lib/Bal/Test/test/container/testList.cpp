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

        list.push_back(Data("1"));
        list.push_back(Data("2"));
        list.push_back(Data("3"));
        list.push_back(Data("4"));
        list.push_back(Data("5"));
        list.push_back(Data("6"));
        list.push_back(Data("7"));
        list.push_back(Data("8"));
        list.push_back(Data("9"));
        list.push_back(Data("10"));

        for (const Data& data : list) { data.print(); }

        BAL_LOG_INFO("==== list::push_front ====\n");
        list.clear();

        list.push_front(Data("1"));
        list.push_front(Data("2"));
        list.push_front(Data("3"));
        list.push_front(Data("4"));
        list.push_front(Data("5"));
        list.push_front(Data("6"));
        list.push_front(Data("7"));
        list.push_front(Data("8"));
        list.push_front(Data("9"));
        list.push_front(Data("10"));

        for (const Data& data : list) { data.print(); }

        BAL_LOG_INFO("==== list::pop_back ====\n");
        list.clear();

        list.push_back(Data("1"));
        list.push_back(Data("2"));
        list.push_back(Data("3"));
        list.push_back(Data("4"));
        list.push_back(Data("5"));
        list.push_back(Data("6"));
        list.push_back(Data("7"));
        list.push_back(Data("8"));
        list.push_back(Data("9"));
        list.push_back(Data("10"));

        list.pop_back();
        list.pop_back();
        list.pop_back();
        list.pop_back();
        list.pop_back();

        for (const Data& data : list) { data.print(); }

        BAL_LOG_INFO("==== list::pop_front ====\n");
        list.clear();

        list.push_back(Data("1"));
        list.push_back(Data("2"));
        list.push_back(Data("3"));
        list.push_back(Data("4"));
        list.push_back(Data("5"));
        list.push_back(Data("6"));
        list.push_back(Data("7"));
        list.push_back(Data("8"));
        list.push_back(Data("9"));
        list.push_back(Data("10"));

        list.pop_front();
        list.pop_front();
        list.pop_front();
        list.pop_front();
        list.pop_front();

        for (const Data& data : list) { data.print(); }

        BAL_LOG_INFO("==== list::access ====\n");
        list.clear();

        list.push_back(Data("1"));
        list.push_back(Data("2"));
        list.push_back(Data("3"));
        list.push_back(Data("4"));
        list.push_back(Data("5"));
        list.push_back(Data("6"));
        list.push_back(Data("7"));
        list.push_back(Data("8"));
        list.push_back(Data("9"));
        list.push_back(Data("10"));

        list.front().print();
        list.back().print();

    }
}

// ----------------------------------------------------------------------------

}
