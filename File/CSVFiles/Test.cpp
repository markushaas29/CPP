#include <iostream>
#include <vector>

class X
{
private:
    std::vector<char> s = {'T','e','s','t','\0'};
public:
    const char operator [](int i){ std::cout<<"const char"<<std::endl; return s[i]; } const
    char& operator [](int i){ std::cout<<"char& "<<std::endl; return s[i]; }
    //const std::string get(){ std::cout<<"const std::string"<<std::endl; return s; }
//     std::string& get(){ std::cout<<"std::string"<<std::endl; return s; }
};


int main()
{
    std::cout<<"Test"<<std::endl;
    
    X x;
    
    char s = x[1];
    std::cout<<"s :"<<s<<std::endl;
    
    x[0] = "A";
    std::cout<<"s :"<<x[0]<<std::endl;
    
    return 0;
}

