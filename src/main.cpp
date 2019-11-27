#include <iostream>
#include <unistd.h>
#include <iostream>
#include "mytest.h"
#include "local.h"
#include <fstream>
#include <time.h>
#include <string.h>
#include <sys/time.h>

class TimeCost {
public:
    TimeCost(const std::string& prefix) : prefix_(prefix){
        gettimeofday(&start_, NULL);
    }

    ~TimeCost() {

        struct timeval end;
        gettimeofday(&end, NULL);
        std::cout << prefix_ <<" cost time:" << (end.tv_sec - start_.tv_sec) + double(end.tv_usec - start_.tv_usec) / 1000000 << std::endl;
    }

private:
    struct timeval  start_;
    std::string prefix_;
};



void testprint()
{

    for (int i=0; i <100; i++)
        printf("%d", i);
    fflush(stdout);
    usleep(10000000);

}


void test() {
    mytest  test;
    std::cout << "Hello, World!" << std::endl;
//    testprint();
    //test_or_align();
    //test_freelist_index();
    test_N(2);
}


char d[1024];
char buff[1024 * 10000]={0};
void test_fwrite(const std::string& file, int n  = 10000, int type = 0) {
    memset(d, 0x01, 1024);
    std::cout << "N :" << n
              << ", File :" << file <<  std::endl;
    FILE * ls = fopen(file.c_str(), "ab");
    if (type ==0) {
        for (int i = 0; i < n; i++) {
            fwrite(d, 1024, 1, ls);
        }
    } else {
        for (int i = 0; i < n; i++) {
            memcpy(buff + i * 1024, d, 1024);
        }

        fwrite(buff, sizeof(buff), 1, ls);

    }

    fclose(ls);
}

//xy_stl::initialized_fill_n(1, 8, int)

int main() {

    int n = 1000;
    test_fwrite("1&N.txt",n,0);
    test_fwrite("N&1.txt",n,1);
    std::cout << "********5******" <<std::endl;

//
//
//    n = 10000;
//    test_fwrite("1&N.txt",n,0);
//    test_fwrite("N&1.txt",n,1);
//    std::cout << "**************" <<std::endl;
//
//
//    n = 100000;
//    test_fwrite("1&N.txt",n,0);
//    test_fwrite("N&1.txt",n,1);
//    std::cout << "**************" <<std::endl;

    n = 10000;
    test_fwrite("1&N.txt",n,0);
//    test_fwrite("N&1.txt",n,1);
    std::cout << "**************" <<std::endl;

    return 0;
}