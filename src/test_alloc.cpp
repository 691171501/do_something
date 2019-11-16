//
// Created by jingzheng on 6/15/19.
//

#include <cstdio>
#include "local.h"
#include <iostream>
//#include "alloc.h"
using namespace std;


void test_or_align(){
    int a =(5 + 8 - 1) & ~(8 - 1);
    int b =(9 + 8 - 1) & ~(8 - 1);
    int c =(13 + 8 - 1) & ~(8 - 1);
    int d =(17 + 8 - 1) & ~(8 - 1);
    cout<<a<<endl;  // 8  （5 -》 8）
    cout<<b<<endl;  // 16  （9 -》 16）
    cout<<c<<endl;  // 16   （13 -》 16）
    cout<<d<<endl;  // 24    （17 -》 24）
}


void test_freelist_index(){
    size_t bytes = 5;
    size_t idx = (((bytes) + 8 - 1) / 8 - 1);
    std::cout<<idx<<endl;  //0

    bytes = 13;
    idx = (((bytes) + 8- 1) / 8- 1);
    std::cout<<idx<<endl; // 1

}

void test_N(int N){

    cout<<"test_N: " << ((N) & (~N+1)) <<endl;

}
