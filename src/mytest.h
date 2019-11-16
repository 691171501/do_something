//
// Created by jingzheng on 6/14/19.
//

#ifndef XCW_MYTEST_H
#define XCW_MYTEST_H

#include <stdio.h>

#include <iostream>
using namespace std;


class mytest {
public:
    mytest()
    {
        printf("start enter mytest construct\n");
    }


    ~mytest()
    {
        printf("leave mytest\n");
    }


};


#endif //XCW_MYTEST_H
