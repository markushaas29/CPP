#include <iostream>
#include <vector>

struct S1
{
    long l1;
    char c1;
    long l2;
    char c2;
};

struct S2
{
    char c1;
    char c2;
    long l1;
    long l2;
};


int main()
{
    S1 s1;
    S2 s2;
    
    std::cout<<"S1: "<<sizeof(s1)<<std::endl;
    std::cout<<"S2: "<<sizeof(s2)<<std::endl;
    
    return 0;
}

